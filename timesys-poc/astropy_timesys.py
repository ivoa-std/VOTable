# Sample code illustrating that TIMESYS works. this is proof-of-concept
# level code for the RFC for VOTable 1.4.  The API in astropy should
# certainly be completely different (presumably adding the annotation
# directly to column metadata while it's being parsed).
#
# If you're an astronomer looking to process time series, you probably
# shouldn't be looking at this.
#
# See also http://docs.astropy.org/en/stable/time/
#
# This code is provided under a CC-0 licence.

import datetime
import io
import numpy

from astropy.io import votable
from astropy import coordinates
from astropy import table
from astropy import units
from astropy.time import Time

MAGIC_TIMEORIGINS = {
  "MJD-origin": 2400000.5,
  "JD-origin": 0,
  }

TIMESYS_SCALES_TO_ASTROPY = {
    'TAI': 'tai',
    'TT': 'tt',
    'UT': 'ut1',
    'UTC': 'utc',
#    'GPS': is handled separately, needs to be researched
    'TCG': 'tcg',
    'TCB': 'tcb',
    'TDB': 'tdb',
}

################### astropy technicalities

def monkeypatch_timesys_into_astropy():
    """adds provisional timesys support to astropy's VOTable parser.
    """
    from astropy.io.votable.tree import Resource

    aut_orig = Resource._add_unknown_tag
    def add_unknown_tag_with_timesys(self, iterator, tag, data, config, pos):
        if self.timesystems is None:
            self.timesystems = {}
        if tag=='TIMESYS':
            if "timeorigin" in data:
                data["timeorigin"] = float(
                    MAGIC_TIMEORIGINS.get(
                        data["timeorigin"], data["timeorigin"])
                    )*units.d
            self.timesystems[data["ID"]] = data
        else:
            return aut_orig(self, iterator, tag, data, config, pos)
            
    Resource._add_unknown_tag = add_unknown_tag_with_timesys
    Resource.timesystems = None

monkeypatch_timesys_into_astropy()

def change_time_scale(time_coo, time_scale):
    """returns time_coo in time_scale (which may be time_coo itself and 
    change it)

    (this works around an astropy version dependency)
    """
    try:
        time_coo.scale = time_scale
        return time_coo
    except AttributeError:
        return getattr(time_coo, time_scale)


#################### loading tables, finding columns, transforming times

def locate_time_column(table, timesystems):
    """returns a numpy array of times in the VOTable table together with
    its TIMESYS metadata.

    The time is the first column referencing something in timesystems.
    """
    for col_index, field in enumerate(table.fields):
        if field.ref in timesystems:
            break
    else:
        raise Exception("No column referencing a TIMESYS found")

    return table.to_table().columns[col_index], timesystems[field.ref]


def compute_travel_time_to_barycenter(ref_time, refpos, obs_location):
    """returns timedeltas for the light travel time from ref_time to
    barycenter.
    """
    geocenter = coordinates.EarthLocation(
        x=0*units.m, y=0*units.m, z=0*units.m)
    if refpos=="BARYCENTER":
        return 0*units.s
    elif refpos=="GEOCENTER":
        return ref_time.light_travel_time(obs_location,
            location=geocenter)
    elif refpos=="HELIOCENTER":
        # See below on adding terms here (spoiler: it's wrong).
        return (ref_time.light_travel_time(
                obs_location, kind='heliocentric', location=geocenter)
            -ref_time.light_travel_time(obs_location, location=geocenter))
    elif refpos=="EMBARYCENTER":
        raise Exception("EMBARYCENTER is part of a transformation."
            " We *could* do a light travel time correction for"
            " that, but nobody has bothered to write to code.")
    elif refpos in ["UNKNOWN", "TOPOCENTER"]:
        raise Exception("Cannot compute light travel time corrections"
            " for {}".format(refpos))


def compute_light_travel_time(ref_time, obs_location, from_refpos, to_refpos):
    """returns a timedelta for the time lights needs from from_refpos
    to to_refpos when coming from obs_location.

    from_refpos and to_refpos come from the IVOA refposition vocabulary.
    """
    # TODO: This is pretty certainly wrong -- you can't really add
    # these times because they're not along collinear paths.  But I'm too lazy
    # to figure out the ephemeris computations properly.  With them, things
    # would be simple: Just compute the distance between to_refpos and
    # from_refpos and divide by c.
    return compute_travel_time_to_barycenter(
            ref_time, to_refpos, obs_location
        )-compute_travel_time_to_barycenter(
            ref_time, from_refpos, obs_location)


def to_tcb_barycenter(times, times_meta, obs_location):
    """tries to convert the column times described by the timesys attributes
    times_meta to TCB barycenter.

    The location of the observation, obs_location, can be None if
    the reference position is already the barycenter.

    This is where we use TIMESYS/@refposition.
    """
    times = change_time_scale(times, "tcb")
    return times+compute_light_travel_time(
        times, 
        obs_location,
        times_meta["refposition"], 
        "BARYCENTER")


def to_jd(times, times_meta):
    """returns Time instances given in JD.

    This is where we use FIELD metadata, TIMESYS/@timeorigin and 
    TIMESYS/@timescale
    """
    try:
        astropy_scale =TIMESYS_SCALES_TO_ASTROPY[times_meta["timescale"]]
    except KeyError:
        raise Exception("Unsupported timescale: %s"%times_meta["timescale"])

    if "timeorigin" not in times_meta:
        # no timeorigin given: it's some sort of civic year
        if times.dtype=='O': 
            # These are strings and hence presumably timestamps
            # (we should rather inspect xtype here)
            # This should really work like this:
            # times = Time(times, format="isot", scale=astropy_scale)
            # but because of breakage in Debian stretch we do it half-manually:
            times = Time(
                [datetime.datetime.strptime(
                        v.split(b".")[0].decode("ascii"), "%Y-%m-%dT%H:%M:%S")
                    for v in times],
                scale=astropy_scale)
            times.format = 'jd'

        else:
            # in VOTable, these must be julian or besselian years
            if times.unit not in ["yr", "byr"]:
                raise Exception("Floats without timeorigin only allowed when"
                    " unit is year.")
            times = Time(times, 
                format={"yr": "jyear", "byr": "byear"}[str(times.unit)],
                scale=astropy_scale)
            times.format = 'jd'

    else:
        times = Time(
            times.to(units.d)+times_meta["timeorigin"],
            format="jd",
            scale=astropy_scale)
    
    return times


#################### bookkepping and such

def load_timeseries(in_file, value_name, obs_location):
    """returns a time series from in_file in JD for TCB BARYCENTER.

    in_file must give a VOTable literal.

    value_name names the column to take the value from.

    obs_location is a SkyCoordinate giving the (ICRS) position of
    the object observed.

    The time column will be the first FIELD that references TIMESYS,

    [the label column is just for debugging)
    """
    vot = votable.parse(in_file)
    timesystems = vot.resources[0].timesystems
    time_series = vot.resources[0].tables[0]
    
    times, times_meta = locate_time_column(time_series, timesystems)
    times = to_jd(times, times_meta)
    times = to_tcb_barycenter(times, times_meta, obs_location)

    apy_table = time_series.to_table()
    values = apy_table.columns[value_name]
    # need to use times.value here since astropy vstack can't deal with
    # mixin columns.
    return table.Table(data=[
            times.value, 
            apy_table.columns[value_name],
            apy_table.columns["label"]],
        names=("time", "value", "label"))


def main():
    series = []
    for literal in TIME_SERIES_LITERALS:
        series.append(load_timeseries(
            io.BytesIO(literal),
            value_name="flux",
            obs_location=OBS_LOCATION))

    result = table.vstack(series)

    # Now replace the JD with seconds relative to start of series
    # Note: this is haemorraghing bits, so don't do it like in production
    # (rather, determine the minimum while it's still times)
    start_of_series = min(result.columns["time"])
    print("Start of series: {}".format(start_of_series))
    result.replace_column("time", 
        (result.columns[0]-start_of_series)*3600*24)
    with open("joined.vot", "w") as f:
        result.write(f, format="votable")


#################### only embedded data south of here

OBS_LOCATION = coordinates.SkyCoord(
	34.83663376*units.deg, -2.97763767*units.deg)


# the following stuff is generated by the accompanying makeseries.py
TIME_SERIES_LITERALS = [
b"""<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" refposition="BARYCENTER" timeorigin="0" timescale="TCB"/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" datatype="double" unit="d"
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA><TR><TD>2450000.00032</TD><TD>800</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>800</TD><TD>A</TD></TR>
<TR><TD>2450000.00064</TD><TD>800</TD><TD>A</TD></TR>
<TR><TD>2450000.0008</TD><TD>800</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>784</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>768</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>752</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>736</TD><TD>A</TD></TR>
<TR><TD>2450000.00048</TD><TD>720</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>800</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>784</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>768</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>752</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>736</TD><TD>A</TD></TR>
<TR><TD>2450000.00112</TD><TD>720</TD><TD>A</TD></TR>
<TR><TD>2450000.00144</TD><TD>720</TD><TD>A</TD></TR>
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
""",
b"""<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" refposition="BARYCENTER" timeorigin="MJD-origin" timescale="TCB"/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" datatype="double" unit="yr"
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA><TR><TD>136.891174374</TD><TD>736</TD><TD>B</TD></TR>
<TR><TD>136.891174374</TD><TD>752</TD><TD>B</TD></TR>
<TR><TD>136.891174374</TD><TD>768</TD><TD>B</TD></TR>
<TR><TD>136.891174374</TD><TD>784</TD><TD>B</TD></TR>
<TR><TD>136.891174374</TD><TD>800</TD><TD>B</TD></TR>
<TR><TD>136.891174812</TD><TD>784</TD><TD>B</TD></TR>
<TR><TD>136.89117525</TD><TD>768</TD><TD>B</TD></TR>
<TR><TD>136.891175688</TD><TD>784</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>800</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>784</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>768</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>752</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>736</TD><TD>B</TD></TR>
<TR><TD>136.891176126</TD><TD>720</TD><TD>B</TD></TR>
<TR><TD>136.891177002</TD><TD>800</TD><TD>B</TD></TR>
<TR><TD>136.891177002</TD><TD>784</TD><TD>B</TD></TR>
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
""",
b"""<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" refposition="GEOCENTER" timeorigin="2440587.5" timescale="TT"/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" datatype="double" unit="s"
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA><TR><TD>813240660.149</TD><TD>768</TD><TD>C</TD></TR>
<TR><TD>813240660.149</TD><TD>752</TD><TD>C</TD></TR>
<TR><TD>813240660.149</TD><TD>736</TD><TD>C</TD></TR>
<TR><TD>813240660.149</TD><TD>720</TD><TD>C</TD></TR>
<TR><TD>813240673.973</TD><TD>800</TD><TD>C</TD></TR>
<TR><TD>813240687.798</TD><TD>800</TD><TD>C</TD></TR>
<TR><TD>813240701.622</TD><TD>800</TD><TD>C</TD></TR>
<TR><TD>813240729.271</TD><TD>784</TD><TD>C</TD></TR>
<TR><TD>813240729.271</TD><TD>768</TD><TD>C</TD></TR>
<TR><TD>813240743.095</TD><TD>752</TD><TD>C</TD></TR>
<TR><TD>813240756.919</TD><TD>752</TD><TD>C</TD></TR>
<TR><TD>813240743.095</TD><TD>800</TD><TD>C</TD></TR>
<TR><TD>813240756.919</TD><TD>800</TD><TD>C</TD></TR>
<TR><TD>813240770.744</TD><TD>784</TD><TD>C</TD></TR>
<TR><TD>813240770.744</TD><TD>736</TD><TD>C</TD></TR>
<TR><TD>813240494.257</TD><TD>704</TD><TD>C</TD></TR>
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
""",
b"""<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" refposition="HELIOCENTER" timescale="UTC"/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" datatype="char" arraysize="*" xtype="timestamp"
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA><TR><TD>1995-10-09T12:00:25.746</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:00:53.394</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:01:48.690</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:16.338</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:30.162</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:43.986</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:57.810</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:30.162</TD><TD>752</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:02:43.986</TD><TD>752</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:04:06.930</TD><TD>720</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:03:53.106</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:03:39.282</TD><TD>704</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:03:25.458</TD><TD>720</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:04:34.578</TD><TD>800</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:04:48.402</TD><TD>784</TD><TD>D</TD></TR>
<TR><TD>1995-10-09T12:05:02.226</TD><TD>768</TD><TD>D</TD></TR>
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
""",
b"""<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" refposition="GEOCENTER" timescale="TCG"/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" datatype="double" unit="yr"
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA><TR><TD>1995.7700476</TD><TD>800</TD><TD>E</TD></TR>
<TR><TD>1995.77004716</TD><TD>784</TD><TD>E</TD></TR>
<TR><TD>1995.77004672</TD><TD>752</TD><TD>E</TD></TR>
<TR><TD>1995.77004672</TD><TD>736</TD><TD>E</TD></TR>
<TR><TD>1995.77004672</TD><TD>720</TD><TD>E</TD></TR>
<TR><TD>1995.77004672</TD><TD>704</TD><TD>E</TD></TR>
<TR><TD>1995.77004847</TD><TD>784</TD><TD>E</TD></TR>
<TR><TD>1995.77004847</TD><TD>768</TD><TD>E</TD></TR>
<TR><TD>1995.77004891</TD><TD>800</TD><TD>E</TD></TR>
<TR><TD>1995.77004935</TD><TD>800</TD><TD>E</TD></TR>
<TR><TD>1995.77004979</TD><TD>784</TD><TD>E</TD></TR>
<TR><TD>1995.77004891</TD><TD>752</TD><TD>E</TD></TR>
<TR><TD>1995.77004935</TD><TD>752</TD><TD>E</TD></TR>
<TR><TD>1995.77004979</TD><TD>736</TD><TD>E</TD></TR>
<TR><TD>1995.77004979</TD><TD>720</TD><TD>E</TD></TR>
<TR><TD>1995.77004935</TD><TD>704</TD><TD>E</TD></TR>
<TR><TD>1995.77004891</TD><TD>704</TD><TD>E</TD></TR>
<TR><TD>1995.77004847</TD><TD>720</TD><TD>E</TD></TR>
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
"""
]


if __name__=="__main__":
    main()

# vi:et:sta:sw=4

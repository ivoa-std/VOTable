# make time series examples for astropy-timesys (and the points are 
# meant to give a funny result when you join all them).
#
# Here's the plan: We have some artificial data which we assume to be
# JDs for barycentric TDB.  The object observed is assumed to be Mira.
#
# We partition this into n random subsamples, each of which is serialised in a
# different way; these different ways are encoded in time_makers, where the
# function (third item in the tuple) must of course match the metadata for
# TIMESYS (first item) and FIELD (second item).
#
# What's written is a python assignment expression for use in 
# astropy-timesys.py.
#
# This code is provided unter CC-0

import random

from astropy.time import Time
from astropy import coordinates
from astropy import units

SKY_LOCATION = coordinates.SkyCoord(
	34.83663376*units.deg, -2.97763767*units.deg)

def process_point(x, y):
	return 2450000+x*1e-5, y


allpoints = [process_point(*p) for p in [ 
(32,800), (48,800), (64,800), (80,800), (48,784), (48,768), (48,752), 
(48,736), (48,720), (112,800), (112,784), (112,768), (112,752), (112,736), 
(112,720), (144,720), (144,736), (144,752), (144,768), (144,784), (144,800), 
(160,784), (176,768), (192,784), (208,800), (208,784), (208,768), (208,752), 
(208,736), (208,720), (240,800), (240,784), (240,768), (240,752), (240,736), 
(240,720), (256,800), (272,800), (288,800), (320,784), (320,768), (336,752), 
(352,752), (336,800), (352,800), (368,784), (368,736), (48,704), (112,704), 
(144,704), (208,704), (240,704), (256,704), (272,704), (288,704), (256,752), 
(272,752), (368,720), (352,704), (336,704), (320,720), (400,800), (416,784), 
(432,768), (464,800), (448,784), (432,752), (432,736), (432,720), (432,704), 
(496,784), (496,768), (512,800), (528,800), (544,784), (512,752), (528,752), 
(544,736), (544,720), (528,704), (512,704), (496,720)]]


vot_template = """<VOTABLE xmlns="http://www.ivoa.net/xml/VOTable/v1.3"
		version="1.3">
   <RESOURCE>
    <TIMESYS ID="ts" {time_meta}/>
    <TABLE name="ndpohaahtsea">
      <FIELD ID="obs_time" name="obs_time" {field_meta}
      	ref="ts" ucd="time.epoch"/>
      <FIELD ID="flux" datatype="float" name="flux" 
      	ucd="phot.flux;em.opt.V" unit="1/s"/>
      <FIELD ID="label" datatype="char" name="label"/>
      <DATA>
        <TABLEDATA>{data}
        </TABLEDATA>
      </DATA>
    </TABLE>
  </RESOURCE>
</VOTABLE>
"""

def make_votable(time_meta, field_meta, transformer, data, label):
	data = "\n".join(
			"<TR><TD>{}</TD><TD>{}</TD><TD>{}</TD></TR>".format(
				transformer(t), v, label)
		for t, v in data)
	return vot_template.format(**locals())


random.shuffle(allpoints)

def jdn_to_unix_TT(t):
	t = Time(t, format='jd', scale="tcb",
		location=coordinates.EarthLocation(x=0*units.m, y=0*units.m, z=0*units.m))
	t = t+t.light_travel_time(SKY_LOCATION)
	# in newer astropy, use t.scale = 'tt'
	t = t.tt
	# format=unix sets scale=utc, so we compute things by hand.
	return (t.value-2440587.5)*24*3600


def jdn_to_timestamp_UTC(t):
	t = Time(t, format='jd', scale="tcb",
		location=coordinates.EarthLocation(x=0*units.m, y=0*units.m, z=0*units.m))
	# in newer astropy, use t.scale = 'tt'
	t = t.utc
	t = t-(t.light_travel_time(SKY_LOCATION)
			-t.light_travel_time(SKY_LOCATION, kind='heliocentric'))
	t.format = "isot"
	return t.value


def jdn_to_jyr_TCG(t):
	t = Time(t, format='jd', scale="tcb",
		location=coordinates.EarthLocation(x=0*units.m, y=0*units.m, z=0*units.m))
	# in newer astropy, use t.scale = 'tcg'
	t = t.tcg
	t = t+t.light_travel_time(SKY_LOCATION)
	t.format = "jyear"
	return t.value


time_makers = [
	('refposition="BARYCENTER" timeorigin="0" timescale="TCB"',
		'datatype="double" unit="d"',
		lambda t: t,
		'A'),
	('refposition="BARYCENTER" timeorigin="MJD-origin" timescale="TCB"',
		'datatype="double" unit="yr"',
		lambda t: (t-2400000.5)/365.25,
		'B'),
	('refposition="GEOCENTER" timeorigin="2440587.5" timescale="TT"',
		'datatype="double" unit="s"',
		jdn_to_unix_TT,
		'C'),
	('refposition="HELIOCENTER" timescale="UTC"',
		'datatype="char" arraysize="*" xtype="timestamp"',
		jdn_to_timestamp_UTC,
		'D'),
	('refposition="GEOCENTER" timescale="TCG"',
		'datatype="double" unit="yr"',
		jdn_to_jyr_TCG,
		'E'),
]
partition_points = [int(len(allpoints)/float(len(time_makers)))*i 
	for i in range(1,len(time_makers))]
slices = zip([0]+partition_points, 
	partition_points+[len(allpoints)+1])


literals = []
for (time_meta, field_meta, transformer, label), s in zip(time_makers, slices):
	literals.append(
		make_votable(
			time_meta, field_meta, transformer, allpoints[slice(*s)], label))
print("TIME_SERIES_LITERALS = [\n{}\n]".format(
	",\n".join('b"""{}"""'.format(l) for l in literals)))

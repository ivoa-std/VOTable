/*+++++++++++++++++++++
.IDENTIFICATION cgiprint.c
.LANGUAGE       C
.AUTHOR         Francois Ochsenbein [CDS]
.ENVIRONMENT    HTML, TeX
.KEYWORDS       LaTeX, HTML, HTTP, WWW
.VERSION  1.0   10-Oct-1994
.VERSION  1.1   07-Nov-1995: Options
		  -bib  (Recognize CDS BibCode   )
		  -cat  (Recognize CDS Catalogues)
		  -html (Recognize basic HTML tags)
		  -tex  (Recognize basic TeX  tags)
.VERSION  1.2   18-Nov-1995: <A#: for NAMEd anchor;
		\begin{env}, \end{env} recognized
.VERSION  1.3   18-Feb-1996: Allow embedded TeX.
.VERSION  1.4   18-Mar-1996: -f option
.VERSION  1.5   30-Apr-1996: Multiple parameters in TeX.
.VERSION  1.6   14-Jun-1996: Converted argument to absolute locations.
.VERSION  1.7   28-Sep-1996: Can have parameters in \begin{env parameters...}
.VERSION  1.8   08-Oct-1996: Corrected bug in case of long input lines...
				Undefined tex macros \macro{....} become
				<macro ....>
.VERSION  1.9   16-Oct-1996: Options -%..., and also \c{c} works...
.VERSION  2.0   06-Dec-1996: Fix bug: exec_cgi DOES NOT rescan its argument!
			Additions for GLU.
			Option -sec (secure, file MUST start with \cgidef{)
.VERSION  2.1   12-Dec-1996: Usage of $Arel for Relative Anchors
.VERSION  2.2   02-Jan-1997: Output pipe to a dedicated _of file handler
.VERSION  2.3   06-Feb-1997: Added \env, and $-substitutions
.VERSION  2.4   23-Apr-1997: Bug when parameters stacked, added push_buf
.VERSION  2.5   14-May-1997: Bugs in TABULAR context ---
.VERSION  2.6   16-May-1997: MODE_emptycell added, \emptycell
.VERSION  2.7   12-Jun-1997: Macro \plainbrowser, when non-3.0 Browser.
.VERSION  2.8   22-Jul-1997: List of Acceptable HTML 3.0
.VERSION  2.9   27-Aug-1997: Allow to understand telnet://user:pwd@node/
.VERSION  3.0   04-Sep-1997: \plain = Don't try to find anchors
				Added \typein
.VERSION  3.1   26-Sep-1997: \hbox{ ... } hides the calling environment ...
				(added clearMode)
.VERSION  3.2   04-Oct-1997: \ifnum \iffalse \iftrue \ifmmode \ifmatch \ifregexp
.VERSION  3.3   10-Nov-1997: Bug with empile_env corrected.
.VERSION  3.4   30-Nov-1997: Save environment parameters in dedicated stack.
				in order to allow definitions like
				\def\mymacro#1{\begin{env} #1 ... }
.VERSION  3.5   15-Jan-1998: Change definition of \A
.VERSION  3.6   20-Jan-1998: Added \ifhtx, don't change basic definitions !
.VERSION  3.7   23-Jan-1998: Added \Xcolor
.VERSION  3.8   24-Apr-1998: Added \bibobj  \objS
.VERSION  3.9   04-May-1998: Added -keep option. Take care of input Controls !
.VERSION  3.91  14-May-1998: Checked on TURBO-C.
.VERSION  4.0   25-May-1998: Added \exec (get result of a command)
				and \special{! ...} (VERBATIM command result)
.VERSION  4.1   11-Jun-1998: Added counters
.VERSION  4.2   26-Jun-1998: Added \tableofcontents
.VERSION  4.3   03-Oct-1998: Removed bug in \exec / \special
.VERSION  4.4   14-Oct-1998: -pop option + \JavaScript
.VERSION  4.5   05-Nov-1998: \ at end-of-line ignore spaces.
.VERSION  4.6   11-Nov-1998: Added save_dic   \savebuF{name}{value}\getbuF
.VERSION  4.7   05-Jan-1999: Added JSversion (=vMozilla)
.VERSION  4.8   18-Jan-1999: \xcolor works with hexadecimal
.VERSION  4.9   01-Feb-1999: Arguments are kept with the tex2 mode if necessary
.VERSION  5.0   22-Feb-1999: \sub{reg}{subs}{applied}
.VERSION  5.1   27-Feb-1999: rescan() allocates in a NEW PIECE of memory
.VERSION  5.2   09-May-1999: added \putenv
.VERSION  5.3   20-May-1999: accept 8-bit ascii
.VERSION  5.4   17-Jul-1999: can \input compressed files
.VERSION  5.5   20-Sep-1999; \sc flexible length
.VERSION  5.6   25-Sep-1999: Review the \lowercase \uppercase \ifxx
				Macros \GET1arg  \GETargop  \GETarglen
.VERSION  5.7   24-Jan-2000: Bug when closing braces missing --
				limit depth of braces to MAX_DEP
.VERSION  5.8   03-Feb-2000: Accept accept only on a set of letters,
.VERSION  5.9   07-Feb-2000: Corrected bug for "soft-mode"
.VERSION  6.0   27-Mar-2000: Corrected bug for empty cells (<BR> was missing)
.VERSION  6.1   08-Apr-2000: Allow \Beg{..}{HTML_opt}
.VERSION  6.2   25-Apr-2000: URL Writing as a MODE (introduced the \+ as the
				mark for #- in references)
.VERSION  6.3   02-May-2000: Better mark  intag
.VERSION  6.4   24-Jul-2000: \expand1arg
.VERSION  6.5   24-Jul-2000: {verbatim}  makes use of <PRE>
				rather than <LISTING>
.VERSION  6.6   26-Aug-2000: Keep the { } within <HTML>
.VERSION  6.7   04-Sep-2000: {JavaScript} environment
.VERSION  6.8   24-Sep-2000: Improvement of tabular conversions (be sure the &
				closes all opened \bf etc)
.VERSION  6.9   17-Oct-2000: Convert / into %2f
.VERSION  6.91  25-Oct-2000: Bug in maketitle corrected
.VERSION  6.92  27-Oct-2000: \protect
.VERSION  6.93  17-Nov-2000: Accept terminate bibcode by \
.VERSION  6.94  02-Dec-2000: Don't force the ^ in regexp
.VERSION  6.95  18-Dec-2000: # is output # unless within \htarg
.VERSION  6.96  28-Dec-2000: Cosmetics
.VERSION  7.0   19-Jan-2001: The & were rejected from http !!!
.VERSION  7.1   07-Mar-2001: \elsif..
.VERSION  7.11  31-Mar-2001: Never store EOF in collector buffer
.VERSION  7.12  05-Apr-2001: Corrected bug in regexp (substitute regexp)
.VERSION  7.13  09-Apr-2001: Replace \+ (SET1mode{#}) by \Escape
				and use isGraph (8-bit chars)
.VERSION  7.2   01-May-2001: Accept \begin{Env HTML_parameter} when Env is known
.VERSION  7.21  03-May-2001: Accept \begin{Env HTML_parameter} CORRECT
.VERSION  7.22  07-May-2001: PutChar algorithm revisited
.VERSION  7.23  12-May-2001: \tagobject \tagobjS \winHead
.VERSION  7.3   29-May-2001: target option for -b-b -ref
.VERSION  7.31  11-Jun-2001: \tag{...}  leave the % in the \tag
.VERSION  7.32  14-Jun-2001: Pbs with soft-tex fixed
.VERSION  7.33  30-Jun-2001: GLUTAG: requires alphabetic
		(syndrome: table <&16.18&> was taken as GLU tag)
.VERSION  7.34  10-Jul-2001: Accept bibcode followed by :
.VERSION  7.35  27-Jul-2001: {sqrt}: soft option, so not texize when there
.VERSION  7.36  02-Aug-2001: bug in regexp (action of ^) fixed.
.VERSION  7.40  03-Aug-2001: Corrected the \cgidef (stack)
.VERSION  7.41  18-Sep-2001: Recognize also astro-ph !
.VERSION  7.42  20-Sep-2001: \tagobjA \tagobjac etc...
.VERSION  7.43  26-Sep-2001: Accept "http:..." (whithin quotes)
.VERSION  7.44  29-Sep-2001: Bug in match_tex while in \soft
				(syndrome: doesnùt work properly in tables)
.VERSION  7.45  04-Nov-2001: Problem with \begin{row}, added itable in TEX_ENV
.VERSION  7.46  17-Nov-2001: again some problems in TeX2 mode fixed...
.VERSION  7.47  07-Dec-2001: Take care of comments in <!-- -->
.VERSION  7.48  27-Dec-2001: But with generated macro def_macro
.VERSION  7.49  21-Jan-2002: Accept <!DOCUMENT...>
.VERSION  7.5   23-Jan-2002: Accept -+tex (remove tex option)... (minus_plus)
.VERSION  7.6   12-Mar-2002: \exec is expanded, added \specialexpanded
.VERSION  7.7   25-Mar-2002: Added the section counter, \appendix
.VERSION  7.8   07-May-2002: Corrected 2 bugs about optional argument
		(i.e. accept \begin{figure}[hp], not \begin{figure} [hp])
		and B/C is not a catalog!!
		Also, don't accept optional arguments in env. marked by \21\6
.VERSION  7.81  12-May-2002: Problem GCPD was not more accessible...
.VERSION  7.82  11-Jul-2002: Problem with <FONT> which were considered as output
		characters -- inserted the \17i (inTag) mode in this case.
.VERSION  7.83  31-Aug-2002: 2 problems fixed: appendix in ToC, out1c
.VERSION  7.84  28-Sep-2002: out1d fixed
.VERSION  7.85  07-Oct-2002: \Alay{layer}{http}{text}, and color as \laycolor
.VERSION  7.86  07-Nov-2002: The & is NOT accepted in match_cat !!
.VERSION  7.87  27-Nov-2002: \wHead \wPage -- valid also for windows
.VERSION  7.9   02-Dec-2002: \objectname[cat-id]{text}
.VERSION  7.91  09-Dec-2002: \listofobjects revisited.
.VERSION  7.92  19-Dec-2002: Added an alarm
.VERSION  7.93  05-Jan-2003: Bug when definition with blank \m #2 { #1 #2}
.VERSION  7.94  13-Jan-2003: Use the length begin{tabular}{lp{0.25}} ..
				...&\multicolumn{1}{r}{...}  use 25% !!
.VERSION  7.95  25-Jan-2003: Accept a \tooLongText macro, mailed in case
.VERSION  7.96  21-Feb-2003: Cosmetic in \\listofobjects
.VERSION  7.97  27-Feb-2003: -f2 option = File to Check in case of Alarm
				of problem.
.VERSION  7.98  07-Mar-2003: In \Alay, the quote must be written \&apos;
.VERSION  7.99  25-Mar-2003: \resetAlarm
.VERSION  8.0   22-Apr-2003: \plain{...} also removes SOFT MATH
.VERSION  8.01  21-May-2003: match_if revisited.
				Added Wlay
.VERSION  8.02  26-Jun-2003: Problems TeXsoft revisited.
.VERSION  8.03  28-Jun-2003: Email Addresses Crypted.
.VERSION  8.04  25-Aug-2003: Use window.HISTORY.back !!!
.VERSION  8.05  07-Sep-2003: \image{src} \img{src}{alt} \IMG{src}{alt}{options}
.VERSION  8.06  18-sep-2003: \taboptions \rowoptions \imgoptions
.VERSION  8.1   18-Sep-2003: \bg works in table cells
.VERSION  8.11  24-Sep-2003: accept catalog terminated by a /;
.VERSION  8.12  26-Sep-2003: #-comment ==> as <!--comment......-->
.VERSION  8.13  03-Oct-2003: \cr emulates \\ (use \n or \nl to emulate lineskip)
.VERSION  8.14  12-Nov-2003: Problem with ipenv / itexenv in push_tex / pop_tex
			ipenv = value at BEGINNING of push_tex
.VERSION  8.15  28-Nov-2003: Problem with glu -- changed objS etc
.VERSION  8.16  03-Dec-2003: Accept e-mail as who@where?Subject=
.VERSION  8.2   18-Jan-2004: Deal properly with Javascript -- \select
.VERSION  8.21  24-Jan-2004: vMozilla>=5 > problem & # + : double escape
.VERSION  8.22  26-Jan-2004: Corrected bug introduced by v8.2 !
.VERSION  8.23  28-Jan-2004: Changed \\input
.VERSION  8.24  17-Feb-2004: Accept cgidef{tex} (not -tex)
.VERSION  8.25  19-Mar-2004: Changes in \listofobjects, usage of SESAME_r env.
.VERSION  8.26  23-Mar-2004: \fghead (defines the color of chapters, etc)
.VERSION  8.27  30-Mar-2004: Another bug in rescan_arg...
.VERSION  8.3   23-Apr-2004: Use <UL> for Contents.
.VERSION  8.31  28-Apr-2004: Encode email addresses
.VERSION  8.32  05-May-2004: \echo1x (echo one time only)
.VERSION  8.33  12-May-2004: \tableofcontents*{} (with macrostar)
.VERSION  8.34  15-May-2004: error message when \fi within { ... }
.VERSION  8.35  03-Jun-2004: bug in table of contents
.VERSION  8.36  28-Jun-2004: bug in isanif
.VERSION  8.37  18-Jul-2004: limit number of errors
.VERSION  8.38  24-Jul-2004: Accept \long\def{ (blank lines possible) }
.VERSION  8.39  11-Nov-2004: Problem in &amp;
.VERSION  8.4   17-Nov-2004: Problem in \SET1mode (syndrome: never quit Soft)
.VERSION  8.41  30-Dec-2004: Problem \htarg{\&}
.VERSION  8.5   01-Jan-2005: Multilingual (multilang) -- echo4 multilingual
.VERSION  8.51  18-Feb-2005: Minor bug in match_mail
.VERSION  8.52  20-Mar-2005: \listofobjects, be sure in nomath mode
.VERSION  8.53  30-Mar-2005: -gluw -ogluw option
.VERSION  8.54  16-Apr-2005: \+ in a cgi parameter --> + (not %2b)
.VERSION  8.55  29-Jun-2005: Bug in end_env
.VERSION  8.56  30-Jun-2005: Bug in tex2 mode
.VERSION  8.57  12-Aug-2005: Regular expression defining catalog modified
				to avoid a match with J/H/K
.VERSION  8.6   12-Sep-2005: Accept \begin{ ... } on several lines
.VERSION  8.61  19-Oct-2005: \fi badly placed --> limit number of errors
.VERSION  8.62  16-Dec-2005: Special regexp for B catalogues
.VERSION  8.63  20-Dec-2005: match_if problem with \%
.VERSION  8.64  15-Feb-2006: \theXXX  is blank by default
.VERSION  8.65  08-Apr-2006: accept astro.ph
.VERSION  8.66  02-Jul-2006: problem in begin_env
.VERSION  8.67  17-Jul-2006: problem in tex_def
.VERSION  8.68  14-Dec-2006: \objac
.VERSION  8.70  15-Mar-2007: addition of a few unicode symbols
.VERSION  8.71  04-May-2007: Use %16 (instead of %26) for the & in http argument
.VERSION  8.72  12-Jun-2007: Problem J/other/...
.VERSION  8.73  29-Aug-2007: vMozilla + vhtml changed in cgiver + print_services
.VERSION  8.74  04-Sep-2007: Substitution requires # + num
.VERSION  8.75  13-Sep-2007: Problem Show-s solved. arXiv
.VERSION  8.8   03-Dec-2007: Option -D (definition)
.VERSION  8.81  26-Jan-2008: \lay{etiquette}{text}
.VERSION  8.82  01-Feb-2008: Addition \atom{atomic_number|Z}
.VERSION  8.83  08-Apr-2008: \fi  does not gbble all spaces any more
.VERSION  8.84  14-May-2008: accept \Beg{TABULAR}{#1}{#2}
.VERSION  8.85  19-May-2008: add TABLESORT
.VERSION  8.86  15-Sep-2008: \textwidth = 100%
.VERSION  8.87  18-Sep-2008: pb length<1
.VERSION  8.88  19-Sep-2008: some more symbols (mdash etc)
.VERSION  8.89  22-Sep-2008: In case of alarm: dump conditions
.VERSION  8.9   27-Nov-2008: Other error message in push_tab
.VERSION  8.91  13-Dec-2008: \CDShead -> CDStail or CDStail1
.VERSION  9.0   18-Dec-2008: Accept Unicode
.VERSION  9.01  02-Jan-2009: pb \ionZ{NIII} interpreted as \ionZ{NiII} !!
.VERSION  9.02  07-Feb-2009: Accept Ion [forbidden line]
.VERSION  9.03  03-Apr-2009: bug fixed in [forbidden line]
.VERSION  9.04  03-Aug-2009: popen uses a sub-shell
.VERSION  9.05  29-Aug-2009: hiddensoft in output
.VERSION  9.06  20-Sep-2009: \undefined \ifx (\ifdef \ifundef)
.VERSION  9.07  09-Oct-2009; \yA \yW (anchor with title) \yA{why}{href}{text}
			     \yW{why}{name}{href}{text}
.VERSION  9.08  27-Oct-2009: IMG has alt + title
.VERSION  9.09  17-Dec-2009: Action of \ and end-of-line: gobble blanks.
.VERSION  9.10  07-Apr-2010: Edition of dates with zeroes
.VERSION  9.11  12-Apr-2010: memory bug corected in compile.h.
.VERSION  9.12  25-Apr-2010: pr TABULAR in soft-tex
.VERSION  9.13  30-Apr-2010: added \Column{html_opt}{text}
.VERSION  9.14  01-Sep-2010: pop_tex messages changed
.VERSION  9.15  17-Nov-2010: ..
.VERSION  9.16  26-Nov-2010: \overtext{small text}{full text}
.VERSION  9.17  11-Dec-2010: regexp of catalogs
.VERSION  9.18  02-Feb-2011: Pb ToC with more than 9 sub-items...
.VERSION  9.19  16-Mar-2011: -js option was not properly taken into account!
.VERSION  9.20  25-Mar-2011: Pb with \glutag and \htarg
.VERSION  9.21  07-Apr-2011: Add strike = sout
.VERSION  9.22  17-Apr-2011: Add a few geometrical Unicodes (square, triangle..)
.VERSION  9.23  21-Jul-2011: Accept in soft-mode texts like "{prop.to}"
				(converted into "{propto}")
.VERSION  9.24  12-Aug-2011: Bug in e.g. {\bf*} wad not correctly interpreted
.VERSION  9.25  08-Oct-2011: Bug in PutChar in case of javascript
.VERSION  9.26  01-Dec-2011: Addition \objP (object with Prefix}
.VERSION  9.3   13-Dec-2011: Accept free-text ~ => +/- +- etc... and \not
.VERSION  9.31  20-Feb-2012: interpretation of minuses as \rule
.VERSION  9.32  27-Feb-2012: Added \objNED \objLEDA
			     Added macros Apop{size}{url}{text} and glupop=Wglu
.VERSION  9.4   28-Apr-2012: added \graph{num} [symbol GRAPH program]
.VERSION  9.41  07-Jun-2012: problem with < within \Alay (fixed with \<)
.VERSION  9.42  26-Jun-2012: Message when document not available
.VERSION  9.43  02-Jul-2012: Problem in match_if (keep newline after \fi)
.VERSION  9.44  12-Jul-2012: pb when used as a function
.VERSION  9.45  03-Aug-2012: error message in case reaching out of memory
.VERSION  9.46  16-Aug-2012: PutChar: Terminate if too many errors
.VERSION  9.47  30-Aug-2012: Priobmlem with Show-s
.COMMENTS       This program converts a text into HTML.
 -------- Note about -D option in compilation:
    The two variables   OGLU  and  RGB
    contain the path for
    - the  glufilter   program  (OGLU) and
    - the table of colours      (RGB)
    Defaults are provided, which can be changed at the compilation time.
 -------- Note: with #FUNCTION defined, knows the function  exec_cgi ------
	It has some knowledge of the HTML-significant <tags> (with -html),
	basic \TeX conventions like accents, \bf, \hline ... (with -tex),
	knowldge of embedded BibCodes =Bib (Bib) <Bib> [Bib] (with -bib),
	knowldge of embedded Catalogues written <Cat-name>   (with -cat),
	knowldge of e-mail addresses <User@Machine....>      (with -mail),
	and the basic "service" tag written as
	<Sname:arg text>        translated into an anchor
	                        <A HREF=_anchor_>text</A>
	The construction of _anchor_ from Sname:arg is done as follows:
	1) the symbolic name Sname is translated into its equivalent
	   from environment variable $URL_<Sname> , or if not existing
	   into its value from the -f file
   	2) the translation refers to the argument as $0, $i, $$i,
	   or $.i or $$.i for argument, subfield#i(separator /),
	   subfields #i until end (therefore $$1 is identical to $0);
	   the dot is used as a separator in the last two examples.
	The context (tex, etc) may be defined either as option, or in
	the first line is it is introduced by the 'magic word'
	#cgi-text [plain|html|tex|end] (options)...

	Default translations are defined as follows:
	<http:// ... >		<ftp:// .. >
	<A: Anchor text> 	<A#: Anchor_name>
	<:>		(Single <)
	<Abs: ... >	(Abstract)
	<Bib: ... >	(BibCode)
	<Cat: ... >	(Catalogue)
	<Dic: ... >	(Nomenclature, i.e. Dic)
	<Uni: ... >	(Nomenclature, i.e. Dic)
	<Fit: ... >	(File in FITS format)
	Lines starting by # (or % in -tex option) are converted to HTML comments
 Debugging Definitions:
    To simplify the debugging, use the following aliases in dbx:
       alias pc "print (char)c"
       alias pi "print &inbuf->text[inbuf->pos]"
       alias pp "print itexenv,(texenv-tex_stack),itexparm,itag_stack"
       alias pm "print texenv->mode, texenv->status"
       alias pt "print *texenv"
       alias pf "print &inbuf->prev->text[inbuf->prev->pos]"
       alias ptag "print itag_stack,tag_stack"
       alias ptab "print itab_stack,tab_stack,tab_taglev"
       alias pcol "print &collector->text[0]"

 Javascript required for \Alay: see ~cds/httpd/lib/viz-2.js

//------------------------- Javascript for layer ----------------------------
var nver = navigator.appVersion.substring(0,4);
var isNS = (navigator.appName.indexOf("Netscape") !=-1);
var laycolor = "#ffffe0";       // Default = LightYellow
var x,y;
function set_laycolor(color) { laycolor = color }
function lay(text){
  var t = "<table border='1' cellpadding='3' cellspacing='0' bgcolor='" +
       laycolor + "'>" + "<tr><td>";
  t = t + text.replace(/&apos;/g, "'");
  t = t + "</td></tr></table>";
  df = document.getElementById('xplain');
  df.innerHTML=t;
  df.style.background=laycolor; // "rgb(255,255,224)"
  df.style.left=x-50;
  df.style.top=y+10;
}
function laz(){ // Out of layer
  df = document.getElementById('xplain');
  df.innerHTML="";
}
function handlerMM(e){
    x = (isNS) ? e.pageX : event.clientX + document.body.scrollLeft;
    y = (isNS) ? e.pageY : event.clientY + document.body.scrollTop;
}
// Install the event handler
document.onmousemove = handlerMM;
document.write("<DIV ID=\"xplain\" style=\"position:absolute; visibility:show; left:25px; top:-50px; z-index:2\"><br></DIV>") ;
//------------------------- Javascript for layer ----(end)-------------------

---------------*/

#define VERSION	"9.47"
/* #define TRACE_MEMORY		--- very verbose tracing of memory usage ---- */
#ifndef SUBDIR		/* All documents are from a subdirectory... */
#define SUBDIR	"Pages"
#endif
#ifndef MAIL
#define MAIL "/usr/lib/sendmail"
#endif
/* #ifndef EMAIL
#define EMAIL "francois@astro.u-strasbg.fr"
#endif */

#ifndef _ARGS
#ifdef __STDC__
#define _ARGS(A)      A       /* ANSI */
#else
#define _ARGS(A)      ()      /* Traditional */
#define const
#endif
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef MAXERR_WRITE		/* V9.46: exit if write errors>MAXERR_WRITE */
#define MAXERR_WRITE 	100
#endif

#ifndef MAX_TIME		/* As a main program, max time */
#define MAX_TIME	900	/* Stop after 900s, but may continue */
#endif

#ifndef MAX_PIPES
#define MAX_PIPES 	16
#endif

#ifndef MAX_TIME		/* As a main program, max time */
#define MAX_TIME	900	/* Stop after 900s, but may continue */
#endif

#if DEBUG
#define Flush	fflush(stdout)
#else
#define Flush
#endif

#define LEN_TAG		511	/* Maximal size for a < ... > HTML */
#define LEN_TEX          64	/* Maximal length of TeX macro	   */
#define BUFINCR	       1024	/* MUST BE a POWER of 2	(increment)*/
#define MAX_DEP          16	/* Maximal depth for parameters	   */
#define MAX_LEP        8000	/* Maximal length of a parameter   */
#define MAX_NLP		100	/* Maximal number of lines in param*/
#define MAX_TAB		  7	/* Maximal depth of embedded tables*/
#define MAX_TAG		 64	/* Maximal depth of embedded tags  */
#define MAX_TEX	   (127-'0')	/* Maximal depth of embedded TeX   */
#define MAX_TSK		 16	/* Maximal depth of embedded cols  */
#define MAX_ENV	    MAX_TEX	/* Maximal number Environment parms*/
#define MAX_FCT		 64	/* Maximal number of Functions     */

#ifdef  TRACE_MEMORY
#define malloc  osmalloc
#define free    osmfree
#define realloc osmexpand
#define strdup	osmdup
#endif

#include <ctype.h>
#include <strings.h>	/* Standard string manipulations */
#include <string.h>	/* Standard string manipulations */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>	/* File Control		*/
#include <time.h>
#ifndef O_BINARY	/* ===================  Unix 	*/
#define  knowsFork	 1
#include <signal.h>	/* For fork popen etc.	*/
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#define O_BINARY 0
#ifndef RGB
#define RGB	"/usr/lib/X11/rgb.txt"
#endif
#ifndef OGLU
#define OGLU	"/usr/local/bin/glufilter"
#endif
#else			/* =================== MS-DOS	*/
#define  knowsFork	 0
#include <dir.h>	/* MS-DOS Definitions	*/
#include <sys\stat.h>	/* MS-DOS Definitions	*/
#define	 wait(aint)	*(aint)=0
#ifndef RGB
#define RGB	"/rgb.txt"
#endif
#ifndef OGLU
#define OGLU	""
#endif
#endif

double atof() ;		/* Never know where this is declared....   */
#ifndef strdup		/* Problem in Solaris, not declared...	   */
char *strdup();
#endif
FILE *fdopen();		/* Never know where this is declared....   */
FILE *popen();		/* Never know where this is declared....   */
int putenv();		/* Never know where this is declared....   */
#ifdef isupper
#ifndef isascii
#define isascii(c)	((c)&(~0xff))
#endif
#endif

#define MAX(x,y)        ((x) >= (y) ? (x) : (y))
#define MIN(x,y)        ((x) >= (y) ? (y) : (x))
#define ITEMS(a)        (int)(sizeof(a)/sizeof(a[0]))
#define isid(c)		(isalnum(c)||((c)=='_'))
#undef  putchar

/* Accept 8-bit charactesr */
#define isGraph(c)      (c && (!isspace(c)))

typedef int (*SORT_FCT)(/* const void *, const void * */) ;
typedef char *PTR;
typedef PTR (*PTR_FCT)() ;	/* Function returning char pointer */
typedef int (*INT_FCT)() ;	/* Function returning integer      */

#define BibCodeExp		/* BibCode Regular Expression      */ "\
[12][890][0-9][0-9]\
[A-Za-z][A-Za-z0-9&+.]\\{4\\}\
[0-9CTBbokph.]\\{4\\}\
[A-Za-z.012][0-9.]\\{4\\}[A-Z:.]"
/* Old Expression (no ADS Extensions)
[12][890][0-9][0-9]\
[A-Z][A-Za-z0-9&+.]\\{4\\}\
[0-9CTB.]\\{4\\}\
[A-Za-z.][0-9.]\\{4\\}[A-Z:.]"
*/
static char *arXivExp[] = {	/* V8.75 -- #0 = BibCode */
    BibCodeExp,
    "astro-*ph[/:][0-9][0-9][01][0-9]\\{4\\}",	/* astrophExp */
    "arXiv[/:][0-9][0-9][01][0-9]\\{4\\}",	/* astrophExp */
    "astro-*ph[/:][0-9]\\{4\\}[.][0-9]\\{4\\}",	/* arXiva_Exp */
    "arXiv[/:][0-9]\\{4\\}[.][0-9]\\{4\\}"	/* arXiv__Exp */
};
static char *arXivGlu[] = {	/* GLU mark */
    "%R",			/* Bibcode */
    "&astro-ph.num" ,		/* Old astro-ph "&arXiv astro-ph/" */
    "&astro-ph.num" ,		/* Old astro-ph "&arXiv astro-ph/" */
    "&arXiv",			/* New arXiv */
    "&arXiv"			/* New arXiv */
};

/*
#define astrophExp		"astro-*ph[/:][0-9][0-9][01][0-9]\\{4\\}"
#define arXiv__Exp		"astro-*ph[/:][0-9]\\{4\\}[.][0-9]\\{4\\}"
#define arXivt_Exp		"arXiv[/:][0-9]\\{4\\}[.][0-9]\\{4\\}"
*/
static char arule[] = "\
===================================================================";
static char *f2check;				/* File to check   */
static int  max_time;				/* timeout tooLong */
static int  errors_fi;				/* errors reported */
static int reset_alarm_ticks;			/* see reset_alarm */
static long input_bytes;			/* Bytes processed */
static time_t start_time;
static char *Arel ; 				/* /local/cgi-bin  */
static char *output_filter;			/* Output filter   */
static char *oglu_filter = OGLU ;
static char verbop = 0 ;			/* Verbose option  */
static char *emptycell ;			/* Set by init()   */
#define ignore_output (texenv->mode&MODE_ignore)
static char c1sep[] = " \t\n,;|!()[]{}<>&" ;	/* Separators, &   */
	/* The = is part of HTTP addresses */
static char not1char;				/* Additional sep  */
static char crypt_char;				/* Replace @ by at */

/* The pending status variable is set when a <TD> (cell) needs to be
   opened before any output.
   it is shared between  match_tex   and   table_out
   It cas be reset by setimode('w')
*/
static char pending = 0 ;

/* There are the following output routines here :
   => the default  fwrite
   => table_out    writing out in a {tabular} context
   => buf_out	   writing out into the "collector" buffer
   => buf1out	   writing out into the "collector" buffer, but issue line
			breaks.
   => com_out	   writing out in the context of HTML comments (remove <>)
*/

/* There are several dictionnaries used here, defined as 2-column
   tables:
   the services,
   TeX definitions (introduced by the \ in definition file)
   TeX environments (introduced by the { in definition file)
   cgi_contexts (introduced by the ! in definition file)
   valid HTML tags (introduced by the < in definition file))
*/
typedef struct table2s {
    struct table2s *prev ;
    char *buf;
    int items, flags;
#define T2_SORTED	1	/* Flag Dic is Sorted	*/
#define T2_BUFREE	2	/* Flag buf is to free	*/
#define T2_BUFDUP	4	/* Duplicate Buffer	*/
#define T2_amp1      0x10	/* Keep 1st ampersand xplanation */
#define T2_amp2      0x20	/* Keep 2nd ampersand xplanation */
    char *rows[2] ;		/* Actually 2*items elements */
} TABLE2;
static char c1def[] = "\\{$!<%&:*+'" ;	/* First letter in -f file 	*/
static TABLE2 *t2def[sizeof(c1def)] ;
#define texdef_dic t2def[0]
#define texenv_dic t2def[1]
#define  env_dic t2def[2]
#define  cgi_dic t2def[3]
#define html_dic t2def[4]
#define  glu_dic t2def[5]	/* %  Glu Data		*/
#define aglu_dic t2def[6]	/* &  Glu Actions 	*/
#define serv_dic t2def[7]	/* :  Services		*/
#define unit_dic t2def[8]	/* *  Unit Length	*/
#define  cnt_dic t2def[9]	/* +  Counters...	*/
#define save_dic t2def[10]	/* '  Words saved	*/
static TABLE2 **saved_t2def;	/* Definitions saved in do_conv */

/* List of characters which delimit words		*/
static char bword[] = " ,=()[]>\t\n" ;
/* The input is stacked (lookahead is required) in linked buffers;
   the top buffer is "inbuf", and when empty, getchar is used.
*/
typedef struct sbuf {	/* Linked buffers */
    struct sbuf *prev;
    int size, used, pos;	/* Allocated, used, current position 	*/
    char flags ;		/* Special flags 			*/
#define BUF_parm	1	/* Buffer is in parameter list, no free */
#define BUF_env		2	/* Buffer is in env.args. list, no free */
    char text[7];		/* The actual size depends on size 	*/
} BUF;
static int  do_option ;		/* Arguments to do_conv as :		*/
#define DO_CHECK	1	/* ... check starts by \cgidef{...	*/
#define DO_PLAIN	2	/* ... no translation on do_conv	*/
#define DO_NOGET	4	/* ... when inbuf exhausted, stop	*/
#define DO_NOCLOSE	8	/* ... no closing at end of do_conv	*/
static BUF  *inbuf;
static BUF  *collector ;	/* Used by the collect  output routine	*/
static BUF  *unescaped_buf;	/* Used in unescaped_string+length	*/
static FILE *infile;		/* Current Input File			*/
static FILE *_of ;		/* Current Output File			*/
static char *httpd_home ;	/* Root of httpd (only for secure)	*/
static int  outFilter = 0;	/* In case an output processs launched	*/
static int children[MAX_PIPES]; /* Store the pids of children (for wait)*/
static int last_child = 0 ;	/* Set by open_pipe (for outFilter)	*/

/* Variables which can be retrieved, and are retrieved via get1val	*/
static char *val1buf[] = {
  (char *)0, 		/* +00: Program designation	*/
  "(stdin)", 		/* +01: Opened filename 	*/
  (char *)0,		/* +02: HTML Version		*/
  (char *)0,		/* +03: JavaScript Version	*/
#define val1buf_2free 4	/* ---- From here, allocated	*/
  (char *)0,		/* +04: Color of next column	*/
  (char *)0,		/* +05: Argument HTML next row	*/
#define theprogram 	val1buf[0]
#define thefile 	val1buf[1]
#define vhtml 		val1buf[2]
#define vMozilla	val1buf[3]	/* JS version */
#define bg1col		val1buf[4]
#define arg1row		val1buf[5]
};
/* static char *thefile="(stdin)";	--    "     "    FileName	*/
/* static char *theprogram;       	--    "     "    Program 	*/
/* static char *vhtml ;			--  HTML Version	*/
/* static char *vMozilla ;		--  JavaScript   */

/* The HTML tags (which have to be closed) are stacked */
static char *tag2html;		/* Detault URL_ */
static char *tag_stack[MAX_TAG] ;
static int  itag_stack = 0;

/* The TeX parameters (of a macro) are saved as buffers	*/
static char texparm1 ;		/* Shared by get_texparm, '[' or '{'	*/
static char *begin_more ;	/* Shared by begin_env, in \Beg context	*/
static BUF *texparm[MAX_TEX];
static int itexparm = 0;
/* The TeX environment & environment parameters are saved apart	*/
static BUF *ptexenv[MAX_ENV] ;
static int  itexenv = 0 ;
/* Maximal number of embedded tables */
static BUF *tabenv[MAX_TAB+1] ;

static char keep_soft_mode[] = "\33{\37m\33}";	/* Used get_texparm */
static char *doconv_topush ;	/* Used exec_cgi an do_conv	*/
static char *doconv_toset ;	/* Used exec_cgi and do_conv	*/

static char href_status;	/* While in HREF="..."		*/
#define HREF_started	 001	/* Just started, HREF=" issued	*/
#define HREF_file	 002	/* Before the ?			*/
#define HREF_arg	 003	/* After the ?			*/
#define HREF_inJS	 004	/* The href contains JavaScript	*/
#define HREF_inJSarg	 005	/* Within '...' of javascript 	*/

/* The TeX current environment is described by a TEX_ENV structure */
typedef struct {
    char *env;			/* Current environment		*/
    TABLE2 **defs ;		/* Saved definitions...		*/
    INT_FCT output_routine ;	/* Current Output Routine	*/
    short iparm;		/* Position in texparm		*/
    short ipenv;		/* How many env params used 	*/
    unsigned char ibrace ;	/* Current depth in { .. }	*/
    unsigned char itable ;	/* Column Arguments in Table	*/
    unsigned char itag ;	/* Position in tag_stack	*/
    char status;		/* non-blank issued, if status	*/
#define MODE_out1d   	0x01	/*   At least 1 non-blank char	*/
#define MODE_out1c   	0x02	/*   At least 1  "  " " in cell */
    int mode;			/* Current mode			*/
    				/* 0 or . ==> set mode to Zero	*/
#define MODE_PRE 	0001	/* P Within <PRE>          	*/
#define MODE_HTML	0002	/* H Within {html}		*/
#define MODE_tab	0004	/* T Within {tabular}		*/
#define MODE_plain	0010	/* q Within \plain (no anchor)	*/
#define MODE_VERB	0020	/* V Within {verbatim}		*/
#define MODE_intag	0040	/* t Within <tag>: beware &quot	*/
#define MODE_ignore	0100	/* - Ignore the output		*/
#define MODE_soft	0200	/* S Within ``soft'' TeX	*/
#define MODE_col	0x0100	/* C Within <TD>		*/
#define MODE_row	0x0200	/* R Row started		*/
#define MODE_baseline	0x0400	/*   Row in BASELINE alignement	*/
#define MODE_tab2	0x0800	/*   Table emulation		*/
#define MODE_math	0x1000	/* m LaTeX Mathematical Mode ($)*/
#define MODE_MATH	0x2000	/* M LaTeX DisplayMath Mode ($$)*/
#define MODE_hiddensoft	0x4000	/*   Hidden ``soft'' TeX 	*/
#define MODE_SOFT		    (MODE_soft|MODE_hiddensoft)
#define MODE_emptycell	0x8000  /* c Allows empty cells<TD></TD>*/
#define MODE_upper   0x0010000	/* u Convert to UpperCase	*/
#define MODE_lower   0x0020000	/* l Convert to LowerCase	*/
#define MODE_2html   0x0040000	/* h Convert  & < > ' "		*/
#define MODE_obeylines 0x80000  /* o Within \obeylines		*/
#define MODE_false   0x0100000	/*   Set to 1 within \iffalse	*/
#define MODE_ifori   0x0200000	/*   Ignore mode at starting \if*/
#define MODE_verb1   0x0400000	/*   Just a single character	*/
#define MODE_quot    0x0800000	/* " In Quotes " for Arguments  */
#define MODE_href1   0x1000000	/* ? href just started...	*/
#define MODE_href2   0x2000000	/* 2 href: before the ?		*/
#define MODE_hrefa   0x3000000	/* a After the ? -- arguments	*/
#define MODE_js	     0x4000000	/* j href: in Javascript 	*/
#define MODE_jsArg   0x5000000	/* ' href: in Javascript 'arg'	*/
#define MODE_href    0x7000000	/*   All HREF or SRC modes	*/
#define MODE_xArg    0x8000000	/* % Convert to %hexa even &	*/
#define MODE_jsquo  0x10000000	/* ` Backslash \&apos;	(js)	*/
/*#define MODE_Aargq 0x20000000	-- ? Be prepared for Aarg afer?	*/
/*#define MODE_Aarga 0x40000000	-- % Convert to %ascii even &	*/
#define MODE_Aglu   0x80000000	/* g Be prepared for |-matching */
                    		/* # Anchor,  generate the  # 	*/
} TEX_ENV;
/* --------------------------- v8.2 -----------------------------
  Note that in href mode (output of a HREF="...") there are the
  following modes for theMode&MODE_href:
    (1) MODE_href1  ==> Just started HREF, look if javascript
    (2) MODE_href2  ==> 'Standard' HREF, and no ? has been found
    (3) MODE_hrefa  ==> 'Standard' HREF, in list of arguments (& is separator)
    (4) MODE_js     ==> Type HREF="javascript:...."
    (5) MODE_jsArg  ==> In quoted javascript, e.g. HREF="javascript:fct('Here')
*/

static TEX_ENV tex_stack[MAX_TEX] = { {""} };
static TEX_ENV *texenv = tex_stack ;
static TEX_ENV *textop = tex_stack ;	/* Used in do_conv	*/
#define etexenv &(tex_stack[MAX_TEX-1])
#define theMode 	 (texenv->mode)
#define theStatus 	 (texenv->status)
#define if_mode(m)	 if (theMode&(m))
#define soft_tex	 (theMode&(MODE_soft|MODE_hiddensoft))
#define theOutputRoutine (texenv->output_routine)
static INT_FCT OutFct ;
static INT_FCT saved_OutFct ;
static int ToC_status ;		/* Status word for TOC hierarchy	*/

static char tex_ok[128];	/* Valid TeX chars, set by load_services */
static char notag[128];		/* %-lines not to be tagged		 */
static char theflag = 1 ;	/* Value 3 if argument, 1 otherwise	 */

/* Options, saved as a structure		*/
static struct s_options {
    char *bib;			/* -bib  option	*/
    char *cat;			/* -cat  option	*/
    char *mail;			/* -mail option	*/
    char *dic;			/* -dic  option	*/
    int   html;			/* -html option	*/
    int   pre;			/* -pre  option	*/
    int   lis;			/* -lis  option	*/
    int   glu;			/* -glu  option	*/
    int   tex;			/* -tex  option, 0x10 if TeX-verbatim   \
						and 0x02 for "soft-TeX */
    int   sec;			/* Secure option */
    int   lang;			/* Language option */
#define LANG_en		0
#define LANG_fr		1
    int   nomath ;		/* -nomath in italics option */
    				/* 2: ^ implies: math mode   */
    int   keep ;		/* 1=^ 2=_ 4=$ 8=~ 16=%      */
    int   pop ;			/* -pop window	*/
} options;

#define obib	options.bib	/* -bib  option	*/
#define ocat	options.cat	/* -cat  option	*/
#define ohtml	options.html 	/* -html option	*/
#define omail	options.mail	/* -mail option	*/
#define odic	options.dic	/* -dic  option	*/
#define otex	options.tex  	/* -tex  option, 0x10 if TeX-verbatim	 \
						 and 0x02 for "soft-TeX	*/
#define opre	options.pre	/* -pre  option	*/
#define olis	options.lis	/* -lis  option	*/
#define oglu	options.glu	/* -lis  option	-- 02 if -gluw (v8.53)	*/
#define onomath	options.nomath	/* -mathit opt	*/
#define okeep	options.keep	/* -keep   opt	*/
#define opop	options.pop	/* -pop    opt	*/

static char  disable_output;	/* -nooutput	*/
static char keep_slash = '/' ;	/* Netscape accept / in arguments. */

/* List of some browsers accepting TABLES */
static char *HTML3Agents[] = {
  "Mozilla/3",
  "Mozilla/1.12IS ",
  "Mozilla/2.0 (compatible",
  (char *)0
  } ;
/* List of some browsers accepting JAVASCRIPT  -- USELESS, All are Mozilla
static char *JS3Agents[] = {
  "Mozilla/3", "3.0",
  "Mozilla/4", "4.0",
  (char *)0
} ;
*/
/* Transformation from TeX:
   1) Accents, where HTML and TeX are different
      (TeX writes \accent,letter, HTML writes &letter,accent)
*/
static char tex_acc[] = "'`^\"~c=bH.uv";
static char *html_acc[sizeof(tex_acc)] = {
    "acute;", "grave;", "circ;", "uml;", "tilde;" /*, "cedil;" */
  };
static char *letters_acc[] = {	/* Letters with a allowed accent */
    /* acute */	"AEIOUYaeiou",
    /* grave */	"AEIOUaeiou",
    /* circ  */	"AEIOUaeiou",
    /* uml   */	"AEIOUaeiouy",
    /* tilde */	"AONaon"
} ;

/* 2) Other equivalences; the first char, if a control, indicates:
      \1 = Accent; the letter indicates its equivalence in tex_acc
      \2 = Typeface;  the letter indicates the shape etc..
      \3 = Font;  the letter indicates the size etc..
      \4 = Reset texparm (to avoid too many parameters) ;
      \5 = Environment, followed by + for options or / for end
      \6 = Call a dedicated routine, next byte = routine number
      \7 = Mode Change (verbatim / HTML ...), next byte = new mode.
      \10= Issue verbatim the following byte\n\
      \16= Call a dedicated routine NO RESCAN, next byte = routine number
      \17= Set Mode (equivalent to SET1mode), next byte = see set1mode
      \20= Environment Parameters	(\20 =NOOP)
      \21= Argument Used, followed by arg_num
      \27= Set Mode (only last byte of mode)
      \30= AND Mode, next byte = argument to AND (almost unused)
      \33= push/pop_tex (in verbatim context) -- Open/close cgidef
      \37=  OR Mode, next byte = argument to OR
      \sf	#0{\3FACE='Lucida Sans'\10>}\n\
*/
static char charlot[256] = {	/* Special Control Chars Meaning:
    xx0 xx1 xx2 xx3 xx4 xx5 xx6 xx7 Indicates how many chars to read	 */
     0,  1,  1,  1,  2,  0,  1,  1, 	/* 00x	*/
     1,  0,  0,  0,  0,  0,  1,  1, 	/* 01x	*/ /* !! 014 015 MUST BE 0 */
     1,  1,  0,  0,  0,  0,  0,  1, 	/* 02x	*/
     1,  0,  0,  1,  0,  0,  0,  1, 	/* 03x	*/
  } ;
static short counter[64];		/* Counters */
static char counter_mode[64] ;		/* How to edit a counter */
#define COUNT_buffer	13
#define COUNT_long	15		/* \long used (\long\def..) */
#define COUNT_tabline	14		/* Also the highest counter */
#define COUNT_head	16		/*  Whether \CDShead issued */
#define COUNT_macrostar	11		/* Also the highest counter */
#define COUNT_chapter	 2		/* Highest level in counter */
#define COUNT_deepest	 5		/* Deepest level in counter */
static int used_counters ;
static short obj_counter ;	/* Counter in objlist */

/* Extended ascii (iso 8859): TeX Equivalents. */
static char *tex8[] = {
  "\200", "\201", ","   , "{\\em f}", "''",
                              "{\\dots}", "{\\dag}", "{\\ddag}" , /* 128-135 */
  "\\^{ }", "\211", "_" , "<" , "{\\OE}", "\215", "\216", "\217", /* 136-143 */
  "\220", "`"   , "'"   , "``"  , "''", "{\bullet}", "---", "--", /* 144-151 */
  "\\~{ }", "^{TM}", "\\_", ">",
	                     "{\\oe}"  , "\235", "\236", "\"{Y}", /* 152-159 */
  "~"   , "!`", "{\\cents}", "{\\pounds}",
			       "\\$", "{\\Yens}", "|"  , "{\\S}", /* 160-167 */
  "\\\"{ }", "{\\copyright}.", "^a", "{\\langle\\!\\langle}",
                            "{\\not}", "--"  , "{\\TM}", "^-"   , /* 168-175 */
  "{\\deg}", "{\\pm}", "^2", "^3",
                         "\\'{ }", "{\\mu}", "{\\P}", "{\\cdot}", /* 176-183 */
  "\\c{ }", "^1", "^o"  , "{\\rangle\\!\\rangle}",
            "\\frac{1}{4}", "\\frac{1}{2}", "\\frac{3}{4}", "?`", /* 184-191 */
  "\\`{A}", "\\'{A}", "\\^{A}", "\\~{A}", "\\\"{A}", "{\\AA}",
                                           "{\\AE}", "\\c{C}"   , /* 192-199 */
  "\\`{E}", "\\'{E}", "\\^{E}", "\\\"{E}",
                         "\\`{I}", "\\'{I}", "\\^{I}", "\\\"{I}", /* 200-207 */
  "{\\Eth}", "\\~N", "\\`{O}", "\\'{O}",
                     "\\^{O}", "\\~{O}", "\\\"O", "{\\times}"   , /* 208-215 */
  "{\\O}", "\\`{U}", "\\'{U}", "\\^{U}", "\\\"U",
                               "\\'{Y}", "{\\Thorn}", "{\\ss}"  , /* 216-223 */
  "\\`{a}", "\\'{a}", "\\^{a}", "\\~{a}", "\\\"a",
                                    "{\\aa}", "{\\ae}", "\\c{c}", /* 224-231 */
  "\\`{e}", "\\'{e}", "\\^{e}", "\\\"{e}",
                 "\\`{\\i}", "\\'{\\i}", "\\^{\\i}", "\\\"{\\i}", /* 232-239 */
  "{\\eth}", "\\~{n}", "\\`{o}", "\\'{o}", "\\^{o}", "\\~{o}",
                                         "\\\'{o}", "{\\div}"   , /* 240-247 */
  "{\\o}", "\\`{u}", "\\'{u}", "\\^{u}", "\\\"u",
                               "\\'{y}", "{\\thorn}", "\\\"y"     /* 248-255 */
} ;

/* Explanations of some of the Macros */
static char explain_macros[] = "\
############# HTML-specific definitions:\n\
\\cgidef {Options & Definitions}	Specifies used dialect, assigns macros\n\
\\HTMLversion				== Version of Browser (e.g. 3.0)\n\
\\html {text}				text assumed to be valid HTML\n\
\\tag {tag}				Enclose tag within <>\n\
\\tagged {tag} {text}			Text within <tag> ... </tag>\n\
\\basefont {font_size}			HTML <BASEFONT>\n\
\\break					HTML <BR>\n\
\\hr					HTML <HR>\n\
\\thickrule 				HTML <HR NOSHADE>\n\
\\title	{title}				HTML <TITLE>title</TITLE>\n\
\\Item {item_name}			To use instead of \\item[item_name]\n\
\\term {item_name}			Same meaning as \\Item\n\
\\option {option_name}			HTML <Option>\n\
\\Option {option_name}			HTML <option SELECTED>\n\
\\fg	{color_name}			Defined foreground Colour\n\
\\htarg {argument}			Convert argument (e.g. + into %2b)\n\
\\htval {argument}			Convert also Quotes (\"') into &quot;\n\
\\plain	{argument}			Don't try to interpret e.g. bibcodes\n\
\\plainbrowser 				== \"plain\" for pre-3.0 browser\n\
############# Table-specific definitions:\n\
\\colheader {text}			Write Column Header (label)\n\
\\emptycell   				Allows empty cells (without borders)\n\
\\noemptycell 				Empty cells not allowed (filled with <BR>)\n\
\\multicolumn {N arg}{c|l|r|p}{text}	Ex: \\multicolumn{1 BGCOLOR=Red}{c}{text}\n\
############# Anchor Generation:\n\
\\ref {label}				Cross-reference (standard latex)\n\
\\A {address} {underlined text}		Create an Anchor\n\
\\Aref {label} {underlined text} 	Anchor to inside text\n\
\\eref {label} {mark}			Extended HREF#mark\n\
\\Arel {filename} {underlined text}	Anchor to file in same directory\n\
\\W{target} {address} {underlined text}	Anchor to target window\n\
\\Wrel{target} {fle} {underlined text}	Targeted anchor in same directory\n\
############# GLU-related Macros:\n\
\\glutag   {glu_action}			Generate <&glu_action>\n\
\\Aglutag  {glu_action} {text}		Generate GLU action mark\n\
\\gludata  {glu_datatype}		Generate <%glu_datatype>\n\
\\Agludata {glu_datatype} {text}	Generate GLU datatype mark\n\
\\yA       {why}{href}{text}		Anchor with explanation\n\
\\yW       {why}{name}{href}{text}	Anchor in window with explanation\n\
############# CDS-specific Headers & Menus\n\
\\CDShead {title} {logo} 		See <&CDShead>\n\
\\CDSpage {title} {logo} 		== <TITLE> + \\CDShead\n\
\\CDStail {Mail_topic}			See <&CDStail>\n\
\\CDStailmenu {Mail-topic}		See <&CDS.tailmenu>\n\
\\CDStitle {title} {logo} 		See <&CDStitle>\n\
\\objA {@-number} {Identification}	Anchor to SIMBAD object (no coos)\n\
\\objS {Simbad-name} {Identification}	Anchor to SIMBAD object (no passowrd)\n\
\\objC {Simbad-name} {Identification}	Anchor to SIMBAD CORRECTED object)\n\
\\objM {Simbad-names} {Identification}	Anchor to SIMBAD LIST of objects)\n\
\\objP {prefix (acronym)} {designation}	Anchor to SIMBAD {acronym}{desig}\n\
\\obja {@-number} {Identification}	Anchor to SIMBAD object (no passowrd)\n\
\\objac{@-number} {Identification}{pos}	Anchor to SIMBAD object (no passowrd)\n\
\\object {Identification}		Anchor to SIMBAD (with password)\n\
\\objnS	{Identification}		Non-SIMBAD object\n\
\\soft {text}				Interprets text A&A Abstracts-like\n\
############# Other useful macros:\n\
\\ionZ {symbol or Z}			Convert atomic number(+ion) <-> symbol\n\
\\obeylines 				Line breaking is kept\n\
\\typein {prompt}			Get input from terminal\n\
\\cr					Writes a newline (but not a <BR>)\n\
\\deg					Degree symbol\n\
\\xcolor {color_name}			Convert color name or r,g,b to #RRGGBB\n\
\\thefile 				Name of file being processed\n\
\\theprogram 				HTTPD-name of program being executed\n\
\\basename {filename}			Remove directories in filename\n\
\\dirname  {filename}			Extracts directory prt from filename\n\
\\env {variable_name}			Get the value of an environment variable\n\
\\putenv {variable=text}		\tAdd an environment variable\n\
\\ifhtx ... \\fi			\tAlways true (in htx context)\n\
\\ifmatch  {pattern} {text}    ... \\fi	Test text matches the pattern\n\
\\ifregexp {regular_exp} {text} .. \\fi	Test text matches the regular_expression\n\
\\ifplainbrowser  ... \\fi		Test browser has version < 3.0\n\
\\inputverbatim	{file}			Display text from file in verbatim mode\n\
\\lowercase {arg}			Convert text to lowercase\n\
\\uppercase {arg}			Convert text to uppercase\n\
\\sed  {s/regexp/subsg}{string}		String substitution (like sed)\n\
\\whenplainbrowser {text}		Issue text if pre-3.0 browser\n\
\\list	{title}				Display all known macros\n\
\\quit					Terminates (equivalent to \\bye)\n\
############# Interesting Environments:\n\
{TABLESORT}{cols}	Sortable table\n\
{tabular}{cols}		Standard Tables\n\
{TABULAR}{cols}		Table using the full width\n\
{glossary}		Explanations of words (HTML <DL>)\n\
{tex}			Contains full-TeX\n\
{soft}			Text in \"soft-TeX\" (like A&A Abstracts)\n\
{plain}			No automatic recognitions in this environment\n\
{alltt}			Synonym of {PRE}\n\
{HTML}			Assumed fully HTML-formatted text\n\
{JavaScript}		Javascript\n\
";

/* Definition of default tagfile */
/* \\special #1{\33{\7\30\6\40#1\33}}\n\ */
/* NOTE: \021x with x < '0' is NOOP -- used when get1arg (\6\040) is called.
  This is required to ensure that the 'execute' routine does not
  pop any argument
{\7\10\10<\10%\37\2#1 |#2\10>}}\n\
... Replaced in v8.15:
#\\objA		#2{\10<\10&simq \"#1\" \"#2\"|#2\10>}\n\
#\\objC		#2{\10<\10&smb.query \"#1\"|#2 (read: #1)\10>}\n\
#\\objM		#2{\10<\10&smb.list \"#1\"|#2 (read: #1)\10>}\n\
#\\objName	#1{\10<\10&smb.query \"Name #1\"|#1\10>}\n\
#\\objS		#2{\10<\10&smb.query \"#1\"|#2\10>}\n\
#\\obja		#2{\10<\10&alasimq \"#1\" \"#2\"|#2\10>}\n\
#\\objac	#3{\10<\10&simq \"#1\" \"#2\" \"#3\"|#2\10>}\n\
#\\object	#1{\10<\\%I #1\10>}\n\
#\\objectname	#0{\10<\\%I \\ifargop\6\022-o\6\022-a\"#1\"|#2\\else\
 \6\022-a#1\\fi\10>}\n\
*/

static char default_defs[] = "\
######### Default GLU definitions\n\
%E:GCPD/ http://obswww.unige.ch/gcpd/ph$2.html\n\
%E:BDA/  http://obswww.unige.ch/webda/cgi-bin/ocl_page.cgi?$$2\n\
#Default definitions for CDS Services\n\
$TAG2HTML=URL_\n\
$URL_Abs=http://astrobib.u-strasbg.fr:2008/cgi-bin/cdsbib??\n\
$URL_Bib=http://astrobib.u-strasbg.fr:2008/cgi-bin/cdsbib??\n\
$URL_Cat=http://cdsarc.u-strasbg.fr/cgi-bin/Cat?\n\
$URL_Dic=http://cds.u-strasbg.fr/cgi-bin/Dic?cat=Exact&version=-1&\n\
$URL_Fit=http://vizier.u-strasbg.fr/viz-bin/getCatFile?\n\
$URL_FTP=ftp://cdsarc.u-strasbg.fr/pub/cats/\n\
$URL_Uni=http://vizier.u-strasbg.fr/viz-bin/Unit?\n\
$FTP_Cat=ftp://cdsarc.u-strasbg.fr/pub/cats/\n\
######### Services ($$0 ==> CRYPT)\n\
:plain:	$0\n#$0 can't be an environment variable, no anchor generation\n\
:::	&lt;\n\
:http: 	http:\n\
:ftp: 	ftp:\n\
:telnet: telnet:\n\
:file: file:\n\
:A:\n\
:A#: #\n\
:Mail: mailto:$$0\n\
# Non-CDS Catalogues \n\
:Cat:GCPD/ http://obswww.unige.ch/gcpd/ph$2.html\n\
:Cat:BDA/  http://obswww.unige.ch/webda/cgi-bin/ocl_page.cgi?$$2\n\
######### TeX Definitions (Leave Typefaces (\2) at top)\n\
#     \\1 = Accent; the letter indicates its equivalence in tex_acc\n\
#     \\2 = Typeface;  the letter indicates the shape etc..\n\
#     \\3 = Font;  the letter indicates the size etc..\n\
#     \\4 = Reset texparm (to avoid too many parameters);\n\
#     \\5 = Environment, followed by + for options or / for end\n\
#     \\6 = Definitions related to Functions  \20 =NOOP \n\
#     \\7 = Mode Change (verbatim / HTML ...), next byte = new mode.\n\
#     \\10= Issue verbatim the following byte\n\
#     \\16= Definitions related to Functions SPECIAL \n\
#     \\17= Equivalent to \\SET1mode\n\
#     \\21= Argument used, followed by arg_num (#)\n\
#     \\27= set last byte of mode only\n\
########### WARNING: List below IS SORTED !!!!\n\
\\\014	\17#\n\
\\\015	{}\n\
\\ 	\\x20\n\
\\!	&#161;\n\
\\!<	\\not<\n\
\\!=	\\neq\n\
\\!>	\\not>\n\
\\!~	\\not~\n\
\\#	\10#\n\
\\$	\10$\n\
\\%	\10%\n\
\\&	&amp;\n\
\\(	$\n\
\\)	$\n\
\\+	+\n\
\\+-	\\pm\n\
\\+/-	\\pm\n\
\\,	\10 \n\
\\-	{}\n\
\\-+	\\mp\n\
\\--	\\ndash\n\
\\---	\\mdash\n\
\\----	\\rule\n\
\\-->	\\longrightarrow\n\
\\-/+	\\mp\n\
\\->	\\rightarrow\n\
\\.	\1.\n\
\\/	{}\n\
\\/<	\\not<\n\
\\/=	\\neq\n\
\\/>	\\not>\n\
\\/~	\\not~\n\
\\;	\10 \n\
\\<	\10<\n\
\\<-	\\leftarrow\n\
\\<--	\\longleftarrow\n\
\\<<	\\ll\n\
\\<=	\\le\n\
\\<==	\\Longleftarrow\n\
\\<==>	\\Longleftrightarrow\n\
\\<~	\\la\n\
\\=	\1=\n\
\\==	\\equiv\n\
\\====	\10<HR size=4\10>\n\
\\==>	\\Longrightarrow\n\
\\=~	\\simeq\n\
\\>	\10>\n\
\\>=	\\ge\n\
\\>>	\\gl\n\
\\>~	\\ga\n\
\\@	#1{#1{ }}\n\
\\A	#2{{{\7\10\10<A HREF=\"\33{\17?#1\33}\"\10>}{\37\10#2}\10</A\10>\33}}\n\
#\\A	#2{{{\7\10\10<A HREF=\"\33{\6\17#1\33}\"\10>}{\37\10#2}\10</A\10>\33}}\n\
\\AA	&Aring;\n\
\\AE    &#198;\n\
\\Aglu	#2{\\glutag{#1|#2}}\n\
\\Agludata	#2{\\gludata{#1|#2}}\n\
\\Aglutag	#2{\\glutag{#1|#2}}\n\
\\Alay	#3{\33{\33{\7\10\10<A HREF=\"\33{\17?#2\33}\"\
 onMouseOver=\"lay('\33{\17h\17`#1\33}')\"\
 onMouseOut=\"laz()\"\10>\33}\33{\37\10#3\33}\10</A\10>\33}}\n\
#\\Alay	#3{\33{\33{\7\10\10<A HREF=\"\33{\17?#2\33}\"\
 title=\"{\17h\17`#1\33}\"\10>\33}\33{#3\33}\10</A\10>\33}}\n\
\\Alph	#1{\\edicnt{A#1}}\n\
\\Apop  #3{{{\7\10\10<A href=\"\33{\17?#2\33}\" onClick=\"window.open(this\
.href,'\6\070#1','\6\071#1');return false\"\10>}{\37\10#3}\10</A\10>\33}}\n\
#\\Aref  #2{\\ref{#1}#2\10</A\10>}\n\
\\Aref	#2{{\37\10\10<A HREF=\"\\\014#1\"\10>#2\10</A\10>\33}}\n\
\\Arefx #3{{\37\10\10<A HREF=\"\33{\17?#1\33}\17##2\"\10>#3\10</A\10>\33}}\n\
\\Arel	#2{\\A{/\\dirname{\\thefile}/#1}{#2}}\n\
\\Beg	\5+\n\
\\Begin	\5+\n\
\\Big	#1{{\\Large #1}}\n\
\\Bigl	#1{{\\Large #1}}\n\
\\Bigm	#1{{\\Large #1}}\n\
\\Bigr	#1{{\\Large #1}}\n\
\\CDShead #2{\\ifmatch{*/[Ww]/*}{\\thefile}\\winHead{{\\Large\\bf #1}}\\else\
 \\glutag{CDS.head \"#1\" #2}\\stepcounter{head}\\fi}\n\
\\CDSpage #2{\10<HTML\10>\\x0A\\CDShead{#1}{#2}}\n\
\\CDStail #1{\\ifnum\\thehead>0\\glutag{CDS.tail \"#1\"\
 \\ifdef\\signature\"\\signature\"\\fi}\
\\else\\glutag{CDS.tail1 \"#1\" \\ifdef\\signature\"\\signature\"\\fi}\\fi}\n\
#\\CDStaila #2{\\ifnum\\thehead>0\\glutag{CDS.tail \"#1\" \"#2\"}\
#\\else\\glutag{CDS.tail1 \"#1\" \"#2\"}\\fi}\n\
\\CDStailmenu	#1{\\glutag{CDS.tailmenu \"#1\"}}\n\
#\\CDStitle	#2{\\glutag{CDS.head0 \"#1\" #2}}\n\
\\CDStitle #2{\\ifmatch{*/[Ww]/*}{\\thefile}\\winHead{{\\Large\\bf #1}}\\else\
 \\glutag{CDS.head0 \"#1\" #2}\\fi}\n\
\\Column #2{\\multicolumn{1 #1}{}{#2}}\n\
\\Delta	\\{Delta\\}\n\
\\ENV 	#1{\6\13#1}\n\
\\Earth \\{Earth\\}\n\
\\End	\5/\n\
### GET1ARG get a RESCANNED argument (GET1arg+expandarg)\n\
\\GET1ARG  \6\022-a\6\55\21\6\n\
\\GET1arg  \6\022-a\n\
\\GETarglen \6\022-l\n\
\\GETargop  \6\022-o\n\
\\Gamma	\\{Gamma\\}\n\
\\H	\1H\n\
#\\HTMLversion	\6\17\n\
\\HTMLversion	\\ed1buf{2}\n\
\\Huge	\3+4\n\
# The 3 arguments are SRC ALT, other HTML tags (border, height, width, etc)\n\
\\IMG    #3{\33{\7\10\10<IMG SRC=\"\33{\17?#1\33}\"\
 {\\ifmatch{}{#2}\\else ALT=\"\33{\17h#2\33}\" TITLE=\"\33{\17h#2\33}\"\\fi}\
 #3\10>\33}}\n\
#ALT=\"\33{\17h#2\33}\" #3\10>\33}}\n\
\\Item	#1{\\tag{DT\10>#1\10<DD}}\n\
\\JSversion	\\vMozilla\n\
#\\JSversion	{}\n#Space4Change\n\
\\JavaMail {\\echo1x{1}}\n\
\\JavaPop {\\ifnum\\vMozilla>1\\begin{javascript}window.focus()\
;\\end{javascript}\\fi}\n\
#\\JavaScript #1{\\ifnum\\vMozilla>0{\33{\7\2\37\010<SCRIPT\
# LANGUAGE=\"JavaScript\">#1</SCRIPT>\33}}\\fi}\n\
\\Jupiter \\{Jupiter\\}\n\
\\L	L\n\
\\LARGE	\3+3\n\
\\LaTeX	{\33{\\SET1mode{m}L_AT_EX\33}}\n\
\\Lambda \\{Lambda\\}\n\
\\Large	\3+2\n\
\\Leftarrow \\verb+<=+\n\
\\Leftrightarrow  \\verb+<=>+\n\
\\Longleftarrow  \\verb+<==+\n\
\\Longleftrightarrow  \\verb+<==>+\n\
\\Longrightarrow \\verb+==>+\n\
\\Mars \\{Mars\\}\n\
\\Mercury \\{Mercury\\}\n\
\\Moon \\{Moon\\}\n\
\\Neptune \\{Neptune\\}\n\
\\O	&Oslash;\n\
#\\OE	OE\n\
\\OE	&#338;\n\
\\Omega	\\{Omega\\}\n\
\\Option #1{\10<OPTION SELECTED\10>#1}\n\
\\P	&#182;\n\
\\Phi	\\{Phi\\}\n\
\\Pi 	\\{Pi\\}\n\
\\Psi	\\{Psi\\}\n\
\\Rightarrow \\verb+=>+\n\
\\Roman	#1{\\edicnt{R#1}}\n\
\\S	&#167;\n\
\\SEP1	</TD><TD>\n\
\\SEP1l	</TD><TD ALIGN=LEFT>\n\
\\SET1mode #1{\6\21#1}\n\
\\SETmode4TeX \6\24\n\
\\SEToptions #1{\6\17#1}\n\
#\\SETmode4TeX \\SET1mode{0}\n\
\\Saturn \\{Saturn\\}\n\
\\Select #2{\10<OPTION SELECTED value=\"#1\"\10>#2\10</OPTION\10>}\n\
\\Sigma \\{Sigma\\}\n\
\\Sun \\{Sun\\}\n\
\\TeX	{\33{\\SET1mode{m}T_EX\33}}\n\
\\Theta	\\{Theta\\}\n\
#\\ToClabel {\\ifnum\\themacrostar=0\\stepcounter{ToC}ToC\\theToC\\fi}\n\
\\ToClabel {\\stepcounter{ToC}ToC\\theToC}\n\
\\Uparrow   \\{Up\\}\n\
\\Updownarrow   \\{UpDown\\}\n\
\\Uranus \\{Uranus\\}\n\
\\Venus \\{Venus\\}\n\
\\Vert	||\n\
\\W	#3{\\ifmatch{*[0-9]x[0-9]*}{#1}\\Apop{#1}{#2}{#3}\
\\else{\\Wa{#1}{#2}{#3}}\\fi}\n\
\\Wa	#3{{{\7\10\10<A HREF=\"\33{\17?#2\33}\" TARGET=\"#1\"\10>}{\37\10#3}\
\10</A\10>\33}}\n\
\\Wglu  #3{{{\7\10\10<A href=\"\10<\10&\6\072#2\10>\"\
 onClick=\"window.open(this.href,'\6\070#1','\6\071#1'); return false\"\10\
>}{\37\10#3}\10</A\10>\33}}\n\
\\Wlay	#4{{{\7\10\10<A TARGET=\"#1\" HREF=\"\33{\17?#3\33}\"\
 onMouseOver=\"lay('\33{\17h\17`#2\33}')\"\
 onMouseOut=\"laz()\"\10>}{\37\10#4}\10</A\10>\33}}\n\
#\\Wlay	#4{{{\7\10\10<A TARGET=\"#1\" HREF=\"\33{\17?#3\33}\"\
 title=\"{\17h\17`#2\33}\"\10>}{\37\10#4}\10</A\10>\33}}\n\
\\Wrel	#3{\\W{#1}{/\\dirname{\\thefile}/#2}{#3}}\n\
\\Xi 	\\{Xi\\}\n\
\\\\	<BR>\n\
\\_	\10_\n\
\\____	\rule\n\
\\aa	&aring;\n\
\\about \10~\n\
\\addcontentsline #3{}\n\
\\addtocounter #2{\6\043#1#2}\n\
\\adic	#1{\10<\\%M #1\10>}\n\
\\ae	&#230;\n\
\\allowbreak	{}\n\
\\alph	#1{\\edicnt{a#1}}\n\
\\alpha	\\{alpha\\}\n\
\\appendix {\\label{\\ToClabel}\\x0A\\ifnum\\thechapter=0\\setcounter{section}{\
*0A}\\else\\setcounter{chapter}{*0A}\\fi}\n\
\\approx \10~\n\
\\arabic #1{\\edicnt{+#1}}\n\
\\ast	*\n\
\\author #1{\\savebuF{author}{#1}}\n\
\\b	\10\\\n\
\\backslash \\verb+\\+\n\
\\bar 	#1{\\{bar\\}#1}\n\
\\basefont #1{\10<BASEFONT #1\10>}\n\
\\baselineskip #1{}\n\
\\basename #1{\6\20#1}\n\
\\begin	\5\n\
\\begingroup \33B\n\
\\belowdisplayshortskip {}\n\
\\belowdisplayskip {}\n\
\\beta	\\{beta\\}\n\
\\bf	\2<B>\n\
\\bffam	\2<B>\n\
\\bg	#1{\\ifmode{T}\\set1buf{04}{\\bg0x{#1}}\\else\\bg0x{#1}\\fi}\n\
\\bg0x	#1{bgcolor=\"\\xcolor{#1}\"}\n\
\\bgroup \33B\n\
\\bibitem #1{\\label{bib:#1}\10<LI\10>}\n\
\\bibobj #2{\10<\10&bibobj \\verb|#1| \"#2\"|#2\10>}\n\
\\big	#1{{\\large #1}}\n\
\\bigbreak \\bigskip\n\
\\bigl	#1{{\\large #1}}\n\
\\bigm	#1{{\\large #1}}\n\
\\bigr	#1{{\\large #1}}\n\
\\bigskip \\vspace{2ex}\n\
\\bmod	{ mod }\n\
\\box	#1{}\n\
\\br	<BR>\n\
\\break	<BR>\n\
\\bullet &#8226;\n\
\\bye	\377\377\n\
\\c	\1c\n\
\\cal	#0{\3\033{\17tFACE='cursive' COLOR='\\xcolor{Maroon}'\10>\033}}\n\
\\cap	\\{intersect\\}\n\
\\caption #1{\\tagged{CAPTION}{#1}}\n\
\\cdot	&#x22C5;\n\
\\cdotp	&#x22C5;\n\
\\cdots	&#x22C5;&#x22C5;&#x22C5;\n\
\\centerline #1{{\\tagged{CENTER}{#1}\\\\}}\n\
#### No {} for cgidef ------\n\
\\cgi	 #1\6\5#1\n\
\\cgidef #1\6\5#1\n\
\\chapter #1{{\\fghead\\label{\\ToClabel}\\x0A\
\10<CENTER\10>\10<H1\10>\\ifnum\\themacrostar=0\\addtocounter{*2}{1}\
Chapter \\thechapter\\br\\fi{}#1\10</H1\10>\10</CENTER\10>}}\n\
\\chi	\\{chi\\}\n\
\\circ	o\n\
\\cite #1{\\Aref{bib:#1}{[ref]}}\n\
\\clearpage	{\\par}\n\
\\closetag \5/=\n\
\\colheader	#1{\\multicolumn{1}{h}{#1}}\n\
\\colon	{:}\n\
\\color	#1{\3\033{\17tCOLOR=\"\\xcolor{#1}\"\10>\033}}\n\
\\cong	{=\\verb+~+}\n\
\\copyright &#169;\n\
\\cos	{\\rm cos}\n\
\\cosec	{\\rm cosec}\n\
\\cotan	{\\rm cotan}\n\
\\cr	\\\\\n\
\\crcr	\\cr\\nl\n\
\\cup	\\{union\\}\n\
\\d	&#176;\n\
#\\dag	+\n\
\\dag	&#8224;\n\
\\date	#1{\6\060#1}\n\
#\\ddag	++\n\
\\ddag	&#8225;\n\
\\ddot	#1{\\\"#1}\n\
\\debug	#1{\6\031#1}\n\
\\def	\6\1\n\
\\deg	&#176;\n\
\\delta	\\{delta\\}\n\
\\dirname #1{\6\10#1}\n\
\\disableoutput \\SEToptions{-nooutput}\n\
\\div	&#247;\n\
# documentstyle; 1 optional + 1 required argument\n\
\\documentstyle	#0{\6\022o\6\022a}\n\
\\dot	#1{\\{dot\\}#1}\n\
\\dots	...\n\
\\echo1x #1{\6\063#1}\n\
\\echo4  #1{\6\064#1}\n\
\\ed1buf #1{\6\011#1}\n\
\\edicnt #1{\6\041#1}\n\
\\egroup \33E\n\
\\eject	\\bigskip\n\
\\ell	{\\em l}\n\
\\else	\21\6 [***\10\\else Badly Placed!]\n\
\\elsif	\21\6 [***\10\\elsif Badly Placed!]\n\
\\elsifmatch \21\6 [***\10\\elsif Badly Placed!]\n\
\\elsifmmode \21\6 [***\10\\elsif Badly Placed!]\n\
\\elsifnum \21\6 [***\10\\elsif Badly Placed!]\n\
\\elsifregexp \21\6 [***\10\\elsif Badly Placed!]\n\
\\em	\2<EM>\n\
\\emptycell   \6\15\n\
\\emptyset \\O\n\
\\emsp \\qquad\n\
\\enableoutput \\SEToptions{-+nooutput}\n\
\\end	\5/\n\
\\endgroup \33E\n\
\\enskip { }\n\
\\ensp \\quad\n\
\\enspace { }\n\
\\env 	#1{\6\13#1}\n\
\\epsilon \\{epsilon\\}\n\
\\eqalign {}\n\
\\eqalignno {}\n\
\\eqno #1{      #1}\n\
\\eref	#2{{\37\10\10<A HREF=\"#1\\\014#2\"\10>\33}}\n\
\\eta	\\{eta\\}\n\
\\exec	#1{\16\37#1}\n\
\\execexpanded	#1{\6\37#1}\n\
\\exp	{\\rm exp}\n\
\\expand1arg #1{\6\55#1}\n\
\\fam	#1{}\n\
\\fbox	#1{{\\fg{Blue}#1}}\n\
\\fg	#1{\3\033{\17tCOLOR='\\xcolor{#1}'\10>\033}}\n\
## fghead used for sections subsections etc\n\
\\fghead {}\n\
\\fi	\21\6 [***\10\\fi Badly Placed!]\n\
\\filbreak {}\n\
\\filedate #1{\6\060#1}\n\
# \6\040 = get next argument\n\
\\font	#0{\6\040\6\040\021\021}\n\
\\footnote #1{\\stepcounter{footnote}}\n\
\\footnotesize	\3-2\n\
\\frac	#2{#1/#2}\n\
\\gamma	\\{gamma\\}\n\
\\gdef	\6\1\n\
\\ge	>=\n\
\\geq	>=\n\
\\getargquoted	\33{\6\33\33}\n\
\\getbuF #1{\6\47#1}\n\
\\gg	>>\n\
\\global {}\n\
### Warning: Can't use \7\12 (SET1mode plain+HTML)\n\
\\gludata	#1{\33{\10<\10%\7\10\17g#1\10>\33}}\n\
\\glupop  #3{{{\7\10\10<A href=\"\10<\10&\6\072#2\10>\"\
 onClick=\"window.open(this.href,'\6\070#1','\6\071#1'); return false\"\10\
>}{\37\10#3}\10</A\10>\33}}\n\
\\glutag	#1{\33{\10<\10&\7\10\17g#1\10>\33}}\n\
\\goodbreak {}\n\
\\graph #1{\6\073#1}\n\
\\grave #1{\\`#1}\n\
\\halign #1{\\begin{halign}#1\\end{halign}}\n\
\\hang	{}\n\
\\hangafter #1{}\n\
\\hangindent #1{}\n\
\\hat	#1{\\^#1}\n\
# hbox has 1arg = length, 1arg=contents, remove MathMode\n\
\\hbox	#0{\33{\6\022l\6\022a\\SET1mode{0}#2\33}}\n\
\\hfil	{  }\n\
\\hfill	{    }\n\
\\hfuzz	{}\n\
\\hglue	{~}\n\
\\hline	\6\3\n\
\\hr	<HR>\n\
#\\href  #1{\6\17#1}\n\
\\hrule	#0{\6\040\10<HR\10>}\n\
\\hskip	#0{\6\040\\hspace{#1}}\n\
\\hspace #1{\\tag{SPACER TYPE=HORIZONTAL SIZE=\\length{#1}}}\n\
\\htarg #1{\33{\17%#1\33}}\n\
\\html	#1{\33{\37\002#1\33}}\n\
\\htval #1{\33{\17h#1\33}}\n\
\\hyphenation #1{}\n\
\\i	i\n\
\\if 	#2{\6\35#1#2}\n\
\\ifargop \6\061\n\
\\ifdef #1{\\ifx\\undefined#1\\else}\n\
\\iff	{\\verb+<==>+}\n\
\\iffalse \\ifnum0=1\n\
# The \21\6 acts as NOOP, but the \6 says it can't be removed\n\
\\ifhtx   \\ifnum1=1\21\6\n\
\\ifmatch  #2{\6\27#1#2}\n\
\\ifmmode \\ifmode{Mm}\n\
\\ifmode #1{\6\26#1}\n\
\\ifnum  #3{\6\25#1#2#3}\n\
\\ifplainbrowser \\ifnum\\HTMLversion<3\n\
\\ifregexp #2{\6\30#1#2}\n\
\\iftmode \\ifmode{T}\n\
\\iftrue  \\ifnum1=1\n\
\\ifundef #1{\\ifx\\undefined#1}\n\
\\ifx 	#2{\16\35#1#2}\n\
\\ignore #1{}\n\
#image is a single image, img has the 'alt' argument\n\
\\image	 #1{\33{\7\10\10<IMG SRC=\"\33{\17?#1\33}\"\33}\\ifmatch\
{}\\theimgopt\\else\33{\17t \\theimgopt\33}\\fi\10>}\n\
\\img    #2{\33{\7\10\10<IMG SRC=\"\33{\17?#1\33}\"\
 ALT=\"\33{\17h#2\33}\"\
 TITLE=\"\33{\17h#2\33}\"\33}\\ifmatch\
{}\\theimgopt\\else\33{\17t \\theimgopt\33}\\fi\10>}\n\
\\imgoptions #1{\\expand1arg{#1}\\def\\theimgopt{#1}}\n\
\\indent {\\hspace{2em}}\n\
#\\in_mode_: from here set to the specified mode\n\
\\inplain	\6\21q\n\
#\\input	#1{\6\6#1}\n\
\\input	\\GET1ARG\6\6 \n\
#\\inputhtml	#1{\\begin{html}\6\6#1\\end{html}}\n\
\\inputhtml \\GET1ARG\6\6H\n\
#\\inputverbatim #1{\\expand1arg{#1}\\begin{verbatim}\6\6#1\\end{verbatim}}\n\
\\inputverbatim	\\GET1ARG\\begin{verbatim}\6\6V\\end{verbatim}\n\
\\int	\\{integral\\}\n\
\\intersect \\{intersect\\}\n\
\\ionZ  #1{\6\065#1}\n\
\\iota	\\{iota\\}\n\
\\it	\2<I>\n\
\\item	<LI>\n\
\\j	{j}\n\
\\joinrel {}\n\
\\kappa	\\{kappa\\}\n\
\\kern	#1{}\n\
\\kill	\6\054\n\
\\ksi 	\\{xi\\}\n\
\\l	l\n\
\\label	#1{{\37\10\17t\10<A NAME=\"#1\"\10>\10</A\10>\33}}\n\
\\lambda \\{lambda\\}\n\
\\langle &lt;\n\
\\large	\3+1\n\
\\lay	#2{\33{\33{\7\10\10<DIV\
 onMouseOver=\"lay('\33{\17h\17`#1\33}')\"\
 onMouseOut=\"laz()\"\10>\33}\33{\37\10#2\33}\10</DIV\10>\33}}\n\
\\laycolor #1{\\tagged{SCRIPT}{laycolor=\"\\xcolor{#1}\"}}\n\
\\ldots	...\n\
\\le	<=\n\
\\leftarrow  \\verb+<-+\n\
\\leftrightarrow  \\verb+<->+\n\
\\leftskip #0{\6\040\021\021}\n\
\\length #1{\6\34#1}\n\
\\leq	<=\n\
\\lim	\\{lim\\}\n\
\\liminf \\{lim.inf\\}\n\
\\limits {}\n\
\\limsup \\{lim.sup\\}\n\
\\line	#1{#1\\}\n\
\\list	#1{\6\4#1}\n\
\\listofobjects \6\57\n\
\\ll	<<\n\
\\llap	#1{///#1}\n\
\\ln	{\\rm ln}\n\
\\lnot	\\{lnot\\}\n\
\\log	{\\rm log}\n\
\\long	{\\setcounter{long}{1}}\n\
\\longleftarrow  \\verb+<--+\n\
\\longleftrightarrow  \\verb+<-->+\n\
\\longrightarrow \\verb+-->+\n\
\\lowercase #1{{\17l#1}}\n\
\\lq `\n\
\\mS0	{\\setcounter{macrostar}{0}}\n\
\\maketitle {\\begin{center}{\\Huge\\bf\\getbuF{title}}\\par\\em\\Large\\getbuF{author}\\end{center}\\par}\n\
\\mapsto \\verb+-->+\n\
\\math	#1{{\\em #1}}\n\
\\max \\{max\\}\n\
\\mdash	\10-\10-\10-\n\
\\medskip	\\vspace{1ex}\n\
\\min \\{min\\}\n\
\\minus \10-\n\
\\mit {}\n\
\\mkern	#1{}\n\
\\mp	\\{-/+\\}\n\
\\mu	&#181;\n\
\\multicolumn	#2{\6\2#1#2}\n\
\\nabla	\\{nabla\\}\n\
\\ndash	\10-\10-\n\
\\ne	{!=}\n\
\\neg	&#172;\n\
\\neq	{!=}\n\
\\newbox #1{}\n\
\\newcommand	\6\1\n\
\\newcounter	#1{\6\042#1}\n\
\\newenvironment \6\032\n\
\\newline \\br\n\
\\newpage <P><P>\n\
\\nl	\10\n\n\
\\noalign {}\n\
\\nobreak {}\n\
\\noemptycell \6\16\n\
\\noindent {}\n\
\\nopagenumbers {}\n\
\\normalbottom {}\n\
\\normalsize	\3+0\n\
\\not	#1{\\{!#1\\}}\n\
\\not=	\\{!=\\}\n\
\\nu 	\\{nu\\}\n\
\\null	{}\n\
\\o	&oslash;\n\
\\obeylines 	\17o\n\
\\objA		#2{\10<\10&simq \"#1\" \"#2\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\objC		#2{\10<\10&smb.query \"#1\"\
|\"\33{\6\21\"#2 (read: #1)\33}\"\10>}\n\
\\objLEDA	#2{\10<\10&LEDA.query \"#1\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\objM		#2{\10<\10&smb.list \"#1\"\
|\"\33{\6\21\"#2 (read: #1)\33}\"\10>}\n\
\\objNED	#2{\10<\10&Nedquery.obj \"#1\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\objName	#1{\10<\10&smb.query \"Name #1\"|\"\33{\6\21\"#1\33}\"\10>}\n\
\\objP		#2{\10<\10&smb.query \"#1 #2\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\objS		#2{\10<\10&smb.query \"#1\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\obja		#2{\10<\10&alasimq \"#1\" \"#2\"|\"\33{\6\21\"#2\33}\"\10>}\n\
\\objac		#3{\10<\10&simq \"#1\" \"#2\" \"#3\"\
|\"\33{\6\21\"#2\33}\"\10>}\n\
#\\object	#1{\10<\\%I #1\10>}\n\
#\\object	#1{\10<\10&smb.query \"#1\"\10>}\n\
\\object	#1{<&smb.query \"#1\">}\n\
\\objectname	#0{\10<\\%I \\ifargop\6\022-o\6\022-a\"#1\"\
|\"\33{\6\21\"#2\33}\"\\else \6\022-a#1\\fi\10>}\n\
\\objnS		#1{{\\fg{red4}#1}}\n\
\\odot	\\{sun\\}\n\
#\\oe	oe\n\
\\oe	&#339;\n\
\\oint	\\{Ointegral\\}\n\
\\omega	\\{omega\\}\n\
\\omicron \\{omicron\\}\n\
\\omit	{}\n\
\\opentag \5=\n\
\\oplain #1{#1}\n\
\\oplus  \\{oplus\\}\n\
\\option #1{\10<OPTION\10>#1}\n\
## Ordinal male/female\n\
\\ordf &#170;\n\
\\ordm &#186;\n\
\\oslash \\o\n\
\\otimes \\{otimes\\}\n\
\\over	{ / }\n\
\\overline #1{\\=#1}\n\
\\overtext #2{\10<span title=\"\33{\17h#1\33}\"\10>#2\10</span\10>}\n\
\\pageno \\thepage\n\
\\par	<P>\n\
\\para	&#182;\n\
\\parallel ||\n\
\\parskip #0{\6\040\021\021}\n\
\\partial \\{d\\}\n\
#\\partial &#240;\n\
\\permil &#8240;\n\
\\perp	\\{perp\\}\n\
\\phi	\\{phi\\}\n\
\\pi 	\\{pi\\}\n\
\\plain	#1{\33{\7\10#1\33}}\n\
\\plainbrowser 	\6\14\n\
\\pm	&#177;\n\
\\pound &#163;\n\
\\pounds &#163;\n\
\\prime	&#180;\n\
\\prod	\\Pi\n\
\\propto \\{prop.to\\}\n\
\\protect {}\n\
\\psi	\\{psi\\}\n\
\\putenv #1{\6\051#1}\n\
\\qquad	 &#160;&#160;&#160;&#160;\n\
\\quad	 &#160;&#160;\n\
\\quit	\377\377\n\
\\quote #1{\10\"\33{\17h#1\33}\10\"}\n\
\\raggedbottom {}\n\
\\raggedleft {}\n\
\\raggedright {}\n\
\\raise	#1{}\n\
\\rangle &gt;\n\
\\ref	#1{{\37\10\10<A HREF=\"\\\014#1\"\10>#1\10</A\10>\33}}\n\
\\relax {}\n\
\\renewcommand	\6\1\n\
\\resetAlarm	\6\62\n\
\\rf	\2\n\
\\rho 	\\{rho\\}\n\
\\rightarrow  \\verb+->+\n\
\\rightskip #0{\6\040\021\021}\n\
\\rlap	{}\n\
\\rm	\2\n\
\\roman	#1{\\edicnt{r#1}}\n\
\\rowoptions #1{\\expand1arg{#1}\\def\\therowopt{#1}}\n\
\\savebuF #2{\6\46#1#2}\n\
#\\sc	\2<B>\n\
\\sc	\\SET1mode{u}\n\
\\scriptsize \3-3\n\
\\sec	{\\rm sec}\n\
\\sect	\\S\n\
\\section #1{{\\fghead\\label{\\ToClabel}\\x0A\
\10<H2\10>\\ifnum\\themacrostar=0\\addtocounter{*3}{1}\\edicnt{*+3}\\quad\\fi\
#1\10</H2\10>}}\n\
\\sed	#2{\16\50#1#2}\n\
\\select #2{\10<OPTION value=\"#1\"\10>#2\10</OPTION\10>}\n\
\\set1buf #2{\6\7#1#2}\n\
\\setcounter #2{\\addtocounter{#1}{=#2}}\n\
\\sf	#0{\3\033{\17tFACE='sans-serif'\10>\033}}\n\
\\sigma \\{sigma\\}\n\
\\sim	\10~\n\
\\sime	=\10~\n\
\\simeq	=\10~\n\
\\sin	{\\rm sin}\n\
\\sinh	{\\rm sinh}\n\
\\sl	\\it\n\
\\small	\3-1\n\
\\smallskip \\vspace{3}\n\
\\soft	#1{{\37\200#1\33}}\n\
\\sout  #1{\\tagged{S}{#1}}\n\
\\special #1{\\begin{verbatim}\16\37#1\\end{verbatim}}\n\
\\specialexpanded #1{\\begin{verbatim}\6\37#1\\end{verbatim}}\n\
\\sqrt	#1{sqrt(#1)}\n\
\\ss	&szlig;\n\
\\star	*\n\
\\stepcounter #1{\\addtocounter{#1}{1}}\n\
\\strike #1{\\tagged{S}{#1}}\n\
\\strut	{}\n\
\\sub	#1{$_{#1}$}\n\
\\subsection	#1{{\\fghead\\label{\\ToClabel}\\x0A\
\10<H3\10>\\ifnum\\themacrostar=0\\addtocounter{*4}{1}\\edicnt{*+4}\\quad\\fi\
#1\10</H3\10>}}\n\
\\subset \\{in\\}\n\
\\subseteq \\{in\\}\n\
\\subsubsection	#1{{\\fghead\\label{\\ToClabel}\\x0A\
\10<H4\10>\\ifnum\\themacrostar=0\\addtocounter{*5}{1}\\edicnt{*+5}\\quad\\fi\
#1\10</H4\10>\\mS0}}\n\
\\sum	\\{Sum\\}\n\
\\sun	\\odot\n\
\\sup	#1{$^{#1}$}\n\
\\supset \\{includes\\}\n\
\\sz	sz\n\
\\tableofcontents \6\045\n\
\\taboptions #1{\\expand1arg{#1}\\def\\thetabopt{#1}}\n\
#\\taboptions {}\n\
\\tag	#1{\33{\7\50\10<#1\10>\33}}\n\
#\\tagged #2{\10<#1\10>#2\10</#1\10>}\n\
\\tagged #2{\\tag{#1}#2\\tag{/#1}}\n\
\\tagobj  #1{{\\it\\object{#1}}}\n\
\\tagobjA #2{{\\it\\objA{#1}{#2}}}\n\
\\tagobjC #2{{\\it\\objC{#1}{#2}}}\n\
\\tagobjNA #1{{\\it\\tagobj{#1}}}\n\
\\tagobjS #2{{\\it\\objS{#1}{#2}}}\n\
\\tagobjac #3{{\\it\\objac{#1}{#2}{#3}}}\n\
\\tagobjnS #1{{\\it\\objnS{#1}}}\n\
\\tan	{\\rm tan}\n\
\\tanh	{\\rm tanh}\n\
\\tau	\\{tau\\}\n\
\\term	#1{\10<DT\10>#1\10<DD\10>}\n\
\\textbf #1{{\\bf #1}}\n\
\\textem #1{{\\em #1}}\n\
\\textit #1{{\\it #1}}\n\
\\textrm #1{{\\rm #1}}\n\
\\textsf #1{{\\sf #1}}\n\
\\textsl #1{{\\sl #1}}\n\
\\texttt #1{{\\tt #1}}\n\
\\theToC	\\edicnt{+0}\n\
\\thechapter	\\edicnt{+2}\n\
\\theequation	\\edicnt{+9}\n\
\\thefigure	\\edicnt{+8}\n\
#\\thefile \6\7\n\
\\thefile \\ed1buf{1}\n\
\\thefootnote	\\edicnt{+6}\n\
\\thehead	\\edicnt{+16}\n\
\\thelong	\\edicnt{+15}\n\
\\themacrostar	\\edicnt{+11}\n\
\\thepage	\\edicnt{+10}\n\
\\thepart	\\edicnt{+1}\n\
\\theprogram \\ed1buf{0}\n\
\\thesecnumdepth \\edicnt{+12}\n\
\\thesection	\\edicnt{*+3}\n\
\\thesubsection	\\edicnt{*+4}\n\
\\thesubsubsection	\\edicnt{*+5}\n\
\\theta	\\{theta\\}\n\
\\thetable	\\edicnt{+7}\n\
\\thickmuskip	\\;\n\
\\thickrule <HR NOSHADE>\n\
\\thinmuskip	\\,\n\
\\thinspace	{}\n\
\\thisrowoptions #1{\\expand1arg{#1}\\set1buf{05}{#1}}\n\
#\\tilde	\10~\n\
\\tilde	&#8764;\n\
\\times	&#215;\n\
\\tiny	\3-3\n\
\\title	#1{\\savebuF{title}{#1}\\tagged{TITLE}{#1}}\n\
\\to	\\rightarrow\n\
\\today \\date{}\n\
\\top	\\{T\\}\n\
\\triangle \\Delta\n\
\\tt	\2<TT>\n\
\\typeToC	#1{\6\044#1}\n\
\\typein	#1{\6\023#1}\n\
\\u	\1u\n\
\\undefined  \6\067\n\
\\underline  #1{\\tagged{U}{#1}}\n\
\\unicode    #1{\6\066#1}\n\
\\uparrow   \\{up\\}\n\
\\updownarrow  \\{updown\\}\n\
\\uppercase  #1{{\17u#1}}\n\
\\upsilon \\{upsilon\\}\n\
\\v	\1v\n\
\\vMozilla	{}\n#Space4Change\n\
### varGreek are just set to their standard designation\n\
\\varepsilon	\\epsilon\n\
\\varphi	\\phi\n\
\\varpi		\\pi\n\
\\varrho	\\rho\n\
\\varsigma	\\sigma\n\
\\vartheta	\\theta\n\
\\vbox	{}\n\
#\\vec	#1{\\{vec\\}#1}\n\
\\vec	#1{{\\bf\\underline{#1}}}\n\
\\verb	\33{\7\30\6\33\33}\n\
\\verbatim  #1{\33{\7\30\10<TT\10>#1\10</TT\10>\33}}\n\
\\vert	|\n\
\\vfil	{\\br\\br}\n\
\\vfill	{\\par\\par}\n\
\\vglue #1{}\n\
\\vrule	 #0{\6\040\021\021}\n\
\\vskip	 #0{\6\040\\vspace{#1}}\n\
\\vspace #1{\10<SPACER TYPE=VERTICAL SIZE=\\length{#1}\10>}\n\
\\wHead	#1{\\ifmatch{*/[Ww]/*}{\\thefile}\\winHead{{\\Large\\bf #1}}\\else\
 \\centerline{\10<H1\10>#1\10</H1\10>}\\thickrule\\fi}\n\
\\wPage	#1{\\title{#1}\\x0A\\wHead{#1}}\n\
\\whenplainbrowser #1{\\ifplainbrowser #1\\fi}\n\
\\winHead #1{\\Beg{TABULAR}{CELLSPACING=0 CELLPADDING=8}{pr}#1&\
 {[\\A{javascript:window.history.back()}{Back}]} \\cdot{ }\
 {[\\A{javascript:window.history.forward()}{Forwd}]} \\cdot{ }\
 {[\\A{javascript:if (window.print) window.print()}{Print}]} \\cdot{ }\
 {[\\A{javascript:window.close()}{Close}]}\
 \\end{TABULAR}\\thickrule}\n\
\\xcolor #1{\6\036#1}\n\
\\xi 	\\{xi\\}\n\
\\yA	#3{{{\7\10\10<A HREF=\"\33{\17?#2\33}\"\
 TITLE=\"\33{\17h\17`#1\33}\"\10>}{\37\10#3}\10</A\10>\33}}\n\
\\yAref	#3{{\37\10\10<A HREF=\"\\\014#2\"\
 TITLE=\"\33{\17h\17`#1\33}\"\10>#3\10</A\10>\33}}\n\
\\yArefx #4{{\37\10\10<A HREF=\"\33{\17?#2\33}\17##3\"\
 TITLE=\"\33{\17h\17`#1\33}\"\10>#4\10</A\10>\33}}\n\
\\yW	#4{{{\7\10\10<A TARGET=\"#2\" HREF=\"\33{\17?#3\33}\"\
 TITLE=\"\33{\17h\17`#1\33}\"\10>}{\37\10#4}\10</A\10>\33}}\n\
\\yen	&#165;\n\
\\zeta	\\{zeta\\}\n\
\\{	\10{\n\
\\}	\10}\n\
\\~~	\\asymp\n\
###### TeX environments (mode introduced by \7)\n\
###### (default is to generate the tag <environment>)\n\
###### Remember, the \21\6 (=NOOP) indicates NO OPTIONAL ARGUMENT in [..]\n\
######           and \21\6\6 also indicates not not open table cells.\n\
{HTML}	\37\2\n\
{JS}	{JavaScript}\n\
{JavaScript}	\7\2\37\010SCRIPT LANGUAGE=\"JavaScript\"\n\
#{JavaScript}	#0{\7\2\37\010<SCRIPT LANGUAGE=\"JavaScript\">}{</SCRIPT>}\n\
#{PRE}	\37\1PRE\n\
{PRE}	#0{\\SET1mode{P}\10<PRE\10>}{\10</PRE\10>}\n\
{TABLESORT}	#1\7\4TABLE class='sort'\n\
{TABULAR}	#1\7\4TABLE WIDTH=\"100%\"\n\
### The following is a problem, it forbids \\begin{TR option}\n\
#{TR}		#0{\21\6\06\056o}{\06\056c}\n\
{TeX}		#0{\21\6\\SETmode4TeX}{}\n\
{alltt}		{PRE}\n\
{alltty}	{PRE}\n\
{array}		#1\7\4TABLE\n\
{document}	BODY\n\
{enumerate}	OL\n\
{eqnarray}	#1\7\4TABLE\n\
{glossary}	DL\n\
{halign}	#0{\10<TABLE BORDER=0\10>\
 \\x0A\\def&{\\SEP1}\6\052L\\def\\cr{\6\053>1}}\
 {\6\052-\10</TABLE\10>}\n\
###{html}	\21\6\7\2\n\
{html}	\37\2\n\
{ignore}	#0{\\SET1mode{-}}{}\n\
{include}	#0{\\SET1mode{0}}{}\n\
{itemize}	UL\n\
{javascript}	{JavaScript}\n\
{list}		#2UL\n\
{picture} #0{\\\\[TeX picture ignored]\\\\\\begin{ignore}}{\\end{ignore}}\n\
#{plain}	\21\6\37\10\n\
{plain}		#0{\\SET1mode{q}}{}\n\
{pre}		{PRE}\n\
{preformatted}	{PRE}\n\
{quote}		BLOCKQUOTE\n\
{row}		#0{\21\6\06\056o}{\06\056c}\n\
{soft}		\21\6\37\200\n\
{style}		\7\2\37\010style\n\
{tabbing}	#0{\10<TABLE BORDER=0\10>\
 \\x0A\\def\\>{\\SEP1l}\6\052L\\def\\\\{\6\053}}\
 {\6\052-\10</TABLE\10>}\n\
{tabular}	#1\7\4TABLE\n\
{tabular*}	#2\7\4TABLE\n\
{tex}		#0{\21\6\\SETmode4TeX}{}\n\
{thebibliography} #1{\\section{References}\\begin{UL}}{\\end{UL}}\n\
#{verbatim}	\7\30LISTING\n\
{verbatim}	\21\6\7\30PRE\n\
###### valid HTML tags\n\
<![^>]*>	# HTML comment ou DOCUMENT\n\
</[A-Z]*[0-9]*>\n\
</[a-z]*[0-9]*>\n\
<[A-Z]* [^>]*>	# Tag with parameter\n\
<[A-Z][A-Z]*[0-9]*>\n\
<[a-z]* [^>]*>	# Tag with parameter\n\
<[a-z][a-z]*[0-9]*>\n\
###### cgi-tex enviromnents -- . just to start new set\n\
!begin	.\n\
!cat	!/bin/cat\n\
!end	\377\n\
!html	\7\2\n\
#!plain	\7\30LISTING\n\
!plain	\7\30PRE\n\
!pre	PRE\n\
!tex	\0170\n\
###### Equivalences of length (into pixels)\n\
*\\hsize 420\n\
#*\\textwidth 420\n\
*\\textwidth 100%\n\
*cm	30\n\
*em 	12\n\
*ex 	8\n\
*in	76\n\
*mm	3\n\
*pt	1\n\
*true   0\n\
*truecm 30\n\
*truept 1\n\
###### Counters : macrostar=11 (set if macro contains the *)\n\
+ToC	0\n\
+chapter	2\n\
+buffer	13\n\
+equation	9\n\
+figure	8\n\
+footnote	6\n\
+head		16\n\
+long		15\n\
+macrostar	11\n\
+tabline	13\n\
+page	10\n\
+part	1\n\
+secnumdepth	12\n\
+section	3\n\
+subsection	4\n\
+subsubsection	5\n\
+table	7\n\
######################## End of Definitions ######\n\
";

static char unicode_symb[] = "\
#### Unicode symbols (first byte set to 0 when loaded)\n\
\\,	&thinsp;&#8201;\n\
\\Alpha &Alpha;&#913;\n\
\\Beta &Beta;&#914;\n\
\\Box &square;&#x25A1;\n\
\\Chi &Chi;&#935;\n\
\\Dagger &Dagger;&#8225;\n\
\\Delta &Delta;&#916;\n\
\\Downarrow &Downarrow;&#x21D3;\n\
\\ETH &ETH;&#208;\n\
\\Earth &oplus;&#8853;\n\
\\Epsilon &Epsilon;&#917;\n\
\\Eta &Eta;&#919;\n\
\\Gamma &Gamma;&#915;\n\
\\Iota &Iota;&#921;\n\
\\Join &Join;&#8904;\n\
\\Jupiter &Jupiter;&#9795;\n\
\\Kappa &Kappa;&#922;\n\
\\Lambda &Lambda;&#923;\n\
\\Leftarrow &Leftarrow;&#x21D0;\n\
\\Leftrightarrow &Leftrightarrow;&#x21D4;\n\
\\Longleftarrow &Longleftarrow;&#x27F8;\n\
\\Longleftrightarrow &Longleftrightarrow;&#x27FA;\n\
\\Longrightarrow &Longrightarrow;&#x27F9;\n\
\\Mars &Mars;&#9794;\n\
\\Mercury &Mercury;&#9791;\n\
\\Moon &Moon;&#9789;\n\
\\Mu &Mu;&#924;\n\
\\Neptune &Neptune;&#9798;\n\
\\Nu &Nu;&#925;\n\
\\OE    &OElig;&#338;\n\
\\OElig &OElig;&#338;\n\
\\Omega &Omega;&#937;\n\
\\Omicron &Omicron;&#927;\n\
\\Phi &Phi;&#934;\n\
\\Pi &Pi;&#928;\n\
#\\Prime &Prime;&#8243;\n\
\\Psi &Psi;&#936;\n\
\\Rho &Rho;&#929;\n\
\\Rightarrow; &Rightarrow;&#x21D2;\n\
\\Saturn &Saturn;&#9796;\n\
#\\Scaron &Scaron;&#352;\n\
\\Sigma &Sigma;&#931;\n\
\\Sun &odot;&#x2609;\n\
\\THORN &THORN;&#222;\n\
\\Tau &Tau;&#932;\n\
\\Theta &Theta;&#920;\n\
\\Uparrow &Uparrow;&#x21D1;\n\
\\Updownarrow &Updownarrow;&#x21D5;\n\
\\Upsilon &Upsilon;&#933;\n\
\\Uranus &Uranus;&#9797;\n\
\\Venus &Venus;&#9792;\n\
\\Xi &Xi;&#926;\n\
\\Zeta &Zeta;&#918;\n\
\\alefsym &alefsym;&#8501;\n\
\\aleph &alefsym;&#8501;\n\
\\alpha &alpha;&#945;\n\
\\and &and;&#8743;\n\
\\ang &ang;&#8736;\n\
\\asymp &asymp;&#8776;\n\
\\bdquo &bdquo;&#8222;\n\
\\beta &beta;&#946;\n\
\\blackcircle &filcircle;&#x25CF;\n\
\\blackdiamond &fildiamond;&#x25C6;\n\
\\blacknabla &filnabla;&#x25BC;\n\
\\blacksquare &filsquare;&#x25A0;\n\
\\blacktriangle &filtriangle;&#x25B2;\n\
\\blacktriangleleft &filltriangle;&#x25C0;\n\
\\blacktriangleright &filrtriangle;&#x25B6;\n\
\\brvbar &brvbar;&#166;\n\
\\bull &bull;&#8226;\n\
\\cap &cap;&#8745;\n\
#\\cedil &cedil;&#184;\n\
#\\cent &cent;&#162;\n\
\\chi &chi;&#967;\n\
\\circ &circ;&#x25CB;\n\
\\clubs &clubs;&#9827;\n\
\\cong &cong;&#8773;\n\
\\crarr &crarr;&#8629;\n\
\\cup &cup;&#8746;\n\
\\curren &curren;&#164;\n\
\\dArr &dArr;&#8659;\n\
\\dag    &dagger;&#8224;\n\
\\dagger &dagger;&#8224;\n\
\\darr &darr;&#8595;\n\
\\delta &delta;&#948;\n\
\\diamond &diamond;&#x25C7;\n\
\\diams &diams;&#9830;\n\
\\downarrow &downarrow;&#x2193;\n\
\\empty &empty;&#8709;\n\
\\emsp &emsp;&#8195;\n\
\\ensp &ensp;&#8194;\n\
\\epsilon &epsilon;&#949;\n\
\\equiv &equiv;&#8801;\n\
\\eta &eta;&#951;\n\
\\eth &eth;&#240;\n\
\\euro &euro;&#8364;\n\
\\exist &exist;&#8707;\n\
\\exists &exist;&#8707;\n\
\\fnof &fnof;&#402;\n\
\\forall &forall;&#8704;\n\
#\\frac12 &frac12;&#189;\n\
#\\frac14 &frac14;&#188;\n\
#\\frac34 &frac34;&#190;\n\
\\frasl &frasl;&#8260;\n\
\\ga &gabout;&#8819;\n\
\\gabout &gabout;&#8819;\n\
\\gamma &gamma;&#947;\n\
\\ge &ge;&#8805;\n\
\\gg &muchgt;&#8811;\n\
\\gsim &gabout;&#8819;\n\
\\hArr &hArr;&#8660;\n\
\\harr &harr;&#8596;\n\
\\hearts &hearts;&#9829;\n\
\\hellip &hellip;&#8230;\n\
\\iexcl &iexcl;&#161;\n\
\\iiint &iiint;&#8749;\n\
\\iint &iint;&#8748;\n\
#\\image &image;&#8465;\n\
\\in &isin;&#8712;\n\
\\infin &infin;&#8734;\n\
\\infinity &infin;&#8734;\n\
\\infty &infin;&#8734;\n\
\\int &int;&#8747;\n\
\\intersect &cap;&#8745;\n\
\\iota &iota;&#953;\n\
\\iquest &iquest;&#191;\n\
\\isin &isin;&#8712;\n\
\\kappa &kappa;&#954;\n\
\\ksi &xi;&#958;\n\
\\lArr &lArr;&#8656;\n\
\\la &labout;&#8818;\n\
\\labout &labout;&#8818;\n\
\\lambda &lambda;&#955;\n\
\\lang &lang;&#9001;\n\
\\laquo &laquo;&#171;\n\
\\larr &larr;&#8592;\n\
\\lceil &lceil;&#8968;\n\
\\ldquo &ldquo;&#8220;\n\
\\le &le;&#8804;\n\
\\leftarrow &leftarrow;&#x2190;\n\
\\leftrightarrow &leftrightarrow;&#x2194;\n\
\\lfloor &lfloor;&#8970;\n\
\\ll &muchlt;&#8810;\n\
\\lnot &not;&#172;\n\
\\longleftarrow &longleftarrow;&#x27F5;\n\
\\longleftrightarrow &longleftrightarrow;&#x27F7;\n\
\\longrightarrow &longrightarrow;&#x27F6;\n\
\\lowast &lowast;&#8727;\n\
\\loz &loz;&#9674;\n\
\\lrm &lrm;&#8206;\n\
\\lsaquo &lsaquo;&#8249;\n\
\\lsim &labout;&#8818;\n\
\\lsquo &lsquo;&#8216;\n\
\\macr &macr;&#175;\n\
\\mdash &mdash;&#8212;\n\
\\middot &middot;&#183;\n\
\\minus &minus;&#8722;\n\
\\mp &mp;&#x2213;\n\
#\\mu &mu;&#956;\n\
\\nabla &nabla;&#8711;\n\
\\nbsp &nbsp;&#160;\n\
\\ndash &ndash;&#8211;\n\
\\ne &ne;&#8800;\n\
\\neg &not;&#172;\n\
\\neq &ne;&#x2260;\n\
\\ni &ni;&#8715;\n\
\\not #1{#1\10&\10#x0338;}\n\
\\not= &ne;&#8800;\n\
\\notin &notin;&#8713;\n\
\\nsub &nsub;&#8836;\n\
\\nu &nu;&#957;\n\
\\odot  &odot;&#x2609;\n\
\\oe    &oelig;&#339;\n\
\\oelig &oelig;&#339;\n\
\\oiint &oiint;&#8751;\n\
\\oint &oint;&#8750;\n\
\\oline &oline;&#8254;\n\
\\omega &omega;&#969;\n\
\\omicron &omicron;&#959;\n\
\\oplus &oplus;&#8853;\n\
\\or &or;&#8744;\n\
\\ordf &ordf;&#170;\n\
\\ordm &ordm;&#186;\n\
\\otimes &otimes;&#8855;\n\
\\part &part;&#8706;\n\
\\partial &partial;&#8706;\n\
\\permil &permil;&#8240;\n\
\\perp &perp;&#8869;\n\
\\phi &phi;&#966;\n\
\\pi &pi;&#960;\n\
\\pm &pm;&#177;\n\
#\\pounds &pound;&#163;\n\
#\\pound &pound;&#163;\n\
\\prime &prime;&#8242;\n\
\\prod &prod;&#8719;\n\
\\prop &prop;&#8733;\n\
\\propto &prop;&#8733;\n\
\\psi &psi;&#968;\n\
\\rArr &rArr;&#8658;\n\
\\radic &radic;&#8730;\n\
\\rang &rang;&#9002;\n\
\\raquo &raquo;&#187;\n\
\\rarr &rarr;&#8594;\n\
\\rceil &rceil;&#8969;\n\
\\rdquo &rdquo;&#8221;\n\
\\real &real;&#8476;\n\
\\reg &reg;&#174;\n\
\\rfloor &rfloor;&#8971;\n\
\\rho &rho;&#961;\n\
\\rightarrow &rightarrow;&#x2192;\n\
\\rlm &rlm;&#8207;\n\
\\rsaquo &rsaquo;&#8250;\n\
\\rsquo &rsquo;&#8217;\n\
\\sbquo &sbquo;&#8218;\n\
#\\scaron &scaron;&#353;\n\
\\sdot &sdot;&#8901;\n\
#\\sect &sect;&#167;\n\
\\shy &shy;&#173;\n\
\\sigma &sigma;&#963;\n\
\\sigmaf &sigmaf;&#962;\n\
\\sim &sim;&#8764;\n\
\\simeq &simeq;&#8771;\n\
\\spades &spades;&#9824;\n\
\\square &square;&#x25A1;\n\
#\\sub &sub;&#8834;\n\
#\\sube &sube;&#8838;\n\
\\subset &sub;&#8834;\n\
\\subseteq &sube;&#8838;\n\
\\sum &sum;&#8721;\n\
\\sun &odot;&#x2609;\n\
#\\sup &sup;&#8835;\n\
#\\sup1 &sup1;&#185;\n\
#\\sup2 &sup2;&#178;\n\
#\\sup3 &sup3;&#179;\n\
\\supe &supe;&#8839;\n\
\\sz    &szlig;&#223;\n\
\\szlig &szlig;&#223;\n\
\\tau &tau;&#964;\n\
#\\there4 &there4;&#8756;\n\
\\theta &theta;&#952;\n\
#\\thinsp &thinsp;&#8201;\n\
\\thorn &thorn;&#254;\n\
\\times &times;&#215;\n\
\\trade &trade;&#8482;\n\
\\triangle &triangle;&#x25B3;\n\
\\triangleleft &ltriangle;&#x25C1;\n\
\\triangleright &rtriangle;&#x25B7;\n\
\\uArr &uArr;&#8657;\n\
\\uarr &uarr;&#8593;\n\
\\union &cup;&#8746;\n\
\\uparrow &uparrow;&#x2191;\n\
\\updownarrow &updownarrow;&#x2195;\n\
\\upsih &upsih;&#978;\n\
\\upsilon &upsilon;&#965;\n\
\\varphi &phisymb;&#981;\n\
\\varpi &piv;&#982;\n\
\\vartheta &thetasym;&#977;\n\
\\weierp &weierp;&#8472;\n\
\\xi &xi;&#958;\n\
\\zeta &zeta;&#950;\n\
\\zwj &zwj;&#8205;\n\
\\zwnj &zwnj;&#8204;\n\
";

/* Symbols used for graph ((Zapf Dingbats)) */
static unsigned short graph_unicode[256] = {
/* From ftp://ftp.unicode.org/Public/MAPPINGS/VENDORS/ADOBE/zdingbat.txt
 *   for code>=32; otherwise GRAPH character
 * Unicode / Graph number / Explain / Postscript number */
  0x0000, /* #00 = 0  */
  0x22C5, /* #01 = 1  = dot  */
     '+', /* #02 = 2  = plus */
  0x2217, /* #03 = 3  = asterisk */
  0x25CB, /* #04 = 4  = circle */
  0x00D7, /* #05 = 5  = cross */
  0x25A1, /* #06 = 6  = square */
  0x25B3, /* #07 = 7  = triangle */
  0x25C7, /* #08 = 8  = diamond */
  0x2605, /* #09 = 9  = star (0x2217 a bit too small) */
  0x2207, /* #0a = 10 = nabla */
  0x273A, /* #0b = 11 = starburst */
  0x271A, /* #0c = 12 = fancy plus */
  0x2716, /* #0d = 13 = fancy cross */
  0x25B1, /* #0e = 14 = fancy square */
  0x25CA, /* #0f = 15 = fancy diamond */
  0x25CF, /* #10 = 16 = filled circle */
  0x25A0, /* #11 = 17 = filled square */
  0x25B2, /* #12 = 18 = filled triangle */
  0x25C6, /* #13 = 19 = filled diamond */
  0x25BC, /* #14 = 20 = filled nabla */
  0x25B0, /* #15 = 21 = filled fancy square */
  0x2666, /* #16 = 22 = filled fancy diamond */
  0x2609, /* #17 = 23 = half filled circle (U25D2 is half) */
  0x25A3, /* #18 = 24 = half filled square */
  0x21A5, /* #19 = 25 = half filled triangle (upward arrow) */
  0x25C8, /* #1a = 26 = half filled diamond */
  0x21A7, /* #1b = 27 = half filled nabla (downward arrow) */
  0x25A8, /* #1c = 28 = half filled fancy square */
  0x25CD, /* #1d = 29 = half filled fancy diamond */
  0x2B21, /* #1e = 30 = octagon */
  0x2B22, /* #1f = 31 = filled octagon */
  0x0020, /* #20 # SPACE	# space*/
  0x2701, /* #21 # UPPER BLADE SCISSORS	# a1*/
  0x2702, /* #22 # BLACK SCISSORS	# a2*/
  0x2703, /* #23 # LOWER BLADE SCISSORS	# a202*/
  0x2704, /* #24 # WHITE SCISSORS	# a3*/
  0x260E, /* #25 # BLACK TELEPHONE	# a4*/
  0x2706, /* #26 # TELEPHONE LOCATION SIGN	# a5*/
  0x2707, /* #27 # TAPE DRIVE	# a119*/
  0x2708, /* #28 # AIRPLANE	# a118*/
  0x2709, /* #29 # ENVELOPE	# a117*/
  0x261B, /* #2A # BLACK RIGHT POINTING INDEX	# a11*/
  0x261E, /* #2B # WHITE RIGHT POINTING INDEX	# a12*/
  0x270C, /* #2C # VICTORY HAND	# a13*/
  0x270D, /* #2D # WRITING HAND	# a14*/
  0x270E, /* #2E # LOWER RIGHT PENCIL	# a15*/
  0x270F, /* #2F # PENCIL	# a16*/
  0x2710, /* #30 # UPPER RIGHT PENCIL	# a105*/
  0x2711, /* #31 # WHITE NIB	# a17*/
  0x2712, /* #32 # BLACK NIB	# a18*/
  0x2713, /* #33 # CHECK MARK	# a19*/
  0x2714, /* #34 # HEAVY CHECK MARK	# a20*/
  0x2715, /* #35 # MULTIPLICATION X	# a21*/
  0x2716, /* #36 # HEAVY MULTIPLICATION X	# a22*/
  0x2717, /* #37 # BALLOT X	# a23*/
  0x2718, /* #38 # HEAVY BALLOT X	# a24*/
  0x2719, /* #39 # OUTLINED GREEK CROSS	# a25*/
  0x271A, /* #3A # HEAVY GREEK CROSS	# a26*/
  0x271B, /* #3B # OPEN CENTRE CROSS	# a27*/
  0x271C, /* #3C # HEAVY OPEN CENTRE CROSS	# a28*/
  0x271D, /* #3D # LATIN CROSS	# a6*/
  0x271E, /* #3E # SHADOWED WHITE LATIN CROSS	# a7*/
  0x271F, /* #3F # OUTLINED LATIN CROSS	# a8*/
  0x2720, /* #40 # MALTESE CROSS	# a9*/
  0x2721, /* #41 # STAR OF DAVID	# a10*/
  0x2722, /* #42 # FOUR TEARDROP-SPOKED ASTERISK	# a29*/
  0x2723, /* #43 # FOUR BALLOON-SPOKED ASTERISK	# a30*/
  0x2724, /* #44 # HEAVY FOUR BALLOON-SPOKED ASTERISK	# a31*/
  0x2725, /* #45 # FOUR CLUB-SPOKED ASTERISK	# a32*/
  0x2726, /* #46 # BLACK FOUR POINTED STAR	# a33*/
  0x2727, /* #47 # WHITE FOUR POINTED STAR	# a34*/
  0x2605, /* #48 # BLACK STAR	# a35*/
  0x2729, /* #49 # STRESS OUTLINED WHITE STAR	# a36*/
  0x272A, /* #4A # CIRCLED WHITE STAR	# a37*/
  0x272B, /* #4B # OPEN CENTRE BLACK STAR	# a38*/
  0x272C, /* #4C # BLACK CENTRE WHITE STAR	# a39*/
  0x272D, /* #4D # OUTLINED BLACK STAR	# a40*/
  0x272E, /* #4E # HEAVY OUTLINED BLACK STAR	# a41*/
  0x272F, /* #4F # PINWHEEL STAR	# a42*/
  0x2730, /* #50 # SHADOWED WHITE STAR	# a43*/
  0x2731, /* #51 # HEAVY ASTERISK	# a44*/
  0x2732, /* #52 # OPEN CENTRE ASTERISK	# a45*/
  0x2733, /* #53 # EIGHT SPOKED ASTERISK	# a46*/
  0x2734, /* #54 # EIGHT POINTED BLACK STAR	# a47*/
  0x2735, /* #55 # EIGHT POINTED PINWHEEL STAR	# a48*/
  0x2736, /* #56 # SIX POINTED BLACK STAR	# a49*/
  0x2737, /* #57 # EIGHT POINTED RECTILINEAR BLACK STAR	# a50*/
  0x2738, /* #58 # HEAVY EIGHT POINTED RECTILINEAR BLACK STAR	# a51*/
  0x2739, /* #59 # TWELVE POINTED BLACK STAR	# a52*/
  0x273A, /* #5A # SIXTEEN POINTED ASTERISK	# a53*/
  0x273B, /* #5B # TEARDROP-SPOKED ASTERISK	# a54*/
  0x273C, /* #5C # OPEN CENTRE TEARDROP-SPOKED ASTERISK	# a55*/
  0x273D, /* #5D # HEAVY TEARDROP-SPOKED ASTERISK	# a56*/
  0x273E, /* #5E # SIX PETALLED BLACK AND WHITE FLORETTE	# a57*/
  0x273F, /* #5F # BLACK FLORETTE	# a58*/
  0x2740, /* #60 # WHITE FLORETTE	# a59*/
  0x2741, /* #61 # EIGHT PETALLED OUTLINED BLACK FLORETTE	# a60*/
  0x2742, /* #62 # CIRCLED OPEN CENTRE EIGHT POINTED STAR	# a61*/
  0x2743, /* #63 # HEAVY TEARDROP-SPOKED PINWHEEL ASTERISK	# a62*/
  0x2744, /* #64 # SNOWFLAKE	# a63*/
  0x2745, /* #65 # TIGHT TRIFOLIATE SNOWFLAKE	# a64*/
  0x2746, /* #66 # HEAVY CHEVRON SNOWFLAKE	# a65*/
  0x2747, /* #67 # SPARKLE	# a66*/
  0x2748, /* #68 # HEAVY SPARKLE	# a67*/
  0x2749, /* #69 # BALLOON-SPOKED ASTERISK	# a68*/
  0x274A, /* #6A # EIGHT TEARDROP-SPOKED PROPELLER ASTERISK	# a69*/
  0x274B, /* #6B # HEAVY EIGHT TEARDROP-SPOKED PROPELLER ASTERISK	# a70*/
  0x25CF, /* #6C # BLACK CIRCLE	# a71*/
  0x274D, /* #6D # SHADOWED WHITE CIRCLE	# a72*/
  0x25A0, /* #6E # BLACK SQUARE	# a73*/
  0x274F, /* #6F # LOWER RIGHT DROP-SHADOWED WHITE SQUARE	# a74*/
  0x2750, /* #70 # UPPER RIGHT DROP-SHADOWED WHITE SQUARE	# a203*/
  0x2751, /* #71 # LOWER RIGHT SHADOWED WHITE SQUARE	# a75*/
  0x2752, /* #72 # UPPER RIGHT SHADOWED WHITE SQUARE	# a204*/
  0x25B2, /* #73 # BLACK UP-POINTING TRIANGLE	# a76*/
  0x25BC, /* #74 # BLACK DOWN-POINTING TRIANGLE	# a77*/
  0x25C6, /* #75 # BLACK DIAMOND	# a78*/
  0x2756, /* #76 # BLACK DIAMOND MINUS WHITE X	# a79*/
  0x25D7, /* #77 # RIGHT HALF BLACK CIRCLE	# a81*/
  0x2758, /* #78 # LIGHT VERTICAL BAR	# a82*/
  0x2759, /* #79 # MEDIUM VERTICAL BAR	# a83*/
  0x275A, /* #7A # HEAVY VERTICAL BAR	# a84*/
  0x275B, /* #7B # HEAVY SINGLE TURNED COMMA QUOTATION MARK ORNAMENT	# a97*/
  0x275C, /* #7C # HEAVY SINGLE COMMA QUOTATION MARK ORNAMENT	# a98*/
  0x275D, /* #7D # HEAVY DOUBLE TURNED COMMA QUOTATION MARK ORNAMENT	# a99*/
  0x275E, /* #7E # HEAVY DOUBLE COMMA QUOTATION MARK ORNAMENT	# a100*/
  0x275F, /* #7F # undefined */
  0xF8D7, /* #80 # MEDIUM LEFT PARENTHESIS ORNAMENT	# a89 (CUS)*/
  0xF8D8, /* #81 # MEDIUM RIGHT PARENTHESIS ORNAMENT	# a90 (CUS)*/
  0xF8D9, /* #82 # MEDIUM FLATTENED LEFT PARENTHESIS ORNAMENT	# a93 (CUS)*/
  0xF8DA, /* #83 # MEDIUM FLATTENED RIGHT PARENTHESIS ORNAMENT	# a94 (CUS)*/
  0xF8DB, /* #84 # MEDIUM LEFT-POINTING ANGLE BRACKET ORNAMENT	# a91 (CUS)*/
  0xF8DC, /* #85 # MEDIUM RIGHT-POINTING ANGLE BRACKET ORNAMENT	# a92 (CUS)*/
  0xF8DD, /* #86 # HEAVY LEFT-POINTING ANGLE QUOTATION MARK ORNAMENT	# a205 (CUS)*/
  0xF8DE, /* #87 # HEAVY RIGHT-POINTING ANGLE QUOTATION MARK ORNAMENT	# a85 (CUS)*/
  0xF8DF, /* #88 # HEAVY LEFT-POINTING ANGLE BRACKET ORNAMENT	# a206 (CUS)*/
  0xF8E0, /* #89 # HEAVY RIGHT-POINTING ANGLE BRACKET ORNAMENT	# a86 (CUS)*/
  0xF8E1, /* #8A # LIGHT LEFT TORTOISE SHELL BRACKET ORNAMENT	# a87 (CUS)*/
  0xF8E2, /* #8B # LIGHT RIGHT TORTOISE SHELL BRACKET ORNAMENT	# a88 (CUS)*/
  0xF8E3, /* #8C # MEDIUM LEFT CURLY BRACKET ORNAMENT	# a95 (CUS)*/
  0xF8E4, /* #8D # MEDIUM RIGHT CURLY BRACKET ORNAMENT	# a96 (CUS)*/
  0xF8E5, /* 8E undefined */
  0xF8E6, /* 8F undefined */
  0X27F0, /* #90 undefined */
  0X27F1, /* #91 undefined */
  0X27F2, /* #92 undefined */
  0X27F3, /* #93 undefined */
  0X27F4, /* #94 undefined */
  0X27F5, /* #95 undefined */
  0X27F6, /* #96 undefined */
  0X27F7, /* #97 undefined */
  0X27F8, /* #98 undefined */
  0X27F9, /* #99 undefined */
  0X27FA, /* #9A undefined */
  0X27FB, /* #9B undefined */
  0X27FC, /* #9C undefined */
  0X27FD, /* #8D undefined */
  0X27FE, /* 9E undefined */
  0X27FF, /* 9F undefined */
  0x0000, /* #A0 undefined */
  0x2761, /* #A1 # CURVED STEM PARAGRAPH SIGN ORNAMENT	# a101*/
  0x2762, /* #A2 # HEAVY EXCLAMATION MARK ORNAMENT	# a102*/
  0x2763, /* #A3 # HEAVY HEART EXCLAMATION MARK ORNAMENT	# a103*/
  0x2764, /* #A4 # HEAVY BLACK HEART	# a104*/
  0x2765, /* #A5 # ROTATED HEAVY BLACK HEART BULLET	# a106*/
  0x2766, /* #A6 # FLORAL HEART	# a107*/
  0x2767, /* #A7 # ROTATED FLORAL HEART BULLET	# a108*/
  0x2663, /* #A8 # BLACK CLUB SUIT	# a112*/
  0x2666, /* #A9 # BLACK DIAMOND SUIT	# a111*/
  0x2665, /* #AA # BLACK HEART SUIT	# a110*/
  0x2660, /* #AB # BLACK SPADE SUIT	# a109*/
  0x2460, /* #AC # CIRCLED DIGIT ONE	# a120*/
  0x2461, /* #AD # CIRCLED DIGIT TWO	# a121*/
  0x2462, /* #AE # CIRCLED DIGIT THREE	# a122*/
  0x2463, /* #AF # CIRCLED DIGIT FOUR	# a123*/
  0x2464, /* #B0 # CIRCLED DIGIT FIVE	# a124*/
  0x2465, /* #B1 # CIRCLED DIGIT SIX	# a125*/
  0x2466, /* #B2 # CIRCLED DIGIT SEVEN	# a126*/
  0x2467, /* #B3 # CIRCLED DIGIT EIGHT	# a127*/
  0x2468, /* #B4 # CIRCLED DIGIT NINE	# a128*/
  0x2469, /* #B5 # CIRCLED NUMBER TEN	# a129*/
  0x2776, /* #B6 # DINGBAT NEGATIVE CIRCLED DIGIT ONE	# a130*/
  0x2777, /* #B7 # DINGBAT NEGATIVE CIRCLED DIGIT TWO	# a131*/
  0x2778, /* #B8 # DINGBAT NEGATIVE CIRCLED DIGIT THREE	# a132*/
  0x2779, /* #B9 # DINGBAT NEGATIVE CIRCLED DIGIT FOUR	# a133*/
  0x277A, /* #BA # DINGBAT NEGATIVE CIRCLED DIGIT FIVE	# a134*/
  0x277B, /* #BB # DINGBAT NEGATIVE CIRCLED DIGIT SIX	# a135*/
  0x277C, /* #BC # DINGBAT NEGATIVE CIRCLED DIGIT SEVEN	# a136*/
  0x277D, /* #BD # DINGBAT NEGATIVE CIRCLED DIGIT EIGHT	# a137*/
  0x277E, /* #BE # DINGBAT NEGATIVE CIRCLED DIGIT NINE	# a138*/
  0x277F, /* #BF # DINGBAT NEGATIVE CIRCLED NUMBER TEN	# a139*/
  0x2780, /* #C0 # DINGBAT CIRCLED SANS-SERIF DIGIT ONE	# a140*/
  0x2781, /* #C1 # DINGBAT CIRCLED SANS-SERIF DIGIT TWO	# a141*/
  0x2782, /* #C2 # DINGBAT CIRCLED SANS-SERIF DIGIT THREE	# a142*/
  0x2783, /* #C3 # DINGBAT CIRCLED SANS-SERIF DIGIT FOUR	# a143*/
  0x2784, /* #C4 # DINGBAT CIRCLED SANS-SERIF DIGIT FIVE	# a144*/
  0x2785, /* #C5 # DINGBAT CIRCLED SANS-SERIF DIGIT SIX	# a145*/
  0x2786, /* #C6 # DINGBAT CIRCLED SANS-SERIF DIGIT SEVEN	# a146*/
  0x2787, /* #C7 # DINGBAT CIRCLED SANS-SERIF DIGIT EIGHT	# a147*/
  0x2788, /* #C8 # DINGBAT CIRCLED SANS-SERIF DIGIT NINE	# a148*/
  0x2789, /* #C9 # DINGBAT CIRCLED SANS-SERIF NUMBER TEN	# a149*/
  0x278A, /* #CA # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT ONE	# a150*/
  0x278B, /* #CB # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT TWO	# a151*/
  0x278C, /* #CC # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT THREE	# a152*/
  0x278D, /* #CD # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FOUR	# a153*/
  0x278E, /* #CE # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT FIVE	# a154*/
  0x278F, /* #CF # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SIX	# a155*/
  0x2790, /* #D0 # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT SEVEN	# a156*/
  0x2791, /* #D1 # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT EIGHT	# a157*/
  0x2792, /* #D2 # DINGBAT NEGATIVE CIRCLED SANS-SERIF DIGIT NINE	# a158*/
  0x2793, /* #D3 # DINGBAT NEGATIVE CIRCLED SANS-SERIF NUMBER TEN	# a159*/
  0x2794, /* #D4 # HEAVY WIDE-HEADED RIGHTWARDS ARROW	# a160*/
  0x2192, /* #D5 # RIGHTWARDS ARROW	# a161*/
  0x2194, /* #D6 # LEFT RIGHT ARROW	# a163*/
  0x2195, /* #D7 # UP DOWN ARROW	# a164*/
  0x2798, /* #D8 # HEAVY SOUTH EAST ARROW	# a196*/
  0x2799, /* #D9 # HEAVY RIGHTWARDS ARROW	# a165*/
  0x279A, /* #DA # HEAVY NORTH EAST ARROW	# a192*/
  0x279B, /* #DB # DRAFTING POINT RIGHTWARDS ARROW	# a166*/
  0x279C, /* #DC # HEAVY ROUND-TIPPED RIGHTWARDS ARROW	# a167*/
  0x279D, /* #DD # TRIANGLE-HEADED RIGHTWARDS ARROW	# a168*/
  0x279E, /* #DE # HEAVY TRIANGLE-HEADED RIGHTWARDS ARROW	# a169*/
  0x279F, /* #DF # DASHED TRIANGLE-HEADED RIGHTWARDS ARROW	# a170*/
  0x27A0, /* #E0 # HEAVY DASHED TRIANGLE-HEADED RIGHTWARDS ARROW	# a171*/
  0x27A1, /* #E1 # BLACK RIGHTWARDS ARROW	# a172*/
  0x27A2, /* #E2 # THREE-D TOP-LIGHTED RIGHTWARDS ARROWHEAD	# a173*/
  0x27A3, /* #E3 # THREE-D BOTTOM-LIGHTED RIGHTWARDS ARROWHEAD	# a162*/
  0x27A4, /* #E4 # BLACK RIGHTWARDS ARROWHEAD	# a174*/
  0x27A5, /* #E5 # HEAVY BLACK CURVED DOWNWARDS AND RIGHTWARDS ARROW	# a175*/
  0x27A6, /* #E6 # HEAVY BLACK CURVED UPWARDS AND RIGHTWARDS ARROW	# a176*/
  0x27A7, /* #E7 # SQUAT BLACK RIGHTWARDS ARROW	# a177*/
  0x27A8, /* #E8 # HEAVY CONCAVE-POINTED BLACK RIGHTWARDS ARROW	# a178*/
  0x27A9, /* #E9 # RIGHT-SHADED WHITE RIGHTWARDS ARROW	# a179*/
  0x27AA, /* #EA # LEFT-SHADED WHITE RIGHTWARDS ARROW	# a193*/
  0x27AB, /* #EB # BACK-TILTED SHADOWED WHITE RIGHTWARDS ARROW	# a180*/
  0x27AC, /* #EC # FRONT-TILTED SHADOWED WHITE RIGHTWARDS ARROW	# a199*/
  0x27AD, /* #ED # HEAVY LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW	# a181*/
  0x27AE, /* #EE # HEAVY UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW	# a200*/
  0x27AF, /* #EF # NOTCHED LOWER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW	# a182*/
  0x27B0, /* #F0 undefined... */
  0x27B1, /* #F1 # NOTCHED UPPER RIGHT-SHADOWED WHITE RIGHTWARDS ARROW	# a201*/
  0x27B2, /* #F2 # CIRCLED HEAVY WHITE RIGHTWARDS ARROW	# a183*/
  0x27B3, /* #F3 # WHITE-FEATHERED RIGHTWARDS ARROW	# a184*/
  0x27B4, /* #F4 # BLACK-FEATHERED SOUTH EAST ARROW	# a197*/
  0x27B5, /* #F5 # BLACK-FEATHERED RIGHTWARDS ARROW	# a185*/
  0x27B6, /* #F6 # BLACK-FEATHERED NORTH EAST ARROW	# a194*/
  0x27B7, /* #F7 # HEAVY BLACK-FEATHERED SOUTH EAST ARROW	# a198*/
  0x27B8, /* #F8 # HEAVY BLACK-FEATHERED RIGHTWARDS ARROW	# a186*/
  0x27B9, /* #F9 # HEAVY BLACK-FEATHERED NORTH EAST ARROW	# a195*/
  0x27BA, /* #FA # TEARDROP-BARBED RIGHTWARDS ARROW	# a187*/
  0x27BB, /* #FB # HEAVY TEARDROP-SHANKED RIGHTWARDS ARROW	# a188*/
  0x27BC, /* #FC # WEDGE-TAILED RIGHTWARDS ARROW	# a189*/
  0x27BD, /* #FD # HEAVY WEDGE-TAILED RIGHTWARDS ARROW	# a190*/
  0x27BE, /* #FE # OPEN-OUTLINED RIGHTWARDS ARROW	# a191*/
  0x27BF, /* #FF UNDEFINED */
};

/* Javascript to decode the email address */

static char js4mail[] = "\
\\begin{javascript}\n\
cle = \"-aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890\";\n\
urm = \"mail\" + \"to:\";\n\
function acode(a,q) {\n\
  var len = a.length; var j=0;\n\
  var i = a.indexOf(\"@\"); if(i<0) i=0;\n\
  var m = a.lastIndexOf(\".\"); if(m<0) m=len;\n\
  var ad = a.substr(0,i); \n\
  var at = ad + \"(at)\";\n\
  while (i<m) {\n\
    if (cle.indexOf(a.charAt(i))==-1) ad+=a.charAt(i);\n\
    else {\n\
      j = (cle.indexOf(a.charAt(i))-len+cle.length) % cle.length;\n\
      ad+=(cle.charAt(j));\n\
    }\n\
    i++;\n\
  }\n\
  ad += a.substr(m);\n\
  at += ad.substr(at.length-3);\n\
  if(q.length>0) ad += \"?\" + q;\n\
  document.write(\"<a href='\" + urm + ad + \"'>\" + at + \"</a>\");\n\
}\n\
\\end{javascript}\
";

/* List of strings known for echo1x (echoed only ONCE) */
static char *echo1a[] = {
    "****Impossible echo1 \\#132456789...",
    js4mail
};

/* Languages are defined in the simple string, control=LANG-value+1 */
static char languages[] = "\1e\1f\2";
/* Words language-dependent, items known by echo4 */
static char undeflang_en[] = "****Undefined echo4 \\#132456789...";
static char *deflang_en[] = {
    undeflang_en,	/*  [0] always undefined */
    "Contents",		/*  [1] */
    "Chapter",		/*  [2] */
    "Appendix",		/*  [3] */
    (char *)0
};
static char undeflang_fr[] = "****echo4 non-defini \\#132456789...";
static char *deflang_fr[] = {
    undeflang_fr,	/*  [0] always undefined */
    "Table des Mati&egrave;res",
    "Chapitre",
    "Appendice",
    (char *)0
};

static char **multilang[] = {
    deflang_en,
    deflang_fr
};

static char usage[] = "\
Usage: cgiprint [-HELP] [-LIST] [-bib[=target]] [-cat[=target]] [-html|-glu]\n\
       [-Ddef=val] [-%X] [-lis] [-mail] [-oglu[w]] [-o output_filter] [-pre]\n\
       [-ic] [-tex[2]] [-tag tag] [-vhtml_version] [-jsJavaScript_version] \n\
       [-f def_file] [-n filename]";

/* There is a need to have a special function which provides the
   next argument
*/
static char **aargs; static int nargs;
static char **aaenv;
static char *args;	/* Shared with getarg() */
static char cargs;

/* Declaration of recursive functions */
static void push_tex(), pop_tex(), tooLong();
static int do_text() ;
static int do_conv() ;
static int PutString() ;
static char *rescan(), *rescan21() ;
static char *setopt();
static int match_if() ;
static int table_out() , table_cell() ;
static int next_char(), get_char() ;


/*==================================================================
		Lowest level Output Routines
 *==================================================================*/
/* There are so many output functions.... the lowest level being
   putchar / putstring / putbuffer
*/
static char putbuf2[2];
#if 1
#define putchar(x)	disable_output ? 0 : (\
			OutFct 	? putbuf2[0]=(x), (*OutFct)(putbuf2, 1) \
				: putc((x), _of))
#define putstring(str)	disable_output ? 0 : (\
			OutFct  ? (*OutFct)(str, strlen(str)) 	\
				: fprintf(_of, "%s", str))
#define putbuffer(s,l)	disable_output ? 0 : (\
			OutFct  ? (*OutFct)(s, l) 	\
				: fwrite(s, 1, l, _of))
#else
static int putchar(int x) {
    if (disable_output)
	return(0);
    else if (OutFct) {
	putbuf2[0] = x;
	return((*OutFct)(putbuf2, 1));
    }
    else
	return(putc(x, _of));
}
static int  putstring(char *str) {
    if (disable_output)
	return(0);
    else if (OutFct)
	return((*OutFct)(str, strlen(str)));
    else
	return(fprintf(_of, "%s", str));
}
static int putbuffer(char *s, int l) {
    if (disable_output)
	return(0);
    else if (OutFct)
	return((*OutFct)(s, l));
    else
	return(fwrite(s, 1, l, _of));
}
#endif
/*==================================================================
		Verification of Buffers
 *==================================================================*/

#if DEBUG > 0
void show_buf(BUF *b) {
     printf("[BUF %p: pos=%d, '%s']",b,b->pos,b->text) ;
}
#endif

static void bufpb(s, b)
  char *s;	/* IN: Message */
  BUF *b;
{
  static char *s0; static BUF *b0; static BUF prev; static int len0;
    fprintf(stderr, "\n++++Pb with buffer '%s' (%p):", s, b);
    if ((b == b0) && (s0) && (strcmp(s,s0) == 0)) {
	if ((b->size == prev.size) && (b->used == prev.used) &&
	    (strlen(b->text) == len0))
	    { putstring(" (again)\n"); return; }
    }
    fprintf(stderr,
	"\n\tsize=%d, used=%d, length=%d, pos=%d, flags=%d, prev=%p",
	b->size, b->used, (int)strlen(b->text), b->pos, b->flags, b->prev);
    fprintf(stderr, "\n\t'%s\n", b->text);
    b0 = b ; if (s0) free(s0); s0 = strdup(s) ;
    memcpy(&prev, b, sizeof(BUF));
    len0 = strlen(b->text);
    return ;
}
#ifdef TRACE_MEMORY
static void verify_buffers()
{
  BUF *b; int i; char ed[20];
    /* Verify Input Buffers */
    for (i=0; i<ITEMS(texparm); i++) {
	if (!texparm[i]) continue;
	sprintf(ed, "texparm[%d]", i);
	if (texparm[i]->used > texparm[i]->size) bufpb(ed, texparm[i]);
	if (strlen(texparm[i]->text) > texparm[i]->used)
	    bufpb(ed, texparm[i]);
    }
    for (b=inbuf; b; b = b->prev) {
	if (b->used > b->size) bufpb("inbuf", b);
	if (b->flags) continue;
	if (strlen(b->text) > b->used) bufpb("inbuf", b);
    }
    if (collector) {
      if (collector->used > collector->size)
	bufpb("collector", collector);
      if (strlen(collector->text) > collector->used)
	bufpb("collector", collector);
    }
    if (unescaped_buf) {
      if (unescaped_buf->used > unescaped_buf->size)
	bufpb("unescaped_buf", unescaped_buf);
      if (strlen(unescaped_buf->text) > unescaped_buf->used)
	bufpb("unescaped_buf", unescaped_buf);
    }
}
#endif

/*==================================================================
		FORK if required the output
 *==================================================================*/

static int open_pipe(argv, mode)
/*++++++++++++++++
.PURPOSE  Open a command (emulate popen)
.RETURNS  The file handle
.REMARKS  Use a fork (popen can't work in ftpd mode)
	  Also sets the last_child with pid number
-----------------*/
  char **argv;	/* IN: Function & Arguments (terminated by 0) */
  int  mode;	/* IN: 0=Read, 1=Write */
{
  int fifo[2], i, pid, mod1;

    if (!argv)  return(mode);	/* No command ! */
    if (!*argv) return(mode);	/* No command ! */
    if (verbop) fprintf(_of, "\n<!-- ....open_pipe(%s, %d) -->\n", *argv, mode);
#if !knowsFork
    fprintf(stderr, "++++CALL open_pipe(\"%s\", %d)\n", *argv, mode);
    return (open(*argv, mode));
#else
    mod1 = mode^1;	/* Transforms 0 into 1 and vice-versa */
    if (pipe(fifo)) { perror(*argv); return(-1); }
			/* Find a slot in table of children   */
    for (i=0; children[i] && (i < ITEMS(children)); i++ ) ;
    pid = fork();
    if (pid < 0) { perror(*argv); return(-1); }
    if (pid) {	/* Father just closes the useless side of pipe */
#if DEBUG
	fprintf(stderr, "....open_pipe(%s) slot#%d, created pid=%d\n",
		*argv, i, pid);
#endif
	if (i < ITEMS(children)) last_child = children[i] = pid;
	else fprintf(stderr, "****Too many children (%d), pid=%d\n", i, pid) ;
	signal(SIGCHLD, SIG_IGN);	/* Remove zombie problem 	*/
	close(fifo[mod1]);
	return(fifo[mode]);
    }
	/* Here the child process has to execute the command */
    close(mod1); i = dup(fifo[mod1]);
    close(fifo[0]); close(fifo[1]);
    execvp(*argv, argv);	/* Replace child by executable */
    perror(*argv);		/*===Should never be here ! ===*/
    return(-1);
#endif
}

static void filter_output(program)
/*++++++++++++++++
.PURPOSE  Link the output to another process
.RETURNS  ---
.REMARKS  The & separate the arguments.
-----------------*/
  char *program ;
{
#if !knowsFork		/* Just error and Exit	*/
    fprintf(stderr, "****Can't fork '%s', sorry!\n", program) ;
#else 			/* Unix Stations Only	*/
  static char *started_pgm = (char *)0 ;
  char **argv, **a ; char *command, *p ;
#if 0
  int fifo[2] ;
#endif
  int np ;

    if (started_pgm) return ;
    started_pgm = program ;
    /* Allocate the command line(s), disentangle the arguiments */
    /* REMIND ! In secure option, add   $httpd_home/bin  to program */
    /*			(EXCEPT for -oglu)                      */
    if (options.sec && httpd_home && (program != oglu_filter)) {
	command = malloc(strlen(program) + strlen(httpd_home) + 8);
	strcpy(command, httpd_home);
	strcat(command, "/bin/") ;
	strcat(command, program) ;
    }
    else command = strdup(program) ;
    for (p=command, np=3; *p; p++) if (*p == '&') np++ ;
    a = argv = (char **)malloc(np*sizeof(char *));
    *(a++) = command ;
    for (p=command; *p; p++) {
	if (*p == '&') *(p++) = 0, *(a++) = p ;
    }
    *a = (char *)0 ;	/* Terminate the array of arguments	*/

#if 0	/* OLD CODE, direct output pipe ---			*/
    if (pipe(fifo)) { perror(program); exit(1); }

    switch(outFilter = fork()) {
    case -1:	/* FAIL  */
	perror(program) ;
	exit(1);
    case 0:	/* CHILD */
	close(fifo[1]); 	/* Side 1 = write, for father	*/
	close(0);
	dup(fifo[0]);		/* Now fileno#0 is fifo[0]	*/
	close(fifo[0]);
	execvp(*argv, argv) ;
	exit(127);		/* NEVER REACHED normally!!	*/
    default:	/* FATHER*/
	close(fifo[0]);  	/* Side 0 = read, for child	*/
	fflush(_of) ;
	_of = fdopen(fifo[1], "w") ;
    }
#else
    fflush(_of) ;
    np = open_pipe(argv, 1) ;
    if (np >= 0) {
	_of = fdopen(np, "w") ;
	outFilter = last_child ;
    }
#endif
#endif
}

static char *exec_command(command)
/*++++++++++++++++
.PURPOSE  Switch to a the result of the execution of a command
.RETURNS  ""
-----------------*/
  char *command;	/* IN: The command to execute	*/
{
  BUF *old_inbuf; FILE *old_infile ; FILE *f;
  char *old_fn, *s, *tmp ; TEX_ENV *old_textop ;
  /* int old_opt ; */

    /* Remove the leading shell symbol */
    while (isspace(*command)) command++ ;
    if (*command == '!') command++ ;
    while (isspace(*command)) command++ ;
#if knowsFork
    /* Interprets the '\21' arguments if any */ 	/* v7.6 */
    if (strchr(command, '\021')) command = tmp = rescan21(command, -1) ;
    else tmp = (char *)0 ;
    s = malloc(strlen(command)+8) ;
    strcpy(s, "(") ; strcat(s, command) ; strcat(s, ") 2>&1") ; /* Mod. V9.04 */
    if (verbop) fprintf(_of, "\n<!-- \\special{! %s}  -->\n", s) ;
    f = popen(s, "r");
    if (!f) {
	perror(s) ;
	PutString("\n<!-- **Error Starting: ");
	PutString(s) ;
	PutString(" -->\n") ;
	if (tmp) free(tmp) ;
	return("\377");
    }

    if (_of) fflush(_of);
    old_fn = thefile ;    thefile = s ;
    old_infile = infile ; infile = f ;
    old_inbuf = inbuf;    inbuf = (BUF *)0 ;
    old_textop = textop ; textop = texenv ;
    /* old_opt = do_option ; do_option = 0 ; */
    while (do_conv(0 /*do_option*/) == 1) ;
    pclose(infile) ; /* Mod. V9.04 */
    /* do_option = old_opt ; */
    textop = old_textop ;
    if (inbuf) inbuf->size = 0, free(inbuf) ;
    inbuf = old_inbuf ;
    infile = old_infile;
    thefile = old_fn ;
    if (_of) fflush(_of);
    if (tmp) free(tmp) ;
    return("") ;
#else
    fprintf(_of, "\n<!-- *** Can't \\special{! %s}  -->\n", s) ;
    return("\n****Can't fork!****\n") ;
#endif
}

static void on_death(dummy)
/*++++++++++++++++
.PURPOSE  Trapping Routine to remove Zombies
.RETURNS  ---
.REMARKS  Read child's status and remove the pid in the table
-----------------*/
  int dummy ;	/* We know which signal it is ... */
{
  int st, pid, i;
#if DEBUG
    fprintf(stderr, "....on_death() called; wait");
#endif
    pid = wait (&st);
#if DEBUG
    fprintf(stderr, " pid=%d, child status=%d\n", pid, st);
#endif
    if ((pid>0) && st) fprintf(stderr,
	"++++Child (pid=%d) ended with status: %d\n", pid, st);
    for (i=0; i<ITEMS(children);  i++) {
	if (children[i] == 0) continue ;
	if (pid < 0) children[i] = 0;	/* pid=-1 ==> no child ... */
	if (children[i] == pid) {
	    children[i] = 0;
	    break;
	}
    }
}

static int wait_pipe(opt)
/*++++++++++++++++
.PURPOSE  Wait until all opened pipes are closed
.RETURNS  0 / -1
.REMARKS  Just performs a wait until all pipes opened via open_pipe are closed
-----------------*/
  int  opt;	/* For future use */
{
  int i;
#if DEBUG	/* List the used slots */
    fprintf(stderr, "....wait_pipe has slots:");
    for (i=0; i<ITEMS(children); i++) if (children[i])
        fprintf(stderr, " %d", children[i]);
    fprintf(stderr, "\n");
#endif
    for (i=0; i<ITEMS(children); i++) {
	if (children[i]) on_death(0);
    }
    return(0) ;
}

/*==================================================================
		Open a file, even when compressed
 *==================================================================*/

static char *zcat[] = {	/* Command + Options to decompress a file */
	"gzip", "-c", "-d",  (char *)0, (char *)0
};
#define zcatfile	(ITEMS(zcat)-2)

static char *zext[] = { 	/* Extensions & Programs */
	".Z",  /* Standard Unix compress */
	".z",  /* GNU zip */
	".gz",
};

static char *zfilename(name)
/*++++++++++++++++
.PURPOSE  Complete a file name with compression extension.
.RETURNS  The complete file name (file exists) / NULL
-----------------*/
  char *name;	/* IN: Name of the file to open */
{
  static char *buf = (char *)0; static int lbuf = 0;
  struct stat bstat;
  int i, len;

    if (stat(name, &bstat) == 0) 	/* File exists...*/
    	return(name);

    for (i=0; i<ITEMS(zext); i++) {
	len = 2 + strlen(name) + strlen(zext[i]);
	if (len > lbuf) {
	    lbuf = (len|0xf) + 1;
	    if (buf) free(buf);
	    buf = malloc(lbuf);
	}
	strcpy(buf, name); strcat(buf, zext[i]);
	if (stat(buf, &bstat) == 0) 	/* File exists...*/
	    return(buf);
    }
    return ((char *)0);
}

static FILE *fzopen(name)
/*++++++++++++++++
.PURPOSE  Open a compressed file for READ ONLY
.RETURNS  The FILE / NULL if error
.REMARKS  Signal SIGCHLD is ignored !
-----------------*/
  char *name;	/* IN: Name of the file to open */
{
  FILE *f; char *fname; int fd;

    /*fprintf(stderr, "#...fzopen(%s)\n", name);*/
    if (f = fopen(name, "r"))	return(f);

    if (errno == ENOENT) { 	/* File not found */
    	if (fname = zfilename(name)) {
	    zcat[zcatfile] = fname;
	    fd = open_pipe(zcat, 0);		/* Open for READ */
	    if (fd >= 0) f = fdopen(fd, "r");	/* Associate STREAM */
	}
    }
    return (f);
}

/*==================================================================
		Output Functions
 *==================================================================*/
static void terminate(message)
/*++++++++++++++++
.PURPOSE  Terminate in case of Errors
.RETURNS  to OS
-----------------*/
  char *message ;	/* IN: Message issued */
{
    putstring("<H1>");
    putstring(message) ;
    putstring("</H1>");
    fprintf(stderr, "#***cgiprint terminated with error message: %s\n",
	    message);
    wait_pipe(0);
    exit(1) ;
}

static int PutChar(c)
/*++++++++++++++++
.PURPOSE  Output a character
.RETURNS  Character issued / EOF
.REMARKS  An EOF character forces the output.
	NOTE: The MODE_out1d flag (significant output char) is set.
-----------------*/
  int  c;         /* IN: Char to issue	*/
{
  static int error_count=0;
  char *str, edx[8] ;  int i, v1, href_mode, stat=0, js_hexa2;

    if (ignore_output) return(0) ;
    if (theMode&MODE_upper) c = toupper(c) ;
    else if (theMode&MODE_lower) c = tolower(c) ;

    /* Special Case for HREF="..." */
#if 1	/* Changed in v8.2 */
    js_hexa2 = 0;
    if ((href_mode = (theMode&MODE_href))) {	/* v8.2 */
# if 0	/* NOTE: the tests MODE_href1 and MODE_href2 are not
	   necessary here, it's done in do_conv()
	*/
	if (href_mode == MODE_href1)
	  theMode ^= (MODE_href1|MODE_href2);	/* ref1->ref2 */
	else if (href_mode == MODE_href2) {
	    /* I'm in a HREF, before the '?' */
	    if (c == '?') theMode |= MODE_hrefa|MODE_verb1 ;
	}
# endif
	if (href_mode & MODE_js /* 1 */) {	/* v8.21 */
	    if (*vMozilla>='5') js_hexa2 = 1;
	}
    }
#else	/* OBSOLETE, Before v8.2 */
    /* Special Case for ? */
    if (((theMode&(MODE_Aargq|MODE_Aarg|MODE_Aarga|MODE_verb1)) == MODE_Aargq)
       &&(c == '?'))
	theMode |= (MODE_Aarg|MODE_verb1) ;
#endif
    /* V9.20: MODE_Aglu && MODE_xArg ==> MUST translate */
    v1 = theMode&(MODE_Aglu|MODE_verb1);
    if (theMode&(/*MODE_Aglu|*/MODE_verb1)) theMode &= ~MODE_verb1 ;
    /* Mod. V9.25 */
    else if (((href_mode&MODE_hrefa) == MODE_hrefa) || (theMode&MODE_xArg))
           /*(theMode&(MODE_Aarg|MODE_Aarga))*/ switch(c) {
#if 0				/* v8.2: it can't be the first ? */
      case '?':			/* Not the first ?, edit  */
	if (theMode & MODE_Aarg) goto case_hexa;	/* Not first ? */
	else { theMode |= MODE_Aarg; break ; }
#endif
      /* Version v8.2: \vNote('\htarg(&)') need DOUBLE encoding
	 It's a stupid behaviour of IE and Mozilla >= 5
      */
      case '#':
	if (theMode & MODE_xArg) {
	    if (js_hexa2) goto case_hexa2;
	    else goto case_hexa;
	}
	break ;
      case '&':
	if (theMode & MODE_xArg) {
	    if (js_hexa2) goto case_hexa2;
	    else /* goto case_hexa; */ {	/* V8.71: use %16 */
		str = "%16";
		goto case_str;
	    }
	}
	else {		/* === & = Separation of Arguments */
      	    str = "&amp;" ;
      	    goto case_str ;
	}
      case '/':
	if (keep_slash) break ;
	goto case_hexa ;
      case ';':
	if (theMode & MODE_xArg) goto case_hexa ;
	break ;		/* Keep the ;	*/
      case '+':
	if (theMode & MODE_xArg) {
	    if (js_hexa2) goto case_hexa2;
	}
	goto case_hexa;
      case '\'':
	/* if ((theMode&MODE_jsArg) == MODE_jsArg)  break ;
	   --- test done in do_conv
	*/
      case ' ':
      case '?':		/* Not the first ? */
      case '%':
      case '*':
      case '<': case '>':
      case '"': case '`':
      case '[': case ']':
      case '(': case ')':
      case '{': case '}':
      case '|': case '\\':
      case_hexa:
	sprintf(str=edx, "%%%02x", c);
      case_str:
	/*i = strlen(str) ;*/
	if(theOutputRoutine) stat = (*theOutputRoutine)(str,strlen(str)) ;
	else stat = putstring(str) ;
	if (!(theMode&MODE_intag)) theStatus |= MODE_out1d ;
	goto Check_Write; /* Mod. V9.46 return(stat); */
      case_hexa2:
	sprintf(str=edx, "%%%02x%02x", '%', c);
	goto case_str;
      default:
	if (iscntrl(c)) goto case_hexa ;
    }
    else if (theMode&(MODE_2html|MODE_intag|MODE_VERB|MODE_quot)) switch(c) {
      case '&':  str = "&amp;";  goto case_str ;
      case '<':  /*str = "&lt;";   goto case_str ; Mod. V9.41 */
		 /*printf("[mode=%X]",theMode&(MODE_jsquo|MODE_intag));*/
		 str = (theMode&(MODE_jsquo|MODE_intag|MODE_math))==MODE_jsquo ?
		 /* V9.44: added |MODE_math */
		     "&amp;#60;" : "&lt;"; goto case_str ;
      case '>':  str = "&gt;";   goto case_str ;
      case '\'': if (!(theMode&MODE_2html)) break ;
		 str = theMode&MODE_jsquo ? /*"&amp;#39;"*/
		     "\\&apos;" : "&apos;"; goto case_str ;
      case '"':  if (!(theMode&(MODE_2html|MODE_quot))) break ;
		 str = "&quot;"; goto case_str ;
      case '\\': if (theMode&MODE_jsquo) { str = "\\\\"; goto case_str; }
		 break;	/* Added V9.25 */
    }

    /* Set the flag indicating a valid char  was issued for table cells */
    if ((!v1) && (!(theMode&MODE_intag)) && isgraph(c))	/* v7.84 */
	theStatus |= MODE_out1d ;

#if 0	/* This is done in do_conv	-- v8.2 */
    /* Set the flag used in javascript for Quote in Quote as
       \A{javascript:myFct('http://machine/cgi-bin/pgm?arg=val')}{click}
    */
    if ((theMode&MODE_Aargq) && (c == '\'')) {
	if (theMode&MODE_apos) /* Second Quote encountered -- Stop %xx */
	    theMode &= ~(MODE_Aarga|MODE_Aarg) ;
	theMode ^= MODE_apos ;
    }
#endif

    if(theOutputRoutine)  {
    	edx[0] = c ; /* fflush(stdout);  */
    	return((*theOutputRoutine)(edx, 1)) ;
    }
    if ((c&0xff) == 0xff) return(EOF) ;	/* Not an error */
    stat = putchar(c);
  Check_Write:
    if (stat==(-1)) {
	if (++error_count > MAXERR_WRITE)
	    terminate(strerror(errno));
    }
    return(stat);
}

static void PutComment(char *text, char *end)
/*++++++++++++++++
.PURPOSE  Write the text as comment
.RETURNS  ---
-----------------*/
{
  char *p ; int old_mode;
    if (ignore_output) return;
    if (theOutputRoutine) return;	/* No comment in buffers */
    old_mode = theMode;
    fprintf(_of, "<!--");
    for (p=text; *p; p++) switch(*p) {
      case '&': fprintf(_of, "&amp;") ; continue ;
      case '<': fprintf(_of, "&lt;")  ; continue ;
      case '>': fprintf(_of, "&gt;")  ; continue ;
      case '-':
	  while (p[1] == '-') fputc('_', _of), p++;
      /* NO BREAK */
      default:	fputc(*p, _of);
    }
    fprintf(_of, "-->");
    if (end) fprintf(_of, "%s", end);
}

static int PutString(s)
/*++++++++++++++++
.PURPOSE  Output a String
.RETURNS  cf printf
-----------------*/
  char *s; 	/* IN: String to issue  */
{
  char *p ; int stat=0;
    if (ignore_output) return(0) ;
    if (theMode&MODE_verb1) {
	stat = PutChar(*s) ;	/* Will modify theMode */
	if (stat>=0) return(stat+PutString(++s)) ;
	for (p=s; *p; p++) stat += PutChar(*p) ;
	return(stat) ;
    }
    else if (theMode&
      (MODE_upper|MODE_lower|MODE_VERB|MODE_2html|MODE_href|MODE_xArg)) {
	for (p=s; *p; p++) stat += PutChar(*p) ;
	return(stat) ;
    }
    /* V9.46: Verify the Write status */
    if (theOutputRoutine)
	return((*theOutputRoutine)(s, strlen(s))) ;
    stat = putstring(s) ;
    if (stat==(-1)) /* An error occured: write the characters one by one ,
		       so an error will be raised */
	for (p=s; *p; p++) stat += PutChar(*p);
    return(stat);
}

static int PutText(s, len)
/*++++++++++++++++
.PURPOSE  Output a character
.RETURNS  cf printf
-----------------*/
  char *s; 	/* IN: String to issue  */
  int  len; 	/* IN: Length to issue	*/
{
  char *p ; int stat=0;
    if (ignore_output) return(0) ;
    if (theMode&(MODE_upper|MODE_lower|MODE_href|MODE_xArg)) {
	for (p=s; --len >= 0; p++) stat += PutChar(*p) ;
	return(stat) ;
    }
    if(theOutputRoutine) return((*theOutputRoutine)(s, len)) ;
    if (len > 0) {	/* V9.46: Verify the Write status */
	stat = putbuffer(s, len) ;
	if (stat==(-1)) while (--len >= 0) stat += PutChar(*p) ;
    }
    return(stat) ;
}

static char *type_ToC(alevel)
/*++++++++++++++++
.PURPOSE  Edit the item at specified level for Table of Contents
.RETURNS  ""
.REMARKS  "0" ==> at end (close all) ;
	not-numeric ==> just reset (toggle another time).
	V8.3: Changed list as <OL> (ordered list) into <UL> with no style
-----------------*/
  char *alevel;	/* IN: The level (2 for Chapter, 3 for Section...) */
{
  static char buf[60];		/* Edited section number */
  char atitle[128];
  char *p ;
  int m, m1, i, level ;
  INT_FCT f ;

    /* Toggle between output functions */
    f = OutFct ; OutFct = saved_OutFct ; saved_OutFct = f ;
    /* Test only :
    fprintf(stderr, "#...type_ToC(%c) [buf=%s]\n", *alevel, buf); */

    /* Decide what to write ... */
    p = (char *)0 ;
    if (*alevel == 'C') {		/* Beginning of ToC */
	sprintf(atitle, "<h2>%s:</h2>\n", multilang[options.lang][1]) ;
	p = atitle;			/* p = "<h2>Contents:</h2>\n" */
	buf[0] =  0 ;
    }
    else if (*alevel == 'A') {
	p = "<P>";	/* "<B>Appendix:</B>"; */
	buf[0] = '\1';
    }
    else if (!isdigit(*alevel)) return("") ;

    /* Write out the text */
    if (p) {
	putstring(p) ;
	return("") ;
    }

    /* Increase ToC counter, and change OutFct to effectively output TOC */
    counter[0] += 1 ;
    level = atoi(alevel) ;

    /* Close all opened lists */
    m = 1 << level ;
    m1 = m << 1 ;
    for (i = (1<<6) ; ToC_status >= m1 ; i >>= 1) {
    	if (ToC_status&i) {
	    putstring("</UL>\n") ;
	    if (p = strrchr(buf, '.')) *p = 0;
	}
    	ToC_status &= ~i ;
    }
    /* Open a new list --- a zero level asks to close ! */
    if (level > 0) {
        if (!(ToC_status & m)) {
    	    putstring("<UL style=\"list-style: none;\">\n");
    	    ToC_status |= m ;
	    if (iscntrl(buf[0])) {	/* First level */
		buf[0] = buf[0]&1 ? 'A'-1 : '0';
		buf[1] = 0;
	    }
	    else {			/* Next levels */
		p = buf + strlen(buf);
		*(p++) = '.';
		*(p++) = '0';
		*(p++) = 0;	/* V9.18: Terminate string */
	    }
        }
	if (p = strrchr(buf, '.')) {
	    p++;
	    sprintf(p, "%d", atoi(p)+1);
	}
	else if (isdigit(buf[0])) sprintf(buf, "%d", atoi(buf)+1);
	else buf[0] += 1;
        putstring("<LI>") ;
	if (strlen(buf) >= sizeof(buf)-5)
	    putstring("****too large ToC???***");
        putstring(buf) ;
	putstring(". ");
    }

    return("") ;
}

/*==================================================================
		Very basic utilities...
 *==================================================================*/
static char *html_spec(accent, letter)
/*++++++++++++++++
.PURPOSE  Special HTML Accents
.RETURNS  The equivalence
-----------------*/
  int accent ;	/* IN: Accent type, e.g. v	*/
  int letter ;	/* IN: Letter, e.g. 'S'		*/
{
  static char buf[4] ;
    switch(accent) {
      case 'c':		/* Cedilla */
	switch(letter) {
	  case 'c': return("&ccedil;") ;
	  case 'C': return("&Ccedil;") ;
	}
	break ;
      case 'v':
	switch(letter) {
	  case 's': return("&#353;") ;
	  case 'S': return("&#352;") ;
	}
	break ;
    }
    /* Default is just the letter */
    buf[0] = letter ;
    buf[1] = 0 ;
    return(buf) ;
}

static void strhtml(text)
/*++++++++++++++++
.PURPOSE  Convert HTML strings
.RETURNS  ---
-----------------*/
  char *text;     /* MOD: String to interpret */
{
  char *i, *o ; int c1, c2 ;
    for (i=o=text; *i; i++, o++) {
	if (*i == '&') { *o = '\t' ; continue ; }
	if (*i != '%') { *o = *i   ; continue ; }
	if ((!isxdigit(i[1])) || (!isxdigit(i[2]))) { *o = *i ; continue ; }
	++i ; c1 = toupper(*i) ;
	++i ; c2 = toupper(*i) ;
	c1 -= isdigit(c1) ? '0' : ('A'-10) ;
	c2 -= isdigit(c2) ? '0' : ('A'-10) ;
	*o = (c1<<4) | c2 ;
    }
    *o = 0 ;
}

static void acode(char *str)
/*++++++++++++++++
.PURPOSE  Encode an e-mail address
.RETURNS  ---
-----------------*/
{
  static char cle[] = \
         "-aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ1234567890";
  int perm;
  char *p, *s, *d;
  perm = strlen(str);
    d = (char *)0;
    for (s=str; *s && (*s != '@'); s++) ;
    for (*s ? s : str; *s; s++) {
        for (p=cle; *p && (*p != *s); p++) ;
        if (*p) *s = cle[((p-cle) + perm) % (sizeof(cle)-1)];
        else if (*s == '.') d = s;
    }
    /* Return back what comes after the last dot (keep country) */
    if (d) for (s=d+1; *s; s++) {
        for (p=cle; *p && (*p != *s); p++) ;
        if (*p) *s = cle[((p-cle) - perm + sizeof(cle)-1) % (sizeof(cle)-1)];
    }
}


static int x2i(text)
/*++++++++++++++++
.PURPOSE  Interpret an hexadecimal number
.RETURNS  Number
-----------------*/
  char *text ;	/* IN: Text top interpret */
{
  int val ; char *p ;
    p = text ;
    if (*p == '#') p++ ;
    while ((p[0] == '0') || (p[0] == 'x')) p++;
    val = 0 ;
    while(*p) {
	val <<= 4 ;
	if (isdigit(*p)) val |= (*p - '0') ;
	else if (isxdigit(*p)) val |= 10 + (toupper(*p) - 'A') ;
	else if (isspace(*p)) ;
	else fprintf(stderr, "<!-- **PROBLEM x2i(%s) -->\n", text) ;
	p++ ;
    }
    return(val) ;
}

static char *i2a(val)
/*++++++++++++++++
.PURPOSE  Edit (in local buffer) the number
.RETURNS  Edited number
-----------------*/
  int val ;	/* IN: Value to edit */
{
  static char anum[20] ;
    sprintf(anum, "%d", val);
    return(anum) ;
}

static char *i2rom(num)
/*++++++++++++++++
.PURPOSE  Edit (in local buffer) the number as ROMAN
.RETURNS
.REMARKS
-----------------*/
  int num ;	/* IN: Value to edit */
{
  char *p; int i ;
  static char buf[40] ;
    if ((num<=0)||(num>=10000)) return(i2a(num)) ;
    p = buf ; i = num ;
    while (i >= 1000) *(p++) = 'M', i -= 1000 ;
    if    (i >= 500)  *(p++) = 'D', i -= 500 ;
    while (i >= 100)  *(p++) = 'C', i -= 100 ;
    if    (i >= 50)   *(p++) = 'L', i -= 50 ;
    if    (i >= 40)   *(p++) = 'X', *(p++) = 'L', i -= 40 ;
    while (i >= 10)   *(p++) = 'X', i -= 10 ;
    if    (i >= 9)    *(p++) = 'I', *(p++) = 'X', i -= 9 ;
    if    (i >= 5)    *(p++) = 'V', i -= 5 ;
    if    (i >= 4)    *(p++) = 'I', *(p++) = 'V', i -= 4 ;
    while (i >= 1)    *(p++) = 'I', i -= 1 ;
    *p = 0 ;
    return(buf) ;
}

static int roman(c)
/*++++++++++++++++
.PURPOSE  Just give the number equivalent to Roman letter
.RETURNS  Value / 0
-----------------*/
  int c ;	/* IN: The character */
{
    switch(toupper(c)) {
      case 'I': return(1) ;
      case 'V': return(5) ;
      case 'X': return(10) ;
      case 'L': return(50) ;
      case 'C': return(100) ;
      case 'D': return(500) ;
      case 'M': return(1000) ;
    }
    return(0) ;
}

static int match_roman (char *string, int *value)
/*++++++++++++++++
.PURPOSE  Analyze the incoming and get integer value, can be in roman
.RETURNS  How many bytes matched
.REMARKS
-----------------*/
{
  char *p ; int n, m;
    p = string; *value = 0 ;
    for (n = roman(*p); n; n = roman(*++p)) {
    	m = roman(p[1]);
    	if (n < m) *value += (m-n), p++ ;
    	else *value += n ;
    }
    return (p-string) ;
}

static char *date(itime)		/* v7.6 */
/*++++++++++++++++
.PURPOSE  Edit (in local buffer) the date
.RETURNS  Edited number
.REMARKS: from version 9.08, edit in numeric
-----------------*/
  time_t *itime ;	/* IN: Unix Time */
{
  static char month[] = "\
JanFebMarAprMayJunJulAugSepOctNovDec";
  static char adate[20] ;
  static time_t now ;
  char *t; int m;
    if (!itime) now = time((long *)0), itime = &now ;
    t = ctime(itime) ;
#if 0
    adate[0] = t[8]; adate[1] = t[9] ; adate[2] = t[10] ;
    adate[3] = t[4]; adate[4] = t[5] ; adate[5] = t[6] ;
    strcpy(adate+6, t+19) ;
    adate[11] = 0 ;
#else
    /* Get month number */
    adate[0] = t[4]; adate[1] = t[5]; adate[2] = t[6]; adate[3] = 0;
    m = 1+(strstr(month, adate)-month)/3;
    adate[0] = t[20]; adate[1] = t[21]; adate[2] = t[22]; adate[3] = t[23];
    adate[4] = '-';   adate[5] =(m/10)|'0'; adate[6] = (m%10)|'0';
    adate[7] = '-';   adate[8] = t[8] |'0'; adate[9] = t[9];
    adate[10] = 0;
#endif
    return(adate) ;
}

static char *filedate(name)		/* v7.6 */
/*++++++++++++++++
.PURPOSE  Fidn the date of a file
.RETURNS  The date / NULL
-----------------*/
  char *name;	/* IN: Name of the file to open */
{
  struct stat bstat;
  time_t *t ;
    t = (time_t *) 0 ;
    if (name) {
	if (stat(name, &bstat) == 0) 	/* File exists...*/
	t = &bstat.st_mtime ;
    }
    return(date(t));
}

static char *echo1x(num)		/* v8.32 */
/*++++++++++++++++
.PURPOSE  Echo the text ONLY ONCE from echo1a array
.RETURNS  The text (non-void only at first call)
-----------------*/
  char *num;	/* IN: Number in echo1a */
{
  int i; char *p;
    i = atoi(num);
    if (i>=ITEMS(echo1a)) {
	sprintf(strchr(echo1a[0], '#')+1, "%d", i);
	i=0;
    }
    p = echo1a[i];
    echo1a[i] = "";
    return(p);
}

static char *echo4(num)			/* v8.5  */
/*++++++++++++++++
.PURPOSE  Echo the multingual text from multilang
.RETURNS  The text
-----------------*/
  char *num;	/* IN: Number in deflang_en */
{
  int i; char *p;
    i = atoi(num);
    if ((i>=ITEMS(deflang_en)) || (i<=0)) {
	p = multilang[options.lang][0];
	sprintf(strchr(p, '#')+1, "%d", i);
    }
    else p = multilang[options.lang][i];
    return(p);
}

static int strbloc(text, c)
/*++++++++++++++++
.PURPOSE  Locate (loast) specified character
.RETURNS  Index of located char / -1
-----------------*/
  char *text;     /* IN: String to interpret */
  int  c;         /* IN: Char to look for */
{
  char *s;
    for ( s = text + strlen(text) -1; s>=text; s--)      if (*s == c)    break;
    return(s-text);
}

static int strloc(text, c)
/*++++++++++++++++
.PURPOSE  Locate specified character
.RETURNS  Index of located char
-----------------*/
  char *text;     /* IN: String to interpret */
  int  c;         /* IN: Char to look for */
{
  char *s;
    for ( s = text; *s; s++)      if (*s == c)    break;
    return(s-text);
}

static int strbrace(text)
/*++++++++++++++++
.PURPOSE  Locate the closing brace
.RETURNS  Index of  closing brace
-----------------*/
  char *text;     /* IN: String to interpret, '{' skipped */
{
  int depth; char *p ;
    for (depth = 1, p = text; depth && *p ; p++) switch(*p) {
	case '\020':
	case '\021':
    	case '\\': p++; continue;
    	case '{': depth++; continue ;
    	case '}': depth-- ; continue ;
    }
    if (depth == 0) p-- ;
    return(p-text);
}

static int strquote(s)
/*++++++++++++++++
.PURPOSE  Find the matching quote
.RETURNS  Offset of next "
-----------------*/
  char *s;	/* IN: String starting by " or ' */
{
  char *p, q;
    p = s; q = *(p++) ;
    p += strloc(p, q) ;
    return(p-s) ;
}

static int strhttp(s)
/*++++++++++++++++
.PURPOSE  Find the end of a http expression
.RETURNS  The length
-----------------*/
  char *s;	/* IN: String starting by " or ' */
{
  char *p, q ;
    for (p=s , q=0; isgraph(*p); p++) {
	if (*p == '?') {
	    if (q) break ;	/* Not two ? */
	    q = *p;
	    continue ;
	}
	if (q) {
	    if ((*p == '(') || (*p == ')') || (*p == '<') || (*p == '>')
	      ||(*p == '"') || (*p == '`') || (*p == '\'')) break ;
	    continue ;
	}
	if (c1sep[strloc(c1sep,*p)]) break ;
	if (*p == not1char) break ;
    }
    return(p-s) ;
}

static int strput(s, out)
/*++++++++++++++++
.PURPOSE  Interpret a char, store it
.RETURNS  Number of bytes processed
-----------------*/
  char *s; 	/* IN: String to interpret */
  char *out;	/* OUT: Where to store the result */
{
  int i0, i1;
    if((s[0]=='\\')&&(s[1]=='x')&&isxdigit(s[2])&&isxdigit(s[3])) {
	i0 = s[2] - (isdigit(s[2]) ? '0' : (isupper(s[2]) ? 'A'-10 : 'a'-10)) ;
	i1 = s[3] - (isdigit(s[3]) ? '0' : (isupper(s[3]) ? 'A'-10 : 'a'-10)) ;
	*out = (i0<<4)|i1 ;
	return(4);
    }
    *out = *s;
    return(1);
}

static int strdiff1(s1, s2)
/*++++++++++++++++
.PURPOSE  Compare 2 strings, but only 1st word
.RETURNS  0 (first word identical) / difference
-----------------*/
  char *s1;     /* IN: String to interpret */
  char *s2;     /* IN: String to interpret */
{
    while ((*s1 == *s2) && isgraph(*s1)) s1++, s2++ ;
    if (isgraph(*s1) || isgraph(*s2)) return(*s1 - *s2) ;
    return(0) ;
}

static int str2int(text, value)
/*++++++++++++++++
.PURPOSE  Extract a number from text
.RETURNS  Number of bytes scanned
-----------------*/
  char *text;     /* IN: String to interpret */
  int  *value;    /* OUT: resulting value    */
{
  char *s;
    for (s = text, *value=0; isdigit(*s); s++)
	*value = (*value * 10) + (*s - '0');
    return(s-text);
}

static int gobble_spaces()	/* Added V8.83 */
/*++++++++++++++++
.PURPOSE  Move in the input stream and skip spaces
.RETURNS  0 (no blank/space) \n (newline read) blank (spaces read) EOF
-----------------*/
{
  int st=0;
  int c;
    c = next_char();
    while (isspace(c)) {
	if ((c == '\n') && (st == '\n'))	/* One newline only */
	    break;
	if (st != '\n') st = c;
	get_char();
	c = next_char();
    }
    return(st);
}

/*==================================================================
		Pattern Matching (filenames rules)
 *==================================================================*/

int strmatch(template, name)
/*++++++++++++++++
.PURPOSE  Check name against (file) template
.RETURNS  1 (Match) / 0 (mismatch)
.REMARKS  * ? [...] recognized as wild chars
-----------------*/
  char *template;	/* IN: Name template with * ? */
  char *name;		/* IN: Name to check */
{
  char *t, *f; int m;
    for (t = template, f = name; *t; t++) switch(*t) {
      case '?':			/* Matches one character */
	if (*f) f++;
	else return(0);
	continue;
      case '*':			/* Match 0 or n characters */
	if (! *++t) return(1);	/* The * is the last char of template */
#if 1		/* Try LONGEST possible matching */
	for (m = strlen(f); --m >= 0; ) {
	    if (strmatch(t, f+m))
		return(1);
	}
#else		/* Try SHORTEST possible matching */
	for (; *f; f++) {	/* Look for first matching char */
	    if (isalnum(*t) && (*f != *t)) continue;
	    if (strmatch(t,f))	/* Recursive call */
		return(1);
	}
#endif
	return(0);
      case '[':		/* A set of characters is provided */
	++t; m=0;
	if (*t == ']') { /* Closing brace is acceptable	*/
	    t++;
	    m = (*f == ']');
	}
	while ((m==0) && *t) {	/* Check: does *f be in set ? 	*/
	    if (*t == ']') return(0);		/* End of set	*/
	    if ((t[1] == '-') && (t[2] != ']')) {      /* Range	*/
		m = (*f >= t[0]) && (*f <= t[2]);
		t += 3;
	    }
	    else m = (*f == *t), t++;
	}
	if (m == 0) return(0);
	t += strloc(t, ']');
	f++;
	continue;
      case '\\':	/* Escaped Char. NO BREAK HERE! */
	t++;
      default:
	if (*t != *f) return(0);
	f++;
	continue;
    }
    return((!*t) && (!*f));	/* Returns 1 if *t AND *f are NUL */
}

/*==================================================================
		Regular Expressions
 *==================================================================*/

static char *pregexp ;	/* Shared with compile (regexp.h) */
#define INIT
/* #define INIT		register char *pregexp = instring ; */
#define GETC()		*(pregexp++)
#define PEEKC()		(*pregexp)
#define UNGETC(c)	(--pregexp)
#define RETURN(c)	return(c);
#define	ERROR(c)	fprintf(_of, \
			"\n<!-- **Error #%d in regexp '%s'+%d -->\n",\
			c,instring,(int)(pregexp-instring))
#include <regexp.h>

int strexp(cexp, input, implied_circf)
/*++++++++++++++++
.PURPOSE  Check whether input satisfies the compiled regular expression
.RETURNS  Number of matched chars / 0 for mismatch
-----------------*/
  char *cexp;		/* IN: Compiled regular expression */
  char *input;		/* IN: String to check	*/
  int implied_circf ;	/* IN: 1 if the ^ was omitted */
{
  int saved_circ, n;
    saved_circ = circf ;
    circf = implied_circf ;
    if (step(input, cexp)) n = (loc2-loc1);
    else n = 0;
    circf = saved_circ ;
    return(n);
}

static char *compile_expr(input)
/*++++++++++++++++
.PURPOSE  Compile a regular expression
.RETURNS  The (newly allocated) compiled expression / NULL
.REMARKS  The ^ (beginning of text anchor) is IGNORED!
-----------------*/
  char *input;	/* IN: The regular expression */
{
  char buf[BUFSIZ] ; char *p; int n;
    memset(buf, 0, sizeof(buf));
    pregexp = input ;
    p = compile(input, buf, buf+sizeof(buf), 0);
    if (circf) fprintf(stderr, "++++ '^' ignored in regexp '%s'\n", input) ;
    /* Note: bytes scanned in compilation are pregexp - input */
    n = (p-buf) +1 ;
    p = malloc(n);
    memcpy(p, buf, n);
    return(p) ;
}

static int regsub(cexp, arg, end_char, inputa)
/*++++++++++++++++
.PURPOSE  Substitute a string (like sed)
.RETURNS  0 (no match, nathing printed) / 1 (matched)
.REMARKS  Use stumatch for compiled expressions.
-----------------*/
  char *cexp;	/* IN: Compiled regular expression */
  char *arg;	/* IN: Substitution String	*/
  int  end_char;/* IN: Character terminating arg*/
  char *inputa;	/* IN: String to transform	*/
{
  int saved_circ, n, i, c;
  char *input = inputa;
  char *a ;
    saved_circ = circf ;
    circf = 0 ;
    loc2 = input ;
    for (n = 0 ; step(input, cexp); n++, input = loc2) {
	PutText(input, loc1-input) ;
	for (a = arg; *a && (*a != end_char); a++) {
	    if (*a != '\\') { PutChar(*a); continue; }
	    ++a ;
	    if (isdigit(*a)) {
	        i = *a-'1' ;
	        if ((i >= 0) && (i < nbra)) {
		    PutText(braslist[i], braelist[i] - braslist[i]) ;
		    continue ;
	        }
	    }
	    if (!*a) { PutChar('\\') ; continue ; }
	    switch(c = *a) {
	      case 'n': c = '\n'; break ;
	      case 't': c = '\t'; break ;
	      case 'v': c = '\v'; break ;
	      case 'r': c = '\r'; break ;
	      case 'f': c = '\f'; break ;
	    }
	    PutChar(c) ;
        }
	if (*a != end_char) fprintf(_of, "\n<!-- **Error in \\sed{...%s} -->\n",
	    arg) ;
	if (*a) a++ ;
	if (*a != 'g') break ;
	if (loc1 == loc2) {	/* Zero-length string matched??? */
	    fprintf(stderr, "#***Zero-length regexp matched in \"%s\"+%d^%s\n",
		    inputa, (int)(input-inputa), input);
	    break;
	}
    }
    PutString(loc2) ;
    circf = saved_circ ;
    return(n);
}

static int trcompile(text, buf)		/* v7.6 */
/*++++++++++++++++
.PURPOSE  Compile for the 'tr' function
.RETURNS  Scanned text
.REMARKS  Write y/[\/A-Z]/[\\a-z]/ ,   not
		y/[/A-Z]/[\a-z]/
-----------------*/
  char *text ;	/* IN: to compile e.g. /[0-9]//d   to delete the quoted chars */
  char *buf ;	/* OUT: The translation table */
{
  char *t, stop ;
  char arg1[256], arg2[256], *a, *e ;
  int c, f, dash ;

    for (c=0; c<256; c++) buf[c] = c ;
    dash = 0;	/* when set, dash means 'set' e.g. A-Z */
    arg1[0] = arg2[0] = 0 ;
    t = text ; while (isspace(*t)) t++ ;
    if (*t == '[') ++t, stop = ']', dash=1 ;
    else if (*t == '(') stop = ')', ++t ;
    else if (ispunct(*t)) stop = *(t++) ;
    else {
	fprintf(stderr, "****Can't compile '%s' for tr()\n", text) ;
	return(-1) ;
    }
    a = arg1 ; e = arg1 + sizeof(arg1)-1 ;

    while (*t && (a < e)) {
	if (*t == '[') {
	    if (dash) c = '[' ;
	    else c = 0 ;
	    ++t ; dash = 1 ;
	    continue ;
	}
	else if (*t == '-') {
	    if (dash && c) {		/* c is the last read character */
		f = (*++t) & 0xff ;
		if (f == ']') f = '-', --t ;
		else for(++c; (c < f) && (a < e); c++) *(a++) = c ;
		c = f ;
	    }
	    else c = '-' ;
	}
	else if (*t == '\\') c = *++t ;
	else if (*t == stop) {
	    if (a < e) *a = 0; else *e = 0 ;
	    dash = 0;
	    ++t ;
	    if (e == arg2 + sizeof(arg2)-1) break ;
	    a = arg2; e = arg2 + sizeof(arg2)-1 ; ;
	    continue ;
	}
	else if (dash && (*t == ']')) {
	    dash = 0;
	    ++t;
	    continue;
	}
	else c = *t ;
	*(a++) = c ;
	++t ;
    }
    if (a < e) *a = 0; else *e = 0 ;

    /* arg1 and arg2 contain the argument strings for the transformation */
    for (t=arg1, a=arg2; *t; t++) {
	c = (*t) & 0xff ;
	buf[c] = *a ;
	if (*a) a++ ;
    }
    return(0) ;
}

static int trsub(buf, text)		/* v7.6 */
/*++++++++++++++++
.PURPOSE  Execute the translation
.RETURNS  0 (no match, nathing printed) / 1 (matched)
-----------------*/
  char *buf ;	/* IN: The translation table */
  char *text ;	/* IN: to compile e.g. /[0-9]//d   to delete the quoted chars */
{
  int n = 0; int c ; char *t ;
    for (t = text; *t; t++) {
	c = *t & 0xff ;
	if (buf[c]) n++, PutText(buf+c, 1) ;
    }
    return(n) ;
}


/*==================================================================
		Unicode edition
 *==================================================================*/

static char *unicode(int u)				/* v9.0 */
/*++++++++++++++++
.PURPOSE  Echo the unicode for HTML
.RETURNS  The text (&#x...;)
-----------------*/
{
  static char buf[16] = "\10&#";
    u &= 0xffff;
    if (u < 256) sprintf(buf, "\10&#%d;", u);
    else sprintf(buf, "\10&#x%04X;", u);
    return(buf);
}

static char *edit_unicode(char *text)			/* v9.0 */
/*++++++++++++++++
.PURPOSE  Echo the unicode for HTML
.RETURNS  The text (&#x...;)
-----------------*/
{
  int u;
    if (*text == '#') text++;
    if (*text == '0') text++;
    if (*text == 'x')    	/* Hexadecimal */
         u = x2i(text);
    else u = atoi(text);
    return(unicode(u));
}

static char *edit_graph(char *text)			/* v9.0 */
/*++++++++++++++++
.PURPOSE  Echo the unicode for \graph{#}
.RETURNS  The text (&#x...;)
-----------------*/
{
  int u;
    if (*text == '#') text++;
    if (*text == '0') text++;
    if (*text == 'x')    	/* Hexadecimal */
         u = x2i(text);
    else u = atoi(text);
    return(unicode(graph_unicode[u&0xff]));
}

/*==================================================================
		Atomic elements
 *==================================================================*/
static char atomic_symbol[] = "\
  H HeLiBeB C N O F NeNaMgAlSiP S ClArK CaScTiV CrMnFeCoNiCu\
ZnGaGeAsSeBrKrRbSrY ZrNbMoTcRuRhPdAgCdInSnSbTeI XeCsBaLaCePr\
NdPmSmEuGdTbDyHoErTmYbLuHfTaW ReOsIrPtAuHgTlPbBiPoAtRnFrRaAc\
ThPaU NpPuAmCmBkCfEsFmMdNoLrRfHaSgNsHsMt" ;	/* Elements 0 --> 109 */

static char *atom(char *text)			/* v8.82 */
/*++++++++++++++++
.PURPOSE  Echo the multingual text from multilang
.RETURNS  The text (number if text allphabetic, alphabetic if numeric)
-----------------*/
{
  static char buf[40];
  int Z, I, b=0; char *p, *a;
    for (p=text; isspace(*p); p++) ;
    if (*p == '[') /* Forbidden line */ buf[b++] = *(p++) ;
    if (isdigit(*p)) {
	Z = atoi(p);
	if (Z >= (sizeof(atomic_symbol)/2)) /* too large Z */
	    return(text);
	I = Z<<1;
	buf[b++] = atomic_symbol[I++];
	buf[b++] = atomic_symbol[I];
	buf[b] = 0;
	/* if (buf[1] == ' ') buf[1] = 0; */
	while (isdigit(*p)) p++;
	if (*p == '.') {    /* Ion specified */
	    ++p; I = atoi(p);
	    if (I > Z)      /* Impossible ionization value */
		return(text);
	    strcat(buf, i2rom(I+1));
	}
	if (buf[0] == '[') strcat(buf, "]");
	return(buf);
    }
    /* Here an element name -- interpret it. Accept Fe or FE for iron.
     * If ion specified, ONLY from 3rd char. */
    if (!isalpha(*p)) return(text);
    buf[0] = toupper(*p); p++; buf[1] = ' '; buf[2] = 0;
    if (islower(*p) ||           /* V9.01: Accept NII != NiI */
       (isupper(*p) && (*p != 'I'))) {     /* Accept FE = Fe */
	buf[1] = tolower(*p); p++;
    }
    a = strstr(atomic_symbol, buf);
    if ((!a) && (buf[1] == 'i')) {  /* Maybe Oiii for Oxygen */
	buf[1] = ' '; --p;
	a = strstr(atomic_symbol, buf);
    }
    if (!a)
	return(text);
    Z = (a-atomic_symbol)/2;
    while ((*p == ' ') || (*p == '.')) p++;
    /* Interpret the ionisation */
    if (isdigit(*p)) {
	I = atoi(p);
	while (isdigit(*p)) p++;
    }
    else p += match_roman(p, &I);
    if (isgraph(*p) && (*p != ']')) /* Does not match a roman number */
	return(text);
    if (I > (Z+1))
	return(text);
    if (b) sprintf(buf, "[%d", Z);
    else    sprintf(buf, "%d", Z);
    if (I>0) {
	--I;			/* FeI corresponds to 26.0      */
	sprintf(buf+strlen(buf), Z>=10 ? ".%02d" : ".%d", I);
    }
    if (b) strcat(buf, "]");
    return(buf);
}

static char *winame(char *text)			/* v9.3 */
/*++++++++++++++++
.PURPOSE  Specification of window.open: name, size
.RETURNS  'name', '
-----------------*/
{
  static char buf[20]; static int wno=0;
  int i;
    if(isalpha(*text)) {	/* Name supplied */
	buf[0] = *text;
	for (i=1; (i<sizeof(buf)) && isalnum(text[i]); i++) buf[i] = text[i];
	if (i>=sizeof(buf)) i = sizeof(buf)-1;
	buf[i] = 0;
    }
    else sprintf(buf, "W%03d", ++wno);
    return(buf);
}

static char *winopt(char *text)			/* v9.3 */
/*++++++++++++++++
.PURPOSE  Specification of window.open: name, size
.RETURNS  'name', '
-----------------*/
{
  static char opt[] = "\
scrollbars,resizable,toolbar=0,location=0,status=0,menubar=0\
,width=64000,height=32000";
  static char *o=0;
  int len, nx=0, ny=0;
  char *p;
    if(!o) o = strstr(opt, ",width");
    *o = 0;
    if (!strmatch("*[0-9]x[0-9]*", text)) 	/* No size */
	return("");
    if (!(p = strchr(text, '_'))) p = text;
    while (isalpha(*p)||ispunct(*p)) p++;
    nx = atoi(p); p += strloc(p, 'x'); ny = atoi(p+1);
    if (nx>9999) nx = 9999;
    if (ny>9999) ny = 9999;
    p = o;
    if (nx>0) p += sprintf(p, ",width=%d", nx);
    if (ny>0) p += sprintf(p, ",height=%d", ny);
    return(opt);
}

static char *glupop(char *text)			/* v9.3 */
/*++++++++++++++++
.PURPOSE  Interpretation of glu = glutag,u args
.RETURNS  text
-----------------*/
{
  static char *buf=0; static int lbuf=0;
  int len; char *p;
    len = strlen(text)+4;
    if (len>lbuf) {
	lbuf = (len|7)+1;
	buf = buf ? realloc(buf, lbuf) : malloc(lbuf);
    }
    for (p=buf; isgraph(*text); p++, text++) *p = *text;
    *(p++) = ','; *(p++) = 'u';
    strcpy(p, text);
    return(buf);
}

/*==================================================================
		Manipulation of Dictionaries
 *==================================================================*/

static int t2cmp(e1, e2)
/*++++++++++++++++
.PURPOSE  SORT function to sort rows of a Dictionary
.RETURNS  -1 / 0 / 1
-----------------*/
  char **e1, **e2 ;	/* IN: Rows to compare */
{
    /* printf("....Compare '%s' and '%s'\n", *e1, *e2) ;  */
    return(strcmp(*e1, *e2)) ;
}

static TABLE2 *t2new(items)
/*++++++++++++++++
.PURPOSE  Allocate a new dictionnary
.RETURNS  The translation
-----------------*/
  int  items;	/* IN: Number of entries */
{
  TABLE2 *new;
    new = (TABLE2 *)malloc(sizeof(TABLE2) + (items-1)*2*sizeof(char *));
    new->buf = (char *)0;
    new->prev = 0;
    new->items = items;
    new->flags = 0;
    return(new);
}

static void t2kill(dic)
/*++++++++++++++++
.PURPOSE  Delete a Dic
.RETURNS  ---
-----------------*/
  TABLE2 *dic;	/* IN: Dictionnary to remove */
{
    if (dic->items > 0) dic->rows[0] = (char *)0 ;
    if (dic->flags&T2_BUFREE) free(dic->buf) ;
    dic->buf = (char *)0;
    dic->items = 0;
    free(dic);
    return;
}

static char *lookup(dic, s, len)
/*++++++++++++++++
.PURPOSE  Find out correspondance in e.g. texdef_dic dic
.RETURNS  The translation
-----------------*/
  TABLE2 *dic;	/* IN: The dictionnary involved	*/
  char *s; 	/* IN: String to lookup		*/
  int  len;	/* IN: Length of s		*/
{
  int i; char **a, *arg, x;
    if (len <= 0) len = strlen(s) ;
    while (dic) {
    	if (dic->flags&T2_SORTED) {	/* SORTED Dic.	*/
	    if (x = s[len]) s[len] = 0 ;
	    arg = s ;
	    a = bsearch(&arg, dic->rows, dic->items,
	    	2*sizeof(char *), (SORT_FCT)t2cmp) ;
	    if (x) s[len] = x ;
	    if (a) return(a[1]) ;
    	}
    	else for (i=dic->items, a=dic->rows; --i >= 0; a+= 2) {
    	    if (strncmp(a[0], s, len)) continue;
    	    if (a[0][len]) continue;
    	    return(a[1]);
	}
	dic = dic->prev ;
    }
    return((char *)0);
}

static int lookup2_len ;	/* Set by lookup2	*/
static char *lookup2(dic, s1, s2)
/*++++++++++++++++
.PURPOSE  Find out correspondance of "s1:s2" in e.g. texdef_dic dic
.RETURNS  The translation
-----------------*/
  TABLE2 *dic;	/* IN: The dictionnary involved	*/
  char *s1; 	/* IN: Left-part of String 		*/
  char *s2; 	/* IN: Right-part of String 		*/
{
  int i, len; char *p, **a;
    len = strlen(s1);
    while (dic) {
    	for ( i=dic->items, a=dic->rows; --i >= 0; a += 2) {
    	    if (strncmp(a[0], s1, len)) continue;
    	    if (a[0][len] != ':') continue;
	    p = a[0] + len + 1;
	    lookup2_len = strlen(p);
	    if (strncmp(p, s2, lookup2_len)) continue;
    	    return(a[1]);
    	}
	dic = dic->prev ;
    }
    lookup2_len = 0;
    return((char *)0);
}

static char *getequiv(s)
/*++++++++++++++++
.PURPOSE  Find out correspondance in dic defined by s
.RETURNS  The translation
-----------------*/
  char *s; 	/* IN: String to lookup, first byte is in c1def	*/
{
  int i, len;
    len = strlen(s);
    i = strloc(c1def, *s);
    if (c1def[i]) return(lookup(t2def[i], s+1, len-1));
    return(lookup(serv_dic, s, len));
}

static char *getequiv2(s1, s2)
/*++++++++++++++++
.PURPOSE  Find out correspondance of "s1:s2" in e.g. texdef_dic dic
.RETURNS  The translation
-----------------*/
  char *s1; 	/* IN: Left-part of String 		*/
  char *s2; 	/* IN: Right-part of String 		*/
{
  int i;
    i = strloc(c1def, *s1);
    if (c1def[i]) return(lookup2(t2def[i], s1+1, s2));
    return(lookup2(serv_dic, s1, s2));
}

static char *get_env(var)
/*++++++++++++++++
.PURPOSE  Find out the correspondance for an environment variable
.RETURNS  (Priority = specified in Dictionaries)
-----------------*/
  char *var; 	/* IN: Variable to lookup	*/
{
  char *p;
    if (p = lookup(env_dic, var, strlen(var)))
	return(p);
    return(getenv(var));
}

static int a4len(char *source, double *vlen, char *edited)
/*++++++++++++++++
.PURPOSE  Interpret the string to get a length, taking into account the Units
.RETURNS  Number of characters interpreted.
.REMARKS  Example of valid input: 0.1truecm.
	Unit added in V8.86
-----------------*/
{
  static char *prefix[] = {
    "to ", "height ", "depth ", "width ", (char *)0
  } ;
  char *s, *u, *m, **a, *unit=(char *)0;
  double plus, *alen ;
  int i ;
    s = source ;
    if (edited) *edited = 0;	/* Added V8.86 */
    if (isalpha(*s)) {		/* Special definitions, e.g. \rule */
	alen = vlen ;
	for (a = prefix; *a; a++) {
	    if (strncmp(s, *a, i = strlen(*a))) continue ;
	    s += i ;
	    while (*s == ' ') s++ ;
	    s += a4len(s, alen, (char *)0) ;
	    alen = &plus ;
	}
	return(s-source) ;
    }
    *vlen = atof(s) ;
    if (*s == '-') s++ ;
    while(isdigit(*s)) s++ ;
    if (*s == '.') for (++s; isdigit(*s); s++) ;
    /* Number is scanned. Now comes the unit... */
    if (*s == ' ') s++ ;
    u = s ;
    if (*u == '\\') s++ ;
    while(islower(*s)) s++ ;
    if (m = lookup(unit_dic, u, s-u)) {
	plus = atof(m) ;
	while (isdigit(*m)) m++;	/* V8.86 */
	if (*m == '.') for (++m; isdigit(*m); m++) ;
	if (*m && edited) 		/* V8.86: we assume unit large enough */
	    unit = m;
	/* Ignore the "flexible" length */
	while (*s == ' ') s++ ;
	if (plus == 0) {
	    u = s ;
	    while(islower(*s)) s++ ;
	    m = lookup(unit_dic, u, s-u) ;
	    if (m) plus = atof(m)  ;
	    else m = "", plus = 1. ;
	}
	*vlen *= plus ;
	if (strncmp(s, "plus ", 5) == 0) {
	    s += 5 ;
	    while (*s == ' ') s++ ;
	    s += a4len(s, &plus, (char *)0) ;
	}
	if (strncmp(s, "minus ", 6) == 0) {
	    s += 6 ;
	    while (*s == ' ') s++ ;
	    s += a4len(s, &plus, (char *)0) ;
	}
    }
    else s = u ;

    /* Edit the value */
    if (edited) {
	/* V8.87: Force in % */
        if ((*vlen<1.0) && (*vlen>0) && (!unit)) {
	    unit = "%";
	    *vlen *= 100.;
        }
	if (unit) {
	    sprintf(edited, "\"%f", *vlen);
	    /* Remove useless zeroes */
	    for (i=strlen(edited)-1; (i>0) && edited[i] == '0'; i--) ;
	    if (edited[i] != '.') ++i;
	    strcpy(edited+i, unit);
	    strcat(edited+i, "\"");
	}
	else      sprintf(edited, "%.0f", *vlen);
    }
    return(s-source) ;
}

/*==================================================================
	Reading input and keeping BUFFERs of input data
 *==================================================================*/

static char *getarg()
/*++++++++++++++++
.PURPOSE  Get the next argument, either from the main argv list (saved in
	aargs), or from the #cgi-text string (saved in args)
.RETURNS  NULL if list exhausted
.REMARKS  When an argument is starting by a newline, all remaining text
	is returned (for \cgidef)
-----------------*/
{
  char *s, *n;
    if (! args)	{	/* Arguments from MAIN */
	if (nargs <= 0) return((char *)0);
	nargs--; theflag = 3 ;
	return(*(aargs++));
    }

    theflag = 1 ;
    if (cargs) *args = cargs;
    while (isspace(*args)) {
	if (*args == '\n') { 	/* Returns all remaining bytes */
	    s = args; cargs = 0;
	    args += strlen(args);
	    return(s);
	}
	args++;
    }
    if (!*args) return((char *)0);
    for (n = args; isgraph(*n); n++) ;
    cargs = *n; *n = 0;
    s = args ; args = n;
    return(s);
}

static BUF *buf_alloc(size)
/*++++++++++++++++
.PURPOSE  Allocate a new BUF, and returns
.RETURNS  New BUF / NULL
-----------------*/
  int size;	/* IN: Size to allocate	*/
{
  BUF *new; int len;
    len = size ;
    if (len < 7) len = 7;
    len |= 7;
    new = (BUF *)malloc(sizeof(BUF) + len - 7);
    /* fprintf(_of, "....I allocated BUF %p\n", new); */
    if (! new) {
    	fprintf(_of, "\n<!-- **buf_alloc: Can't allocate %d bytes -->\n", size);
	terminate("***Not Enough Memory***"); /* return(new); */
    }
    new->size = len;
    new->used = 0;
    new->pos  = 0;
    new->flags = 0;
    new->prev = (BUF *)0;
    return(new);
}

static BUF *buf_expand(b, bytes)
/*++++++++++++++++
.PURPOSE  Expand a buffer
.RETURNS  New BUF / NULL
-----------------*/
  BUF *b;	/* IN:  Buffer to expand */
  int bytes;	/* IN: Size of increment */
{
  BUF *new; int i;

    if (!b) return(buf_alloc(bytes)) ;
    i = b->size + (bytes > 0 ? bytes : BUFINCR);
#if 0	/* Can happen from begin_env */
    if (b->flags) { char sep[2];
        fprintf(stderr, "#***buf_expand(%p,%d->%d) has flags set (%d) [",
	    b, b->size, bytes, b->flags);
	sep[0] = sep[1] = 0;
	for (i=0; i<ITEMS(texparm); i++) {
	    if(texparm[i] != b) continue;
	    fprintf(stderr, "%s#%d", sep, i);
	    sep[0] = ',';
	}
	fprintf(stderr, "]\n");
    }
#endif
    if ((new = buf_alloc(i))) {
    	new->used  = b->used;
    	new->pos   = b->pos;
    	new->flags = b->flags;
    	new->prev  = b->prev;
    	memcpy(new->text, b->text, b->used);
	new->text[new->used] = 0;
    	b->size = 0;
    	free(b) ;
    }
    else {
    	fprintf(_of,
	   "\n<!-- **buf_expand(%p, %d+%d = %d): not enough memory -->\n",
	    b, b->size, bytes, i);
	terminate("***Not Enough Memory***"); /* return(new); */
    }
    return(new);
}

static BUF *buf_append(b, text, len)
/*++++++++++++++++
.PURPOSE  Append a text in a buffer -- expand if necessary
.RETURNS  New BUF / NULL
-----------------*/
  BUF *b;	/* IN: Concerned buffer */
  char *text;	/* IN: Text to append   */
  int len;	/* IN: Length of text	*/
{
  BUF *new; int i, more;
    new = b;
    if (len < 0) len = strlen(text);
    if (!b) new = b = buf_alloc(len+1) ;
    i = b->used + len ;
    if (i >= b->size) { 			/* Must realloc ...	*/
	more = i - b->size + 1 ;
	if (more < BUFINCR) more = BUFINCR ;
	new = buf_expand(b, more);
    }
    memcpy(new->text + new->used, text, len) ;
    new->text[i] = 0;
    new->used = i;
    return(new);
}

static BUF *buf_append_env(b, text)
/*++++++++++++++++
.PURPOSE  Append a text in a buffer -- substitute the environment $...
.RETURNS  New BUF / NULL
-----------------*/
  BUF *b;	/* IN: Concerned buffer */
  char *text;	/* IN: Text to append   */
{
  int i; char *e, c;
    while (*text) {
	i = strloc(text, '$');
	b = buf_append(b, text, i);
	text += i;
	if (*text != '$') continue;
	/* Substitute e.g. $SESAME_r by its equivalent */
	text++;
	for (e=text; isalnum(*e) || (*e == '_'); e++) ;
	i = (e-text);
	c = text[i]; text[i] = 0;
	if (e = getenv(text)) b = buf_append_env(b, e, -1);
	text[i] = c;
	text += i;
    }
    return(b);
}

static BUF *buf_str3(buf, fmt, s1, s2, s3)
/*++++++++++++++++
.PURPOSE  Append a text to buf, \1 -> s1, \2 -> s2, \3 -> s3
.RETURNS  The result
-----------------*/
  BUF *buf ;	/* MOD: Original buffer */
  char *fmt; 	/* IN: "Pilot" string 	*/
  char *s1 ;	/* IN: String s1	*/
  char *s2 ;	/* IN: String s2	*/
  char *s3 ;	/* IN: String s3	*/
{
  char *p, *n;
    for (p = fmt; *p; p = n) {
	/* Length of text */
	for (n=p; (*n)&(~3); n++) ;
	if (n == p) {
	    n++;
	    if (*p == 1) p = s1;
	    else if (*p == 2) p = s2;
	    else if (*p == 3) p = s3 ;
	    buf = buf_append(buf, p, -1);
	    continue ;
	}
	buf = buf_append(buf, p, n-p) ;
    }
    return(buf) ;
}

static int buf_out(text, len)
/*++++++++++++++++
.PURPOSE  Write out into the "collector" buffer
.RETURNS  Number of bytes
-----------------*/
  char *text;	/* IN: Text to write out */
  int len;	/* IN: Length of text	 */
{
    if (! collector) collector = buf_alloc(1024) ;
    if (*text != EOF) collector = buf_append(collector, text, len) ;
    return(collector ? len : -1 ) ;
}

static char *buf_out0()
/*++++++++++++++++
.PURPOSE  Emulate the \kill
.RETURNS  Number of bytes
-----------------*/
{
    if (!collector) return("") ;
    collector->pos = collector->used ;
    if (next_char() == '\n') get_char() ;
    return("") ;
}

static char *buf_out1()
/*++++++++++++++++
.PURPOSE  Issue the contents of the "collector" buffer
.RETURNS  ""
.REMARKS  If next byte is > nn ==> skip the line
-----------------*/
{
  INT_FCT current ;
  char *b ;
  int n=0, i=0;
    if (inbuf->pos < inbuf->used)
        b = inbuf->text + inbuf->pos ;
    else b = "";
    if (*b == '>') {
        inbuf->pos += 1 ;
	b++ ;
	if (isdigit(*b)) {
	    n = atoi(b) ;
	    while (isdigit(*b)) b++, inbuf->pos += 1 ;
	}
    }
    else n = 0 ;
    counter[COUNT_tabline] += 1 ;
    if (counter[COUNT_tabline] <= n) return(buf_out0()) ;
    if (!collector) return(0) ;
    n = collector->used-collector->pos ;
    if (n <= 0) return(0) ;
    current = OutFct ;
    OutFct  = saved_OutFct ;
    /* Insert TR and enf-of-TR */
    if (b = getequiv("\\SEP1l")) {
        putstring("<TR>") ;
	i = strloc(b, '>') ;
	if (b[i]) i++ ;
	putstring(b+i) ;
    }
    putbuffer(collector->text+collector->pos, n) ;
    if (b) {
	putbuffer(b, i) ;
        putstring("</TR>\n") ;
    }
    OutFct  = current ;
    collector->used = collector->pos ;
    return("") ;
}

static int com_out(text, len)
/*++++++++++++++++
.PURPOSE  Write out in the context of Comment
.RETURNS  Number of bytes
-----------------*/
  char *text;	/* IN: Text to write out */
  int len;	/* IN: Length of text	 */
{
  char *e, *p;

    for (p=text, e=text+len; p<e; p++) switch(*p) {
	case '<': putc('[', _of) ; continue ;
	case '>': putc(']', _of) ; continue ;
	default : putc(*p,  _of) ; continue ;
    }
    return(p-text) ;
}

static int buf_new(size)
/*++++++++++++++++
.PURPOSE  Defines a new BUF, and add into BUF stack
.RETURNS  0 (OK)
-----------------*/
  int size;	/* IN: Size to allocate	*/
{
  BUF *new;
    if (new = buf_alloc(size)) {
	if (new == inbuf) { fprintf(_of,
	    "\n<!-- **PROBLEM, links in buffers identical -->\n");
	    bufpb("(buf_new/new)  ", new) ;
	    bufpb("(buf_new/inbuf)", inbuf) ;
	}
    	new->prev = inbuf;
    	inbuf = new;
    	return(0);
    }
    return(-1);
}

static int push_buf(b)
/*++++++++++++++++
.PURPOSE  Put the specified buffer in the input stack
.RETURNS  0 (OK) / -1
-----------------*/
  BUF *b;	/* IN: Concerned buffer */
{
  BUF *new ;

    for (new = inbuf; new && (new != b) ; new = new->prev) ;
    if (b->pos)	new = b ;	/* Already in use, must clone */
    if (new) 			/* Already in use, must clone */
	new = buf_append((BUF *)0, b->text,  b->used) ;
    else new = b ;
    if (! new) return(-1) ;
    new->prev = inbuf ;
    inbuf = new ;
    return(0) ;
}

static char *push_text(str, len)
/*++++++++++++++++
.PURPOSE  Push the text into a new buffer (will be processed)
.RETURNS  The input text
-----------------*/
  char *str ; 	/* IN: Text to insert	*/
  int len;	/* IN: Length of str	*/
{
    if (len < 0) len = strlen(str) ;
    if (buf_new(len+2) < 0) return((char *)0) ;
    strncpy(inbuf->text, str, len);
    inbuf->text[len] = 0;
    inbuf->used = len;
    return(inbuf->text);
}

static char *push_text3(s1, s2, s3)
/*++++++++++++++++
.PURPOSE  Push the concatenation of the 3 strings into a new buffer
.RETURNS  The input text
-----------------*/
  char *s1, *s2, *s3 ; /* IN: Text to insert	*/
{
  int ls[3] ;
  char *p ;
    ls[0] = ls[1] = ls[2] = 0 ;
    if (s1) ls[0] = strlen(s1) ;
    if (s2) ls[1] = strlen(s2) ;
    if (s3) ls[2] = strlen(s3) ;
    if (buf_new(ls[0]+ls[1]+ls[2]+2) < 0) return((char *)0) ;
    p = inbuf->text ;
    if (ls[0]) strcpy(p, s1), p += ls[0] ;
    if (ls[1]) strcpy(p, s2), p += ls[1] ;
    if (ls[2]) strcpy(p, s3), p += ls[2] ;
    inbuf->used = (p - inbuf->text);
    return(inbuf->text);
}

static char *unescaped_string(text, length)
/*++++++++++++++++
.PURPOSE  Remove the \ in TeX
.RETURNS  A new string (or the same)
-----------------*/
  char *text; 	/* IN: String to interpret */
  int length ;	/* IN: Maximal length of result */
{
  int ltext ;
  char *s, *e;
    if (!otex) return(text);
    if (!unescaped_buf) unescaped_buf = buf_alloc(1024);
    if (text == unescaped_buf->text) 	/* Don't rescan! */
	return(unescaped_buf->text) ;
    ltext = length ;
    if (ltext <= 0) ltext = strlen(text);
    if (unescaped_buf->size <= ltext) {
	unescaped_buf->size = 0; free(unescaped_buf);
	unescaped_buf = buf_alloc(ltext+1);
    }
    unescaped_buf->used = unescaped_buf->pos = 0;

    for (s = unescaped_buf->text, e=s+ltext; *text && (s<e); text++) {
	if ((text[0] == '{') && text[1] == '}') { text++; continue ; }
	else if (*text == '\\') {
	    if (text[1] == '\\') break ;
	    if (tolower(text[1] == 'e') && (text[2] == 'n') && (text[3] == 'd'))
		break ;
	    text += text[1] ? 1 : 0 ;
	}
	*(s++) = *text ;
    }
    *s = 0;
    ltext = s - (unescaped_buf->text);
    unescaped_buf->used = ltext ;
    return(unescaped_buf->text);
}

static int unescaped_length(text, len)
/*++++++++++++++++
.PURPOSE  Count the actual length of text generating 'len'
.RETURNS  Length = len + number of \ in text[0..len]
-----------------*/
  char *text;	/* IN: String to interpret */
  int len;	/* IN: Length to scan	   */
{
  char *s;
    if (!otex) return(len);
    if (text == unescaped_buf->text)
	return(len);
    for (s=text; len > 0; s++) {
	if ((s[0] == '{') && s[1] == '}') { s++; continue ; }
	if (*s == '\\') {
	    if (s[1] == '\\') break ;
	    s++ ;
	}
	len-- ;
    }
    return(s-text) ;
}

static int next_char()
/*++++++++++++++++
.PURPOSE  "Peek", i.e. tell what's the next char without getting it
.RETURNS  The character / EOF
-----------------*/
#define RETURNc(r)	return(r) \
	/* { fprintf(_of, "....next_char returns %c\n", r); return(r); }  */
{
  BUF *b, *b0; int c, leq;
  char *p, *eq, *e;
  time_t now ;
  static int error_count = 0;
  static int error_mod   = 1;

    /* Look first in input buffer */
    if (!inbuf) inbuf = buf_alloc(BUFINCR);
    if (inbuf->size == 0) {
	fprintf(_of, "\n<!-- **PROBLEM, inbuf was freed ??? -->\n");
	bufpb("(inbuf)  ", inbuf) ;
    }
    if (inbuf->pos < inbuf->used) {
	c = inbuf->text[inbuf->pos] ;
	if ((c&0xff) == 0xff) c = EOF ;
	RETURNc(c);
    }

    /* Possibly in some stacked buffer */

    b0 = inbuf ;
    while (inbuf->prev) {	/* There is something stored 	*/
	if (b0 ==  inbuf->prev) {
	    fprintf(_of, "\n<!-- **PROBLEM, links in buffers identical -->\n");
	    bufpb("(inbuf)  ", inbuf) ;
	    bufpb("(prev.)", inbuf->prev) ;
	    inbuf->prev = (BUF *)0 ;
	    continue ;
	}
	b = inbuf;
	inbuf = inbuf->prev;	/* Unstack buffers	*/
	if (inbuf->size == 0) {
	    fprintf(stderr, "\n<!-- **PROBLEM, inbuf was freed ??? -->\n");
	    bufpb("(inbuf)  ", inbuf) ;
	    bufpb("(from:)  ", b) ;
	}
	if (b->flags == 0) {  b->size = 0; free(b); }
    	if (inbuf->pos < inbuf->used) {
	    c = inbuf->text[inbuf->pos] ;
	    if ((c&0xff) == 0xff) c = EOF ;
	    RETURNc(c);
	}
    }
    if (do_option&DO_NOGET) RETURNc(EOF) ;

    /* There is nothing in input buffer. Get the full line */
    if (inbuf->flags) {		/* Buffer used to store parameters... */
	if (errors_fi++ <10) fprintf(stderr,
	    "#+++input buffer is flagged %d (prev=%p, size=%d, used=%d): %s\n",
	    inbuf->flags, inbuf->prev, inbuf->size, inbuf->used, inbuf->text);
        inbuf = buf_alloc(BUFINCR);
    }
    else {			/* Use same buffer... */
        inbuf->pos = inbuf->used = 0;
    }
    p = inbuf->text + inbuf->used ;
    e = inbuf->text + inbuf->size-1 ;	/* Max address */
  getNextChar:
    while((c = fgetc(infile)) != EOF) {
	input_bytes++;
	eq = (char *)0 ;
	/* 8-bit ascii characters !!!!	*/
	if ((c&128)  && ((okeep&128) == 0)) { int c1, u;
	    /* V9.0: Accept Unicode, transcript always as &#x... */
	    c1 = fgetc(infile);
	    if (c1 == EOF) {
		if (otex) eq = tex8[c&127];
	    }
	    if (((c&0xc0) == 0xc0) && ((c1&0xc0) == 0x80)) {
		/* Multibyte Unicode */
		u = ((c&0x1f)<<6) | (c1&0x3f);
		if ((c&0xf0) == 0xe0) {   /* 3-byte Unicode */
		    c1 = fgetc(infile);
		    if ((c1&0xc0) != 0x80) fprintf(stderr,
			"++++Invalid Unicode-multibyte x%02X-%02X-%02X\n",
			c&0xff, (u&0xbf)|0x80, c1&0xff);
		    u = (u<<6) | (c1&0x3f);
		}
		eq = unicode(u);
	    }
	    else {
		if (otex) eq = tex8[c&127];
		ungetc(c1, infile);
	    }
	}
	/* CHECK NOT AN INVALID CHAR !!! */
	else if (iscntrl(c) && ((okeep&128) == 0)) {
	    switch(c) {
	      case '\t':
	      case '\n':
		break ;
	      case '\r':
	      case 032:	/* ^Z, ignore */
		continue ;
	      default:	/* Write the exact moment, for LOGs */
		error_count++;
		now = time((long *)0) ;
		if (error_count%error_mod) continue;
		if ((error_count/error_mod) == 10) error_mod *= 10;
		fprintf(stderr,
		    "++++Invalid input char \\%03o (#%d) ignored at: %s", c,
		    error_count, ctime(&now));
		if (error_mod >= 10000) {
		    fprintf(stderr, "****Too many (%d) bad input chars!\n",
		       error_count);
		    exit(1);
		}
		continue ;
	    }
	}
	if (eq) leq = strlen(eq) ;
	else leq = 0 ;
    	if ((p+leq) >= e) {		/* I need a reallocation ... */
	    inbuf->used = (p-inbuf->text);
	    inbuf = buf_expand(inbuf, BUFINCR);
	    p = inbuf->text + inbuf->used ;
	    e = inbuf->text + inbuf->size-1 ;
	}
	if (eq) strcpy(p, eq), p += leq ;
	else *(p++) = c ;
	if (c == '\n') break ;
    }
    if (c == EOF) {			/* Could be just a signal ! */
	if (errno == EINTR) goto getNextChar;
    }
    inbuf->used = (p-inbuf->text);
    *p = 0;			/* Terminate properly the current line */
    if (inbuf->pos >= inbuf->used) RETURNc(EOF);
    RETURNc(inbuf->text[inbuf->pos]) ;
}

static int get_char()
/*++++++++++++++++
.PURPOSE  Get the next char, either from stored or from stdin
.RETURNS  The character / EOF
-----------------*/
{
  int c;
    c = next_char();
    inbuf->pos += 1;
#ifdef TRACE_MEMORY
    verify_buffers();
#endif
    return(c);
}

static int unget_char(c)
/*++++++++++++++++
.PURPOSE  Put the specified char in stack to be retrieved later
.RETURNS  0=OK
-----------------*/
  int c;	/* IN: The char to push 	*/
{
  int i;	/* Flag which asks for a new buffer */
  static char text[2] ;

    if (!c) return(0);	/* Can't insert NUL */
    if (inbuf && inbuf->pos) {
	i = inbuf->pos - 1;
	if (inbuf->text[i] == c) {
	    inbuf->pos = i ;
	    return(0);
	}
    }
    text[0] = c ;
    push_text(text, 1) ;
    return(0);
}

static TABLE2 **save_defs()
/*++++++++++++++++
.PURPOSE  Save the current definitions
.RETURNS  The saved definitions
-----------------*/
{
  TABLE2 **adef;
    adef = malloc(sizeof(t2def)) ;
    memcpy(adef, t2def, sizeof(t2def));
    return(adef) ;
}

static int restore_defs(saved_def)
/*++++++++++++++++
.PURPOSE  Save the current definitions
.RETURNS  The saved definitions
.REMARKS  saved_def not FREED.
	### WARNING, saved_t2def whould NOT be freed.
-----------------*/
  TABLE2 **saved_def ;
{
  TABLE2 *t2;
  int i, count ;
    /* Remove the loaded definitions */
    for (i=0, count = 0 ; i<ITEMS(t2def); i++) {
	/* Verify first we're not trying to remove a definition
	   which is in use...
	*/
	for (t2 = t2def[i]; t2; t2 = t2->prev) {
	    if (t2 == saved_def[i]) break;
	}
	if (t2) {	/* ERROR, a definition is still in use */
	    /* if (t2 != t2def[i]) fprintf(stderr, "++++restore_defs: %s\n",
		"trying to remove definitions still in use!");
	    */
	    continue;
	}
	while (saved_def[i] != t2def[i]) {
	    t2 = t2def[i];
	    t2def[i] = t2->prev;
	    t2kill(t2);
	    count += 1 ;
	}
    }
    return(count) ;
}

static char *save_env(str, len)
/*++++++++++++++++
.PURPOSE  Save the string in the ptexenv stack
.RETURNS  The saved string / NULL
-----------------*/
  char *str;	/* IN: String to save */
  int len;	/* IN: Length of str  */
{
  BUF *b;
    if (len<0) len = strlen(str);	/* Added V8.85 */
    if (itexenv >= ITEMS(ptexenv)) {
	putstring("\n<!-- **Too deep TeXenv (") ; putstring(i2a(itexenv)) ;
	putstring(") in save_env -->\n") ;
	return((char *)0) ;
    }
    if (b = ptexenv[itexenv]) {
    	if (b->size > len) ;
    	else b->size = 0, free(b), ptexenv[itexenv] = (BUF *)0;
    }
    if (b = ptexenv[itexenv]) ;
    else b = ptexenv[itexenv] = buf_alloc(len+1);
    if (!b) return((char *)0) ;
    itexenv += 1;
    strncpy(b->text, str, len);
    b->prev = (BUF *)0 ;
    b->used = len;
    b->pos = 0;
    b->flags |= BUF_env ;
    b->text[len] = 0;
#if DEBUG>0
    fprintf(_of, "....Saved TeXenv #%d '%s' (ipenv=%d)\n",
    	itexenv-1, b->text, texenv->ipenv);
#endif
#ifdef TRACE_MEMORY
    verify_buffers();
#endif
    return(b->text) ;
}

static char *save_parm(str, len)
/*++++++++++++++++
.PURPOSE  Save the string as the next parameter
.RETURNS  The saved parameter / NULL
-----------------*/
  char *str;	/* IN: String to save */
  int len;	/* IN: Length of str  */
{
  BUF *b;
    /* Copy the parameter into parmtex buffer */
    if (itexparm >= ITEMS(texparm)) {
	putstring("\n<!-- **Too deep TeX (") ; putstring(i2a(itexparm)) ;
	putstring(") in save_parm -->\n") ;
    	/*fprintf(_of, "\n<!**Too deep TeX (%d) in save_parm-->\n", itexparm);*/
	return((char *)0) ;
    	/* return("\377") ; */
    }
    if (b = texparm[itexparm]) {
	if (b->size<0)
	    fprintf(stderr,
	      "#***Pb texparm[%d]: prev=%p, size=%d, used=%d, pos=%d, fl=%d\n",
	      itexparm, b->prev, b->size, b->used, b->pos, b->flags);
    	if (b->size > len) ;
    	else b->size = 0, free(b), texparm[itexparm] = (BUF *)0;
    }
    if (b = texparm[itexparm]) ;
    else b = texparm[itexparm] = buf_alloc(len+1);
    if (!b) return((char *)0) ;
    itexparm += 1;
    strncpy(b->text, str, len);
    b->prev = (BUF *)0 ;
    b->used = len;
    b->pos = 0;
    b->flags |= BUF_parm ;
    b->text[len] = 0;
#if DEBUG>1
    fprintf(_of, "....Saved param #%d '%s'\n", itexparm-1, b->text);
#endif
#ifdef TRACE_MEMORY
    verify_buffers();
#endif
    return(b->text) ;
}

static char *get_texparm(options)
/*++++++++++++++++
.PURPOSE  Get the next TeX parameter (\word / { ... } )
.RETURNS  The corresponding parameter, saved.
.REMARKS  The matching {...} MUST (not any more!) BE on the same line !
	The static texparm1 is set to first character of Argument.
-----------------*/
  int options ; 	/* IN: 1 if \macro=number accepted,
			       2 to get the whole word
			    0x10 for length like 1cm plus 3pt ...
			    0x80 (MODE_soft): it's argument for \begin{}{}
			   0x100 DON't check existence of macro with number
			*/
{
  BUF *b, *bb;
  int len, depth, c, nl;
  double vlen ;
  char *o, *p, *d, *m;
  static char parm1[2] ;

    depth = 0; b = (BUF *)0; p = (char *)0;
    m = (char *)0 ;	/* Used only for length computation */
    c = get_char();
    nl =  (c == '\n' ? 1 : 0) ;
    while(isspace(c)) {
	c = get_char() ;
	if (c == '\n') nl++ ;
	if (nl >= 2) return(save_parm("", 0)) ;
    }
    texparm1 = c ;
    keep_soft_mode[3] = 0 ;	/* Set to non-zero when keep_mode iunserted */
    if (c == '{') /* '}' */ {
	if ((theMode&MODE_soft) && ((options&0x80)==0)) { /* V9.12 */
	    /* Save Argument within its Context */
	    if (!b) b = buf_alloc(1023) ;
	    keep_soft_mode[3] = MODE_soft ;
	    b = buf_append(b, keep_soft_mode, 4) ;
	}
    	o = p = inbuf->text + inbuf->pos ; nl = 0 ;
	for (depth = 1; depth ; p++) switch(*p) {
	    case '\020':
	    case '\021':
    	    case '\\': p++; continue;
    	    case '{':
	        if (depth < MAX_DEP) { depth++; continue ; }
		/* Closing brace missing ? */
    		inbuf->pos = (p - inbuf->text) ;
		b = buf_append(b, o, (p-o));
		goto case_add_brace;
    	    case '}':
	        depth--;
		continue ;
	    case_add_brace:
		bb = buf_append((BUF *)0,
		   "\n[****UnclosedBrace??****]", -1) ;
		bb = buf_append(bb, b->text, b->used) ;
		free(b) ;
		b = buf_append(bb,
	    	   "\n[****ClosingBrace(s)Inserted****]", -1);
		while (depth > 0) {
		    b = buf_append(b, /*"{"*/"}", 1) ;
	    	    depth-- ;
		}
		p-- ;
		continue ;
	    case 0:	/* End of Line */
    		inbuf->pos = (p - inbuf->text) ; nl = 1 ;
		/* if (!b) b = buf_alloc(1023); */
		b = buf_append(b, o, (p-o));
		if (b->used >= MAX_LEP) goto case_add_brace;
		if ((nl >= 2) && (counter[COUNT_long] == 0))
		    goto case_add_brace;       /* Too long definition */
		c = get_char() ;	   /* Force reading next line */
		if (c == EOF) goto case_add_brace;
		unget_char(c) ;
		if ((c == '\n') && (counter[COUNT_long] == 0))
		    goto case_add_brace;      /* Too long definition  */
		o = p = inbuf->text + inbuf->pos ;
		p--;
	}

    	inbuf->pos = (p - inbuf->text) ;
    	len = (p-o) -1;
    }
    else if (c == '[') {	/* Take param */
    	o = p = inbuf->text + inbuf->pos ;
	len = strloc(p, ']') ;
    	inbuf->pos += len ;
	if (p[len]) inbuf->pos++;
    }
    else if (c == '\\') {	/* Take macro */
    	p = inbuf->text + inbuf->pos ;
    	o = p-1;
    	if (isalpha(*p)) {
	    while(isalpha(*p)) p++;
	    if (isdigit(*p)) {
		/* e.g. \hd1000 . Convention is that
		   0x100: Keep the macro \hd1000 without check.
		   -----  If \hd1000 is not existing, assume \hd
		*/
		d = p ;
		while (isalnum(*p)) p++ ;
		if (!(options&0x100)) {		/* Check macro exists ??? */
		    if (!lookup(texdef_dic, o, (p-o))) p = d ;
		}
	    }
	}
    	else if (*p) p++;
    	inbuf->pos = (p - inbuf->text) ;
	if (*p == ' ') inbuf->pos += 1 ;	/* Gobble blank after macro */
    	len = p-o ;
    }
    else if (c == '#') {
    	p = inbuf->text + inbuf->pos ;
    	o = p-1;
	while (c == '#') {
	    if (*p == '#') p++ ;
    	    while (isdigit(*p)) p++ ;
    	    inbuf->pos = (p - inbuf->text) ;
    	    len = p-o ;
	    c = *p;
	}
    }
    else if (options&2) {
	p = inbuf->text + inbuf->pos ;
	o = p-1 ;
	while(isgraph(*p) && (*p != '\\') && (*p != '%')) p++;
	inbuf->pos = (p - inbuf->text) ;
	len = p-o;
    }
    else if (isdigit(c) || (c == '-') || (c == '.') || (options&0x10)) {
	p = inbuf->text + inbuf->pos ;
	o = p-1 ;
	len = a4len(o, &vlen, (char *)0) ;
	p = o + len ;
    	inbuf->pos = (p - inbuf->text) ;
	/* I have a length -- if the value has to be re-edited,
	   like 0.1\textwidth, set in "m" the edited number */
	for (m=o; (m<p) && (isdigit(*m)||(*m=='.')||(*m=='-')); m++)  ;
	if (m < p) m = i2a(len=vlen) ;
	else m = (char *)0 ;
    }
       		/* Do not accept \macro=value, because of \ifnum	*/
    else if ((options&1) && (c == '=')) {	/* e.g. \macro=value	*/
    	p = o = inbuf->text + inbuf->pos ;
	while(isgraph(*p) && (*p != '\\') && (*p != '%')) p++ ;
    	inbuf->pos = (p - inbuf->text) ;
	len = p-o ;
    }
    else {	/* 1 character... */
	parm1[0] = c ;
	len = 1 ;
	if ((c >= '\020') && (c <= '\021'))
	    parm1[1] = get_char(), len = 2 ;
	else if (c == EOF) len = 0 ;
	parm1[len] = 0 ;
    	o = parm1;
    }

    /* Copy the parameter into parmtex buffer */
    if (b) {
    	if (itexparm >= ITEMS(texparm)) {
	    putstring("\n<!-- **Too deep TeX (") ;
	    putstring(i2a(itexparm)) ;
	    putstring(") in get_texparm('\n") ;
	    putbuffer(o, len) ;
	    putstring("') -->\n") ;
	    terminate("****Too deep parameter") ;
    	}
	b = buf_append(b, o, (p-o));
	if (depth == 0)  		/* Remove ending brace */
	    b->used -= 1, b->text[b->used] = 0;
	if (keep_soft_mode[3]) b = buf_append(b, keep_soft_mode+4, -1) ;
	b->flags |= BUF_parm;
	texparm[itexparm++] = b;
#ifdef TRACE_MEMORY
	verify_buffers();
#endif
	return(b->text) ;
    }
    else if (m) {	/* Return the computed length */
	return(save_parm(m, strlen(m))) ;
    }
    else return(save_parm(o,len));
}

static void set_texparm(macro, origin, envori)
/*++++++++++++++++
.PURPOSE  Convert the #n argument into absolute ones
.RETURNS  ---
-----------------*/
  char *macro;	/* MOD: The macro from dictionnary	*/
  int origin;	/* IN: Offset to add to macro args	*/
  int envori;	/* IN: Offset to add to env. args	*/
{
  char *p;
    if ((origin == 0) && (envori == 0))
    	return ;	/* No change at all... 	*/
    for (p=macro; *p; p++) switch(*p) {
    	case '\020': p++; if (*p != '/') *p += envori;	continue ;
    	case '\021': p++; *p += origin;	continue ;
	default: 	/* Skip bytes if control */
	  p += charlot[(*p)&0xff];
	  continue ;
    }
}

static void reset_texparm()
/*++++++++++++++++
.PURPOSE  Reset itexparm to its value
.RETURNS  ---
.REMARKS  Changes the itexparm value
-----------------*/
{
  int c, i ;
    c = get_char() ;
    if (c < '0') {
	putstring("\n<!-- **reset_texparm with bad argument '") ;
	putchar(c) ;
	putstring("' -->\n") ;
	return ;
    }
    i = (c&0xff) - '0' ;
    c = (get_char()&0xff) - '0' ;
    if (c > itexparm) {
        putstring("\n<!-- **Change itexparm from ") ;
        putstring(i2a(itexparm)) ;
        putstring(" to ") ; putstring(i2a(c)) ;
        putstring(") in reset_texparm -->\n") ;
        c = itexparm ;
	return ;
    }
    if (texenv == (&tex_stack[i])) itexparm = c ;
    /* if (texenv == (&tex_stack[i])) itexparm -= c ; */
    else {
        tex_stack[i].iparm = c ;
        /* tex_stack[i].iparm -= c ; */
        /* fprintf(stderr, "****itexparm changed in %d (I'm in %d)\n",
        i, texenv-tex_stack) ; */
    }
}
/*==================================================================
		Load file or part of file
 *==================================================================*/

static char *load_file(name, show)
/*++++++++++++++++
.PURPOSE  Load a file completely into memory
.RETURNS  The loaded text terminated by NULL / NULL if error
-----------------*/
  char *name;	/* IN: Name of the file to load */
  int show;	/* IN: Non-zero to show errors	*/
{
  int  i;
  long count;
  char *buffer;

	/* Load file into Memory */
    i = open(name, O_BINARY);
    if (i <= 0) {
    	if (show)  {
	    perror(name),
	    putstring("\n<!-- **Can't load file: ") ,
	    putstring(name) ,
	    putstring("  -->\n") ;
	}
    	return ((char *)0);
    }
    count = lseek(i, 0L, 2); lseek(i, 0L, 0);

    if (buffer = malloc(count+1)) {
    	if (read(i, buffer, count) != count) {
    	    perror(name);
    	    free(buffer);
    	    return ((char *)0);
    	}
    	buffer[count] = 0;
    }
    close(i);
    return(buffer);
}

static char *load_until(sentinel)
/*++++++++++++++++
.PURPOSE  Load the input file until a line containing sentinel\n is found
.RETURNS  The loaded text terminated by NULL / NULL if error
-----------------*/
  char *sentinel;/* IN: the "tag" which stops */
{
  char *b, *e, *p, *s; int n, c, colno;

    p = b = malloc(BUFINCR);
    e = b + BUFINCR-2;
    s = (char *)0;
    colno = 0;

    while ((c = get_char()) != EOF) {
	if (p == e) {	/* No more room -- realloc ! */
	    n = (e-b);	/* Current size	*/
	    b = realloc(b, n+BUFINCR+2);
	    p = b+n; 	/* Current position in new buffer */
	    e = p + BUFINCR ;
	}
	colno++;
	if (s && (*s == '\0') && isspace(c)) {	/* Sentinel match */
	    p -= strlen(sentinel);
	    break;
	}
	if (c == '\n') {
	    colno = 0;
	    s = (char *)0;
	}
	*(p++) = c;
	if (colno == 1) s = sentinel;
	if (s) {		/* Compare with sentinel */
	    if (*s == c) s++;	/* Compare with sentinel */
	    else s = (char *)0;
	}
    }
    *p = 0;	/* Terminate string */

    return(b);
}

static int color(char *text)
/*++++++++++++++++
.PURPOSE  Find the color (as rgb) from /usr/lib/X11/rgb.txt
.RETURNS  Color (r<<16)|(g<<8)|b / -1 for error
.REMARKS  text assumed to be in correct case...
	A set of up to 16 colors names is saved to speed up the retrieval.
-----------------*/
{
  static char *rgb_file ;
  static char *mycols[17] ;
  char *p, *t, *col, **a ;
  int vcol ;

    if (strchr(text, ',')) {		/* Form r,g,b */
	vcol = atoi(p = text) << 16 ;
	while (isdigit(*p) || isspace(*p)) p++ ; if (*p) p++ ;
	vcol |= atoi(p) << 8 ;
	while (isdigit(*p) || isspace(*p)) p++ ; if (*p) p++ ;
	vcol |= atoi(p) ;
	return(vcol) ;
    }
    if ((*text == '#') || isdigit(*text)) 	/* #RRGGBB or 0xRRGGBB */
	return(x2i(text)) ;

    if (strlen(text) == 6) {			/* RRGGBB */
	vcol = 0 ;
	for (p=text; isxdigit(*p); p++) ;
	if (!*p) return(x2i(text)) ;
    }

    /* Load the color file */
    if (!rgb_file) {
	rgb_file = load_file(RGB, 1) ;
	if (!rgb_file) return(-1) ;
	/* for (p=rgb_file; *p; p++) *p = tolower(*p) ; */
    }

    /* Look first in the list of saved colors	*/
    for (a=mycols; *a; a++) {
	for (t=text, p=*a; *t==*p; t++, p++) ;
	if ((!*t) && (*p == '\n')) {	/* Colour found ! */
	    p = *a ; while ((p>rgb_file)&&(*p != '\n')) p-- ;
	    if (*p == '\n') p++ ;
	    while (isspace(*p)) p++; vcol = atoi(p)<<16; while(isgraph(*p)) p++;
	    while (isspace(*p)) p++; vcol |= atoi(p)<<8; while(isgraph(*p)) p++;
	    while (isspace(*p)) p++; vcol |= atoi(p);    while(isgraph(*p)) p++;
	    return(vcol) ;
	}
    }
    /* Step in file to look for specified color */
    col = rgb_file ;
    while (col) {
	if (*col == '\n') col++ ;
	if (ispunct(*col)) {
	    col = strchr(col, '\n') ;
	    continue ;
	}
	p = col ; while (isspace(*p)) p++ ;
	while (isgraph(*p)) p++ ;	/* Skip red   */
	while (isspace(*p)) p++ ;
	while (isgraph(*p)) p++ ;	/* Skip Green */
	while (isspace(*p)) p++ ;
	while (isgraph(*p)) p++ ;	/* Skip Blue  */
	while (isspace(*p)) p++ ;
	for (t=text; *t==*p; t++, p++) ;
	if ((!*t) && (*p == '\n')) {	/* Colour found ! */
	    p = col ;
	    while (isspace(*p)) p++; vcol = atoi(p)<<16; while(isgraph(*p)) p++;
	    while (isspace(*p)) p++; vcol |= atoi(p)<<8; while(isgraph(*p)) p++;
	    while (isspace(*p)) p++; vcol |= atoi(p);    while(isgraph(*p)) p++;
	    if ((a-mycols) < ITEMS(mycols)-1) {		/* Save color */
	        while (isspace(*p)) p++ ;
		*a = p ;
	    }
	    return(vcol) ;
	}
	col = strchr(col, '\n') ;
    }
    return(-1) ;
}

/*==================================================================
		Dealing with Services
 *==================================================================*/

static int print_envariable(name)
/*++++++++++++++++
.PURPOSE  Print the environment variable (or original); remind that
	the variable may be written {name}
.RETURNS  The scanned length
-----------------*/
  char *name;     /* IN: name to translate */
{
  char *p, *e, c, has_braces;
    p = name;
    if (*p == '$') p++;
    e = p;
    if (has_braces = (*e == '{'))
	 p++, e += strloc(name, '}');
    else while(isid(*e)) e++;
    c = *e; *e = 0;
    if (p = get_env(p)) PutString(p);
    else {
	PutChar('$'); /* if (has_braces) PutChar('{'); */
	PutString(name); if (has_braces) PutChar('}');
    }
    if (c) *e = c;
    if (has_braces) e++;
    return(e-name);
}

static void load_serv1(list, opt)
/*++++++++++++++++
.PURPOSE  Load the services from the buffer
.RETURNS  ---
-----------------*/
  char *list;	/* MOD: The definition to load, written as load_service */
  int opt ;	/* IN: T2_... option */
{
  TABLE2 *t2 ;
  char *p, *anp, *apup, clob ;
  int i, i2, error ;

    error = 0 ; apup = (char *)0;
    if (opt&T2_BUFDUP) list = strdup(list), opt |= T2_BUFREE ;
    p = list ;
    i = strloc(c1def, *p);	/* Type of definition */
    t2 = (TABLE2 *)malloc(sizeof(TABLE2)) ;
    t2->buf = list ;
    t2->items = 1 ; t2->flags = opt&T2_BUFREE ;
    t2->rows[0] = ++p ;

    /* Where does the definition stop ? */
    clob = 0 ;
    switch(*list) {
      case '$':
	p += strloc(p, '=');
	if (*p) *(p++) = 0 ;
	break ;
      case '<':	/* HTML Definitions: find out the LAST '>' */
	while (*p) {
	    i2 = strloc(p, '>');
	    if (! p[i2]) break;
	    p += i2+1 ;
	}
	if (*p) *(p++) = 0;
	break ;
      case '\'':
	p += strloc(p, '\'') ;
	if (*p) *(p++) = 0;
	break ;
      case '\\':	/* TeX Definitions  */
	p += 1;	/* At least one char */
	while (isgraph(*p)) p++;
    	while (isspace(*p)) *(p++) = 0 ;
	clob = '\021' ;
	break ;
      case '{':
	while (isgraph(*p)) p++;
	if (p[-1] == '}') p[-1] = 0;
    	while (isspace(*p)) *(p++) = 0 ;
	clob = '\020' ;
	break ;
      case ':':
	while (isgraph(*p)) p++;
	if (p[-1] == ':') p[-1] = 0;
    	while (isspace(*p)) *(p++) = 0 ;
	break ;
      default:
	while (isgraph(*p)) p++;
    	while (isspace(*p)) *(p++) = 0 ;
	break ;
    }
    t2->rows[1] = p ;

    if (clob && (*p == '#')) {	/* Modify the Arguments */
	anp  = apup = p+1 ;	/* Check no argument higher than np */
	error = 1 ;
	while (*p) {
	    /* Beware, the \# is OK ... */
	    if ((p[0] == '\\') && (p[1] == '#')) { p += 2; continue; }
	    if (charlot[*p]) error = 0, p += charlot[*p] ;
	    else if (*p == '#') {
		*p = clob ;
		if (p[1] > *apup) apup = p+1 ;
	    }
	    p++ ;
	}
	if (*apup <= *anp) 	/* Not too many parameters... */
	    error = 0 ;
    }
    /* New definitions on top */
    if (error) {
	putstring("\n<!-- **Can't interpret definition of ") ;
	clob = apup[-2]; apup[-2] = 0 ;
	putstring(list) ;
	apup[-2] = clob ;
	putstring(": mismatch in number of arguments -->\n") ;
	free(t2) ;
    }
    else t2->prev = t2def[i], t2def[i] = t2;
}

static void load_services(list, opt)
/*++++++++++++++++
.PURPOSE  Load the services from the buffer
.RETURNS  ---
.REMARKS  opt has T2_... values
   Lines starting by - are options -- which are executed;
   replaced by # after execution.
-----------------*/
  char *list;	/* MOD: The loaded buffer */
  int  opt ;	/* IN: Flag */
{
  int i, i2, len, n ;
  char *p, *e, *t, *next, *amaxarg, **a;
  int counts[sizeof(c1def)] ;
  TABLE2 *dic[sizeof(c1def)], *t2 ;
  char hash_value, maxarg, clob;
  /* clob = flag to remove trailing spaces until the closing brace */

    if (! tex_ok['0']) for (i=0; i<sizeof(tex_ok); i++) switch(i) {
	case '\\': case '{':  case '}': case '#': case '%':
	/* case '<':	// Can be a problem in GLU tags... */
	case '$':  case '^': case '_': continue ;
	default: if (isprint(i)) tex_ok[i] = 1;
    }
#if DEBUG>1
    fprintf(_of, "....tex_ok loaded\n");
#endif

    /* Duplicate the input string if necessary */
    if (opt&T2_BUFDUP) list = strdup(list), opt |= T2_BUFREE  ;

    len = strlen(list);
    for (i=0; i<ITEMS(counts); i++) counts[i] = 0, dic[i] = (TABLE2 *)0;

    /* Count the number of services */
    for (p=list, e=p+len; p<e; p = next) {
	i = strloc(p, '\n'); p[i] = 0; next = p+i+1;
	if (!ispunct(*p)) {	/* Can be a continuation...	*/
	    if (p > list) p[-1] = '\n';
	    continue;
	}
	if (*p == '-') {	/* v8.3: Option */
	    t = setopt(p);
	    *p = '#';
	    if (t && *t) {	/* Option includes file! */
		fflush(_of);
		fprintf(stderr, "****Recursive definition in a file: ");
		perror(t) ;
		fprintf(_of, "****Recursive definition in a file: %s\n", t);
		exit(1);
	    }
	}
	if (*p == '#') continue;

	/* Which dictionary is defined by the first byte ;
	   however, the default is the continuation of preceding line.
	*/
	i = strloc(c1def, *p);
	if (!c1def[i]) { if (p > list) p[-1] = '\n'; continue ;  }
	if (c1def[i])  counts[i] += 1;
    }

    /* Allocate the dictionnaries, and set up the pointers */
    for (i=0, p=list; i<ITEMS(counts); i++) {
	if (counts[i] == 0) continue;
	dic[i] = t2 = t2new(counts[i]);
	t2->buf = p;
	p = (char *)0;	/* A single dictionnary only refers to list */
	t2->prev = t2def[i]; t2def[i] = t2;
    }
#if DEBUG>1
    fprintf(_of, "....Dictionnaries allocated\n");
#endif

    /* Remark : flags is used (temporarily) as counter	*/
    for (p=list, e=p+len; p<e; p = next) {
	next = p + strlen(p) + 1;
	if (!ispunct(*p)) 	continue ;
    	if (*p == '#') 		continue ;
	i = strloc(c1def, *p);
	if (!c1def[i]) 		continue ;
	t2 = dic[i];
	t2->rows[t2->flags] = ++p; t2->flags += 1 ;
	t2->rows[t2->flags] = (char *)(-1);	/* Not Set */
	hash_value = clob = 0;
	/* Locate the entry / explanation (pointed by p) */
	switch(c1def[i]) {
	  case '$':
	    i2 = strloc(p, '=');
	    if (p[i2]) {	/* Prefer environment if existing */
	    	p[i2] = 0;
		if (t = getenv(p)) t2->rows[t2->flags] = t;
		p += i2+1;
	    }
	    break ;
	  case '<':	/* HTML Definitions: find out the corresponding '>' */
	    while (*p) {
		i2 = strloc(p, '>');
		if (! p[i2]) break;
		p += i2+1 ;
	    }
	    if (*p) *(p++) = 0;
	    t2->rows[t2->flags] = (char *)0;
	    break ;
	  case '\'':
	    p += strloc(p, '\'') ;
	    if (*p) *(p++) = 0;
	    break ;
	  case '\\':	/* TeX Definitions  */
	    p += 1;	/* At least one char */
	    while (isgraph(*p)) p++;
	    clob = hash_value = '\021' ;
	    break ;
	  case '{':
	    while (isgraph(*p)) p++;
	    if (p[-1] == '}') p[-1] = 0;
	    clob = hash_value = '\020' ;
	    break ;
	  case ':':
	    while (isgraph(*p)) p++;
	    if (p[-1] == ':') p[-1] = 0;
	    break ;
	  default:
	    while (isgraph(*p)) p++;
	    break ;
	}
	/* Remove trailing spaces if clob set to 1 */
	if (clob) {
	    for (t=p+strlen(p)-1 ; isspace(*t); t--) ;
	    if (*t == /*{*/ '}') for (++t; isspace(*t); t++) *t = 0 ;
	}
	/* Move to the explanation, and transform '#' char */
	if (t2->rows[t2->flags] == (char *)(-1)) {
    	    while (isspace(*p) && isascii(*p)) *(p++) = 0;
	    /* Option T2_amp1 & T2_amp2: used for unicode_symb:
	       keep either first explanation, or second one
	    */
	    if ((*p == '&') && (opt&(T2_amp1|T2_amp2))) {
		if ((opt&T2_amp1) == 0) {	/* Skip first equivalence */
		    t = strchr(p+1, '&');
		    if (t) p = t;
		}
		/* Remove the following & */
		if (t = strchr(p+1, '&')) *t = 0;
	    }
	    t2->rows[t2->flags] = p;
	    t = p;				/* t -> rewriting addr */
	    amaxarg = (char *)0;
	    if (hash_value && (*p == '#')) {	/* First '#' to change */
		amaxarg = p+1;
	        maxarg = p[1] ;
		if (maxarg == '0') 		/* Can use \\GET1arg   */
		      maxarg = '9' ;
		*(t++) = hash_value; p++;
		*(t++) = *(p++);
		while (isspace(*p)) p++;	/* If blanks before {} */
	    }
	    else hash_value = maxarg = 0 ;
	    for (; *p; t++, p += i2) {
		if (!hash_value) { i2 = strput(p, t); continue ; }
		if (*p == '\\') {
		    i2 = strput(p, t);
		    if (i2 == 1) *++t = *++p ;	/* To keep \#	*/
		    continue ;
		}
		i2 = 1 ;
		*t = *p ;
		if ((*p == '#') && isdigit(p[1])) { 	/* V8.74 */
		    if (p[1]>maxarg) {
			fprintf(stderr,
			  "++++Argument #%c outside limits for macro \\%s\n",
			  p[1], t2->rows[t2->flags-1]) ;
			if (amaxarg)		/* v8.12 increase argcount */
			  *amaxarg = p[1];
		    }
		    *t = hash_value;
		    continue;
		}
		if (charlot[*p]) *++t = *++p ;
	    }
	    *t = 0;
	}
	t2->flags += 1 ;
    }

    /* Sort the Dictionnary if there are many (> 20) definitions */
    for (i=0, p=list; i<ITEMS(counts); i++) {
    	if (counts[i] == 0) continue ;
    	t2 = dic[i] ;
    	t2->items = t2->flags>>1 ;
    	t2->flags = 0 ;
	if (opt&T2_BUFDUP) {	/* Allocated buffer: keep the flag */
	    t2->flags |= T2_BUFREE ;
	    opt &= ~T2_BUFDUP ;	/* ... but buffer allocated once ! */
	}
    	if (t2->items >= 20) {		/* SORT */
    	    if (opt&T2_SORTED) {	/* Just verify it's sorted */
    	    	/* printf("....Compare %d items\n", t2->items) ;  */
    	    	for (i2 = t2->items, a = t2->rows; --i2 > 0; a += 2) {
    	    	    if (t2cmp(a, a+2) >= 0) fprintf(stderr,
    	    	        "****Macro list badly sorted '%s' vs '%s'\n",
    	    	        a[0], a[2]) ;
    	    	}
	    }
    	    else qsort(t2->rows, t2->items, 2*sizeof(char *), (SORT_FCT)t2cmp) ;
    	    t2->flags = T2_SORTED ;
    	}
    }

    /* Setup the Counters Count...  */
    if (counts[i = strloc(c1def, '+')]) {
	t2 = dic[i] ;
	for (a=t2->rows, i=t2->items; --i >= 0; a += 2) {
	    n = atoi(a[1]) + 1 ;
	    if (used_counters < n) used_counters = n ;
	    counter[n-1] = 0 ;
	}
    }

    /* Find out the TAG */
    if (!tag2html) tag2html = get_env("TAG2HTML");
    if (!tag2html) tag2html = "URL_" ;

    return ;
}

static char *print_services(why)
/*++++++++++++++++
.PURPOSE  Display the known services
.RETURNS  NULL
-----------------*/
  char *why ; 	/* IN: Title */
{
  int i, n, t, b; char **a, *p; TABLE2 *t2;
  static char services[] = ":%&" ;	/* Services to show */
  char *pserv;

  if (olis < 2) {
    fprintf(_of, "<!-- ========List of services from %s options -->\n", why);
    fprintf(_of, "<!-- --List of <{$variables}: -->\n");
    fprintf(_of, "<!-- $TAG2HTML=%s\t-->\n", tag2html);
    for (a=aaenv, i=strlen(tag2html); *a; a++) {
	if (strncmp(tag2html, *a, i) == 0) fprintf(_of, "<!-- $%s\t-->\n", *a);
    }
    for (n=0, t2 = env_dic ; t2->prev ; t2 = t2->prev) n++;
    for (t2 = env_dic ; t2 ; t2 = t2->prev, n--) {
	fprintf(_of, "<!-- ....(Dictionnary #%d).... -->\n", n);
    	for (i=t2->items, a=t2->rows; --i>=0; a += 2) {
	    if (! a[1]) continue ;
    	    fprintf(_of, "<!-- $%s=%s\t-->\n", a[0], a[1]);
	}
    }

    for (pserv=services; *pserv; pserv++) {
        fprintf(_of, "<!-- --List of <{services '%c'}: -->\n", *pserv);
        for (n=0, t2 = serv_dic ; t2->prev ; t2 = t2->prev) n++;
	i = strloc(c1def, *pserv);
        for (t2 = t2def[i] ; t2 ; t2 = t2->prev, n--) {
	    fprintf(_of, "<!-- ....(Dictionnary #%d).... -->\n", n);
    	    for (i=t2->items, a=t2->rows; --i>=0; a += 2) {
    	        fprintf(_of, "<!--  %-12s\t", *a);
    	        for (p=a[1]; *p; p++) {
		    if (*p == '$') {
		        p++;
		        if ((*p == '{' /*}*/) || isalpha(*p))
			     p += print_envariable(p);
		        else PutChar('$');
		        p-- ;
		    }
		    else PutChar(*p);
	        }
    	        fprintf(_of, "\t-->\n");
	    }
        }
    }

    if (otex==1) { int priority;	/* V8.73 */
    	puts("<!-- --List of recognized TeX macros (accents not listed) -->");
	for (t2 = texdef_dic, priority=0; t2 ; t2 = t2->prev, priority++) {
	    fprintf(_of, "<!-- Priority %d (highest=0):", priority);
	    t = 3;	/* Number of tabs on the current line */
    	    for (i=t2->items, a=t2->rows; --i>=0; a += 2) {
	        p = a[1]; n = 0 ;	/* Count # arguments */
		if ((*p >= '\020') && (*p <= '\021')) n = p[1]-'0';
		t += 1+((strlen(a[0])+1+2*(n>0))/8);
		if (t>=8) fputc('\n', _of), t=0;
    	        fprintf(_of, "\t\\%s", a[0]);
	        if (n > 0) fprintf(_of, "#%d", n);
	    }
            fprintf(_of, "\t-->\n");
	}
    	puts("<!-- --List of recognized TeX Environments -->");
	for (t2 = texenv_dic, priority=0; t2 ; t2 = t2->prev, priority++) {
	    fprintf(_of, "<!-- Priority %d (0 is highest):", priority);
	    t = 3;
    	    for (i=t2->items, a=t2->rows; --i>=0; a += 2) {
		t += 1+((strlen(a[0])+3)/8);
		if (t>=8) fputc('\n', _of), t=0;
    	        fprintf(_of, "\t{%s}", a[0]);
	    }
            fprintf(_of, "\t-->\n");
	}
    }

    if (ohtml) {
    	puts("<!-- --List of recognized HTML tag templates -->");
	for (t2 = html_dic ; t2 ; t2 = t2->prev) {
	    fprintf(_of, "<!-- ");
	    for (i=t2->items, a=t2->rows; --i>=0; a += 2)
		fprintf(_of, "\t<%s", a[0]);
	    fprintf(_of, "\t-->\n");
	}
    }

    if (oglu) {
    	puts("<!-- --List of recognized GLU datatypes -->");
	for (t2 =  glu_dic ; t2 ; t2 = t2->prev) {
	    fprintf(_of, "<!-- ");
	    for (i=t2->items, a=t2->rows; --i>=0; a += 2)
		fprintf(_of, "\t<%%%s>", a[0]);
	    fprintf(_of, "\t-->\n");
	}
    	puts("<!-- --List of recognized GLU actions   -->");
	for (t2 = aglu_dic ; t2 ; t2 = t2->prev) {
	    fprintf(_of, "<!-- ");
	    for (i=t2->items, a=t2->rows; --i>=0; a += 2)
		fprintf(_of, "\t<&%s>", a[0]);
	    fprintf(_of, "\t-->\n");
	}
    }
    puts("<!-- ========(End of list of Services)===========------------ -->");
  }

    if (olis>1) {	/* List Complete TeX Actions */
	printf("\n{...Explanations about some TeX macros and Environments}\n%s",
		explain_macros);
    	puts("\n====Full Set of TeX Definitions");
	for (t2 = texdef_dic; t2 ; t2 = t2->prev) {
    	    for (i=t2->items, a=t2->rows; --i>=0; a += 2) {
    	        fprintf(_of, "\\%-16s ", a[0]);
	        p=a[1]; n = 0 ;
		if ((*p >= '\020') && (*p <= '\021'))
			n = p[1]-'0', p += 2;
	        if (n > 0) fprintf(_of, "#%d", n);
		for (; *p; p++) {
		    if (isprint(*p)) { putchar(*p); continue ; }
		    switch(*p) {
		      case '\1':  fprintf(_of, "{-accent-}"); continue ;
		      case '\2':  fprintf(_of, "{TypeFace}"); continue ;
		      case '\3':  fprintf(_of, "{--Font--}"); continue ;
		      case '\4':  fprintf(_of, "{-parmod-}"); continue ;
		      case '\5':  fprintf(_of, "{-beg/end}"); continue ;
		      case '\16':
		      case '\6':  ++p; fprintf(_of, "{-Fct#%02o-}", *p);
				  continue ;
		      case '\7':
			++p; fprintf(_of, "{Mode=%03o}", *p&0xff); continue ;
		      case '\17':
			++p; fprintf(_of, "{SET1mode:%c}", *p); continue ;
		      case '\30':  	/* v8.3 */
			++p; fprintf(_of, "{Mode&%03o}", *p&0xff); continue ;
		      case '\37':
			++p; fprintf(_of, "{Mode|%03o}", *p&0xff); continue ;
		      case '\10': ++p;
		      	if (isprint(*p)) fprintf(_of, "{'%c'}", *p);
		      	else fprintf(_of, "'\\%03o'", (*p&0xff)) ;
		      	continue ;
		      case '\033': ++p; fprintf(_of, "{%s}",
		      		*p == '{' /*}*/? "push" : "pop"); continue ;
		      case '\020':
		      case '\021': ++p; putchar('#') ;
			if (isprint(*p)) putchar(*p);
			else fprintf(_of, "%d", *p);
			continue ;
		    }
		    fprintf(_of, "{\\%03o}", *p&0xff); continue;
		}
		putchar('\n') ;
	    }
	}
    	puts("{...Full Set of TeX Environments (default is <env>)}");
	for (t2 = texenv_dic; t2 ; t2 = t2->prev) {
    	    for (i=t2->items, a=t2->rows; --i>=0; a += 2)  {
    	        fprintf(_of, "{%s}", a[0]);
		for (b=16-strlen(a[0]); --b>=0; putchar(' ')) ;
		for (p=a[1]; *p; p++) {
		    if (isprint(*p))  { putchar(*p);  continue ; }
		    if (*p == '\021') { putchar('#'); continue ; }
		    if (*p == '\020') { putchar('#'); continue ; }
		    if (*p == '\007') { ++p;
					fprintf(_of, "{Mode=%03o}", *p&0xff);
					continue ; }
		    if (*p == '\037') { ++p;
					fprintf(_of, "{Mode|%03o}", *p&0xff);
					continue ; }
		    fprintf(_of, "{\\%03o}", *p&0xff); continue;
		}
		putchar('\n') ;
	    }
	}
	wait_pipe(0);
	exit(0) ;
    }
    return((char *)0);
}

static void help()
/*++++++++++++++++
.PURPOSE  Display help, then exit
.RETURNS  ---
-----------------*/
{
static char text[] = /* \1 indicates the OGLU */ "\n\
This program converts a text into HTML -- plain HTML or for the GLU system.\n\
The context is normally specified in the input by lines starting by \n\
\\cgidef{ [plain|html|tex|end] (options...) \n\
          (definitions of TeX macros until the closing }\n\
or by the options:\n\
  -f file: read definitions from the specified file\n\
     -bib: bibcodes   are recognized; in GLU default -bib%R. =target window\n\
     -cat: Catalogues are recognized; in GLU default -cat%E. =target window\n\
     -glu: Generates tags for the GLU system - gluw with target window (w)\n\
      -D : Insert definition on the command line, e.g. -D'\\ifpub=\\iffalse'\n\
      -%X: Don't tag lines starting by %X ('X' may be any user-chosen letter)\n\
    -HELP: List a full help about cgiprint known TeX macros\n\
    -html: input contains basic HTML tags to be kept\n\
      -ic: Ignore Comments (don't output comment lines)\n\
     -lis: lists the known service tags as HTML tags\n\
    -LIST: Lists all known TeX definitions, and stops\n\
    -mail: UserName@Address are transformed (in GLU as <%M >\n\
  -n name: to override the '(stdin)' input filename designation\n\
  -nomath: to avoid to italicize the mathematical mode\n\
    -oglu: filter the output with \1\n\
  -o prog: filter the output with specified output program\n\
           (prog includes '&' to separate the arguments)\n\
     -pre: input is pre-formatted: blanks/linebreaks are meaningful\n\
     -sec: secure: only files starting by magic '\\cgidef{' are accepted\n\
     -tex: input contains LaTeX (avoid \\def's, and use \\cgidef)\n\
    -tex2: input contains `abstract-like' TeX (also named 'soft-TeX')\n\
    -v###: set HTML version (defaut 3.0; no=2 doesn't know tables)\n\
Each line of the def_file (-f option) contains, with starting byte\n\
    $ (dollar): a definition for an environment variable\n\
    : (colon):     a definition for a service\n\
    % (percent):   a definition of a GLU data tag\n\
    & (ampersand): a definition of a GLU action tag\n\
    < (less_than): a regular expression defining valid HTML tags\n\
    \\ (backslash): a TeX definition; if n parameters, the definition\n\
                    must be #n{....}\n\
    { (left brace): a TeX environment definition}\n\
    # (hash): a comment, or context-definition when #cgi-text\n\n\
    blank or tab: continuation from preceding line\n\
    Non-ascii characters are written \\x.. (backslash-x-hexa value)\n\
The following environment variables are used:\n\
    SESAME_r: which name resolvers to use in \\listofobjects (default SNV)\n\
\n\
A more complete documentation can be found at:\n\
http://cdsarc.u-strasbg.fr/doc/man/cgiprint.htx\n\
";
  int i;

#if 0
static char anchor_rule[] = "\
An anchor rule is made of two columns, service_name and translation,\n\
    the translation may include definitions ($), \n\
    or reference to the argument ($0), \n\
    or to the i-th subfield of the argument as $i or $/i (separator /)\n\
    or to the i-th subfield of the argument as $.i (separator .)\n\
    or to subfields of the arguments from #i as $$.i (separator .)\n\
" ;
#endif

    printf("%s\n", usage);
    i = strloc(text, '\1'); text[i] = 0;
    printf("%s%s%s", text, OGLU, text+i+1);
    text[i] = '\1';

    if ((!otex) && (!ohtml)) {
    	puts("More about recognized TeX with -tex -lis options, and");
    	puts("More about recognized HTML tags with -html -lis options");
    	return;
    }
    else if (! olis) print_services("default");
}

/*==================================================================*
		Management of HTML Tags
 *==================================================================*/

static void push_tag(tag)
/*++++++++++++++++
.PURPOSE  Insert a tag in the stack
.RETURNS  ---
-----------------*/
  char *tag;	/* IN: Tag to stack	*/
{
#if DEBUG>1
    fprintf(_of, "....push_tag %p(%s)\n", tag, tag);
#endif
    if (itag_stack >= MAX_TAG) {
	putstring("\n<!-- **Can't push tag '") ; putstring(tag) ;
	putstring("' at depth "); putstring(i2a(itag_stack)) ;
	putstring(" -->\n") ;
    }
    else tag_stack[itag_stack] = tag;
    itag_stack++;
}

static void pop_tag()
/*++++++++++++++++
.PURPOSE  Write the stacked tag
.RETURNS  ---
-----------------*/
{
  char *tag; int omode ;
    tag = "";
    if (itag_stack <= 0) { static int count=0;
	 count++;
	 if ((count<10) || ((count%100)==0)) {
	     putstring("\n<!-- **Can't pop tag '") ; putstring(tag) ;
	     putstring("' from depth "); putstring(i2a(itag_stack)) ;
	     putstring(" x"); putstring(i2a(count));
	     putstring(" -->\n") ;
	 }
    }
    else {		/* V7.5 */
	--itag_stack ;
	if (itag_stack < MAX_TAG) tag = tag_stack[itag_stack];
    }
#if DEBUG>1
    fprintf(_of, ".... pop_tag %p(%s)\n", tag, tag);
#endif
    if (!tag) return ;
    if (*tag == '\3') {	/* End of Font	*/
	omode = theMode&MODE_intag; theMode |= MODE_intag ;
    	putstring("</FONT>");
	if (omode==0) theMode &= ~MODE_intag;
    	return;
    }

    if (*tag == '\2')	/* Typeface */ tag++;
    /* Close tag -- done with first word only */
    if (isascii(*tag) && isprint(*tag)) {
	omode = theMode&MODE_intag; theMode |= MODE_intag ;
    	putchar('<');
    	if (*tag == '<') tag++;
    	putchar('/');
    	if (*tag == '/') tag++;
    	while (isgraph(*tag)) {
	    if (*tag == '{') /*}*/ break ;
	    putchar(*tag), tag++;
	}
    	if (tag[-1] != '>') putchar('>');
	if (omode==0) theMode &= ~MODE_intag;
    }
}

/*==================================================================*
		Management of TeX Tables
 *==================================================================*/

/* The cell start/end are managed by a special stack, because
   a TeX macro can include several columns...
   But a single char is just enough to know which type of cell !
*/
static char tab_stack[MAX_TSK] ;	/* Saves the 'D' or 'H' */
static int  tab_taglev[MAX_TSK] ;	/* Saves the depth in itag_stack */
static int itab_stack = 0 ;
static void push_tab(c)
/*++++++++++++++++
.PURPOSE  Stack a TAB separator
.RETURNS  ---
-----------------*/
  int c;
{
static char fmt[] = "\
<!-- ** Problem tab_stack (non-closed TD?), itab_stack=%d -->";
    if (itab_stack >= MAX_TSK) {  char buf[sizeof(fmt)+20];	/* Mod. V8.9 */
	sprintf(buf, fmt, itab_stack);
	putstring(buf);
	fflush(stdout);
    }
    else {
	tab_stack[itab_stack] = c ;
	tab_taglev[itab_stack] = itag_stack ;
    }
#if 0	/* For test */
    if (itab_stack>0) printf("<!-- push_tab(%c): itab_stack=%d -->", c,
	    itab_stack);
    fflush(stdout);
#endif
    itab_stack++ ;
}

static void pop_tab()
/*++++++++++++++++
.PURPOSE  Pop the previous TAB separator
.RETURNS  ---
-----------------*/
{
#if 0
    if (itab_stack>1) printf("<!-- pop_tab(): itab_stack=%d -->", itab_stack);
    fflush(stdout);
#endif
    if (itab_stack > 0) itab_stack-- ;
    if (itab_stack < MAX_TSK) /* Close the <B> etc */
    	while (itag_stack > tab_taglev[itab_stack])  pop_tag();
    putstring("</T") ;
    if (itab_stack < MAX_TSK) putchar(tab_stack[itab_stack]) ;
    else putchar('D') ;
    putchar('>') ;
}

static char *table_open(str)
/*++++++++++++++++
.PURPOSE  Start a new table
.RETURNS  Table declaration
-----------------*/
  char *str;	/* IN: The environment Qualification */
{
  BUF *parm; char *p; int i, border ;
  char env[160] ;

    /* Verify it's really a table ! */
    if (! *str) return(str) ;

    /* Is there enough room to start another table ? */
    if (texenv->itable >= MAX_TAB) {
	putstring("\n<!-- **Can't create a new table '"); putstring(str);
	putstring("' to depth "); putstring(i2a(MAX_TAB)) ;
	putstring("  -->\n") ;
	terminate("****TeX to HTML loop ? ");
    }
    texenv->itable += 1;

    /* If more than 1 parameter (tabular*), the last parameter
       contains the columns specifications
    */
    for (i = texenv->ipenv + 1; i < itexenv; i++) {
	parm = ptexenv[i-1] ;
	ptexenv[i-1] = ptexenv[i] ;
	ptexenv[i] = parm ;
    }

    parm = ptexenv[texenv->ipenv-1] ;	/* Table Descriptor (v8.14: -1) */
    tabenv[texenv->itable] = parm ;
    parm->pos = 0;

    /* A new output_routine ...    */
    theOutputRoutine = table_out ;

    /* Determine the type of table */
    if (atof(vhtml) < 3.0) theMode |= MODE_tab2 ;

    /* Mode is BASELINE if a dimension, or a 'p', exists */
    border = strloc(parm->text, 'p') ;
    if (!parm->text[border]) border = strloc(parm->text, '{'/*}*/) ;
    if (parm->text[border]) theMode |= MODE_baseline ;

    /* Compute the border parameter */
    for (border=0, p=parm->text; *p; p++) {
	if (isalpha(*p)) break;
	if (*p == '|') border++;
    }

    if (theMode & MODE_tab2) {
	if (begin_more) free(begin_more), begin_more = (char *)0 ;
	return("<PRE>") ;
    }

    sprintf(env, "%s BORDER=%d", str, border);

    /* Don't forget the HTML arguments saved in begin_more */
    if (begin_more) {
	parm = buf_append((BUF *)0, env, -1) ;
	parm = buf_append(parm, " ", 1) ;
	parm = buf_append(parm, begin_more, -1) ;
	free(begin_more) ;
	begin_more = (char *)0 ;
	p = save_env(parm->text, parm->used) ;
	free(parm) ;
	return(p) ;
    }
    /* There could be options set by \taboptions */
    else if (p = lookup(texdef_dic, "thetabopt", -1)) {
	i = strlen(env);
	if (*p) env[i++] = ' ';
        strncpy(env+i, p, sizeof(env)-i);
	env[sizeof(env)-1] = 0;
    }
    return(save_env(env, strlen(env)));
}

static int table_putsep(o)
/*++++++++++++++++
.PURPOSE  Issue what's between columns
.RETURNS  0
-----------------*/
  int o;	/* IN: Option 1 to issue  the separation text */
{
  BUF *parm; char *p; int len ;

    parm = tabenv[texenv->itable] ;
    if (!parm) {	/* Trying opening a row OUTSIDE a table ? */
	fprintf(_of, "\n<!-- **Trying to put column sep outside table ? -->\n");
	return(0) ;
    }
    pending = 0 ;

    for (p = parm->text + parm->pos ; *p; p++) {
	if (isalpha(*p)) break;
	if (*p == '{') {	/*} Skip what's between { ... }	*/
	    ++p ;
	    p += strbrace(p) ;
	    if (! *p) p-- ;
	    continue ;
	}
	if ((p[0] == '@') && (p[1] == '{')) {	/* }@{text}	*/
	    if (!(theMode & MODE_tab2)) continue ;
	    p += 2; len = strbrace(p) ;
	    if (o) push_tex("\07\01", (char *)0) ;   /* <PLAIN> */
    	    if (p[len]) len++;
    	    if (o) do_text(p, len, do_option|DO_NOCLOSE);
	    /* pop_tex("\07\01") ; Triggered by closing brace 	*/
	    p += (len-1) ;
	    continue ;
	}
    	if (! (theMode&MODE_tab2)) continue ;
    	if (o) putchar(*p) ;
    }
    parm->pos = p - parm->text ;
    return(0) ;
}

static int table_advance(n)
/*++++++++++++++++
.PURPOSE  Advance n columns in table
.RETURNS  Number of columns moved.
-----------------*/
  int n;
{
  BUF *parm; char *p; int i ;

    parm = tabenv[texenv->itable] ;
    if (!parm) {	/* Trying opening a row OUTSIDE a table ? */
	fprintf(_of, "\n<!-- **Trying to change cell outside table ? -->\n");
	return(0) ;
    }
    for (i=0; i<n; i++) {
    	table_putsep(0);
    	p = parm->text + parm->pos ;
    	if (! *p) break;
    	if (isalpha(*p)) parm->pos += 1 ;
    }
    return(i);
}

static int table_cclose()
/*++++++++++++++++
.PURPOSE  Close a table cell
.RETURNS  0 / -1
.REMARKS  The number of defined parameters is not modified...
-----------------*/
{
  int itexparm0 ;
    itexparm0 = itexparm ;
    if (!(theMode & MODE_col)) {	/* The column is empty..*/
	table_cell("p", "") ;
	/* if ((theMode & (MODE_emptycell|MODE_tab2)) == 0)
	    putstring("<BR>") ;
	*/
    }
    if (theMode & MODE_col) {
#if 0
    	if (!(theMode & MODE_tab2)) pop_tex((char *)0) ;
#else
	/* if ((theMode & (MODE_emptycell|MODE_tab2|MODE_out1c)) == 0) */
	if (((theStatus&MODE_out1c)==0) &&
	    ((theMode  & (MODE_emptycell|MODE_tab2)) == 0))
	    putstring(emptycell), theStatus |= MODE_out1c ;

	if (!(theMode & MODE_tab2)) 	/* Close what was opened */
	    pop_tab() ;

	/* Report the output of significative char in cell to
	   output of significative char in document
	*/
	if (theStatus & MODE_out1c) theStatus |= MODE_out1d ;
	theMode   &= ~(MODE_col|MODE_intag) ;
	theStatus &= ~(MODE_out1c) ;
#endif
    	table_advance(1) ;
	Flush;
    }
    itexparm = itexparm0 ;
    table_out("", 0) ;		/* Force pending setup	*/
    return(0) ;
}

static void table_ropen()
/*++++++++++++++++
.PURPOSE  Start a new Row
.RETURNS  0 / -1
-----------------*/
{
  BUF *parm; TEX_ENV *t; char *p; int i;
  char env[160];
    pending = 0 ;
    parm = tabenv[texenv->itable] ;
    if (!parm) {	/* Trying opening a row OUTSIDE a table ? */
	fprintf(_of, "\n<!-- **Trying to open a row outside table ? -->\n");
	return ;
    }
    t = texenv ;			/* Definition of <TABLE>  */

    Flush ;
    if (!(theMode & MODE_row)) {	/* Row not yet started 	  */
    	theMode |= MODE_row ;		/* Modify here, but....	  */
    	if (!(theMode & MODE_tab2)) { int itexenv0;
	    strcpy(env, "<TR");

       if (strstr(t->env,"class='sort'") ){
	   static char class[] = " class='tuple-1'";
	   strcat(env, class);
	   class[sizeof(class)-3] ^= 3;	/* Transform 1->2 and 2->1 */
       }

	    if(theMode & MODE_baseline) strcat(env, " VALIGN=BASELINE");
            /* There could be options set by \taboptions */
	    p = arg1row;
	    if (!p) p = lookup(texdef_dic, "therowopt", -1);
	    if (p) {
		i = strlen(env);
		if (*p) env[i++] = ' ';
		env[i++] = ' ';
        	strncpy(env+i, p, sizeof(env)-i);
		env[sizeof(env)-1] = 0;
		env[sizeof(env)-2] = 0;
		if (arg1row) { free(arg1row); arg1row = (char *)0; }
    	    }
	    strcat(env, ">");
	    itexenv0 = itexenv;
	    push_tex(save_env(env, strlen(env)), (char *)0);
	    texenv->ipenv = itexenv0;	/* Number of used texenv */
#if 0
	    push_tex( theMode & MODE_baseline ?
	       "<TR VALIGN=BASELINE>" : "<TR>",
	       (char *)0);
#endif
	}
	t->mode &= ~MODE_row ;		/* ... reset here */
    	parm->pos = 0;
    	table_putsep(1) ;		/* Issue beginning of row */
    	table_out("", 0) ;		/* Force pending setup	*/
    }
    return ;
}

static int table_rclose()
/*++++++++++++++++
.PURPOSE  Close a table row (open it if not yet done)
.RETURNS  0 / -1
-----------------*/
{
    if (!(theMode & MODE_row)) table_ropen() ;
    /* if(theMode & MODE_col) */
    	table_cclose() ;
    if(theMode & MODE_row) {
    	table_putsep(1);
	if (!(theMode & MODE_tab2)) {
	    itexenv = texenv->ipenv;	/* Added v8.11 */
	    pop_tex((char *)0) ;
	}
    }
    return(0) ;
}

static char *table_close(str)
/*++++++++++++++++
.PURPOSE  Close a table
.RETURNS  The HTML environment
-----------------*/
  char *str;	/* IN: The env definitions, starting by \07     */
{
    if (theMode & MODE_row) table_rclose();
    pending = 0 ;
    return("") ;
}

static int table_cell(align, opt)
/*++++++++++++++++
.PURPOSE  Start a new Cell
.RETURNS  0 / -1
-----------------*/
  char *align;	/* IN: Align option (overrides table's alignement) */
  char *opt;	/* IN: Further options */
{
  BUF *parm;
  char *p, *env, from_tabular ;
  char abuf[80] ;
  double w ;

    parm = tabenv[texenv->itable] ;
    if (!parm) {	/* Trying opening a row OUTSIDE a table ? */
	fprintf(_of, "\n<!-- **Trying to open a cell outside table ? -->\n");
	return(0) ;
    }

    if (!(theMode & MODE_row)) 		/* Row not yet started 	  */
    	table_ropen() ;
    pending = 0 ;
    table_putsep(1) ;			/* Issue beginning of cell */

    from_tabular = 0 ;
    p = parm->text + parm->pos ;
    if (!align) align="";
    if (!*align) align = p, from_tabular = 1 ;
    if (!opt) opt = "";
    if (!(theMode & MODE_tab2)) {
    	switch(*align) {
    	  case 'h': env =  "TH"; break;
          case 'H': env =  "TH class='sort-colheader'"; break;
    	  case 'l': env =  "TD ALIGN=LEFT NOWRAP"; break;
    	  case 'r': env =  "TD ALIGN=RIGHT NOWRAP"; break;
    	  case 'c': env =  "TD ALIGN=CENTER NOWRAP"; break;
    	  case 'L': env =  "TD ALIGN=LEFT NOWRAP VALIGN=MIDDLE"; break;
    	  case 'R': env =  "TD ALIGN=RIGHT NOWRAP VALIGN=MIDDLE"; break;
    	  case 'C': env =  "TD ALIGN=CENTER NOWRAP VALIGN=MIDDLE"; break;
    	  case 't': env =  "TD VALIGN=TOP"; break;
    	  case 'T': env =  "TD ALIGN=CENTER VALIGN=TOP"; break;
    	  case 'P': env =  "TD VALIGN=MIDDLE"; break;
    	  case 'b': env =  "TD VALIGN=BOTTOM"; break;
    	  case 'B': env =  "TD ALIGN=CENTER VALIGN=BOTTOM"; break;
    	  default:  env =  "TD"; break;
	}

	push_tab(env[1]) ;
	putchar('<') ;
	putstring(env) ;
	if ((align[1] == '{') /*}*/ && isdigit(align[2])) {
	    align += 2 ;
	    /* w = atof(align) ; */
	    align += a4len(align, &w, abuf) ;
	    /* if (w <= 1.) sprintf(abuf, "%.0f%%", w*100.) ;
	    else sprintf(abuf, "%.0f", w) ; */
	    putstring(" WIDTH=") ;
	    putstring(abuf) ;
	    if (*align == /*{*/ '}') align++;
	}
	else if (p[0] && p[1] == '{' /*}*/ && isdigit(p[2])) {
	    p += 2; p += a4len(p, &w, abuf);
	    /* if (w <= 1.) sprintf(abuf, "%.0f%%", w*100.); */
	    putstring(" WIDTH="); putstring(abuf);
	}
	if (*align) align++ ;	/* Look Vertical Alignement */
	if (from_tabular) env = (char *)0 ;
	else switch(*align) {
    	  case 'c': env = "CENTER"; break ;
    	  case 't': env = "TOP"; break;
    	  case 'b': env =  "BOTTOM"; break;
    	  default:  env = (char *)0 ;
	}
	if(env) {	/* Vertical Alignement	*/
	    putstring(" VALIGN=") ;
	    putstring(env) ;
	}
	if (*opt) {	/* Concatenate env and opt */
	    putchar(' ') ;
	    putstring(opt) ;
	}
	/* Concatenate the backgroud color */
	if(bg1col) {
	    putchar(' ');
	    putstring(bg1col);
	    free(bg1col);
	    bg1col = (char *)0;
	}
	putchar('>') ;
    }
    theMode |= MODE_col ;
    theMode &= ~MODE_intag ;
    theStatus &= ~MODE_out1c ;
    return(0);
}

static int table_out(str, len)
/*++++++++++++++++
.PURPOSE  Write out a text in TABLE environment
.RETURNS  Number of non-blank bytes written
.REMARKS  The 'pending' variable is set
-----------------*/
  char *str;	/* IN: String to write */
  int len;	/* IN: How many bytes  */
{
  char *p, *e ;
    if (!(theMode & MODE_tab)) 		/* Table mode hidden..*/
    	return(putbuffer(str, len)) ;
    while ((len > 0) && isspace(*str)) putchar(*str), str++, len-- ;
    if (len == 0) {
	Flush ;
    	pending = !(theMode & MODE_col) ;
    	return(0) ;
    }
    pending = 0 ;
    if (!(theMode & MODE_col)) 	/* Cell not yet started	*/
    	table_cell("", "");
    if ((*str & 0xff) == 0xff) return(EOF) ;
    for (p=str, e=p+len; p<e; p++) {
	/* Verify something already output in the curent data cell */
	if (isgraph(*p) && ((theMode&MODE_intag) == 0))
	    theStatus |= MODE_out1c;
#if 0
	if (((theStatus&MODE_out1c) == 0) && isgraph(*p) &&
	    ((theMode&MODE_intag) == 0)) {
	    if (isspace(*p)) ;	/* No Valid Character Output Yet   */
	    else theStatus |= MODE_out1c ;
	}
#endif
	putchar(*p) ;
    }
    /*stat = putbuffer(str, len); */
    Flush ;
    return(p-str) ;
}

static char *table_multicol(nc,al/*,text*/)
/*++++++++++++++++
.PURPOSE  Multicolumn
.RETURNS  The equivalence text
-----------------*/
  char *nc;	/* IN: Number of columns */
  char *al;	/* IN: Alignment definition */
  /*char *text;	// IN: The text		*/
{
  char span[120], *p, *s; int n=1;
    for (p=nc; isspace(*p); p++) ; Flush;
    if (isdigit(*p)) {
	n = atoi(p);
	while (isdigit(*p)) p++ ;
	while (isspace(*p)) p++ ;
    }
    if (n > 1) sprintf(span, " COLSPAN=%d", n);
    else span[0] = 0 ;
    s = span + strlen(span) ;
    if (*p && ((strlen(span)+strlen(p)) < sizeof(span)-2))
	sprintf(s, " %s", p) ;
    table_cell(al, span);
    if (n > 1) table_advance(n-1) ;
    /* table_out(text, strlen(text)); */
    /* table_cclose() ;		The next & provokes a <TD>... */
    return("") ;
}

static char *table_hline()
/*++++++++++++++++
.PURPOSE  Output a Horizontal Rule in Table Env.
.RETURNS  The equivalence text
-----------------*/
{
    Flush ;
    if ((theMode&MODE_tab) && (!(theMode&MODE_tab2)))
	return("") ;
    return("\10<HR\10>") ;
}

/*==================================================================*
		Open/Close TeX Environments
 *==================================================================*/

static char *tex_open(s)
/*++++++++++++++++
.PURPOSE  Open a TeX Environment
.RETURNS  The HTML tag to stack
-----------------*/
  char *s;	/* IN: The env definitions, starting by \07	*/
{
  BUF *b ;
    if (*s == '\07') s += 2;
    /* Execute the context-dependent part */
    if_mode(MODE_tab) return(table_open(s)) ;
    if_mode(MODE_VERB) otex |=/*=*/ 0x10 ;
    if (begin_more) {	/* Concatenate the Environment */
	b = buf_append((BUF *)0, s, -1) ;
	b = buf_append(b, " ", 1) ;
	b = buf_append(b, begin_more, -1) ;
        s = save_env(b->text, b->used) ;
	free(b) ;
	free(begin_more) ;
	begin_more = (char *)0 ;
    }
    return(s) ;
}

static char *tex_close(s)
/*++++++++++++++++
.PURPOSE  Close a TeX Environment
.RETURNS  The HTML tag to stack
-----------------*/
  char *s;	/* IN: The env definitions, starting by \07	*/
{
    if (*s == '\07') s += 2;
    /* Execute the context-dependent part */
    if_mode(MODE_tab) return(table_close(s)) ;
    if_mode(MODE_VERB) otex &= ~0x10 /*, otex |= 0x01*/ ;
    return(s) ;
}

/*==================================================================*
		Convert to HTML
 *==================================================================*/

static int print_subfield(text, select, html)
/*++++++++++++++++
.PURPOSE  Write out text converted for URLs, eventually split
.RETURNS  Number of bytes interpreted in select.
.REMARKS  $$0 = Crypt the Argument
-----------------*/
  char *text;	/* IN: Text to write out */
  char *select;	/* IN: Field selector, e.g. .2 (second field when separator=.),\
			$2 (all fields from second one)... */
  int html;	/* IN: Option '1' to convert special characters,
			like the arguments of a href call */
{
  static char crypt[8];
  char *p, sep; int i, m, old_mode;
    crypt_char = 0;
    p = select;
    if (m = (*p == '$')) p++;	/* multiple fields   */
    sep = '/';			/* Default separator */
    if (ispunct(*p)) sep = *(p++);
    p += str2int(p, &i);
    if ((i == 0) && m) {	/* $$0 = Crypt (mailto) */
	old_mode = theMode;
	crypt_char = '@';
	while (*text && m) {		/* v8.16 */
	    theMode |= MODE_verb1;
	    if (*text == crypt_char) PutChar(crypt_char);
	    else if (*text == '?') { 	/* v8.16 */
	        PutChar(*text);
		m = 0;			/* STOP! */
	    }
	    else {
		sprintf(crypt, "&#%d;", (*text)&0xff);
	        PutString(crypt);
	    }
	    text++;
	}
	theMode = old_mode;
	if (!*text) return(p-select);	/* v8.16: ?Subject in Mail */
	sep = 0; 			/* v8.16 */
    }
    if (i == 0) m = 1;		/* $0 = all fields   */

    while (sep && (--i > 0)) { 	/* Go to first field */
	text += strloc(text, sep);
	if (*text) text++;
    }
    if (m) sep = 0;		/* Forces to go to end */

#if 0	/* Changed v8.2 */
    if (html) html = (theMode&MODE_Aarg) ^ MODE_Aarg ;
    if (html) theMode ^= MODE_Aarg ;
    while(*text && (*text != sep)) i =  *(text++), PutChar(i);
    if (html) theMode ^= MODE_Aarg ;
#else
    old_mode = theMode&MODE_href;
    if (html) theMode = (theMode&(~MODE_href)) | MODE_hrefa;
    while(*text && (*text != sep)) i =  *(text++), PutChar(i);
    if (html) theMode = (theMode&(~MODE_href)) | old_mode;
#endif
    return(p-select);
}


/*==================================================================*
		Special TeX Functions
 *==================================================================*/

/* The TeX functions are saved	*/
static int   null_out() { return(0); }
static char *null_fct() { return(""); }

static void setimode(mode) int mode;
{
    switch(mode) {
      case ' ':	/* NOTHING */  	break;
      case 'H':
	theMode |= MODE_HTML;	break ;
      case 'T':
	theMode |= MODE_tab ; 	break ;
      case 'q': case 'p':
	theMode &= ~(MODE_soft|MODE_math|MODE_MATH);
	theMode |= MODE_plain ;	break ;
      case 'V':
	theMode |= MODE_VERB;	break ;
      case 'P':
	theMode &= ~MODE_obeylines;
	theMode |= MODE_PRE ;	break ;
      case 'S':
	theMode |= MODE_soft ;	break ;
      case 'C':
	theMode |= MODE_col ;	break ;
      case 'R':
	theMode |= MODE_row ;	break ;
      case 'm':
	theMode |= MODE_math ;	break ;
      case 'M':
	theMode |= MODE_MATH ;	break ;
      case 'c':
	theMode |= MODE_emptycell ;	break ;
      case 'h':		/* Convert e.g. < into &lt; */
	/* theMode &= ~MODE_plain;	Added V9.44 */
	theMode |= MODE_2html ;	break ;
      case 'j':		/* In href="javascript:..." */
	theMode |= MODE_js;	break ;
      case '`':		/* Convert ' into \&pos; < into \&lt;   */
	theMode |= MODE_jsquo;	break ;
      case 'o':
	theMode |= MODE_obeylines ;	break ;
      case 't': /* For Links */
	theMode |= MODE_intag;	break ;
      case 'a': /* Encode the cgi arguments -- but not the & # */
	theMode |= MODE_hrefa /* MODE_Aarg */;	break ;
      case 'g': /* For glutag*/
	theMode |= MODE_Aglu;	break ;
      case '\"': /* For Quotes in Quotes */
	theMode |= MODE_quot;	break ;
      case '\'': /* For being in Javascript Argument */
	theMode |= MODE_jsArg;	break ;
      case '1':
      case '?': /* For Links */
	/* theMode |= MODE_intag; */
	theMode |= MODE_href1 /* MODE_Aargq */;	break ;
      case '%': /* For Links */
	theMode |= MODE_xArg;	break ;
      case '#': /* For Links */
	theMode &= ~(MODE_href|MODE_xArg)
		/* ~(MODE_Aarg|MODE_Aargq|MODE_Aarga)*/ ;
	push_text("\10#", 2) ;
	break ;
      case '-': /* Ignore */
	theMode |= MODE_ignore;	break ;
      case 'l': case 'L':	/* Lowercase */
	theMode = (theMode&(~(MODE_upper|MODE_lower))) | MODE_lower; break ;
      case 'u': case 'U':	/* Uppercase */
	theMode = (theMode&(~(MODE_upper|MODE_lower))) | MODE_upper; break ;
      case '0': case '.':
	theMode = 0 ; break ;
      case 'w':	/* Wait before starting rows etc... */
	pending = 0 ; break ;
    }
}

static char *set1mode(arg) char *arg;
{
  char c;			/* v8.15 */
    if (arg && *arg) {
	while (*arg) setimode(*(arg++)) ;	/* V9.25: several modes */
    }
    else setimode(get_char());	/* v8.15; mod. V9.25 */
    return("") ;
}

static char *mode4tex() { 	/* For \begin{TeX} ... \end{TeX} */
    /* We specify here the bits which have to stay when entering
       \begin{tex} */
    theMode &= MODE_plain|MODE_2html|MODE_href/*MODE_apos*/|MODE_quot
	    |MODE_href|MODE_xArg/*MODE_Aarg|MODE_Aargq|MODE_Aarga*/|MODE_Aglu;
    return("");
}
static char *set_emptycell()   { theMode |=  MODE_emptycell ; return(""); }
static char *clear_emptycell() { theMode &= ~MODE_emptycell ; return(""); }
static char *debug(arg) char *arg ;
{
    printf("\n<!-- DEBUG '%s' Mode=%X Stacks: tag=%d, tex=%d, &=%d",
	arg, theMode, itag_stack, (int)(texenv - tex_stack), itab_stack) ;
    printf(" -- itexparm=%d, itexenv=%d -->\n",
     	itexparm, itexenv) ;
    return("") ;
}
static char *ed1buf(char *index) 	/* Edit one buffer */
{
  int i;
    i = atoi(index);
    return((i<0) || i >= ITEMS(val1buf) ? "" : val1buf[i]);
}
static char *set1buf(char *index, char *value)	/* Set one buffer */
{
  int i;
    i = atoi(index);
    if((i<val1buf_2free) || i >= ITEMS(val1buf))	/* ERROR !! */
	return("****set1buf error****");
    if (val1buf[i]) free(val1buf[i]);
    val1buf[i] = rescan21(value, -1);
    return("");
}
#if 0		/* Old code */
static char *theProgram()  { return(theprogram) ; }
static char *theFile()  { return(thefile) ; }
static char *theHTMLversion() 		/* text if plain   */
{
    return(vhtml) ;
}
#endif
static char *thePlain()
{
    if (*vhtml < '3') return("plain");
    return("") ;
}
static char *tex_def() ;
static char *tex_defenv() ;
static char *table_multicol() ;
static char *table_hline() ;
static char *print_services() ;
static char *exec_cgi() ;
static char *exec_input() ;
static char *length(str) char *str ;	/* Convert 0.1in into pixels */
{
  double w ; static char abuf[40] ;
    a4len(str, &w, (char *)0) ;
    if ((w>0) && (w<=1.)) sprintf(abuf, "%.0f%%", w*100.) ;
    else sprintf(abuf, "%.0f", w) ;
    return(abuf) ;
}
#if 0
static char *match_else()
/*++++++++++++++++
.PURPOSE  Change the status
.RETURNS  ---
.REMARKS  Triggered by \else
-----------------*/
{
    theMode ^= MODE_false ;
    theMode &= ~(MODE_ignore) ;
    if (theMode&(MODE_ifori|MODE_false)) theMode |= MODE_ignore ;
    return("") ;
}
#endif
static char *xcolor(str) char *str ;	/* Color in hexadecimal */
{
  static char ax[20]; int i ;
    i = color(str) ;
    if (i == -1) return(str) ;
    sprintf(ax, "\\#%06X", i) ;
    return(ax) ;
}
static char *reset_alarm() { alarm(max_time); reset_alarm_ticks++; return(""); }
static char *basename(filename) char *filename ;
{
  char *p ;
    for (p = filename + strlen(filename) ;
	(p > filename) && (p[-1] != '/'); p-- )  ;
    return(p) ;
}
static char *dirname(filename) char *filename ;
{
  char *a, *p; static char name[128];
    /* Look first if the sequence "/./" can be found... */
    for (p=filename; *p; p++) {
	if (*p != '/') continue ;
	if (p[1] != '.') continue ;
	++p;
	if (p[1] == '/') { p += 2; break ; }
    }
    if (!*p) p = filename ;
    if (Arel) {			/* There is a root to relative names... */
	strcpy(name, Arel);
	a = name + strlen(name) ;
	*(a++) = '/' ;
    }
    else a = name ;
    strncpy(a, p, sizeof(name)-1-(a-name)); name[sizeof(name)-1] = 0;
    for (p=name + strlen(name); (p>name) && (*p != '/'); p--) ;
    *p = 0;
#if DEBUG
    fprintf(_of, "\n----Dirname: Arel=%s, return %s\n", Arel ? Arel:"(nil)",
	name) ;
#endif
    if (p == name) return(".") ;
    return(name) ;
}
static char *typein(msg) char *msg ;
{
  char answer[BUFSIZ] ;
    if (isatty(0) && (isatty(1)|isatty(2))) ;
    else {
	fprintf(_of, "\n<!-- **\\typein can't work, not a terminal -->\n") ;
	return("\377\377") ;
    }
    fprintf(isatty(1) ? stdout : stderr, "\n\\typein{%s}: ", msg) ;
    if (!fgets(answer, sizeof(answer), stdin)) answer[0] = 0; /* Error */
    return(strdup(answer)) ;
}
static char *ifmode(char *mode)	/* Calls only when Mode is on */
{
  int omode, cond; char *p;
    omode = theMode; theMode = 0;
    for (p=mode; *p; p++) setimode(*p);
    /* TRUE if ANY of the specified modes is true */
    cond = omode & theMode;
    theMode = omode;
    match_if(cond);
    return("");
}
#if 0		/* Replace by ifmode */
static char *ifmmode()	/* Calls only when MMode is on */
{
    match_if(theMode&(MODE_math|MODE_MATH)) ;
    return("") ;
}
#endif
static char *ifnum(c1, op, c2)  char *c1, *c2, *op ;	/* Compare */
{
  double v1, v2 ;
  int cond ;
    v1 = atof(c1) ; v2 = atof(c2) ;
    switch(*op) {
      case '=': cond = v1 == v2 ;  break ;
      case '<': cond = v1 <  v2 ;  break ;
      case '>': cond = v1 >  v2 ;  break ;
      default:
      	fprintf(_of, "\n<!-- **Bad comparison %s '%s' %s -->\n", c1, op, c2) ;
      	return("") ;
    }
    match_if(cond) ;
    return("") ;
}
static char *ifargop()			/* If Exists Optional Argum. */
{
  BUF *b; char *o;
    b = inbuf; if (b->prev) b = b->prev;
    o = b->text + b->pos;
    match_if(*o == '[') ;
    return("") ;
}
static char *ifgen(s1, s2)	/* If generic or \ifx, mod. V9.06 */
  char *s1, *s2 ;
{
  int i;
    if (s1 && (*s1 == '\21')) {
	i = s1[1]-'1';
	if (i>=0&& texparm[i] && texparm[i]->used)
	   return(ifgen(texparm[i]->text, s2)) ;
    }
    if (s2 && (*s2 == '\21')) {
	i = s2[1]-'1';
	if ((i>=0) && texparm[i] && texparm[i]->used)
	    return(ifgen(s1, texparm[i]->text)) ;
    }
    i = 0;
    if (s1 && (*s1 == '\\')) { i++; s1 = lookup(texdef_dic, s1+1, -1); }
    if (s2 && (*s2 == '\\')) { i++; s2 = lookup(texdef_dic, s2+1, -1); }
    if (!s1) s1 = lookup(texdef_dic, "undefined", -1);
    if (!s2) s2 = lookup(texdef_dic, "undefined", -1);
    if (i == 0) match_if(strdiff1(s1, s2) == 0);
    else match_if(s1 == s2);
    return("") ;
}
static char *ifmatch(pattern, string)	/* If string Matches Pattern */
  char *pattern, *string ;
{
    match_if(strmatch(pattern, string)) ;
    return("") ;
}
static char *ifregexp(regexp, string)	/* If string Matches RegExpr */
  char *regexp, *string ;
{
  char buf[BUFSIZ] ; char ending_char;
    if (*regexp == '/') ending_char = *(regexp++) ;
    else ending_char = 0 ;
    pregexp = regexp ;
    compile(regexp, buf, buf+sizeof(buf), ending_char);
    /* circf is set by compile */
    match_if(strexp(buf, string, circf)) ;
    return("") ;
}
static char *regexp(cmd, string)/* If string Matches RegExpr */
  char *cmd, *string ;
{
  char buf[BUFSIZ] ; char ending_char, *tmp;
  INT_FCT old_routine;
  static BUF *mycollector ;
  BUF *old_collector ;
    /* Save the current settings */
    old_routine = theOutputRoutine ;
    old_collector = collector ;
    theOutputRoutine = buf_out ;
    collector = mycollector ;
    if (collector) collector->used = 0 ;
    /* Remove special formatting flags */
    push_tex((char *)0, (char *)0) ;
    theMode = MODE_HTML; /*(MODE_VERB|MODE_plain) ;*/
    /* Interprets the '\21' arguments if any */ 	/* v7.6 */
    if (strchr(string, '\021')) string = tmp = rescan21(string, -1) ;
    else tmp = (char *)0 ;

    /* Examine the conversion command -- might be substitute or 'y' */
    while (isspace(*cmd)) cmd++ ;
    if (*cmd == 'y') {			/* TRANSLATE */	/* V7.6 */
	cmd++;
	while (isspace(*cmd)) cmd++;
	trcompile(cmd, buf) ;
	trsub(buf, string) ;
    }
    else {
	if (*cmd == 's') cmd++ ;
        ending_char = *(cmd++) ;
        pregexp = cmd ;
        compile(cmd, buf, buf+sizeof(buf), ending_char);   /* pregexp -> / */
        /* circf is set by compile */
        /* if (*pregexp == '/') pregexp++ ; */
        regsub(buf, pregexp, ending_char, string) ;
    }

    /* Terminate: restore the environment */
    if (tmp) free(tmp) ;
    pop_tex("") ;
    mycollector = collector ;
    theOutputRoutine = old_routine ;
    collector = old_collector ;
    return((mycollector ? mycollector->text : "")) ;
}

static char *getargquoted()		/* e.g. \verb'sdcs'	*/
{
  BUF *b ;
  int c, len ; char *o, *p ;
    b = inbuf->prev ;
    if (!b) return("") ;
    p = b->text + b->pos;
    c = (*p)&0xff ;	/* 1st Char */
    if (!c) return("") ;
    if (len = charlot[c]) {
	b->used += len ;
	push_text(p, len) ;
	return("") ;
    }
    o = ++p ;
    while (*p && (*p != c)) p++ ;
    push_text(o, p-o) ;
    if (*p == c) p++ ;
    b->pos = (p - b->text) ;
    return("") ;
}

static char *get1arg1 ()	/* Get one argument (facultative or not) */
{
  BUF *saved_inbuf, *bp=(BUF *)0;
  char *o ;
  int depth = 0 ;
  int type  = 0 ;
    o = inbuf->text + inbuf->pos ;
    while (*o == '-') depth++, o++ ;
    if (*o) type = *(o++) ;
    inbuf->pos = o - inbuf->text ;
    if (!inbuf->prev) return("") ;

    /* We have to keep the link between the stak of input buffers ---
       therefore bp is the buffer which points to the actual input */
    saved_inbuf = inbuf ;
    while ((depth-- >= 0)  && (inbuf->prev))
	bp = inbuf, inbuf = inbuf->prev ;	/* Move to input level */
    o = inbuf->text + inbuf->pos ;
    switch(tolower(type)) {
      case 'o':		/* Optional argument */
	if (*o == '[') get_texparm(0) ;
	else save_parm("", 0) ;
	break ;
      case 'l':		/* Get a flexible length */
    	if (isalnum(*o) || (*o == '-') || (*o == '.')) get_texparm(0x11) ;
    	else save_parm("", 0) ;
	break ;
      default:
	get_texparm(2) ;	/* v8.23: get a whole word */
    }

    /* Restore the chain of buffers */
    if (bp->prev != inbuf) bp->prev = inbuf ;
    inbuf = saved_inbuf ;

    return("") ;
}

static char *get1arg ()		/* Get one argument ON THE SAME LINE, no {} */
{
  BUF *b ;
  char *o ;
    if (!inbuf->prev) return("") ;
    b = inbuf ;		/* Saved */
    inbuf = inbuf->prev ;
    o = inbuf->text + inbuf->pos ;
    if (isalnum(*o) || (*o == '-') || (*o == '.')) get_texparm(0x11) ;
    else save_parm("", 0) ;

    /* Re-append the continuation */
    if ( b->used > b->pos )
        push_text(b->text + b->pos, b->used - b->pos) ;
    if (b->flags == 0) {  b->size = 0; free(b); }

    return("") ;
}
static char *rescan_arg()	/* expand1arg */
/* Next input = argument, expand it in place of preceding
   If no next argument, take the LATEST
*/
{
  BUF *b, *prev ;
  int i, len, flags, old_mode;
  char *p ;
    if (next_char() == '\21') {	/* v8.23 */
        get_char();
	i = get_char()&0xff;
	if (i == '\6') i = itexparm-1;	/* NOOP */
        else i -= '1' ;
        if (i < 0) return("")  ;			/* NO ARGUMENT ! */
    }
    else i = itexparm-1;
    if (i<0) return("");
    if (!texparm[i]) return("") ;		/* no argument ? */
    /* May be a stacked arugment */
    while ((texparm[i]->used == 2) && (texparm[i]->text[0] == '\21'))
	i = texparm[i]->text[1] - '1';
    /* Is it necessary to rescan ? */
    for (p = texparm[i]->text; *p; p++) {
	if (iscntrl(*p)) break ;
	if (*p == '\\') break ;
	if (*p == '$') break ;
	if (*p == '^') break ;
    }
    if (! *p) return ("") ;			/* No need rescan */
    b = texparm[i] ;
    flags = b->flags ;
    prev  = b->prev ;
    p     = b->text ;
    /* Rescan the argument outside any special (tabular etc) context */
    old_mode = theMode; theMode = 0;		/* V8.27 */
    p = rescan(p, strlen(p)) ;
    theMode = old_mode;				/* V8.27 */
    len = strlen(p) ;
    free(b) ;
    texparm[i] = b = buf_alloc(len+1) ;
    buf_append(texparm[i], p, len) ;
    b->flags = flags ;
    b->prev  = prev ;
    free(p) ;
    return("") ;
}

#if 0
static char *href(href) char *href;	/* Edit HREF="..." 	*/
/*++++++++++++++++
.PURPOSE  Writes the HREF="..." argument
.RETURNS  ""
.REMARKS  Install the various mode flags:
-----------------*/
{
    return(href);
}
#endif

static char *edit_counter(s) char *s;	/* Edit a Counter	*/
/*++++++++++++++++
.PURPOSE  Edit a counter.
.RETURNS
.REMARKS  Argument stars by a '*' for s4ection0like (1.1 1.2 1.1.2 etc...)
-----------------*/
{
  char *p, *b, star, c, editype;
  int countno, i, countmin, val, base, o ;
  static char buf[80] ;
    if (*s == '*')  	/* Special: chapter, section, subsection... */
    	star = *(s++) ;
    else star = 0 ;
    p = s+1 ;
    if (isalpha(*p)) p = lookup(cnt_dic, p, -1) ;
    if (!p) return(s) ;			/* Unknown counter...	*/
    i = countno = atoi(p) ;		/* Counter number	*/
    if (i >= used_counters) return(s) ;	/* Bad number,..	*/
    editype = *s ;
    if (editype == '+') editype = counter_mode[i] ;
    if (star) {
	countmin = COUNT_chapter;
	if (counter[COUNT_chapter] == 0) countmin++ ;
    }
    else countmin = countno ;

    b = buf + sizeof(buf) ;
    *--b = 0 ;
    while (countno >= countmin) {
        val = counter[countno--] ; o = 0 ;
        switch(editype) {
          case 'a': base = 27 ; o = 'a' - 1; goto case_generic;
          case 'A': base = 27 ; o = 'A' - 1; goto case_generic;
          case 'r':
	    p = i2rom(val) ; i = strlen(p);
	    while (i > 0) { c = p[--i]; *--b = tolower(c); }
	    break ;
          case 'R':
	    p = i2rom(val) ; i = strlen(p);
	    while (i > 0) *--b = p[--i];
          default:
	    base = 10; o = '0' ;
	  case_generic:
	    while (val > 0) {
	        *--b = (val%base) + o ;
	        val /= base ;
	    }
            if (! *b) *--b = '0' ;
	    break ;
        }
	if (countno >= countmin) *--b = '.' ;
	editype = counter_mode[countno];
    }
    /* Remove the leading dots... */
    while (*b == '.') b++;
    return(b) ;
}
static char *new_counter(s) char *s;	/* New Counter	*/
{
  char buf[200], x ;
    /* Does the counter alreay exist ? */
    if (lookup(cnt_dic, s, -1)) return("") ;
    if (used_counters >= ITEMS(counter))
	return("****Too many counters defined****") ;
    if ((int)strlen(s) > 32) x=s[32], s[32]=0;
    else x = 0 ;
    sprintf(buf, "\\the%s\t\\edicnt{+%d}\n+%s\t%d\n", s,
	used_counters, s, used_counters) ;
    used_counters += 1 ;
    load_services(buf, T2_BUFDUP);
    if (x) s[32]=x ;
    return("") ;
}
static char *mod_counter(s, o) char *s, *o ;
/*++++++++++++++++
.PURPOSE  Change a counter
.RETURNS
.REMARKS  The '*' means: reset all down levels to zero. e.g.
	\setcounter{*chapter}{3} ==> set to zero \thesection, etc...
	\setcounter{*chapter}{3:a} ==> use an alphabetical counter
-----------------*/
{
  char *p; int i, value, flags;
    p = s ; flags = 0 ;
    if (*p == '*') flags |= 0x10, p++ ;
    if (isalpha(*p)) p = lookup(cnt_dic, p, -1) ;
    if (!p) return(s) ;			/* Unknown counter...	*/
    i = atoi(p) ;			/* Counter number	*/
    if (i >= used_counters) return(s) ;	/* Bad number,..	*/
    while (isspace(*o)) o++ ;
    while (ispunct(*o)) {
        if (*o == '*') flags |= 0x10, o++ ;
	else if (*o == '=') flags |= 0x01, o++ ;
	else break ;
    }
    value = atoi(o) ;
    while ((*o == '+') || (*o == '-') || isspace(*o)) o++ ;
    while(isdigit(*o)) o++ ;
    if (isalpha(*o)) counter_mode[i] = *o ;
    if (flags&0x01) counter[i] = value ;
    else counter[i] += value ;
    if (flags&0x10) while (i < COUNT_deepest) counter[++i] = 0 ;
    return("") ;
}

static char *save_buf(char *name, char *text)
/*++++++++++++++++
.PURPOSE  Save in the text as a named buffer
.RETURNS  ""
.REMARKS  The equivalence text is tagged to avoid a further translation !
-----------------*/
{
  char *p, *s ;
    p = s = malloc(strlen(name)+strlen(text)+ 4 /*''*/ + 6 /* { .. } */) ;
    *(p++) = '\'' ;
    strcpy(p, name) ; p += strlen(p) ;
    *(p++) = '\'' ;
    strcpy(p, "\33{\7\02"), p += strlen(p) ;
    strcpy(p, text) ; p += strlen(p) ;
    strcpy(p, "\33}")  ;
    load_serv1(s, 0) ;
    return ((char *)0);
}

static char *put_env(char *text)
/*++++++++++++++++
.PURPOSE  Interface to putenv
.RETURNS  ""
.REMARKS  For \putenv macro
-----------------*/
{
    while (isspace(*text)) text++ ;
    putenv(strdup(text));
    return ("") ;
}

static char *row_operate(char *text)
/*++++++++++++++++
.PURPOSE  Open/Close a Row
.RETURNS  ""
.REMARKS  Type of operation defined by the next byte o/1 = Open c/0 = Close
-----------------*/
{
  int c ;
    c = get_char() ;
    if (isdigit(c)) {
	if (c == '0') table_cclose() ;
	else table_ropen() ;
    }
    else if (toupper(c) == 'C') table_rclose() ;
    else table_ropen() ;
    return ("") ;
}

static char *get_buf(char *name)
/*++++++++++++++++
.PURPOSE  Retrieve the text from a named buffer
.RETURNS  Equivalence
.REMARKS  For \getbuF macro
-----------------*/
{
  char *p ;
    p = lookup(save_dic, name, strlen(name)) ;
    return (p ? p : "") ;
}

static char *setbuf1mode ()		/* Define OutputRoutine	*/
/*++++++++++++++++
.PURPOSE  Change the Buffer Mode
.RETURNS  ""
.REMARKS  The type of buffer is defined as the next byte
   - = Revert to previous
   0 = Standard Output Routine
   L = Line-by-line Output Mode
   + = to a new buffer, name follows
-----------------*/
{
  char buf[80] ;
  char *p ;
  int c ;
    c = get_char() ;
    switch(c) {
      case 'L':
      case '1':		/* Set Buffering Mode by Line */
	counter[COUNT_tabline] = 0 ;	/* Line Counter */
	sprintf(buf, "'#%d'%ld %ld %ld", counter[COUNT_buffer] ,
	    (long)OutFct, (long)saved_OutFct, (long)collector) ;
	counter[COUNT_buffer] += 1 ;
	load_serv1(buf, T2_BUFDUP) ;
	saved_OutFct = OutFct ;
	OutFct =  buf_out ;
	collector = (BUF *)0 ;
	return("") ;
      case '-':
	counter[COUNT_buffer] -= 1 ;
	sprintf(buf, "'#%d", counter[COUNT_buffer]) ;	/* Current routines */
	if (collector) free(collector) ;
	collector = (BUF *)0 ;
	OutFct = saved_OutFct = (INT_FCT)0 ;
	if (p = getequiv(buf)) {
	    OutFct = (INT_FCT)atol(p);
	    while (isdigit(*p)) p++ ; while (isspace(*p)) p++ ;
	    saved_OutFct = (INT_FCT)atol(p);
	    while (isdigit(*p)) p++ ; while (isspace(*p)) p++ ;
	    collector = (BUF *)atol(p);
	}
	return("") ;
      case '+':
      case '0':		/* Remove the output routines */
      default:
	putstring("<!-- Can't deal with mode of buffering -->\n") ;
	unget_char(c) ;
	return("") ;
    }
}

static char *exec_ToC()
/*++++++++++++++++
.PURPOSE  Execute the Table of Contents
.RETURNS  ""
-----------------*/
{
static char new_defs[] = "\\cgidef{ -tex \n\
\\execToC #2{\033{\\typeToC{#1}\\Aref{ToC\\theToC}{#2}\\x0A\\typeToC{-}\033}}\n\
\\chapter #1{\\ifnum\\themacrostar=0{\\execToC{2}{#1}}\\else\\stepcounter{ToC}\
\\fi\\mS0}\n\
\\section #1{\\ifnum\\themacrostar=0{\\execToC{3}{#1}}\\else\\stepcounter{ToC}\
\\fi\\mS0}\n\
\\subsection #1{\\ifnum\\themacrostar=0{\\execToC{4}{#1}}\
\\else\\stepcounter{ToC}\\fi\\mS0}\n\
\\subsubsection #1{\\ifnum\\themacrostar=0{\\execToC{5}{#1}}\
\\else\\stepcounter{ToC}\\fi\\mS0}\n\
\\part #1{\\execToC{1}{#1}}\n\
\\appendix {\\execToC{0}{}\\setcounter{section}{*0A}\\typeToC{A}\\typeToC{-}}\n\
\\inputverbatim #1{}\n\
\\cgidef #1{}\n\
\\tableofcontents {}\n\
}\n\
\\typeToC{C}\\typeToC{-}\n" ;
  INT_FCT old_saved, old_out ;
  char save_counter[sizeof(counter)] ;
  char save_counter_mode[sizeof(counter_mode)] ;
  char *p, cond[2];
  BUF *b ;

    /* Can't do a tableofcontents if not a file ! */
    if ((!thefile) || (*thefile == '(' /*)*/)) {
    	putstring("\n****Can't execute \\tableofcontents on '") ;
    	putstring(thefile ? thefile : "(null)") ;
    	putstring("'\n") ;
    	return("") ;
    }

    /* In tableofcontents*{} , add also the section*
       Done by changing the condition
       \themacrostar=0  into \themacrostar<9
    */
    if (counter[COUNT_macrostar] == 0)
	 { cond[0] = '='; cond[1] = '0'; }
    else { cond[0] = '<'; cond[1] = '9'; }
    p = strstr(new_defs, "themacrostar");
    while(p) {
	while(islower(*p)) p++;
	if (ispunct(*p)) { p[0] = cond[0]; p[1] = cond[1]; }
	p = strstr(p, "themacrostar");
    }

    /* Save the current environment */
    old_saved = saved_OutFct ; saved_OutFct = OutFct ;
    old_out   = OutFct ;             OutFct = null_out ;
    memcpy(save_counter, counter, sizeof(counter)) ;
    memcpy(save_counter_mode, counter_mode, sizeof(counter_mode)) ;
    memset(counter, 0, sizeof(counter)) ;
    memset(counter_mode, 0, sizeof(counter_mode)) ;

    /* Prepare the text to scan for the edition, and execute it */
    b = buf_append((BUF *)0, new_defs, -1) ;
    b = buf_append(b, "\\input{", -1) ;
    /* b = buf_append(b, thefile, -1) ; Mod. V9.47: */
    b = buf_append(b, basename(thefile), -1) ;
    b = buf_append(b, "}", 1) ;

    /* The interpretation for the table of contents requires
       to close properly all the opened tags.
    */
    do_text(b->text, b->used, do_option&(~DO_NOCLOSE)) ;
    b->size = 0 ;
    free(b) ;

    /* Terminate the Table of Contents */
    type_ToC("0") ;
    /* Reset the values */
    memcpy(counter, save_counter, sizeof(counter)) ;
    memcpy(counter_mode, save_counter_mode, sizeof(counter_mode)) ;
    OutFct = old_out ;
    saved_OutFct = old_saved ;
    return("") ;
}

static char *exec_ListObj()
/*++++++++++++++++
.PURPOSE  Execute the \listofobjects
.RETURNS  The ending
.REMARKS  typeToC just toggles the output mode between edit / ignore
	The option -o2Hr in openSesame edits the result in HTML as
	<TD> position </TD><TD> comments </TD>, in red for errors
\\glutag{openSesame,r -o2Hr \"#1\"}\\tag{/TR}\\x0A\
\\tag{TH}Text\\tag{/TH}\\tag{TH}Name used\\tag{/TH}\
\\tag{TH}J2000 position\\tag{/TH}\\tag{TH}Comments\\tag{/TH}\
\\object #1{\033{\\typeToC{+}\\tag{TR}\\tag{TD}\\gludata{I #1}\\tag{/TD}\
\\tag{TD}#1\\tag{/TD}\
-----------------*/
{
static char new_defs[] = "\\cgidef{ -tex \n\
\\object #1{\033{\\typeToC{+}\
\\glutag{openSesame,r \"#1\" -o4Hr $SESAME_r}\\x0A\
\\typeToC{-}\033}}\n\
\\objectname #0{\033{\\typeToC{+}\
\\ifargop\6\022-o\6\022-a\\glutag{openSesame,r \"#1\" -o4Hr $SESAME_r|#2}\
\\else\6\022-a\\glutag{openSesame,r \"#1\" -o4Hr $SESAME_r}\
\\fi\\x0A\
\\typeToC{-}\033}}\n\
\\cgidef #1{}\n\
\\listofobjects {}\n\
}\n" ;
static char begin_list[] = "\\begin{TeX}\033{\\typeToC{+}\
\\section*{Summary of objects marked in the text}\n\
\\tag{TABLE CELLSPACING=0 CELLPADDING=4 BORDER=2}\
\\tag{TR}\
\\tag{TH}object\\tag{/TH}\\tag{TH}OType\\tag{/TH}\
\\tag{TH}J2000 position\\tag{/TH}\\tag{TH}Nrefs\\tag{/TH}\\tag{TH}Resolver\\tag{/TH}\
\\tag{/TR}\
\\typeToC{-}\033}\n" ;
static char end_list[] = "\033{\\typeToC{+}\
\\tag{/TABLE}\\par\n\\typeToC{-}\033}\\end{TeX}\n";
  char save_counter_mode[sizeof(counter_mode)] ;
  char save_counter[sizeof(counter)] ;
  INT_FCT old_saved, old_out ;
  int old_count, old_math;
  BUF *b ;

    /* Can't do a listofobjects if not a file ! */
    if ((!thefile) || (*thefile == '(' /*)*/)) {
    	putstring("\n****Can't execute \\listofobjects on '") ;
    	putstring(thefile ? thefile : "(null)") ;
    	putstring("'\n") ;
    	return("") ;
    }

    /* Save the current environment */
    old_saved = saved_OutFct ; saved_OutFct = OutFct ;
    old_out   = OutFct ;             OutFct = null_out ;
    old_count = obj_counter;
    old_math = options.nomath;	options.nomath = 1;	/* No <I> */
    memcpy(save_counter, counter, sizeof(counter)) ;
    memcpy(save_counter_mode, counter_mode, sizeof(counter_mode)) ;
    memset(counter, 0, sizeof(counter)) ;
    memset(counter_mode, 0, sizeof(counter_mode)) ;

    /* Prepare the text to scan for the edition, and execute it */
    b = (BUF *)0;
    b = buf_append_env(b, new_defs);
    b = buf_append(b, begin_list, -1);
    b = buf_append(b, "\\input{", -1) ;
    b = buf_append(b, thefile, -1);
    b = buf_append(b, "}", 1);
    b = buf_append(b, end_list, -1);

    /* Execute the prepared text */
    do_text(b->text, b->used, do_option&(~DO_NOCLOSE)) ;
    b->size = 0 ;
    free(b) ;

    /* Reset the values */
    memcpy(counter, save_counter, sizeof(counter)) ;
    memcpy(counter_mode, save_counter_mode, sizeof(counter_mode)) ;
    OutFct = old_out ;
    saved_OutFct = old_saved ;
    obj_counter = old_count ;
    options.nomath = old_math;
    return("") ;
}

static PTR_FCT fct_stack[MAX_FCT] = {
    /* 000 */	null_fct,
    /* 001 */	tex_def, 		/* \def 	*/
    /* 002 */	table_multicol, 	/* \multicolumn	*/
    /* 003 */	table_hline ,		/* \hline	*/
    /* 004 */	print_services, 	/* \list	*/
    /* 005 */ 	exec_cgi, 		/* \cgidef	*/
    /* 006 */	exec_input, 		/* \input	*/
    /* 007 */	set1buf ,		/* \set1buf	*/
    /* 010 */	dirname ,		/* \dirname	*/
    /* 011 */	ed1buf ,		/* \ed1buf	*/
    /* 012 */	null_fct,		/* newline!!!   */
    /* 013 */	get_env ,		/* \env		*/
    /* 014 */	thePlain,		/* \plainbrowser*/
    /* 015 */	set_emptycell ,		/* \emptycell	*/
    /* 016 */	clear_emptycell ,	/* \noemptycell	*/
    /* 017 */	setopt ,		/* \SEToptions	*/
    /* 020 */	basename ,		/* \basename	*/
    /* 021 */	set1mode ,		/* \SET1mode	*/
    /* 022 */	get1arg1 ,		/* \GET1arg  	*/
    /* 023 */	typein ,		/* \typein	*/
    /* 024 */	mode4tex ,		/* \SETmode4TeX */
    /* 025 */	ifnum ,			/* Basic \ifxxx	*/
    /* 026 */	ifmode ,		/* Basic \ifxxx	*/
    /* 027 */	ifmatch ,		/* Agrees Patt.	*/
    /* 030 */	ifregexp ,		/* RegExpr	*/
    /* 031 */	debug ,			/* Display Deb.	*/
    /* 032 */	tex_defenv ,		/* Env. Defin.	*/
    /* 033 */	getargquoted ,		/* for \verb	*/
    /* 034 */	length ,		/* e.g. 0.1in	*/
    /* 035 */	ifgen ,			/* e.g. \if.{ }	*/
    /* 036 */	xcolor ,		/* Set xcol	*/
    /* 037 */	exec_command ,		/* Command exec	*/
    /* 040 */	get1arg ,		/* get1arg	*/
    /* 041 */	edit_counter ,		/* edicnt	*/
    /* 042 */	new_counter ,		/* newcounter	*/
    /* 043 */	mod_counter ,		/* counter op's	*/
    /* 044 */	type_ToC ,		/* Increase ToC	*/
    /* 045 */	exec_ToC ,		/* Write a  ToC	*/
    /* 046 */	save_buf ,		/* Save text    */
    /* 047 */	get_buf ,		/* Get  text    */
    /* 050 */	regexp ,		/* substitute   */
    /* 051 */	put_env ,		/* \putenv      */
    /* 052 */	setbuf1mode ,		/* Change bufmod*/
    /* 053 */	buf_out1 ,		/* \\           */
    /* 054 */	buf_out0 ,		/* \kill        */
    /* 055 */	rescan_arg ,		/* \expand1arg  */
    /* 056 */	row_operate ,		/* open/clos row*/
    /* 057 */	exec_ListObj ,		/* A&A Context  */
    /* 060 */	filedate  ,		/* \filedate	*/	/* v7.6 */
    /* 061 */	ifargop  ,		/* \ifargop	*/	/* v7.9 */
    /* 062 */	reset_alarm  ,		/* \resetAlarm	*/	/* v7.99*/
    /* 063 */	echo1x  ,		/* \echo1x	*/	/* v8.32*/
    /* 064 */	echo4   ,		/* \echo4	*/	/* v8.5 */
    /* 065 */	atom   ,		/* \ionZ	*/	/* v8.8 */
    /* 066 */	edit_unicode  ,		/* \unicode	*/	/* v9.0 */
    /* 067 */	null_fct  ,		/* \undefined	*/	/* v9.06*/
    /* 070 */	winame  ,		/* \Apop	*/	/* v9.3 */
    /* 071 */	winopt  ,		/* \Apop	*/	/* v9.3 */
    /* 072 */	glupop  ,		/* \glupop	*/	/* v9.3 */
    /* 073 */	edit_graph  ,		/* \graph{n}	*/	/* v9.4 */
};
static int ifct_stack = 1;
static int nfct_stack = 1;

static char *tex_def()
/*++++++++++++++++
.PURPOSE  Add a new definition (\def macro)
.RETURNS  ---
-----------------*/
{
  char *m, *def, *buf, *p; int np , len, itexparm0 ;
  char *tmp = (char *)0;
#if 0
  static char must_not_change[] = "\
\\ifhtx\n\
\\Beg\n\
\\End\n\
" ;
#endif
    itexparm0 = itexparm ;
    m  = get_texparm(0) ;		/* Name of Macro */
    np = -1 ;
    def = get_texparm(0x100);		/* Accept numbers in macro... */
    if (texparm1 == '[') np = atoi(def) ;
    else if (*def == '#') {
	while(*def == '#')
	np = *++def - '0', def += strloc(def, '#') ;
    }
    if (np > 9) np = 9;
    if (np >= 0) def = get_texparm(0) ;

    /* The \long has no more signification (v8.38) */
    counter[COUNT_long] = 0;

    /* Definition ignored within iffalse */
    /* if (ignore_output) return ((char *)0); */

    /* Verify that the definition cannot be changed !!! */
#if 1	/* Strict method: don't modify called macros */
    /* Such "basic" definitions have a \6 (function call)	*/
    if (p = lookup(texdef_dic, m+1, -1)) {
	p = strchr(p, '\6') ;
    }
#else
    len = strlen(m) ; m[len++] = '\n' ;
    p = must_not_change ;
    while(p) {
	if (strncmp(p, m, len) == 0) break ;
	if (p = strchr(p, '\n')) p++ ;
    }
    m[--len] = 0 ;
#endif
    if (p) {	/* Ignore the Definition */
	if (theMode&MODE_ignore) ;
	else {
	    putstring("<!-- Macro can't be changed: ") ;
	    putstring(m) ;
	    putstring(" -->\n") ;
	}
    	itexparm = itexparm0 ;		/* Reset # parameters */
    	return ((char *)0);
    }

    /* The definition string should not contain \21x argument,
       except \imgoptions  \taboptions
    */
    if (p = strchr(def, '\21')) {
        if ((def[0] == '\21') || isdigit(p[1])) /* Miduf V8.67 */
	  def = tmp = rescan21(def, -1);
    }
    /* Allocate a buffer to hold the definition, and insert as a dic */
    len = strlen(m) + strlen(def) + 8;
    buf = p = malloc(len);
    strcpy(p, m); p += strlen(p);
    *(p++) = ' ';
    if (np >= 0) *(p++) = '#', *(p++) = np + '0';
    if (np >= 0) *(p++) = '{';
    strcpy(p, def); p += strlen(p);
    if (np >= 0) *(p++) = '}' ;
    *p = 0 ;
    load_serv1(buf, T2_BUFREE) ;
    if (tmp) free(tmp);
    itexparm = itexparm0 ;		/* Reset # parameters */
    return ((char *)0);
}

static char *tex_defenv()
/*++++++++++++++++
.PURPOSE  Add a new Enviromenmt
.RETURNS  ---
.REMARKS  Form \newenviromnent{name}[nargs]{BEGIN}{END}
-----------------*/
{
  char *n, *begenv, *endenv, *buf, *p ; int np, len, itexparm0 ;
    itexparm0 = itexparm ;
    n  = get_texparm(0) ;		/* Name of Env. */
    np = 0 ;
    begenv = get_texparm(0);
    if (texparm1 == '[') {		/* #Parameters	*/
	np = atoi(begenv);
	begenv = get_texparm(0) ;
    }
    endenv = get_texparm(0) ;
    if (np < 0) np = 0;
    if (np > 9) np = 9;

    /* Definition ignored within iffalse */
    /* if (ignore_output) return ((char *)0); */

    /* Allocate a buffer to hold the definition, and insert as a dic */
    len = strlen(n) + strlen(begenv) + strlen(endenv) + 16;
    buf = p = malloc(len);
    *(p++) = '{' ; strcpy(p, n); p += strlen(p); *(p++) = '}' ;
    *(p++) = ' '; *(p++) = '#', *(p++) = np + '0';
    *(p++) = '{' ; strcpy(p, begenv) ; p += strlen(p); *(p++) = '}' ;
    *(p++) = '{' ; strcpy(p, endenv) ; p += strlen(p); *(p++) = '}' ;
    *p = 0 ;
    load_serv1(buf, T2_BUFREE) ;
    itexparm = itexparm0 ;		/* Reset # parameters */
    return ((char *)0);
}

int tex_link(macro, fct)
/*++++++++++++++++
.PURPOSE  Link a user-supplied function to a Macro
.RETURNS  A function number / -1
.REMARKS  macro must be "\macro #n"
-----------------*/
  char *macro;	/* IN: The name of the Macro 	*/
  PTR_FCT fct;	/* IN: The user Function	*/
{
  char *buf, *p; int np , i, len ;

    if (ifct_stack >= (MAX_FCT-1)) {
	putstring("\n<!-- **Can't add function '"); putstring(macro);
	putstring("', array full**** -->\n") ;
    	return(-1);
    }
    if (*macro == '\\') macro++;
    i = strloc(macro, '#') ;
    if (macro[i]) np = macro[i+1] - '0' ;
    else np = 0;
    if (np < 0) np = 0; if (np > 9) np = 9 ;

    /* Allocate a buffer to hold the definition, and insert as a dic */
    len = strlen(macro) + 2*np + 8;
    buf = p = malloc(len);
    *(p++) = '\\';
    strncpy(p, macro, i); p += i;
    *(p++) = ' ';
    if (np > 0) *(p++) = '#', *(p++) = np + '0';
    *(p++) = '\6';
    *(p++) = ifct_stack ; fct_stack[ifct_stack++] = fct;
    for (i=1; i<=np; i++) *(p++) = '#', *(p++) = '0' + i ;
    *(p++) = '\n';
    *p = 0 ;
    load_services(buf, 0) ;
    return (ifct_stack-1);
}

/*==================================================================*
		Management of TeX Stacks
 *==================================================================*/

static void depile_env(env)
/*++++++++++++++++
.PURPOSE  Depile the TeX environments
.RETURNS  ---
-----------------*/
  char *env;	/* IN: Environment starting by '{' }	*/
{
  char *p; int len ;
    for (p=env; *p; p += len) {
	len = 1 ;
	if (*p == /*{*/ '}') { inbuf = buf_append(inbuf, p, 1); continue ; }
	if (*p != '{') /* } */ continue;
	push_text("\\end{", 5) ;	/* } */
	++p ; len = strbrace(p) ;
	inbuf = buf_append(inbuf, p, len) ;
    }
}

static void empile_env(env, arg)
/*++++++++++++++++
.PURPOSE  Empile the TeX environments
.RETURNS  ---
-----------------*/
  char *env;	  /* IN: Environment starting by '{' 	*/
  char *arg;	  /* IN: The argument to add before '}' */
{
  char *p; int len ;
    push_text("", 0) ;	/* Start a new input buffer 	*/
    if (begin_more) {
	fprintf(_of, "\n++++Ignored begin_more '%s'\n", begin_more) ;
	free(begin_more) ;
	begin_more = (char *)0 ;
    }
    for (p=env; *p; p += len) {
	len = 1 ;
	if (*p == /*{*/ '}') {
	    if (arg) inbuf = buf_append(inbuf, arg, -1) ;
	    arg = (char *)0 ;
	    inbuf = buf_append(inbuf, p, 1);
	    continue ;
	}
	if (*p != '{') /* } */ continue;
	inbuf = buf_append(inbuf, "\\begin{", 7) ;	/* } */
	++p ; len = strbrace(p) ;
	inbuf = buf_append(inbuf, p, len) ;
    }
    /* set_texparm(inbuf->text, itexparm0, itexenv0); */
}

static void push_tex(env, arg)
/*++++++++++++++++
.PURPOSE  Starting of a new tex environment
.RETURNS  ---
.REMARKS  The arguments of an environment are grabbed here.
		There are arguments if env starts by \20
-----------------*/
  char *env;	/* IN: Environment Tag 	 */
  char *arg;	/* IN: Argument before > */
{
  int i, add_hiddensoft; char *p, c=0; /*V9.12: add_hiddensoft*/
#if DEBUG>0
    fprintf(_of, "....push_tex('%s','%s',%d), pending=%d\n", env, arg,used_env,pending);
#endif

    if(verbop > 1) { int n; n = (texenv - tex_stack) ;
	fprintf(_of,
	    "\n<!-- ....push_tex(%s,%s) [nextinput=%s]; stack= -->\n<!-- ",
	    env?env:"(nil)", arg?arg:"(nil)",inbuf->text + inbuf->pos);
	for (i=0; i<=n; i++) fprintf(_of, "\t(%s)",
	    tex_stack[i].env ? tex_stack[i].env : "(nil)");
	fprintf(_of, " -->\n");
    }
    if (!env) env = "";
    if (texenv >= etexenv) {
	putstring("\n<!-- **Can't push tex '"); putstring(env);
	putstring("' to depth "); putstring(i2a(MAX_TEX));
	putstring("  -->\n") ;
	terminate("****TeX to HTML loop ? ");
    }

    /* Caution: Table started, but no row yet issued....	*/
    if ((theMode & (MODE_tab|MODE_row)) == MODE_tab)	pending = 1 ;
    if (pending) PutChar(EOF) ;	/* Force the starting rows, etc */
    texenv->iparm = itexparm ;
    texenv++ ;
    memcpy(texenv, texenv-1, sizeof(TEX_ENV));
    texenv->env = env ;
    texenv->ibrace = 0 ;
    texenv->itag = itag_stack;
    texenv->ipenv = itexenv;	/* v8.14 -- will be reset in pop_tex */
    texenv->defs = (TABLE2 **)0 ;
    /* Nothing output yet in this definition */
    texenv->status &= ~(MODE_out1c|MODE_out1d) ;

    /* Set up Special Modes -- a new environment starts only with \07 */
#if 1
    add_hiddensoft = theMode&MODE_soft ? MODE_hiddensoft : 0;
    for (i=0; iscntrl(*env); env += 2) {
	if (*env == '\7') i++, theMode = env[1] & 0xff ;
	else if (*env == '\17') setimode(env[1] & 0xff) ;
	else if (*env == '\37') {	/* Must hide the table... */
	    if (i == 0) theMode &= ~(MODE_tab|MODE_VERB|MODE_intag);
	    theMode |= (env[1] & 0xff) ;
	}
	else break ;
	if (add_hiddensoft && ((theMode&MODE_soft)==0))  /*V9.12*/
	    theMode |= MODE_hiddensoft;
    }
    if (i) env = tex_open(env) ;
#else
    if (*env == '\7') {		/* Set Special Mode */
	theMode = env[1] & 0xff ;
	env = tex_open(env+2) ;
    }
    else if (*env == '\37') {	/* OR Special Mode  */
	theMode = (theMode&(~0xff)) | (env[1] & 0xff) ;
	env = tex_open(env+2) ;
    }
#endif

    /* ==== Write out the Environment + Argument ==== */
    if (isascii(*env) && isprint(*env)) {
	if (*env != '<') putchar('<');
	for (p=env; isprint(*p); p++) {
	    c = *p; if (c == '{') /*}*/ break ;
	    if (c == '>') { c = 0 ; break ; }
	    putchar(c);
	}
	if (begin_more) {
	    putchar(' ') ;
	    putstring(begin_more) ;
	    free(begin_more) ;
	    begin_more = (char *)0 ;
	}
	if (arg) { char *aa ;
	    if (strchr(arg, '\021') || strchr(arg, '\020'))
		 p = aa = rescan(arg, strlen(arg)) ;
	    else p = arg, aa = (char *)0 ;
	    while (*p /* isprint(*p) --- V8.6*/ ) {
		c = *(p++);
		if (isprint(c) || isspace(c)) putchar(c);
		else fprintf(stderr,
		    "++++Invalid environment char \\%03o(#%d) ignored", c, c);
	    }
	    if (aa) free(aa) ;
	}
	if (c != '>') putchar('>');
	Flush ;
    }
    if (*env)  {
#if DEBUG>1
    fprintf(_of, "====push_tex(%s)\n", env);
#endif
    	push_tag(env);
    	texenv->env = env;
    }
}

static int begin_env(opt)
/*++++++++++++++++
.PURPOSE  Execute the \begin{...}
.RETURNS  0 (doesn't match) / 1
.REMARKS  Input text starts by { ... } (argument of \begin)
	opt is:
	  + if Beg (HTML options)
	  = if \opentag (no special action)
     arg contains argument e.g. the "bgcolor=..." in BODY bgcolor=...
-----------------*/
  int opt ;	/* IN: '+' for an extra HTML parameter with option (\Beg) */
{
  char env_buf[256], *env_name; BUF *b ;
  char *tofree = (char *)0;
  char *p, *e, *arg, *env ;
  int itexenv0, i;
  int accept_op=1;	/* Ignore the optional argument within [...] */
  TEX_ENV *otexenv ;

    /* begin_more = (char *)0 ;		// Beg context	*/
    p = inbuf->text + inbuf->pos ;
    while (isspace(*p)) p++ ;
    if (*p != '{' /*}*/) return(0) ;

    /* Start the new column if necessary */
    if ((theMode & (MODE_tab|MODE_row)) == MODE_tab)	pending = 1 ;

    e = ++p ; i = strbrace(p) ; p += i ;
    if (*p && (i<sizeof(env_buf))) {
	env_name = env_buf;
	strncpy(env_name, e, i);
	env_name[i]=0;
    }
    else {				/* V8.6: Read until closing brace! */
      int bytes, len;			/* Read input until found... */
	len = i;
	bytes = (len|1023)+1;
	tofree = malloc(bytes);
	strncpy(tofree, e, len);
	tofree[len] = 0;
	while (!*p) {
	    inbuf->pos = (p - inbuf->text);	/* Input Used	*/
	    if (next_char() == EOF) {
		push_text("\n[****UnclosedBrace??****]", -1);
		break;
	    }
	    p = inbuf->text + inbuf->pos;
	    i = strbrace(p);
	    if ((i+len) >= bytes) {
		bytes = ((i+len)|1023) +1;
		tofree = realloc(tofree, bytes);
	    }
	    strncpy(tofree+len, p, i);
	    len += i; p += i;
	}
	tofree[len] = 0;
	env_name = tofree;
    }
    if (*p) p++ ;
    inbuf->pos = (p - inbuf->text) ;	/* Input Used	*/

    /* Look for the first word only ! */
    for (arg=env_name; isgraph(*arg); arg++) ;
    if (opt == '=') env = save_env(env_name, arg-env_name);
    else env = lookup(texenv_dic, env_name, arg-env_name) ;

    /* Before entering a new environment, in the context of a table,
       be sure thtat the column is started.
       However, the {row} environment does NOT start a column !
    */
    if (env) {
	if (strncmp(env, "\21\6", 2) == 0) accept_op=0, env+=2;
	else if ((env[0] == '\20') && (strncmp(env+3, "\21\6", 2)==0)) {
	    accept_op = 0;
	    if (env[4] == '\6') pending = 0 ;	/* Special {row} */
	}
    }
    if (pending) PutChar(EOF) ;

    /* arg = (char *)0 ; */
    itexenv0 = itexenv ;	/* Pop after Created Environment */
    if (!env) {			/* Environment does not exist. Create it */
	env = save_env(env_name, arg-env_name) ;
    }
    if (!*arg) arg = (char *)0 ;

    /* Get the HTML Option, if opt = '+' */
    if (opt == '+') {
	get_texparm(0) ;
	b = texparm[itexparm-1];	/* V8.66: rescan may re-use texparm */
	p = rescan(b->text, b->used) ;
	--itexparm;		/* V8.66: texparm[itexparm-1] now available */
	if (begin_more) free(begin_more) ;
	begin_more = p; 	/* rescan returns allocated */
        p = inbuf->text + inbuf->pos ;
    }

    /* Ignore optional argument e.g. \begin{figure}[hp]
       -- BUT no blank accepted, e.g. \begin{figure} [hp] is NOT
	  optional argument !!!
    */
    if (accept_op && (next_char() == '[')) {
        p = inbuf->text + inbuf->pos;
	i = strloc(p, ']') ;
	if (p[i]) p += i+1 ;
	inbuf->pos = (p - inbuf->text);	/* Input Used   */
    }

#if 1	/* V8.55 The "simple substitution" should allow to keep
		 arguments -- but causes problems !
	*/
    /* ==== Recursive Environments (pb with argument!) -- */
    if (*env == '{' ) {		/* Recursive environments */
	i = strloc(env, '}') +1;
	if (i == strlen(env)) {	/* A simple substitution  */
	    e = lookup(texenv_dic, env+1, i-2) ;
	    if (e) {
		env = e ;
	    }
	}
    }
#endif
    if (*env == '{' /*}*/) {	/* Recursive environments */
	if (arg) fprintf(_of, "\n****Argument ignored in \\begin{%s}!\n",
	    env_name);
    	texenv->ipenv = itexenv0 ;
        empile_env(env, arg) ;
	if (tofree) free(tofree);
        return(1) ;
    }

    /* ==== With Argument: can't use Clauses \begin \end  */
    if (arg) {
	if (*env == '\20') fprintf(_of,
	    "\n****Argument ignored in \\begin{%s}!\n    %s\n",
	    env_name, "(Can't have arguments in macros with arguments)") ;
    	i = p[-1] ; p[-1] = 0 ;
    	push_tex(env, arg) ;
	texenv->ipenv = itexenv0 ;
    	p[-1] = i ;
	if (tofree) free(tofree);
    	return(1) ;
    }

    /*==== Environments with parameters are defined:
     {env}	#n{ (begin instructions) }{ (end instructions) }
    */
    if (*env == '\20') {		/* There are parameters */
        /* Save the Current Definitions */
	texenv->defs = save_defs() ;
    	env++; i = *env - '0'; env++;	/* Number of arguments	*/
    	/* Get each environment parameter into texparm, but then
    	   pop from texparm and push it to ptexenv stack
        */
    	while (--i >= 0) {
    	    get_texparm(/*V9.12* 0*/0x80);  /* No context in softex */
    	    b = texparm[--itexparm] ;
	    if (b && strchr(b->text, '\021')) {  /* Mod. V8.85 */
	      char *s;
	        s = rescan21(b->text, -1);
		save_env(s, -1);
		free(s);
	    }
    	    else save_env(b->text, b->used) ;
	}
	if (*env == '{' /*}*/) {	/* User-Defined Environment  */
	    env++ ; i = strbrace(env) ;
	    p = push_text(env, i) ;	/* BEGIN clause	*/
	    set_texparm(p, 0, itexenv0);
	    env += i ;
	    if (*env) env++ ;
	    while (isspace(*env)) env++ ;
	    /* Save the "\end{...}" in Dic, as "{.../" } */
	    save_env("{", 1) ; /*}*/
	    /* !!! There is a trick here, b can change...
	       Therefore don;t forget to RESET ptexenv[itexenv-1]
	       after the various append's....
	    */
	    b = ptexenv[itexenv-1] ;
	    b = buf_append(b, env_name, strlen(env_name)) ;
	    b = buf_append(b, "/ {", 3) ;
	    if (*env == '{' /*}*/) ++env,
	        b = buf_append(b, env, strbrace(env)) ;
	    b = buf_append(b, "}", 2) ;
	    ptexenv[itexenv-1] = b ;
	    /* --- End of Trick -- */	/* END clause */
	    set_texparm(b->text, 0, itexenv0);
	    load_serv1(b->text, T2_BUFDUP); /* load_services(b->text, 0) ; */
	    env = "" ;
	    /* return(1) ; */
	}
    }
    otexenv = texenv ; i = otexenv->ipenv ; otexenv->ipenv = itexenv0 ;
    push_tex(env, arg) ;
    otexenv->ipenv = i ;
    texenv->ipenv = itexenv0 ;
    if (tofree) free(tofree);
    return(1) ;
}

static int end_env(int opt)
/*++++++++++++++++
.PURPOSE  Execute the \end{...}
.RETURNS  1 (OK) / 0 (can't match)
.REMARKS  Input text starts by { ... } (argument of \end)
	opt '='  (macro closetag)
-----------------*/
{
  /* TABLE2 *t1, *t2 ; */
  char *p, *e, *arg, *env, buf[60];
  int i /* itexenv0, mode */ ;

    p = inbuf->text + inbuf->pos ;
    while (isspace(*p)) p++ ;
    if (*p != '{' /*}*/) return (0) ;

    e = ++p ; i = strbrace(p) ; p += i ;
    if (opt == '=') {
	strncpy(buf, e, sizeof(buf));
	if (i<sizeof(buf)) buf[i] = 0;
	else  buf[sizeof(buf)-1]  = 0;
	env = buf;
    }
    else env = lookup(texenv_dic, e, p-e) ;
    if (!env) 	/* Environment does not exist. Create it */
	env = save_env(e, p-e) ;

    /* ==== Test we're in VERBATIM ==== */
    if_mode(MODE_VERB) {
    	e = env ;
	if (strncmp(e, "\21\6", 2) == 0) e += 2;
    	if (*e == '\20') e += 2 ;
    	if (*e != '\7') return(0) ;
    	if ((e[1]&MODE_VERB) != MODE_VERB) return(0) ;
    }

    if (*p) p++ ;
    inbuf->pos = (p - inbuf->text) ;	/* Input Used	*/

    /* ==== Recursive Environments (can't have arguments) --- */
    if (*env == '{' /*}*/) { 	/* Recursive environments */
	/* V8.55 PROBLEM WITH "simple substitution" -- */
	i = strloc(env, '}') +1;
	arg = (char *)0;
	if (i == strlen(env)) 	/* A simple substitution  */
	    arg = lookup(texenv_dic, env+1, i-2) ;
	if (arg) env = arg;
	else {
            depile_env(env);
	    return(1);
	}
    }

    if (env[0] == '\20') {
    	/* ==== User-defined Environments ==== */
    	env += 2 ;
        if (*env == '{' /*}*/) {
    	    i = p[-1] ; p[-1] = '/' ;
    	    arg = lookup(texenv_dic, e, p-e) ;
    	    p[-1] = i ;
	    if (arg) {
    	        push_text("\33E", 2) ;	/* Will call pop_tex */
    	        push_text(arg, strlen(arg)) ;
		/* Close tags opened by this level ...	*/
    		while (itag_stack > (int)texenv->itag)  pop_tag();
    	        if (*arg == '{' /*}*/)
    	            inbuf->pos += 1, inbuf->used -= 1 ;
                /* Remove the definition of \end clause done in restore_defs */
		/*
                for (t1=(TABLE2 *)0, t2=texenv_dic; t2 && (t2->rows[1] != arg) ;
                     t1 = t2, t2 = t2->prev) ;
	        if (t1) t1->prev = t2->prev ;
	        else  texenv_dic = t2->prev ;
                t2kill(t2) ;
		*/
	    }
	    else {
		putstring("\n<!-- ++Environment was not opened {") ;
		putbuffer(e, p-e) ;
		putstring("} -->\n") ;
	    }
	    env = "" ;
            return(1) ;
        }
    }
    itexenv = texenv->ipenv ;
    pop_tex(env) ;
    return(1) ;
}

static void pop_tex(env)
/*++++++++++++++++
.PURPOSE  Get out of current environment
.RETURNS  ---
-----------------*/
  char *env;	/* IN: Environment Tag */
{
  /*static char edbuf[60]; */
  int i;
#if DEBUG>1
    fprintf(_of, "....pop_tex(%s)\n", env);
#endif

    if(verbop > 1) { int n ; n = texenv - tex_stack ;
	fprintf(_of,
	    "\n<!-- .....pop_tex(%s) [itex_stack=%d]; stack= -->\n<!-- ",
	    env?env:"(nil)", n);
	for (i=0; i<=n; i++) fprintf(_of, "\t(%s)",
	    tex_stack[i].env ? tex_stack[i].env : "(nil)");
	fprintf(_of, " -->\n");
    }
    if (!env) env = "";
    if (texenv  == textop) ;
    else if (texenv < textop /* tex_stack -- v8.23 */ ) { static int count=0;
	count++;
	if ((count<10) || ((count%100)==0)) {
	    putstring("\n<!-- **Can't pop tex '") ;
	    putstring(env) ;
	    putstring("' from relative depth 0 = ");
	    putstring(i2a(textop-tex_stack));
	    putstring(" x"); putstring(i2a(count));
	    putstring(" -->\n");
	}
    }
    else {
    	if (*env == '\020') env += 2;	/* There are parameters */
#if DEBUG>1
    fprintf(_of, "====pop_tex(%s)\n", env);
#endif
	/* Execute special parts */
	if (*env == '\7') {
	    env = tex_close(env) ;
	}

	/* Close tags opened by this level ...	*/
    	while (itag_stack > (int)texenv->itag)  pop_tag();
    	if (strdiff1(texenv->env, env)
    	   && isgraph(*texenv->env)
	   && (OutFct != null_out)
    	   && isgraph(*env)) { INT_FCT ofct ; ofct = OutFct;
	   putstring("\n<!-- ++{") ; putstring(texenv->env) ;
	   OutFct = com_out ;
	   putstring("} closed by {"); putstring(env) ;
	   OutFct = ofct ;
	   putstring("} -->\n") ;
	}
	itexenv = texenv->ipenv;	/* v8.11 */
	--texenv ;
	/* *** There can be a problem (e.g. with missing brace)
	 * when the input is in the parameters... Just verify !
	*/
	if ((inbuf->flags) && (inbuf->pos < inbuf->used)) {
	    i = 0 ;
	    while (itexparm > texenv->iparm) {
		if (inbuf == texparm[itexparm--]) i++ ;
	    }
	    if (i) push_text(
	    "\n\\verbatim{[***Badly placed \\end***]}", -1) ;
	}
	/* Normally, itexparm should not increase... */
	if (texenv->iparm < itexparm)
    	    itexparm = texenv->iparm ;
	/* Restore Current Definitions */
	if (texenv->defs) {
	    restore_defs(texenv->defs) ;
	    free(texenv->defs) ;
	    texenv->defs = (TABLE2 **)0 ;
	}
	/* In tabular definitions, propagate MODE_out1c */
	if (i = texenv[1].status&(MODE_out1d|MODE_out1c)) {
	    theStatus |= MODE_out1d ;
	    if (theMode&MODE_col) theStatus |= MODE_out1c /* i -- 7.83*/ ;
	}
    }
}

/*==================================================================*
			HTML conversion
 *==================================================================*/
static void rm_typeface()
/*++++++++++++++++
.PURPOSE  Remove saved typefaces
.RETURNS  ---
-----------------*/
{
    while ( (itag_stack > (int)texenv->itag) &&
    	    (*tag_stack[itag_stack-1] == '\2'))  pop_tag();
    return;
}

static void make_anchor(tag, arg, text)
/*++++++++++++++++
.PURPOSE  Create an Anchor from
		<A ${tag}arg>text</A>
.RETURNS  ---
.REMARKS  in tag, the '$' asks to insert the argument.
	If tag is NULL, the url is in arg;
	If tag starts by "#", it's a NAME anchor
	If tag starts by "&", just issue url, arg
	If tag starts by ",", it's an option
-----------------*/
  char *tag;	/* IN: Prefix, e.g. Bib or Cat, or %E..	*/
  char *arg;	/* IN: Text to insert as argument	*/
  char *text;	/* IN: The text related to anchor	*/
{
  char *t=0, *u, *eq, *opt;
  int i, np, ct, href, arg_eq_text;
  char url[LEN_TAG], atag[LEN_TEX];
  TABLE2 *dic; static char prefix[2] = {0,0};

    /* fprintf(_of, "\n....make_anchor(%s,%s,%s)\n",tag, arg, text); */
    /* If tag is empty, it means that the first word of arg is the URL,
       the other being the arg
    */
    prefix[0] = 0; dic = serv_dic ;
    if (!tag) tag = "";
    if (*tag == ',') {		/* V8.53: Special option -gluw */
	opt = tag;
	tag = "";
    }
    else opt = "";
    if (! *tag) {
	tag = arg;
	while (isgraph(*arg)) arg++;
	i = arg - tag; if (i >= sizeof(atag)) i = sizeof(atag) - 1;
	strncpy(atag, tag, i); atag[i] = 0;
	tag = atag;
	while (isspace(*arg)) arg++;
    }
    if (! text) text = "";

    /* Which dictionary (:%&) to use ? 	*/
    i = strloc(c1def, *tag);
    if (c1def[i]) prefix[0] = *(tag++), dic = t2def[i];
    if (prefix[0] == ':') prefix[0] = 0;

    /* Convert the tag into URL: First look in Environment,
       otherwise in services, or finally leave it
    */
    if (prefix[0] == '$') u = get_env(tag);
    else {
	u = lookup2(dic, tag, arg);
	if (!u) u = lookup(dic, tag, strlen(tag));
	if ((!u) && (!prefix[0])) {
	    strcpy(url, tag2html), strcat(url, tag);
	    u = get_env(url);
	}
    }
    if (!u) {
	PutChar('<'); PutString(prefix); PutString(tag);
	PutString(opt);		/* V8.53: -gluw */
	/* V8.75: if a space in glu tag, no more space */
	/* PutChar(oglu ? ' ' : ':');  */
	if (oglu) {
	    if (strchr(tag, ' ')) ;
	    else PutChar(' ');
	}
	else PutChar(':');
	PutString(arg);
	if (*text) {
	    PutChar(oglu ? '|' : ' '); 	/* --- */
	    PutString(text);
	}
	PutChar('>');
	return ;
    }
    eq = u;				/* Keep  Equivalence */

    if (*u == '#') {
	PutString("<A NAME=");
	PutString(u+1); PutString(arg);
	PutChar('>') ;
	return;
    }
    if (*u == '&') { 	/* e.g. &lt; */
	PutString(u); PutString(arg);
	return;
    }

    /* If text is not defined, take the second word of arg */
    np = 0;			/* Number of parameters written out */
    ct = 0;			/* Saved char */
    if (!*text) {
	if (prefix[0]) {	/* The '|' separates the text from argument */
	    text = arg + strloc(arg, '|');
	    if (*text) t = text++;
	}
	else {			/* Take from second word */
    	    for (text=arg; isgraph(*text); text++) ;
	    t = text ;
	    while(isspace(*text)) text++ ;
	}
	if (*text) ct = *t, *t = 0;
	else text = arg;
    }
    arg_eq_text = (text == arg);

    /* fprintf(_of, "....make_anchor(tag='%s', arg='%s') url=%s\n",
       tag, arg, u); */
    /* The equivalence is NOT an anchor if it is a non-env variable */
    if (*u == '$') {
	href = 0 ;
	if (isalpha(u[1]) || (u[1] == '{')) href = 1;	/* } */
    }
    else href = (*u != '<') ;	/* Indicates to generate an anchor */

    if (href) PutString("<A HREF=\"");
    while (isprint(*u)) {
    	if (*u == '$') {	/* Argument or Env Substitution	*/
	    u++;
	    if (isalpha(*u) || (*u == '{')) u += print_envariable(u); 	/* } */
	    else u +=  print_subfield(arg, u, href), np++;
    	}
    	else PutChar(*u), u++;
    }

    /* If no parameter defined, it's assumed to be already converted */
    /* if (np == 0) print_subfield(arg, "", href); */
    if (np == 0) print_subfield(arg, "", 0);

    /* Terminate anchor with text; when no specific text was provided,
       and the URL ends with the colon (example http: or ftp:),
       add it also in the displayed text
    */
    if (ct) *t = ct ;	/* Restore end-of-arg 	*/
    text = unescaped_string(text, 0);
    if (href) {		/* Terminate anchor	*/
	PutString("\">");
    	if ((u[-1] == ':') && (arg_eq_text)) PutString(eq);
	/* do_text(text, strlen(text), do_option|DO_NOCLOSE|DO_PLAIN);  */
	i = strloc(text, crypt_char);
	crypt_char = text[i];
	if (crypt_char) text[i] = 0;
	PutString(text);
	if (crypt_char) {
	    if (crypt_char == '@') PutString("(at)");
	    text[i++] = crypt_char;
	    PutString(text+i);
	}
	PutString("</A>") ;
    }
    else if (!arg_eq_text) PutChar(' '), PutString(text);
}

/*==================================================================
		Check the input against stored knowledge
 *==================================================================*/

static int starts_by(s)
/*++++++++++++++++
.PURPOSE  Check if the input starts by the specified word
.RETURNS  0 (false) / 1 (true)
-----------------*/
  char *s;	/* IN: Word to check */
{
  char *p;
    p = inbuf->text + inbuf->pos ;
    return(!(strncmp(p, s, strlen(s)))) ;
}

static int isanifmacro(o)
/*++++++++++++++++
.PURPOSE  Check if the input is an IF condition
.RETURNS  0 (false) / 1 (true)
-----------------*/
  char *o ;	/* IN: Macro to test */
{
  char *p, *s;
    if (*o == '\21') 	/* Arguments!	*/
	o += 2 ;
    if (*o == '{' /*}*/) o++ ;
    if (*o == '\\') {
	p = ++o ;
        if (*(p++) != 'i') return(0) ;
        if (*(p++) != 'f') return(0) ;
        while (isalpha(*p)) p++ ;
        s = lookup(texdef_dic, o, (p-o));
        if (!s) return(0) ;
	return(isanifmacro(s)) ;
    }
    if (*o == '\6') return(1) ;
    return(0) ;
}

static int isanif()
/*++++++++++++++++
.PURPOSE  Check if the input is an IF condition
.RETURNS  0 (false) / 1 (true)
-----------------*/
{
  char *o, *p, *s;
    p = o = inbuf->text + inbuf->pos ;
    if (*(p++) != 'i') return(0) ;
    if (*(p++) != 'f') return(0) ;
    while (isalpha(*p)) p++ ;
    s = lookup(texdef_dic, o, (p-o));
    return( s ? isanifmacro(s) :0 ) ;
}

static int starts_alpha(str)
/*++++++++++++++++
.PURPOSE  Match the input text as alphabetic string starting by str
.RETURNS  0 (false) / 1 (true)
-----------------*/
  char *str;	/* IN: Word to check */
{
  char *p, *s;
    s = str;
    p = inbuf->text + inbuf->pos ;
    if (!*s) return(isalpha(*p)) ;
    while (*p && (*p == *s)) p++, s++ ;
    if (isalpha(*p) || isalpha(*s)) return(0) ;
    return(1) ;
}

static char *match_comment(o)
/*++++++++++++++++
.PURPOSE  Load and convert a Comment (until end of line)
.RETURNS  The comment
-----------------*/
  int o; 	/* IN: Option 1 to save the comment */
{
  static char *com;
  char *p; int n;

    p = inbuf->text + inbuf->pos ;
    n = strloc(p, '\n');
    if (o) {	/* Copy of the comment */
    	if (com) free(com);
    	com = strdup(p);
    	com[n] = 0;
	if (verbop&1)
	    PutString("<!-- #"), PutString(com), PutString(" -->\n") ;
    }
    else com = (char *)0;
    if (p[n]) n++;
    inbuf->pos += n ;
    return(com);
}

static int match_env()
/*++++++++++++++++
.PURPOSE  Match an environment variable, and print its equivalence
.RETURNS  0 (not environment var) / 1 (done, printed)
-----------------*/
{
  char *p;
    p = inbuf->text + inbuf->pos ; 	/* $ already scanned */
    inbuf->pos += print_envariable(p) ;
    return(1) ;
}

static int match_latin()
/*++++++++++++++++
.PURPOSE  Load a phrase; '&' has been read, look for ';' and check
	if "LATIN ISO" expression
.RETURNS  0 (not LATIN) / 1 (LATIN string printed)
-----------------*/
{
  static char *expr;
  char *p;
  int n;

    if (!expr) 		/* Compile expression */
	expr = compile_expr("[#X0-9A-Za-z]*;");
    p = inbuf->text + inbuf->pos ;
    if (n = strexp(expr, p, 1)) {
	theMode |= MODE_verb1 ;	/* The character is just issed */
	PutChar('&');
	while (--n >= 0) theMode |= MODE_verb1, PutChar(*p), p++;
	inbuf->pos = (p - inbuf->text);
	return(1);
    }
    return(0);
}

static int match_glu(end)
/*++++++++++++++++
.PURPOSE  Match a GLU Service, as <[%&]tag args|text>
.RETURNS  0 (not GLU) / 1 (GLU, printed)
.REMARKS  tag must start by an ALPHABETIC. The 'w' option is added here.
-----------------*/
  int end;	/* IN: Character ending the phrase (must be in c1sep) */
{
  int n; char *o, *p, *s, *ow, *tmp;
    o = inbuf->text + inbuf->pos ;
    p = s = unescaped_string(o, 0);
    if ((*p != '%') && (*p != '&')) return(0) ;
    /* V8.53: if option ogluw specified */
    ow = (*p == '&') && (oglu&2) ? ",w" : "";
    p++;
    if (!isalpha(*p)) return(0) ;
    /* V8.53: append the ,w option to the glu tag. Find first the GLU tag */
    while (isgraph(*p) && (*p != ',') && (*p != end)) p++;
    if (*p == ',') ow = "";	/* V8.53: don't touch existing GLU options */

    while (*p && (*p != end)) {
	if ((*p == '"') || (*p == '\'')) p += strquote(p);
	if (*p) p++ ;
    }
    if (*p != end) return(0);		/* '>' not found  */
    *p = 0;				/* The byte was > */
    n = p-s ;	/* Matching length */
    inbuf->pos += unescaped_length(o, n);

    /* V8.53: May have arguments! */
    if (strchr(s, '\021')) s = tmp = rescan21(s, -1);
    else tmp = (char *)0;
    make_anchor(ow, s, "");
    if(tmp) free(tmp);

    if (end) *p = end, inbuf->pos += 1;
    return(1) ;
}

static int match_serv(end)
/*++++++++++++++++
.PURPOSE  Match a Service, as <serv:...>
.RETURNS  0 (not HTML) / 1 (HTML, printed)
.REMARKS " or ' are allowed
-----------------*/
  int end;	/* IN: Character ending the phrase (must be in c1sep) */
{
  int n1, n2, n3; char *o, *p1, *p2, *p3, *pe, quote;
  static char *expr;

    if (!expr) 		/* Compile expression */
	expr = compile_expr("[A-Za-z][A-Za-z0-9_#]*:");
    quote = 0;
    o  = inbuf->text + inbuf->pos ;
    p1 = unescaped_string(o, 0);
    n1 = strexp(expr, p1, 1) ;
    if (n1 <= 0) return(0);
    p2 = p1 + n1 ;	/* What follows the : */
    n1-- ;
    /* Check the "http://" or "ftp://" or "file:/" */
    if (! end) {
	if ( p2[0] != '/') return(0) ;
	if ((p2[1] == '/') && isalnum(p2[2])) ;
	else if (isalnum(p2[1])) ;
	else return(0) ;
    }
    /* if ((! end) && ((p2[0] != '/') || (p2[1] != '/') || (!isalnum(p2[2]))))
	return(0); */
    if ((*p2 == '"') || (*p2 == '\'')) {	/* Argument within quotes */
    	quote = *(p2++) ;
    	n2 = strloc(p2, quote);
    	if (! p2[n2]) return(0) ;
    	p3 = p2 + n2 + 1;
    }
    else {
	if (end) {
	    n2 = strloc(p2, end);
	    if (! p2[n2]) return(0) ;
	}
	else {
	    n2 = strhttp(p2) ;
    	    /* for (p3=p2+1; *p3 && (!c1sep[strloc(c1sep,*p3)]); p3++ ) ; */
	}
	p3 = p2 + n2;
    }

    if(end) {					/* Look for text	*/
    	n3 = strloc(p3, end) ;
    	if (! p3[n3]) return(0);
    	pe = p3 + n3 + 1;
    	if (! isspace(end)) while (isspace(*p3)) p3++, n3--;
    }
    else pe = p2 + n2, n3 = 0;

    if (n3 == 0) p3 = p2, n3 = n2;

    quote = *pe;
    p1[n1] = 0; p2[n2] = 0; p3[n3] = 0;
    make_anchor(p1, p2, p3);
    *pe = quote;
    inbuf->pos += unescaped_length(o, pe - p1);
    return(1);
}

static int match_serv1(nothisbyte)
/*++++++++++++++++
.PURPOSE  Match a Service, excludes the byte specified
.RETURNS  See match_serv
-----------------*/
  int nothisbyte;	/* IN: Character may end the phrase */
{
  int saved_not1, ret ;
    saved_not1 = not1char ;
    not1char = nothisbyte ;
    ret = match_serv(0) ;
    not1char = saved_not1 ;
    return(ret) ;
}


static int match_mail(end)
/*++++++++++++++++
.PURPOSE  Match a Mail Service, as <addr>
.RETURNS  0 (not HTML) / 1 (HTML, printed)
-----------------*/
  int end;	/* IN: Character ending the phrase (0 = any c1sep) */
{
  int alen, atot, i; char *p, *o, *a, echar;
  static char *expr;
  static char x02[4] = "%02";
    if (!expr) 		/* Compile expression */
	expr = compile_expr(
	"[A-Za-z][$A-Za-z0-9_.-]*@[A-Za-z][$A-Za-z0-9_-]*[.][$A-Za-z0-9_.-]*");
    o = inbuf->text + inbuf->pos ;
    p = unescaped_string(o, 0);
    alen = strexp(expr, p, 1) ;		/* v8.16: Length of address */
    if (alen <= 0) return(0);

    /* If there is a match, verify that:
       => if end is provided, this character is following the match ;
       => otherwise, the following char is a separator (in "sep")
       => Accept however the ?Subject= !
    */
    if (p[alen] == '?') {		/* v8.16: Accept a Subject */
        for (i=alen; isgraph(p[i]) && (p[i] != end) && (!strchr(c1sep, p[i]));
	   i++ ) ;
	atot = i;
    }
    else atot = alen;
    if (end) {
	if (p[atot] != end) return(0);
	if (end == '>') p[atot++] = 0;
    }
    else {
    	i = strloc(c1sep, p[atot]) ;
    	if (c1sep[i] == p[atot]) ;		/* Version 6.93 */
	else if (p[atot] == '\\') ;
	else return(0) ;
    }

    echar = p[atot] ; p[atot] = 0;
#if 1	/* v8.3: Crypt Email Address -- only when there is an output */
    if (OutFct != null_out) {
        putstring("<SCRIPT language=\"JavaScript\">\n");
        a = echo1x("1");	/* #1 = Javascript Mail */
        if (*a) {
	    /* Skip the \begin{javascript} line, and \end{javascript} */
	    a += strloc(a, '\n');
	    if (*a) a++;
	    i = strbloc(a, '\n')+1;
	    if (i>0) putbuffer(a, i);
        }
        /*if (js4mail[0]) putstring(js4mail); js4mail[0] = 0;*/
        a = strdup(p);
        a[alen] = 0; acode(a);
        putstring("acode(\"");
        putstring(a);
        putstring("\",\"");
        x02[1] = 0;		/* Write Subject= */
        while (++alen < atot) {			/* There is a Subject */
	    if (a[alen] == '&') x02[1] = 0;	/* Separe arguments   */
	    if (!x02[1]) {
	        putchar(a[alen]);
	        if (a[alen] == '=') x02[1] = 1;
	        continue;
	    }
	    sprintf(x02+1, "%02X", a[alen]&0xff);
	    putstring(x02);
        }
        putstring("\")\n");
        putstring("</SCRIPT>");
        free(a);
    }
#else
    if (alen < atot) {				/* v8.16: Subject not in text */
        a = strdup(p);
	a[alen] = 0;
    }
    else a = (char *)0;
    make_anchor("Mail", p, a);
    if (a) free(a);
#endif
    p[atot] = echar;
    inbuf->pos += unescaped_length(o, atot) ;
    return(1);
}

static int match_html(int in_comment)
/*++++++++++++++++
.PURPOSE  Load a phrase; '<' has been read, look for '>' and check
	if HTML expression
.RETURNS  0 (not HTML) / 1 (HTML, printed -- but stopped at \ or $ !!)
	  2 (in comment -- added V9.03)
-----------------*/
{
  TABLE2 *t2; char **a;
  char *p; int i, n ;

    p = inbuf->text + inbuf->pos ;
    n = 0;	/* Match indicator */

    if (in_comment&2) /* V9.03: if in a comment, print until end of comment */
	goto in_HTML_comment;
    for (t2 = html_dic ; (n==0) && t2 ; t2 = t2->prev) {
	for (i=t2->items, a=t2->rows; (n==0) && (--i>=0); a += 2) {
	    if (! a[1]) a[1] = compile_expr(a[0]);
	    n = strexp(a[1], p, 1);
	}
    }
    if (n <=0) {   /* Could be unclosed HTML comment */
	if (starts_by("!--")) { PutChar('<') ; in_comment = 2; }
	else return(0) ;
    }
    /* V9.03: if in a comment, print until end of comment */
  in_HTML_comment:
    if (in_comment&2) { char *e; int r = 2;
	if ((e = strstr(p, "-->"))) { e += 3; r = 1; }
	else e = p + strlen(p);
	while (p<e) { PutChar(*p); p++; inbuf->pos += 1; }
	return(r);	/* Returns 1 if end-of-comment found, 2 if not found */
    }

    /* Matched; print characters until \ or $ */
    PutChar('<') ;
    while (--n >= 0) {
	if (*p == '$') break ;
	if (*p == '\\') break ;
	PutChar(*p) ;
	p++; inbuf->pos += 1 ;
    }
    return(1);
}

static int match_bib(end)
/*++++++++++++++++
.PURPOSE  Load and convert Phrase <Ref>
.RETURNS  0 (not a BibCode, text in inbuf) / 1=Anchor edited
.REMARKS The ending character is never issued.
-----------------*/
  int end;	/* IN: Character ending the phrase (0 = any c1sep) */
{
  int n, i, ire; char *p, *o, *tag, x;
  static char *arXiv[ITEMS(arXivExp)];

    if (!arXiv[0]) {	/* Compile expression */
	/* expr = compile_expr(BibCodeExp);   */
	for (i=0; i<ITEMS(arXivExp); i++)
	    arXiv[i] = compile_expr(arXivExp[i]) ;
    }
    o = inbuf->text + inbuf->pos ;
    p = unescaped_string(o, 19);
    n = ire = -1;
    while ((n<=0) && (ire<(ITEMS(arXivExp)-1))) {
        n = strexp(arXiv[++ire], p, 1) ;
    }
    if (n <= 0) return(0);

    /* If there is a match, verify that:
       => if end is provided, this character is following the match ;
       => otherwise, the following char is a separator (in "sep")
    */
    if (end) {
	if (p[n] != end) return(0);
	p[n] = 0;
	n++ ;
    }
    else {
    	i = strloc(c1sep, p[n]) ;
    	if (c1sep[i] == p[n]) ;		/* Version 6.93 */
	else if (p[n] == '\\') ;
	else if (p[n] == ':') ;		/* Version 7.34 */
	else return(0) ;
    }

    i = 0;				/* Bytes to skip in input        */
    if (obib) {				/* Use GLU */
        tag = arXivGlu[ire];
	if (ire>0) {			/* astro-ph: skip the "astro-ph" */
	    while ((p[i] != ':') && (p[i] != '/')) i++;
	    if (p[i]) i++;
	}
	else tag = obib;
    }
    else tag = "Bib" ;
    x = p[n] ; p[n] = 0;
    make_anchor(tag, p+i, p /* (char *)0 */);
    p[n] = x;
    inbuf->pos += unescaped_length(o, n) ;
    return(1);
}

static int match_cat(end)
/*++++++++++++++++
.PURPOSE  Load and convert Phrase <Catalogue>
.RETURNS  0 (not a catalogue, text in inbuf) / 1=Anchor edited
-----------------*/
  int end;	/* IN: Character ending the phrase (0 = any) */
{
  static char *expr;
  static char *expr2;	/* Extended expression for GCPD etc  */
  char *o, *p, *u, *arg, *text; int c;
  int n, i;

    if (!expr) {	/* Compile expression */
	/* NO, the & is NOT allowed in a catalog !!! Mod. v7.86
	expr = compile_expr("[IJVBX][IVX]*[/][A-Za-z0-9][A-Za-z0-9/+&._]*");
	*/
	expr = compile_expr(
	  "[IJVBX][IVX]*[/][A-Za-z1-9][A-Za-z0-9+._]*[A-Za-z0-9/+._-]*");
	expr2= compile_expr("[A-Z][A-Z]*[/][A-Za-z0-9][A-Za-z0-9]*[>]") ;
    }
    o = inbuf->text + inbuf->pos ;
    p = unescaped_string(o, 0);

    arg = p ; u = oglu ? ocat : "Cat" ;
    text = (char *)0;

    /* CDS accepts a catalogue written in a four-digit code */
    if (isdigit(*p) && (end == '>')) {
	for (n=0; isdigit(p[n]); n++) ;
	if (n != 4) return(0);
	if (p[n] != end) return(0);
	p[4] = 0;
	make_anchor(u, p, text);
    	inbuf->pos += unescaped_length(o, n);
	inbuf->pos += 1;
    	return(1);
    }

    /* Look from regular expression. Beware, <GCPD/..> accepted ! */
    n = strexp(expr, p, 1) ;
    if ((n <= 0) &&  (end == '>'))
	n = strexp(expr2,  p, 1) ;
    if (n <= 0)  return(0);

    /* B/... may be confused with a lot of things... Verify (V8.62) */
    if ((p[0] == 'B') && (!islower(p[2]))) return(0);
    if ((p[0] == 'J') && (!isupper(p[2]))	/* Accept however J/other */
       && (p[2]!='o') && (p[2]!='t'))  return(0);

    /* The last character of catalog expression MUST be letter or digit */
    while (!isalnum(p[n-1])) n--;

    /* Beware, B/A is not a catalog !! */
    if ((n < 4) && (!isdigit(p[n-1])))
	return(0) ;

    /* If there is a match, verify that:
       => if end is provided, this character is following the match ;
       => otherwise, the following char is a separator (in "sep")
    */
    if (end) {
	if (p[n] != end) return(0);
    }
    else {
    	i = strloc(c1sep, p[n]) ;
    	if (c1sep[i] == p[n]) ;		/* Version 6.93 */
	else if (p[n] == '\\') ;
	else if (p[n] == '/') ;		/* Version 8.11 */
	else return(0) ;
    }

    /* Does the catalogue match a fully described type ? */
    if (getequiv2(u, p)) {
	c = p[n]; p[n] = 0;
	make_anchor(u, p, text);
	p[n] = c;
	inbuf->pos += unescaped_length(o, n);
	if (end) inbuf->pos += 1;
    	return(1);
    }

    switch(*p) {
      case 'B':
	if (p[1] != '/') return(0);
	break ;
      case 'J':
    	if (*++p != '/') return(0);
	p++ ;
	while (isalnum(*p) || (*p == '+')) p++;
	if (*p != '/') return(0);
	break;
      case 'I': case 'V':
	while ((*p == 'I') || (*p == 'V') || (*p == 'X')) p++;
    	if (*(p++) != '/') return(0);
	if (! isdigit(*p)) return(0);
	break;
      default:
	return(0);
    }

	/* It is most likely a Catalogue. Just remove the last / */
    p = arg + n ;
    if (p[-1] == '/') p[-1] = 0;
    c = *p ; *p = 0;
    make_anchor(u, arg, text);
    *p = c;
    inbuf->pos += unescaped_length(o, n);
    if (end) inbuf->pos += 1;
    return(1);
}

static char *symb1(int symbol)
/*++++++++++++++++
.PURPOSE  Matching single-char symbol
.RETURNS  Equivalence string
-----------------*/
{
    switch(symbol) {
	case '~': return("\\sim");
	case '-': return("\\ndash");
    }
    return((char *)0);
}

static int match_symbop(char *text)
/*++++++++++++++++
.PURPOSE  Matching symbolic operators like +/- <~ ...
.RETURNS  Length of matched symbol (0 = nothing, min=2)
-----------------*/
{
  int i;
    if (!ispunct(text[0])) return(0);
    if (!ispunct(text[1])) return(*text == '~' ? 1 : 0); /* Single ~ is \sim */
    if (ispunct(text[2])) { 	/* Try 3 symbols */
	if (ispunct(text[3])) {
	    if(lookup(texdef_dic, text, 4)) {	/* Fixed V9.31 */
	        if ((text[1]==text[0]) && (text[2]==text[0])
			               && (text[3]==text[0]))  {
		    /* Special rules ==== and ---- and ____ */
		    for (i=4; text[i] == text[0]; i++) ;
		    if (text[i] == '\n') return(++i);
		    if (text[i] == 0) return(i);
		    return(0);
		}
		return(4);
	    }
	}
	if (lookup(texdef_dic, text, 3)) return(3);
    }
    if (lookup(texdef_dic, text, 2)) return(2);
    return(0);
}

static int match_tex2(int end)
/*++++++++++++++++
.PURPOSE  Load a soft-TeX expression (^..^ _.._  {MACRO})
.RETURNS  0 (not TeX) / 1 (TeX, translation in inbuf->text).
		Last byte read assumed to be \
   Accept ^2 ^  but not ^hello ^
.REMARKS  end=0 ==> free-search for things like <= <~ ~~
-----------------*/
{
  char *o, *p, *s, macro[24];
  int has_punct=0;

    if ((theMode&MODE_math) && ((end == '^') || (end == '_'))) {
	pop_tag(); theMode ^= MODE_math;
	return(1);
    }
    p = o = inbuf->text + inbuf->pos;
    if (end==0) {
	p += match_symbop(o);
	if (p == o) return(0);
	has_punct = 1;
    }
    else while (*p != end) {		/* No blank allowed	*/
	if (!*p) return(0);
	if (isspace(*p)) {
	    if (p[1] != end) return(0);
	    if ((p-o) > 3) return(0);
	}
	if (!isalnum(*p)) has_punct++;
	p++;
    }
    /* Take care of Indices / Exponents */
    s = (char *)0;
    if (end == '^') s = "<SUP>";
    else if (end == '_') s = "<SUB>";
    if (s) {
	theMode |= MODE_math;
	PutString(s); push_tag(s);
	return(1);
    }

    /* I have something like {ALPHA}. Just look if the corresponding macro
       \ALPHA exists as a HTML tag &...;. I yes, edit it -- otherwise,
       just ignore
    */
    s = lookup(texdef_dic, o, p-o);
    if ((!s) && has_punct && ((p-o)>4)) s = lookup(texdef_dic, o, 4);
    if (!s) {
	macro[0] = 0;
	if (((p-o)==1) && (s = symb1(*o))) 	/* e.g. ~ --> simeq */
	    strcpy(macro, s+1);
	else if (has_punct && ((p-o)<sizeof(macro))) {
	    /* e.g. {prop.to} -> popto */
	    for (has_punct=0, s=o; s<p; s++) {
		macro[has_punct] = *s;
		if (*s != '.') has_punct++;
	    }
	    macro[has_punct] = 0;
	}
	s = lookup(texdef_dic, macro, strlen(macro));
    }
    if (!s) return(0);
    if (*s == '\21')	/* Macro with arguments, not accepted (v7.35) */
       return(0);
    inbuf->pos += (p-o); if (end) inbuf->pos += 1;
    if (*s == '&') PutString(s);
    else {
	push_text("\\end{tex}", -1);
	push_text(s, -1) ;
	push_text("\\begin{tex}", -1);
    }
    /* PutString(s); */
    return(1);
}

static int match_tex()
/*++++++++++++++++
.PURPOSE  Load a TeX expression (accents)
.RETURNS  0 (not TeX) / 1 (TeX, translation in inbuf->text).
		Last byte read assumed to be \
-----------------*/
{
  char def_macro[2*LEN_TEX+24];		/* #1{<macro>#1</macro>}	*/
  static char amod_itexparm[4] = "\4" ;
  static char amod_texmode[4] = "\27" ;
  static char accent[4];
  /* static char amod_soft[4] = "\7" ; */
  int  itexparm0, itexenv0, accept_equal, macrostar, omode ;
  char *p, *o, *s, *a=0, c;
  int i, len;

    /* 	Fetch chars in input line; stop when a TeX macro is over,
	i.e. when a non-alpha char is encountered.
	The equivalence of a macro is saved in the 's' variable,
	which is "\1accent_symbol" for accentuation.
    */

    p = o = inbuf->text + inbuf->pos;
    s = (char *)0;				/* s is translated macro*/
    c = *(p++);
    macrostar = 0 ;
    accent[2] = 0;	/* Set to \1  for accents / special translation */
    if (isalpha(c)) {
	accept_equal = 1;	/* By default accept e.g. pagewidth=15cm */
    	while (isalpha(*p)) p++;
	if (*p == '=') { 	/* V9.3: \not= is a problem */
	    p++;
	    if (lookup(texdef_dic, o, (p-o))) accept_equal = 0;
	    else p--;
	}
	else if (isdigit(*p)) {			/* "Extended" TeX macro */
	    for (a = p; isalnum(*a); a++) ;
	    if (lookup(texdef_dic, o, (a-o))) p = a ;
	}
    	s = lookup(texdef_dic, o, (p-o));
	/* A macro starting by "\the" and something is blank by default */
	if ((!s) && (strncmp(o, "the", 3) == 0) && isalpha(o[3])) /* V8.64 */
	  s = "";
	/* Accept as a valid macro e.g.   \vskip*{12cm}	*/
	if (s && (p[0] == '*') && (p[1] == '{') /*}*/) {
	    counter[COUNT_macrostar] = macrostar = 1 ;
	    p++ ;
	}
	/* Eat up the spaces (if NOT IN PRE only !!!) */
	if (!(theMode&MODE_PRE))
	    while ((*p == ' ') || (*p == '\t')) p++;
	c = *p ;
	/* Implicit definition \foo{text} --> <foo text> or <foo>text</foo> */
	if ((!s) && (c == '{') && ((p-o)<LEN_TEX)) {	/* } Def. Macro	*/
	    a = def_macro;	/* Define as {}, redefined later at DFT */
	    *(a++) = '\021' ; *(a++) = '1' ; *(a++) = '{' ;
	    *(a++) = '\10'; *(a++) = '<';
	    for (s=o; s<p; s++) *(a++) = *s ;
	    *(a++) = '\10'; *(a++) = '>';
	    *(a++) = '\021' ; *(a++) = '1' ;
	    *(a++) = '\10'; *(a++) = '<'; *(a++) = '/' ;
	    for (s=o; s<p; s++) *(a++) = *s ;
	    *(a++) = '\10'; *(a++) = '>'; *(a++) = '}' ;
	    *a = 0;
	    s = def_macro ;
	}
    }
    else {
    	i = strloc(tex_acc, c);
    	if (tex_acc[i]) {
    	    s = accent;
    	    s[0] = '\1'; s[1] = c;
	    c = *p;
    	}
    	else {	/* Escaped character */
	    accent[0] = c;
	    accent[2] = '\1';	/* Indicates special char e.g. & or + */
    	    s = lookup(texdef_dic, accent, 1);
	}
    }

    if (!s)  {			/* Can't be TeX ... Look however \macro=value */
	if_mode(MODE_VERB) return(0) ;
	if (*p == '=') { unget_char('\\'); tex_def() ; return(1); }
    	return(0);
    }

    /* Write out the equivalence string, taking care of
       special cases  \1 (Accents) and \2 (Typefaces)
    */
    switch(*s) {
      case '\1':		/* Accent as & letter HTML_tag	*/
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
    	inbuf->pos = (p - inbuf->text);
    	i = strloc(tex_acc, s[1]);
	p = get_texparm(0) ;
	/* if ((p[0] == ' ') && (p[1] == 0)) PutChar(tex_acc[i]) ;*/
	if ((*p == '\0') || (*p == ' ')) PutChar(tex_acc[i]) ;
	else { char *aa ;
	    if (*p == '\\') p = aa = rescan(p, strlen(p)) ;
	    else aa = (char *)0 ;
	    if (html_acc[i]) {		/* Standard translation	*/
		if (strchr(letters_acc[i], *p)) {
		    theMode |= MODE_verb1 ;
	            PutChar('&');
		    theMode |= MODE_verb1 ;
		    PutString(p),
		    theMode |= MODE_verb1 ;
	            PutString(html_acc[i]) ;
		}
		else PutString(p) ;	/* Ignore the Accent	*/
	    }
	    else 			/* Need Subroutine	*/
		PutString(html_spec(tex_acc[i], *p)) ;
	    if (aa) free(aa) ;
	}
	itexparm-- ;
	return(1) ;
      case '\2':		/* Typefaces...			*/
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
      	s++;
      	if (! *s) {		/* Remove the typefaces		*/
      	    rm_typeface();
      	    break;
    	}
	omode = (theMode&MODE_intag); theMode |= MODE_intag;
    	PutString(s);
	if (omode==0) theMode &= ~MODE_intag;
    	push_tag(s-1);		/* Beware, the \2 MUST be stacked... */
      	break;
      case '\3':		/* Font Sizes...		*/
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
      	s++;
	omode = (theMode&MODE_intag); theMode |= MODE_intag;
	PutString("<FONT SIZE='"), PutString(s), PutString("'>") ;
	if (omode==0) theMode &= ~MODE_intag;
      	push_tag("\3");
      	break;
      case '\5':		/* TeX Environments		*/
	if_mode(MODE_VERB) {	/* in{verbatim}	*/
	    if (!s[1]) return(0) ;
	    while (isspace(*p)) p++ ;
	    if (strncmp(p, "{verbatim}", 10) != 0) return(0) ;
	}
	len = (p - inbuf->text) - inbuf->pos ;	/* Processed...	*/
    	inbuf->pos += len ;
        if (s[1] == '/')  {	/* \end{ env }			*/
	    i = end_env(s[2]) ;
	    if (i == 0) inbuf->pos -= len ;
            return(i) ;
	}
        return(begin_env(s[1])) ;
      case '\10':	/* Verbatim next byte   */
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
        s++;
        PutChar(*s);
        s++;
        break ;
      case '\021':	/* There are parameters */
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
    	inbuf->pos = (p - inbuf->text);		/* Input chars used	*/
	itexparm0 = itexparm; itexenv0 = itexenv;
	i = *++s - '0' ; s++ ;
	if (i!=1) accept_equal = 0;	/* e.g. pagewidth=12cm */
    	while (--i >= 0) {
	    if (a = get_texparm(accept_equal)) continue;
	    push_text("\377\377", 2) ;
	    return(0) ;
	}
	/* Automatically generated definition: use <tag \verbatim{arg}>
	   when the argument contains equal signs, e.g. IMG....	 DFT	*/
	if ((s == (def_macro+2)) && strchr(a, '=')) {
	    a=s+strlen(s) ; while (*(a--) != '\021') ;
	    strcpy(a-1, "\\verb\v \021\061\v\010>}") ;	/* v7.48 */
	}
	if (*s == '\3') {		/* FONT Colors (or other)...	*/
	    s++;
	    omode = theMode&MODE_intag;
	    theMode |= MODE_intag;
	    PutString("<FONT ") ;
	    if (omode==0) theMode &= ~MODE_intag;
	    push_tag("\3");
	}
	if (*s) {			/* There is an Equivalence	*/
	    push_text(s, strlen(s)) ;
	    set_texparm(inbuf->text, itexparm0, 0);
	    amod_itexparm[1] = '0' + (texenv - tex_stack);
	    /* Remove the outer braces of a Definition */
	    if ((inbuf->text[0] == '{') &&
		(inbuf->text[inbuf->used-1] == '}')) {
		inbuf->pos  += 1 ;		/* Remove the outer { }	*/
		inbuf->used -= 1 ;
#if 1		/* The `soft' mode cannot work. Remove it completely,
		   otherwise there are terrible problems with the \if's
		   V8.4: However don't go back to soft
		   when e.g. SET1mode !!!
		   The change of status is indicated by
		   \07 \17 \27 \37  (i.e. c&0xe7 == 7)
		   or \06\21 (setimode)
	    	*/
	    	if (soft_tex == MODE_soft){	/* Hide the soft mode	*/
		    p = inbuf->text + inbuf->pos;
		    if (((*p&0xe7)==7) || ((p[0]=='\6')&&(p[1]=='\21'))) {
		    }
		    else {	/* Remove the Soft mode, add it afterward */
		        amod_texmode[1] = theMode&0xff;
		        theMode ^= MODE_soft;
		        inbuf = buf_append(inbuf, amod_texmode, 2);
		    }
	        }
#endif
		/* Have to reset to '*' counter if necessary */
		if (macrostar) inbuf = buf_append(inbuf, "\\mS0", 4) ;
		/* The absolute resetting is required for macros like
		   \def\crossover#1#2{ #1 & #2}
		*/
		if (itexparm != itexparm0) {
		    /* amod_itexparm[2] = '0' + (itexparm - itexparm0) ; */
		    amod_itexparm[2] = '0' + itexparm0 ;
		    inbuf = buf_append(inbuf, amod_itexparm, 3) ;
		}
	    }
    	}
      	return(1);
      case '&':					/* Latin-HTML counterp.	*/
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
	/* ADDED V8.41: in MODE_xArg, be sure to issue the
	   %-transformed of ORIGINAL character
	*/
	if_mode(MODE_xArg) {
	    if (accent[2] == '\1') { PutChar(accent[0]); break; }
	}
	/* Don't Translate the & again into &amp -- therefore
	   remove the MODE_2html !
	*/
#if 1	/* Change in V8.39 */
	while (*s) {
	    theMode |= MODE_verb1 ;
	    PutChar(*s);
	    s++;
	}
#else	/* Before V8.39    */
	theMode |= MODE_verb1;
	PutString(s);
	s += strlen(s) ;
#endif
	break ;
      case '+':			/* V8.54: take care of '+' in cgi */
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
	if (accent[2] == '\1') {		/* Comes from \+ */
	    if_mode(MODE_xArg) {
		PutChar(accent[0]);
		break;
	    }
	    else if ((theMode&MODE_hrefa) == MODE_hrefa) {
		theMode |= MODE_verb1;
		PutChar(*s);
		break;
	    }
	}
	/* NOBREAK -- use the detfault */
      default:
	if_mode(MODE_VERB) return(0) ;		/* in{verbatim}	*/
	while (tex_ok[*s]) PutChar(*s), s++;
	if (*s) {
    	    inbuf->pos = (p - inbuf->text);	/* Input chars used	*/
	    len = strlen(s) ;
    	    buf_new(len+1);
    	    strcpy(inbuf->text, s);
    	    inbuf->used = len;
	    if (*(inbuf->text) == '{') /*}*/ {	/* Push immediately	*/
		push_tex((char *)0, (char *)0);
		inbuf->pos += 1;
		if (soft_tex) theMode ^= MODE_soft ;
	    }
    	    return(1) ;
	}
    }

    /* The HTML is issued. Advance in buffer of processed chars. */
    inbuf->pos = (p - inbuf->text);

    return(1);
}

static int match_if(cond)
/*++++++++++++++++
.PURPOSE  Match an \if .. \else|\elsif .. \fi Condition
.RETURNS  0  / -1 (error)
.REMARKS  We assume that se start just after the \ifxx.
	The matching clause (then or else) is stacked.
	*** Hard Definitions of \else  \fi
	It's also possible to use
	\ifmatch{template}{value} bla bla
	\elsifnum{value}op{value} bla bla bla
	\else { etc }
	\fi
-----------------*/
  int cond ;	/* IN: 0 if \else clause to keep only	*/
{
#if 1	/* Hard Version ... In the Soft, just set the mode */
  int state, depthif, depthbra, in_comment, k, c ;
  BUF *accu ; char a ;

    state = cond ? 1 : 0 ;
    in_comment = 0 ;
    depthif = 1 ;
    depthbra = 0;
    accu = (BUF *)0 ;
    /* Accumulates the input into the 'accu' buffer	*/
    k = 0 ;	/* Number of continuation characters	*/
    while (depthif > 0) {
    	c = get_char() ;
      storeByte:
	if (k > 0) {
	    if (state) a = c, accu = buf_append(accu, &a, 1) ;
	    k-- ;
	    continue ;
	}
    	if (c == EOF) { depthif = 0 ; in_comment = 0; continue ; }
	else if (c == '\n') { in_comment = 0 ; k = 1; goto storeByte ; }
	else if (c == '%')  in_comment = 1 ;
	else if (c == '{')  depthbra++;
	else if (c == '}')  {
	    if(depthbra) { depthbra--;  k = 1; goto storeByte; }
	    fprintf(stderr, "++++cgiprint: right brace ignored in if_match\n");
	}
	else if (c == '\4') { reset_texparm() ; continue ; }
	if (in_comment) { k = 1; goto storeByte ; }
	if (depthbra)   { k = 1;
	    if (c == '\\') {
		if (!starts_alpha("")) { k = 2 ; goto storeByte ; }
	        if (starts_alpha("fi")) {
		    errors_fi++;
		    if (errors_fi<3) fprintf(stderr,
	              "++++cgiprint: \\fi encountered at brace level: %d\n",
		      depthbra);
		}
	    }
	    goto storeByte ; 	/* In an argument */
	}
	k = charlot[c&0xff] ;	/* Number of bytes to use readily */
    	if (c == '\\') {
	    /* When a non-alpha macro, save the 2 characters e.g. \% */
	    if (!starts_alpha("")) { k = 2 ; goto storeByte ; }
    	    if (starts_alpha("fi")) {
    	    	depthif--;
    	    	if (depthif == 0) {
    	    	    get_char(); get_char(); 	/* Gobble fi ... and spaces */
		    /* V9.43: keep newline following \fi */
		    if (!(theMode&MODE_PRE)) {  int last_space;
			last_space = gobble_spaces();
			if ((last_space>0) && (cond==0) && iscntrl(last_space))
			    unget_char(last_space);
		        /* while (isspace(next_char())) get_char(); */
		    }
    	    	    continue;
    	    	}
	    }
    	    else if (starts_alpha("else") && (depthif == 1)) {
    	    	state = cond ? 0 : 1 ;
    	    	get_char() ; get_char() ;
    	    	get_char() ; get_char() ;
		gobble_spaces(); /* while (isspace(next_char())) get_char(); */
    	    	continue ;
    	    }
	    else if (starts_by("elsif") && (depthif == 1)) {
	        inbuf->pos += 3 ;	/* skip e l s */
		if (!isanif()) {
		    inbuf->pos -= 3;
		    k = 5 ;
		    goto storeByte ;
		}
		inbuf->pos -= 3;
		get_char() ; get_char() ; get_char() ;
		/* The elsif terminates if a true alternative was found. */
		if (cond) {	/* True alternative was already   found. */
		    state = 0 ;
		    get_char() ; get_char() ;
		    continue ;
		}
		unget_char('\\') ;	/* Transforms \elsifx into \ifx  */
		break ;
	    }
    	    else if (isanif()) 	/* Is it an \if macro ? */
		depthif++ ;
    	}
    	if (state) a = c, accu = buf_append(accu, &a, 1) ;
    }
    if (accu) push_buf(accu) ;
    return(c == EOF ? -1 : 0) ;
#else	/* Soft version: \if \else \fi installed by status mode */
  int output_status ;
    push_tex((char *)0, (char *)0);
    theMode &= ~(MODE_ifori|MODE_false) ;
    if (ignore_output) theMode |= MODE_ifori ;
    if (!cond) theMode |= MODE_false ;
    theMode &= ~(MODE_ignore) ;
    if (theMode&(MODE_ifori|MODE_false)) theMode |= MODE_ignore ;
    return(0) ;
#endif
}

/*==================================================================
		Make the Conversion
 *==================================================================*/

static char *setopt(opt_string)
/*++++++++++++++++
.PURPOSE  Set up the options -- REMOVES the option
.RETURNS  The next non-option (i.e. the file to process)
-----------------*/
  char *opt_string; 	/* IN: Options string (default is arg) */
{
static char fmt_target[] = "\n\1 <A TARGET=\"\2\" HREF=\"<\1,u $1>\">$1</A>" ;
  char *p, *j, *why, *unicode, *tofree=0;
  int np, pre, minus_plus;
  int prev_disable_output;
  int c;
  BUF *target, *defs;

    np = pre = 0;
    target = defs = (BUF *)0 ;
    unicode = (char *)0;
    cargs = 0 ;

    prev_disable_output = theMode&MODE_ignore;	/* Allow / Disable Comments */
    if (!verbop) theMode |= MODE_ignore      ;	/* ignore_output = !verbop; */
    /* V9.44: keep a copy */
    if (opt_string) { why = " input" ; tofree = args = strdup(opt_string); }
    else            { why = ""; tofree = args = (char *)0; }
    while (p = getarg()) {
    	if (np == 0) {
	    PutString("<!-- cgiprint-"), PutString(VERSION), PutString(why);
	    if (verbop) PutString(" options:") ;
	}
	np++;
	if (*p == '\n') {			/* Load Definitions	*/
	    PutString(" (definitions included)");
	    load_services(p, T2_BUFDUP);
	    continue ;
	}
	if (verbop) PutChar(' '), PutString(p) ;
	if (*p != '-') break;
	++p ;
	if (minus_plus = *p == '+') ++p ;
    	switch(*p) {
	    case 'D':			/* V8.8: additional definitions */
		if (! *++p) {		/* Must be followed by a definition */
		    fprintf(stderr, "++++Definition (-D) not complete\n%s\n",
			    usage);
		    exit(1);
		}
		if (isalnum(*p)) {	/* Missing \ , add it */
		    defs = buf_append(defs, "\\", 1);
		    c = 0;
		}
		else c = 1;
		while (isalnum(p[c])) c++;
		if (p[c] == '=') 	/* Offset of '=' for replacement */
		    c += defs ? defs->used : 0 ;
		else c = 0;
		defs = buf_append(defs, p, strlen(p));
		if (c) 			/* Replace '=' by a blank */
		    defs->text[c] = ' ';
		defs = buf_append(defs, "\n", 1);
		continue;
    	    case 'r': /* -ref	*/
    	    case 'b': /* -bib	*/
		while(isalpha(*p)) p++;
		if (*p == '=') j = p+1, obib = "%R";
		else if (*p)   {
		    obib = strdup(p) ;
		    if (j = strchr(obib, '=')) *(j++) = 0 ;
		}
		else  j = (char *)0, obib = "%R";
		if (j) target = buf_str3(target, fmt_target, obib, j, "") ;
		if (minus_plus) obib = (char *)0 ;
		continue;
    	    case 'c': /* -cat	*/
		if (strncmp(p, "cntr", 4) == 0) {	/* Keep Cntrl */
		    okeep |= 128 ;
		    continue ;
		}
		while(isalpha(*p)) p++;
		if (*p == '=') j = p+1, ocat = "%E";
		else if (*p)   {
		    ocat = strdup(p) ;
		    if (j = strchr(ocat, '=')) *(j++) = 0 ;
		}
		else  j = (char *)0, ocat = "%E";
		if (j) target = buf_str3(target, fmt_target, ocat, j, "") ;
		if (minus_plus) ocat = (char *)0 ;
		continue;
    	    case 'm': /* -mail	*/
		if (strncmp(p, "mat", 3) == 0) {
		    if (strchr(p, '^')) onomath |= 2 ;
		    continue;
		}
		while(isalpha(*p)) p++;
		omail = *p ? p : "%M";
		if (minus_plus) omail = (char *)0 ;
		continue;
	    case 'g': oglu = 1;
		/* fprintf(stderr, "++++-glu not yet completed\n"); */
		if (minus_plus) oglu = 0;
		else if (strchr(p, 'w'))	/* V8.53: -gluw */
		    oglu |= 02;
		continue ;
    	    case 't': 	/* tex[2] or tag */
		if (p[1] == 'e') {
		    otex = 1;
		    while(isalpha(*p)) p++;
		    if (*p) otex =2, theMode |= MODE_soft ;
		    if (minus_plus) otex=0;
		    continue;
		}
		/* NO BREAK */
	    case 'T':	/* -TAG */
		tag2html = getarg();
		PutChar(' '), PutString(tag2html) ;
		continue;
	    case 'o':
		if (output_filter && output_filter != oglu_filter)
		    free(output_filter) ;
		if (strncmp(p, "oglu", 4) == 0) {
		    oglu=1, output_filter = oglu_filter;
		    if (strchr(p, 'w'))	/* V8.53: -gluw */
			oglu |= 02;
		}
		else if (output_filter = getarg())
		    output_filter = strdup(output_filter);
		continue ;
    	    case 'p':
		if (strncmp(p, "pop", 3) == 0) opop = !minus_plus ;
		else pre = !minus_plus;
	    	continue;
    	    case 'f':	/* Load File */
		c = p[1];		/* -f2 for Checking */
    	    	p = getarg();
		if (!p) p = "----Missing_file_name_(-f_option)" ;
		PutChar(' '), PutString(p) ;
		if (c == '2') {		/* -f2 Option */
		    f2check = p;
		    tooLong(0);
		    continue;
		}
    	    	p = (*p == '#' ? load_until(p) : load_file(p,1));
    	    	if (!p) { wait_pipe(0); exit(1); }
    	    	load_services(p, 0);
    	    	continue;
	    case '-':		/* Assum --ic	*/
		verbop |= 1 ;
		continue ;
	    case 'i':		/* Ignore Comments */
		if (!minus_plus) verbop &= (~1) ;
		continue ;
	    case 'k':		/* -keep */
		while(isalpha(*p)) p++ ;
		if (minus_plus) okeep = 0;
		else for (; *p; p++) switch(*p) {
		  case '^': okeep |= 1 ; break ;
		  case '_': okeep |= 2 ; break ;
		  case '$': okeep |= 4 ; break ;
		  case '~': okeep |= 8 ; break ;
		  case '%': okeep |= 16 ; break ;
		}
		continue ;
    	    case 'L':
		olis= minus_plus ? 0 : 2;
    	    	continue;
    	    case 'l':
		if (p[1] == 'a') {	/* LANG option */
		    if (minus_plus) { options.lang = 0; continue; }
		    c = -1;
		    if (j = getarg()) {	/* Which language ? */
			c = strloc(languages, *j);
			if (languages[c] && iscntrl(languages[c-1])) {
			    while(isgraph(languages[c])) c++;
			    options.lang = languages[c]-1;
			}
		    }
		    if (c<0) {		/* Language can't be understood */
			fprintf(stderr, "++++Language ");
			if (j) fprintf(stderr, "'%s' ", j);
			fprintf(stderr, "unknown in option: -%s\n", p);
			c = 0;
		    }
		    continue;
		}
		olis= minus_plus ? 0 : 1;
    	    	continue;
    	    case 'n':
		if (strncmp(p, "nomath", 6) == 0) {
		    if (minus_plus) onomath = 0;
		    else           onomath |= 1;
		}
		else if (strncmp(p, "noout", 5) == 0) {	/* NO OUTPUT */
		    if (minus_plus)
			 disable_output = 0;
		    else disable_output = 1;
		}
		else /* option -n stdin_name */
		    thefile = getarg();
    	    	continue;
	    case 'H': 	/* Full Help 	  */
		obib = "%R"; ocat = "%E";
		olis = 2; otex = 1; ohtml = 0; oglu = 0;
		PutString(" -->\n");
		help();
		print_services("default");
		exit(0);
	    case 'h':	/* help or html ? */
	    	if (p[1] == 'e') { PutString(" -->\n"); help(); exit(0); }
		ohtml = minus_plus ? 0 : 2;
		continue;
	    case 'j': 	/* Java Script Version	*/
		while(isalpha(p[1])) p++ ;
		if (p[1] == '=') p++;
		vMozilla = p+1 ;
		/* V9.19: Add the value here ! */
		if (p = lookup(texdef_dic, "vMozilla", -1))
		    strncpy(p, vMozilla, 8);
		continue ;
#if 0
	    case 'u': 		/* Use unicode: symbols or numeric */
		if (unicode_symb[0] == 0) fprintf(stderr,
		    "++++%s: already loaded\n", p);
		else unicode = p;
		continue;
#endif
	    case 'v': 	/* Verbose or HTML version ? */
		if (strncmp(p, "ver", 3) == 0) {	/* -version */
		    printf("cgiprint -- version %s\n", VERSION);
		    exit(0);
		}
		if (!isdigit(p[1])) {
		    if (minus_plus) verbop = 0;
		    else verbop |= 2;
		}
    	    	else { vhtml = p + 1 ; vMozilla = vhtml; }
    	    	continue ;
	    case '%':	/* No tags */
		notag[p[1]&0177] = theflag ;
		continue ;
    	    default:
		PutString(" -->\n");
		PutString("\n<!-- **cgiprint: don't know -");
		PutString(p);
		PutString(" option -->\n") ;
		fprintf(stderr, "%s\n", usage);
    	    	exit(1);
	}
    }
    /* NO -- Use only the Basic Definition (in init) to set up JSversion
     * if (j = lookup(texdef_dic, "JSversion", -1))
	strncpy(j, JSversion, 8) ;
    */
    if (np) PutString(" -->\n");

    theMode = (theMode&(~MODE_ignore)) | prev_disable_output ;
    /* disable_output = prev_disable_output ;  */

    /* V8.8: Add the definitions */
    if (defs) load_services(defs->text, strlen(defs->text));

    /* Definitions for Targets used in Bibcodes and Catalog Designations */
    if (target) load_services(target->text, 0);
    if (pre) {
	push_tex(lookup(cgi_dic, "pre", 3),(char *)0);
	texenv--; PutChar('\n');
    }
    opre = pre ;
    /* When there is an output_file to start, do it NOW */
    if (output_filter && *output_filter) {
	fflush(stdout) ;
	filter_output(output_filter) ;
	output_filter = (char *)0 ;
    }
    if (olis) print_services(why);
    if (tofree) free(tofree); args = (char *)0; /* Added V9.44 */
    return(p);
}

static char *exec_cgi(opline)
/*++++++++++++++++
.PURPOSE  Set up the options. (triggered by \cgidef)
.RETURNS  A string "\377" (for end), or "" ;
.REMARKS  opline may incude the "keep_soft_mode"
-----------------*/
  char *opline; 	/* IN: Options string, e.g. "tex -cat" ...  */
{
  char *p, *e;
  int c, i, skipped_bytes ;
    if(verbop>1) {
	i = strloc(opline, '\n'); c = opline[i]; opline[i] = 0;
	PutString("<!-- #exec_cgi ") ; PutString(opline) ;
	if (c) PutString("(+definitions) "), opline[i] = c;
	PutString(" -->\n") ;
    }
    /* Remove the "keep mode" -- (only in SOFT case) */
    /* Isolate the 1st word  -- can be \33{\37m\33} */
    keep_soft_mode[3] = MODE_soft ;
    if (strncmp(opline, keep_soft_mode, sizeof(keep_soft_mode)-3) == 0)
	 skipped_bytes = sizeof(keep_soft_mode)-3 ;
    else skipped_bytes = 0 ;
    /* Remove leading blanks */
    for (opline += skipped_bytes; isspace(*opline); opline++) ;
    for (e=p=opline; isgraph(*e); e++) ;
    c = *e; *e = 0;
    p = lookup(cgi_dic, opline, strlen(opline));
    *e = c ;
    if (p) { 	/* There is a special environment to start */
	if (*p == '!')	{ 	/* Launch another program... */
	  char *argv[2] ;
	    argv[0] = p+1; argv[1] = 0 ;
	    fflush(stdout) ;
	    execv(argv[0], argv) ;
	    fprintf(stderr, "****Can't execv(%s)", argv[0]) ;
	    perror("") ;
	}
	if (*p == '\377') return(p) ; 	/* This indicates a STOP */
	if (*p != '.')
	    doconv_topush = strdup(p) ;	/* Will be executed */
        /* push_tex(p, (char *)0), texenv--; PutChar('\n'); */
        p=e; 	/* Prepare the continuation for options */
        if (c) p++;
	doconv_toset = strdup(p) ;	/* Will be executed */
	return("\33O") ;		/* ==> exec do_conv */
    }
    /* Options only -- no need to restart a do_conv */
    setopt(opline);
    /* if (ohtml && omail && echo1a[1]) return("\\echo1x{1}"); */
    return("") ;
}

static void execute(fct, ospec)
/*++++++++++++++++
.PURPOSE  Execute the Specified Function
.RETURNS  ---
-----------------*/
  PTR_FCT fct;	/* IN: Function to execute */
  int ospec ;	/* IN: Special function	*/
{
  char *p, *q, *parm[10] ; int np, i, tofree ;

    /* Resolving the arguments must not be done for rescan_arg */
    if (fct == rescan_arg) { rescan_arg() ; return ; }

    /* tofree contains a bit to indicate a newly allocated rescan'ed */
    for (np=tofree=0; next_char() == '\021'; np++, tofree <<= 1) {
    	get_char();
    	i = (get_char()&0xff) - '1' ;
	if (i < 0) break ;			/* NO ARGUMENT */
	p = texparm[i] ? texparm[i]->text : "" ;
	parm[np] = q = p ;
	/* NOTE: ospec requires NOT to rescan its argument... */
	if ((fct != exec_cgi) && (!ospec)) {
	    while (*q) {
		if (iscntrl(*q)) break ;
		if (*q == '\\') break ;
		if (*q == '$') break ;
		if (*q == '^') break ;
		q++ ;
	    }
	    if (*q) parm[np] = rescan(p, strlen(p)), tofree |= 1 ;
	}
    }
    for (i=np; i<10; i++) parm[i] = "" ;

    p = (*fct)(parm[0], parm[1], parm[2], parm[3], parm[4], parm[5],
        parm[6], parm[7], parm[8], parm[9]) ;
    if (p && *p) push_text(p, strlen(p)) ;

    /* Free the rescanned parameters	*/
    while(tofree) {
	if (tofree&1) free(parm[np]) ;
	np--;
	tofree >>= 1 ;
    }
}

static int do_conv(opt)
/*++++++++++++++++
.PURPOSE  Execute the Conversion from Opened File
.RETURNS  0 (OK) / 1 (Not terminated) / -1 (error)
-----------------*/
  int opt ;	/* IN: Combination of DO_* flags	*/
{
  int c, i, nl, byteno, bw;	/* byteno is position in line,
				   bw>0 if beginning of word */
  int dontag, omode;			/* is 1 if current line not to tag */
  char *tofree, *p;
  int saved_do_option ;
  TEX_ENV *old_textop ;
  int itag_saved, ignore_spaces, inHTML;
  char saved_options[sizeof(options)];
  char html_string[8] ;
  TABLE2 *saved_def[sizeof(c1def)] , *t2, **old_saved_t2def;

    /* --- In Secure Option, verify the file starts with the
       \cgidef{....}  marker
    */
    itag_saved = itag_stack ;
    ignore_spaces = 0;		/* Turned on by \ and EOL */
    inHTML = 0;
    saved_do_option = do_option ;
    do_option = opt|DO_NOCLOSE;	/* Called action do not need to close all
				   open tags */
    if ((opt&DO_CHECK) && options.sec) {
	next_char() ;	/* This loads the first line */
	if (strncmp(inbuf->text, "\\cgidef{", 8) != 0) /*}*/ {
	    putstring("\n****File can't be displayed (Sorry)\n") ;
	    do_option = saved_do_option ;
	    return(-1) ;
	}
    }

    /* Copy in PLAIN is asked for... */
    if (opt&DO_PLAIN) {
	while ((c = get_char()) != EOF) PutChar(c) ;
	do_option = saved_do_option ;
	return (0) ;
    }
    memcpy(saved_options, &options, sizeof(options));
    memcpy(saved_def, t2def, sizeof(t2def));
    old_saved_t2def = saved_t2def; saved_t2def = saved_def;

    tofree = (char *)0 ;

    /* Execute the doconv definitions (from version 7.4) */
    if (doconv_topush) {
	push_tex(doconv_topush, (char *)0) ;
	tofree = doconv_topush ;
	doconv_topush = (char *)0 ;
    }
    if (doconv_toset) {
	setopt(doconv_toset) ;
	free(doconv_toset) ;
	doconv_toset = (char *)0 ;
    }

    bw = byteno = 1;
    nl = -1 ;
    dontag = 0 ;

    if (opop) {
	push_text("\\cgidef{-tex}\n\\JavaPop\n\\cgidef{end}\n", -1) ;
	opop = 0 ;
    }

    /* Loop on Input Text */

    for (byteno = 1 ; byteno>0; byteno++)  switch(c = get_char()) {
	putChar:
	    /* Last Chance for the unused special parameters of
	       a new environment specified by begin_mode
	       This parameter is already in HTML, enclose
	       therefore the begin_mode in this mode.
	    */
	    if (begin_more && (c == '>')) {
		push_text3("\33{\7\2 ", begin_more, ">\33}") ;
		free(begin_more) ;
		begin_more = (char *)0 ;
		continue ;
	    }
	    PutChar(c) ;
	    nl = ignore_spaces = 0 ;
	    continue ;

	case EOF:
	    byteno = -1 ;	/* Indicates to STOP */
	    continue ;

	case '#':
	    if (byteno != 1) goto putChar ;
	    if (otex && (theMode&MODE_VERB)) goto putChar ;
	    p = match_comment(next_char() == 'c' ? 1 : (verbop&1)|otex);
	    byteno = 0 ;
	    if (!p) continue ;
	    if (strncmp(p, "cgi-text", 8)) {	/* V8.11 Write Out Comment */
		PutComment(p, "\n");
	    }
	    else { 	 	/* #cgi-text definitions */
		for (p += 8; isspace(*p); p++) ;
		p = exec_cgi(p);
		if (*p == '\377') byteno = -1;
	    }
	    continue;

	case '\3':		/* FONT	*/
	    omode = theMode&MODE_intag;
	    theMode |= MODE_intag;
	    PutString("<FONT ") ;
	    if (omode==0) theMode &= ~MODE_intag;
	    push_tag("\3");
	    continue ;

        case '\4':		/* Change itexparm    */
            reset_texparm() ;
            continue ;

        case '\6':		/* Dedicated Function */
            c = get_char() ;
            execute(fct_stack[c&0xff], 0) ;
            continue ;

        case '\16':		/* Dedicated Function */
            c = get_char() ;
            execute(fct_stack[c&0xff], 1) ;
            continue ;

	case '\30':		/* (AND)   Mode  */
	    c = get_char();
	    theMode &= (0xffffff00|(c&0xff));
	    continue;

	case '\37':		/* (OR)    Mode	 */
            c = get_char() ;
	    theMode |= (c & 0xff) ;
            continue ;

	case '\7':		/* Change Mode	 */
            c = get_char() ;
	    /* theMode = (theMode&(~0xff)) | (c & 0xff) ; */
	    mode4tex();		/* Clear (not all) flags */
	    theMode |= c & 0xff ;
            continue ;

	case '\27':		/* Change Mode	 */
            c = get_char() ;
	    theMode = (theMode&(~0xff)) | (c & 0xff);
            continue ;

	case '\17':		/* Change Mode	 */
            c = get_char() ;
	    setimode(c & 0xff) ;
            continue ;

        case '\10':		/* Verbatim next byte 	*/
            c = get_char() ;
	    if (strchr(bword, c)) bw = 1 ;
	    theMode |= MODE_verb1 ;
	    goto putChar ;

        case '\020':		/* Env Parameter */
            c = get_char(); i = c - '1' ;
            if (i < 0) continue ;	/* NOOP */
            if (ptexenv[i]) push_buf(ptexenv[i]) ;
            else PutChar('#'), PutChar(c),
		 PutString(" TeX(env) arg. undefined#") ;
            continue ;

        case '\021':		/* TeX Parameter */
            c = get_char(); i = c - '1' ;
            if (i < 0) continue ;	/* NOOP */
            if (texparm[i]) push_buf(texparm[i]) ;
	    else if_mode(MODE_soft) ;
	    else if(otex&2) ;	/* SOFT... */
            else PutChar('#'), PutChar(c),
		 PutString(" TeX argument undefined#") ;
            continue ;

	case '+':		/* Verify +/-	*/
	    if (!(otex&3/*1*/)) goto putChar;
	    if (!soft_tex) goto putChar;
#if 0	/* Pre-9.3 */
	    if (next_char() != '/') goto putChar;
	    i = c ;
	    c = get_char() ;
	    if (next_char() != '-') { /* +/- */
		PutChar(i) ;
		goto putChar ;
	    }
	    c = get_char() ;
	    PutString("&#177;") ;
            continue ;
#else
	    unget_char(c);
	    if (match_tex2(0)) continue;
	    get_char();
	    goto putChar;
#endif
	case '-': 		/* Just change -- into - */
	    if (!(otex&3/*1*/)) goto putChar;
	    if(opre | (theMode&(MODE_plain|MODE_HTML|MODE_PRE|MODE_VERB)))
		goto putChar;
	    /* -- => ndash, --- => mdash */
#if 0
	    if (next_char() == '-') {
		c = get_char();
		if (next_char() == '-') {
		     c = get_char();
		     PutString("&#8212;");
		}
		else PutString("&#8211;");
		continue;
	    }
#else
	    /* True, tex or soft-mode */
	    if (next_char() == '-') {
	        unget_char(c);
	        if (match_tex2(0)) continue;
	        get_char();
	    }
	    else if (theMode&(MODE_MATH|MODE_math)) { /* V9.3: Math mode
					                 ==> use large minus */
		PutString("&#8211;");
		continue;
	    }
	    goto putChar ;
#endif

	case '~':
	    if (!(otex&3/*1*/)) goto putChar;
	    if_mode(MODE_plain|MODE_HTML|MODE_VERB)
		goto putChar;
	    if (okeep&8) goto putChar;
	    if (soft_tex) {
		unget_char(c);
		if (match_tex2(0)) continue;
		get_char();
		goto putChar;
	    }
	    PutString("&#160;") ;
	    continue ;

	case '^': case '_':
	    if (!(otex&3/*1*/)) goto putChar;
	    if_mode(MODE_2html) ;	/* Added V9.44 */
	    else if_mode(MODE_VERB|MODE_plain|MODE_HTML) goto putChar;
	    if (soft_tex) {
		if (match_tex2(c)) continue ;
		else goto putChar ;
	    }
	    else if ((c == '^')&&(okeep&1)) goto putChar ;
	    else if ((c == '_')&&(okeep&2)) goto putChar ;
	    else if (!(theMode&(MODE_math|MODE_MATH))) goto putChar ;
	    p = (c == '^' ? "<SUP>" : "<SUB>");
	    PutString(p); push_tag(p) ;
	    p = get_texparm(0); i = strlen(p) ;
	    if (i == 1) PutChar(*p);
	    else do_text(p, i, do_option|DO_NOCLOSE) ;
	    pop_tag() ;
	    continue ;

	case '%':
	    if (byteno == 1) dontag = notag[next_char()] ;
	    if (dontag) goto putChar;
	    if (!otex) goto putChar;
	    if_mode(MODE_VERB|MODE_SOFT|MODE_HTML|MODE_intag|MODE_plain)
	       goto putChar;
	    if (okeep&16) goto putChar;
	    match_comment(0);
	    continue;

        case '$':			/* Can be Math TeX, or env_var */
	    if (!(otex&3 /*1*/)) {
		if (match_env()) continue ;
		goto putChar;
	    }
	    if (okeep&4) goto putChar;
	    if (theMode&MODE_HTML) {
	    	if (match_env()) continue ;
		goto putChar;
	    }
	    if_mode(MODE_VERB|MODE_SOFT) goto putChar;
	    if (theMode&MODE_math) {
	        pop_tex("") ;
		if (theMode&MODE_math) {
		    putstring(
		    "\n<!-- ++ $ (MathMode) not properly closed -->\n") ;
		}
		continue ;
	    }
	    if (next_char() == '$') {	/* DisplayMath Mode */
		get_char();
		if (theMode&MODE_MATH) {
		    pop_tex(""); pop_tex("");
		    if (theMode&MODE_MATH) putstring(
		     "\n<!-- ++ $$ (DisplayMathMode) not properly closed -->\n"
		    ) ;
		    continue;
		}
		push_tex("CENTER", (char *)0); push_tex("I", (char *)0) ;
		theMode |= MODE_MATH;
		theMode &= ~MODE_soft;
		continue;
	    }
	    /* if (soft_tex) goto putChar ; -- already done! */
	    push_tex( (onomath&1? "" : "I"), (char *)0) ;
	    theMode |= MODE_math ;
	    continue;

        case '{':
	    if (!(otex&3/*1*/)) goto putChar;
	    if_mode(MODE_VERB|MODE_HTML) goto putChar ;
	    if (soft_tex && (next_char() != '\\')) {	/* Fixed V9.24 */
		if (match_tex2(/*{*/'}')) continue ;
	    }
	    if (theMode&MODE_soft) {  /* Not in MODE_hiddensoft (V9.05) */
		if (next_char() != '\\') {	/* e.g. {\bf} */
		    texenv->ibrace += 1 ;
		    goto putChar;
		}
		push_tex((char *)0, (char *)0);
		theMode &= ~MODE_soft;
		theMode |= MODE_hiddensoft ;
		continue ;
	    }
            push_tex((char *)0, (char *)0);
	    continue;

	case '\33':		/* TeX return (esc)	*/
	    switch(next_char()) {
	    case 'b': case 'B':
	    case '{':	push_tex((char *)0, (char *)0); break ;
	    case 'e': case 'E':
		itexenv = texenv->ipenv ;	/* Terminate Group */
	    case '}':	pop_tex("") ;			break ;
	    case 'o': case 'O':			/* Open New cgidef */
		get_char() ;
		old_textop = textop ; textop = texenv ;
		do_conv(0) ;
		textop = old_textop ;
		continue ;
	    case 'c': case 'C':			/* Open New cgidef */
		byteno = -1 ;				break ;
	    detault:    continue ;
	    }
	    get_char() ;
	    continue ;

        case '}':
	    if (!(otex&3/*1*/)) goto putChar;
	    if_mode(MODE_VERB|MODE_HTML) goto putChar ;
	    i = theMode & (MODE_MATH|MODE_math); /** VERUFY MATH */
	    if (texenv->ibrace > 0) {
		texenv->ibrace -= 1;
		goto putChar;
	    }
	    /* if (theMode&MODE_soft) goto putChar; // ! hiddensoft */
	    pop_tex("");
	    if ((theMode & (MODE_MATH|MODE_math)) != i) {
		putstring(
		"\n<!-- ++ {The MathMode expands over  a } -->\n") ;
	    }
            continue;

	case '|':	/* Look for GLU */
	    if (theMode&MODE_Aglu) theMode &= ~(MODE_Aglu|MODE_HTML) ;
	    goto putChar;

    	case '&':
    	    if (ohtml && match_latin()) continue;
	    if (theMode&MODE_VERB) goto putChar ;
	    if (theMode&(MODE_HTML|MODE_Aglu|
		MODE_href|MODE_xArg/*MODE_Aarg|MODE_Aargq*/|MODE_2html))
		goto putChar ;
	    if (otex&3/*1*/) {
		Flush ;
		/* WARNING: Not a column separator if not plain table */
	    	if ((theMode&
		   (MODE_PRE|MODE_HTML|MODE_tab|MODE_plain|MODE_VERB|MODE_soft))
		    == MODE_tab) { table_cclose();  bw = 1; }
		else if (p = getequiv("&")) { push_text(p, -1); continue ; }
	    	else PutString("&amp;"); /* PutChar('\t'); */
	    	continue;
	    }
	  putAmp:
	    theMode |= MODE_verb1 ;
	    PutString("&amp;");
	    continue;

	case '?':	/* Look for the ? in href! */
	    if (i = (theMode&MODE_href)) {
		PutChar(c);
		if (i == MODE_href2) 	/* Was waiting for ? */
		    theMode += MODE_href1;
		continue;
	    }
	    goto case_default;

	case '\'':	/* Look for javascript arguments */
	    PutChar(c);
	    if (theMode&MODE_js)
		theMode ^= MODE_href1;
	    else bw = c ;
	    continue;

	case '"':
	    PutChar(c);
	    bw = c ;
	    continue;

	case '(':
	    PutChar(c);
	    bw = ')';
	    continue ;

	case '[':
	    PutChar(c);
	    bw = ']';
	    continue ;

	case '=': case ',': case ';': case ')': case ']':
	    if (soft_tex) {
		unget_char(c);
		if (match_tex2(0)) continue;
		get_char();
	    }
	    PutChar(c);
	    bw = 1 ;		/* A new word starts...*/
	    continue ;

    	case '<': 		/* -glu: Check first bib, cat ...	*/
	    if (theMode&MODE_HTML) {
		inHTML = match_html(inHTML);
		if (inHTML) { bw = 1 ; continue ; }
		goto putChar;
	    }
	    if (dontag) goto putChar;
	    if_mode(MODE_VERB|MODE_2html) goto putChar ;
	    if_mode((MODE_math|MODE_MATH)) goto putLT ;
	    if_mode(MODE_plain) goto putLT /* putChar */ ;
	    if (soft_tex) {
		unget_char(c);
		if (match_tex2(0)) continue;
		get_char();
	    }
	    if (oglu && match_glu('>')) continue;
	    if ((!oglu) && match_serv('>')) continue;
	    if (obib && match_bib('>')) continue;
	    if (ocat && match_cat('>')) continue;
	    if (omail && match_mail('>'))  continue;
	    if (oglu  && match_serv('>')) continue;
	    if (ohtml) {
		inHTML = match_html(inHTML);
		if (inHTML) { if (ohtml == 2) bw = 1 ; continue; }
	    }
	  putLT:
	    theMode |= MODE_verb1 ;
	    PutString("&lt;");
	    continue;

    	case '>':
	    bw = 1 ;
	    if_mode(MODE_HTML|MODE_VERB|MODE_2html) goto putChar ;
	    if (soft_tex) {
		unget_char(c);
		if (match_tex2(0)) continue;
		get_char();
	    }
	    theMode |= MODE_verb1 ;
	    PutString("&gt;");
	    continue;

        case '\n':		/* Look for several lines = paragraph 	*/
	    fflush(stdout);
            if (nl > 0) nl = 0;		/* First empty line not printed */
	    byteno = 0; bw = 1 ; dontag = 0 ;	/* A new word starts...	*/
	    if ((otex&3) && (theMode&MODE_obeylines) && (!soft_tex)) {
		push_text("\\\\\\nl", 5) ;
		continue ;
	    }
	    if (theMode&(MODE_PRE|MODE_HTML|MODE_plain|MODE_VERB
		      |MODE_intag|MODE_ignore)) goto putChar ;
            while (isspace(c)) {
            	if (nl >= 0) PutChar(c);
		if (c == '\n') { nl++ ; byteno = 0; }
		else if (c != '\r') byteno++ ;
            	c = get_char();
            }
            if (c != EOF) {
		if ((nl > 1) && (!(opre|ohtml))) PutString("<P>\n");
		unget_char(c);
	    }
	    nl = 0 ;
            continue;

        case '\\':		/* Look for TeX Translations */
            if (! otex) {
		if ((byteno == 1) && (starts_by("cgidef{"))) /*}*/ nl = -1 ;
		else { PutChar(c); continue; }
	    }
            if ((next_char() == '\\')  && (theMode & MODE_tab)) {
            	get_char();
            	table_rclose();
		c = next_char() ;
		if (c == '[')  {	/* Space... ignore	*/
		    while ((c != ']') && (c != '\n')) c = get_char();
		}
            	bw = 1;
            	continue ;
            }
	    if (next_char() == '\n') {
		/* ignore_spaces = 1;	// Ignore \ at end of line */
		/* PutChar(' ') ; --- Gobble blanks & tabs (V9.09) */
		get_char();
		while (1) {
		    c = next_char();
		    if ((c==' ')||(c=='\t')) get_char();
		    else break;
		}
		continue ;
	    }
            if (match_tex()) {
		continue;
	    }
            PutChar(c);
    	    continue;

	case 'f':
	    if (theMode&MODE_plain) goto putChar;
	    if (bw && (starts_by("tp://") || starts_by("ile:/"))) {
		unget_char(c);
		if (match_serv1(bw)) continue ;
		get_char() ;
	    }
	    goto case_default;

	case 'j':	/* Verify the javascript in href */
	    if ((theMode&MODE_href) == MODE_href1) {
		if (starts_by("avascript:")) theMode
		  = (theMode&(~MODE_href)) | MODE_js;
		PutChar(c);
		continue;
	    }
	    goto case_default;

	case 'h':
	    if (theMode&MODE_plain) goto putChar;
	    if (bw && (starts_by("ttp://")||starts_by("ttps://"))) {
		unget_char(c);
		if (match_serv1(bw)) continue ;
		get_char() ;
	    }
	    goto case_default;

	case 't':
	    if (theMode&MODE_plain) goto putChar;
	    if (bw && starts_by("elnet://")) {
		unget_char(c);
		if (match_serv1(bw)) continue ;
		get_char() ;
	    }
	    goto case_default;

	case_default:
    	default:
	    if ((theMode&MODE_href) == MODE_href1) {
		/* Just started a href='...', which is NOT javascript */
		theMode += MODE_href1;	/* Change MODE_href1 to MODE_href2 */
	    }
	    if (inHTML&2) {     /* V9.03: in HTML comment */
		inHTML = match_html(inHTML);
		if (inHTML) { if (ohtml == 2) bw = 1 ; continue; }
	    }
	    if (c&128) {	/* 8-bit ascii	*/
		if (okeep&128) goto putChar ;
		sprintf(html_string, "&#%d;", c&255) ;
		PutString(html_string) ;
		nl = ignore_spaces = 0 ;
		continue ;
	    }
	    if (isspace(c)) {
		if (ignore_spaces) continue ;
		bw = 1 ;
		goto putChar ;
	    }
	    if (dontag) goto putChar;
	    if (bw && ((theMode&(MODE_HTML|MODE_plain|MODE_VERB)) == 0)) {
		/* Beginning of a new word */
	    	unget_char(c);
	    	/* if (match_serv1(bw)) continue ; */
		if (obib && match_bib(0)) continue;
	    	if (ocat && match_cat(0)) continue;
	    	if (omail && match_mail(0))  continue;
	    	get_char() ;
	    }
    	    PutChar(c);
	    bw = 0 ;
	    nl = 0;		/* Added V8.83 */
	    /* fflush(_of); */
    	    continue;
    }

    if (tofree) free(tofree) ;

    /* Terminate all non-closed environments	*/
    if (opt&DO_NOCLOSE) {
        do_option = saved_do_option ;
	saved_t2def = old_saved_t2def;
        return( c != EOF );
    }

    while (texenv > textop) { INT_FCT ofct ; ofct = OutFct;
	if (OutFct != null_out) {
	    putstring("\n<!-- ++Closing tex environment[");
	    OutFct = com_out ;
	    putstring(i2a(texenv-tex_stack)) ;
	    putstring("] '"); putstring(texenv->env);
	    OutFct = ofct ;
	    putstring("' -->\n") ;
	}
        pop_tex("");
    }
    while (itag_stack > itag_saved) pop_tag();
    if (byteno) PutChar('\n');
    memcpy(&options, saved_options, sizeof(options));
    /* Remove the loaded definitions */
    for (i=0; i<ITEMS(saved_def); i++) {
	while (saved_def[i] != t2def[i]) {
	    t2 = t2def[i];
	    t2def[i] = t2->prev;
	    t2kill(t2);
	}
    }
    /* Remove the -%.. definitions */
    for (i=0; i < sizeof(notag); i++) notag[i] &= ~1 ;
    i = next_char() != EOF ;
    do_option = saved_do_option ;
    saved_t2def = old_saved_t2def;
    return( i );
}

static int do_text(text, len, do_option)
/*++++++++++++++++
.PURPOSE  Execute only the specified text
.RETURNS  0 (OK) / 1 (Not terminated) / -1 (error)
-----------------*/
  char *text;		/* IN: Text to convert	*/
  int len ;		/* IN: Length of text	*/
  int do_option;	/* IN: Argument for do_conv */
{
  char old_pending; BUF *b, *old_inbuf; int r ;
  TEX_ENV *old_textop ;

    b = buf_alloc(len+1);
    if (!b) return(-1);

    strncpy(b->text, text, len);
    b->used = len ;
    b->text[len] = 0;
    old_inbuf = inbuf;  inbuf = b;
    old_pending = pending ; pending = 0 ;
    old_textop  = textop ; textop = texenv ;
    /* The conversion must only take the text, no file input ! */
    do_option &= ~DO_CHECK ;
    r = do_conv(do_option|DO_NOGET) ;
    textop = old_textop ;
    pending = old_pending ;
    inbuf = old_inbuf ;
    b->size = 0; free(b) ;
    return(r) ;
}

static char *exec_input(/* filename -- changed in v8.23 */)
/*++++++++++++++++
.PURPOSE  Switch to a new file (triggered by \input)
.RETURNS  ""
.REMARKS  From v8.23, the file name is saved in the parameter
-----------------*/
{
  static char missing_file[120];
  BUF *old_inbuf; FILE *old_infile ; FILE *f;
  char *old_fn, *tmp ; TEX_ENV *old_textop ;
  char *filename, opt;
  int   old_otex;
  /* int saved_do_option; */

    /* v8.23: get the file name from the input directly */
    opt =  inbuf->pos < inbuf->used ? get_char() : 0;
    if (itexparm>0) {
        filename = texparm[--itexparm]->text;
    }
    else filename = "-";
    push_tex((char *)0, (char *)0);

    /* Be sure the substitution arguments were processed! */ 	/* v7.6 */
    if (strchr(filename, '\021')) filename = tmp = rescan21(filename, -1) ;
    else tmp = (char *)0 ;
    if (verbop) fprintf(_of, "\n<!-- \\input{%s} -->", filename) ;
    f = fzopen(filename);
    if (!f) {
	perror(filename) ;
        fprintf(stderr, "\n#---cwd=%s\n", getcwd((char *)0, 128));
	PutString("\n<!-- **Error Opening File: ");
	PutString(filename) ;
	PutString(" -->\n") ;
	if (tmp) free(tmp) ;
	pop_tex("");
	strcpy(missing_file, "[missing file: ");
	strncpy(missing_file+15, filename, sizeof(missing_file)-17);
	missing_file[sizeof(missing_file)-2] = 0;
	strcat(missing_file, "]");
	return(missing_file);
    }

    old_fn = thefile ;    thefile = strdup(filename);
    old_infile = infile ; infile = f ;
    old_inbuf = inbuf;    inbuf = (BUF *)0 ;
    old_textop = textop ; textop = texenv ;

    /* Install the environment -- verbatim, html... */
    setimode(opt);
    old_otex = otex;
    /* \inputhtml ==> forget the TeX mode */
    if (opt == 'H') otex = 0;

    /* Obviously, I MUST read the input file... */
    /* saved_do_option = do_option; */
    /* do_option &= ~DO_NOGET; */
    /* do_option |= DO_NOCLOSE; -- NO \tableofcontents need this! */
    while (do_conv(do_option & (~DO_NOGET)) == 1) ;
    /* saved_do_option = do_option; */

    otex = old_otex;
    fclose(infile) ;
    textop = old_textop ;
    if (inbuf) inbuf->size = 0, free(inbuf) ;
    inbuf = old_inbuf ;
    infile = old_infile;
    free(thefile); thefile = old_fn ;
    if (tmp) free(tmp) ;
    pop_tex("");
    return("") ;
}

static char *rescan(text, len)
/*++++++++++++++++
.PURPOSE  Rescan a TeX text,
.RETURNS  Its equivalence in a NEW PIECE OF MEMORY
	--- WARNING: Collector Buffer not reset !!
-----------------*/
  char *text;			/* IN: Text to convert	*/
  int len ;			/* IN: Length of text	*/
{
  INT_FCT old_routine, old_out ;
  int old_used, start;
    /* Be sure a NUL terminates preceding string */
    old_used = collector ? collector->used : 0 ;
    collector = buf_append(collector, "", 1) ;
    start = collector->used ;
    old_routine = OutFct ; OutFct = buf_out ;
    old_out = theOutputRoutine ; theOutputRoutine = (INT_FCT)0 ;
    do_text(text, len, do_option|DO_NOCLOSE) ;
    theOutputRoutine = old_out; OutFct = old_routine ;
    collector->used = old_used ;
    return(strdup(collector->text + start)) ;
}

static char *rescan21(text, len) 	/* v7.6 */
/*++++++++++++++++
.PURPOSE  Rescan -- only replace the '\21' parameters
.RETURNS  Its equivalence in a NEW PIECE OF MEMORY
.REMARKS  Remove also the keep_soft_mode (v8.56)
-----------------*/
  char *text;			/* IN: Text to convert	*/
  int len ;			/* IN: Length of text	*/
{
  int i, plen; char *p, *t, *e ; BUF *b ;
    if (len < 0) len = strlen(text) ;
    b = (BUF *)0 ;
    for (t=text, e=text+len; t<e; t++) {
	if (*t != '\021') { b = buf_append(b, t, 1); continue ; }
	i = ((*++t)&0xff) - '1' ;
	if (i < 0) break ;
	p = texparm[i] ? texparm[i]->text : "" ;
	/* Remove also the keep_soft_mode, if present (v8.56) */
	i = sizeof(keep_soft_mode)-3;
	plen = strlen(p);
        keep_soft_mode[3] = MODE_soft ;
        if ((plen>i) && (strncmp(p, keep_soft_mode, i) == 0)) {
	     p += i;
	     plen -= sizeof(keep_soft_mode)-1;
	}
	b = buf_append(b, p, plen);
	if (strloc(p, '\021')<plen) {	/* Yet Other Substitutions! */
	    p = rescan21(b->text, -1) ;
	    b->used = 0;		/* Replace the current text */
	    b = buf_append(b, p, -1) ;
	    free(p) ;
	}
    }
    if (b) {
	p = strdup(b->text) ;
	free(b) ;
    }
    else p = strdup("") ;
    return(p) ;
}

/*==================================================================
		Initialisations
 *==================================================================*/
static void init()
/*++++++++++++++++
.PURPOSE  Just initalize the various pieces
.RETURNS  ---
-----------------*/
{
  static int done = 0 ;
  char **a, *p ;
    if (done) return ;

    if (!theprogram) theprogram = "cgiprint" ;
    /* Look which HTML version: it HTTP_USER_AGENT not in list of
       HTML3Agents, taken number */
    if ((!vhtml) && (p = getenv("HTTP_USER_AGENT"))) {
	for (a=HTML3Agents; *a && (!vhtml); a++) {
	    if (strncmp(*a, p, strlen(*a)) == 0) vhtml = "3.0" ;
	}
	if (p && (!vhtml)) {
	    p += strloc(p, '/');
	    if (*p) { 			/* Keep only 1st word */
		++p ;
		vhtml = strdup(p) ;
		for (p = vhtml; isgraph(*p); p++) ;
		*p = 0 ;
	    }
	    else keep_slash = 0 ;
    	}
    }
    if (!vhtml) vhtml = "3.0" ;
    if ((!vMozilla) && (p = getenv("HTTP_USER_AGENT"))) {
	if (strncmp(p, "Mozilla/", 8) == 0) {
	    if (isdigit(p[8])) {
		/* MSIE behaviour for JS is similar to
		   Mozilla 5
		*/
	        vMozilla = strdup(p+8);
		if ((*vMozilla == '4') && strstr(p, "MSIE"))
		    *vMozilla = '5';
	        for (p=vMozilla; isgraph(*p); p++) ;
	        *p = 0;
	    }
	}
    }
    if (!vMozilla) vMozilla = "5.0" ;
    emptycell = *vhtml >= 3 ? "&nbsp;" : "<BR>" ;
    done = 1 ;
    load_services(default_defs, T2_SORTED); 	/* Install the Service */
    if (p = lookup(texdef_dic, "vMozilla", -1))
	strncpy(p, vMozilla, 8) ;

    /* Install the Unicode symbols */
    if (unicode_symb[0] && (*vMozilla >= '5')) { int c;
	c = T2_amp2;			/* Default numeric symbols  */
#if 0
	if (ispunct(*unicode) && (unicode[1] == 's'))	/* Symbolic */
	    c = T2_amp1;
#endif
	load_services(unicode_symb, c|T2_SORTED);
	unicode_symb[0] = 0;				/* Indicates loaded */
    }

    /* Count the standard Functions */
    for (ifct_stack=0; fct_stack[ifct_stack]; ifct_stack++) ;
    nfct_stack = ifct_stack ;
}

/*==================================================================
		Terminate in case of too Long
 *==================================================================*/

static void prstring(FILE *fm, char *text)
/*++++++++++++++++
.PURPOSE  Write out the text
.RETURNS  ---
-----------------*/
{
  char *p;
    for (p=text; *p; p++) {
	if (isprint(*p)) fputc(*p, fm);
	else fprintf(fm, "\\%03o", (*p)&255);
    }
}

static char *signame(int signo)
/*++++++++++++++++
.PURPOSE  Find the name of a signal
.RETURNS  The name
.REMARKS
-----------------*/
{
  static char asig[8];
    switch(signo) {
      case SIGHUP : return("SIGHUP");	/* Hangup */
      case SIGINT : return("SIGINT");	/* Interrput */
      case SIGILL : return("SIGILL");	/* Illegal Instruction */
      case SIGURG : return("SIGURG");	/* Urgent (signal from socket) */
      case SIGFPE : return("SIGFPE");	/* Floating-Point Exception */
      case SIGIO  : return("SIGIO");	/* Lost file locking */
      case SIGPIPE: return("SIGPIPE");	/* PIPE error */
      case SIGUSR1: return("SIGUSR1");	/* User Signal (for CleaCache) */
      case SIGUSR2: return("SIGUSR2");	/* User Signal (for Report) */
      case SIGSEGV: return("SIGSEGV");	/* Segmentation Violation */
      case SIGABRT: return("SIGABRT");	/* Abort Signal */
      case SIGTERM: return("SIGTERM");	/* Termination signal */
      case SIGALRM: return("SIGALRM");	/* Alarm */
      case SIGCHLD: return("SIGCHLD");	/* Child died */
      case SIGQUIT: return("SIGQUIT");	/* Quit */
      case SIGCONT: return("SIGCONT");	/* Quit */
      default:
	sprintf(asig, "sig#%02d", signo);
	return(asig);
    }
}

static void tooLong(int sig)
/*++++++++++++++++
.PURPOSE  TERMINATE in case of kill -1 : Log the arguments into a file
.RETURNS  ---
.REMARKS  If a f2check is given, verify the modification date of
	  this file has not changed -- otherwise we increase the delay...
	A zero signal just counts the dates, bytes processed, etc.
-----------------*/
{
static BUF *bmsg;
#if 0
static char intro[] = "\
Subject: alarm (tooLong) in cgiprint\n\
\n\
****cgiprint stopped by alarm #%d at +%ds\n\
	     \\resetAlarm=%d and %ld bytes processed ***\n\
" ;
#else
static char intro[] = "#***cgiprint alarm stop#%d (%s) at +%ds,\
 \\resetAlarm=%d and %ld bytes processed ***\n";
#endif
static char fmcheck[] = "\
Control file: %s\n\
  File Times: from %10d to %10d (maxtime=%d)\n\
       Bytes: from %10d to %10d\n\
";
static int counter = 0;
static int next_chance = 0;	/* Set to 1 to give a last chance... */
static time_t now, previous_ftime;
static long previous_input_bytes;
  struct stat bstat;
  char buf[256];
  TEX_ENV *pt=0;
  FILE *fm;
  BUF *b;
  char *p;
  int st, i;
    now = time((time_t *)0);
    if (verbop) fprintf(stderr, "++++tooLong after %ds\n",
	    (int)(now-start_time));
    if (f2check) {
        st = stat(f2check, &bstat);
	if (st<0) {
	    /* Ignore the Error ---
	    fprintf(stderr, "++++tooLong check Control file ");
	    perror(f2check);
	    */
	    bstat.st_mtime = previous_ftime;
	    if (!bstat.st_mtime) bstat.st_mtime = start_time;
	    sprintf(buf, "%+06ds file=%s, age=[**]",
		    (int)(now-start_time), f2check);
	}
	else sprintf(buf, "%+06ds file=%s, age=%04d",
	    (int)(now-start_time),
	    f2check, (int)(bstat.st_mtime-start_time));
	sprintf(buf+strlen(buf), " bytes=%ld\n", input_bytes);
	bmsg = buf_append(bmsg, buf, -1);
    }
    if (sig == 0) {
        previous_input_bytes = input_bytes;
	previous_ftime = bstat.st_mtime;
	return;
    }
    counter++;
    if (f2check && (input_bytes > previous_input_bytes)) {
	/* If the file 2 check was not modified, we have to STOP */
        if (verbop) fprintf(stderr,
	  "++++tooLong: f2check=%s, input_bytes=%ld\n", f2check, input_bytes);
	if (bstat.st_mtime > previous_ftime) next_chance = 2;
	else if (next_chance == 1) next_chance = -1;
	else next_chance = 1;
	if (next_chance > 0) {
	    previous_ftime = bstat.st_mtime;
	    previous_input_bytes = input_bytes;
	    signal(SIGALRM, tooLong);
	    alarm(max_time);
	    return;
	}
    }
#if 0
    sprintf(buf, "%s %s", MAIL, EMAIL);
    fm = popen(buf, "w");
    if (!fm) exit(1);
    fprintf(fm, intro, counter, now-start_time,
	reset_alarm_ticks, input_bytes);
    if (f2check) fprintf(fm, fmcheck, f2check,
	previous_ftime, bstat.st_mtime, max_time,
	previous_input_bytes, input_bytes);
    if (bmsg) fprintf(fm, "%s", bmsg->text);
    fprintf(fm, "\n----Current state:\n");
    if (p = getequiv("\\tooLongText")) fprintf(fm, "%s\ntooLongText=%s%s\n",
       arule, p, arule);
    fprintf(fm, "%4d  mode=%08X, ibrace=%d, itag=%d, iparm=%d, ipenv=%d, env=",
	(texenv-tex_stack), texenv->mode, texenv->ibrace,
	texenv->itag, texenv->iparm, texenv->ipenv);
    if (pt) prstring(fm, pt->env); fputc('\n', fm);

    i = inbuf->pos-8; if (i<0) i=0;
    fprintf(fm, "Current input: "); prstring(fm, inbuf->text + i);
    fputc('\n', fm);

    /* List the texenv stack */
    fprintf(fm, "\nTEX_ENV stack:\n");
    for (pt=texenv, i=0; (pt>=tex_stack) && (i<100); pt--) {
	++i;
	fprintf(fm,
	  "%4d: mode=%08X, ibrace=%d, itag=%d, iparm=%d, ipenv=%d, env=",
	  i, pt->mode, pt->ibrace, pt->itag, pt->iparm, pt->ipenv);
	prstring(fm, pt->env); fputc('\n', fm);
    }

    /* List the Input stack */
    fprintf(fm, "\nInput stack:\n");
    for (b=inbuf, i=0; b && (i<100); b=b->prev) {
	++i;
	fprintf(fm, "......@%08p[%d]+%04d/%04d: ",
	  b, b->flags, b->pos, b->used);
	prstring(fm, b->text); fputc('\n', fm);
    }
    if (b) fprintf(fm, "++++++and more......\n");

    /* List the Arguments */
    fprintf(fm, "\nArguments stack (octal):\n");
    for (i=0; i < itexparm; i++) {
        b = texparm[i];
	fprintf(fm, "%5o @%08p[%d]+%04d/%04d: ",
	  i, b, b->flags, b->pos, b->used);
	prstring(fm, b->text);
    }
    pclose(fm);
#else
    fprintf(stderr, intro, counter, signame(sig), now-start_time,
	     reset_alarm_ticks, input_bytes);
    fprintf(stderr,
	"#State=%-2d mode=%08X, ibrace=%d, itag=%d, iparm=%d, ipenv=%d\n",
	(int)(texenv-tex_stack), texenv->mode, texenv->ibrace, texenv->itag,
	texenv->iparm, texenv->ipenv);
    fprintf(stderr,
	"#   Current input buffer: used=%d, pos=%d:\n",
	inbuf->used, inbuf->pos);
    i = inbuf->pos>60 ? inbuf->pos-60: 0;
    inbuf->text[inbuf->pos] = 0;
    prstring(stderr, inbuf->text + i);
    fprintf(stderr, "\n#---cwd=%s\n", getcwd((char *)0, 128));
    for (i=0; i < itexparm; i++) {
	b = texparm[i];
	fprintf(stderr, "#...Stack#%-2d: %s\n", i, b->text);
    }
#endif
    sleep(1);
    exit(1);
}


#ifdef FUNCTION
/*==================================================================
		Function cgitr
 *==================================================================*/
static BUF *tmp_buf ;

char *cgiver(html_version)
/*++++++++++++++++
.PURPOSE  Set the HTML version (default is 3.0)
.RETURNS  The previous version
-----------------*/
  char *html_version ;	/* IN: e.g. "2" or "3" */
{
  char *v ;
    v = vhtml ;
    if (!html_version) html_version = "" ;
    if (*html_version) vhtml = vMozilla = html_version;	/* v8.73 */
    return(v) ;
}

char *cgidef(opt_string)
/*++++++++++++++++
.PURPOSE  Set up the options.
.RETURNS  NULL if OK, or file to execute
-----------------*/
  char *opt_string; 	/* IN: Options string (default is arg) */
{
    return(setopt(opt_string)) ;
}

static int tmp_collect(str, len)
/*++++++++++++++++
.PURPOSE  Output routine for cgitr
.RETURNS  len
-----------------*/
  char *str; 	/* String to append */
  int len ;	/* Length of string */
{
    if (!tmp_buf) tmp_buf = buf_alloc(8192) ;
    tmp_buf = buf_append(tmp_buf, str, len) ;
    return(len) ;
}

char *cgitr(text)
/*++++++++++++++++
.PURPOSE  The routine which executes the cgiprint
.RETURNS  The translated string
.REMARKS  Default is tex2
-----------------*/
  char *text ;	/* IN: Text to interpret */
{
  static char *null_ptr = (char *)0 ;
  static char init_done = 0 ;
  BUF *old_inbuf; INT_FCT old_routine ;
  int itexparm0 ;
  char *b ;

    /* Initialisation */
    if (!_of) _of = stdout ;
    aargs = aaenv = &null_ptr ; nargs = 0 ;

    /* Save current environment and initialize */
    old_routine = OutFct ; OutFct = tmp_collect ;
    old_inbuf   = inbuf ;
    itexparm0 = itexparm ;

    init() ;
    push_text(text, strlen(text)) ;
    if (tmp_buf) tmp_buf->used = 0 ;

    /* Execute the Conversion */
    while (do_conv(do_option|DO_NOGET) == 1) ;

    /* Free all tex parameters */
    if (inbuf) inbuf->size = 0, free(inbuf) ;
    for (itexparm=ITEMS(texparm); --itexparm > itexparm0; ) {
	if (!texparm[itexparm]) continue;
	free(texparm[itexparm]);
	texparm[itexparm] = (BUF *)0;
    }

    /* Reset the current environment */
    OutFct = old_routine ;
    inbuf = old_inbuf ;
    if (tmp_buf && tmp_buf->used) b = tmp_buf->text ;
    else b = "" ;
    return(strdup(b)) ;
}

#ifdef TEST
main()
{
/* Example of definitions */
static char options[] = "\
-tex -bib -mail -ic -glu\n\
\\cst	 #1{\\tag{INPUT NAME=\"#1\" SIZE=\"16\"}}\n\
\\hidden #2{\\tag{INPUT TYPE=\"hidden\" NAME=\"#1\" VALUE=\"#2\">}\n\
\\Push   #2{\\tag{INPUT TYPE=\"checkbox\" NAME=\"#1\" VALUE=\"#2\" CHECKED}}\n\
\\push   #2{\\tag{INPUT TYPE=\"checkbox\" NAME=\"#1\" VALUE=\"#2\"}}\n\
\\radio  #2{\\tag{INPUT TYPE=\"radio\" NAME=\"#1\" VALUE=\"#2\"}}\n\
\\Radio  #2{\\tag{INPUT TYPE=\"radio\" NAME=\"#1\" VALUE=\"#2\" CHECKED}}\n\
\\Ahelp  #2{\\W{explain}{/cgi-bin/Help?VizieR/#1}{#2}}\n\
\\Acat   #2{\\W{explain}{/cgi-bin/Cat?#1}{#2}}\n\
\\catal  #2{ & {\\bf #1} & \\Acat{#1}{#2}}\n\
\\table  #2{{\\times} & #1 & #2}\n\
\\Table  #2{\\push{-source}{#1}& #1 & #2}\n\
\\Unit   #1{\\glutag{Unit #1}}\n\
\\Page   #1{\\CDSpage{#1}{VizieR.logo}}\n\
" ;
  char buf[BUFSIZ];
  char *input, *result ;

    printf("----Passed to cgidef: %s", options) ;
    cgidef(options) ;
    while(1) {
	printf("Give a text, or <name of a file to process: ");
    	if (!gets(buf)) break ;
	if (buf[0] == '<') {
	    input = load_file(buf+1, 0) ;
	    if (input) result = cgitr(input) ;
	    else result = strdup("****Error****") ;
	}
	else { strcat(buf, "\n") ; result = cgitr(buf) ; }
	printf("\n\n====Input:\n%s\n====Result:\n%s", buf, result) ;
	free(result) ;
    }
}
#endif

#else
/*==================================================================
		Main Routine
 *==================================================================*/

int main(argc, argv, envp)  int argc; char **argv, **envp;
{
static char msg_pb[] = "\n\
<P><font size='-1'><i>Thank you for reporting the problem to \
<A href='mailto:%s?Subject=%%5bmissing@%s%%5d%%20%s'>%s</A></i><font><br>\n";
  char *p, *HTTP_HOME, httpd_buf[128], *host, *query_arg;
  char *qargs = (char *)0 ;
  int plain = 0 ;
  int status ;
  int i ;

    /* Initialisation */
    _of = stdout ;
    host = getenv("SERVER_NAME"); if (!host) host = "server";
    /* Save the arguments and Options... */
    aargs = argv +1 ; nargs = argc -1 ; aaenv = envp;
    for (p = *argv + strlen(*argv);  --p >= *argv; ) {
        if (*p == '/') break ;
    }
    theprogram = ++p ;
    /* if (strcmp(theprogram+strlen(theprogram)-6, "-plain") == 0)
	printf("Content-Type: text/plain\n\n") ;
    */

    /*----- Examine the Program. It started from HTTPD,
      ----- we have to check its name, move to the Pages subdirectory
      ----- if it exists, and set the Arel variable from the
      SCRIPT_NAME environment variable.
    */
    HTTP_HOME = getenv("DOCUMENT_ROOT") ;
    if (!HTTP_HOME) HTTP_HOME = "(undefined)" ;
    if (strncmp(theprogram, "Show", 4) == 0) {
	verbop &= (~1) ;
	for (p = theprogram + 4; *p == '-'; p += strloc(p, '-')) switch(*++p) {
	  case 's': options.sec = DO_CHECK ; continue ;
	  case 'p': plain |= DO_PLAIN ; continue ;
	}
#if DEBUG
    	fprintf(_of, "Content-Type: text/plain\n\n") ;
    	fprintf(_of, "====Arg#0 '%s' from home=%s\n",
	    *argv, getcwd((char *)0, 128)) ;
	fprintf(_of, "I'm     in %s\n", getcwd((char *)0, 128)) ;
	fprintf(_of, "DOCUMENT_ROOT=%s\n", HTTP_HOME) ;
#endif
	p = *HTTP_HOME == '/' ? HTTP_HOME : "/usr/httpd" ;
	if (chdir(p) < 0) fprintf(stderr,
		"****Can't chdir(%s) (HTTP_HOME=%s)\n", p, HTTP_HOME) ;
	httpd_home = getcwd(httpd_buf, sizeof(httpd_buf));
#if DEBUG
	fprintf(_of, "I'm now in %s\n", httpd_home) ;
#endif
	if (p = getenv("SCRIPT_NAME")) { char *abin;	/* Mod. V8.75 */
    	    theprogram = p ;
	    if (*p == '/') p++ ;
	    if ((abin = strstr(p, "-bin/"))) {	/* e.g. /local/cgi-bin/Show */
		Arel = strdup(p) ;
		while ((abin>p) && (*abin != '/')) --abin;
		if (abin>p) 			/* e.g. "local" */
		    Arel[(abin-p)] = 0;
		else {				/* No need to chdir */
		    free(Arel);
		    Arel = (char *)0;
		}
		if (chdir(Arel) < 0) fprintf(stderr,
		    "++++Can't chdir(%s) from %s\n", Arel, httpd_home);
#if DEBUG
		fprintf(_of, "Execute chdir(%s)\n", Arel) ;
	        fprintf(_of, "I'm now in %s\n", getcwd((char *)0, 128)) ;
#endif
	    }
	}
	httpd_home = getcwd(httpd_buf, sizeof(httpd_buf));

	if (chdir(SUBDIR) < 0) fprintf(stderr, "++++Can't chdir(%s) from %s\n",
	    SUBDIR, httpd_home) ;
	httpd_home = getcwd(httpd_buf, sizeof(httpd_buf));
#if DEBUG
	fprintf(_of, "Execute chdir(%s)\n", SUBDIR) ;
	fprintf(_of, "I'm now in %s\n", getcwd((char *)0, 128)) ;
#endif
	if (p = getenv("QUERY_STRING")) {	/* args from QUERY_STRING */
	    qargs = strdup(p) ; nargs = 0 ; strhtml(qargs) ;
	    nargs = 0 ;
	    fprintf(_of, "Content-Type: text/%s\n\n", plain ? "plain" : "html");
	}
    }

#ifdef TRACE_MEMORY
    osmtrace(1);
#endif
    /* Be sure the -v option is taken into account */
    max_time = MAX_TIME;
    p = *aargs;
    if ((nargs>0) && p && (p[0] == '-') && (p[1] == 'v') && isdigit(p[2])) {
	vhtml = vMozilla = p+2;
	max_time *= 10;
	++aargs; --nargs;
    }
    init() ;

    /* Be sure the program doesn't loo forever */
    signal(SIGALRM, tooLong) ;
    start_time = time((time_t *)0);
    alarm(max_time);

    plain |= options.sec ;	/* This is option of do_conv:
	     it combines the -sec and the -plain options.... */
    query_arg = qargs ? strdup(qargs) : qargs; 	/* V9.47 */
    while (p = setopt(qargs)) {
	/*fprintf(stderr, "#...qargs=%s; p=%s;\n", qargs, p);*/
	if (!*p) p = query_arg;  /* V9.47 */
	thefile = p; qargs = (char *)0 ;
    	if (verbop>1) PutString("<!-- Input File: "),
    	    PutString(p), PutString(" -->\n");
    	infile = fzopen(p);
    	/*if (!infile) { fflush(_of); perror(p) ; wait_pipe(0); exit(1); } */
        if (!infile) { int st = errno; char *af;
            perror(p) ;
            fprintf(_of,
                "<FONT size='+1' color='red'>Page not accessible</FONT> (");
            PutString(strerror(st));
            fprintf(_of, "): ");
            while ((af=strchr(p, '/'))) { /* Keep file name as .../.../... */
                if (!strchr(af+1, '/')) break;
                p = af+1;
            }
            PutString(p);
            af = getenv("SERVER_ADMIN");
            if (!af) af = "cds-question@unistra.fr";
            fprintf(_of, msg_pb, af, host, p, af);
            fflush(_of);
            wait_pipe(0);
            exit(1);
        }
	if (chdir(dirname(p))!=0) { fprintf(stderr, "#***"); perror(p); }
    	while (do_conv(plain) == 1) ;
	if (httpd_home) {
	    if (chdir(httpd_home)!=0) {
		fprintf(stderr, "#***");
		perror(httpd_home);
	    }
	}

	fclose(infile);
#if 0	/* Can't revert the output pipe... */
    	if (outFilter > 0) {
	    if(fileno(_of) > 2) fclose(_of), _of = stdout ;
	    wait(&status);
	    outFilter = 0 ;
        }
#endif
    }

    if (!infile) {	/* Nothing done. Operate on stdin */
    	infile = stdin;
	PutString("<!-- Input on stdin (cgiprint-");
	PutString(VERSION);
	PutString(") -->\n");

    	while (do_conv(plain) == 1) ;
    }

#if 0
    /* Free all tex parameters */
    for (i=ITEMS(texparm); --i >= 0; ) {
	if (!texparm[i]) continue;
	free(texparm[i]);
	texparm[i] = (BUF *)0;
    }
#endif

    if (errors_fi > 10) fprintf(stderr,
	"++++Total number of errors: %d\n", errors_fi);

    if (verbop) {
	for (i=status=0; i<ITEMS(children); i++) { if (children[i]) status++ ; }
	if (status) {
	    fprintf(_of, "<!-- ....wait for %d children:", status) ;
	    for (i=0; i<ITEMS(children); i++) {
		if (children[i]) fprintf(_of, " %%%d=%d", i, children[i]) ;
	    }
	    fprintf(_of, " -->\n") ;
        }
    }

    /* Close the output pipe, if any */
    fflush(_of) ;
    if(fileno(_of) > 2) fclose(_of), _of = stdout ;

    wait_pipe(0);
    exit(0);
}
#endif

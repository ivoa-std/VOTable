/*++++++++++++++
.IDENTIFICATION cgigraph.c
.LANGUAGE       C
.AUTHOR         Francois Ochsenbein [CDS]
.ENVIRONMENT    
.KEYWORDS       
.VERSION  1.0   04-Jul-2001
.VERSION  1.1   03-Jan-2003: Added -l (log axis)
.VERSION  1.2   26-Apr-2003: Several parameters in a table
		  name=value are understood this way only
		  when no blank exists in name.
.VERSION  1.21  03-Oct-2004: don't take list of several params like in cat#6113
.VERSION  1.22  02-Jan-2005: -nolog option
.VERSION  1.23  21-Jan-2006: -cap:... text (caption centered below the image)
			or -cap (caption in standard input)
.VERSION  1.24  24-Feb-2007: accept SELECT values as
			parameter  e.g. P=value
			transmitted as     @P="val1;val2;..."
			in environment LIST_P="val1;val2;..."
.VERSION  1.25  14-Aug-2007: Bug corrected in bitmap-size interpretation
.VERSION  1.26  11-Nov-2007: Accept a fraction of the option.
.VERSION  1.27  31-Jan-2009: Pb bitmap or bitmap-size
.VERSION  1.28  25-Apr-2009: Accept more than 2 arguments
.VERSION  1.29  22-Aug-2009: Problem in J/A+A/501/687 with Menu having =
.VERSION  1.30  11-Sep-2009: Adjust size of box (boxlen)
.VERSION  1.31  10-Dec-2009: Accept BITMAPSIZE
.VERSION  1.32  12-Dec-2010: Pb in case exact match
.VERSION  1.4   28-Jun-2011: Options -left -lf -right -rf
.VERSION  1.41  17-Nov-2011: Accept $theImg (means image is ready)
.VERSION  1.42  21-Nov-2011: Adapt size of boxes
.COMMENTS       Deal with GRAPH arguments:
---------------*/

#define VERSION "1.42"
/* #define DEBUG */
#include <string.h>	/* Standard string manipulations */
#include <stdlib.h>	
#include <unistd.h>	
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif
#define ITEMS(a)        sizeof(a)/sizeof(a[0])

/* Definition of interesting ENV variables */
static char *envar[] = {
    "REQUEST_METHOD",
    "PATH_INFO",
    "QUERY_STRING",
    "HTTP_PLOT",
    "CONTENT_LENGTH",
    "CONTENT_TYPE",     /* For FILES */
    "SCRIPT_NAME",
    "theCat",           /* Current catalog */
    "theTab",           /* Current table   */
    "thePro",           /* Current exec.   */
    "theImg",           /* If image ready  (V1.41) */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
    (char *)0,		/* Other variables */
} ;
static char *enval[ITEMS(envar)];

#define REQUEST_METHOD  0
#define PATH_INFO       1
#define QUERY_STRING    2
#define HTTP_PLOT 	3
#define CONTENT_LENGTH  4
#define CONTENT_TYPE    5
#define SCRIPT_NAME     6
#define theCat          7
#define theTab          8
#define thePro          9
#define theImg         10

/* Options -- 0 is print */
#define Bshell		1
#define Cshell		2
#define Perl		3
#define Exec		4
#define Html		5
#define Htx		6
static char ostdin ;		/* -- option 	*/
static char osh, ov;		/* Bool Options	*/
static char *sep = "\n";	/* Delimiter 	*/

/* Expandable buffer */
typedef struct { int allocated, used ; char *buf ; } BUF ;
static BUF *bgraph ;	/* The -graph= or -graph buffer */
static BUF *barg0  ;	/* All other arguments */
static BUF *bargs  ;	/* All other arguments */
/* Basic GRAPH arguments */
static int logxy = 0;	/* 0 1(x) 2(y) 3(both) 
			   0x10 0x20 0x30 = Don't show these */
static char *ax[10];
static char *ay[10];
static char *bitmap ;	
static char *ops ;
static char *otxt ;
static char *ocap ;
static char *oimg ;
static char *oform ;

static char *at_left[20], *at_right[20];

static char help[] = "\
Usage: cgigraph [-ps|-img|-txt|-form] [[-ps|-img|-txt|-form|-cap]:text]...\n\
       [-form] [-nolog] [-left 'text'|-lf file|-right 'text'|-rf file]...\n\
       {-print|-csh|-sh|-pl|-html|-htx}\n\
       [args...] [-graph graph_args...|--]\n\
 Interprets the parameters to generate a GRAPH execution + Adapt Form\n\
 The  -ps:text ... arguments are additional formats to get the plot (links)\n\
 The -cap:text ... argument gives a caption (centered below the image);\n\
     -cap- assumes the caption is in the standard input (must be in HTML)\n\
    -form: write a FORM (at the bottom) where the parameters can be changed\n\
   -print: write to standard output the GRAPH parameters, one per line\n\
    -html: issue the FORM to redefine the GRAPH parameters\n\
	   (issue also the link to PS, TXT, and the IMG if argument present)\n\
     -htx: issue the FORM to redefine the GRAPH parameters (for htx)\n\
     -csh: issue a command to define the GRAPH argv array in C-shell\n\
      -pl: issue a command to define the GRAPH ARGV array in Perl\n\
      -sh: issue a command to define the GRAPH argv array in Bourne-shell\n\
           (e.g. via eval `cgigraph -sh`)\n\
    -l...: Addition columns to the left of the image (text or file)\n\
    -r...: Addition columns to the right of the image (text or file)\n\
     args: other arguments required for the image generation.\n\
	   Arguments in the form 'name=value' generate additional form boxes;\n\
	   SELECT menus are created with LIST_name=item1;item2;... or shorter\n\
	   @name=item1;item2;... ; default item is defined with name=choice\n\
   -graph: GRAPH arguments, written either in the GRAPH way (e.g. -x 0 1) or\n\
	   in the FORM way (.e.g -x0=0 -x1=1).When both ways are used for the\n\
	   same parameter, the FORM way is the winner.\n\
       --: all arguments are given in standard input\n\
 Environment variables used: \n\
   SCRIPT_NAME = action to call (in the form) for re-plotting\n\
     HTTP_PLOT = program to generate the image: SRC=$HTTP_PLOT/$PATH_INFO\n\
     PATH_INFO = type of image (appended to HTTP_PLOT)\n\
 theCat/theTab = catalog and table used (transmitted to the plot program)\n\
 Defaults are  SCRIPT_NAME=/viz-bin/vizExec\n\
		 HTTP_PLOT=/viz-bin/nph-Plot\n\
		 PATH_INFO=/gif\n\
" ;

/*==================================================================
		String utilities
 *==================================================================*/

static int xval(c)
/*++++++++++++++++
.PURPOSE  Get the meaning of a char
.RETURNS  corresponding value
-----------------*/
  int c;	/* IN: Hexadigit to interpret */
{
    if (isxdigit(c)) {
	if (isdigit(c)) return (c - '0');
	if (isupper(c)) return (c - ('A' - 10));
	return (c - ('a' - 10));
    }
    return(0);
}

static char *i2a(val)
/*++++++++++++++++
.PURPOSE  Get the edition of a number
.RETURNS  corresponding value
-----------------*/
  int val;	/* IN: Value to edit */
{
  static char buf[16] ;
    sprintf(buf, "%d", val) ;
    return(buf) ;
}


int strloc(text, c)
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

static char *basename(name)
/*++++++++++++++++
.PURPOSE  Locate the filename (remove directory definitions)
.RETURNS  The last part
-----------------*/
  char *name;     /* IN: String to interpret */
{
  static char ok[128] ;	
  char *s; int i ;
    /* Initialize the list of available chars */
    if (!ok['a']) {
	for (i='0'; i<= '9'; i++) ok[i] = i ;
	for (i='A'; i<= 'Z'; i++) ok[i] = i ;
	for (i='a'; i<= 'z'; i++) ok[i] = i ;
	ok['.'] = '.' ;
	ok['-'] = '-' ;
	ok['_'] = '_' ;
    }
    for (s=name+strlen(name)-1; (s>=name) && isascii(*s) && ok[*s]; s--) ;
    return(++s) ;
}

int strbloc(text, c)
/*++++++++++++++++
.PURPOSE  Locate specified character backward
.RETURNS  Index of located char / -1 if not found
-----------------*/
  char *text;     /* IN: String to interpret */
  int  c;         /* IN: Char to look for */
{
  char *s;

    for (s=text+strlen(text); --s >= text; )  if (*s == c)    break;
    return(s-text);
}

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
	    fprintf(stderr, "#***Can't load file '%s': ", 
		    name ? name : "(nil)");
	    perror("");
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

/*==================================================================
		Local environment
 *==================================================================*/
static int env_add(char *name, char *value)
/*++++++++++++++++
.PURPOSE  Add name/value in environment
.RETURNS  Index in envar / -1
-----------------*/
{
  int i;
    for (i=theImg; (i<ITEMS(envar)) && envar[i]; i++) ;
    if (i >= ITEMS(envar)) return(-1);
    envar[i] = strdup(name);
    enval[i] = value;
    return(i);
}

static char *env_get(char *name)
/*++++++++++++++++
.PURPOSE  Retrieve in envar
.RETURNS  Value / NULL
-----------------*/
{
  int i;
    for (i=0; (i<ITEMS(envar)) && envar[i] && strcmp(name, envar[i]); i++) ;
    if ((i>=ITEMS(envar)) || (!envar[i]))
	return((char *)0);
    return(enval[i]);
}

/*==================================================================
		Expandable buffers
 *==================================================================*/

static BUF *buf_alloc(bytes)
/*++++++++++++++++
.PURPOSE  Allocate a buffer
.RETURNS  ---
-----------------*/
  int bytes ;	/* IN: Size to allocate	*/
{
  BUF *b ;
    b = (BUF *)malloc(sizeof(BUF)) ;
    b->allocated = b->used = 0 ;
    b->buf = (char *)0 ;
    if (bytes > 1) {
	b->buf = malloc(bytes) ;
	b->allocated = bytes ;
    }
    return(b);
}

static void buf_append(b, text)
/*++++++++++++++++
.PURPOSE  Append text to an expandable buffer
.RETURNS  ---
-----------------*/
  BUF *b ;	/* IN: Buffer 		*/
  char *text ;	/* IN: Text to append 	*/
{
  int len ;
    if (!b) return ;	/* Append to NULL ==> just ignore... */
    len = strlen(text);
    if ((b->used + len) >= b->allocated) {	/* Multiple of 512 */
	b->allocated = 1 + ((b->used + len + 1) | 511) ;
	b->buf = realloc(b->buf, b->allocated) ;
    }
    memcpy(b->buf + b->used, text, len+1) ;
    b->used += len ;
}

static void buf_append1(b, c)
/*++++++++++++++++
.PURPOSE  Append 1 byte to expandable buffer
.RETURNS  ---
-----------------*/
  BUF *b ;	/* IN: Buffer	*/
  int c ;	/* IN: Byte 	*/
{
  static char buf[2] ;
    if (!b) return ;	/* Append to NULL ==> just ignore... */
    if ((b->used +1) < b->allocated) {
	b->buf[b->used] = c ;
	b->used += 1 ;
	b->buf[b->used] = 0 ;
	return ;
    }
    buf[0] = c ;
    buf[1] = 0 ;
    buf_append(b, buf) ;
}

/*==================================================================
		Interpret the Graph Arguments
 *==================================================================*/

static int get3(text, a)
/*++++++++++++++++
.PURPOSE  Interpret up to 3 arguments
.RETURNS  Number of bytes scanned
.REMARKS  a = array
-----------------*/
  char *text;	/* IN: String to scan */
  char **a ; 	/* OUT: 3 values      */
{
  char *p ;
  int i;
    p = text; i = 0 ;
    a[0] = a[1] = a[2] = (char *)0 ;
    while(*p) {
	if (*p == '\n') { p++; continue ; }
        if ((p[0] == '-') && (isalpha(p[1]) || (p[1] == '-')) )
	    return(p-text) ;	/* Another argument starts */
	a[i++] = p ;
	p += strloc(p, '\n') ;
	if (*p) *(p++) = 0 ;
    }
    while (*p == '\n') p++ ;
    return(p-text) ;
}

static int set_graph(text, b)
/*++++++++++++++++
.PURPOSE  Interpret GRAPH arguments: either -x -y --bitmap  , or append to b
.RETURNS  Number of arguments appended to b
.REMARKS  The arrays ax ay bitmap are set only when void.
	When b is void, no argument append -- just ignored
-----------------*/
  char *text;	/* MOD: String to scan */
  BUF *b;	/* OUT: Non-xy arguments */
{
  char *p, *a3[3], **a, x ;
  int np = 0;
  int i ;

    /* Extract the basic GRAPH arguments: cuts, bitmapsize */
    if (!text) return(np) ;
    p = text ;
    while (*p) {

	if (*p == '\n') {
	    *(p++) = 0 ;
	    continue ;
	}

	if (*p == '-') switch(p[1]) {

	  case '-':
	    if (strncmp(p, "--bitmap", 8) == 0) {
		p++;
		goto case_bitmap;
	    }
	    break;

	  case 'b':
          case_bitmap:
	    if (strncmp(p, "-bitmap", 7) == 0) {
		i = 7;
		if (strncmp(p+i, "-size", 5) == 0) i += 5;
		if ((p[i] == '\n') || (p[i] == '=')) {
		    p += i+1;
		    /*if (bitmap) free(bitmap); */
		    bitmap = strdup(p);
		    p += strloc(p, '\n');
		    bitmap[strloc(bitmap, '\n')] = 0;
		    continue;
		}
	    }
#if 0
	    if (strncmp(p, "--bitmap-size=", 14) == 0) {	/* Mod. V1.25 */
	        p += 14 ; 
	        if (!bitmap) bitmap = p ;
		p += strloc(p, '\n') ;
	        continue ;
	    }
	    if (strncmp(p, "--bitmap=", 9) == 0) {
	        p += 9; 
	        bitmap = p ;
		p += strloc(p, '\n') ;
	        continue ;
	    }
#endif
	    break ;
	  
	  case 'l':	/* Log scale */
	    if ((p[2] == '\n') || (p[2] == '=')) {
		if (tolower(p[3]) == 'x') logxy |= 1;
		if (tolower(p[3]) == 'y') logxy |= 2;
		p += 3;
		p += strloc(p, '\n');
		continue;
	    }
	    break;

	  case 'x': case 'y':
	    a = p[1] == 'x' ? ax : ay ;
	    if (p[2] == '\n') { 
	        p += 3 ;
		p += get3(p, a3) ;
		if (!a[0]) a[0] = a3[0] ;
		if (!a[1]) a[1] = a3[1] ;
		if (!a[2]) a[2] = a3[2] ;
		continue ;
	    }
	    if (isdigit(p[2]) && (p[3] == '=')) {
		i = p[2]-'0' ;
		a[i] = p+4 ;
		p += strloc(p, '\n') ;
		if (!a[i][0]) a[i] = "-" ;
		continue ;
	    }
	}
	/* Just append the argument */
	i = strloc(p, '\n') ;
	x = p[i]; p[i] = 0 ;
	if (b) {
	    buf_append(b, p) ;
	    buf_append1(b, '\n') ;
	    np++ ;
	}
	if (x) i++ ;
	p += i ;
    }

    return(np) ;
}


/*==================================================================
		Write the Output
 *==================================================================*/

static char *str_arg(arg)
/*++++++++++++++++
.PURPOSE  Convert arguments
.RETURNS  The converted string
-----------------*/
  char *arg;	/* IN: Argument to transform */
{
  static char accepted_symbols[] = "-=.,:_&?~"; 
  static BUF buf ;
  char a4[6], *p ;
    buf.used = 0 ;
    for (p=arg; *p; p++) {
      if (isalnum(*p)) buf_append1(&buf, *p) ;
      else if (isascii(*p) && strchr(accepted_symbols, *p)) 
	   buf_append1(&buf, *p) ;
      else sprintf(a4, "%%%02x", (*p)&0xff), buf_append(&buf, a4) ;
    }
    return(buf.buf) ;
}

static void print_attr(name, value)
/*++++++++++++++++
.PURPOSE  Write a name="attr"
.RETURNS  ---
-----------------*/
  char *name;	/* IN: Name */
  char *value; 	/* IN: Corresponding value */
{
  char *p ;
    if (name) printf("%s=", name) ;
    putchar('"') ;
    for (p=value; *p; p++) {
        if (*p == '\n') putchar('\t') ;
	else if (*p == '"') printf("&quot;") ;
	else if (*p == '<') printf("&lt;") ;
	else if (*p == '>') printf("&gt;") ;
	else if (*p == '&') printf("&amp;") ;
	else putchar(*p) ;
    }
    putchar('"') ;
}

static void print_hidden(name, value)
/*++++++++++++++++
.PURPOSE  Write a HIDDEN text
.RETURNS  ---
-----------------*/
  char *name;	/* IN: Name */
  char *value; 	/* IN: Corresponding value */
{
    printf(" <INPUT TYPE=\"hidden\" name=\"%s\" ", name) ;
    if (value) print_attr("value", value) ;
    printf(">\n") ;
}

static void print_select(char *name, char *list, char *value)
/*++++++++++++++++
.PURPOSE  Write a SELECT text
.RETURNS  ---
.REMARKS  list is semicolon-separated, or blank-separated
-----------------*/
{
  int sep, i, x, nopt=0;
  char *p, *v;

    /* Find out the separator */
    sep = ';'; 
    if (!strchr(list, sep)) sep = ' ';

    /* V1.26: Value selected is the longest matching string,
     * saved in pointer v */
    p = list ; v = (char *)0;
    while (p) {
	if (*p == ';') p++;
	for (i=0; value[i] && p[i] && (value[i] == p[i]); i++) ;
	if (!isgraph(value[i])) {	/* Added V1.32 */
	    if ((p[i] == ';') || (!p[i])) i++;
	}
	if (i > nopt)	/* Best match */
	    nopt=i, v=p;
	p = strchr(p, ';');
    }

    nopt = 0;
    while (*list) {
	i = strloc(list, sep);
	if (i == 0) { list++; continue; }	/* Empty option */
	if (nopt == 0) 
    	    printf("<SELECT name=\"%s\">", name);
	nopt++;
	printf("<OPTION");
	if (list == v) printf(" SELECTED");
	/* for (p=list, v=value; (*p == *v) && *p; p++, v++) ;
	if ((*v == 0) && ((*p == 0) || (*p == sep)))
	    printf(" SELECTED");
	    */
	x = list[i]; list[i] = 0;
	printf(">%s", list);
	list[i] = x;
	list += i;
    }
    if (nopt) printf("</SELECT>");
}

static void write_form(argv, np, logxy, ax, ay, bitmap)
/*++++++++++++++++
.PURPOSE  Write the FORM
.RETURNS  ---
.REMARKS  Adds the columns at_left at_right
-----------------*/
  char **argv;	/* IN:  Array of GRAPH arguments */
  int np ;	/* IN: Number of GRAPH arguments */
  int logxy;	/* IN: Use log scale for plot */
  char **ax, **ay ;
  char *bitmap;	/* IN: Image size */
{
  BUF *arg0, *arg1 ;
  static char elist[80] = "LIST_";
  char **a, *p, *al, x, *e, *pname, *pval;
  int i, j, len0, len1 ;
  int in_graph, col_2, is_hidden, is_param, nparm=0, boxlen=0;
  int lists = 0;	/* Number of lists added */
  int ncols = 3;	/* V1.4 */

    if (!enval[SCRIPT_NAME]) enval[SCRIPT_NAME] = "/viz-bin/vizExec" ;
    if (!enval[HTTP_PLOT])   enval[HTTP_PLOT]   = "/viz-bin/nph-Plot" ;

    /* The Catalog + Table */
    arg1 = buf_alloc(0) ;
    if (p = enval[theCat]) {
        buf_append(arg1, p) ;
	if (p = enval[theTab]) buf_append(arg1, "/./"), buf_append(arg1, p);
    }
    else buf_append(arg1, ".") ;	/* Default Catalogue */
    len1 = arg1->used ;
    /* Add all arguments --- EXECPT lists! */
    for (a=argv; *a; a++) {
	p = *a;
	if (*p == '@') continue;
	if (strncmp(p, "LIST_", 5) == 0) continue;
	buf_append1(arg1, '&');
	buf_append(arg1, p) ;
    }

    /* The script to call */
    arg0 = buf_alloc(0) ;
    buf_append(arg0, enval[HTTP_PLOT]) ;
    if (p = enval[PATH_INFO]) {
	if (*p && (*p != '/')) buf_append1(arg0, '/') ;
	buf_append(arg0, p) ;
    }
    len0 = arg0->used ;

    /* The Plot first */
    for (i=0; (i<ITEMS(at_left)) && at_left[i];  i++) ncols++;
    for (i=0; (i<ITEMS(at_left)) && at_right[i]; i++) ncols++;
    printf("<TABLE WIDTH=\"%s\" BORDER=0><TR><TD>\n", "100%");
    printf("<TABLE BORDER=0 CELLPADDING=5 CELLSPACING=0>\n");
    if (oimg) {
	/* Complete the SRC address */
	buf_append(arg0, "/gif?") ;
	buf_append(arg0, str_arg(arg1->buf)) ;
	printf("<TR>");
	for (i=0; (i<ITEMS(at_left)) && at_left[i];  i++) {
	    if (osh == Htx) printf("%s", "<TD>\\end{HTML}%\n") ;
	    printf("%s", at_left[i]);
	    if (osh == Htx) printf("%s", "\\begin{HTML}</TD>\n") ;
	}
	printf("<TD ALIGN='center' NOWRAP  COLSPAN=%d><IMG %s ", 3, oimg);
	/* V1.41: use image if ready */
	print_attr("SRC", enval[theImg] ? enval[theImg] : arg0->buf) ;
	printf("></TD>") ;
	for (i=0; (i<ITEMS(at_right)) && at_right[i];  i++) {
	    if (osh == Htx) printf("%s", "<TD>\\end{HTML}%\n") ;
	    printf("%s", at_right[i]);
	    if (osh == Htx) printf("%s", "\\begin{HTML}</TD>\n") ;
	}
	printf("</TR>\n") ;
    }
    if (ops || otxt || ocap) {
	printf("<TR>\n") ;
	for (i=0; (i<ITEMS(at_left)) && at_left[i];  i++)
	    printf("<TD></TD>");
	printf(" <TD ALIGN=LEFT NOWRAP>") ;
	if (ops) {
	    arg0->used = len0; 
	    buf_append(arg0, "/ps?") ;
	    buf_append(arg0, str_arg(arg1->buf)) ;
	    printf("&nbsp;&nbsp;<A ") ;
	    print_attr("HREF", arg0->buf) ;
	    printf(">%s</A>", ops) ;
	}
	printf("</TD>\n") ;
	printf(" <TD ALIGN='center'>") ;
	if (ocap) {
	    printf("%s", ocap);
	}
	printf("</TD>\n") ;
	printf(" <TD ALIGN=RIGHT NOWRAP>") ;
	if (otxt) {
	    arg0->used = len0; 
	    buf_append(arg0, "/txt?") ;
	    buf_append(arg0, str_arg(arg1->buf)) ;
	    printf("<A ") ;
	    print_attr("HREF", arg0->buf) ;
	    printf(">%s</A>&nbsp;&nbsp;", otxt) ;
	}
	printf("</TD>");
	for (i=0; (i<ITEMS(at_right)) && at_right[i];  i++)
	    printf("<TD></TD>");
	printf("</TR>");
    }
    /*printf("\n</TR></TABLE>\n") ;*/
    printf("\n</TABLE></TD></TR>\n") ;

    /* In the FORM, another script to call */
    arg0->used = 0 ;
    buf_append(arg0, enval[SCRIPT_NAME]) ;
    if (p = enval[PATH_INFO]) {
	if (*p && (*p != '/')) buf_append1(arg0, '/') ;
	buf_append(arg0, p) ;
    }
    len0 = arg0->used ;

    if (oform) {
        printf(" <TR><TD><FORM METHOD=GET ");
        print_attr("ACTION", arg0->buf) ;
        printf(">\n") ;
        arg1->used = len1 ; arg1->buf[len1] = 0 ;
        printf(" <INPUT TYPE=\"hidden\" name=\"cat\" value=\"%s\">\n", 
	  arg1->buf) ;

	/* Count how many parameters (nparm) can be modified:
	 * if >2, the "Adapt Plot" text is removed 
	 Also, estimate size of boxes for params */
	for (i=0; i<np; i++) {
	    p = argv[i];
	    if (!isalpha(*p)) continue;
	    j = strloc(p, '=');
	    if (strloc(p, ' ')<=j) continue;	/* no value */
	    /* Estimate size of box */
	    nparm++;
	    if (strncmp(p, "LIST_", 5) == 0) { int lpv=0;
		/* A list is made of a list of item separated by a
		 * semi-colon (;). Compute in "lpv" the longest
		 * of these items. */
		nparm--;   /* Just the list, not true parameter */
		p += j+1;
		for (j=0; *p; p += lpv) {
		    lpv = strloc(p, ';');
		    if (lpv>j) j=lpv;
		    if (p[lpv]) lpv++;
		}
	    }
	    else j = strlen(p+j+1);	/* Length if text following '=' */
	    if (j>boxlen) boxlen=j;
	}
	boxlen = (3*boxlen)/4;
	if (boxlen>12) boxlen=12;
	if (boxlen<6)  boxlen=6;

        printf("<TABLE BORDER=3 CELLPADDING=5>\n");
        /*printf("<TABLE BORDER=3 CELLPADDING=5 WIDTH=\"%s\">\n", "100%") ;*/
        printf("<TR>");
	if (nparm<=2) printf(
		"<TD ALIGN='center' NOWRAP>%s</TD>", "<B>Adapt<BR>Plot</B>") ;

        /* graph: Replace the \n by a TAB */
	in_graph = 0 ; col_2 = 0 ;
	/* ==== Arguments written as name=value are 
	  shown in the Form.
	  in_graph is set while the parameters are set.
	  col_2    is a counter of elements in column#2
	*/
        while (np > 0) {
	    /* There is a parameter to show when there is an = in the first
	       word of the argument, and no other = sign  (V1.21)
	       The parameter name MUST start by alphabetic
	    */
	    is_param = 0;
	    pname = p = *argv; 			/* Name  of parameter	*/
	    i = strloc(p, '=');
	    pval = p+i;				/* Value of parameter	*/
	    if (p[i] && (strloc(p, ' ') > i)) {	/* Can be param=value 	*/
		pname[i] = 0;
		++pval;
	        if ((*pname == '@') && *pval) {	/* @name <==> LIST_name	*/
		    strncpy(elist+5, p+1, sizeof(elist)-6);
		    pname = elist;
		    is_param = 1;
		}
		else if (isalpha(*p)) {		/* Yes, can be a aparam */
		    is_param = 1;
		}
		else				/* No, not a parameter	*/
		    pname[i] = '=';
	    }

	    if (is_param) {			/* parameter=value	*/
	    			/* There is a parameter to show */
		if (in_graph) printf("\">");
		in_graph = 0 ;	/* Close the HIDDEN field */
		if (strncmp(pname, "LIST_", 5) == 0) { /* SELECT list..	*/
		    env_add(pname, pval);
		    is_hidden = 1;
		    lists += 1;
		}
		else if (strchr(pval, '=')) {   /* Need to hide param
			IF contains = BUT is not corresponding to a list... */
		     strncpy(elist+5, pname, sizeof(elist)-6);
		     is_hidden = env_get(elist) ? 0 : 1;
		}
		else is_hidden = 0;

		if (((col_2&1) == 0) && (!is_hidden)) {	/* 1sr param.	*/
		    if (col_2) printf(" </TABLE></TD>");
		    printf("\n <TD><TABLE CELLPADDING=5>\n");
		}
		if (is_hidden) printf(	/* V1.21: hidden only */
		   "\n  <INPUT TYPE=\"hidden\" name=\"%s\" value=\"%s\">", 
		     pname, pval);
		else {
		    col_2++ ;
		    printf("  <TR><TD ALIGN=\"RIGHT\">%s</TD><TD>", pname);
		    /* V1.24: Accept a menu (SELECT) */
		    strncpy(elist+5, pname, sizeof(elist)-6);
		    if ((e = env_get(elist))) { /* A menu (LIST) found 	*/
			print_select(pname, e, pval);
			lists -= 1;		/* List done */
			*e = 0;			/* Blank out the list	*/
		    }
		    else {
		        printf("<INPUT TYPE=\"text\" SIZE=%d NAME=\"%s\" ", 
		          boxlen, pname);
		        print_attr("VALUE", pval);
			putchar('>');
		    }
		    printf("</TD></TR>\n") ;
		}
	    }
	    else {					/* Hidden param */
		if (!in_graph) in_graph = 1, printf(
		  " <INPUT TYPE=\"hidden\" name=\"-graph\" value=\"") ;
	        printf("%s", p);
		if (np > 1) putchar('\t') ;
	    }
	    argv++ ; np-- ;
        }
        if (in_graph) printf("\">\n") ;

	/* Add the SELECTs if not yet done */
	if (lists) {
	    if ((col_2&1) == 0) {    /* A new column */
		if (col_2) printf(" </TABLE></TD>");
	        printf("\n <TD><TABLE CELLPADDING=5>\n  ");
	    }
	    col_2++ ;
	    for (i=7; i<ITEMS(envar); i++) {
		if (!envar[i]) continue;
		if (strncmp(envar[i], "LIST_", 5) != 0) continue;
		printf("  <TR><TD ALIGN=\"RIGHT\">%s</TD><TD>", envar[i]+5);
		print_select(pname+5, enval[i], "");
	    }
	}
	if (col_2) printf("</TABLE></TD>\n ") ;

        printf("<TD ALIGN=LEFT NOWRAP>\n") ;
        printf(" <TABLE BORDER=0 CELLSPACING=0 CELLPADDING=6 WIDTH=\"%s\">\n",
           "100%") ;
        printf("  <TR><TD ALIGN=RIGHT>%s</TD>\n", "x cuts:") ;
        for (i=0; i<2; i++) {
            printf("   <TD><INPUT TYPE=\"text\" SIZE=%d NAME=\"-x%d\" ", 4, i);
	    if (ax[i]) print_attr("VALUE", ax[i]) ;
	    printf("></TD>\n") ;
        }
	if (ax[2]) print_hidden("-x2", ax[2]);
	/* The x log scale check box */
        printf("   <TD ALIGN=RIGHT>");
	if ((logxy&0x11) == 0x10) ;	/* NO LOG */
	else {
            printf("   %s", "xlog");
            printf(" <INPUT TYPE=\"checkbox\" NAME=\"-l\" VALUE=\"%s\"", "x");
	    if (logxy&1) printf(" CHECKED");
	    printf(">");
	}
	printf("</TD>\n");
	/* Plot size */
        printf("   <TD ALIGN=right>Bitmap size:\n") ;
        printf("    <INPUT TYPE=\"text\" SIZE=%d NAME=\"%s\" VALUE=\"%s\">\n", 
            boxlen, "--bitmap", bitmap ? bitmap : "600x600") ;
        printf("   </TD></TR>\n") ;
        printf("  <TR><TD ALIGN=RIGHT>%s</TD>\n", "y cuts:") ;
        for (i=0; i<2; i++) {
            printf("   <TD><INPUT TYPE=\"text\" SIZE=%d NAME=\"-y%d\" ", 4, i);
	    if (ay[i]) printf("VALUE=\"%s\"", ay[i]) ;
	    printf("></TD>\n") ;
        }
        if (ay[2]) print_hidden("-y2", ay[2]) ;
	/* The y log scale check box */
        printf("   <TD ALIGN=RIGHT>");
	if ((logxy&0x22) == 0x20) ;	/* NO LOG */
	else {
            printf("   %s", "ylog");
            printf(" <INPUT TYPE=\"checkbox\" NAME=\"-l\" VALUE=\"%s\"", "y");
	    if (logxy&2) printf(" CHECKED");
	    printf(">");
	}
	printf("</TD>\n");
	/* The SUBMIT Button */
        printf("   <TD ALIGN=right>") ;
        printf("<INPUT TYPE=\"submit\" VALUE=\" %s \">\n", "Adapt the plot") ;
        printf("   </TD></TR>\n </TABLE></TD>\n</TR>\n</TABLE>\n</FORM>\n") ;
    }
    printf("</TD></TR></TABLE>\n");

    free(arg0->buf) ; free(arg0) ;
    free(arg1->buf) ; free(arg1) ;
}

static void print_shell(text)
/*++++++++++++++++
.PURPOSE  Write the definitions of arguments (depending on the shell)
.RETURNS  ---
-----------------*/
  char *text;	/* IN: Filled buffer to write to log file */
{
  char *p, **a, **argv ;
  int i, np ;

    if (!text) text = "" ;
    /* Count the number of arguments */
    for (np=1, p=text; *p; p++) { if (*p == '\n') np++; }

#ifdef DEBUG
    fprintf(stderr, "....print_shell osh=%d, logxy=%d, text:\\\nn%s\n\n",
    osh, logxy, text);
#endif

    /* Keep all arguments in a new array.
      The np parameter represents the number of parameters
      before the "cuts" -- i.e. the parameters beyond np
      will be presented in the "Adapt Plot" form.
    */
    argv = a = (char **)malloc((np+16)*sizeof(char *)) ;
    *(a++) = "graph" ;
    p = text ;
    np = 0;
    while(*p) {
        if (*p == '\n') { *(p++) = 0; continue; }
        *(a++) = p;
	np++ ;
        p += strloc(p, '\n'); 
    }
    if (logxy) {
	if (logxy&1) *(a++) = "-l", *(a++) = "x";
	if (logxy&2) *(a++) = "-l", *(a++) = "y";
    }
    if (ax[0]) {
        *(a++) = "-x"; 
        *(a++) = ax[0]; 
        if (ax[1]) *(a++) = ax[1] ;
        if (ax[2]) *(a++) = ax[2] ;
    }
    if (ay[0]) {
        *(a++) = "-y"; 
        *(a++) = ay[0]; 
        if (ay[1]) *(a++) = ay[1] ;
        if (ay[2]) *(a++) = ay[2] ;
    }
    if (bitmap) {
        *(a++) = "--bitmap-size" ;
        *(a++) = bitmap ;
    }
    *a = (char *)0 ;
#ifdef DEBUG
    for (a = argv; *a; a++) fprintf(stderr, "....arg=>%s\n", *a);
    fprintf(stderr, "\n");
#endif

    if (ov || (osh == 0)) {
	if (ov) fprintf(stderr, "----Resulting arguments: graph\n") ;
	for (a = argv+1; *a; a++) {
	    printf("%s", *a) ;
	    if (a[1]) printf("%s", sep) ;
	}
	if (*sep == '\n') printf("%s", sep) ;
    }
    if (osh == 0) return ;

    /* Launch the new program */
    if (osh == Exec) {
        execvp(*argv, argv) ;
	fprintf(stderr, "****Executing ") ;
	perror(*argv) ;
	exit(127) ;
    }

    /* Write a FORM */
    if (osh >= Html) {
        if (osh == Htx) printf("\\begin{HTML}\n") ;
	write_form(argv+1, np, logxy, ax, ay, bitmap) ;
        if (osh == Htx) printf("\\end{HTML}\n") ;
	return ;
    }

    /* Initialisation */
    if (osh == Bshell) printf("set");
    if (osh == Cshell) printf("set argv = (");
    if (osh == Perl)   printf("@ARGV = (");

    for (a = argv+1; *a; a++) {
	putchar('\'') ;
        for (p = *a; *p; p++) {
	    if (*p == '\'') {
	        if (osh == Perl) printf("' . \"'\" . '") ;
	        else printf("'\"'\"'"); 
	        continue ;
	    }
	    if ((*p == '!') && (osh == Cshell)) putchar('\\') ;
	    putchar(*p) ;
	}
	putchar('\'') ;
	if (a[1]) {		/* Separator */
	    if (osh == Perl) putchar(',') ;
	    else putchar(' ') ;
	}
    }

    /* Terminate the list of arguments */
    if (osh == Cshell) putchar(')');
    if (osh == Perl)   printf(");");
    putchar('\n') ;
}

/*==================================================================
		Main Program
 *==================================================================*/
main(argc, argv)  int argc; char **argv;
{
  char **a, *p ;
  char line[132] ;
  int i ; 

    if (argc < 2) goto ErrorExit ;

    /* Get the interesting Env. Variables */
    for (i=0; envar[i]; i++) enval[i] = getenv(envar[i]);
    bitmap = getenv("BITMAPSIZE");

    /* First argument: verbose */
    for (--argc, ++argv ; argc > 0; --argc, ++argv) {
	p = *argv;
	if (*p == '-') switch(p[1]) {
	  case 'v': 
	    if (strncmp(*argv, "-ver", 4) == 0) {
		printf("cgigraph (CDS) Version %s\n", VERSION);
		exit(0);
	    }
	    ov=1; 
	    continue;
	  /* case 'd': 
	    if (p[2]) sep = p+2 ;
	    else sep = *++argv, --argc;
	    continue ;
	  */
	  case 'f':
	    if ((strncmp(p, "-form",5) == 0) && ((p[5] == 0) || (p[5] == ':'))){
		oform = p + strloc(p, ':') ;
		if (*oform) oform++ ;
		if (!*oform) oform = "Adapt the Plot" ;
		continue ;
	    }
	    break ;
	  case 'i':
	    if ((strncmp(p, "-img", 4) == 0) && ((p[4] == 0) || (p[4] == ':'))){
		oimg = p + strloc(p, ':') ;
		if (*oimg) oimg++ ;
		continue ;
	    }
	    break ;
	  case 'n':
	    if (strncmp(p, "-nolog", 6) == 0) {
		p += 6;
		if (!*p) logxy |= 0x30;
		else while (*p) {	/* 'x'&7 = 070&7 = 0 */
		    logxy |= (*p&7) << 4;
		    p++;
		}
		continue;
	    }
	    break ;
	  case 'p':
	    if ((strncmp(p, "-ps", 3) == 0) && ((p[3] == 0) || (p[3] == ':'))){
		ops = p + strloc(p, ':') ;
		if (*ops) ops++ ;
		continue ;
	    }
	    break ;
	  case 'c':
	    if (strncmp(p, "-cap-", 5) == 0) {	/* Get Caption from stdin */
	      BUF *b;
		b = buf_alloc(512);
                while(fgets(line, sizeof(line), stdin)) 
	            buf_append(b, line) ;
	        if (ov) fprintf(stderr, "....-cap- from stdin: \n%s", b->buf) ;
		ocap = b->buf;
		continue;
	    }
	    if ((strncmp(p, "-cap", 4) == 0) && ((p[4] == 0) || (p[4] == ':'))){
		ocap = p + strloc(p, ':');
		if (*ocap) ocap++;
		continue;
	    }
	    break ;
	  case 't':
	    if ((strncmp(p, "-txt", 4) == 0) && ((p[4] == 0) || (p[4] == ':'))){
		otxt = p + strloc(p, ':') ;
		if (*otxt) otxt++ ;
		continue ;
	    }
	    break ;
	  case 'l':	/* Left columns */
	    a = at_left; goto additional_column;
	  case 'r':	/* Left columns */
	    a = at_right; goto additional_column;
          additional_column:
	    for(i=0; (i<ITEMS(at_left))&&a[i]; i++) ;
	    if (i>=ITEMS(at_left)) {
		fprintf(stderr, "#***Too many left/right elements: %d>%d\n",
			i+1, (int)(ITEMS(at_left)+1));
		exit(1);
	    }
	    ++argv; argc--;
	    if(p[strlen(p)-1] == 'f') 	/* Take from a file */
		 a[i] = load_file(*argv, 1);
	    else a[i] = *argv;
	    continue;
	}
	break ;
    }
    if (oform) osh = Html;	/* -form: accept default HTML */
    if (argc < 1) {
	if (!osh) goto ErrorExit ;
    }
    else {			/* Which kind of output ? */
	p = *argv;
	if (*p == '-') switch(p[1]) {
          case 'e':
	    osh = Exec ;
	    --argc; ++argv;
	    break ;
          case 'h':	
	    if (p[2] == 't') {	/* HTML or HTX */
	        if (p[3] == 'x') osh = Htx ;
	        else osh = Html ;
	        --argc; ++argv;
	        break ;
	    }
	    /* Assume -help ... */
    	    printf("%s", help) ;
	    exit(0) ;
          case 'p':	/* -print */
	    if ((p[2] == 'l') || (strcmp(p, "-perl") == 0)) {
	        osh = Perl ; 
	    }
	    /* else -print */
	    --argc; ++argv;
	    break ;
          case 'c':	/*  -csh  */
	    osh = Cshell; 
	    --argc; ++argv;
	    break ;
          case 's':	/*   -sh  */
	    osh = Bshell; 
	    --argc; ++argv;
	    break ;
        }
    }
    /* if (!osh) goto ErrorExit; */
    bargs = buf_alloc(0) ;

    /* Take all other arguments... */
    while (--argc >= 0) {
	p = *(argv++) ;
	if (strcmp(p, "--") == 0) { ostdin = 1 ; continue; }
	if (strcmp(p, "graph") == 0) p = "-graph" ;
	if (strncmp(p, "-graph", 6) == 0) {
	    if (!p[6]) {	/* Take rest of arguments as input */
		if (bgraph) break ;	/* graph= loaded... */
		bgraph = buf_alloc(0) ;
		while (--argc >= 0) {
		    buf_append(bgraph, *(argv++)) ;
		    buf_append1(bgraph, '\n') ;
		}
		set_graph(bgraph->buf, bargs);
		break ;
	    }
	    if (p[6] == '=') {
		if (!bgraph) bgraph = buf_alloc(0) ;
		else bgraph->used = 0 ;
		buf_append(bgraph, p+7) ;
		buf_append1(bgraph, '\n') ;
		for (p = bgraph->buf; *p; p++) { if (*p == '\t') *p = '\n'; }
		set_graph(bgraph->buf, bargs);
		continue ;
	    }
	}
	if (bgraph) {	/* Can be --bitmap -x0, etc */
	    set_graph(p, bargs) ;
	    continue ;
	}
	buf_append(bargs, p) ;
	buf_append1(bargs, '\n') ;
    }
    /* Get the stdin arguments */
    if (ostdin) {
        barg0 = buf_alloc(0) ;
        while(fgets(line, sizeof(line), stdin)) 
	    buf_append(barg0, line) ;
	if (ov) fprintf(stderr, "....from stdin: \n%s", barg0->buf) ;
        set_graph(barg0->buf, bargs) ;
    }

    /* Remove the terminating \n  */
    p = bargs->buf ;
    i = bargs->used -1;
    if (p && i && (p[i] == '\n')) {
	p[i] = 0 ;
	bargs->used = i ;
    }
    /* Show the result according to option */
    print_shell(bargs->buf) ;
    exit(0);

  ErrorExit:
    fwrite(help, 1, strloc(help, '\n'), stderr);
    fputc('\n', stderr) ;
    exit(1);
}

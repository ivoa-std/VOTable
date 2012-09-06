/*++++++++++++++
.IDENTIFICATION cgitr.h
.LANGUAGE       C
.AUTHOR         Francois Ochsenbein [CDS]
.ENVIRONMENT    cgiprint, glu
.KEYWORDS       
.VERSION  1.0   15-May-1997
.COMMENTS       Functions to translate tex into gml/html
---------------*/

#ifndef CGITR_DEF
#define CGITR_DEF	0
#ifndef _ARGS
#ifdef __STDC__
#define _ARGS(A)      A       /* ANSI */
#else
#define _ARGS(A)      ()      /* Traditional */
#define const
#endif
#endif

/*============================================================================
		Function Templates: usage of Meta-directory
 *============================================================================*/

/* Just 3 routines:
   1) Set up the HTML version (default is from $HTTP_USER_AGENT, or "3.0")
   2) Define the options via cgidef ; returns the file to be executed
   3) Execute a translation via cgitr; returns the translated text,
	which can be released via free(3c)
*/
char *cgiver _ARGS((char *html_version)) ;
char *cgidef _ARGS((char *options)) ;
char *cgitr  _ARGS((char *text)) ;
 
#endif

#++++++++++++++++
#.IDENTIFICATION Makefile
#.LANGUAGE       Make (makefile script)
#.AUTHOR         Francois Ochsenbein [CDS]
#.ENVIRONMENT    Unix
#.KEYWORDS       
#.VERSION  1.0   07-Jun-2004
#.PURPOSE        (Re)Create dependent files in this Directory
#.COMMENTS       All details for VOTable-1.2 can be downloaded from
#		ftp://cdsarc.u-strasbg.fr/sw/votable-export.tar.gz
#		ftp://cdsarc.u-strasbg.fr/sw/cgiprint.tar.gz
#----------------

SHELL=/bin/sh
#MACHINE=local

# Defaut rule (e.g. copy from a public directory)
.DEFAULT:

# Programs:
CC      = cc
COPY    = cp -p
STRIP   = strip
RANLIB  = ranlib
AR      = ar ruv

# Definitions:
DEBUG =
C_OPT = -O
SYS =
CFLAGS = $(C_OPT) $(DEBUG) $(SYS) -I. -I$(INCDIR)
COMPILE= $(CC) $(CFLAGS) -c
CGIPRINT = cgiprint/bin/cgiprint

# Files
IMGs = circ2.gif leadsto2.gif oplus.gif serial.png binary.gif
XSD  = VOTable.xsd

default: votable.pdf votable.html

votable.pdf: votable.tex VOTable.attr.tex  VOTable.elem.tex serial.png
	pdflatex votable && \
	pdflatex votable && \
	pdflatex votable

votable.html: votable.tex votable.htx $(CGIPRINT)
	$(CGIPRINT) votable.htx > votable.html

votable.tar: votable.html votable.htx votable.pdf $(IMGs) $(XSD)
	$(MAKE) votable.html
	$(MAKE) votable.pdf
	mv votable.html votable.tmp; trim votable.tmp | gawk '\
	 { if(x<1) { if ($$0 == "") next; x++; sub(/^ */, "") } print}' \
	 > votable.html; rm votable.tmp
	tar cvf $@ votable.html votable.pdf $(IMGs) $(XSD)
	gzip -v9 $@ 

cgiprint/bin/cgiprint:
	cd cgiprint && \
        ./configure --prefix=`pwd` && \
        make && \
        make install

# Make export tar
export:	CLEAN
	h=`pwd`;d=`basename $$h`; tar cvf /tmp/$$d.tar -C .. $$d; \
	gzip -v9 /tmp/$$d.tar

# Remove useless files:
clean: 
	rm -f *.log *.aux *.out
	cd cgiprint && rm -f cgiprint cgigraph cgiparm

# Remove depedent files:
CLEAN: clean
	rm -f votable.toc votable.pdf votable.html
	cd cgiprint && rm -rf include lib bin
	cd cgiprint && rm -f config.log config.status Makefile
	cd cgiprint && rm -f cgiprint.lis versions

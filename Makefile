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
XDOC_CLASSPATH = figicon/xdoc.jar:figicon/epsgraphics.jar:figicon/Text_shrunk.jar
BINFIGURE = java -classpath $(XDOC_CLASSPATH):. BinFigure

# Files
IMGs = circ2.gif leadsto2.gif oplus.gif serial.png binary.png binary2.png \
       ivoa-archi.png
XSD  = VOTable.xsd
UPLOAD_NAME = PR-VOTable-1.4-20190604

default: votable.pdf votable.html

votable.pdf: votable.tex VOTable.attr.tex  VOTable.elem.tex \
             serial.png binary.pdf binary2.pdf ivoa-archi.pdf \
             VOTable.xsd stc_example1.vot stc_example2.vot timesys_example.vot
	pdflatex votable && \
	pdflatex votable && \
	pdflatex votable

votable.html: votable.tex votable.htx $(CGIPRINT) \
              binary.png binary2.png ivoa-archi.png \
              VOTable.xsd stc_example1.vot stc_example2.vot timesys_example.vot
	$(CGIPRINT) votable.htx > votable.html

upload: $(UPLOAD_NAME).tar

$(UPLOAD_NAME).tar: votable.html votable.pdf $(IMGs)
	rm -rf tmp/
	mkdir tmp
	cp votable.html tmp/$(UPLOAD_NAME).html
	cp votable.pdf tmp/$(UPLOAD_NAME).pdf
	cp $(IMGs) tmp/
	cp $(XSD) tmp/
	cd tmp; tar cvf ../$@ $(UPLOAD_NAME).html $(UPLOAD_NAME).pdf $(IMGs) $(XSD)
	rm -rf tmp/

cgiprint/bin/cgiprint:
	cd cgiprint && \
        ./configure --prefix=`pwd` && \
        make && \
        make install

BinFigure.class: BinFigure.java
	javac -classpath $(XDOC_CLASSPATH) BinFigure.java

binary.png: BinFigure.class
	$(BINFIGURE) -png BINARY >$@

binary.pdf: BinFigure.class
	$(BINFIGURE) -pdf BINARY >$@

binary2.png: BinFigure.class
	$(BINFIGURE) -png BINARY2 >$@

binary2.pdf: BinFigure.class
	$(BINFIGURE) -pdf BINARY2 >$@

ivoa-archi.png: ivoa-archi.pdf
	convert -antialias -resize 800 $< $@

# Make export tar
export:	CLEAN
	h=`pwd`;d=`basename $$h`; tar cvf /tmp/$$d.tar -C .. $$d; \
	gzip -v9 /tmp/$$d.tar

# Remove useless files:
clean: 
	rm -f votable.log votable.aux votable.out votable.toc
	rm -f BinFigure.class binary.pdf binary2.pdf
	rm -f votable.tar
	cd cgiprint && rm -f cgiprint cgigraph cgiparm
	cd cgiprint && rm -rf include lib bin
	cd cgiprint && rm -f config.log config.status Makefile
	cd cgiprint && rm -f cgiprint.lis versions

# Remove depedent files:
CLEAN: clean
	rm -f votable.pdf votable.html $(UPLOAD_NAME).tar
	rm -f binary.png binary2.png ivoa-archi.png

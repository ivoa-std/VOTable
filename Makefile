# ivoatex Makefile.  The ivoatex/README for the targets available.

# short name of your document (edit $DOCNAME.tex; would be like RegTAP)
DOCNAME = VOTable

# count up; you probably do not want to bother with versions <1.0
DOCVERSION = 1.5

# Publication date, ISO format; update manually for "releases"
DOCDATE = 2023-02-28

# What is it you're writing: NOTE, WD, PR, REC, PEN, or EN
DOCTYPE = WD

# An e-mail address of the person doing the submission to the document
# repository (can be empty until a make upload is being made)
AUTHOR_EMAIL=???

# Source files for the TeX document (but the main file must always
# be called $(DOCNAME).tex
SOURCES = $(DOCNAME).tex role_diagram.pdf \
          VOTable.attr.tex VOTable.elem.tex \
          VOTable.xsd stc_example1.vot stc_example2.vot timesys_example.vot \
          binary.pdf binary2.pdf

# List of image files to be included in submitted package (anything that
# can be rendered directly by common web browsers)
FIGURES = role_diagram.svg \
          serial.png binary.png binary2.png circ2.gif leadsto2.gif oplus.gif

# List of PDF figures (figures that must be converted to pixel images to
# work in web browsers).
VECTORFIGURES =

# Additional files to distribute (e.g., CSS, schema files, examples...)
AUX_FILES = VOTable.xsd

# Include Makefile from ivoatex
include ivoatex/Makefile

# Custom build rules
XDOC_CLASSPATH = \
    figicon/xdoc.jar:figicon/epsgraphics.jar:figicon/Text_shrunk.jar
BINFIGURE = java -classpath $(XDOC_CLASSPATH):. BinFigure

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

clean-more: clean
	rm -f binary.png binary.pdf binary2.png binary2.pdf BinFigure.class
	rm -f ivoatex/tth_C/tth

ivoatex/Makefile:
	@echo "*** ivoatex submodule not found.  Initialising submodules."
	@echo
	git submodule update --init

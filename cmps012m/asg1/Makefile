# $Id: Makefile,v 1.3 2013-10-13 01:38:08-07 - - $
#Peter Greer-Berezovsky pbgreerb
#James Robert Anderson Brower jrbrower

JAVASRC    = jfmt.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = jfmt
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES}
JARFILE    = jfmt
SUBMITDIR  = cmps012b-wm.f13 asg1
LISTING    = Listing.ps

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	cid + $<
	javac $<
	checksource $<

clean :
	- rm ${JARCLASSES} test.output

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	 cid + ${SOURCES} test.input
	 checksource ${SOURCES}

lis : all
	${JARFILE} test.input >test.output
	mkpspdf ${LISTING} ${SOURCES} test.input test.output

submit : ${SOURCES} ci
	submit ${SUBMITDIR} ${SOURCES}

again : ${SOURCES}
	gmake --no-print-directory spotless lis
	

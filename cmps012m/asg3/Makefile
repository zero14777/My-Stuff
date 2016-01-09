# $Id: Makefile,v 1.2 2013-10-17 18:28:59-07 - - $

JAVASRC    = bitreecalc.java bitree.java linked_stack.java \
             symbol_table.java auxlib.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = bitreecalc
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES} linked_stack\$$node.class
JARFILE    = bitreecalc
LISTING    = Listing.ps
SUBMITDIR  = cmps012b-wm.f13 asg3

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	javac $<

clean :
	- rm ${JARCLASSES} Manifest

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	- checksource ${SOURCES}
	cid + ${SOURCES}

lis : ${SOURCES}
	mkpspdf ${LISTING} ${SOURCES}

submit : ${SOURCES}
	submit ${SUBMITDIR} ${SOURCES}
	testsubmit ${SUBMITDIR} ${SOURCES}

again :
	gmake --no-print-directory spotless ci all lis


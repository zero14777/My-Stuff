# $Id: Makefile,v 1.1 2013-10-03 15:37:00-07 - - $

JAVASRC    = hello.java
SOURCES    = README Makefile ${JAVASRC}
MAINCLASS  = hello
CLASSES    = hello.class
JARFILE    = hello
JARCLASSES = ${CLASSES}
SUBMITDIR  = cmps012b-wm.f13 lab1

all: ${JARFILE}

${JARFILE}: ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class: %.java
	cid + $<
	javac $<

clean:
	- rm ${CLASSES} test.output

spotless: clean
	- rm ${JARFILE}
	- ls -ago

ci: ${SOURCES}
	cid + ${SOURCES}

check: ${SOURCES}
	- checksource ${SOURCES}

test: ${JARFILE}
	( echo "%%%%%%%% ${JARFILE}" \
	; ${JARFILE} 2>&1 \
	; echo "%%%%%%%% Exit status = $$?" \
	; echo "%%%%%%%% uname -a" ; uname -a | sed 's/#/\n#/' \
	) >test.output
	cat -nv test.output

submit: check ${SOURCES}
	submit ${SUBMITDIR} ${SOURCES}

again:
	gmake --no-print-directory spotless ci all test check

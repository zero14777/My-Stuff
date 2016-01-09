GCC      = gcc -g -O0 -Wall -Wextra -std=gnu99
CSOURCES = l8stat.c
SOURCES  = ${CSOURCES} Makefile
EXECBINS = l8stat
LISTING  = Listing.ps
SUBMIT   = submit cmps012b-wm.f13 lab8

all : ${EXECBINS}

% : %.c
	${GCC} $< -o $@

ci :
	cid + ${SOURCES}
	- checksource $<

clean :
	- rm -vf core

spotless : clean
	- rm -vf ${EXECBINS}

lis : ${CSOURCES} Makefile
	mkpspdf ${LISTING} ${SOURCES}

submit : ${SOURCES}
	${SUBMIT} ${SOURCES}

again : ${SOURCES}
	gmake --no-print-directory spotless ci all lis



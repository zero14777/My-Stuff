GCC      = gcc -g -O0 -Wall -Wextra -std=gnu99
CSOURCES = inssort.c numsort.c linesort.c
CHEADER  = inssort.h
OBJECTS  = ${CSOURCE:.c=.o}
SOURCES  = ${CHEADER} ${CSOURCES} Makefile
EXECBINS = numsort linesort
SUBMIT   = submit cmps012b-wm.f13 lab9

all : ${EXECBINS}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	${GCC} -c $< ${CSOURCES}

inssort : inssort.c
	${GCC} -c $<

numsort : numsort.o inssort.o
	${GCC} numsort.o inssort.o -o numsort

linesort : linesort.o inssort.o
	${GCC} linesort.o inssort.o -o linesort

ci :
	cid + ${SOURCES}

submit : ${SOURCES}
	${SUBMIT} ${SOURCES}



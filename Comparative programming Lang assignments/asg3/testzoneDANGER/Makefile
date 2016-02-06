# $Id: Makefile,v 1.8 2014-10-07 17:37:02-07 - - $
# Peter Greer-Berezovsky   pbgreerb@ucsc.edu
# Jim Brower               jrbrower@ucsc.edu

GPP   = g++ -g -O0 -Wall -Wextra -std=gnu++11
GRIND = valgrind --leak-check=full --show-reachable=yes

all : oc

oc : main.o stringset.o auxlib.o
	${GPP} main.o stringset.o auxlib.o -o oc

%.o : %.cpp
	${GPP} -c $<

ci :
	cid + Makefile auxlib.h auxlib.cpp stringset.h stringset.cpp main.cpp

spotless : clean
	- rm oc Listing.ps Listing.pdf test.out test.err

clean :
	-rm stringset.o main.o auxlib.o

test : oc
	${GRIND} oc * * * >test.out 2>test.err

lis : test
	mkpspdf Listing.ps stringset.h stringset.cpp main.cpp \
	        Makefile test.out test.err

# Depencencies.
main.o: main.cpp stringset.h auxlib.h
stringset.o: stringset.cpp stringset.h


# $Id: Makefile,v 1.8 2014-10-07 17:37:02-07 - - $
# Peter Greer-Berezovsky   pbgreerb@ucsc.edu
# Jim Brower               jrbrower@ucsc.edu

GPP   = g++ -g -O0 -Wall -Wextra -std=gnu++11
GRIND = valgrind --leak-check=full --show-reachable=yes
OBJECTS = yyparse.o yylex.o lyutils.o astree.o \
		auxlib.o stringset.o main.o \
		trevorsal.o symboltable.o

all : oc

oc : ${OBJECTS}
	${GPP} $^ -o $@

%.o : %.cpp
	${GPP} -c $<

yyparse.cpp : parser.y
	bison --defines=yyparse.h --output=yyparse.cpp parser.y

yylex.cpp : scanner.l
	flex -o yylex.cpp scanner.l

ci :
	cid + Makefile auxlib.h auxlib.cpp stringset.h \
		stringset.cpp main.cpp trevorsal.cpp

spotless : clean
	rm oc

clean :
	rm -f *.o yyparse.* yylex.cpp

test : oc
	${GRIND} oc * * * >test.out 2>test.err

lis : test
	mkpspdf Listing.ps stringset.h stringset.cpp main.cpp \
	        Makefile test.out test.err

# Dependencies.
main.o: main.cpp stringset.h auxlib.h lyutils.h
stringset.o: stringset.cpp stringset.h
auxlib.o: auxlib.cpp auxlib.h
yylex.o: scanner.l auxlib.h lyutils.h
lyutils.o: lyutils.cpp auxlib.h lyutils.h
astree.o: astree.cpp astree.h stringset.h lyutils.h
yyparse.o: yyparse.cpp yyparse.h lyutils.h astree.h
trevorsal.o: trevorsal.cpp astree.h trevorsal.h
symboltable.o: symboltable.cpp symboltable.h trevorsal.h
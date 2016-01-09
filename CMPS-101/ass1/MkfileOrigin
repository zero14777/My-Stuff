# Makefile for intstack module - by DPH, modified Spring 2014
# It compiles the module and creates the intstackdr executable

intstackdr : intstackdr.o intstack.o
	gcc -ansi -Wall -Wextra -pedantic -o intstackdr intstackdr.o intstack.o

intstack.o : intstack.h intstack.c
	gcc -c -ansi -Wall -Wextra -pedantic intstack.c

intstackdr.o : intstack.h intstackdr.c
	gcc -c -ansi -Wall -Wextra -pedantic intstackdr.c

clean :
	rm intstackdr intstackdr.o intstack.o

# an old version of the makefile used the additional gcc option
# -I/usr/lang/SC1.0/ansi_include but it is not needed on my system
# we should double check that that option is not needed on the cats system
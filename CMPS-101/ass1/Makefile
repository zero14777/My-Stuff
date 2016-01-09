# Makefile for intstack module - by DPH, modified Spring 2014
# Modified by pbgreerb to compile the decToBinary executable
# instead of the intstackdr executible.
# It compiles the module and creates the decToBinary executable

decToBinary : decToBinary.o intstack.o
	gcc -ansi -Wall -Wextra -pedantic -o decToBinary decToBinary.o intstack.o

intstack.o : intstack.h intstack.c
	gcc -c -ansi -Wall -Wextra -pedantic intstack.c

decToBinary.o : intstack.h decToBinary.c
	gcc -c -ansi -Wall -Wextra -pedantic decToBinary.c

clean :
	rm decToBinary decToBinary.o intstack.o

# an old version of the makefile used the additional gcc option
# -I/usr/lang/SC1.0/ansi_include but it is not needed on my system
# we should double check that that option is not needed on the cats system

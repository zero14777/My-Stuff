# Peter Greer-Berezovsky
# pbgreerb
# 4/3/2014
#
# pbgreerb@ucsc.edu
# Cruz ID: 1301397
#
# Compiles the Listdr and store driver programs for the List ADT

TARGET = Listdr store

all : $(TARGET)

Listdr : Listdr.o List.o
	gcc -ansi -Wall -Wextra -pedantic -o Listdr Listdr.o List.o

store : store.o List.o
	gcc -ansi -Wall -Wextra -pedantic -o store store.o List.o

store.o : List.h store.c
	gcc -c -ansi -Wall -Wextra -pedantic store.c

List.o : List.h List.c
	gcc -c -ansi -Wall -Wextra -pedantic List.c

Listdr.o : List.h Listdr.c
	gcc -c -ansi -Wall -Wextra -pedantic Listdr.c

clean :
	rm List.o Listdr.o


#
#

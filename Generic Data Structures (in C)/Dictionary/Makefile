#Peter Greer-Berezovsky
# pbgreerb
# 4/26/2014
#
# pbgreerb@ucsc.edu
# Cruz ID: 1301397
#
# A makefile that complies multiple module and driver programs.
# It makes Listdr, Dictionarydr, LibrarySearch, List.o, Dictionary.o,
# Listdr.o, Dictionarydr.o, and LibrarySearch.o.

TARGET = Listdr Dictionarydr LibrarySearch

all : $(TARGET)

Listdr : Listdr.o List.o
	gcc -ansi -Wall -Wextra -pedantic -o Listdr Listdr.o List.o

Dictionarydr : Dictionarydr.o Dictionary.o
	gcc -ansi -Wall -Wextra -pedantic -o Dictionarydr Dictionarydr.o Dictionary.o List.o

LibrarySearch : LibrarySearch.o Dictionary.o
	gcc -ansi -Wall -Wextra -pedantic -o LibrarySearch LibrarySearch.o Dictionary.o List.o

List.o : List.h List.c
	gcc -c -ansi -Wall -Wextra -pedantic List.c

Listdr.o : List.h Listdr.c
	gcc -c -ansi -Wall -Wextra -pedantic Listdr.c

Dictionary.o : Dictionary.h Dictionary.c List.o
	gcc -c -ansi -Wall -Wextra -pedantic Dictionary.c

Dictionarydr.o : Dictionary.h Dictionarydr.c
	gcc -c -ansi -Wall -Wextra -pedantic Dictionarydr.c

LibrarySearch.o : Dictionary.h LibrarySearch.c
	gcc -c -ansi -Wall -Wextra -pedantic LibrarySearch.c

clean :
	rm List.o Listdr.o Dictionary.o Dictionarydr.o LibrarySearch.o Listdr Dictionarydr LibrarySearch


#
#

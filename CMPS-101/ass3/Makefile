#Peter Greer-Berezovsky
# pbgreerb
# 4/26/2014
#
# pbgreerb@ucsc.edu
# Cruz ID: 1301397
#
# Compiles the heapDriver, sortPrint, and sortComp programs for the heap ADT

TARGET = heapDriver sortPrint sortComp

all : $(TARGET)

heapDriver : heapDriver.o heap.o
	gcc -ansi -Wall -Wextra -pedantic -o heapDriver heapDriver.o heap.o

sortPrint : sortPrint.o heap.o insertionSort.o heapSort.o
	gcc -ansi -Wall -Wextra -pedantic -o sortPrint sortPrint.o heap.o insertionSort.o heapSort.o

sortComp : sortComp.o heap.o insertionSort.o heapSort.o
	gcc -ansi -Wall -Wextra -pedantic -o sortComp sortComp.o heap.o insertionSort.o heapSort.o

heap.o : heap.h heap.c
	gcc -c -ansi -Wall -Wextra -pedantic heap.c

heapDriver.o : heap.h heapDriver.c
	gcc -c -ansi -Wall -Wextra -pedantic heapDriver.c

sortPrint.o : heap.h sortPrint.c
	gcc -c -ansi -Wall -Wextra -pedantic sortPrint.c

sortComp.o : heap.h sortComp.c
	gcc -c -ansi -Wall -Wextra -pedantic sortComp.c

insertionSort.o : insertionSort.h insertionSort.c
	gcc -c -ansi -Wall -Wextra -pedantic insertionSort.c

heapSort.o : heapSort.h heapSort.c
	gcc -c -ansi -Wall -Wextra -pedantic heapSort.c

clean :
	rm heap.o heapDriver.o sortPrint.o sortComp.o insertionSort.o heapSort.o


#
#

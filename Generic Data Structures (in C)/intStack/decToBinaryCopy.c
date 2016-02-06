/*
 * Peter Greer-Berezovsky
 * pbgreerb@ucsc.edu
 * Cruz ID: 1301397
 *
 * A simple program that takes a non-negative decimal integer as
 * input and prints out the binary value of that integer. AFter
 * dealing with one input the program ends. If the input is not
 * a non-negative integer the program will print our an error
 * message before concluding.
*/

#include <stdlib.h>
#include <stdio.h>
#include "myinclude.h"
#include "intstack.h"

/*
 * Takes an non-negative integer value and pushes the appropriate
 * sequence of 1's and 0's that represents the given integer's value
 * in binary onto the binaryStack. If the given integer is negative
 * nothing will be pushed onto the stack.
*/

void decimalToBinary(int decimalValue, StackHndl binaryStack) {
	if(decimalValue == 0){
		Push(binaryStack, 0);
	}

	while(decimalValue > 0){
		if(decimalValue % 2){
			Push(binaryStack, 1);
			decimalValue--;
		} else {
			Push(binaryStack, 0);
		}
		decimalValue = decimalValue/2;
	}
}

/*
 * Prints out the elements of the binaryStack starting at the top
 * of the stack. 
*/ 

void printBinaryStack(int decimalValue, StackHndl binaryStack) {
	printf("Decimal %d is ", decimalValue);

	while(!IsEmpty(binaryStack)){
		int temp = Top(binaryStack);
		printf("%d", temp);
		Pop(binaryStack);
	}

	printf(" in binary.\n");
}

/*
 * Takes a non-negative decimal integer as input and  prints
 * out the binary representation of that integer. If the input
 * is not a non-negative decimal integer an error is printed.
*/

int main () {
	int input, inputCheck;
	char term;

        StackHndl binaryStack;

        binaryStack = NULL;

        binaryStack = NewStack();

	printf("Input a non-negative decimal integer to convert to binary:\n");
	printf("> ");

	inputCheck = scanf("%d%c", &input, &term);

	if(inputCheck == 2 && term == '\n' && input >= 0){
		decimalToBinary(input, binaryStack);
		printBinaryStack(input, binaryStack);
		FreeStack(&binaryStack);
        	return(0);
	} else {
		fprintf(stderr, "Invalid input.\n");
		fprintf(stderr, "Input must be a non-negative integer.\n");
		FreeStack(&binaryStack);
		return(1);
	}
}

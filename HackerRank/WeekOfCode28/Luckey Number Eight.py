#!/bin/python3
#Given an -digit positive integer, count and print the number of multiples of  that can be formed by concatenating subsequences of the given number's digits, modulo .

#Input Format

#The first line contains an integer denoting . 
#The second line contains a string describing an -digit integer.

#!/bin/python3

import sys

n = int(input().strip())
number = input().strip()
# your code goes here
#keep track of all possible subsequences with a third least sig digit that is odd or even
odds = 0
evens = 0

#keep track of how many subsequences can be made using the various even digits
acc08 = 0
acc6 = 0
acc4 = 0
acc2 = 0

#keep track of digit position we are looking at
pos = 0

output = 0

for c in number:
    if int(c) == 0 or int(c) == 8:
        output = output + 1 + acc08
        acc08 = acc08 + 1 + evens
        acc4 = acc4 + odds
    elif int(c) == 6:
        output = output + acc6
        acc08 = acc08 + odds
        acc4 = acc4 + 1 + evens
    elif int(c) == 4:
        output = output + acc4
        acc08 = acc08 + 1 + evens
        acc4 = acc4 + odds
    elif int(c) == 2:
        output = output + acc2
        acc08 = acc08 + odds
        acc4 = acc4 + 1 + evens
    elif int(c) == 3 or int(c) == 7:
        acc2 = acc2 + 1 + evens
        acc6 = acc6 + odds
    else:#1 or 5 or 9
        acc2 = acc2 + odds
        acc6 = acc6 + 1 + evens
    if int(c) % 2 == 1:
        odds = odds + pow(2, pos)
    else:
        evens = evens + pow(2, pos)
    pos = pos + 1

print(output%1000000007)
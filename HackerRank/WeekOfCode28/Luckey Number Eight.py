#!/bin/python3
#Given an -digit positive integer, count and print the number of multiples of  that can be formed by concatenating subsequences of the given number's digits, modulo .

#Input Format

#The first line contains an integer denoting . 
#The second line contains a string describing an -digit integer.

#!/bin/python3

import sys


n = int(input().strip())
number = input().strip()

result = 0

if n >= 1:
    for a in number:
        if ((ord(a) - 48) % 8) == 0:
            result = result + 1

if n >= 2:
    for a in range(0, (len(number)-1)):
        for b in range(a+1, len(number)):
            seq = (ord(number[a]) -48) * 10
            seq += (ord(number[b]) -48)
            if (seq % 8) == 0:
                result = result + 1

#Way too slow
if n >= 3:
    for c in range(len(number)-1, 1, -1):
        if (ord(number[c]) - 48) % 2 == 1:
            continue
        for b in range(c-1, 0, -1):
            for a in range(b-1, -1, -1):
                seq = (ord(number[a]) -48) * 100
                seq += (ord(number[b]) -48) * 10
                seq += (ord(number[c]) -48)
                if (seq % 8) == 0:
                    result = (result + pow (2, a)) % (1000000007)
        
print(result)
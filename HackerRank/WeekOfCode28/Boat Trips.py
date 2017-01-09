#!/bin/python3

import sys

n,c,m = input().strip().split(' ')
n,c,m = [int(n),int(c),int(m)]
p = list(map(int, input().strip().split(' ')))

TripCapacity = c * m
output = "Yes"

for TotalPassengers in p:
    if TotalPassengers > TripCapacity:
        output = "No"

print(output)
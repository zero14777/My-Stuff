#Every student receives a grade in the inclusive range from 0 to 100.
#Any grade less than 40 is a failing grade.
#Sam is a professor at the university and likes to round each student's grade according to these rules:

#If the difference between the grade and the next multiple of 5 is less than 3, round grade up to the next multiple of 5.
#If the value of grade is less than 38, no rounding occurs as the result will still be a failing grade.
#!/bin/python3

import sys


n = int(input().strip())
for a0 in range(n):
    grade = int(input().strip())
    # your code goes here
    if grade > 37 and grade % 5 > 2: 
        grade = grade + 5 - (grade%5)
    print(grade)
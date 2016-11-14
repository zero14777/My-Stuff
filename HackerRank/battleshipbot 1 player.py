from random import randint

def findnewship (Matrix, MatrixSize):
    most_viable = (-1, (-1, -1))
    for row in range(0, (MatrixSize)):
        for col in range(0, (MatrixSize)):
            if Matrix[row][col] == '-':
                viability = 0
                viability = viability + checkspace (Matrix, MatrixSize, (row - 2), col)
                viability = viability + checkspace (Matrix, MatrixSize, (row - 1), col)
                viability = viability + checkspace (Matrix, MatrixSize, (row + 1), col)
                viability = viability + checkspace (Matrix, MatrixSize, (row + 2), col)
                viability = viability + checkspace (Matrix, MatrixSize, row, (col - 2))
                viability = viability + checkspace (Matrix, MatrixSize, row, (col - 1))
                viability = viability + checkspace (Matrix, MatrixSize, row, (col + 1))
                viability = viability + checkspace (Matrix, MatrixSize, row, (col + 2))
                if viability > most_viable[0]:
                    most_viable = (viability, (row, col))
    return str(most_viable[1][0]) + " " + str(most_viable[1][1])

def checkspace (Matrix, MatrixSize, x, y):
    if x >= MatrixSize or x < 0 or y >= MatrixSize or y < 0 or Matrix[x][y] == 'm' or Matrix[x][y] == 'd':
        return 0
    
    if Matrix[x][y] == 'h':
        return 1
    
    return 3

def checkpair (Matrix, MatrixSize, hit, direction):
    pair = -1
    guess = (-1, -1)
    if direction == "vertical":
        down = checkspace (Matrix, MatrixSize, hit[0], hit[1] + 1)
        up = checkspace (Matrix, MatrixSize, hit[0], hit[1] - 1)
        pair = up + down
        if up == 3:
            guess = (hit[0], hit[1] - 1)
        elif down == 3:
            guess = (hit[0], hit[1] + 1)
    elif direction == "horizontal":
        right = checkspace (Matrix, MatrixSize, hit[0] + 1, hit[1])
        left = checkspace (Matrix, MatrixSize, hit[0] - 1, hit[1])
        pair = left + right
        if left == 3:
            guess = (hit[0] - 1, hit[1])
        elif right == 3:
            guess = (hit[0] + 1, hit[1])
    if pair < 3:
        return (0, guess)
    elif pair == 3:
        return (1, guess)
    elif pair == 4:
        return (3, guess)
    elif pair == 6:
        return (2, guess)

def checkhit (Matrix, MatrixSize, hit):
    vertical = checkpair (Matrix, MatrixSize, hit, "vertical")
    horizontal = checkpair (Matrix, MatrixSize, hit, "horizontal")
    
    if vertical[0] > horizontal[0]:
        return vertical
    elif horizontal[0] > 0:
        return horizontal
    else:
        return vertical

MatrixSize = int(input())
Matrix = [[' ' for x in range(MatrixSize)] for y in range(MatrixSize)] 

hits = []

for row in range(0, (MatrixSize)):
    RowString = input()
    for col in range(0, (MatrixSize)):
        Matrix[row][col] = RowString[col]
        if RowString[col] == 'h':
            hits.append((row, col))
            
guess = (0, ())

for hit in hits:
    best_guess = checkhit (Matrix, MatrixSize, hit)
    if best_guess[0] > guess[0]:
        guess = best_guess
    
if guess[0] != 0:
    print (str(guess[1][0]) + " " + str(guess[1][1]))
else:
    print(findnewship(Matrix, MatrixSize))
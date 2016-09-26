import re

PacMan = input()
PacManSplit = re.findall ('\d+', PacMan)
PacRow = int(PacManSplit[0])
PacCol = int(PacManSplit[1])

Food = input()
FoodSplit = re.findall ('\d+', Food)
FoodRow = int(FoodSplit[0])
FoodCol = int(FoodSplit[1])

MatrixSize = input()
MatrixSizeSplit = re.findall ('\d+', MatrixSize)
MatrixSizeRows = int(MatrixSizeSplit[0])
MatrixSizeCols = int(MatrixSizeSplit[1])

Matrix = [[0 for x in range(MatrixSizeCols)] for y in range(MatrixSizeRows)] 

for row in range(0, (MatrixSizeRows)):
    RowString = input()
    for col in range(0, (MatrixSizeCols)):
        Matrix[row][col] = RowString[col]


Stack = [[PacRow, PacCol]]
Path = []
Explored = []
Seen = []

while (1):
    if len(Stack) == 0:
        print ("Could not find a path")
        break
        
    Current = Stack.pop()
    if len(Path) > 0:
        Prev = Path[len(Path) - 1]
    
        while (Current[2] != Prev[0] or Current[3] != Prev[1]):
            Path.pop()
            Prev = Path[len(Path) - 1]
    
    Explored.append (str(Current[0]) + " " + str(Current[1]))
    Path.append (Current)
    if (Current[0] == FoodRow and Current[1] == FoodCol):
        break

    Upval = (Current[0] - 1)
    checkspace = str(Upval) + " " + str(Current[1])
    if Matrix[Current[0]-1][Current[1]] != "%" and checkspace not in Explored and checkspace not in Seen:
        Stack.append ([Current[0]-1, Current[1], Current[0], Current[1]])
        Seen.append (str(Upval) + " " + str(Current[1]))
    
    Leftval = (Current[1] - 1)
    checkspace = str(Current[0]) + " " + str(Leftval)
    if Matrix[Current[0]][Current[1]-1] != "%" and checkspace not in Explored and checkspace not in Seen:
        Stack.append ([Current[0], Current[1]-1, Current[0], Current[1]])
        Seen.append (str(Current[0]) + " " + str(Leftval))
        
    Rightval = (Current[1] + 1)
    checkspace = str(Current[0]) + " " + str(Rightval)
    if Matrix[Current[0]][Current[1]+1] != "%" and checkspace not in Explored and checkspace not in Seen:
        Stack.append ([Current[0], Current[1]+1, Current[0], Current[1]])
        Seen.append (str(Current[0]) + " " + str(Rightval))
      
    Downval = (Current[0] + 1)
    checkspace = str(Downval) + " " + str(Current[1])
    if Matrix[Current[0]+1][Current[1]] != "%" and checkspace not in Explored and checkspace not in Seen:
        Stack.append ([Current[0]+1, Current[1], Current[0], Current[1]])
        Seen.append (str(Downval) + " " + str(Current[1]))

print(len(Explored))
for val in Explored:
    print(val)
print(len(Path) - 1)
for val in Path:
    print(str(val[0]) + " " + str(val[1]))
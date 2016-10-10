import re
from queue import *

def countdist (coordinates):
    if coordinates == [-1, -1]:
        return -1
    temp = countdist (TracebackMatrix[coordinates[0]][coordinates[1]])
    return temp + 1
    
def traceback(coordinates):
    if coordinates == [-1, -1]:
        return ""
    traceback (TracebackMatrix[coordinates[0]][coordinates[1]])
    print(str(coordinates[0]) + " " + str(coordinates[1]))

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

TracebackMatrix = [[0 for x in range(MatrixSizeCols)] for y in range(MatrixSizeRows)] 
Matrix = [[0 for x in range(MatrixSizeCols)] for y in range(MatrixSizeRows)] 

for row in range(0, (MatrixSizeRows)):
    RowString = input()
    for col in range(0, (MatrixSizeCols)):
        Matrix[row][col] = RowString[col]

q = Queue ()
q.put([PacRow, PacCol])
Matrix[PacRow][PacCol] = "%"
TracebackMatrix[PacRow][PacCol] = [-1, -1]

TotalSearched = 0
SearchLog = ""

while (1):
    if q.empty():
        print ("Could not find a path")
        break
        
    Current = q.get()
    if SearchLog == "" :
        SearchLog = str(Current[0]) + " " + str(Current[1])
    else:
        SearchLog = SearchLog + "\n" + str(Current[0]) + " " + str(Current[1])
    TotalSearched = TotalSearched + 1
        
    if (Current[0] == FoodRow and Current[1] == FoodCol):
        break

    Upval = (Current[0] - 1)
    checkspace = str(Upval) + " " + str(Current[1])
    if Matrix[Upval][Current[1]] != "%":
        q.put([Upval, Current[1]])
        Matrix[Upval][Current[1]] = "%"
        TracebackMatrix[Upval][Current[1]] = [Current[0], Current[1]]
    
    Leftval = (Current[1] - 1)
    checkspace = str(Current[0]) + " " + str(Leftval)
    if Matrix[Current[0]][Leftval] != "%":
        q.put([Current[0], Leftval])
        Matrix[Current[0]][Leftval] = "%"
        TracebackMatrix[Current[0]][Leftval] = [Current[0], Current[1]]
        
    Rightval = (Current[1] + 1)
    checkspace = str(Current[0]) + " " + str(Rightval)
    if Matrix[Current[0]][Rightval] != "%":
        q.put([Current[0], Rightval])
        Matrix[Current[0]][Rightval] = "%"
        TracebackMatrix[Current[0]][Rightval] = [Current[0], Current[1]]
      
    Downval = (Current[0] + 1)
    checkspace = str(Downval) + " " + str(Current[1])
    if Matrix[Downval][Current[1]] != "%":
        q.put([Downval, Current[1]])
        Matrix[Downval][Current[1]] = "%"
        TracebackMatrix[Downval][Current[1]] = [Current[0], Current[1]]
        
print(TotalSearched)
print(SearchLog)
print(countdist ([FoodRow, FoodCol]))
traceback([FoodRow, FoodCol])
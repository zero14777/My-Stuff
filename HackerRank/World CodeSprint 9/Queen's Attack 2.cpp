/*
Input Format
The first line contains two space-separated integers describing the respective values of n (the side length of the board) and k (the number of obstacles). 
The next line contains two space-separated integers describing the respective values of r and c, denoting the position of the queen. 
Each line i of the k subsequent lines contains two space-separated integers describing the respective values of r and c, denoting the position of obstacle i.

Output Format

Print the number of squares that the queen can attack.
*/

#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;


int main(){
    int up, down, left, right, UR, UL, DR, DL = 0;
    int n;
    int k;
    cin >> n >> k;
    int rQueen;
    int cQueen;
    cin >> rQueen >> cQueen;
    
    up = n - rQueen;
    down = rQueen - 1;
    right = n - cQueen;
    left = cQueen - 1;
    UR = min (n - rQueen, n - cQueen);
    UL = min (n - rQueen, cQueen - 1);
    DR = min (rQueen - 1, n - cQueen);
    DL = min (rQueen - 1, cQueen - 1);
    
    for(int a0 = 0; a0 < k; a0++){
        int rObstacle;
        int cObstacle;
        cin >> rObstacle >> cObstacle;
        // horizontal and vertical checks
        if (rObstacle == rQueen) {
            if (cObstacle > cQueen) {
                right = min (right, (cObstacle - 1) - cQueen);
            } else {
                left = min (left, cQueen - (cObstacle + 1));
            }
        } else if (cObstacle == cQueen) {
            if (rObstacle > rQueen) {
                up = min (up, (rObstacle - 1) - rQueen);
            } else {
                down = min (down, rQueen - (rObstacle + 1));
            }
        } else if ((rObstacle - cObstacle) == (rQueen - cQueen)) {// diagonal checks
            if (rObstacle > rQueen) {
                UR = min (UR, (cObstacle - 1) - cQueen);
            } else {
                DL = min (DL, cQueen - (cObstacle + 1));
            }
        } else if ((cObstacle + rObstacle) == (rQueen + cQueen)) {
            if (rObstacle > rQueen) {
                UL = min (UL, (rObstacle - 1) - rQueen);
            } else {
                DR = min (DR, rQueen - (rObstacle + 1));
            }
        }
    }
    
    cout << (up + down + left + right + UR + UL + DR + DL);
    return 0;
}
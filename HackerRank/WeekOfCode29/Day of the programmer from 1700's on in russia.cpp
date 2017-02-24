#include <bits/stdc++.h>

using namespace std;

int main(){
    int y;
    cin >> y;
    // your code goes here
    if (y > 1918) {
        int day = 13;
        if (y % 400 == 0 || ((y % 4 == 0) && (y % 100 != 0))) {
            day = 12;
        }
        cout << day << ".09." << y << endl;
    } else if (y < 1918) {
        int day = 13;
        if (y % 4 == 0) {
            day = 12;
        }
        cout << day << ".09." << y << endl;
    } else {
        cout << "26.09.1918" << endl;
    }
    return 0;
}
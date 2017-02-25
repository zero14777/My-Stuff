//find min value for abs(n/d - pi). for d's from min to max.
#include <bits/stdc++.h>

using namespace std;

int main(){
    long min;
    long max;
    cin >> min >> max;
    // your code goes here
    double pi = 3.1415926535897;
    double minDist = 3.15;
    int outputN = -1;
    int outputD = -1;
    for (double d = min; d <= max; d++) {
        double n = floor(pi*d);
        double dist = abs(n/d - pi);
        if (dist < minDist) {
            minDist = dist;
            outputN = n;
            outputD = d;
        }
        n++;
        dist = abs(n/d - pi);
        if (dist < minDist) {
            minDist = dist;
            outputN = n;
            outputD = d;
        }
    }
    cout << outputN << "/" << outputD;
    return 0;
}
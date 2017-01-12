/*
#!/bin/python3
#Given an -digit positive integer, count and print the number of multiples of  that can be formed by concatenating subsequences of the given number's digits, modulo .

#Input Format

#The first line contains an integer denoting . 
#The second line contains a string describing an -digit integer.
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
    int n;
    cin >> n;
    string number;
    cin >> number;
    
    //keep track of all possible subsequences with a third least sig digit that is odd or even
    unsigned long long odds = 0;
    unsigned long long evens = 0;

    //keep track of how many subsequences can be made using the various even digits
    unsigned long long acc08 = 0;
    unsigned long long acc6 = 0;
    unsigned long long acc4 = 0;
    unsigned long long acc2 = 0;

    //keep track of digit position we are looking at
    unsigned long long pos = 0;

    unsigned long long output = 0;

    for(int c = 0; c < number.length(); c++){
        if ((number.at(c) - 48) == 0 || (number.at(c) - 48) == 8) {
            output += 1 + acc08;
            acc08 += 1 + evens;
            acc4 += odds;
        } else if ((number.at(c) - 48) == 6) {
            output += acc6;
            acc08 += odds;
            acc4 += 1 + evens;
        } else if ((number.at(c) - 48) == 4) {
            output += acc4;
            acc08 += 1 + evens;
            acc4 += odds;
        } else if ((number.at(c) - 48) == 2) {
            output += acc2;
            acc08 += odds;
            acc4 += 1 + evens;
        } else if((number.at(c) - 48) == 3 || (number.at(c) - 48) == 7) {
            acc2 += 1 + evens;
            acc6 += odds;
        } else { //1 or 5 or 9
            acc2 += odds;
            acc6 += 1 + evens;
        }
        if ((number.at(c) - 48) % 2 == 1) {
            odds += ((unsigned long long) pow(2, c));
        } else {
            evens += ((unsigned long long) pow(2, c));
        }
        output = output % 1000000007;
    }

    cout << output;
    return 0;
}
/*
A weighted string is a string of lowercase English letters where each letter has a weight in the inclusive range from 1 to 26, defined below:

image

We define the following terms:

The weight of a string is the sum of the weights of all the string's characters. For example:

image

A uniform string is a string consisting of a single character repeated zero or more times. For example, ccc and a are uniform strings, but bcb and cd are not (i.e., they consist of more than one distinct character).
Given a string, s, let U be the set of weights for all possible uniform substrings (contiguous) of string s. You have to answer n queries, where each query i consists of a single integer, x. For each query, print Yes on a new line if x in set U; otherwise, print No instead.
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
    int charCount[27] = {0};
    string s;
    cin >> s;
    int lastChar = 0;
    int currCount = 0;
    for(int i = 0; i < s.length(); i++) {
        int currChar = s.at(i);
        currChar = currChar - 96;
        if (i == (s.length() - 1)) {
            if (currChar == lastChar) {
                currCount++;
                if (currCount > charCount[currChar]) {
                    charCount[currChar] = currCount;
                }
            } else {
                if (currCount > charCount[lastChar]) {
                    charCount[lastChar] = currCount;
                }
                if (charCount[currChar] < 1) {
                    charCount[currChar] = 1;
                }
            }
        } else if (currChar == lastChar) {
            currCount++;
        } else if (currCount > charCount[lastChar]) {
            charCount[lastChar] = currCount;
            currCount = 1;
            lastChar = currChar;
        } else {
            currCount = 1;
            lastChar = currChar;
        }
    }
    
    //for(int i = 1; i < 27; i++) {
    //    cout << charCount[i] << endl;
    //}
    
    int n;
    cin >> n;
    for(int a0 = 0; a0 < n; a0++){
        int x;
        string result = "No\n";
        cin >> x;
        for(int i = 1; i < 27; i++) {
            if (x%i == 0 && i*charCount[i] >= x) {
                result = "Yes\n";
                break;
            }
        }
        cout << result;
    }
    return 0;
}
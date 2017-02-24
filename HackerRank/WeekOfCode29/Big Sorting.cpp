/*
Your submission will run against only preliminary test cases. Full test cases will run at the end of the day.
Consider an array of numeric strings, unsorted, where each string is a positive number with anywhere from 1 to 10^6 digits. Sort the array's elements in non-decreasing (i.e., ascending) order of their real-world integer values and print each element of the sorted array on a new line.

Input Format

The first line contains an integer, n, denoting the number of strings in unsorted. 
Each of the n subsequent lines contains a string of integers describing an element of the array.

Constraints
1 <= n <= 2*10^5
Each string is guaranteed to represent a positive integer without leading zeros.
The total number of digits across all strings in unsorted is between 1 and 1000000 (inclusive).
Output Format

Print each element of the sorted array on a new line.
*/

#include <bits/stdc++.h>
#include <math.h>

using namespace std;

vector<string> myRadixSort(vector<string> strings){
    for (int pos = strings[0].size()-1; pos >= 0; pos--) {
        vector<string> temp;
        for (int grab = '0'; grab <= '9'; grab++){
            for (int count = 0; count < strings.size(); count++){
                if(strings[count].at(pos) == grab) {
                    temp.push_back(strings[count]);
                }
            }
        }
        strings = temp;//delete original?
    }
    return strings;
}

vector<string> mySort(vector<string> unsorted, int minSize, int maxSize) {
    if (unsorted.size() == 0) {
        return unsorted;
    }
    if (maxSize == minSize) {
        return myRadixSort(unsorted);
    }
    int midpoint = floor((maxSize + minSize)/2) + 1;
    vector<string> lower;
    vector<string> upper;
    for(int i = 0; i < unsorted.size(); i++) {
        if (unsorted[i].size() < midpoint) {
            lower.push_back(unsorted[i]);
        } else {
            upper.push_back(unsorted[i]);
        }
    }
    lower = mySort(lower, minSize, (midpoint-1));
    upper = mySort(upper, midpoint, maxSize);
    lower.insert( lower.end(), upper.begin(), upper.end() );
    return lower;
}

int main(){
    int n;
    cin >> n;
    vector<string> unsorted(n);
    for(int unsorted_i = 0; unsorted_i < n; unsorted_i++){
       cin >> unsorted[unsorted_i];
    }
    // your code goes here
    int maxSize = 0;
    int minSize = 10000000;
    for(int i = 0; i < n; i++){
       if(unsorted[i].size() > maxSize) {
           maxSize = unsorted[i].size();
       }
        if(unsorted[i].size() < minSize) {
           minSize = unsorted[i].size();
       }
    }
    vector<string> sorted = mySort(unsorted, minSize, maxSize);
    for(int i = 0; i < sorted.size(); i++) {
        cout << sorted[i] << endl;
    }
    return 0;
}
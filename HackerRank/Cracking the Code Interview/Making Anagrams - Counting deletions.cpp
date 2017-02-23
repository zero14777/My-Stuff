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

int number_needed(string a, string b) {
    int CharCount[26] = {0};
    int result = 0;
    for (int i = 0; i < a.size(); i++) {
        CharCount[a.at(i) - 'a']++;
    }
    for (int i = 0; i < b.size(); i++) {
        CharCount[b.at(i) - 'a']--;
    }
    for (int i = 0; i < 26; i++) {
        result += abs(CharCount[i]);
    }
    return result;
}

int main(){
    string a;
    cin >> a;
    string b;
    cin >> b;
    cout << number_needed(a, b) << endl;
    return 0;
}

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

bool ransom_note(vector<string> magazine, vector<string> ransom) {
    unordered_map<string, int> words;
    
    for (int i = 0; i < magazine.size(); i++){
        auto got = words.find(magazine.at(i));
        if (got == words.end()) {
            words.insert({magazine.at(i), 1});
        } else {
            got->second++;
        }
    }
    
    for (int i = 0; i < ransom.size(); i++){
        auto got = words.find(ransom.at(i));
        if (got == words.end()) {
            return false;
        } else {
            if (got->second <= 0 ){
                return false;
            }
            got->second = got->second - 1;
        }
    }
    return true;
}

int main(){
    int m;
    int n;
    cin >> m >> n;
    vector<string> magazine(m);
    for(int magazine_i = 0;magazine_i < m;magazine_i++){
       cin >> magazine[magazine_i];
    }
    vector<string> ransom(n);
    for(int ransom_i = 0;ransom_i < n;ransom_i++){
       cin >> ransom[ransom_i];
    }
    if(ransom_note(magazine, ransom))
        cout << "Yes\n";
    else
        cout << "No\n";
    return 0;
}


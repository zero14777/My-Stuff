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
    int lower_count = 0;
    int upper_count = 0;
    vector<int> a(n);
    
    for(int i = 0;i < n;i++){
        cin >> a[i];
    }
        
    std::priority_queue<int, std::vector<int>, std::less<int> > lower;
    std::priority_queue<int, std::vector<int>, std::greater<int> > upper;
    
    for(int i = 0;i < n;i++){
        if (lower_count > upper_count) {
            if (lower.top() <= a[i]) {
                upper.push(a[i]);
            } else {
                upper.push(lower.top());
                lower.pop();
                lower.push(a[i]);
            }
            upper_count++;
        } else {
            if (lower_count == 0) {
                lower.push(a[i]);
            } else if (upper.top() < a[i]) {
                lower.push(upper.top());
                upper.pop();
                upper.push(a[i]);
            } else {
                lower.push(a[i]);
            }
            lower_count++;
        }
        if (lower_count > upper_count) {
            cout << lower.top() << "." << 0 << endl;
        } else {
            cout << ((lower.top() + upper.top())/2) << "." << (((lower.top() + upper.top())%2)*5) << endl;
        }
    }
    return 0;
}

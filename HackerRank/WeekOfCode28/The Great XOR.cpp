/*Given a long integer, x, count the number of values of  satisfying the following conditions : (a xor x) > x && 0 < a < x

where a and x are long integers and  is the bitwise XOR operator.

You are given q queries, and each query is in the form of a long integer denoting x.For each query, print the total number of values of a satisfying the conditions above on a new line.

Input Format

The first line contains an integer, q, denoting the number of queries.
Each of the  subsequent lines contains a long integer describing the value of c for a query.
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
    int q;
    cin >> q;
    for(int a0 = 0; a0 < q; a0++){
        long x;
        cin >> x;
        // your code goes here
        long output = 0;
		long sig_fig = 1;
		while (sig_fig < x) {
			long check = sig_fig&x;
			if (check == 0) {
				output = output + sig_fig;
			}
			sig_fig = sig_fig * 2;
		}
		cout << output << "\n";
    }
    return 0;
}
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

void addIndirectFriend(int x, int y, std::array<std::vector<int>, 50000> *students) {
	if (x == y || std::find(students->at(x).begin(), students->at(x).end(), y) != students->at(x).end()) {
		return;
	}
	students->at(x).push_back(y);
	students->at(y).push_back(x);
	//cout << x << " gets friend " << y << "\n";
	//cout << y << " gets friend " << x << "\n";
	for (std::vector<int>::iterator it = students->at(y).begin(); it != students->at(y).end(); ++it) {
		addIndirectFriend(x, *it, students);
	}
}

void addFriend(int x, int y, std::array<std::vector<int>, 50000> *students) {
	if (x == y || std::find(students->at(x).begin(), students->at(x).end(), y) != students->at(x).end()) {
		return;
	}
	students->at(x).push_back(y);
	//cout << x << " gets friend " << y << "\n";
	for (std::vector<int>::iterator it = students->at(y).begin(); it != students->at(y).end(); ++it) {
		addIndirectFriend(x, *it, students);
	}
}

int main() {
	int t;
	cin >> t;
	for (int a0 = 0; a0 < t; a0++) {
		int n;
		int m;
		cin >> n >> m;

		int total = 0;
		array<std::vector<int>, 50000> students;
		for (int a1 = 0; a1 < m; a1++) {
			int x;
			int y;
			cin >> x >> y;

			addFriend(x, y, &students);
			addFriend(y, x, &students);

			for (int i = 0; i < n; i++) {
				total += students.at(i).size();
			}
			//cout << total << "\n";
		}
		cout << total << "\n";
	}
	return 0;
}
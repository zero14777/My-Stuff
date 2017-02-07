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

struct Node {
   Node* characters[26];
   int count;
};

Node* newNode () {
    Node* temp = new Node;
    temp->characters[26];
    temp->count = 0;
    return temp;
}

void addName (Node* root, string data) {
    root->count++;
    if (data.length() == 0) {
        return;
    }
    
    char temp = data.at(0);
    if (root->characters[temp-'a'] == NULL) {
        root->characters[temp-'a'] = newNode();
    }
    addName(root->characters[temp-'a'], data.substr(1));
}

int findCount (Node* root, string data) {
    if (data.length() == 0) {
        return root->count;
    }
    
    char temp = data.at(0);
    if (root->characters[temp-'a'] == NULL) {
        return 0;
    }
    return findCount(root->characters[temp-'a'], data.substr(1));
}

int main(){
    Node* root = newNode ();
    
    int n;
    cin >> n;
    for(int a0 = 0; a0 < n; a0++){
        string op;
        string contact;
        cin >> op >> contact;
        if (op == "add") {
            addName (root, contact);
        } else if (op == "find") {
            cout << findCount (root, contact) << endl;
        }
    }
    return 0;
}
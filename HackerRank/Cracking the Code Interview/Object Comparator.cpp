#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

struct Player {
    string name;
    int score;
}; 

struct sort_scores
{
    inline bool operator() (const Player& struct1, const Player& struct2)
    {
        if (struct1.score != struct2.score) {
            return (struct1.score > struct2.score);
        } else {
            return struct1.name.compare(struct2.name) < 0;
        }
    }
};

vector<Player> comparator(vector<Player> players) {
    sort( players.begin(), players.end() , sort_scores() ); 
    return players;
}

int main() {
    
    int n;
    cin >> n;
    vector< Player > players;
    string name;
    int score;
    for(int i = 0; i < n; i++){
        cin >> name >> score;
        Player p1;
        p1.name = name, p1.score = score;
        players.push_back(p1);
    }
    
    vector<Player > answer = comparator(players);
    for(int i = 0; i < answer.size(); i++) {
        cout << answer[i].name << " " << answer[i].score << endl;
    }
    return 0;
}

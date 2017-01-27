/*
Abstract base classes in C++ can only be used as base classes. Thus, they are allowed to have virtual member functions without definitions.

A cache is a component that stores data so future requests for that data can be served faster. The data stored in a cache might be the results of an earlier computation, or the duplicates of data stored elsewhere. A cache hit occurs when the requested data can be found in a cache, while a cache miss occurs when it cannot. Cache hits are served by reading data from the cache which is faster than recomputing a result or reading from a slower data store. Thus, the more requests that can be served from the cache, the faster the system performs.

One of the popular cache replacement policies is: "least recently used" (LRU). It discards the least recently used items first.

For example, if a cache with a capacity to store 5 keys has the following state(arranged from most recently used key to least recently used key) -

5 3 2 1 4
Now, If the next key comes as 1(which is a cache hit), then the cache state in the same order will be -

1 5 3 2 4
Now, If the next key comes as 6(which is a cache miss), then the cache state in the same order will be -

6 1 5 3 2
You can observe that 4 has been discarded because it was the least recently used key and since the capacity of cache is 5, it could not be retained in the cache any longer.

Given an abstract base class Cache with member variables and functions: 

mp - Map the key to the node in the linked list
cp - Capacity
tail - Double linked list tail pointer
head - Double linked list head pointer
set() - Set/insert the value off the key, if present, otherwise add the key as the most recently used key. If the cache has reached its capacity, it should replace the least recently used key with a new key.
get() - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1. Then make that key as the most recently used key.

You have to write a class LRUCache which extends the class Cache and uses the member functions and variables to implement an LRU cache.
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node {
	Node* next;
	Node* prev;
	int value;
	int key;
	Node(Node* p, Node* n, int k, int val) :prev(p), next(n), key(k), value(val) {};
	Node(int k, int val) :prev(NULL), next(NULL), key(k), value(val) {};
};

class Cache {

protected:
	map<int, Node*> mp; //map the key to the node in the linked list
	int cp;  //capacity
	Node* tail; // double linked list tail pointer
	Node* head; // double linked list head pointer
	virtual void set(int, int) = 0; //set function
	virtual int get(int) = 0; //get function

};


//// MY CODE START
class LRUCache: public Cache {
    public:
    LRUCache(int capacity) {
        cp = capacity;
        mp.clear();
        head = NULL;
        tail = NULL;
    }
    
    ~LRUCache(void) {
        for (map<int, Node*>::iterator it = mp.begin(); it != mp.end(); ++it)
        {
            free(it->second);
        }
    }
    
    void set(int key, int value) {
        Node* newNode = new Node(key, value);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else { // check and remove existing node in LRUCache
            Node* curr = head;
            while (true) {
                if (curr == NULL) {
                    break;
                }
                if (curr->key == key) {
                    if (curr->prev == NULL) {
                        if (curr->next == NULL) {
                            head = NULL;
                        } else {
                            head = curr->next;
                        }
                    } else {
                        Node* temp = curr->prev;
                        curr->prev->next = curr->next;
                        if (curr->next != NULL) {
                            curr->next->prev = temp;
                        }
                    }

                    if (curr->next == NULL) {
                        if (curr->prev == NULL) {
                            tail = NULL;
                        } else {
                            tail = curr->prev;
                        }
                    } else {
                        Node* temp = curr->next;
                        curr->next->prev = curr->prev;
                        if (curr->prev != NULL) {
                            curr->prev->next = temp;
                        }
                    }
                    //free(curr);
                }
                if (curr->next == NULL) {
                    curr = NULL;
                } else {
                    curr = curr->next;
                }
            }
        }
        
        // keep Cache at or below capacity
        newNode->next = head;
        if (head != newNode && head != NULL) {
            head->prev = newNode;
        }
        head = newNode;
        Node* curr = head;
        for (int i = 0; i < cp; i++) {
            if (curr->next == NULL) {
                break;
            } else if (i == (cp-1)) {
                curr->next = NULL;
                tail = curr;
            }
            curr = curr->next;
        }
        
        try {
            free(mp.at(key));
        } catch (std::out_of_range) {
        }
        mp.erase (key);
        mp.insert ( std::pair<int,Node*>(key,newNode) );
    }
    
    int get(int key) {
        if (head != NULL) {
            Node* curr = head;
            while (true) {
                if (curr->key == key) {
                    return curr->value;
                }
                if (curr == tail) {
                    break;
                }
                curr = curr->next;
            }
        }
        
        try {
            return mp.at(key)->value;
        } catch (std::out_of_range) {
            return -1;
        }
    }
};
//// MY CODE END


int main() {
	int n, capacity, i;
	cin >> n >> capacity;
	LRUCache l(capacity);
	for (i = 0; i<n; i++) {
		string command;
		cin >> command;
		if (command == "get") {
			int key;
			cin >> key;
			cout << l.get(key) << endl;
		}
		else if (command == "set") {
			int key, value;
			cin >> key >> value;
			l.set(key, value);
		}
	}
	return 0;
}

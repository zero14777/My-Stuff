/*
Detect a cycle in a linked list. Note that the head pointer may be 'NULL' if the list is empty.

A Node is defined as: 
    struct Node {
        int data;
        struct Node* next;
    }
*/

bool has_cycle(Node* head) {
    // Complete this function
    // Do not write the main method
    Node* curr = head;
    Node* mark = NULL;
    int dist = 1;
    int rounds = 1;
    
    if (head == NULL) {
        return false;
    }
    
    while(1) {
        dist--;
        if (dist == 0) {
            mark = curr;
            dist = pow(2, rounds);
            rounds++;
        }
        curr = curr->next;
        if (curr == NULL) {
            break;
        }
        if (curr == mark) {
            return true;
        }
    }
    return false;
}
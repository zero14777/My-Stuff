/* Hidden stub code will pass a root argument to the function below. Complete the function to solve the challenge. Hint: you may want to write one or more helper functions.  

The Node struct is defined as follows:
   struct Node {
      int data;
      Node* left;
      Node* right;
   }
*/
   bool checkBST(Node* root) {
      if (checkBSTrec(root, -1, 10001)) {
          return true;
      } else {
          return false;
      }
   }

   bool checkBSTrec(Node* root, int min, int max) {
       if (root == NULL) {
           return true;
       }
       
       bool return_val = true;
       if (root->data <= min || root->data >= max) {
           return_val = false;
       }
       
       if (!(checkBSTrec(root->left, min, root->data)) || !(checkBSTrec(root->right, root->data, max))) {
           return_val = false;
       }
       
       return return_val;
   }
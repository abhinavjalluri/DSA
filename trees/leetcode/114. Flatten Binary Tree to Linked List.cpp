// Given the root of a binary tree, flatten the tree into a "linked list":

// The "linked list" should use the same TreeNode class where the right child pointer points to the next node in the list and the left child pointer is always null.
// The "linked list" should be in the same order as a pre-order traversal of the binary tree.
 

// Example 1:


// Input: root = [1,2,5,3,4,null,6]
// Output: [1,null,2,null,3,null,4,null,5,null,6]
// Example 2:

// Input: root = []
// Output: []
// Example 3:

// Input: root = [0]
// Output: [0]
 

// Constraints:

// The number of nodes in the tree is in the range [0, 2000].
// -100 <= Node.val <= 100

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    void preorder(TreeNode* root, vector<TreeNode*>& nodes){
        if(!root){
            return;
        }
        nodes.push_back(root);
        preorder(root->left, nodes);
        preorder(root->right, nodes);
    }
    void flatten(TreeNode* root) {
        if(!root){
            return;
        }
        vector<TreeNode*> nodes;
        preorder(root, nodes);
        for(int i = 0 ; i < nodes.size()-1; i++){
            nodes[i]->left = NULL;
            nodes[i]->right = nodes[i+1];
        }
    }
};


// Approach 2: In-Place (Optimized Morris-like Traversal)
// Start from the root node.

// If the current node has a left child:

// Find the rightmost node of the left subtree.
// Attach that node’s right to the current node’s right subtree.
// Move the left subtree to the right and set left = null.
// Move to the right child and repeat.

class Solution {
public:
    void flatten(TreeNode* root) {
        TreeNode* curr = root;
        while(curr){
            if(curr->left){
                TreeNode* temp = curr->left;  
                while(temp->right){
                    temp = temp->right;
                }
                temp->right = curr->right;
                curr->right = curr->left;
                curr->left = NULL;
            }
            curr = curr->right;
        }
    }
};
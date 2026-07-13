// Given the root of a Binary Search Tree (BST), return the minimum absolute difference between the values of any two different nodes in the tree.

 

// Example 1:


// Input: root = [4,2,6,1,3]
// Output: 1
// Example 2:


// Input: root = [1,0,48,null,null,12,49]
// Output: 1
 

// Constraints:

// The number of nodes in the tree is in the range [2, 104].
// 0 <= Node.val <= 105

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

    // DFS Solution Inorder Traversal : 1

    void f(vector<int>&sortedList, TreeNode* root){
        if(!root){
            return;
        }
        f(sortedList, root->left);
        sortedList.push_back(root->val);
        f(sortedList, root->right);
    }
    int getMinimumDifference(TreeNode* root) {
        vector<int>sortedList;
        f(sortedList, root);
        int mini = INT_MAX;
        for(int i = 1 ; i < sortedList.size() ; i++){
            mini = min(mini, abs(sortedList[i]- sortedList[i-1]));
        }
        return mini;
    }

    // DFS Solution Inorder Traversal : 2
    
    void f(TreeNode* root, TreeNode*& prev, int& mini){
        if(!root){
            return;
        }
        f(root->left, prev, mini);
        if(prev){
            mini = min(mini, root->val - prev->val);
        }
        prev = root;
        f(root->right, prev, mini);
    }
    int getMinimumDifference(TreeNode* root) {
        int mini = INT_MAX;
        TreeNode* prev = NULL;
        f(root, prev, mini);
        return mini;
    }

};
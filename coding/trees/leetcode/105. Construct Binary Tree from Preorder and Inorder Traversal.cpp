// Given two integer arrays preorder and inorder where preorder is the preorder traversal of a binary tree and inorder is the inorder traversal of the same tree, construct and return the binary tree.

 

// Example 1:


// Input: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
// Output: [3,9,20,null,null,15,7]
// Example 2:

// Input: preorder = [-1], inorder = [-1]
// Output: [-1]
 

// Constraints:

// 1 <= preorder.length <= 3000
// inorder.length == preorder.length
// -3000 <= preorder[i], inorder[i] <= 3000
// preorder and inorder consist of unique values.
// Each value of inorder also appears in preorder.
// preorder is guaranteed to be the preorder traversal of the tree.
// inorder is guaranteed to be the inorder traversal of the tree.


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

//  To build the binary tree:

// Preorder gives the root first, so we process elements from preorder in order.
// Inorder helps us split the tree into left and right subtrees based on the root's position.
// So, the first element from preorder is the root. Find its index in inorder to know where left and right subtrees start and end. Recur for both parts.

// 🧩 Approach
// Store inorder values and their indices in a map for O(1) lookup.

// Maintain a global index to track the current root in preorder.

// Use recursion (helper) with start and end to define the current subtree range.

// At each step:

// Pick the root from preorder[index].
// Find its position in inorder to divide into left and right.
// Recurse on left and right subtrees.
// 📊 Complexity
// Time complexity: O(n) — Each node is processed once.
// Space complexity: O(n) — For hashmap and recursion stack.

class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        unordered_map<int,int>mpp;
        for(int i = 0 ; i < inorder.size(); i++){
            mpp[inorder[i]] = i;
        }
        int index = 0;
        return f(preorder, mpp, 0, inorder.size()-1, index);
    }
    TreeNode* f(vector<int>& preorder, unordered_map<int,int>& mpp, int start, int end, int& index){
        if(start > end){
            return NULL;
        }
        int rootVal = preorder[index];
        index++;
        TreeNode* root = new TreeNode(rootVal);
        int mid = mpp[rootVal];
        root->left = f(preorder, mpp, start, mid-1, index);
        root->right = f(preorder, mpp, mid+1, end, index);
        return root;

    }
};
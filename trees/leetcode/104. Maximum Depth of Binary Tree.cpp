// Given the root of a binary tree, return its maximum depth.

// A binary tree's maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.

 

// Example 1:


// Input: root = [3,9,20,null,null,15,7]
// Output: 3
// Example 2:

// Input: root = [1,null,2]
// Output: 2
 

// Constraints:

// The number of nodes in the tree is in the range [0, 104].
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
    int maxDepth(TreeNode* root) {
        if(root == NULL){
            return 0;
        }
        // DFS
        return 1 + max(maxDepth(root->left), maxDepth(root->right));

        // BFS
        // queue<TreeNode*> q;
        // q.push(root);
        // int depth = 0;
        // while(!q.empty()){
        //     depth++;

        //     int levelSize = q.size();

        //     for(int i = 0 ; i < levelSize; i++){
        //         TreeNode* node = q.front();
        //         q.pop();

        //         if(node->left){
        //             q.push(node->left);
        //         }
        //         if(node->right){
        //             q.push(node->right);
        //         }
        //     }
        // }
        // return depth;
    }
};
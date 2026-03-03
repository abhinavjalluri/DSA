// Given the root of a binary tree, check whether it is a mirror of itself (i.e., symmetric around its center).

 

// Example 1:


// Input: root = [1,2,2,3,4,4,3]
// Output: true
// Example 2:


// Input: root = [1,2,2,null,3,null,3]
// Output: false
 

// Constraints:

// The number of nodes in the tree is in the range [1, 1000].
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

// DFS Solution : 1
class Solution {
public:
    bool f(TreeNode* l, TreeNode* r) {
        if(!l && !r){
            return true;
        }
        else if(!l || !r || l->val != r->val){
            return false;
        }
        return f(l->left, r->right) && f(l->right, r->left);
    }
    bool isSymmetric(TreeNode* root) {
        if(!root){
            return true;
        }
        return f(root->left, root->right);
    }
};

// BFS Solution : 2

class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if(!root){
            return true;
        }
        queue<TreeNode*>q;
        q.push(root->left);
        q.push(root->right);
        while(!q.empty()){
            TreeNode* l = q.front();
            q.pop();
            TreeNode* r = q.front();
            q.pop();
            if(!r && !l){
                continue;
            }
            else if(!l || !r || l->val != r->val){
                return false;
            }

            q.push(l->left);
            q.push(r->right);

            q.push(l->right);
            q.push(r->left);

        }
        return true;
    }
};

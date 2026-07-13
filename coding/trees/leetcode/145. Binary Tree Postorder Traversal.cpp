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

    // DFS Solution : 1
    
    void f(vector<int>&ans, TreeNode* root){
        if(!root){
            return;
        }
        f(ans, root->left);
        f(ans, root->right);
        ans.push_back(root->val);
    }
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int>ans;
        f(ans, root);
        return ans;
    }

    // Iterative Solution : 2

    vector<int> postorderTraversal(TreeNode* root) {
        vector<int>ans;
        if(!root){
            return ans;
        }
        stack<TreeNode*>st;
        TreeNode* node = root;
        while(node || !st.empty()){
            while(node){
                ans.push_back(node->val);
                st.push(node);
                node = node->right;
            }
            node = st.top();
            st.pop();
            node = node->left;
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }

    
};
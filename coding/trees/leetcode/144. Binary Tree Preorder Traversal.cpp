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
        if(root){
            ans.push_back(root->val);
            f(ans, root->left);
            f(ans, root->right);
        }
    }
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int>ans;
        f(ans, root);
        return ans;
    }

    // Iterative Solution : 2
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int>ans;
        if(!root){
            return ans;
        }
        stack<TreeNode*>st;
        st.push(root);
        while(!st.empty()){
            TreeNode* node = st.top();
            st.pop();
            ans.push_back(node->val);
            if(node->right)
            st.push(node->right);
            if(node->left)
            st.push(node->left);
        }
        return ans;
    }
};
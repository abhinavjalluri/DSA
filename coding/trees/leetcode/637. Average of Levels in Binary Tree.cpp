// Given the root of a binary tree, return the average value of the nodes on each level in the form of an array. Answers within 10-5 of the actual answer will be accepted.
 

// Example 1:


// Input: root = [3,9,20,null,null,15,7]
// Output: [3.00000,14.50000,11.00000]
// Explanation: The average value of nodes on level 0 is 3, on level 1 is 14.5, and on level 2 is 11.
// Hence return [3, 14.5, 11].
// Example 2:


// Input: root = [3,9,20,15,7]
// Output: [3.00000,14.50000,11.00000]
 

// Constraints:

// The number of nodes in the tree is in the range [1, 104].
// -231 <= Node.val <= 231 - 1

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

    // DFS
    void f(TreeNode* root, vector<double>& sum, vector<int>& count, int level){
        if(!root) return;

        // If this level is visited first time
        if(level == sum.size()){
            sum.push_back(0);
            count.push_back(0);
        }

        sum[level] += root->val;
        count[level]++;

        f(root->left, sum, count, level + 1);
        f(root->right, sum, count, level + 1);
    }

    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> sum;
        vector<int> count;

        f(root, sum, count, 0);

        vector<double> ans;
        for(int i = 0; i < sum.size(); i++){
            ans.push_back(sum[i] / count[i]);
        }

        return ans;
    }

    // BFS

    vector<double> averageOfLevels(TreeNode* root) {
        vector<double> res;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()) {
            long temp=0;
            int s=q.size();
            for(int i=0;i<s;i++) {
                TreeNode* t=q.front();
                q.pop();
                if(t->left) q.push(t->left);
                if(t->right) q.push(t->right);
                temp+=t->val;
            }
            res.push_back((double)temp/s);
        }
        return res;
    }
};
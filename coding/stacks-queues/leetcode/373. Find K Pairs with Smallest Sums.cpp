// You are given two integer arrays nums1 and nums2 sorted in non-decreasing order and an integer k.

// Define a pair (u, v) which consists of one element from the first array and one element from the second array.

// Return the k pairs (u1, v1), (u2, v2), ..., (uk, vk) with the smallest sums.

 

// Example 1:

// Input: nums1 = [1,7,11], nums2 = [2,4,6], k = 3
// Output: [[1,2],[1,4],[1,6]]
// Explanation: The first 3 pairs are returned from the sequence: [1,2],[1,4],[1,6],[7,2],[7,4],[11,2],[7,6],[11,4],[11,6]
// Example 2:

// Input: nums1 = [1,1,2], nums2 = [1,2,3], k = 2
// Output: [[1,1],[1,1]]
// Explanation: The first 2 pairs are returned from the sequence: [1,1],[1,1],[1,2],[2,1],[1,2],[2,2],[1,3],[1,3],[2,3]
 

// Constraints:

// 1 <= nums1.length, nums2.length <= 105
// -109 <= nums1[i], nums2[i] <= 109
// nums1 and nums2 both are sorted in non-decreasing order.
// 1 <= k <= 104
// k <= nums1.length * nums2.length

class Solution {
public:
    struct Compare{
        bool operator()(vector<int>a, vector<int>b){
            return a[0] > b[0];
        }
    };
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        priority_queue<vector<int>, vector<vector<int>>, Compare>pq;
        int n = nums1.size(), m = nums2.size();
        vector<vector<int>> ans;
        for(int i = 0; i < min(n,k); i++){
            pq.push({nums1[i]+nums2[0], i, 0});
        }
        while(!pq.empty() && k--)
        {
            vector<int> tp = pq.top();
            int x = tp[1], y = tp[2];
            pq.pop();
            ans.push_back({nums1[x], nums2[y]});
            if(y != m-1)
            pq.push({nums1[x]+nums2[y+1], x, y+1});
        }
        return ans;
    }
};

// pair<int,pair<int,int>> approach

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
    priority_queue<pair<int,pair<int,int>>, vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>> pq;
        int n = nums1.size(), m = nums2.size();
        vector<vector<int>> ans;
        for(int i = 0; i < n; i++)
            pq.push({nums1[i]+nums2[0],{i,0}});
        while(!pq.empty() && k--)
        {
            pair<int,pair<int,int>> tp = pq.top();
            int x = tp.second.first, y = tp.second.second;
            pq.pop();
            ans.push_back({nums1[x], nums2[y]});
            if(y != m-1)
            pq.push({nums1[x]+nums2[y+1],{x, y+1}});
        }
        return ans;
        
    }
};

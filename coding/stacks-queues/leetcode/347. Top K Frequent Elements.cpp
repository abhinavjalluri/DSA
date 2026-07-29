// Given an integer array nums and an integer k, return the k most frequent elements. You may return the answer in any order.

 

// Example 1:

// Input: nums = [1,1,1,2,2,3], k = 2

// Output: [1,2]

// Example 2:

// Input: nums = [1], k = 1

// Output: [1]

// Example 3:

// Input: nums = [1,2,1,2,1,2,3,1,3,2], k = 2

// Output: [1,2]

 

// Constraints:

// 1 <= nums.length <= 105
// -104 <= nums[i] <= 104
// k is in the range [1, the number of unique elements in the array].
// It is guaranteed that the answer is unique.
 

// Follow up: Your algorithm's time complexity must be better than O(n log n), where n is the array's size.

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int>mpp;
        int n = nums.size();
        for(int i = 0 ; i < n ; i++){
            mpp[nums[i]]++;
        }
        vector<int>ans;
        priority_queue<pair<int,int>>pq;
        for(auto i : mpp){
            pq.push({i.second, i.first});
        }
        while(k && !pq.empty()){
            ans.push_back(pq.top().second);
            pq.pop();
            k--;
        }
        return ans;
    }
};

// Another approach using min heap of size k

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int>mpp;
        int n = nums.size();
        for(int i = 0 ; i < n ; i++){
            mpp[nums[i]]++;
        }
        vector<int>ans;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>>pq;
        for(auto i : mpp){
            pq.push({i.second, i.first});
            if(pq.size() > k){
                pq.pop();
            }
        }
        while(!pq.empty()){
            ans.push_back(pq.top().second);
            pq.pop();
        }
        return ans;
    }
};
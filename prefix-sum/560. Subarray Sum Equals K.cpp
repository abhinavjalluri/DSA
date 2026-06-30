// Given an array of integers nums and an integer k, return the total number of subarrays whose sum equals to k.

// A subarray is a contiguous non-empty sequence of elements within an array.

 

// Example 1:

// Input: nums = [1,1,1], k = 2
// Output: 2
// Example 2:

// Input: nums = [1,2,3], k = 3
// Output: 2
 

// Constraints:

// 1 <= nums.length <= 2 * 104
// -1000 <= nums[i] <= 1000
// -107 <= k <= 107
 
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int,int>prefixSumMap;
        // Initialize the map with prefixSumMap[0] = 1 to handle the case where a subarray starting from index 0 sums to k.
        prefixSumMap[0] = 1;
        int sum = 0;
        int count = 0;
        for(int i = 0; i < n ; i++){
            sum += nums[i];
            // Check if there is a prefix sum that, when subtracted from the current sum, equals k. If so, it means there exists a subarray that sums to k.
            if(prefixSumMap.count(sum-k) > 0){
                count += prefixSumMap[sum-k];
            }
            prefixSumMap[sum]++;
        }
        return count;
    }
};
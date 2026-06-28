// You are given an integer array nums and an integer k. Find the maximum subarray sum of all the subarrays of nums that meet the following conditions:

// The length of the subarray is k, and
// All the elements of the subarray are distinct.
// Return the maximum subarray sum of all the subarrays that meet the conditions. If no subarray meets the conditions, return 0.

// A subarray is a contiguous non-empty sequence of elements within an array.

 

// Example 1:

// Input: nums = [1,5,4,2,9,9,9], k = 3
// Output: 15
// Explanation: The subarrays of nums with length 3 are:
// - [1,5,4] which meets the requirements and has a sum of 10.
// - [5,4,2] which meets the requirements and has a sum of 11.
// - [4,2,9] which meets the requirements and has a sum of 15.
// - [2,9,9] which does not meet the requirements because the element 9 is repeated.
// - [9,9,9] which does not meet the requirements because the element 9 is repeated.
// We return 15 because it is the maximum subarray sum of all the subarrays that meet the conditions
// Example 2:

// Input: nums = [4,4,4], k = 3
// Output: 0
// Explanation: The subarrays of nums with length 3 are:
// - [4,4,4] which does not meet the requirements because the element 4 is repeated.
// We return 0 because no subarrays meet the conditions.
 

// Constraints:

// 1 <= k <= nums.length <= 105
// 1 <= nums[i] <= 105


class Solution {
public:
    long long maximumSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> frequencyMap;
        set<int>s;
        long long maxi = 0;
        long long sum = 0;
        int left = 0;
        int right = 0;
        while(right < n){
            sum += nums[right];
            frequencyMap[nums[right]]++;
            if(right-left+1 > k){
                sum -= nums[left];
                frequencyMap[nums[left]]--;
                if(frequencyMap[nums[left]] == 0){
                  frequencyMap.erase(nums[left]);
                }
                left++;
            }
            if(right-left+1 == k && frequencyMap.size() == k){
                maxi = max(maxi,sum);
            }
            right++;
            // 
            // Another approach using set to check for distinct elements
            //
            // while(s.count(nums[right]) || s.size() == k){
            //     sum -= nums[left];
            //     s.erase(nums[left]);
            //     left++;
            // }
            // sum += nums[right];
            // s.insert(nums[right]);
            // if(s.size() == k){
            //     maxi = max(sum, maxi);
            // }
            // right++;
        }
        return maxi;
    }
};

// Another approach using unordered_map to check for distinct elements

class Solution {
public:
    long long maximumSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> frequencyMap;
        long long maxi = 0;
        long long sum = 0;
        int left = 0;
        int right = 0;
        while(right < n){
            sum += nums[right];
            frequencyMap[nums[right]]++;
            if(right-left+1 == k){
                if(frequencyMap.size() == k){
                    maxi = max(maxi,sum);
                }
                sum -= nums[left];
                frequencyMap[nums[left]]--;
                if(frequencyMap[nums[left]] == 0){
                    frequencyMap.erase(nums[left]);
                }
                left++;
            }
            right++;
        }
        return maxi;
    }
};
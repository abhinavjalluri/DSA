// Given an integer array nums and an integer k, return true if nums has a good subarray or false otherwise.

// A good subarray is a subarray where:

// its length is at least two, and
// the sum of the elements of the subarray is a multiple of k.
// Note that:

// A subarray is a contiguous part of the array.
// An integer x is a multiple of k if there exists an integer n such that x = n * k. 0 is always a multiple of k.
 

// Example 1:

// Input: nums = [23,2,4,6,7], k = 6
// Output: true
// Explanation: [2, 4] is a continuous subarray of size 2 whose elements sum up to 6.
// Example 2:

// Input: nums = [23,2,6,4,7], k = 6
// Output: true
// Explanation: [23, 2, 6, 4, 7] is an continuous subarray of size 5 whose elements sum up to 42.
// 42 is a multiple of 6 because 42 = 7 * 6 and 7 is an integer.
// Example 3:

// Input: nums = [23,2,6,4,7], k = 13
// Output: false
 

// Constraints:

// 1 <= nums.length <= 105
// 0 <= nums[i] <= 109
// 0 <= sum(nums[i]) <= 231 - 1
// 1 <= k <= 231 - 1

class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int,int>remainderIndex;
        // Initialize the map with remainderIndex[0] = -1 to handle subarrays starting at index 0 correctly.
        remainderIndex[0] = -1;
        int sum = 0;
        for(int i = 0 ; i < n ; i++){
            sum += nums[i];
            int remainder = sum % k;
            if(remainderIndex.find(remainder) != remainderIndex.end()){
                // If the same remainder has been seen before, check if the distance between indices is at least 2
                if(i - remainderIndex[remainder] > 1){
                  return true;
                }
            }
            // Why do we store only the first occurrence? 
            // Because we want to find a subarray of at least length 2. If we store the last occurrence, we might end up with a subarray of length 1, which is not valid.
            // Because we want the largest possible distance.
            else{
               remainderIndex[remainder] = i;
            }
        }
        return false;
    }
};

// Final Algorithm
// Compute a running prefix sum.
// Compute remainder = prefixSum % k.
// Store the first index where each remainder appears.
// Initialize the map with remainderIndex[0] = -1 so subarrays starting at index 0 are handled correctly.
// Whenever the same remainder is seen again, if the distance between indices is at least 2, return true.
// If the loop ends without finding such a pair, return false.

// A % k = r
// B % k = r
// A = x*k + r
// B = y*k + r
// A - B = (x-y)*k
// So, (A - B) % k = 0
// A - B is always divisible by k.
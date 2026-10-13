// Given a non-empty array of integers nums, every element appears twice except for one. Find that single one.

// You must implement a solution with a linear runtime complexity and use only constant extra space.

 

// Example 1:

// Input: nums = [2,2,1]

// Output: 1

// Example 2:

// Input: nums = [4,1,2,1,2]

// Output: 4

// Example 3:

// Input: nums = [1]

// Output: 1

 

// Constraints:

// 1 <= nums.length <= 3 * 104
// -3 * 104 <= nums[i] <= 3 * 104
// Each element in the array appears twice except for one element which appears only once.


// Approach 1: Using XOR operation to achieve O(n) time complexity and O(1) space complexity.

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0;

        for (int num : nums) {
            ans ^= num;
        }

        return ans;
    }
};

// Another approach using hash map to achieve O(n) time complexity and O(n) space complexity.

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> freq;

        for (int num : nums) {
            freq[num]++;
        }

        for (auto& [num, count] : freq) {
            if (count == 1) {
                return num;
            }
        }

        return -1;
    }
};

// Another approach using Mathematical formula with hash set to achieve O(n) time complexity and O(n) space complexity.

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_set<int> st(nums.begin(), nums.end());

        long long distinctSum = 0, totalSum = 0;

        for (int num : st) {
            distinctSum += num;
        }

        for (int num : nums) {
            totalSum += num;
        }

        return 2 * distinctSum - totalSum;
    }
};


// Another approach using sorting to achieve O(n log n) time complexity and O(1) space complexity.

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int n = nums.size();
        for(int i = 0;i<n-1;i++){
            if(nums[i] != nums[i+1]){
                return nums[i];
            }
            i++;
        }
        return nums[n-1];
    }
};
// Given an unsorted array of integers nums, return the length of the longest consecutive elements sequence.

// You must write an algorithm that runs in O(n) time.

 

// Example 1:

// Input: nums = [100,4,200,1,3,2]
// Output: 4
// Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.
// Example 2:

// Input: nums = [0,3,7,2,5,8,4,6,0,1]
// Output: 9
// Example 3:

// Input: nums = [1,0,1,2]
// Output: 3
 

// Constraints:

// 0 <= nums.length <= 105
// -109 <= nums[i] <= 109

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        if(n == 0){
            return 0;
        }
        int maxi = 1;
        int start = 1;
        for(int i = 1; i < n ; i++){
            maxi = max(maxi, start);
            if(nums[i] == nums[i-1]+1){
                start++;
            }
            else if(nums[i] == nums[i-1]){
                continue;
            }
            else{
                start = 1;
            }
        }
        return max(maxi, start);
    }
};

// Given an unsorted array of integers nums, return the length of the longest consecutive elements sequence.
// Another approach using hash set to achieve O(n) time complexity.
 
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> values(nums.begin(), nums.end());
        int longest = 0;

        for (int num : values) {
            // Only start counting at the beginning of a sequence.
            if (values.find(num - 1) != values.end()) {
                continue;
            }

            int current = num;
            int length = 1;

            while (values.find(current + 1) != values.end()) {
                current++;
                length++;
            }

            longest = max(longest, length);
        }

        return longest;
    }
};
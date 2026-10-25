// Given an integer array of size n, find all elements that appear more than ⌊n / 3⌋ times.

 

// Example 1:

// Input: nums = [3,2,3]
// Output: [3]
// Example 2:

// Input: nums = [1]
// Output: [1]
// Example 3:

// Input: nums = [1,2]
// Output: [1,2]
 

// Constraints:

// 1 <= nums.length <= 5 * 104
// -109 <= nums[i] <= 109
 

// Follow up: Could you solve the problem in linear time and in O(1) space?

class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        int n = nums.size();

        int candidate1 = 0;
        int candidate2 = 0;
        int count1 = 0;
        int count2 = 0;

        for(int i = 0 ; i < n ; i++){
            if(candidate1 == nums[i] && count1 > 0){
                count1++;
            }
            else if(candidate2 == nums[i] && count2 > 0){
                count2++;
            }
            else if(count1 == 0){
                candidate1 = nums[i];
                count1 = 1;
            }
            else if(count2 == 0){
                candidate2 = nums[i];
                count2 = 1;
            }
            else{
                count1--;
                count2--;
            }
        }
        count1 = 0;
        count2 = 0;
        for(int i = 0 ; i < n ; i++){
            if(nums[i] == candidate1){
                count1++;
            }
            else if(nums[i] == candidate2){
                count2++;
            }
        }
        vector<int>ans;
        if(count1 > n/3){
            ans.push_back(candidate1);
        }
        if(count2 > n/3){
            ans.push_back(candidate2);
        }
        return ans;
    }
};
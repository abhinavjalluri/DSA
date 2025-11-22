// User function Template for C++
// Given an array arr[] and an integer target. You have to return the pair of elements which sum upto target. You cannot use the same element twice.
// Note: Inputs are given such that only one valid answer exists.

// Examples:

// Input: arr[] = [2, 9, 10, 4, 15], target = 12
// Output: [2, 10]
// Explanation: Pair with sum equal to 12 is (2, 10).
class Solution {
  public:
    vector<int> twoSum(vector<int> &arr, int target) {
        int n = arr.size();
        unordered_map<int, bool>mpp;
        for(int i = 0;i<n;i++){
            int rem = target-arr[i];
            if(mpp.find(rem) != mpp.end()){
                return {rem, arr[i]};
            }
            mpp[arr[i]] = true;
        }
        return {};
    }
};
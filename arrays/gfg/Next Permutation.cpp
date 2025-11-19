// Given an array of integers arr[] representing a permutation, implement the next permutation that rearranges the numbers into the lexicographically next greater permutation. If no such permutation exists, rearrange the numbers into the lowest possible order (i.e., sorted in ascending order). 

// Note:  A permutation of an array of integers refers to a specific arrangement of its elements in a sequence or linear order.

// Examples:

// Input: arr[] = [2, 4, 1, 7, 5, 0]
// Output: [2, 4, 5, 0, 1, 7]
// Explanation: The next permutation of the given array is [2, 4, 5, 0, 1, 7].
// Input: arr[] = [3, 2, 1]
// Output: [1, 2, 3]
// Explanation: As arr[] is the last permutation, the next permutation is the lowest one.
// Input: arr[] = [3, 4, 2, 5, 1]
// Output: [3, 4, 5, 1, 2]
// Explanation: The next permutation of the given array is [3, 4, 5, 1, 2].

class Solution {
  public:
    void nextPermutation(vector<int>& arr) {
        // code here
        int n = arr.size();
        int swap_index = -1;
        for(int i = n-2; i>=0; i--){
            if(arr[i] < arr[i+1]){
                swap_index = i;
                break;
            }
            
        }
        if(swap_index == -1){
            reverse(arr.begin(),arr.end());
            return;
        }
        for(int j = n-1; j> swap_index;j--){
            if(arr[j] > arr[swap_index]){
                swap(arr[j], arr[swap_index]);
                break;
            }
        }
        reverse(arr.begin()+swap_index+1,arr.end());
        return;
        
    }
};

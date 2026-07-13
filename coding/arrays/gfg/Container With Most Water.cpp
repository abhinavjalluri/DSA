// Given an array arr[] of non-negative integers, where each element arr[i] represents the height of the vertical lines, find the maximum amount of water that can be contained between any two lines, together with the x-axis.

// Note: In the case of a single vertical line it will not be able to hold water.

// Examples:

// Input: arr[] = [1, 5, 4, 3]
// Output: 6
// Explanation: 5 and 3 are 2 distance apart. So the size of the base is 2. Height of container = min(5, 3) = 3. So, total area to hold water = 3 * 2 = 6.


class Solution {
  public:
    // TLE
    // int maxWater(vector<int> &arr) {
    //     int n = arr.size();
    //     if(n <= 1){
    //         return 0;
    //     }
    //     int max_water = INT_MIN;
    //     for(int i = 0; i<n-1; i++){
    //         for(int j = i+1;j<n;j++){
    //             max_water = max(max_water, (j-i)*min(arr[i], arr[j]));
    //         }
    //     }
    //     return max_water;
        
    // }
    int maxWater(vector<int> &arr){
        int n = arr.size();
        if(n <= 1){
            return 0;
        }
        int i = 0;
        int j = n-1;
        int max_water = INT_MIN;
        while(i < j){
            max_water = max(max_water, (j-i)*min(arr[i], arr[j]));
            if(arr[i] > arr[j]){
                j--;
            }
            else{
                i++;
            }
        }
        return max_water;
    }
};
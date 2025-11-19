// Given an array, arr[] construct a product array, res[] where each element in res[i] is the product of all elements in arr[] except arr[i]. Return this resultant array, res[].
// Note: Each element is res[] lies inside the 32-bit integer range.

// Examples:

// Input: arr[] = [10, 3, 5, 6, 2]
// Output: [180, 600, 360, 300, 900]
// Explanation: For i=0, res[i] = 3 * 5 * 6 * 2 is 180.
// For i = 1, res[i] = 10 * 5 * 6 * 2 is 600.
// For i = 2, res[i] = 10 * 3 * 6 * 2 is 360.
// For i = 3, res[i] = 10 * 3 * 5 * 2 is 300.
// For i = 4, res[i] = 10 * 3 * 5 * 6 is 900.
// Input: arr[] = [12, 0]
// Output: [0, 12]
// Explanation: For i = 0, res[i] is 0.
// For i = 1, res[i] is 12.
// Constraints:
// 2 <= arr.size() <= 105
// -100 <= arr[i] <= 100

// User function template for C++

class Solution {
  public:
    vector<int> productExceptSelf(vector<int>& arr) {
        vector<int>res(arr.size(), 0);
        long long product = 1;
        int zero_count = 0;
        int index = -1;
        for(int i = 0;i<arr.size();i++){
            if(arr[i] == 0){
                index = i;
                zero_count++;
            }
            else{
                product = product*arr[i];
            }
        }
        if(zero_count > 1){
            return res;
        }
        else if(zero_count == 1){
            res[index] = product;
            return res;
        }
        else{
            for(int i = 0;i<arr.size();i++){
                res[i] = (product/arr[i]);
            }
            return res;
        }
    }
};

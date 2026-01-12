// You have n coins and you want to build a staircase with these coins. The staircase consists of k rows where the ith row has exactly i coins. The last row of the staircase may be incomplete.

// Given the integer n, return the number of complete rows of the staircase you will build.

// Example 1:


// Input: n = 5
// Output: 2
// Explanation: Because the 3rd row is incomplete, we return 2.
// Example 2:


// Input: n = 8
// Output: 3
// Explanation: Because the 4th row is incomplete, we return 3.
 

// Constraints:

// 1 <= n <= 231 - 1

class Solution {
public:
    int arrangeCoins(int n) {
        // return floor(-0.5 + sqrt((double)2 * n + 0.25));
        int i = 0;
        int j = n;
        long long sum;
        int ans;
        while(i <= j){
            long long mid = i+(j-i)/2;
            sum = mid*(mid+1)/2;
            if(sum == n){
                return mid;
            }
            else if(n < sum){
                j = mid-1;
            }
            else{
                ans = (int)mid;
                i = mid+1;
            }
        }
        return ans;
    }
};
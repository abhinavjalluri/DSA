// Given an integer n, return the least number of perfect square numbers that sum to n.

// A perfect square is an integer that is the square of an integer; in other words, it is the product of some integer with itself. For example, 1, 4, 9, and 16 are perfect squares while 3 and 11 are not.

 

// Example 1:

// Input: n = 12
// Output: 3
// Explanation: 12 = 4 + 4 + 4.
// Example 2:

// Input: n = 13
// Output: 2
// Explanation: 13 = 4 + 9.
 

// Constraints:

// 1 <= n <= 104

// Approach 1: Using Recursion and Memoization

class Solution {
public:
    int solve(int n, vector<int>&dp){
        if(n == 0){
            return 0;
        }
        if(dp[n] != -1){
            return dp[n];
        }
        int ans = n;
        for(int i = 1 ; i*i <= n ; i++){
            ans = min(ans, solve(n-i*i, dp)+1);
        }
        dp[n] = ans;
        return dp[n];
    }
    int numSquares(int n) {
        vector<int>dp(n+1, -1);
        return solve(n, dp);
    }
};

// Approach 2: Using Dynamic Programming
// Another approach using dynamic programming
// dp[i] = min(dp[i], dp[i-j*j]+1) for all j such that j*j <= i

class Solution {
public:
    int numSquares(int n) {
        vector<int>dp(n+1,INT_MAX);
        dp[0] = 0;
        for(int i =1;i<=n;i++){
            for(int j = 1;j*j<=i;j++){
                dp[i] = min(dp[i],dp[i-j*j]+1);
            }
        }
        return dp[n];    
    }
};
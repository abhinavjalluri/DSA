// You are given an integer array coins representing coins of different denominations and an integer amount representing a total amount of money.

// Return the fewest number of coins that you need to make up that amount. If that amount of money cannot be made up by any combination of the coins, return -1.

// You may assume that you have an infinite number of each kind of coin.

 

// Example 1:

// Input: coins = [1,2,5], amount = 11
// Output: 3
// Explanation: 11 = 5 + 5 + 1
// Example 2:

// Input: coins = [2], amount = 3
// Output: -1
// Example 3:

// Input: coins = [1], amount = 0
// Output: 0
 

// Constraints:

// 1 <= coins.length <= 12
// 1 <= coins[i] <= 231 - 1
// 0 <= amount <= 104

class Solution {
public:
    int f(vector<vector<int>>&dp, vector<int>&coins,int amount,int i){
        if(i >= coins.size() || amount <= 0){
            if(amount == 0){
               return 0;
            }
            else
            return INT_MAX-1;
        }
        if(dp[i][amount] != -1){
            return dp[i][amount];
        }
        if(coins[i] > amount){
            return dp[i][amount] = f(dp,coins,amount,i+1);
        }
        else{
            int pick = 1+f(dp,coins,amount-coins[i],i);
            int not_pick = f(dp,coins,amount,i+1);
            return dp[i][amount] = min(pick,not_pick);
        }
    }
    int coinChange(vector<int>& coins, int amount) {
        int n = coins.size();
        vector<vector<int>>dp(n+1,vector<int>(amount+1,-1));
        int res = f(dp,coins,amount,0);
        return res == INT_MAX-1 ? -1 : res;

    }

    ////////////////////////////////////////////////////

    int coinChange(vector<int>& coins, int amount) {
        int n = coins.size();
        
        // dp[i][a] = minimum coins to make amount `a` using coins[i..n-1]
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));

        // Initialize base row (i = n)
        for (int a = 1; a <= amount; a++) {
            dp[n][a] = INT_MAX - 1;
        }
        dp[n][0] = 0;

        // Fill table bottom-up
        for (int i = n - 1; i >= 0; i--) {
            for (int a = 0; a <= amount; a++) {
                
                if (a == 0) {
                    dp[i][a] = 0;
                    continue;
                }

                if (coins[i] > a) {
                    dp[i][a] = dp[i + 1][a];
                } else {
                    int pick = (dp[i][a - coins[i]] == INT_MAX - 1)
                               ? INT_MAX - 1
                               : 1 + dp[i][a - coins[i]];

                    int not_pick = dp[i + 1][a];

                    dp[i][a] = min(pick, not_pick);
                }
            }
        }

        return dp[0][amount] == INT_MAX - 1 ? -1 : dp[0][amount];
    }

    ////////////////////////////////////////////////////////


};

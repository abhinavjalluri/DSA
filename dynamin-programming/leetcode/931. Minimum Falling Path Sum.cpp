// Given an n x n array of integers matrix, return the minimum sum of any falling path through matrix.

// A falling path starts at any element in the first row and chooses the element in the next row that is either directly below or diagonally left/right. Specifically, the next element from position (row, col) will be (row + 1, col - 1), (row + 1, col), or (row + 1, col + 1).

// Constraints:

// n == matrix.length == matrix[i].length
// 1 <= n <= 100
// -100 <= matrix[i][j] <= 100

class Solution {
public:
    int f(int row, int col, vector<vector<int>>& dp, vector<vector<int>>& matrix) {
        int n = matrix.size();

        if (col < 0 || col >= n) return 1e8;

        if (row == n - 1) return matrix[row][col];

        if (dp[row][col] != -1) return dp[row][col];

        int val = matrix[row][col];

        int down = val + f(row + 1, col, dp, matrix);
        int left = val + f(row + 1, col - 1, dp, matrix);
        int right = val + f(row + 1, col + 1, dp, matrix);

        return dp[row][col] = min(down, min(left, right));
    }

    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        vector<vector<int>> dp(n, vector<int>(n, -1));

        int ans = 1e8;

        for (int col = 0; col < n; col++) {
            ans = min(ans, f(0, col, dp, matrix));
        }

        return ans;
    }
    /////////////////////////////////////////////////////////
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        vector<vector<int>> dp = matrix;

        int ans = 1e8;
        for(int row = n-2;row>=0;row--){
            for(int col = 0;col < n;col++){
                int down = dp[row+1][col];
                int right = col < n-1 ? dp[row+1][col+1] : 1e8;
                int left = col > 0 ? dp[row+1][col-1] : 1e8;

                dp[row][col] += min(down, min(right,left));
            }

        }

        return *min_element(dp[0].begin(), dp[0].end());
    }
    ///////////////////////////////////////////////////////////
};

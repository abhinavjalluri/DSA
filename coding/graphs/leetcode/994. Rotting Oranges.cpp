// You are given an m x n grid where each cell can have one of three values:

// 0 representing an empty cell,
// 1 representing a fresh orange, or
// 2 representing a rotten orange.
// Every minute, any fresh orange that is 4-directionally adjacent to a rotten orange becomes rotten.

// Return the minimum number of minutes that must elapse until no cell has a fresh orange. If this is impossible, return -1.

 

// Example 1:


// Input: grid = [[2,1,1],[1,1,0],[0,1,1]]
// Output: 4
// Example 2:

// Input: grid = [[2,1,1],[0,1,1],[1,0,1]]
// Output: -1
// Explanation: The orange in the bottom left corner (row 2, column 0) is never rotten, because rotting only happens 4-directionally.
// Example 3:

// Input: grid = [[0,2]]
// Output: 0
// Explanation: Since there are already no fresh oranges at minute 0, the answer is just 0.
 

// Constraints:

// m == grid.length
// n == grid[i].length
// 1 <= m, n <= 10
// grid[i][j] is 0, 1, or 2.

// BFS approach 
// Prefer BFS approach for this problem as it is more efficient in terms of time complexity and space complexity.

class Solution {
public:
    vector<vector<int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    int orangesRotting(vector<vector<int>>& grid) {
        queue<pair<int,int>>q;
        int n = grid.size();
        int m = grid[0].size();
        int freshCount = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(grid[i][j] == 2){
                    q.push({i, j});
                }
                if(grid[i][j] == 1){
                    freshCount++;
                }
            }
        }
        int minuites = 0;
        while(!q.empty() && freshCount > 0){
            int size = q.size();
            while(size--){
                int row = q.front().first;
                int col = q.front().second;
                q.pop();
                for (auto& dir : dirs) {
                    int newRow = row + dir[0];
                    int newCol = col + dir[1];
                    if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < m && grid[newRow][newCol] == 1) {
                        grid[newRow][newCol] = 2;
                        q.push({newRow, newCol});
                        freshCount--;
                    }
                }
            }
            minuites++;
        }
        if(freshCount == 0){
            return minuites;
        }
        return -1;
    }
};


// DFS approach

class Solution {
public:
    vector<vector<int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    void DFS(vector<vector<int>>& grid, vector<vector<int>>& time, int row, int col, int minutes) {
        int n = grid.size();
        int m = grid[0].size();
        // If we already reached this cell
        // in less or equal time, no need to continue.
        if (minutes >= time[row][col]){
            return;
        }
        time[row][col] = minutes;
        for (auto& dir : dirs) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];
            if (newRow >= 0 && newRow < n && newCol >= 0 && newCol < m && grid[newRow][newCol] == 1) {
                DFS(grid, time, newRow, newCol, minutes + 1);
            }
        }
    }

    int orangesRotting(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        // Minimum time required to reach each cell.
        vector<vector<int>> time(
            n, vector<int>(m, INT_MAX)
        );
        // Start DFS from every initially rotten orange.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 2) {
                    DFS(grid, time, i, j, 0);
                }
            }
        }
        int answer = 0;
        // Check all fresh oranges.
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1) {
                    // Never reached by any rotten orange.
                    if (time[i][j] == INT_MAX){
                        return -1;
                    }    
                    answer = max(answer, time[i][j]);
                }
            }
        }
        return answer;
    }
};
// Given an m x n 2D binary grid grid which represents a map of '1's (land) and '0's (water), return the number of islands.

// An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. You may assume all four edges of the grid are all surrounded by water.

 

// Example 1:

// Input: grid = [
//   ["1","1","1","1","0"],
//   ["1","1","0","1","0"],
//   ["1","1","0","0","0"],
//   ["0","0","0","0","0"]
// ]
// Output: 1
// Example 2:

// Input: grid = [
//   ["1","1","0","0","0"],
//   ["1","1","0","0","0"],
//   ["0","0","1","0","0"],
//   ["0","0","0","1","1"]
// ]
// Output: 3
 

// Constraints:

// m == grid.length
// n == grid[i].length
// 1 <= m, n <= 300
// grid[i][j] is '0' or '1'.

// DFS approach

class Solution {
public:
    vector<vector<int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};

    void dfs(vector<vector<char>>& grid, int row, int col){
        grid[row][col] = '0';
        int n = grid.size();
        int m = grid[0].size();
        for(int i = 0 ; i < 4; i++){
            int newRow = row + dirs[i][0];
            int newCol = col + dirs[i][1];
            if(newRow < n && newRow >= 0 && newCol < m && newCol >=0 && grid[newRow][newCol] == '1'){
                dfs(grid, newRow, newCol);
            }
        }
    }

    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        int count = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(grid[i][j] == '1'){
                    dfs(grid, i, j);
                    count++;
                }
            }
        }
        return count;
    }
};

// BFS approach

class Solution {
public:
    vector<vector<int>> dirs = {{0,1},{0,-1},{1,0},{-1,0}};

    void bfs(vector<vector<char>>& grid, int i, int j){
        grid[i][j] = '0';
        int n = grid.size();
        int m = grid[0].size();
        queue<pair<int,int>>q;
        q.push({i, j});
        while(!q.empty()){
            int row = q.front().first;
            int col = q.front().second;
            q.pop();
            for(int i = 0 ; i < 4; i++){
                int newRow = row + dirs[i][0];
                int newCol = col + dirs[i][1];
                if(newRow < n && newRow >= 0 && newCol < m && newCol >=0 && grid[newRow][newCol] == '1'){
                    q.push({newRow, newCol});
                    grid[newRow][newCol] = '0';
                }
            }
        }
    }

    int numIslands(vector<vector<char>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        int count = 0;
        for(int i = 0 ; i < n ; i++){
            for(int j = 0 ; j < m ; j++){
                if(grid[i][j] == '1'){
                    bfs(grid, i, j);
                    count++;
                }
            }
        }
        return count;
    }
};
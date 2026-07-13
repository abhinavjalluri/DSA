// You are given an m x n integer matrix matrix with the following two properties:

// Each row is sorted in non-decreasing order.
// The first integer of each row is greater than the last integer of the previous row.
// Given an integer target, return true if target is in matrix or false otherwise.

// You must write a solution in O(log(m * n)) time complexity.

 

// Example 1:


// Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
// Output: true
// Example 2:


// Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13
// Output: false
 

// Constraints:

// m == matrix.length
// n == matrix[i].length
// 1 <= m, n <= 100
// -104 <= matrix[i][j], target <= 104

// Approach: We can treat the 2D matrix as a 1D array and perform binary search on it. The index of the 1D array can be converted to the 2D matrix using the formula: row = index / number of columns, col = index % number of columns.

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int n = matrix.size()*matrix[0].size();
        int left = 0;
        int right = n-1;
        while(left <= right){
            int mid = left+(right-left)/2;
            int row = mid/matrix[0].size();
            int col = mid%matrix[0].size();
            if(matrix[row][col] == target){
                return true;
            }
            else if(matrix[row][col] < target){
                left = mid+1;
            }
            else{
                right = mid-1;
            }
        }
        return false;
    }
};

// Another approach is to first find the row in which the target may be present and then perform binary search on that row.

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int m = matrix.size();
        int n = matrix[0].size();
        int rowLeft = 0;
        int rowRight = m-1;
        while(rowLeft <= rowRight){
            int mid = rowLeft+(rowRight- rowLeft)/2;
            if(matrix[mid][0] == target){
                return true;
            }
            else if(matrix[mid][0] < target){
                rowLeft = mid+1;
            }
            else{
                rowRight = mid-1;
            }
        }
        if(rowRight < 0){
            return false;
        }
        int row = rowRight;
        int colLeft = 0;
        int colRight = n-1;
        while(colLeft <= colRight){
            int mid = colLeft+(colRight-colLeft)/2;
            if(matrix[row][mid] == target){
                return true;
            }
            else if(matrix[row][mid] < target){
                colLeft = mid+1;
            }
            else{
                colRight = mid-1;
            }
        }
        return false;
    }
};

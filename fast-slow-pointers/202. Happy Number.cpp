// Write an algorithm to determine if a number n is happy.

// A happy number is a number defined by the following process:

// Starting with any positive integer, replace the number by the sum of the squares of its digits.
// Repeat the process until the number equals 1 (where it will stay), or it loops endlessly in a cycle which does not include 1.
// Those numbers for which this process ends in 1 are happy.
// Return true if n is a happy number, and false if not.

 

// Example 1:

// Input: n = 19
// Output: true
// Explanation:
// 12 + 92 = 82
// 82 + 22 = 68
// 62 + 82 = 100
// 12 + 02 + 02 = 1
// Example 2:

// Input: n = 2
// Output: false
 

// Constraints:

// 1 <= n <= 231 - 1

class Solution {
public:

    int f(int n){
        int square_sum = 0;
        while(n){
            int num = n%10;
            square_sum += num*num;
            n /= 10;
        }
        return square_sum;
    }
    bool isHappy(int n) {
        int slow = n;
        int fast = n;
        while(true){
            slow = f(slow);
            if(slow == 1){
                return true;
            }
            fast = f(fast);
            if(fast == 1){
                return true;
            }
            fast = f(fast);
            if(fast == 1){
                return true;
            }
            if(slow == fast){
                return false;
            }
        }
    }
};

// Another approach using hash set to detect cycle

class Solution {
private:
    int nextNumber(int n){
        int newNumber = 0;
        while(n!=0){
            int num = n%10;
            newNumber += num*num;
            n = n/10;
        }
        return newNumber;
    }
public:
    bool isHappy(int n) {
        unordered_set<int> set;
        while(n!=1 && !set.count(n)){
            set.insert(n);
            n = nextNumber(n);
        }
        return n==1;
    }
};
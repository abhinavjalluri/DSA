// Given two strings s and t of lengths m and n respectively, return the minimum window substring of s such that every character in t (including duplicates) is included in the window. If there is no such substring, return the empty string "".

// The testcases will be generated such that the answer is unique.

 

// Example 1:

// Input: s = "ADOBECODEBANC", t = "ABC"
// Output: "BANC"
// Explanation: The minimum window substring "BANC" includes 'A', 'B', and 'C' from string t.
// Example 2:

// Input: s = "a", t = "a"
// Output: "a"
// Explanation: The entire string s is the minimum window.
// Example 3:

// Input: s = "a", t = "aa"
// Output: ""
// Explanation: Both 'a's from t must be included in the window.
// Since the largest window of s only has one 'a', return empty string.
 

// Constraints:

// m == s.length
// n == t.length
// 1 <= m, n <= 105
// s and t consist of uppercase and lowercase English letters.
 

// Follow up: Could you find an algorithm that runs in O(m + n) time?

class Solution {
public:
    string minWindow(string s, string t) {
        int n = s.size();
        int m = t.size();
        if (m > n)
        return "";
        unordered_map<char,int>need;
        unordered_map<char,int>window;
        int startIndex = 0;
        int minLength = INT_MAX;
        for(auto i : t){
            need[i]++;
        }
        int required = need.size();
        int formed = 0;
        int left = 0;
        int right = 0;
        while(right <= n){
            char c = s[right];
            window[c]++;
            if(need.count(c) && window[c] == need[c]){
                formed++;
            }
            while(left<=right && formed == required){
                if(right-left+1 < minLength){
                    minLength = right-left+1;
                    startIndex = left;
                }
                char leftChar = s[left];
                window[leftChar]--;
                if(need.count(leftChar) && window[leftChar] < need[leftChar]){
                    formed--;
                }
                left++;
            }
            right++;
        }
        if(minLength == INT_MAX){
            return "";
        }
        return s.substr(startIndex, minLength);
        
    }
};
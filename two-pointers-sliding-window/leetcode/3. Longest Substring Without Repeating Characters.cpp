// Given a string s, find the length of the longest substring without duplicate characters.

 

// Example 1:

// Input: s = "abcabcbb"
// Output: 3
// Explanation: The answer is "abc", with the length of 3. Note that "bca" and "cab" are also correct answers.
// Example 2:

// Input: s = "bbbbb"
// Output: 1
// Explanation: The answer is "b", with the length of 1.
// Example 3:

// Input: s = "pwwkew"
// Output: 3
// Explanation: The answer is "wke", with the length of 3.
// Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.
 

// Constraints:

// 0 <= s.length <= 5 * 104
// s consists of English letters, digits, symbols and spaces.

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int n = s.size();
        if(n == 0)
        return 0;
        unordered_map<char,int>mp;
        int start = 0;
        int maxi = 0;
        for(int i =0;i<n;i++){
            if(mp.find(s[i]) != mp.end() && mp[s[i]] >= start){
                start = mp[s[i]]+1;
            }
            mp[s[i]] = i;
            maxi = max(maxi,i-start+1);
        }
        return maxi;
    }
};
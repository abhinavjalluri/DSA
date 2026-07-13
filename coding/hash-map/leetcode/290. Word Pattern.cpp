// Given a pattern and a string s, find if s follows the same pattern.

// Here follow means a full match, such that there is a bijection between a letter in pattern and a non-empty word in s. Specifically:

// Each letter in pattern maps to exactly one unique word in s.
// Each unique word in s maps to exactly one letter in pattern.
// No two letters map to the same word, and no two words map to the same letter.
 

// Example 1:

// Input: pattern = "abba", s = "dog cat cat dog"

// Output: true

// Explanation:

// The bijection can be established as:

// 'a' maps to "dog".
// 'b' maps to "cat".
// Example 2:

// Input: pattern = "abba", s = "dog cat cat fish"

// Output: false

// Example 3:

// Input: pattern = "aaaa", s = "dog cat cat dog"

// Output: false

 

// Constraints:

// 1 <= pattern.length <= 300
// pattern contains only lower-case English letters.
// 1 <= s.length <= 3000
// s contains only lowercase English letters and spaces ' '.
// s does not contain any leading or trailing spaces.
// All the words in s are separated by a single space.

class Solution {
public:
    bool wordPattern(string pattern, string s) {
        int n = pattern.size();
        int m = s.size();

        int i1 = 0;
        int i2 = 0;

        unordered_map<char, string> mp1;
        unordered_map<string, char> mp2;

        while(i1 < n && i2 < m){
            string temp = "";
            while(i2 < m && s[i2] == ' '){
                i2++;
            }
            while(i2 < m && s[i2] != ' '){
                temp += s[i2];
                i2++;
            }

            if(mp1.count(pattern[i1])){
                if(mp1[pattern[i1]] != temp){
                    return false;
                }
            }
            else{
                mp1[pattern[i1]] = temp;
            }

            if(mp2.count(temp)){
                if(mp2[temp] != pattern[i1]){
                    return false;
                }
            }
            else{
                mp2[temp] = pattern[i1];
            }
            i1++;
        }
        if((i1 == n && i2 != m) || (i1 != n && i2 == m)){
            return false;
        }
        return true;
    }
};
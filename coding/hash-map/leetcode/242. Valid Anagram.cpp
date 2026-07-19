// Given two strings s and t, return true if t is an anagram of s, and false otherwise.

 

// Example 1:

// Input: s = "anagram", t = "nagaram"

// Output: true

// Example 2:

// Input: s = "rat", t = "car"

// Output: false

 

// Constraints:

// 1 <= s.length, t.length <= 5 * 104
// s and t consist of lowercase English letters.

class Solution {
public:
    bool isAnagram(string s, string t) {
        // sort(s.begin(), s.end());
        // sort(t.begin(), t.end());
        // return s == t;
        if (s.length() != t.length()) return false;
        vector<int>chars(26 ,0);
        for(int i = 0 ; i < s.size(); i++){
            chars[s[i] - 'a']++;
            chars[t[i] - 'a']--;
        }
        for(int i = 0 ; i < 26 ; i++){
            if(chars[i] != 0){
                return false;
            }
        }
        return true;
    }
};

// Another approach using unordered_map

class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.length() != t.length()) return false;
        unordered_map<char,int>map;
        for(int i = 0 ; i < s.size(); i++){
            map[s[i]]++;
        }
        for(int i = 0 ; i < t.size() ; i++){
            map[t[i]]--;
        }
        for(auto i : map){
            if(i.second != 0){
                return false;
            }
        }
        return true;
    }
};
// Given a string s, return the longest palindromic substring in s.

class Solution {
public:
    bool palindrome(string &s, int i, int j){
        while(i<j){
            if(s[i] != s[j]){
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
    string longestPalindrome(string s) {
        int n = s.size();
        if(n <= 1){
            return s;
        }
        int max_length = 1;
        string max_string = s.substr(0,1);
        for(int i = 0; i<n;i++){
            for(int j = i+max_length;j<=n;j++){
                if(palindrome(s,i,j-1)){
                    max_length= j-i;
                    max_string = s.substr(i,j-i);
                }
            }
        }
        return max_string;
    }
};
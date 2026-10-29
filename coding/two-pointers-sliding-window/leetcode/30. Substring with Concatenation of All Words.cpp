// You are given a string s and an array of strings words. All the strings of words are of the same length.

// A concatenated string is a string that exactly contains all the strings of any permutation of words concatenated.

// For example, if words = ["ab","cd","ef"], then "abcdef", "abefcd", "cdabef", "cdefab", "efabcd", and "efcdab" are all concatenated strings. "acdbef" is not a concatenated string because it is not the concatenation of any permutation of words.
// Return an array of the starting indices of all the concatenated substrings in s. You can return the answer in any order.

 

// Example 1:

// Input: s = "barfoothefoobarman", words = ["foo","bar"]

// Output: [0,9]

// Explanation:

// The substring starting at 0 is "barfoo". It is the concatenation of ["bar","foo"] which is a permutation of words.
// The substring starting at 9 is "foobar". It is the concatenation of ["foo","bar"] which is a permutation of words.

// Example 2:

// Input: s = "wordgoodgoodgoodbestword", words = ["word","good","best","word"]

// Output: []

// Explanation:

// There is no concatenated substring.

// Example 3:

// Input: s = "barfoofoobarthefoobarman", words = ["bar","foo","the"]

// Output: [6,9,12]

// Explanation:

// The substring starting at 6 is "foobarthe". It is the concatenation of ["foo","bar","the"].
// The substring starting at 9 is "barthefoo". It is the concatenation of ["bar","the","foo"].
// The substring starting at 12 is "thefoobar". It is the concatenation of ["the","foo","bar"].

 

// Constraints:

// 1 <= s.length <= 104
// 1 <= words.length <= 5000
// 1 <= words[i].length <= 30
// s and words[i] consist of lowercase English letters.


// TLE /////////////////// HARD //////////////////////


// class Solution {
// public:
//     vector<int> findSubstring(string s, vector<string>& words) {
//         int n = s.size();
//         int m = words[0].size();
//         int k = words.size();
//         int left = 0; 
//         int right = k*m-1;
//         unordered_map<string, int>mpp;
//         for(int i = 0 ; i < words.size(); i++){
//             mpp[words[i]]++;
//         }
//         vector<int>ans;
//         while(right < n){
//             string temp = s.substr(left, k*m);
//             int count = 0;
//             unordered_map<string, int>tempMpp = mpp;
//             for(int i = 0 ; i < k*m ; i+=m){
//                 string t = temp.substr(i, m);
//                 if(tempMpp.find(t) != tempMpp.end() && tempMpp[t] > 0){
//                     count++;
//                     tempMpp[t]--;
//                 }
//                 else {
//                     break;
//                 }
//             }
//             if(count == k){
//                 ans.push_back(left);
//             }
//             left++;
//             right++;
//         }
//         return ans;
//     }
// };


class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        int n = s.size();
        int m = words[0].size();
        int k = words.size();
        unordered_map<string, int> mpp;
        for (int i = 0; i < k; i++) {
            mpp[words[i]]++;
        }
        vector<int> ans;
        for (int i = 0; i < m; i++) {
            int left = i;
            int count = 0;
            unordered_map<string, int> tempMpp;
            for (int right = i; right + m <= n; right += m) {
                string t = s.substr(right, m);
                if (mpp.find(t) == mpp.end()) {
                    tempMpp.clear();
                    count = 0;
                    left = right + m;
                    continue;
                }
                tempMpp[t]++;
                count++;
                while (tempMpp[t] > mpp[t]) {
                    string leftWord = s.substr(left, m);
                    tempMpp[leftWord]--;
                    count--;
                    left += m;
                }
                if (count == k) {
                    ans.push_back(left);

                    string leftWord = s.substr(left, m);
                    tempMpp[leftWord]--;
                    count--;
                    left += m;
                }
            }
        }
        return ans;
    }
};


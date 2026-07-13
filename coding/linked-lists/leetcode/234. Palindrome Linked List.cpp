// Given the head of a singly linked list, return true if it is a palindrome or false otherwise.

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
 
class Solution {
public:
    bool f(vector<int>&arr){
        int n = arr.size();
        int start = 0;
        int end = n-1;
        while(start < end){
            if(arr[start] != arr[end])
            return false;
            start++;
            end--;
        }
        return true;
    }
    bool isPalindrome(ListNode* head) {
        vector<int>arr;
        while(head){
            arr.push_back(head->val);
            head = head->next;
        }
        return f(arr);
        
    }
};
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

// Another approach using O(1) space complexity by reversing the second half of the linked list and comparing it with the first half.

class Solution {
public:
    ListNode*middle(ListNode* head){
        ListNode*slow=head;
        ListNode*fast=head;
        while(fast->next!=NULL && fast->next->next!=NULL){
            slow=slow->next;
            fast=fast->next->next;
        }

        return slow;
    }

     ListNode*reversed(ListNode* head){
        ListNode*prev=NULL;
        ListNode*current=head;
        ListNode*nextNode=NULL;
        while(current!=NULL){
            nextNode=current->next;
            current->next=prev;
            prev=current;
            current=nextNode;
        }
        return prev;
        
    }


    bool isPalindrome(ListNode* head) {
        ListNode*first=head;
        ListNode*second=middle(head);
        second=reversed(second->next);
        while(second!=NULL){
            if(first->val!=second->val){
                return false;
            }
            first=first->next;
            second=second->next;
        }

        return true;

        
    }
};
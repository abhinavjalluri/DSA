// Given the head of a linked list and an integer val, remove all the nodes of the linked list that has Node.val == val, and return the new head.

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
    ListNode* removeElements(ListNode* head, int val) {
        ListNode* A = new ListNode();
        A->next = head;
        ListNode* a = A;
        while(a->next){
            if(a->next->val == val){
                a->next = a->next->next;
            }
            else{
                a = a->next;
            }
        }
        return A->next;

        // Recursion 
        // if(!head){
        //     return NULL;
        // }
        // if(head->val == val){
        //     return removeElements(head->next, val);
        // }
        // head->next = removeElements(head->next, val);
        // return head;
    }
};
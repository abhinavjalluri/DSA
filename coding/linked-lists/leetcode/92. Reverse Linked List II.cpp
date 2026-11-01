// Given the head of a singly linked list and two integers left and right where left <= right, reverse the nodes of the list from position left to position right, and return the reversed list.

 

// Example 1:


// Input: head = [1,2,3,4,5], left = 2, right = 4
// Output: [1,4,3,2,5]
// Example 2:

// Input: head = [5], left = 1, right = 1
// Output: [5]
 

// Constraints:

// The number of nodes in the list is n.
// 1 <= n <= 500
// -500 <= Node.val <= 500
// 1 <= left <= right <= n
 


class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode* ans = new ListNode();
        ans->next = head;

        ListNode* prev = ans;

        // Move prev to the node before 'left'
        for (int i = 0; i < left - 1; i++) {
            prev = prev->next;
        }

        ListNode* before = prev;
        ListNode* curr = prev->next;
        ListNode* tail = curr;
        ListNode* next = NULL;

        prev = NULL;

        // Reverse nodes from left to right
        for (int i = 0; i <= right - left; i++) {
            next = curr->next;
            curr->next = prev;

            prev = curr;
            curr = next;
        }

        // Reconnect the reversed portion
        before->next = prev;
        tail->next = curr;

        ListNode* result = ans->next;
        delete ans;
        return result;
    }
};

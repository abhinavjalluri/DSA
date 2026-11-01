// You are given the head of a sorted linked list.

// Delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.

// Return the linked list sorted as well.

 

// Example 1:


// Input: head = [1,2,3,3,4,4,5]
// Output: [1,2,5]
// Example 2:


// Input: head = [1,1,1,2,3]
// Output: [2,3]
 

// Constraints:

// The number of nodes in the list is in the range [0, 300].
// -100 <= Node.val <= 100
// The list is guaranteed to be sorted in ascending order.


class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode* curr = head;
        ListNode* ans = new ListNode();
        ListNode* result = ans;

        while (curr) {
            int val = curr->val;
            int count = 0;

            while (curr && curr->val == val) {
                count++;
                curr = curr->next;
            }

            if (count == 1) {
                ans->next = new ListNode(val);
                ans = ans->next;
            }
        }

        ListNode* newHead = result->next;
        delete result;
        return newHead;
    }
};

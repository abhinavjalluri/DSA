// Given the head of a linked list, rotate the list to the right by k places.

 

// Example 1:


// Input: head = [1,2,3,4,5], k = 2
// Output: [4,5,1,2,3]
// Example 2:


// Input: head = [0,1,2], k = 4
// Output: [2,0,1]
 

// Constraints:

// The number of nodes in the list is in the range [0, 500].
// -100 <= Node.val <= 100
// 0 <= k <= 2 * 109


class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head || !head->next || k == 0) {
            return head;
        }
        ListNode* curr = head;
        int count = 0;
        while (curr) {
            curr = curr->next;
            count++;
        }
        k %= count;
        if (k == 0) {
            return head;
        }
        // Step 1: Reverse entire linked list
        curr = head;
        ListNode* prev = NULL;
        ListNode* next = NULL;
        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        // Step 2: Reverse first k nodes
        curr = prev;
        prev = NULL;
        ListNode* tail = curr;
        
        while (k > 0) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
            k--;
        }

        ListNode* newHead = prev;

        // Step 3: Reverse remaining nodes
        prev = NULL;

        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        // Step 4: Connect both parts
        tail->next = prev;

        return newHead;
    }
};

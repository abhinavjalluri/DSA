// You are given an array of k linked-lists lists, each linked-list is sorted in ascending order.

// Merge all the linked-lists into one sorted linked-list and return it.

 

// Example 1:

// Input: lists = [[1,4,5],[1,3,4],[2,6]]
// Output: [1,1,2,3,4,4,5,6]
// Explanation: The linked-lists are:
// [
//   1->4->5,
//   1->3->4,
//   2->6
// ]
// merging them into one sorted linked list:
// 1->1->2->3->4->4->5->6
// Example 2:

// Input: lists = []
// Output: []
// Example 3:

// Input: lists = [[]]
// Output: []
 

// Constraints:

// k == lists.length
// 0 <= k <= 104
// 0 <= lists[i].length <= 500
// -104 <= lists[i][j] <= 104
// lists[i] is sorted in ascending order.
// The sum of lists[i].length will not exceed 104.

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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int n = lists.size();
        if(n == 0){
            return nullptr;
        }
        int left = 0;
        int right = n-1;
        return mergeListsHelper(lists, left, right);
    }
    ListNode* mergeListsHelper(vector<ListNode*>&lists, int left, int right){
        if(left == right){
            return lists[left];
        }
        int mid = left+(right-left)/2;
        ListNode* l1 = mergeListsHelper(lists,left,mid);
        ListNode* l2 = mergeListsHelper(lists,mid+1,right);
        return merge(l1,l2);

    }
    ListNode* merge(ListNode* l1, ListNode* l2){
        ListNode* ans = new ListNode(0);
        ListNode* temp = ans;
        while(l1 && l2){
            if(l1->val < l2->val){
                temp->next = l1;
                l1 = l1->next;
            }
            else{
                temp->next = l2;
                l2 = l2->next;
            }
            temp = temp->next;
        }
        if(l1){
            temp->next = l1;
        }
        if(l2){
            temp->next = l2;
        }
        return ans->next;
    }
};

// Another approach using priority queue
class Solution {
public:

    struct Compare{
        bool operator()(ListNode* a, ListNode* b){
            return a->val > b->val;
        }
    };
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue<ListNode*, vector<ListNode*>, Compare>pq;
        for(int i = 0 ; i < lists.size(); i++){
            if(lists[i]){
                pq.push(lists[i]);
            }
        }
        ListNode* ans = new ListNode(0);
        ListNode* temp = ans;
        while(!pq.empty()){
            ListNode* node = pq.top();
            pq.pop();
            temp->next = node;
            temp = temp->next;
            if(node->next){
                pq.push(node->next);
            }
        }
        return ans->next;
    }
};

// Complexity Summary
// Approach	                      Time Complexity	Space Complexity
// Divide & Conquer	                   O(N log k)	O(log k) (recursion stack)
// Min Heap (Priority Queue)	       O(N log k)	O(k) (heap)

// Where:

// N = Total number of nodes across all linked lists.
// k = Number of linked lists.

// Which one to use?

// Divide & Conquer uses less auxiliary memory (O(log k)) and is excellent when recursion is acceptable.
// Priority Queue is often easier to understand and implement, and naturally generalizes to problems involving merging multiple sorted streams.
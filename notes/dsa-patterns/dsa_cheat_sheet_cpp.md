# DSA Patterns — C++ Quick Revision Cheat Sheet

```cpp
#include <bits/stdc++.h>
using namespace std;
```

---

## 1. Two Pointers
**Idea:** Two indices walk the data so you avoid nested loops (O(n²) → O(n)).
**Use when:** pairs/triplets/subarrays, sorted (or sortable) data, in-place array edits.

| Variant | Setup | Example |
|---|---|---|
| Opposite ends | `l = 0, r = n-1`, move inward | Pair sum, container with most water, palindrome check |
| Same direction | `slow`, `fast` both from 0 | Remove duplicates, move zeroes |

**Logic:** if the sum is too small, move `l` right (bigger value); if too big, move `r` left.

```cpp
// Pair with given sum in SORTED array
bool pairSum(vector<int>& a, int target) {
    int l = 0, r = a.size() - 1;
    while (l < r) {
        int sum = a[l] + a[r];
        if (sum == target) return true;
        else if (sum < target) l++;
        else r--;
    }
    return false;
}
```
**Tip:** For 3Sum — sort, fix one element `i`, then two-pointer on the rest. O(n²).

---

## 2. Fast & Slow Pointers (Floyd's Tortoise–Hare)
**Idea:** `slow` moves 1 step, `fast` moves 2. If there is a cycle, fast eventually laps slow.
**Use when:** cycle detection, middle of list, cycle start, palindrome list, Happy Number, circular array loop.
**Clues:** repeated traversal/transformation, possible loop, O(1) space needed.

```cpp
// Cycle detection
bool hasCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// Middle of list (2nd middle if even length)
ListNode* middle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow;
}

// Start of cycle
ListNode* cycleStart(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next; fast = fast->next->next;
        if (slow == fast) {                 // met inside the cycle
            slow = head;                    // reset one pointer to head
            while (slow != fast) { slow = slow->next; fast = fast->next; }
            return slow;                    // both meet at cycle start
        }
    }
    return nullptr;
}
```
**Why reset works:** distance from head to cycle start equals distance from the meeting point to cycle start (going around the loop).

**Happy Number** — treat "sum of squares of digits" as `next()`:
```cpp
int next(int n) { int s = 0; while (n) { int d = n % 10; s += d * d; n /= 10; } return s; }
bool isHappy(int n) {
    int slow = n, fast = next(n);
    while (fast != 1 && slow != fast) { slow = next(slow); fast = next(next(fast)); }
    return fast == 1;
}
```
**Complexity:** O(n) time, O(1) space.

---

## 3. Sliding Window
**Idea:** Keep a window `[start, end]` over a contiguous segment; update incrementally (add new, remove old) instead of recomputing.
**Use when:** substring/subarray, "longest/shortest/max/min", "contiguous", n ≈ 10⁵.

### Fixed size (k)
```cpp
int maxSumK(vector<int>& a, int k) {
    int sum = 0, best = INT_MIN;
    for (int i = 0; i < a.size(); i++) {
        sum += a[i];                         // add new element
        if (i >= k) sum -= a[i - k];         // remove outgoing element
        if (i >= k - 1) best = max(best, sum);
    }
    return best;
}
```

### Variable size
**Template:** expand `end` always; shrink `start` *while* the window is invalid/valid (depends on the problem).

```cpp
// Smallest subarray with sum >= target (positive numbers)
int minSubLen(vector<int>& a, int target) {
    int start = 0, sum = 0, ans = INT_MAX;
    for (int end = 0; end < a.size(); end++) {
        sum += a[end];
        while (sum >= target) {
            ans = min(ans, end - start + 1);
            sum -= a[start++];
        }
    }
    return ans == INT_MAX ? 0 : ans;
}

// Longest substring without repeating characters
int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> last;           // char -> last index seen
    int start = 0, ans = 0;
    for (int end = 0; end < s.size(); end++) {
        if (last.count(s[end]) && last[s[end]] >= start)
            start = last[s[end]] + 1;        // jump start past the duplicate
        last[s[end]] = end;
        ans = max(ans, end - start + 1);
    }
    return ans;
}
```
**Note:** Sliding window needs non-negative values for sum-type conditions. With negatives, use Prefix Sum + HashMap.
**Complexity:** O(n).

---

## 4. Prefix Sum
**Idea:** `prefix[i]` = sum of first `i` elements → any range sum in O(1).

```cpp
// Use size n+1 to avoid the i=0 edge case
vector<int> prefix(n + 1, 0);
for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];

// sum of a[l..r] inclusive
int rangeSum = prefix[r + 1] - prefix[l];
```

**Subarray Sum Equals K** (works with negatives):
Idea: if `prefix[j] - prefix[i] = k`, then subarray `(i, j]` sums to k. So for each `prefix[j]`, count earlier prefixes equal to `prefix[j] - k`.
```cpp
int subarraySum(vector<int>& a, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;                             // empty prefix
    int sum = 0, count = 0;
    for (int x : a) {
        sum += x;
        if (freq.count(sum - k)) count += freq[sum - k];
        freq[sum]++;
    }
    return count;
}
```

**2D prefix sum** (sub-rectangle sum):
```cpp
// P[i][j] = sum of rectangle (0,0)-(i-1,j-1)
P[i+1][j+1] = M[i][j] + P[i][j+1] + P[i+1][j] - P[i][j];
// sum of (r1,c1)-(r2,c2):
P[r2+1][c2+1] - P[r1][c2+1] - P[r2+1][c1] + P[r1][c1];
```

**Difference array** (range update in O(1)):
```cpp
diff[l] += v; diff[r + 1] -= v;             // add v to a[l..r]
// final array = prefix sum of diff
```
**Complexity:** O(n) build, O(1) query.

---

## 5. Merge Intervals
**Idea:** Sort by start. Overlap exists if `cur.start <= last.end`.
**Use when:** merging ranges, calendar conflicts, free time, insert interval, meeting rooms.

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());          // sorts by start, then end
    vector<vector<int>> res;
    for (auto& cur : intervals) {
        if (res.empty() || res.back()[1] < cur[0])
            res.push_back(cur);                         // no overlap
        else
            res.back()[1] = max(res.back()[1], cur[1]); // merge
    }
    return res;
}
```
Example: `[[1,3],[2,6],[8,10],[15,18]]` → `[[1,6],[8,10],[15,18]]`

**Related:**
- **Min meeting rooms:** sort by start + min-heap of end times. If `heap.top() <= cur.start`, reuse the room (pop); push `cur.end`. Answer = heap size.
- **Intersection of two lists:** two pointers; overlap = `[max(starts), min(ends)]`; advance the one that ends first.

**Complexity:** O(n log n).

---

## 6. Binary Search
**Idea:** Halve the search space each step. O(log n).
**Two contexts:** (A) sorted array, (B) search on answer (monotonic condition).

### A. Classic
```cpp
int binarySearch(vector<int>& a, int target) {
    int l = 0, r = a.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;           // avoids overflow
        if (a[mid] == target) return mid;
        else if (a[mid] < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}
```

### First / last occurrence (lower bound style)
```cpp
// first index with a[i] >= target  (== lower_bound)
int lowerBound(vector<int>& a, int target) {
    int l = 0, r = a.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (a[mid] >= target) r = mid;
        else l = mid + 1;
    }
    return l;
}
```
STL: `lower_bound(a.begin(), a.end(), x)` → first `>= x`; `upper_bound` → first `> x`.

### B. Binary Search on Answer
**Recipe:** (1) define answer range `[lo, hi]`, (2) write `can(mid)` that is monotonic (false…false true…true), (3) find the first `true`.
```cpp
// Koko Eating Bananas: min speed k to finish in h hours
bool can(vector<int>& piles, int k, int h) {
    long long hours = 0;
    for (int p : piles) hours += (p + k - 1) / k;      // ceil(p/k)
    return hours <= h;
}
int minEatingSpeed(vector<int>& piles, int h) {
    int l = 1, r = *max_element(piles.begin(), piles.end());
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (can(piles, mid, h)) r = mid;               // try smaller
        else l = mid + 1;
    }
    return l;
}
```
**Clues:** "minimum days / maximum capacity / smallest X such that…".
**Examples:** First Bad Version, Ship Packages in D Days, Split Array Largest Sum, Search in Rotated Array, Find Peak Element.

---

## 7. Sorting Algorithms

| Algorithm | Best | Avg | Worst | Space | Stable |
|---|---|---|---|---|---|
| Bubble | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Selection | O(n²) | O(n²) | O(n²) | O(1) | No |
| Insertion | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Merge | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick | O(n log n) | O(n log n) | O(n²) | O(log n) | No |

```cpp
// Bubble — largest bubbles to the end; "swapped" flag gives O(n) best case
void bubbleSort(vector<int>& a) {
    for (int i = 0; i < a.size() - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < a.size() - 1 - i; j++)
            if (a[j] > a[j + 1]) { swap(a[j], a[j + 1]); swapped = true; }
        if (!swapped) break;
    }
}

// Selection — pick min of the rest, place at i
void selectionSort(vector<int>& a) {
    for (int i = 0; i < a.size() - 1; i++) {
        int mn = i;
        for (int j = i + 1; j < a.size(); j++) if (a[j] < a[mn]) mn = j;
        swap(a[i], a[mn]);
    }
}

// Insertion — insert a[i] into sorted left part
void insertionSort(vector<int>& a) {
    for (int i = 1; i < a.size(); i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; j--; }
        a[j + 1] = key;
    }
}

// Merge sort — divide, sort halves, merge
void merge(vector<int>& a, int l, int m, int r) {
    vector<int> tmp;
    int i = l, j = m + 1;
    while (i <= m && j <= r) tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= m) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    for (int k = 0; k < tmp.size(); k++) a[l + k] = tmp[k];
}
void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSort(a, l, m); mergeSort(a, m + 1, r);
    merge(a, l, m, r);
}

// Quick sort — Lomuto partition (pivot = last)
int partition(vector<int>& a, int l, int r) {
    int pivot = a[r], i = l - 1;
    for (int j = l; j < r; j++)
        if (a[j] < pivot) swap(a[++i], a[j]);
    swap(a[i + 1], a[r]);
    return i + 1;
}
void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int p = partition(a, l, r);
    quickSort(a, l, p - 1); quickSort(a, p + 1, r);
}
```
**Quick sort worst case:** already-sorted input with first/last pivot → fix with random/median pivot.
**When to use:** small/mostly sorted → Insertion · guaranteed n log n / stable → Merge · fast in-place average → Quick.
**In practice:** `sort(a.begin(), a.end())` (introsort, O(n log n)).

---

## 8. HashMap (`unordered_map` / `map`)
**Idea:** key → value store with O(1) average get/put/erase.
**Use when:** frequency count, duplicates, "seen before", index lookup, grouping, memoization.

| Container | Order | Lookup | Underlying |
|---|---|---|---|
| `unordered_map` | none | O(1) avg, O(n) worst | Hash table |
| `map` | sorted by key | O(log n) | Red-Black tree |
| `unordered_set` / `set` | same idea, keys only | | |

```cpp
// Frequency count
unordered_map<char, int> freq;
for (char c : s) freq[c]++;                  // missing key defaults to 0

// Two Sum
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> mp;              // value -> index
    for (int i = 0; i < nums.size(); i++) {
        int comp = target - nums[i];
        if (mp.count(comp)) return {mp[comp], i};
        mp[nums[i]] = i;
    }
    return {};
}

// Group Anagrams
vector<vector<string>> groupAnagrams(vector<string>& words) {
    unordered_map<string, vector<string>> mp;
    for (auto& w : words) {
        string key = w;
        sort(key.begin(), key.end());        // sorted word = group key
        mp[key].push_back(w);
    }
    vector<vector<string>> res;
    for (auto& [k, v] : mp) res.push_back(v);
    return res;
}
```
**Classics:** Two Sum, Group Anagrams, Longest Substring w/o Repeating, Top K Frequent, Subarray Sum = K.

**Gotcha:** `mp[key]` *inserts* a default if the key is missing. To only check, use `mp.count(key)` or `mp.find(key) != mp.end()`.
**Gotcha:** `unordered_map` has no hash for `pair` by default — use `map<pair<int,int>, int>` or a custom hash.

### Internals (how a hash table works)
1. `hash(key)` → big integer.
2. `index = hash % bucketCount` → bucket.
3. **Collision** (two keys, same bucket) → handled by **chaining** (linked list per bucket). Java 8+ converts long chains (> 8) into Red-Black trees; C++ `unordered_map` typically keeps linked lists.
4. **put:** find bucket → if key exists overwrite, else append.
5. **get:** find bucket → scan the chain for the key.
6. **Resize (rehash)** when `size / buckets > load factor` (default ≈ 1.0 in C++, 0.75 in Java) → buckets double, all keys re-inserted.
7. Average O(1); worst O(n) when everything collides.

---

## 9. Stack (LIFO)
**Ops:** `push`, `pop`, `top`, `empty` — all O(1).
**Use when:** nested structures, undo/backtrack, next greater/smaller, expression evaluation, reversal, backspace.

```cpp
stack<int> st;
st.push(10); int t = st.top(); st.pop(); bool e = st.empty();
```

```cpp
// Valid Parentheses
bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') st.push(c);
        else {
            if (st.empty()) return false;
            char t = st.top(); st.pop();
            if ((c == ')' && t != '(') || (c == '}' && t != '{') || (c == ']' && t != '['))
                return false;
        }
    }
    return st.empty();
}

// Next Greater Element (Monotonic Stack — decreasing)
vector<int> nextGreater(vector<int>& a) {
    int n = a.size();
    vector<int> res(n, -1);
    stack<int> st;                           // stores INDICES
    for (int i = 0; i < n; i++) {
        while (!st.empty() && a[st.top()] < a[i]) {
            res[st.top()] = a[i];            // a[i] is the next greater for st.top()
            st.pop();
        }
        st.push(i);
    }
    return res;
}
```
**Monotonic stack rule:** need *next greater* → keep stack decreasing; need *next smaller* → keep it increasing. Each element is pushed/popped once → O(n).
**Problems:** Valid Parentheses, Next Greater Element, Evaluate RPN, Decode String (`3[a2[c]]`), Largest Rectangle in Histogram, Remove K Digits, Daily Temperatures.

---

## 10. Queue (FIFO)
**Ops:** `push` (enqueue), `pop` (dequeue), `front`, `empty`.

| Type | C++ | Note |
|---|---|---|
| Queue | `queue<int>` | FIFO |
| Deque | `deque<int>` | add/remove both ends |
| Priority Queue | `priority_queue<int>` | by priority (see Heap) |

```cpp
queue<int> q;
q.push(10); q.push(20);
int f = q.front();   // 10
q.pop();             // removes 10
```

**BFS template — level order traversal:**
```cpp
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;
    queue<TreeNode*> q; q.push(root);
    while (!q.empty()) {
        int sz = q.size();                   // nodes in THIS level
        vector<int> level;
        while (sz--) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        res.push_back(level);
    }
    return res;
}
```

**Sliding Window Maximum (monotonic deque):**
```cpp
vector<int> maxSlidingWindow(vector<int>& a, int k) {
    deque<int> dq;                           // indices, values decreasing
    vector<int> res;
    for (int i = 0; i < a.size(); i++) {
        if (!dq.empty() && dq.front() <= i - k) dq.pop_front();   // out of window
        while (!dq.empty() && a[dq.back()] < a[i]) dq.pop_back(); // smaller useless
        dq.push_back(i);
        if (i >= k - 1) res.push_back(a[dq.front()]);             // front = max
    }
    return res;
}
```
**Use when:** BFS / shortest path in unweighted graph, level order, scheduling, Rotting Oranges, Course Schedule (Kahn's), LRU (list + hashmap).

---

## 11. Heap / Priority Queue
**Idea:** Complete binary tree stored in an array. Min-heap: root is smallest. Max-heap: root is largest.
**Complexity:** push/pop O(log n), top O(1), build heap from n elements O(n).

```cpp
priority_queue<int> maxHeap;                              // DEFAULT = max-heap
priority_queue<int, vector<int>, greater<int>> minHeap;   // min-heap

minHeap.push(5); int t = minHeap.top(); minHeap.pop();

// With pairs: compares first, then second
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;   // (dist, node)
```

**Top K largest** (min-heap of size K — smallest of the K sits on top, so evict it):
```cpp
vector<int> kLargest(vector<int>& a, int k) {
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int x : a) {
        pq.push(x);
        if (pq.size() > k) pq.pop();         // drop the smallest
    }
    vector<int> res;
    while (!pq.empty()) { res.push_back(pq.top()); pq.pop(); }
    return res;
}
```
**Memory trick:** *K largest → min-heap. K smallest → max-heap.* Keeps O(n log k).

**Running Median (two heaps):**
```cpp
priority_queue<int> lo;                                   // max-heap: smaller half
priority_queue<int, vector<int>, greater<int>> hi;        // min-heap: larger half
void add(int x) {
    lo.push(x);
    hi.push(lo.top()); lo.pop();             // keep every lo <= every hi
    if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }   // balance sizes
}
double median() {
    return lo.size() > hi.size() ? lo.top() : (lo.top() + hi.top()) / 2.0;
}
```

| Problem | Heap |
|---|---|
| K Largest Elements | Min heap, size K |
| Merge K Sorted Lists | Min heap |
| Running Median | Min + Max heap |
| Task Scheduler | Max heap |
| Top K Frequent | Min heap / bucket sort |
| Dijkstra | Min heap |

---

## 12. K-Way Merge
**Idea:** Merge K sorted lists. A min-heap always holds the current smallest element of each list.

**Steps:**
1. Push the first element of each list as `(value, listIdx, elemIdx)`.
2. Pop the min → append to result.
3. Push the next element from the same list (if any).
4. Repeat until the heap is empty.

```cpp
vector<int> mergeK(vector<vector<int>>& lists) {
    using T = tuple<int,int,int>;                         // value, list, index
    priority_queue<T, vector<T>, greater<T>> pq;
    for (int i = 0; i < lists.size(); i++)
        if (!lists[i].empty()) pq.push({lists[i][0], i, 0});

    vector<int> res;
    while (!pq.empty()) {
        auto [val, li, ei] = pq.top(); pq.pop();
        res.push_back(val);
        if (ei + 1 < lists[li].size())
            pq.push({lists[li][ei + 1], li, ei + 1});
    }
    return res;
}
```
**Complexity:** O(N log K), N = total elements. Heap size ≤ K.
**Problems:** Merge K Sorted Lists, Kth Smallest in Sorted Matrix, Smallest Range Covering K Lists.

---

## 13. Trees
**Terms:** Root, Parent, Child, Leaf, Sibling, **Depth** (distance from root), **Height** (longest path down to a leaf), Subtree.

```cpp
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

| Type | Note |
|---|---|
| Binary Tree | ≤ 2 children |
| BST | Left < Root < Right (inorder = sorted) |
| AVL | Subtree heights differ ≤ 1 (self-balancing) |
| Red-Black | Self-balancing with color rules |
| N-ary | More than 2 children |
| Segment Tree | Range queries/updates |
| Heap | Complete binary tree |

### Traversals
```cpp
void inorder(TreeNode* r)   { if (!r) return; inorder(r->left); visit(r); inorder(r->right); }   // L Root R
void preorder(TreeNode* r)  { if (!r) return; visit(r); preorder(r->left); preorder(r->right); } // Root L R
void postorder(TreeNode* r) { if (!r) return; postorder(r->left); postorder(r->right); visit(r); } // L R Root
```
**BFS (level order)** → see Queue section.

**Common recursive pattern — Height / Max Depth:**
```cpp
int height(TreeNode* r) {
    if (!r) return 0;
    return 1 + max(height(r->left), height(r->right));
}
```
**Tip:** Most tree problems = "do something at the node + combine answers from left and right subtrees".

---

## 14. DFS (Depth First Search)
**Idea:** Go as deep as possible along one path, then backtrack.
**Use when:** backtracking, tree diameter, cycle detection, connected components, topological sort, maze/puzzles.

```cpp
// Recursive DFS on graph (adjacency list)
void dfs(int u, vector<vector<int>>& adj, vector<bool>& vis) {
    vis[u] = true;
    for (int v : adj[u])
        if (!vis[v]) dfs(v, adj, vis);
}

// Iterative DFS (explicit stack)
void dfsIter(int src, vector<vector<int>>& adj) {
    vector<bool> vis(adj.size(), false);
    stack<int> st; st.push(src);
    while (!st.empty()) {
        int u = st.top(); st.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (int v : adj[u]) if (!vis[v]) st.push(v);
    }
}
```
**Grid DFS (Number of Islands):**
```cpp
void dfsGrid(vector<vector<char>>& g, int i, int j) {
    if (i < 0 || j < 0 || i >= g.size() || j >= g[0].size() || g[i][j] != '1') return;
    g[i][j] = '0';                                        // mark visited
    dfsGrid(g, i+1, j); dfsGrid(g, i-1, j); dfsGrid(g, i, j+1); dfsGrid(g, i, j-1);
}
// count islands: for each cell == '1' → count++, dfsGrid(...)
```
**DFS vs BFS:** DFS → explore all paths, backtracking, recursion-friendly. BFS → shortest path in unweighted graphs, level-by-level.

---

## 15. Graphs
**G = (V, E)** — vertices (nodes) + edges (connections).

**Types:** Directed / Undirected · Weighted / Unweighted · Connected / Disconnected · Cyclic / Acyclic (DAG = Directed Acyclic Graph).

### Representations

| | Adjacency Matrix | Adjacency List |
|---|---|---|
| Space | O(V²) | O(V + E) |
| Edge check | O(1) | O(degree) |
| Best for | Dense graphs | Sparse graphs (most problems) |

```cpp
int n = 5;
// Matrix
vector<vector<int>> mat(n, vector<int>(n, 0));
mat[u][v] = 1;                               // edge u -> v (also mat[v][u] = 1 if undirected)

// List (unweighted)
vector<vector<int>> adj(n);
adj[u].push_back(v);                         // add adj[v].push_back(u) if undirected

// List (weighted): pair = {neighbor, weight}
vector<vector<pair<int,int>>> wadj(n);
wadj[u].push_back({v, w});
```

### BFS on graph (shortest path in unweighted graph)
```cpp
vector<int> bfs(int src, vector<vector<int>>& adj) {
    vector<int> dist(adj.size(), -1);
    queue<int> q; q.push(src); dist[src] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u])
            if (dist[v] == -1) { dist[v] = dist[u] + 1; q.push(v); }
    }
    return dist;
}
```

### Which algorithm?

| Problem | Algorithm |
|---|---|
| Shortest path, unweighted | BFS |
| Shortest path, weighted, non-negative | Dijkstra |
| Shortest path with negative weights | Bellman-Ford |
| Cycle in directed graph | DFS with recursion-stack / Kahn's |
| Dependencies / course schedule | Topological Sort |
| Count islands, clone graph, components | BFS / DFS |

---

## 16. Dijkstra's Algorithm
**Purpose:** Shortest distance from one source to all nodes. **Non-negative weights only.**
**Idea (greedy):** Always expand the closest not-yet-finalized node; relax its neighbors.

**Steps:**
1. `dist[src] = 0`, all others = ∞.
2. Push `{0, src}` into a min-heap.
3. Pop the smallest `{d, u}`. If `d > dist[u]`, skip (stale entry).
4. For each neighbor `v` with weight `w`: if `dist[u] + w < dist[v]` → update and push `{dist[v], v}`.
5. Repeat until the heap is empty.

```cpp
vector<int> dijkstra(int src, vector<vector<pair<int,int>>>& adj) {
    int n = adj.size();
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;   // {dist, node}
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;                        // stale entry
        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```
**Complexity:** O((V + E) log V).
**Why not negative weights?** A finalized node's distance could later be improved by a negative edge — greedy breaks.

---

## 17. Topological Sort
**Idea:** Linear ordering of a **DAG** such that for every edge `u → v`, `u` comes before `v`.
**Use when:** task scheduling, course prerequisites, build order, package dependencies.
**If a cycle exists → no valid ordering.**

### DFS-based
Visit all neighbors first, **then** push the node to a stack. Pop the stack for the order.
```cpp
void dfsTopo(int u, vector<vector<int>>& adj, vector<bool>& vis, stack<int>& st) {
    vis[u] = true;
    for (int v : adj[u]) if (!vis[v]) dfsTopo(v, adj, vis, st);
    st.push(u);                                           // after all descendants done
}
vector<int> topoDFS(int n, vector<vector<int>>& adj) {
    vector<bool> vis(n, false); stack<int> st;
    for (int i = 0; i < n; i++) if (!vis[i]) dfsTopo(i, adj, vis, st);
    vector<int> order;
    while (!st.empty()) { order.push_back(st.top()); st.pop(); }
    return order;
}
```

### BFS-based (Kahn's Algorithm)
1. Compute in-degree of every node.
2. Push all in-degree-0 nodes into the queue.
3. Pop → add to result → decrement neighbors' in-degree → push any that reach 0.
4. If `result.size() < n` → cycle detected.
```cpp
vector<int> topoKahn(int n, vector<vector<int>>& adj) {
    vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; u++) for (int v : adj[u]) indeg[v]++;
    queue<int> q;
    for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) if (--indeg[v] == 0) q.push(v);
    }
    return order;                                         // size < n  => cycle
}
```
**Example — Course Schedule:** can finish all courses ⇔ `topoKahn(...).size() == numCourses`.
**Complexity:** O(V + E).

---

## 18. Trie (Prefix Tree)
**Idea:** Tree where each node represents a character; a root-to-node path spells a prefix. Words sharing a prefix share nodes.
**Use when:** autocomplete, spell check, prefix search, word dictionary, longest prefix match (IP routing).

| Operation | Time |
|---|---|
| insert / search / startsWith | O(L), L = word length |

**Space:** O(26 × L × N) worst case. **Con:** high memory (26 pointers per node).
**vs HashSet:** Trie wins on *prefix* queries; HashSet is simpler for exact lookups.

```cpp
struct TrieNode {
    TrieNode* ch[26] = {};
    bool isEnd = false;
};

class Trie {
    TrieNode* root = new TrieNode();

    TrieNode* walk(const string& s) {                     // follow s; nullptr if path breaks
        TrieNode* cur = root;
        for (char c : s) {
            int i = c - 'a';
            if (!cur->ch[i]) return nullptr;
            cur = cur->ch[i];
        }
        return cur;
    }

    void collect(TrieNode* node, string& path, vector<string>& out) {   // DFS
        if (node->isEnd) out.push_back(path);
        for (int i = 0; i < 26; i++) {
            if (node->ch[i]) {
                path.push_back('a' + i);
                collect(node->ch[i], path, out);
                path.pop_back();                          // backtrack
            }
        }
    }

public:
    void insert(const string& w) {
        TrieNode* cur = root;
        for (char c : w) {
            int i = c - 'a';
            if (!cur->ch[i]) cur->ch[i] = new TrieNode();
            cur = cur->ch[i];
        }
        cur->isEnd = true;                                // mark end of a full word
    }
    bool search(const string& w) {
        TrieNode* n = walk(w);
        return n && n->isEnd;                             // must be a complete word
    }
    bool startsWith(const string& p) {
        return walk(p) != nullptr;                        // any word with this prefix
    }
    vector<string> getSuggestions(string prefix) {
        vector<string> out;
        TrieNode* n = walk(prefix);                       // Phase 1: go to prefix end
        if (n) collect(n, prefix, out);                   // Phase 2: DFS to gather words
        return out;
    }
};
```

---

## 19. Greedy
**Idea:** At each step pick the locally best option; never reconsider.
**Works only if:**
- **Greedy-choice property** — a local optimum leads to a global optimum.
- **Optimal substructure** — the remaining problem has the same form.

**Steps:** understand problem → define objective → make greedy choice → reduce problem → repeat.
**If you can't prove greedy works, think DP.** (e.g., Coin Change with arbitrary coins is DP; greedy fails for coins {1,3,4}, amount 6.)

**Example — Activity Selection:** sort by **end time**; pick the next activity that starts after the last picked one ends.
```cpp
int maxActivities(vector<pair<int,int>>& acts) {          // {start, end}
    sort(acts.begin(), acts.end(), [](auto& a, auto& b) { return a.second < b.second; });
    int count = 0, lastEnd = INT_MIN;
    for (auto& [s, e] : acts) {
        if (s >= lastEnd) { count++; lastEnd = e; }
    }
    return count;
}
```

**Example — Jump Game (can reach the end?):**
```cpp
bool canJump(vector<int>& a) {
    int far = 0;
    for (int i = 0; i < a.size(); i++) {
        if (i > far) return false;                        // stuck
        far = max(far, i + a[i]);
    }
    return true;
}
```
**Other classics:** Gas Station, Fractional Knapsack, Huffman Coding, Merge Intervals, Task Scheduler.

---

## 20. Dynamic Programming
**Idea:** Break into subproblems, solve each **once**, store the result, reuse it.
**Use when:** (1) **Overlapping subproblems** (same subproblem recomputed), (2) **Optimal substructure**.
**Clues:** "count ways", "min/max cost", "is it possible", "longest/shortest…", choices at each step.

### How to solve a DP problem
1. **State** — what does `dp[i]` (or `dp[i][j]`) mean?
2. **Transition** — how does `dp[i]` depend on smaller states?
3. **Base case** — smallest known answers.
4. **Order** — which direction to fill (or recurse).
5. **Answer** — which cell holds it.

| Approach | How | Time | Space |
|---|---|---|---|
| Top-Down (Memoization) | Recursion + cache | O(n) | O(n) + call stack |
| Bottom-Up (Tabulation) | Loop + dp table | O(n) | O(n) |
| Space Optimized | Keep only needed previous values | O(n) | O(1) |

*Names:* **Top-Down** = start from the big problem and break it down. **Bottom-Up** = start from base cases and build up. **Memoization** = "memo" (remember results). **Tabulation** = fill a table.

### Fibonacci in all three forms
```cpp
// 1. Top-Down (Memoization)
int fibMemo(int n, vector<int>& dp) {
    if (n <= 1) return n;
    if (dp[n] != -1) return dp[n];                        // already solved
    return dp[n] = fibMemo(n - 1, dp) + fibMemo(n - 2, dp);
}
// call: vector<int> dp(n + 1, -1); fibMemo(n, dp);

// 2. Bottom-Up (Tabulation)
int fibTab(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; i++) dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

// 3. Space Optimized
int fibOpt(int n) {
    if (n <= 1) return n;
    int prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; i++) {
        int curr = prev1 + prev2;
        prev2 = prev1; prev1 = curr;
    }
    return prev1;
}
```

### Another example — Climbing Stairs / House Robber
```cpp
// House Robber: dp[i] = max(dp[i-1], dp[i-2] + a[i])   (skip house i  OR  rob it)
int rob(vector<int>& a) {
    int prev2 = 0, prev1 = 0;
    for (int x : a) {
        int curr = max(prev1, prev2 + x);
        prev2 = prev1; prev1 = curr;
    }
    return prev1;
}
```

### Common DP families
| Family | Examples | State |
|---|---|---|
| 1D linear | Climbing Stairs, House Robber, Coin Change | `dp[i]` |
| 2D grid | Unique Paths, Min Path Sum | `dp[i][j]` |
| Subsequence | LCS, LIS, Edit Distance | `dp[i][j]` over two strings |
| Knapsack | 0/1 Knapsack, Subset Sum | `dp[i][w]` |

---

## 21. Backtracking
**Idea:** Build a solution one choice at a time. If the current path can't lead to a valid solution, **undo** and try the next option.
**Steps:** **Choose → Explore → Unchoose.**
**Use when:** all combinations / permutations / subsets, constraint puzzles (N-Queens, Sudoku), "generate all", "must not repeat".
**Pruning** (cutting invalid branches early) is what makes it faster than brute force.

### Subsets (include / exclude each number)
```
nums = [1,2]            []
                      /    \
                  [1]        []        <- decide on 1: include / skip
                 /   \      /   \
            [1,2]   [1]  [2]    []     <- decide on 2: include / skip
```
```cpp
void subsets(int i, vector<int>& nums, vector<int>& path, vector<vector<int>>& res) {
    if (i == nums.size()) { res.push_back(path); return; }
    path.push_back(nums[i]);                              // choose (include)
    subsets(i + 1, nums, path, res);                      // explore
    path.pop_back();                                      // unchoose
    subsets(i + 1, nums, path, res);                      // explore without it (skip)
}
```

### Permutations
```cpp
void permute(vector<int>& nums, vector<bool>& used, vector<int>& path, vector<vector<int>>& res) {
    if (path.size() == nums.size()) { res.push_back(path); return; }
    for (int i = 0; i < nums.size(); i++) {
        if (used[i]) continue;
        used[i] = true; path.push_back(nums[i]);          // choose
        permute(nums, used, path, res);                   // explore
        path.pop_back(); used[i] = false;                 // unchoose
    }
}
```

### Combination Sum (reuse allowed) — shows pruning
```cpp
void combSum(int start, int remain, vector<int>& c, vector<int>& path, vector<vector<int>>& res) {
    if (remain == 0) { res.push_back(path); return; }
    for (int i = start; i < c.size(); i++) {
        if (c[i] > remain) continue;                      // prune
        path.push_back(c[i]);
        combSum(i, remain - c[i], c, path, res);          // i (not i+1) => reuse allowed
        path.pop_back();
    }
}
```
**Dedup tip (duplicates in input):** sort first, then `if (i > start && nums[i] == nums[i-1]) continue;`.
**Classics:** N-Queens, Sudoku Solver, Subsets, Permutations, Combinations, Word Search, Rat in a Maze, Knight's Tour.
**Complexity:** often exponential — O(2ⁿ) for subsets, O(n!) for permutations.

---

## 22. Bit Manipulation
**Idea:** Operate directly on binary representation — very fast, O(1) per operation.

| Op | Symbol | Example (5=`0101`, 3=`0011`) |
|---|---|---|
| AND | `&` | `5 & 3 = 1` (`0001`) |
| OR | `\|` | `5 \| 3 = 7` (`0111`) |
| XOR | `^` | `5 ^ 3 = 6` (`0110`) |
| NOT | `~` | `~5 = -6` |
| Left shift | `<<` | `5 << 1 = 10` (× 2ⁿ) |
| Right shift | `>>` | `5 >> 1 = 2` (÷ 2ⁿ) |

**Key facts:** `x ^ x = 0`, `x ^ 0 = x`, `x & (x-1)` clears the lowest set bit.

| Task | Trick |
|---|---|
| Even / odd | `(n & 1) == 0` → even |
| Multiply / divide by 2 | `n << 1` / `n >> 1` |
| Check ith bit | `(n & (1 << i)) != 0` |
| Set ith bit | `n \| (1 << i)` |
| Clear ith bit | `n & ~(1 << i)` |
| Toggle ith bit | `n ^ (1 << i)` |
| Swap without temp | `a ^= b; b ^= a; a ^= b;` |

```cpp
// Power of 2 (only one bit set)
bool isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }

// Count set bits (Brian Kernighan) — loops once per set bit
int countSetBits(int n) {
    int count = 0;
    while (n) { n &= (n - 1); count++; }
    return count;
}
// built-in: __builtin_popcount(n)

// Single Number — every element appears twice except one (pairs cancel out via XOR)
int singleNumber(vector<int>& nums) {
    int res = 0;
    for (int x : nums) res ^= x;
    return res;
}

// Get / Set ith bit
int getBit(int n, int i) { return (n & (1 << i)) != 0; }
int setBit(int n, int i) { return n | (1 << i); }

// Subsets using bitmask: each mask in [0, 2^n) is one subset
for (int mask = 0; mask < (1 << n); mask++)
    for (int i = 0; i < n; i++)
        if (mask & (1 << i)) { /* nums[i] is in this subset */ }
```
**Tip:** use `1LL << i` when `i` can reach 31+ to avoid overflow.

---

## Quick Pattern Picker

| Clue in problem | Pattern |
|---|---|
| Sorted array, pair/triplet sum | Two Pointers |
| Cycle / middle of linked list | Fast & Slow |
| Contiguous subarray / substring | Sliding Window |
| Repeated range sums, subarray sum = K | Prefix Sum (+ HashMap) |
| Overlapping ranges / meetings | Merge Intervals |
| Sorted data / "min X such that…" | Binary Search |
| Frequency / seen before / lookup | HashMap |
| Nested brackets / next greater / undo | Stack |
| Level by level / unweighted shortest path | Queue (BFS) |
| Top K / running min-max / merge K | Heap |
| Prefix / autocomplete | Trie |
| Dependencies / ordering | Topological Sort |
| Weighted shortest path | Dijkstra |
| All combinations / permutations / subsets | Backtracking |
| Local best = global best | Greedy |
| Overlapping subproblems, count ways, min/max cost | DP |
| XOR / parity / masks / power of 2 | Bit Manipulation |
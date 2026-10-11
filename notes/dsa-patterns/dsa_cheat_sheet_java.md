# DSA Patterns — Quick Revision Cheat Sheet

---

## 1. Two Pointers
**Idea:** Two indices traverse the data to avoid nested loops (O(n²) → O(n)).
**Use when:** pairs/triplets/subarrays, sorted data (or sortable).

| Variant | Setup |
|---|---|
| Opposite ends | `left=0, right=n-1`, move inward (pair sum) |
| Same direction | both start at 0, one moves faster |

```java
while (left < right) {
    int sum = a[left] + a[right];
    if (sum == target) return true;
    else if (sum < target) left++;
    else right--;
}
```

---

## 2. Fast & Slow Pointers (Floyd's Tortoise-Hare)
**Idea:** slow moves 1 step, fast moves 2 steps.
**Use when:** cycle detection, middle of list, loop start, palindrome linked list, Happy Number, circular array loop.
**Clues:** repetition, possible cycle, need midpoint, O(1) space.

```java
ListNode slow = head, fast = head;
while (fast != null && fast.next != null) {
    slow = slow.next;
    fast = fast.next.next;
    if (slow == fast) return true;   // cycle
}
return false;
```

- **Middle:** when `fast` hits end, `slow` is the middle.
- **Cycle start:** after meeting, reset `slow = head`, move both 1 step → they meet at the start.
- **Complexity:** O(n) time, O(1) space.

---

## 3. Sliding Window
**Idea:** Maintain a window `[start, end]` over contiguous elements; update incrementally instead of recomputing.
**Use when:** subarray/substring, "longest/shortest/max/min", "contiguous", large n (10^5).

**Fixed size (k):** add new element, remove outgoing one.
```java
for (int i = k; i < n; i++) {
    sum += a[i] - a[i - k];
    max = Math.max(max, sum);
}
```

**Variable size:** expand `end`, shrink `start` while condition holds.
```java
for (int end = 0; end < n; end++) {
    sum += a[end];
    while (sum >= target) {
        minLen = Math.min(minLen, end - start + 1);
        sum -= a[start++];
    }
}
```
**Complexity:** O(n).

---

## 4. Prefix Sum
**Idea:** `prefix[i] = arr[0] + ... + arr[i]` → any range sum in O(1).

```java
prefix[0] = arr[0];
for (int i = 1; i < n; i++) prefix[i] = prefix[i-1] + arr[i];

// sum(i, j) = prefix[j] - (i > 0 ? prefix[i-1] : 0)
```
**Use when:** repeated range-sum queries, subarray sum = K (with HashMap), 2D prefix sums, difference arrays.
**Complexity:** O(n) preprocess, O(1) query.

---

## 5. Merge Intervals
**Idea:** Sort by start; merge if `current.start <= last.end`.
**Use when:** overlapping ranges, meetings, calendar, free time, insert interval, min meeting rooms.

```java
sort(intervals by start);
for (interval : intervals) {
    if (result.isEmpty() || result.last.end < interval.start)
        result.add(interval);
    else
        result.last.end = Math.max(result.last.end, interval.end);
}
```
Example: `[[1,3],[2,6],[8,10],[15,18]]` → `[[1,6],[8,10],[15,18]]`
**Complexity:** O(n log n).

---

## 6. Binary Search
**Idea:** Halve the search space each step. O(log n).

**Classic (sorted array):**
```java
int l = 0, r = n - 1;
while (l <= r) {
    int mid = l + (r - l) / 2;
    if (a[mid] == target) return mid;
    else if (a[mid] < target) l = mid + 1;
    else r = mid - 1;
}
```

**On Answer (min value satisfying condition):**
```java
int l = minPossible, r = maxPossible;
while (l < r) {
    int mid = l + (r - l) / 2;
    if (can(mid)) r = mid;      // try smaller
    else l = mid + 1;
}
return l;
```
**Clues:** sorted array, first/last occurrence, "minimum days / max capacity / smallest X such that…".
**Examples:** First Bad Version, Koko Eating Bananas, Ship Packages in D Days, Search in Rotated Array.

---

## 7. Sorting Algorithms

| Algorithm | Best | Avg | Worst | Space |
|---|---|---|---|---|
| Bubble | O(n) | O(n²) | O(n²) | O(1) |
| Selection | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion | O(n) | O(n²) | O(n²) | O(1) |
| Merge | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick | O(n log n) | O(n log n) | O(n²) | O(log n) |

- **Bubble:** swap adjacent out-of-order; largest bubbles to end.
- **Selection:** pick min of remaining, swap into position.
- **Insertion:** insert each element into sorted left part.
- **Merge:** divide halves, sort, merge (stable, guaranteed n log n).
- **Quick:** pick pivot, partition (left < pivot < right), recurse. Worst case on sorted input with bad pivot.

**When to use:** small/mostly sorted → Insertion · guaranteed n log n → Merge · fast in-place avg → Quick.

---

## 8. HashMap
**Idea:** key-value store, O(1) avg get/put/remove.
**Use when:** frequency count, duplicates, "seen before", index lookup, grouping, memoization.

```java
// Frequency
freq.put(c, freq.getOrDefault(c, 0) + 1);

// Two Sum
int comp = target - nums[i];
if (map.containsKey(comp)) return new int[]{map.get(comp), i};
map.put(nums[i], i);

// Group Anagrams
String key = new String(sortedChars);
map.computeIfAbsent(key, k -> new ArrayList<>()).add(word);
```
**Classic problems:** Two Sum, Group Anagrams, Longest Substring w/o Repeating, Top K Frequent, Subarray Sum = K.
**Tips:** use `getOrDefault`, `computeIfAbsent`; avoid mutable keys; `LinkedHashMap` = ordered, `ConcurrentHashMap` = thread-safe.

### HashMap Internals (Java)
- Array of `Node(hash, key, value, next)` = buckets.
- `hash = h ^ (h >>> 16)`, `index = (n-1) & hash`.
- **put:** empty bucket → insert; collision → traverse chain, overwrite if key exists else append.
- **get:** hash → bucket → traverse chain/tree.
- Collisions: chaining; list → Red-Black Tree when chain > 8.
- Resize when `size > capacity × load factor (0.75)`.
- Worst case O(n) (list) / O(log n) (tree). 1 null key, many null values allowed.

---

## 9. Stack (LIFO)
**Ops:** `push`, `pop`, `peek`, `isEmpty` — all O(1).
**Use when:** nested structures, undo/backtrack, next greater/smaller (monotonic stack), expression evaluation, reversal, backspace.

```java
Stack<Integer> st = new Stack<>();
st.push(10); st.peek(); st.pop();
```
**Problems:** Valid Parentheses, Next Greater Element, Evaluate RPN, Decode String (`3[a2[c]]`), Largest Rectangle in Histogram, Remove K Digits.
**Tips:** monotonic stack of indices for next greater/smaller; stack can replace recursion.

---

## 10. Queue (FIFO)
**Ops:** `offer` (enqueue), `poll` (dequeue), `peek`, `isEmpty`.
**Types:** Simple, Circular, Deque (both ends), Priority Queue (by priority).
**Use when:** BFS, level-order traversal, scheduling, sliding window max (Deque), LRU (Queue + HashMap).

```java
Queue<Integer> q = new LinkedList<>();
q.offer(10); q.peek(); q.poll();
```
**Problems:** Level Order Traversal, BFS shortest path, Rotting Oranges, Course Schedule (Kahn's), Sliding Window Maximum.

---

## 11. Heap / Priority Queue
**Idea:** Complete binary tree; Min Heap (root = smallest) / Max Heap (root = largest).
**Complexity:** insert/delete O(log n), peek min/max O(1).

```java
PriorityQueue<Integer> minHeap = new PriorityQueue<>();
PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Collections.reverseOrder());
```

| Problem | Heap |
|---|---|
| K Largest Elements | Min heap of size K |
| Merge K Sorted Lists | Min heap |
| Running Median | Min + Max heap |
| Task Scheduler | Max heap |
| Top K Frequent | Min heap / bucket sort |
| Dijkstra | Min heap |

**Use when:** top-K, frequent min/max access, merging sorted inputs, priority processing.

---

## 12. K-Way Merge
**Idea:** Merge K sorted lists using a min-heap of the current smallest from each list.

**Steps:**
1. Push first element of each list: `(value, listIdx, elemIdx)`.
2. Pop min → add to result.
3. Push next element from the same list (if any).
4. Repeat until heap is empty.

**Complexity:** O(N log K).

---

## 13. Trees
**Terms:** Root, Parent, Child, Leaf, Sibling, Depth (from root), Height (to deepest leaf), Subtree.

| Type | Note |
|---|---|
| Binary Tree | ≤ 2 children |
| BST | Left < Root < Right |
| AVL | Height diff ≤ 1 (self-balancing) |
| Red-Black | Self-balancing with colors |
| N-ary | > 2 children |
| Segment Tree | Range queries |
| Heap | Complete binary tree |

**Traversals:**
- **DFS:** Inorder (L, Root, R) · Preorder (Root, L, R) · Postorder (L, R, Root)
- **BFS:** Level order using a queue

---

## 14. DFS (Depth First Search)
**Idea:** Go as deep as possible, then backtrack.
**Types:** Recursive (call stack) / Iterative (explicit stack).
**Use when:** backtracking, tree diameter, cycle detection, connected components, topological sort, maze/puzzle solving.

```java
void dfs(int node) {
    visited[node] = true;
    for (int nei : adj[node])
        if (!visited[nei]) dfs(nei);
}
```

---

## 15. Graphs
**G = (V, E)** — vertices + edges.

**Types:** Directed/Undirected · Weighted/Unweighted · Connected/Disconnected · Cyclic/Acyclic (DAG).

| Representation | Space | Note |
|---|---|---|
| Adjacency Matrix | O(V²) | `adj[i][j] = 1` if edge |
| Adjacency List | O(V+E) | Most efficient, list of neighbors per node |

**Which algorithm?**

| Problem | Algorithm |
|---|---|
| Shortest path (weighted, non-negative) | Dijkstra |
| Negative weights | Bellman-Ford |
| Cycle in directed graph | DFS + visited/recursion stack |
| Course schedule / dependencies | Topological Sort |
| Count islands, clone graph | BFS / DFS |

---

## 16. Dijkstra's Algorithm
**Purpose:** Shortest path from source to all nodes; **non-negative weights only**. Greedy + Min-Heap.

**Steps:**
1. `dist[src] = 0`, others = ∞.
2. Push `(0, src)` into min-heap.
3. Pop smallest `(d, u)`.
4. For each neighbor `v`: if `dist[u] + w < dist[v]` → update and push `(dist[v], v)`.
5. Repeat until heap empty.

**Complexity:** O((V + E) log V).

---

## 17. Topological Sort
**Idea:** Linear ordering of a **DAG** such that for every edge `u → v`, `u` comes before `v`.
**Use when:** task scheduling, course prerequisites, build order, dependency resolution.

**DFS-based:** DFS each unvisited node → push to stack **after** visiting all neighbors → pop stack for order.

**BFS-based (Kahn's):**
1. Compute in-degree of all nodes.
2. Push nodes with in-degree 0 into queue.
3. Pop → add to result → reduce neighbors' in-degree → push if it becomes 0.
4. If result size < V → cycle exists.

---

## 18. Trie (Prefix Tree)
**Idea:** Tree where each node = a character; root-to-node path = prefix.
**Use when:** autocomplete, spell check, prefix search, dictionary problems, IP longest-prefix match.

| Operation | Time |
|---|---|
| insert / search / startsWith | O(L) |

Space: O(26 × L × N) · Con: high memory.

```java
class TrieNode {
    TrieNode[] children = new TrieNode[26];
    boolean isEnd;
}
// insert: for each ch → idx = ch - 'a'; create child if null; move down; mark isEnd = true
// search: traverse; return node != null && node.isEnd
// startsWith: traverse; return node != null
// getSuggestions: go to prefix node, then DFS collecting words where isEnd = true
```

---

## 19. Greedy
**Idea:** Pick the locally best option at each step, never undo.
**Works only if:** Greedy-choice property + Optimal substructure.
**Steps:** understand problem → define objective → make greedy choice → reduce problem → repeat.

**Example — Activity Selection:** sort by end time; always pick next activity whose start ≥ last picked end.

**Other classics:** Jump Game, Gas Station, Huffman Coding, Fractional Knapsack.

---

## 20. Dynamic Programming
**Use when:** Overlapping subproblems + Optimal substructure.
**Idea:** Solve each subproblem once, store, reuse.

| Approach | How | Time | Space |
|---|---|---|---|
| Top-Down (Memoization) | Recursion + cache | O(n) | O(n) |
| Bottom-Up (Tabulation) | Loop + dp table | O(n) | O(n) |
| Space Optimized | Keep only needed prev values | O(n) | O(1) |

**Top-Down:**
```java
int solve(int n, int[] dp) {
    if (n <= 1) return n;
    if (dp[n] != -1) return dp[n];
    return dp[n] = solve(n-1, dp) + solve(n-2, dp);
}
```

**Bottom-Up:**
```java
dp[0] = 0; dp[1] = 1;
for (int i = 2; i <= n; i++) dp[i] = dp[i-1] + dp[i-2];
```

**Space Optimized:**
```java
int prev2 = 0, prev1 = 1;
for (int i = 2; i <= n; i++) {
    int curr = prev1 + prev2;
    prev2 = prev1; prev1 = curr;
}
```

---

## 21. Backtracking
**Idea:** Build solution incrementally; abandon a path as soon as it's invalid.
**Steps:** **Choose → Explore → Unchoose.**
**Use when:** all combinations / permutations / subsets, constraint puzzles, "generate all solutions".

```java
void backtrack(List<Integer> path, int start) {
    result.add(new ArrayList<>(path));      // record
    for (int i = start; i < n; i++) {
        path.add(nums[i]);                  // choose
        backtrack(path, i + 1);             // explore
        path.remove(path.size() - 1);       // unchoose
    }
}
```
**Classics:** N-Queens, Sudoku Solver, Subsets, Permutations, Combinations, Word Search, Rat in a Maze, Knight's Tour.

---

## 22. Bit Manipulation

| Op | Symbol | Example |
|---|---|---|
| AND | `&` | `5 & 3 = 1` |
| OR | `\|` | `5 \| 3 = 7` |
| XOR | `^` | `5 ^ 3 = 6` |
| NOT | `~` | `~5 = -6` |
| Left shift | `<<` | `5 << 1 = 10` (×2ⁿ) |
| Right shift | `>>` | `5 >> 1 = 2` (÷2ⁿ) |

**Handy tricks:**

| Task | Trick |
|---|---|
| Even/odd | `n & 1` (0 = even) |
| Multiply / divide by 2 | `n << 1` / `n >> 1` |
| Check ith bit set | `(n & (1 << i)) != 0` |
| Set ith bit | `n \| (1 << i)` |
| Power of 2 | `n > 0 && (n & (n-1)) == 0` |
| Count set bits | `while (n > 0) { n &= (n-1); count++; }` |
| Swap without temp | `a ^= b; b ^= a; a ^= b;` |
| Single non-repeating number | XOR all elements (`x ^ x = 0`) |

---

## Quick Pattern Picker

| Clue in problem | Pattern |
|---|---|
| Sorted array, pair sum | Two Pointers |
| Cycle / middle of linked list | Fast & Slow |
| Contiguous subarray/substring | Sliding Window |
| Repeated range sums | Prefix Sum |
| Overlapping ranges | Merge Intervals |
| Sorted / "min X such that…" | Binary Search |
| Frequency / seen before / lookup | HashMap |
| Nested / next greater / undo | Stack |
| Level by level / BFS | Queue |
| Top K / min-max access / merge K | Heap |
| Prefix / autocomplete | Trie |
| Dependencies / ordering | Topological Sort |
| Weighted shortest path | Dijkstra |
| All combos / permutations | Backtracking |
| Local best = global best | Greedy |
| Overlapping subproblems | DP |
| XOR / parity / masks | Bit Manipulation |
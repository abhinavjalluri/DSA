# DSA Patterns — Complete Study Notes

> A comprehensive, revision-friendly reference for all major Data Structures & Algorithms patterns.

---

## Table of Contents

1. [Two Pointers Pattern](#1-two-pointers-pattern)
2. [Fast and Slow Pointers Pattern](#2-fast-and-slow-pointers-pattern)
3. [Sliding Window Pattern](#3-sliding-window-pattern)
4. [Prefix Sum Pattern](#4-prefix-sum-pattern)
5. [Merge Intervals Pattern](#5-merge-intervals-pattern)
6. [Binary Search Pattern](#6-binary-search-pattern)
7. [Sorting Algorithms](#7-sorting-algorithms)
8. [HashMap](#8-hashmap)
9. [Stack](#9-stack)
10. [Queue](#10-queue)
11. [Heap / Priority Queue](#11-heap--priority-queue)
12. [K-Way Merge Pattern](#12-k-way-merge-pattern)
13. [Trees](#13-trees)
14. [Depth First Search (DFS)](#14-depth-first-search-dfs)
15. [Graphs](#15-graphs)
16. [Dijkstra's Algorithm](#16-dijkstras-algorithm)
17. [Topological Sort](#17-topological-sort)
18. [Trie (Prefix Tree)](#18-trie-prefix-tree)
19. [Greedy Approach](#19-greedy-approach)
20. [Dynamic Programming (DP)](#20-dynamic-programming-dp)
21. [Backtracking](#21-backtracking)
22. [Bitwise Operations](#22-bitwise-operations)
23. [Quick Revision Cheat Sheet](#23-quick-revision-cheat-sheet)

---

## 1. Two Pointers Pattern

### What is it?
The Two Pointers technique uses **two indices (pointers)** to iterate over a data structure (usually an array or string) to solve problems efficiently by avoiding nested loops.

### When to Use?
- Finding pairs, triplets, or subarrays meeting certain conditions.
- When data is **sorted** or can be sorted.
- Optimizing brute force O(n²) solutions to **O(n)** or near-linear time.

### How It Works?
Maintain two pointers that move according to rules:
- **Opposite ends:** One at start, one at end (common for pair-sum problems).
- **Same direction:** Both start at beginning, one moves faster (sliding window).
- Move pointers toward each other or forward based on condition.

### Typical Approach
```
1. Initialize two pointers: left = 0, right = n - 1 (or both at 0)
2. Check condition based on current pointers.
3. Move pointers accordingly:
   - If condition not met → move left or right pointer.
   - If condition met → record answer / move pointers for more solutions.
4. Repeat until pointers cross or reach the end.
```

### Classic Example: Two Sum (Sorted Array)
```
left = 0, right = n - 1
while left < right:
    sum = arr[left] + arr[right]
    if sum == target: return [left, right]
    elif sum < target: left++
    else: right--
```

### Key Signals
- "Find a pair/triplet..."
- "Sorted array..."
- "Container with most water..."
- "Remove duplicates in-place..."

---

## 2. Fast and Slow Pointers Pattern

> Also known as **Floyd's Cycle Detection Algorithm** or **Tortoise and Hare**.

### What is it?
A two-pointer strategy where:
- **Slow Pointer** moves 1 step at a time.
- **Fast Pointer** moves 2 steps at a time.

Used for structures where elements are connected in a sequence (linked list, number transformations, circular arrays).

### Where It's Useful
- Cycle detection (linked lists, number sequences)
- Finding the middle of a list
- Detecting palindromes in linked lists
- Finding the start of a loop
- Math-based cycle problems (e.g., Happy Number)

### How to Recognize
| Signal | Description |
|--------|-------------|
| **Repetition** | Repeatedly transforming/traversing values |
| **Cycle possibility** | Structure/transformation could lead to a loop |
| **Need for midpoint** | Find middle element in one pass |
| **O(1) space required** | No extra memory allowed |

### Step-by-Step Application
```
Step 1: Initialize two pointers
    slow = head;
    fast = head;

Step 2: Traverse
    Move slow one step, fast two steps.
    while (fast != null && fast.next != null) {
        slow = slow.next;
        fast = fast.next.next;
    }

Step 3: Watch for meeting point
    If slow == fast → cycle exists
    If fast hits null → no cycle / list ended

Step 4: Additional logic
    - Cycle start: reset slow to head, move both one step until they meet.
    - Middle: return slow when fast hits end.
    - Happy Number: treat transformation as "next" movement.
```

### Benefits
| Benefit | Explanation |
|---------|-------------|
| **O(n) Time** | Linear traversal — no nested loops |
| **O(1) Space** | Only two pointers — no hashmap |
| **No modification** | Works on original structure |
| **Midpoint access** | Efficient middle-finding |
| **Cycle detection** | Elegant loop detection |

### Use Case Patterns
| Use Case | Question Type | What to Observe |
|----------|---------------|-----------------|
| Detect cycle | Linked List | Check if slow == fast |
| Find loop start | Linked List | Reset slow to head after meeting |
| Find middle | Linked List | Stop when fast == null |
| Happy Number | Integer Problem | Apply next-transform repeatedly |
| Circular array loop | Array + Modulo | Move with (i + nums[i]) % n |

---

## 3. Sliding Window Pattern

### Definition
Technique for problems involving **contiguous segments/subarrays** of an array or string. Maintains a window (range) defined by two pointers (`start`, `end`) that slide over the input.

- Window can be **fixed size** or **variable size**.
- Efficiently updates results by adding new element and removing old element.

### Where It Applies
- Finding something about subarrays/substrings of **contiguous** elements.
- Tracking a property over a moving range.
- Sums, counts, max/min, frequency inside a window.
- **Fixed-size:** "max sum of subarray of size k"
- **Variable-size:** "smallest subarray with sum >= target"

### Two Types

#### a) Fixed-Size Sliding Window
```
1. Initialize sum/frequency for first k elements.
2. Slide window by one:
   - Remove leftmost element.
   - Add new rightmost element.
   - Update result.
```

#### b) Variable-Size Sliding Window
```
1. Initialize start = 0, end = 0.
2. Expand end to include new elements, update tracking variables.
3. When condition met, shrink window by moving start forward.
4. Track best (min/max/count) window found so far.
```

### How to Identify
- Problem deals with **subarrays/substrings**.
- Needs max/min sum, longest/shortest substring, count of distinct elements.
- Keywords: **"window", "substring", "subarray", "contiguous"**.
- Naive solution uses nested loops → sliding window optimizes.
- Constraints large (10^5) where O(n²) is infeasible.

### Benefits
| Benefit | Description |
|---------|-------------|
| **Efficiency** | O(n²) → O(n) |
| **Simplicity** | Easy once understood |
| **Memory** | Constant or linear extra space |
| **Versatility** | Fixed & variable sizes |
| **Real-time** | Ideal for streaming data |

---

## 4. Prefix Sum Pattern

### Definition
Create a new array storing **cumulative sums** from start up to each index.

```
prefix[i] = arr[0] + arr[1] + ... + arr[i]
```

### Why Use It?
Quickly compute sum of any contiguous subarray in **O(1)** after **O(n)** preprocessing.

```
Without prefix sum: O(j - i + 1) per query
With prefix sum:
    sum(i, j) = prefix[j] - prefix[i-1]   (i > 0)
    sum(i, j) = prefix[j]                 (i == 0)
```

### How to Compute
```java
int[] prefix = new int[n];
prefix[0] = arr[0];
for (int i = 1; i < n; i++) {
    prefix[i] = prefix[i - 1] + arr[i];
}
```

### Applications
- Range sum queries
- Count of elements in a range satisfying a condition
- Finding subarrays with a certain sum
- 2D prefix sums for matrix sub-rectangle sums
- Difference arrays and range update queries
- Cumulative frequencies, histograms

### How to Identify
- Ask for sum of elements in a range/subarray **multiple times**.
- Fast repeated sum queries after initial array given.
- Count subarrays satisfying sum-related properties.
- Prefix-based conditions, cumulative constraints.

### Benefits
- Reduces repeated work in sum calculations.
- O(n²) range queries → O(n) preprocessing + O(1) query.
- Simplifies problem logic.
- Helps with subarray sums, histograms, range queries.

---

## 5. Merge Intervals Pattern

### Definition
Approach for dealing with a collection of **intervals** `[start, end]` where you need to:
- Merge all overlapping intervals, OR
- Process intervals efficiently handling overlaps.

**Example:**
```
Input:  [[1,3],[2,6],[8,10],[15,18]]
Output: [[1,6],[8,10],[15,18]]
```

### Where It Applies
- Calendar event merging
- Finding free time slots
- CPU scheduling
- Range compression
- Data compression (storing ranges)
- Genome sequencing / network IP ranges

### How to Apply
```
Step 1: Sort all intervals by start time.
Step 2: Iterate and merge when overlapping.
    - Initialize result with first interval.
    - For each next interval:
        If current.start <= last.end → overlap → merge (update end)
        Else → no overlap → add current to result.
```

**Pseudocode:**
```java
List<int[]> merge(List<int[]> intervals) {
    sort intervals by start;
    result = new empty list;
    for (interval in intervals) {
        if (result is empty || result.last.end < interval.start)
            add interval to result;
        else
            result.last.end = max(result.last.end, interval.end);
    }
    return result;
}
```

### How to Recognize
- Given list/array of intervals or ranges.
- Task involves merging, covering, minimizing overlaps, finding gaps.
- Keywords: **"merge", "combine", "overlap", "conflict", "meeting", "event", "calendar"**.

### Benefits
- **Efficiency:** O(N log N) via sort + linear scan.
- **Simplicity:** Process in order, never look back.
- **Versatility:** Adaptable for overlaps, gaps, union, intersection.

### Summary Table
| Step | Action |
|------|--------|
| 1. Sort | Sort intervals by start |
| 2. Iterate | Compare current with last merged |
| 3. Merge | Merge if overlapping, else add new |
| 4. Return | Return result list |

---

## 6. Binary Search Pattern

### Definition
Divide-and-conquer algorithm that repeatedly divides a **sorted array** or **search space** in half to find a target or optimal solution.

- **Time:** O(log n)
- **Space:** O(1) iterative, O(log n) recursive

### Two Major Contexts

#### A. Classic Binary Search on Arrays
Given sorted array, find element or condition.

#### B. Binary Search on Answer / Search Space
Search space is numeric and monotonic; find min/max possible value, first/last position satisfying condition.

### How to Identify
| Clue | Example |
|------|---------|
| Array is sorted | Find element in sorted array |
| Find first/last occurrence | First bad version |
| "Minimum days", "maximum capacity" | Ship within D days |
| Search within a range | Guess the number (1 to n) |

### Templates

**Classic Binary Search:**
```java
int left = 0, right = n - 1;
while (left <= right) {
    int mid = left + (right - left) / 2;
    if (condition(mid)) {
        // move left or right accordingly
    }
}
```

**Binary Search on Answer:**
```java
int left = minPossible, right = maxPossible;
while (left < right) {
    int mid = left + (right - left) / 2;
    if (can(mid)) {
        right = mid;       // try smaller value
    } else {
        left = mid + 1;    // try larger value
    }
}
return left;
```

### Benefits
| Benefit | Description |
|---------|-------------|
| **Fast** | O(log n) time |
| **Smart brute-force** | Try values smartly |
| **Custom conditions** | No exact target needed |
| **Memory** | In-place in most cases |

### Problem Categories
| Category | Example |
|----------|---------|
| Search in Sorted Array | Binary Search, Rotated Array |
| First/Last Position | First Bad Version, Peak Element |
| Min/Max Condition Value | Ship Packages, Koko Eating Bananas |
| Custom Function | Guess Number, H-index |
| Lower/Upper Bound | Insert Position, Floor/Ceil |

---

## 7. Sorting Algorithms

### 1. Bubble Sort
**Idea:** Repeatedly swap adjacent elements if wrong order.

**Steps:**
1. Traverse from index 0 to n-1.
2. Swap if `arr[i] > arr[i+1]`.
3. After each pass, largest element "bubbles up" to end.

| Case | Complexity |
|------|------------|
| Best | O(n) |
| Average | O(n²) |
| Worst | O(n²) |
| Space | O(1) |

**Examples:** Best `[1,2,3,4,5]`, Worst `[5,4,3,2,1]`

---

### 2. Selection Sort
**Idea:** Find minimum element and move it to correct position.

**Steps:**
1. For each position i, find minimum from i+1 to n-1.
2. Swap with `arr[i]`.

| Case | Complexity |
|------|------------|
| Best | O(n²) |
| Average | O(n²) |
| Worst | O(n²) |
| Space | O(1) |

---

### 3. Insertion Sort
**Idea:** Build sorted array one item at a time.

**Steps:**
1. Assume `arr[0]` is sorted.
2. Take `arr[i]`, insert into sorted left part at correct position.

| Case | Complexity |
|------|------------|
| Best | O(n) |
| Average | O(n²) |
| Worst | O(n²) |
| Space | O(1) |

**Examples:** Best `[1,2,3,4,5]`, Worst `[5,4,3,2,1]`

---

### 4. Merge Sort
**Idea:** Divide and conquer — divide, sort each part, merge.

**Steps:**
1. Divide array into halves until 1 element each.
2. Merge two sorted arrays back recursively.

| Case | Complexity |
|------|------------|
| Best | O(n log n) |
| Average | O(n log n) |
| Worst | O(n log n) |
| Space | O(n) |

---

### 5. Quick Sort
**Idea:** Pick a pivot, partition so left < pivot < right.

**Steps:**
1. Choose pivot (middle, last, random).
2. Partition array.
3. Recurse on left and right.

| Case | Complexity |
|------|------------|
| Best | O(n log n) |
| Average | O(n log n) |
| Worst | O(n²) |
| Space | O(log n) average |

**Examples:** Best random `[4,1,3,2,5]`, Worst `[1,2,3,4,5]` (pivot = first/last)

---

### Comparison Table

| Algorithm | Best | Average | Worst | Space |
|-----------|------|---------|-------|-------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) |

### When to Use What?
| Situation | Suggested Sort |
|-----------|----------------|
| Small input, mostly sorted | Insertion Sort |
| Guaranteed O(n log n) | Merge Sort |
| Average-case fast, in-place | Quick Sort |

---

## 8. HashMap

### What is it?
Data structure storing **key-value pairs** using hashing to compute bucket index.

```java
HashMap<KeyType, ValueType> map = new HashMap<>();
```

### Internal Working (High-Level)
1. Key → hash function → hash code.
2. Hash code mod capacity → bucket index.
3. Collision → chaining (linked list) or tree (high collisions).
4. Lookup/insertion: **O(1) average**.

### Where to Apply
- Fast lookups (O(1) average)
- Counting frequencies
- Checking existence
- Unique mappings (ID → value)
- Caching / memoization

### How to Apply
**Use when you see:** "count", "most", "duplicate", "frequency", "pair", "index lookup", "group by".

### Example Use Cases

**1. Count Frequency:**
```java
Map<Character, Integer> freq = new HashMap<>();
for (char c : str.toCharArray()) {
    freq.put(c, freq.getOrDefault(c, 0) + 1);
}
```

**2. Two Sum:**
```java
Map<Integer, Integer> map = new HashMap<>();
for (int i = 0; i < nums.length; i++) {
    int complement = target - nums[i];
    if (map.containsKey(complement)) {
        return new int[]{map.get(complement), i};
    }
    map.put(nums[i], i);
}
```

**3. Group Anagrams:**
```java
Map<String, List<String>> map = new HashMap<>();
for (String word : words) {
    char[] chars = word.toCharArray();
    Arrays.sort(chars);
    String key = new String(chars);
    map.computeIfAbsent(key, k -> new ArrayList<>()).add(word);
}
```

### How to Recognize
| Hint | Why |
|------|-----|
| "Find duplicates" | Track seen values |
| "Find frequency/count" | Use value as counter |
| "Get index of previous value" | Store index as value |
| "Fast lookups" | O(1) lookup |
| "Group by value" | Key = group, Value = List |
| "Check if seen before" | Use containsKey |

### Advantages
| Advantage | Explanation |
|-----------|-------------|
| O(1) lookup | Fast get/put/remove |
| Easy counting | Frequency maps |
| Memoization | Store intermediate results |
| Generic | Any type key-value |
| Backtracking/DFS | Avoid recomputation |

### Common LeetCode Questions
| Problem | Why HashMap? |
|---------|--------------|
| Two Sum | Index tracking |
| Group Anagrams | Group by sorted key |
| Longest Substring Without Repeating | Store last seen index |
| Top K Frequent Elements | Count frequency |
| Subarray Sum Equals K | Prefix sum tracking |

### Best Practices
- Use `getOrDefault()` for counting.
- Use `computeIfAbsent()` for List values.
- Be careful with mutable objects as keys.

### Summary Table
| Feature | HashMap |
|---------|---------|
| Access time | O(1) avg |
| Stores | Key-Value |
| Null allowed? | 1 null key, many null values |
| Ordered? | No (use LinkedHashMap) |
| Thread-safe? | No (use ConcurrentHashMap) |

---

### HashMap Internal Working (Deep Dive)

#### Internal Data Structure
- Array of `Node<K,V>` (buckets)
- Each node: `hash`, `key`, `value`, `next`

```java
static class Node<K,V> implements Map.Entry<K,V> {
    final int hash;
    final K key;
    V value;
    Node<K,V> next;
}
```

#### How `put(key, value)` Works
1. Compute hash: `hash = hash(key.hashCode());`
2. Index: `index = (n - 1) & hash;` (n = capacity)
3. Insert node:
   - No collision → add directly.
   - Collision → traverse list:
     - Key exists → overwrite value.
     - Key doesn't exist → append at end.

#### How `get(key)` Works
1. Compute hash and index.
2. Traverse bucket:
   - Key found → return value.
   - Else → return null.

#### Example
```java
Map<String, String> map = new HashMap<>();
map.put("A", "Apple");
map.put("B", "Banana");
map.put("A", "Avocado"); // Overwrites "A"
```

#### Time Complexity
| Operation | Average | Worst |
|-----------|---------|-------|
| put() | O(1) | O(log n) tree / O(n) list |
| get() | O(1) | O(log n) or O(n) |
| remove() | O(1) | O(log n) or O(n) |

#### Summary
| Concept | Details |
|---------|---------|
| Storage | Array of Node<K,V> |
| Collision | Chaining (list/tree) |
| Hash function | `(h = key.hashCode()) ^ (h >>> 16)` |
| Resize | When size > threshold (capacity * load factor) |
| Treeify | List → Red-Black Tree if collisions > 8 |
| Null keys/values | 1 null key, multiple null values |

---

## 9. Stack

### Definition
Linear data structure following **LIFO (Last In First Out)**.

**Basic Operations:**
- `push(x)` — add to top
- `pop()` — remove top
- `peek()` / `top()` — view top without removing
- `isEmpty()` — check empty

### Real-World Use Cases
- Browser history (back navigation)
- Undo functionality
- Reversing a string
- Expression evaluation (postfix, infix)
- Call stack in programming

### Coding Problem Use Cases
- Balanced Parentheses `()[]{}`
- Next Greater Element
- Evaluate Reverse Polish Notation
- Decode Strings `"3[a2[c]]"`
- Backspace String Compare
- Largest Rectangle in Histogram
- Remove K Digits
- Monotonic Stack problems

### When to Think of Stack
| Clue/Keyword | Suggests |
|--------------|----------|
| Nested structures `(([]))` | Balanced parentheses → Stack |
| "Undo", "Backtrack", "Previous State" | LIFO → Stack |
| "Next Greater/Smaller Element" | Monotonic Stack |
| "Evaluate expression" | Operator precedence → Stack |
| "Backspace", "Reversal" | Track last characters |
| "Track previous elements" | Stack of indices/values |

### Java Implementation
```java
Stack<Integer> stack = new Stack<>();
stack.push(10);
int top = stack.peek();
stack.pop();
boolean empty = stack.isEmpty();
```

**In problems:**
- `Stack<Integer>` for indices (monotonic)
- `Stack<Character>` for parsing
- `Stack<String>` for decoding

### Benefits
| Benefit | Description |
|---------|-------------|
| Simple & Fast | Push/pop O(1) |
| Natural for recursion | Manual call stack control |
| Memory-efficient | Keeps only what's needed |
| Easy to implement | Built-in most languages |

### Tips
- **Monotonic Stack** for "next greater/smaller element".
- **Classic Stack** for nested/reversal.
- **Stack of pairs** `(value, index)` for advanced tracking.
- Simulate recursion manually if depth is a concern.

---

## 10. Queue

### Definition
Linear data structure following **FIFO (First In First Out)**.

**Operations:**
| Operation | Description |
|-----------|-------------|
| enqueue() | Add to back |
| dequeue() | Remove from front |
| peek() | View front |
| isEmpty() | Check empty |

### Types of Queues
| Type | Description |
|------|-------------|
| Simple Queue | Basic FIFO |
| Circular Queue | Rear connects to front |
| Deque | Add/remove both ends |
| Priority Queue | Removed by priority |

### Where to Apply
| Application | Description |
|-------------|-------------|
| Task scheduling | OS processes, print queue |
| BFS | Tree/graph traversal |
| Level order traversal | Nodes at same level |
| Producer-Consumer | Thread-safe exchange |
| Sliding Window | Fixed-size window state |
| Web requests | Load balancing |
| Cache design (LRU) | Queue + HashMap |

### Java Implementation
```java
Queue<Integer> queue = new LinkedList<>();
queue.offer(10);
queue.offer(20);
System.out.println(queue.peek());  // 10
System.out.println(queue.poll());  // removes & returns 10
```

### How to Identify
| Pattern | Clue |
|---------|------|
| BFS / Level Order | Exploring level by level |
| First Come First Serve | Order preservation |
| Sliding Window | Fixed range in/out |
| Producer-Consumer | Thread-safe pipelines |
| Flood Fill / Shortest Path | Visit neighbors in order |

### Benefits
| Benefit | Description |
|---------|-------------|
| Order Preservation | Maintains insertion order |
| Efficient for BFS | Natural graph/tree traversal |
| Decouples producers/consumers | Async processing |
| Buffers | IO, print queues |
| Avoids recursion | Prevents stack overflow |

### Common Problems
| Problem | Description |
|---------|-------------|
| Level Order Traversal | Tree level by level |
| BFS in Graph | Shortest path |
| Sliding Window Maximum | Deque |
| Rotting Oranges | BFS infection spread |
| Course Schedule | Kahn's algorithm |
| LRU Cache | Queue + HashMap |

### Conclusion
> If you need to process elements **in arrival order**, **level-wise (BFS)**, or within a **fixed-size window**, consider a **Queue**.

---

## 11. Heap / Priority Queue

### Definition
A **complete binary tree** satisfying the heap property:
- **Min Heap:** Parent ≤ children → smallest at root.
- **Max Heap:** Parent ≥ children → largest at root.

Implemented using an **array**.

### Properties
| Property | Description |
|----------|-------------|
| Shape | Complete binary tree |
| Heap Order | Min or Max at root |
| Insert/Delete | O(log n) |
| Access Min/Max | O(1) |

### Use Cases
| Application | Explanation |
|-------------|-------------|
| Priority Queue | Efficient priority handling |
| Top K Elements | O(n log k) |
| Heap Sort | In-place sorting |
| Median of Stream | Min + Max heap combo |
| Dijkstra's | Smallest distance vertex |
| Merge K Sorted | Min-heap O(N log k) |

### How to Apply
Use heap when:
- Frequent access to min/max needed.
- Dynamic set with insertions + deletions.
- Top K / smallest K / largest K problems.
- Merging multiple sorted inputs.
- Priority-based processing.

### Java Implementation
```java
PriorityQueue<Integer> minHeap = new PriorityQueue<>();
PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Collections.reverseOrder());
```

### Benefits
| Benefit | Explanation |
|---------|-------------|
| Fast Access | O(1) to min/max |
| Efficient Updates | O(log n) insert/delete |
| Dynamic | Adapts to changes |
| Memory Efficient | In-place heap sort |
| Versatile | Sorting, streaming, scheduling |

### Example Problems
| Problem | Heap Used | Why |
|---------|-----------|-----|
| K Largest Elements | Min Heap size K | Maintain top K |
| Merge K Sorted Lists | Min Heap | Smallest head each time |
| Running Median | Two Heaps | Balance halves |
| Task Scheduler | Max Heap | Most frequent first |
| Top K Frequent | Min Heap / Bucket | Frequency priority |
| Dijkstra's | Min Heap | Shortest path node |

---

## 12. K-Way Merge Pattern

### Definition
Pattern to **merge K sorted lists/arrays** into one fully sorted list. Extends 2-way merge (like Merge Sort) to K arrays using a **min-heap**.

### Core Idea
1. Maintain min-heap of current smallest element from each list.
2. Extract minimum from heap.
3. Push next element from same list into heap.
4. Repeat until all merged.

### Example: Merge K Sorted Arrays

**Step-by-step:**
1. Initialize min-heap of size K.
2. Each entry: `(value, arrayIndex, elementIndex)`.
3. Push first element from each of K arrays.
4. While heap not empty:
   - Pop smallest.
   - Add to result.
   - Push next from that array (if exists).
5. Continue until all exhausted.

### Complexity
- **Time:** O(N log K) where N = total elements
- **Space:** O(K) for heap

---

## 13. Trees

### What is a Tree?
Non-linear, hierarchical data structure of nodes. Each node has:
- Data
- References to child nodes

### Terminology
| Term | Description |
|------|-------------|
| Root | Topmost node |
| Child | Node descending from another |
| Parent | Node with children |
| Leaf | Node with no children |
| Sibling | Nodes with same parent |
| Depth | Distance from root |
| Height | Longest path to leaf |
| Subtree | Tree within a tree |

### Types of Trees
| Type | Description |
|------|-------------|
| Binary Tree | At most 2 children |
| BST | Left < Root < Right |
| Balanced Tree | Self-balancing BST |
| AVL Tree | Subtree heights differ ≤ 1 |
| Red-Black Tree | Self-balancing with colors |
| N-ary Tree | More than 2 children |
| Segment Tree | Range queries |
| Heap | Complete binary tree for priority |

### Tree Traversals

**Depth-First Search (DFS):**
- **Inorder:** Left → Root → Right
- **Preorder:** Root → Left → Right
- **Postorder:** Left → Right → Root

**Breadth-First Search (BFS):**
- **Level Order:** Traverse level by level using a **queue**

---

## 14. Depth First Search (DFS)

### Definition
Graph/tree traversal algorithm that explores **as far as possible** along each branch before backtracking.

- Starts at root/source.
- Goes deep into each path.
- Backtracks when stuck, tries another path.

### Types
- **Recursive DFS:** Uses call stack.
- **Iterative DFS:** Uses explicit `Stack<Node>`.

### Where It Applies
- Exploring all paths deeply before others
- Backtracking (Sudoku, combinations, permutations)
- Tree/graph traversal (diameter, cycles)
- Finding connected components
- Topological Sorting
- Maze/puzzle solving

### Template (Recursive)
```java
void dfs(Node node, boolean[] visited) {
    visited[node] = true;
    for (Node neighbor : node.neighbors) {
        if (!visited[neighbor]) {
            dfs(neighbor, visited);
        }
    }
}
```

### Template (Iterative)
```java
void dfs(Node start) {
    Stack<Node> stack = new Stack<>();
    Set<Node> visited = new HashSet<>();
    stack.push(start);
    while (!stack.isEmpty()) {
        Node node = stack.pop();
        if (visited.contains(node)) continue;
        visited.add(node);
        for (Node neighbor : node.neighbors) {
            stack.push(neighbor);
        }
    }
}
```

---

## 15. Graphs

### What is a Graph?
Non-linear data structure of:
- **Vertices (Nodes)** — entities
- **Edges** — connections

`G = (V, E)` where V = vertices, E = edges.

### Types of Graphs
| Category | Type | Description |
|----------|------|-------------|
| Direction | Undirected | `(A—B)` |
| | Directed (Digraph) | `(A → B)` |
| Weight | Unweighted | All edges equal |
| | Weighted | Each edge has cost |
| Connectivity | Connected | All reachable |
| | Disconnected | Some unreachable |
| Cycles | Cyclic | Has at least one cycle |
| | Acyclic | No cycles |

### Representations

**Adjacency Matrix:**
- 2D array `graph[i][j] = 1` if edge
- Space: O(V²)

**Adjacency List:**
- List of lists (neighbors per node)
- Space: O(V + E) → most efficient

### Applications
| Domain | Use Case | Graph Type |
|--------|----------|------------|
| Maps & GPS | Shortest path | Directed Weighted |
| Internet | PageRank | Directed |
| Social Networks | Friends/followers | Undirected/Directed |
| E-Commerce | Recommendations | Weighted Undirected |
| Task Scheduling | Course/job scheduling | DAG |
| Puzzle Solving | State transitions | Graph traversal |
| Network Security | Packet routing | Graphs |

### When to Use Graphs
- Relationships/connections matter (roads, dependencies, networks)
- Modeling: routes, dependencies, networks, games/puzzles

### Example DSA Problems
| Problem Type | Algorithm |
|--------------|-----------|
| Shortest path | Dijkstra's / Bellman-Ford |
| Detect cycle in dependencies | DFS with visited[] |
| Can finish all courses | Topological Sort |
| Count islands | BFS / DFS |
| Clone a graph | BFS / DFS |

### Representations in Detail

**Adjacency Matrix:**
- n × n matrix
- `adjMat[i][j] = 1` if edge i→j, else 0

**Adjacency List:**
- Array of lists of size n
- `adjList[i]` contains neighbors of vertex i

---

## 16. Dijkstra's Algorithm

### What is it?
Finds **shortest path** from a source node to all other nodes in a **weighted, non-negative** graph.

### Key Concepts
- Only **non-negative weights**
- **Greedy** approach
- Uses **Priority Queue (Min-Heap)** for optimization

### Step-by-Step
```
1. Set dist[source] = 0, all others = ∞.
2. Push <0, source> into min heap.
3. Pop top element (smallest distance).
4. For each neighbor of current node:
   - Calculate dist[v] = dist[u] + weight[u][v]
   - If shorter than current dist[v], update it.
   - Push <dist[v], v> into min heap.
5. Repeat until heap is empty.
6. Return distance array.
```

### Template
```java
int[] dijkstra(int[][] graph, int src) {
    int n = graph.length;
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);
    dist[src] = 0;
    
    PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[0] - b[0]);
    pq.offer(new int[]{0, src});
    
    while (!pq.isEmpty()) {
        int[] curr = pq.poll();
        int d = curr[0], u = curr[1];
        if (d > dist[u]) continue;
        
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                pq.offer(new int[]{dist[v], v});
            }
        }
    }
    return dist;
}
```

### Complexity
- **Time:** O((V + E) log V) with binary heap
- **Space:** O(V)

---

## 17. Topological Sort

### Definition
Linear ordering of vertices in a **Directed Acyclic Graph (DAG)** such that for every edge `u → v`, `u` comes before `v`.

**Only applicable for DAGs.**

### Where Used
- Task Scheduling (dependencies)
- Course Prerequisites
- Compilation Order
- Build Systems (Makefile)
- Package Dependency Management

### Approaches

#### 1. DFS-Based (Using Stack)
**Key Idea:** Perform DFS, push node to stack on return. Pop stack for result.

**Steps:**
1. Create visited array.
2. For each unvisited node, call DFS.
3. In DFS: visit neighbors, then push node to stack.
4. Reverse stack (or print in pop order).

```java
void dfs(int node, boolean[] visited, Stack<Integer> stack, List<List<Integer>> adj) {
    visited[node] = true;
    for (int neighbor : adj.get(node)) {
        if (!visited[neighbor]) {
            dfs(neighbor, visited, stack, adj);
        }
    }
    stack.push(node);
}
```

#### 2. BFS-Based (Kahn's Algorithm)
**Key Idea:** Track in-degree, push in-degree=0 nodes to queue.

**Steps:**
1. Compute in-degree for all nodes.
2. Push in-degree=0 nodes into queue.
3. While queue not empty:
   - Pop node → add to result.
   - For each neighbor, reduce in-degree by 1.
   - If in-degree becomes 0, push to queue.

```java
List<Integer> topologicalSort(int n, List<List<Integer>> adj) {
    int[] inDegree = new int[n];
    for (int u = 0; u < n; u++) {
        for (int v : adj.get(u)) {
            inDegree[v]++;
        }
    }
    
    Queue<Integer> queue = new LinkedList<>();
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) queue.offer(i);
    }
    
    List<Integer> result = new ArrayList<>();
    while (!queue.isEmpty()) {
        int u = queue.poll();
        result.add(u);
        for (int v : adj.get(u)) {
            inDegree[v]--;
            if (inDegree[v] == 0) queue.offer(v);
        }
    }
    return result;
}
```

### Complexity
- **Time:** O(V + E)
- **Space:** O(V)

---

## 18. Trie (Prefix Tree)

### What is it?
Tree-like data structure storing strings where each node represents a character.

### Key Properties
- Nodes store characters.
- Paths from root to leaf represent words.
- Fast insertion, search, prefix matching.
- Powerful for strings with **shared prefixes**.

### Real-Life Use Cases
- Autocomplete
- Spell checkers
- IP routing (longest prefix match)

### Basic Operations
| Operation | Purpose | Time |
|-----------|---------|------|
| insert(word) | Add word | O(L) |
| search(word) | Check word exists | O(L) |
| startsWith(prefix) | Any word starts with prefix | O(L) |

### Complexity
| Operation | Time | Space |
|-----------|------|-------|
| Insert | O(L) | O(26 × L × N) |
| Search | O(L) | - |
| Prefix Search | O(L) | - |

- L = length of word/prefix
- N = number of words
- 26 = lowercase English letters

### Advantages
- Faster than HashMap/Set for prefix lookups.
- Keeps data in sorted-like structure.
- Great for dictionary-based problems.

### Limitations
- High memory usage (26 children per node).
- Worse for small datasets where HashSet is better.

### Implementation

```java
class TrieNode {
    TrieNode[] children = new TrieNode[26];
    boolean isEndOfWord = false;
}

class Trie {
    private TrieNode root;
    
    public Trie() {
        root = new TrieNode();
    }
    
    // 1. Insert
    public void insert(String word) {
        TrieNode node = root;
        for (char ch : word.toCharArray()) {
            int index = ch - 'a';
            if (node.children[index] == null) {
                node.children[index] = new TrieNode();
            }
            node = node.children[index];
        }
        node.isEndOfWord = true;
    }
    
    // 2. Search
    public boolean search(String word) {
        TrieNode node = root;
        for (char ch : word.toCharArray()) {
            int index = ch - 'a';
            if (node.children[index] == null) return false;
            node = node.children[index];
        }
        return node.isEndOfWord;
    }
    
    // 3. StartsWith
    public boolean startsWith(String prefix) {
        TrieNode node = root;
        for (char ch : prefix.toCharArray()) {
            int index = ch - 'a';
            if (node.children[index] == null) return false;
            node = node.children[index];
        }
        return true;
    }
}
```

### getSuggestions(prefix) — Autocomplete

**Phase 1: Traverse to End of Prefix**
1. Start from root.
2. For each character in prefix, move to child.
3. If missing, return empty list.

**Phase 2: Collect Words Using DFS**
1. Once prefix node reached, initialize result list.
2. DFS from this node.
3. If node marks end of word, add word to result.
4. Recursively visit all non-null children.
5. Return result list.

---

## 19. Greedy Approach

### Definition
Problem-solving strategy where **at each step, make the choice that looks best at the moment**, hoping it leads to global optimum.

Instead of considering all solutions (DP/Backtracking), greedy makes **locally optimal choices** step-by-step.

### Key Characteristics
| Feature | Description |
|---------|-------------|
| Local Optimization | Best option at each step |
| No Backtracking | Decisions never changed |
| Fast Execution | Usually O(n log n) or better |
| Doesn't Always Work | Needs greedy-choice + optimal substructure |

### Conditions for Applying Greedy
1. **Greedy Choice Property:** Local optimal choice leads to global optimal.
2. **Optimal Substructure:** Solution built from subproblem solutions.

### Real-Life Example: Activity Selection
**Problem:** Given n activities with start/end times, select max non-overlapping activities.

**Greedy Approach:**
1. Sort by end time.
2. Always pick next activity with earliest finish that doesn't overlap.

### Steps in Designing Greedy Algorithm
1. Understand problem structure.
2. Define objective function.
3. Make greedy choice (best current option).
4. Reduce the problem.
5. Repeat until solution built.

### When to Use Greedy
- Want a quick solution.
- Can prove greedy choice → optimal.
- Problem satisfies greedy-choice property + optimal substructure.

---

## 20. Dynamic Programming (DP)

### Definition
Powerful technique solving problems by:
1. Breaking into smaller subproblems.
2. Solving each subproblem **only once**.
3. Storing results (**memoization**) to avoid redundant work.

### When to Use DP
Problem has:
1. **Overlapping Subproblems** — subproblems solved multiple times (e.g., Fibonacci).
2. **Optimal Substructure** — optimal solution depends on optimal subproblem solutions.

**Why it helps:** Saves results in memory and reuses them → significant performance boost.

### Types of DP Approaches
| Type | Description | Example |
|------|-------------|---------|
| Top-Down (Memoization) | Recursion + store results | Fibonacci with dp[] |
| Bottom-Up (Tabulation) | Iterative, build from base | Fibonacci with loop |
| Space Optimization | Store only required results | Fibonacci with 2 variables |

---

### 1. Top-Down (Memoization)

**Why this name?**
- "Top-Down": Start from main problem, break down recursively.
- "Memoization": Store subproblem answers in `dp[]`.

**Characteristics:**
- Uses recursion.
- Uses cache (`dp[]`).
- Good if comfortable thinking recursively.

**Template:**
```java
int solve(int n, int[] dp) {
    if (n == 0 || n == 1) return n;
    if (dp[n] != -1) return dp[n];
    return dp[n] = solve(n - 1, dp) + solve(n - 2, dp);
}
```

**Flow:**
```
Start from fib(5)
    → fib(5) = fib(4) + fib(3)
    → recurse on fib(4), fib(3), ...
    → Store in dp[] during way down
```

---

### 2. Bottom-Up (Tabulation)

**Why this name?**
- "Bottom-Up": Solve smaller subproblems first.
- "Tabulation": Use table (array) bottom-up.

**Characteristics:**
- Uses loops (iteration), no recursion.
- Builds from base cases upward.
- More memory efficient (no call stack).

**Template:**
```java
int fib(int n) {
    int[] dp = new int[n + 1];
    dp[0] = 0;
    dp[1] = 1;
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}
```

**Flow:**
```
Build from dp[0] → dp[1] → dp[2] → ... → dp[n]
dp[0] = 0
dp[1] = 1
dp[2] = dp[1] + dp[0] = 1
...
dp[n] = result
```

---

### 3. Space Optimization

**Why this name?**
Optimize space by noticing only a few previous values needed.

**Characteristics:**
- Applies when current state depends on few previous states.
- Reduces space from O(n) → O(1).

**Template:**
```java
int fib(int n) {
    if (n <= 1) return n;
    int prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; i++) {
        int curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
```

---

### Comparison Table
| Approach | Time | Space | Notes |
|----------|------|-------|-------|
| Top-Down (Memoization) | O(n) | O(n) | Recursion + dp[] |
| Bottom-Up (Tabulation) | O(n) | O(n) | Iterative, no recursion |
| Space Optimization | O(n) | O(1) | Best for linear recurrences |

---

## 21. Backtracking

### What is it?
Recursive technique to **build a solution incrementally**, backtrack as soon as current path won't lead to valid solution.

### Key Idea
1. Try a possibility.
2. If doesn't work → backtrack and try next.
3. Repeat until solution found or all possibilities exhausted.

### Steps to Solve
1. **Choose:** Pick an option from valid set.
2. **Explore:** Recurse to explore further.
3. **Unchoose:** Undo choice (backtrack), try another.

### When to Use
- Problem requires all combinations/permutations/subsets.
- Decision making at each step (yes/no, pick/skip).
- Constraints: "must not repeat", "must be unique", "follow specific rules".
- Generate all solutions or find a valid one.

### Common Examples
| Problem | Type |
|---------|------|
| N-Queens | Constraint satisfaction |
| Sudoku Solver | Constraint satisfaction |
| Subsets / Combinations | Combinatorics |
| Word Search in Grid | Path finding |
| Permutations | Combinatorics |
| Rat in a Maze | Path finding |
| Knight's Tour | Chess-based puzzle |

### How It Works: Subset Generation
**Given:** `nums = [1, 2]`

At each level: include or exclude a number. Backtracking explores both choices.

```java
void backtrack(int[] nums, int index, List<Integer> current, List<List<Integer>> result) {
    if (index == nums.length) {
        result.add(new ArrayList<>(current));
        return;
    }
    // Exclude
    backtrack(nums, index + 1, current, result);
    // Include
    current.add(nums[index]);
    backtrack(nums, index + 1, current, result);
    current.remove(current.size() - 1); // backtrack
}
```

---

## 22. Bitwise Operations

### What Are They?
Operations directly on binary representations of integers. Fast and powerful for low-level tasks (bit masks, compression, graphics, cryptography).

### List of Operators
| Operator | Symbol | Description | Example |
|----------|--------|-------------|---------|
| AND | `&` | 1 if both bits 1 | `5 & 3 = 1` |
| OR | `\|` | 1 if at least one bit 1 | `5 \| 3 = 7` |
| XOR | `^` | 1 if bits different | `5 ^ 3 = 6` |
| NOT | `~` | Inverts all bits | `~5 = -6` |
| Left Shift | `<<` | Multiply by 2^n | `5 << 1 = 10` |
| Right Shift | `>>` | Divide by 2^n | `5 >> 1 = 2` |

### Binary Example (5 and 3)
```
5 = 0101
3 = 0011

5 & 3 = 0001 = 1
5 | 3 = 0111 = 7
5 ^ 3 = 0110 = 6
5 << 1 = 1010 = 10
5 >> 1 = 0010 = 2
```

### Operator Usage

**1. AND (`&`) — Clear a bit / mask check**
```java
int a = 5; // 0101
int b = 3; // 0011
System.out.println(a & b); // 1
```

**2. OR (`|`) — Set a bit**
```java
System.out.println(a | b); // 7
```

**3. XOR (`^`) — Toggle bits / swap without temp**
```java
System.out.println(a ^ b); // 6
```

**4. Left Shift (`<<`) — Multiply by 2^n**
```java
System.out.println(a << 1); // 10
```

**5. Right Shift (`>>`) — Divide by 2^n**
```java
System.out.println(a >> 1); // 2
```

### Use-Cases in Real Problems
| Use Case | Bitwise Trick |
|----------|---------------|
| Check even/odd | `n & 1 == 0` (even) |
| Multiply by 2 | `n << 1` |
| Divide by 2 | `n >> 1` |
| Swap numbers | `a ^= b; b ^= a; a ^= b;` |
| Check bit set | `(n & (1 << i)) != 0` |
| Count set bits | `n & (n - 1)` (Brian Kernighan) |

### Example Problems

**1. Check if power of 2:**
```java
boolean isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
```

**2. Count set bits:**
```java
int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        n = n & (n - 1); // removes last set bit
        count++;
    }
    return count;
}
```

**3. Find only non-repeating element:**
```java
int singleNumber(int[] nums) {
    int res = 0;
    for (int num : nums) {
        res ^= num;
    }
    return res;
}
```

**4. Get ith bit:**
```java
int getIthBit(int n, int i) {
    return (n & (1 << i)) != 0 ? 1 : 0;
}
```

**5. Set ith bit:**
```java
int setIthBit(int n, int i) {
    return n | (1 << i);
}
```

---

## 23. Quick Revision Cheat Sheet

### Pattern → When to Use → Complexity

| Pattern | Use When | Time | Space |
|---------|----------|------|-------|
| **Two Pointers** | Pairs, sorted array, avoid nested loops | O(n) | O(1) |
| **Fast & Slow Pointers** | Cycle detection, midpoint, O(1) space | O(n) | O(1) |
| **Sliding Window** | Contiguous subarray/substring | O(n) | O(1) or O(k) |
| **Prefix Sum** | Range sum queries, multiple queries | O(n) pre + O(1) query | O(n) |
| **Merge Intervals** | Overlapping intervals | O(n log n) | O(n) |
| **Binary Search** | Sorted array, search space, min/max | O(log n) | O(1) |
| **HashMap** | Fast lookup, frequency, grouping | O(1) avg | O(n) |
| **Stack** | LIFO, nested, next greater/smaller | O(n) | O(n) |
| **Queue** | FIFO, BFS, level order | O(n) | O(n) |
| **Heap** | Top K, priority, median | O(n log k) | O(k) |
| **K-Way Merge** | Merge K sorted | O(N log K) | O(K) |
| **DFS** | Deep exploration, backtracking | O(V+E) | O(V) |
| **BFS** | Level order, shortest path | O(V+E) | O(V) |
| **Dijkstra** | Shortest path, non-negative weights | O((V+E) log V) | O(V) |
| **Topological Sort** | DAG, dependencies | O(V+E) | O(V) |
| **Trie** | Prefix search, autocomplete | O(L) | O(26×L×N) |
| **Greedy** | Local optimal → global optimal | Varies | Varies |
| **DP** | Overlapping subproblems + optimal substructure | Varies | Varies |
| **Backtracking** | All solutions, constraint satisfaction | Exponential | O(n) |
| **Bitwise** | Low-level, masks, O(1) tricks | O(1) per op | O(1) |

### Sorting Algorithms Cheat Sheet

| Algorithm | Best | Average | Worst | Space | Stable |
|-----------|------|---------|-------|-------|--------|
| Bubble | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Selection | O(n²) | O(n²) | O(n²) | O(1) | No |
| Insertion | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Merge | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick | O(n log n) | O(n log n) | O(n²) | O(log n) | No |

### Data Structure Operations

| Structure | Access | Search | Insert | Delete |
|-----------|--------|--------|--------|--------|
| Array | O(1) | O(n) | O(n) | O(n) |
| Linked List | O(n) | O(n) | O(1) | O(1) |
| Stack | O(n) | O(n) | O(1) | O(1) |
| Queue | O(n) | O(n) | O(1) | O(1) |
| HashMap | O(1) | O(1) | O(1) | O(1) |
| Heap | O(1) min/max | O(n) | O(log n) | O(log n) |
| BST (balanced) | O(log n) | O(log n) | O(log n) | O(log n) |
| Trie | - | O(L) | O(L) | O(L) |

### Key Decision Flow

```
Problem involves...
├── Pairs / sorted array → Two Pointers
├── Cycle / midpoint → Fast & Slow Pointers
├── Contiguous subarray/substring → Sliding Window
├── Range sum queries → Prefix Sum
├── Overlapping intervals → Merge Intervals
├── Sorted / search space → Binary Search
├── Fast lookup / frequency → HashMap
├── LIFO / nested → Stack
├── FIFO / BFS → Queue
├── Top K / priority → Heap
├── K sorted lists → K-Way Merge
├── Deep exploration → DFS
├── Level-wise → BFS
├── Shortest path (non-neg) → Dijkstra
├── Dependencies → Topological Sort
├── Prefix search → Trie
├── Local optimal → Greedy
├── Overlapping subproblems → DP
├── All solutions → Backtracking
└── Bit manipulation → Bitwise Operations
```

---

> **End of Notes** — Happy revising! 🚀
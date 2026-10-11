# DSA Patterns — Complete Study Notes (C++)

> A comprehensive, revision-friendly reference for all major Data Structures & Algorithms patterns in **C++**.

---

## Table of Contents

1. [Two Pointers Pattern](#1-two-pointers-pattern)
2. [Fast and Slow Pointers Pattern](#2-fast-and-slow-pointers-pattern)
3. [Sliding Window Pattern](#3-sliding-window-pattern)
4. [Prefix Sum Pattern](#4-prefix-sum-pattern)
5. [Merge Intervals Pattern](#5-merge-intervals-pattern)
6. [Binary Search Pattern](#6-binary-search-pattern)
7. [Sorting Algorithms](#7-sorting-algorithms)
8. [HashMap / unordered_map](#8-hashmap--unordered_map)
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
```cpp
#include <vector>
using namespace std;

vector<int> twoSum(vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == target) return {left, right};
        else if (sum < target) left++;
        else right--;
    }
    return {};
}
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
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }

Step 3: Watch for meeting point
    If slow == fast → cycle exists
    If fast hits nullptr → no cycle / list ended

Step 4: Additional logic
    - Cycle start: reset slow to head, move both one step until they meet.
    - Middle: return slow when fast hits end.
    - Happy Number: treat transformation as "next" movement.
```

### C++ Implementation

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// Detect cycle
bool hasCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// Find cycle start
ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    return nullptr;
}

// Find middle
ListNode* middleNode(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}
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
| Find middle | Linked List | Stop when fast == nullptr |
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

```cpp
// Max sum of subarray of size k
int maxSumFixedWindow(vector<int>& arr, int k) {
    int n = arr.size();
    int windowSum = 0;
    for (int i = 0; i < k; i++) windowSum += arr[i];
    
    int maxSum = windowSum;
    for (int i = k; i < n; i++) {
        windowSum += arr[i] - arr[i - k];
        maxSum = max(maxSum, windowSum);
    }
    return maxSum;
}
```

#### b) Variable-Size Sliding Window
```
1. Initialize start = 0, end = 0.
2. Expand end to include new elements, update tracking variables.
3. When condition met, shrink window by moving start forward.
4. Track best (min/max/count) window found so far.
```

```cpp
// Smallest subarray with sum >= target
int minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size();
    int left = 0, sum = 0, minLen = INT_MAX;
    for (int right = 0; right < n; right++) {
        sum += nums[right];
        while (sum >= target) {
            minLen = min(minLen, right - left + 1);
            sum -= nums[left++];
        }
    }
    return minLen == INT_MAX ? 0 : minLen;
}
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
```cpp
vector<int> prefix(n);
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

### Example: Subarray Sum Equals K
```cpp
#include <unordered_map>

int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> prefixCount;
    prefixCount[0] = 1;
    int sum = 0, count = 0;
    for (int num : nums) {
        sum += num;
        if (prefixCount.count(sum - k)) count += prefixCount[sum - k];
        prefixCount[sum]++;
    }
    return count;
}
```

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

### C++ Implementation
```cpp
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> merge(vector<vector<int>>& intervals) {
    if (intervals.empty()) return {};
    
    sort(intervals.begin(), intervals.end(),
         [](const vector<int>& a, const vector<int>& b) {
             return a[0] < b[0];
         });
    
    vector<vector<int>> result;
    result.push_back(intervals[0]);
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] <= result.back()[1]) {
            // Overlap → merge
            result.back()[1] = max(result.back()[1], intervals[i][1]);
        } else {
            // No overlap → add new
            result.push_back(intervals[i]);
        }
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
```cpp
int left = 0, right = n - 1;
while (left <= right) {
    int mid = left + (right - left) / 2;
    if (condition(mid)) {
        // move left or right accordingly
    }
}
```

**Binary Search on Answer:**
```cpp
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

### Example: Standard Binary Search
```cpp
#include <vector>
using namespace std;

int binarySearch(vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

### Example: Binary Search on Answer (Koko Eating Bananas)
```cpp
#include <vector>
#include <algorithm>
using namespace std;

bool canEatAll(vector<int>& piles, int k, int h) {
    int hours = 0;
    for (int pile : piles) {
        hours += (pile + k - 1) / k;  // ceil division
    }
    return hours <= h;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int left = 1, right = *max_element(piles.begin(), piles.end());
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (canEatAll(piles, mid, h)) right = mid;
        else left = mid + 1;
    }
    return left;
}
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

### C++ Built-in Functions
```cpp
// Lower bound: first element >= target
auto lb = lower_bound(arr.begin(), arr.end(), target);

// Upper bound: first element > target
auto ub = upper_bound(arr.begin(), arr.end(), target);

// Binary search existence
bool exists = binary_search(arr.begin(), arr.end(), target);
```

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

```cpp
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;  // optimization
    }
}
```

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

```cpp
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        swap(arr[i], arr[minIdx]);
    }
}
```

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

```cpp
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```

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

```cpp
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (int i = 0; i < temp.size(); i++) {
        arr[left + i] = temp[i];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
```

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

```cpp
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
```

**Examples:** Best random `[4,1,3,2,5]`, Worst `[1,2,3,4,5]` (pivot = first/last)

---

### C++ STL Sort
```cpp
#include <algorithm>

sort(arr.begin(), arr.end());                          // ascending
sort(arr.begin(), arr.end(), greater<int>());          // descending
sort(arr.begin(), arr.end(), [](int a, int b) {        // custom
    return a > b;
});
```

---

### Comparison Table

| Algorithm | Best | Average | Worst | Space |
|-----------|------|---------|-------|-------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) |
| `std::sort` | O(n log n) | O(n log n) | O(n log n) | O(log n) |

### When to Use What?
| Situation | Suggested Sort |
|-----------|----------------|
| Small input, mostly sorted | Insertion Sort |
| Guaranteed O(n log n) | Merge Sort |
| Average-case fast, in-place | Quick Sort |
| General purpose | `std::sort` |

---

## 8. HashMap / unordered_map

### What is it?
Data structure storing **key-value pairs** using hashing to compute bucket index.

```cpp
#include <unordered_map>
unordered_map<KeyType, ValueType> map;
```

### Internal Working (High-Level)
1. Key → hash function → hash code.
2. Hash code mod bucket count → bucket index.
3. Collision → chaining (linked list) or open addressing.
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
```cpp
unordered_map<char, int> freq;
for (char c : str) {
    freq[c]++;
}
```

**2. Two Sum:**
```cpp
vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> map;
    for (int i = 0; i < nums.size(); i++) {
        int complement = target - nums[i];
        if (map.count(complement)) {
            return {map[complement], i};
        }
        map[nums[i]] = i;
    }
    return {};
}
```

**3. Group Anagrams:**
```cpp
#include <map>
#include <vector>
#include <string>
#include <algorithm>

vector<vector<string>> groupAnagrams(vector<string>& words) {
    unordered_map<string, vector<string>> map;
    for (const string& word : words) {
        string key = word;
        sort(key.begin(), key.end());
        map[key].push_back(word);
    }
    vector<vector<string>> result;
    for (auto& [key, group] : map) {
        result.push_back(group);
    }
    return result;
}
```

### How to Recognize
| Hint | Why |
|------|-----|
| "Find duplicates" | Track seen values |
| "Find frequency/count" | Use value as counter |
| "Get index of previous value" | Store index as value |
| "Fast lookups" | O(1) lookup |
| "Group by value" | Key = group, Value = vector |
| "Check if seen before" | Use `count()` or `find()` |

### Advantages
| Advantage | Explanation |
|-----------|-------------|
| O(1) lookup | Fast insert/erase/find |
| Easy counting | Frequency maps |
| Memoization | Store intermediate results |
| Generic | Any hashable type |
| Backtracking/DFS | Avoid recomputation |

### Common LeetCode Questions
| Problem | Why Hash Map? |
|---------|--------------|
| Two Sum | Index tracking |
| Group Anagrams | Group by sorted key |
| Longest Substring Without Repeating | Store last seen index |
| Top K Frequent Elements | Count frequency |
| Subarray Sum Equals K | Prefix sum tracking |

### Best Practices
- Use `map[key]++` for counting (default-initializes to 0).
- Use `map.count(key)` or `map.find(key)` before accessing.
- Be careful with custom types as keys (need hash function).

### Summary Table
| Feature | `unordered_map` | `map` |
|---------|-----------------|-------|
| Access time | O(1) avg | O(log n) |
| Order | Unordered | Sorted by key |
| Underlying DS | Hash table | Red-Black Tree |
| Null allowed? | N/A (C++) | N/A (C++) |

---

### HashMap Internal Working (Deep Dive)

#### Internal Data Structure
- Array of buckets (linked lists or trees).
- Each entry: `key`, `value`, `hash`, pointer to next.

#### How `insert(key, value)` Works
1. Compute hash: `std::hash<Key>()(key)`.
2. Index: `hash % bucket_count`.
3. Insert:
   - No collision → add directly.
   - Collision → traverse list:
     - Key exists → overwrite value.
     - Key doesn't exist → append.

#### How `find(key)` Works
1. Compute hash and index.
2. Traverse bucket:
   - Key found → return iterator.
   - Else → return `end()`.

#### Example
```cpp
#include <unordered_map>
#include <string>
using namespace std;

unordered_map<string, string> map;
map["A"] = "Apple";
map["B"] = "Banana";
map["A"] = "Avocado";  // Overwrites "A"
```

#### Time Complexity
| Operation | Average | Worst |
|-----------|---------|-------|
| insert() | O(1) | O(n) |
| find() | O(1) | O(n) |
| erase() | O(1) | O(n) |

#### Summary
| Concept | Details |
|---------|---------|
| Storage | Array of buckets |
| Collision | Chaining or open addressing |
| Hash function | `std::hash<Key>` |
| Rehash | When load factor exceeded |
| Null keys/values | Allowed (except `map[key]` on non-default-constructible) |

---

## 9. Stack

### Definition
Linear data structure following **LIFO (Last In First Out)**.

**Basic Operations:**
- `push(x)` — add to top
- `pop()` — remove top
- `top()` — view top without removing
- `empty()` — check empty

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

### C++ Implementation
```cpp
#include <stack>

stack<int> st;
st.push(10);
int top = st.top();
st.pop();
bool empty = st.empty();
```

**In problems:**
- `stack<int>` for indices (monotonic)
- `stack<char>` for parsing
- `stack<string>` for decoding

### Example: Balanced Parentheses
```cpp
#include <stack>
#include <string>
using namespace std;

bool isValid(string s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            char top = st.top();
            st.pop();
            if ((c == ')' && top != '(') ||
                (c == '}' && top != '{') ||
                (c == ']' && top != '[')) return false;
        }
    }
    return st.empty();
}
```

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
| push() | Add to back |
| pop() | Remove from front |
| front() | View front |
| empty() | Check empty |

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

### C++ Implementation
```cpp
#include <queue>

queue<int> q;
q.push(10);
q.push(20);
cout << q.front();  // 10
q.pop();            // removes 10
```

### How to Identify
| Pattern | Clue |
|---------|------|
| BFS / Level Order | Exploring level by level |
| First Come First Serve | Order preservation |
| Sliding Window | Fixed range in/out |
| Producer-Consumer | Thread-safe pipelines |
| Flood Fill / Shortest Path | Visit neighbors in order |

### Example: BFS Level Order Traversal
```cpp
#include <queue>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;
    
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        vector<int> level;
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}
```

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

### C++ Implementation

```cpp
#include <queue>
#include <vector>
using namespace std;

// Min Heap (default)
priority_queue<int, vector<int>, greater<int>> minHeap;

// Max Heap (default for priority_queue)
priority_queue<int> maxHeap;

// Custom comparator
auto cmp = [](int a, int b) { return a > b; };  // min-heap
priority_queue<int, vector<int>, decltype(cmp)> customHeap(cmp);
```

### Example: Top K Frequent Elements
```cpp
#include <unordered_map>
#include <queue>
#include <vector>
using namespace std;

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int num : nums) freq[num]++;
    
    // Min-heap of pairs (frequency, element)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    for (auto& [num, count] : freq) {
        pq.push({count, num});
        if (pq.size() > k) pq.pop();
    }
    
    vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().second);
        pq.pop();
    }
    return result;
}
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

### Example: Merge K Sorted Lists
```cpp
#include <queue>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;  // min-heap
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, Compare> pq;
    
    // Push first node of each list
    for (ListNode* node : lists) {
        if (node) pq.push(node);
    }
    
    ListNode dummy(0);
    ListNode* tail = &dummy;
    
    while (!pq.empty()) {
        ListNode* node = pq.top(); pq.pop();
        tail->next = node;
        tail = tail->next;
        if (node->next) pq.push(node->next);
    }
    return dummy.next;
}
```

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

### C++ Node Definition
```cpp
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

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

```cpp
// Inorder
void inorder(TreeNode* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->val << " ";
    inorder(root->right);
}

// Preorder
void preorder(TreeNode* root) {
    if (!root) return;
    cout << root->val << " ";
    preorder(root->left);
    preorder(root->right);
}

// Postorder
void postorder(TreeNode* root) {
    if (!root) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->val << " ";
}
```

---

## 14. Depth First Search (DFS)

### Definition
Graph/tree traversal algorithm that explores **as far as possible** along each branch before backtracking.

- Starts at root/source.
- Goes deep into each path.
- Backtracks when stuck, tries another path.

### Types
- **Recursive DFS:** Uses call stack.
- **Iterative DFS:** Uses explicit `stack<Node>`.

### Where It Applies
- Exploring all paths deeply before others
- Backtracking (Sudoku, combinations, permutations)
- Tree/graph traversal (diameter, cycles)
- Finding connected components
- Topological Sorting
- Maze/puzzle solving

### Template (Recursive)
```cpp
void dfs(Node* node, vector<bool>& visited) {
    visited[node->val] = true;
    for (Node* neighbor : node->neighbors) {
        if (!visited[neighbor->val]) {
            dfs(neighbor, visited);
        }
    }
}
```

### Template (Iterative)
```cpp
void dfs(Node* start) {
    stack<Node*> st;
    unordered_set<Node*> visited;
    st.push(start);
    while (!st.empty()) {
        Node* node = st.top(); st.pop();
        if (visited.count(node)) continue;
        visited.insert(node);
        for (Node* neighbor : node->neighbors) {
            st.push(neighbor);
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
- Vector of vectors (neighbors per node)
- Space: O(V + E) → most efficient

### C++ Implementations

**Adjacency Matrix:**
```cpp
int n = 5;
vector<vector<int>> adjMatrix(n, vector<int>(n, 0));
adjMatrix[0][1] = 1;
adjMatrix[1][0] = 1;
```

**Adjacency List:**
```cpp
int n = 5;
vector<vector<int>> adjList(n);
adjList[0].push_back(1);
adjList[1].push_back(0);
```

**Weighted Adjacency List:**
```cpp
vector<vector<pair<int, int>>> adjList(n);  // {neighbor, weight}
adjList[0].push_back({1, 5});
```

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
2. Push {0, source} into min heap.
3. Pop top element (smallest distance).
4. For each neighbor of current node:
   - Calculate dist[v] = dist[u] + weight[u][v]
   - If shorter than current dist[v], update it.
   - Push {dist[v], v} into min heap.
5. Repeat until heap is empty.
6. Return distance array.
```

### C++ Implementation
```cpp
#include <queue>
#include <vector>
#include <climits>
using namespace std;

vector<int> dijkstra(int n, vector<vector<pair<int, int>>>& adj, int src) {
    vector<int> dist(n, INT_MAX);
    dist[src] = 0;
    
    // Min-heap: {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        
        for (auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
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

```cpp
#include <stack>
#include <vector>
using namespace std;

void dfs(int node, vector<bool>& visited, stack<int>& st,
         vector<vector<int>>& adj) {
    visited[node] = true;
    for (int neighbor : adj[node]) {
        if (!visited[neighbor]) {
            dfs(neighbor, visited, st, adj);
        }
    }
    st.push(node);
}

vector<int> topologicalSortDFS(int n, vector<vector<int>>& adj) {
    vector<bool> visited(n, false);
    stack<int> st;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) dfs(i, visited, st, adj);
    }
    
    vector<int> result;
    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }
    return result;
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

```cpp
#include <queue>
#include <vector>
using namespace std;

vector<int> topologicalSortBFS(int n, vector<vector<int>>& adj) {
    vector<int> inDegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            inDegree[v]++;
        }
    }
    
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    vector<int> result;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        result.push_back(u);
        for (int v : adj[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
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

### C++ Implementation

```cpp
#include <string>
using namespace std;

class TrieNode {
public:
    TrieNode* children[26];
    bool isEndOfWord;
    
    TrieNode() {
        for (int i = 0; i < 26; i++) children[i] = nullptr;
        isEndOfWord = false;
    }
};

class Trie {
private:
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    // 1. Insert
    void insert(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!node->children[index]) {
                node->children[index] = new TrieNode();
            }
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }
    
    // 2. Search
    bool search(string word) {
        TrieNode* node = root;
        for (char ch : word) {
            int index = ch - 'a';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return node->isEndOfWord;
    }
    
    // 3. StartsWith
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            int index = ch - 'a';
            if (!node->children[index]) return false;
            node = node->children[index];
        }
        return true;
    }
    
    // 4. getSuggestions (autocomplete)
    void dfs(TrieNode* node, string& current, vector<string>& result) {
        if (node->isEndOfWord) result.push_back(current);
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                current.push_back('a' + i);
                dfs(node->children[i], current, result);
                current.pop_back();
            }
        }
    }
    
    vector<string> getSuggestions(string prefix) {
        TrieNode* node = root;
        for (char ch : prefix) {
            int index = ch - 'a';
            if (!node->children[index]) return {};
            node = node->children[index];
        }
        vector<string> result;
        string current = prefix;
        dfs(node, current, result);
        return result;
    }
};
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

### C++ Implementation
```cpp
#include <vector>
#include <algorithm>
using namespace std;

int activitySelection(vector<pair<int, int>>& activities) {
    // Sort by end time
    sort(activities.begin(), activities.end(),
         [](auto& a, auto& b) { return a.second < b.second; });
    
    int count = 0;
    int lastEnd = -1;
    for (auto& [start, end] : activities) {
        if (start >= lastEnd) {
            count++;
            lastEnd = end;
        }
    }
    return count;
}
```

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
```cpp
#include <vector>
using namespace std;

int solve(int n, vector<int>& dp) {
    if (n == 0 || n == 1) return n;
    if (dp[n] != -1) return dp[n];
    return dp[n] = solve(n - 1, dp) + solve(n - 2, dp);
}

int fib(int n) {
    vector<int> dp(n + 1, -1);
    return solve(n, dp);
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
```cpp
int fib(int n) {
    vector<int> dp(n + 1);
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
```cpp
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

### C++ Implementation: Subset Generation
**Given:** `nums = [1, 2]`

At each level: include or exclude a number. Backtracking explores both choices.

```cpp
#include <vector>
using namespace std;

void backtrack(vector<int>& nums, int index,
               vector<int>& current, vector<vector<int>>& result) {
    if (index == nums.size()) {
        result.push_back(current);
        return;
    }
    // Exclude
    backtrack(nums, index + 1, current, result);
    // Include
    current.push_back(nums[index]);
    backtrack(nums, index + 1, current, result);
    current.pop_back();  // backtrack
}

vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> current;
    backtrack(nums, 0, current, result);
    return result;
}
```

### Example: Permutations
```cpp
void permuteHelper(vector<int>& nums, int start,
                   vector<vector<int>>& result) {
    if (start == nums.size()) {
        result.push_back(nums);
        return;
    }
    for (int i = start; i < nums.size(); i++) {
        swap(nums[start], nums[i]);
        permuteHelper(nums, start + 1, result);
        swap(nums[start], nums[i]);  // backtrack
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> result;
    permuteHelper(nums, 0, result);
    return result;
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
```cpp
int a = 5; // 0101
int b = 3; // 0011
cout << (a & b); // 1
```

**2. OR (`|`) — Set a bit**
```cpp
cout << (a | b); // 7
```

**3. XOR (`^`) — Toggle bits / swap without temp**
```cpp
cout << (a ^ b); // 6
```

**4. Left Shift (`<<`) — Multiply by 2^n**
```cpp
cout << (a << 1); // 10
```

**5. Right Shift (`>>`) — Divide by 2^n**
```cpp
cout << (a >> 1); // 2
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
```cpp
bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
```

**2. Count set bits:**
```cpp
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
```cpp
int singleNumber(vector<int>& nums) {
    int res = 0;
    for (int num : nums) {
        res ^= num;
    }
    return res;
}
```

**4. Get ith bit:**
```cpp
int getIthBit(int n, int i) {
    return (n & (1 << i)) != 0 ? 1 : 0;
}
```

**5. Set ith bit:**
```cpp
int setIthBit(int n, int i) {
    return n | (1 << i);
}
```

### C++ Built-in Functions
```cpp
#include <bit>  // C++20
// or
#include <bitset>

__builtin_popcount(n);        // count set bits (GCC/Clang)
__builtin_ctz(n);             // count trailing zeros
__builtin_clz(n);             // count leading zeros
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
| **Hash Map** | Fast lookup, frequency, grouping | O(1) avg | O(n) |
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
| `std::sort` | O(n log n) | O(n log n) | O(n log n) | O(log n) | No |

### Data Structure Operations

| Structure | Access | Search | Insert | Delete |
|-----------|--------|--------|--------|--------|
| Array | O(1) | O(n) | O(n) | O(n) |
| Linked List | O(n) | O(n) | O(1) | O(1) |
| Stack | O(n) | O(n) | O(1) | O(1) |
| Queue | O(n) | O(n) | O(1) | O(1) |
| Hash Map | O(1) | O(1) | O(1) | O(1) |
| Heap | O(1) min/max | O(n) | O(log n) | O(log n) |
| BST (balanced) | O(log n) | O(log n) | O(log n) | O(log n) |
| Trie | - | O(L) | O(L) | O(L) |

### C++ STL Quick Reference

| Need | Use |
|------|-----|
| Dynamic array | `vector<T>` |
| Key-value store | `unordered_map<K,V>` / `map<K,V>` |
| Set | `unordered_set<T>` / `set<T>` |
| Stack | `stack<T>` |
| Queue | `queue<T>` |
| Double-ended queue | `deque<T>` |
| Priority queue | `priority_queue<T>` |
| Min-heap | `priority_queue<T, vector<T>, greater<T>>` |
| Sort | `sort(v.begin(), v.end())` |
| Binary search | `binary_search`, `lower_bound`, `upper_bound` |
| Min/Max | `min(a,b)`, `max(a,b)`, `*min_element`, `*max_element` |
| Swap | `swap(a, b)` |
| Reverse | `reverse(v.begin(), v.end())` |

### Key Decision Flow

```
Problem involves...
├── Pairs / sorted array → Two Pointers
├── Cycle / midpoint → Fast & Slow Pointers
├── Contiguous subarray/substring → Sliding Window
├── Range sum queries → Prefix Sum
├── Overlapping intervals → Merge Intervals
├── Sorted / search space → Binary Search
├── Fast lookup / frequency → Hash Map
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
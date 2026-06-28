# DSA Java ↔ C++ Reference

> Compact interview reference, with inline comments explaining what each call does.
> Complexity notes use n = number of elements unless stated otherwise.

## Table of Contents

1.  Arrays
2.  ArrayList ↔ vector
3.  LinkedList
4.  Stack
5.  Queue
6.  Deque
7.  Priority Queue
8.  HashMap ↔ unordered_map
9.  TreeMap ↔ map
10. HashSet ↔ unordered_set
11. TreeSet ↔ set
12. Pair / Tuple
13. String
14. StringBuilder
15. 2D Arrays / Matrix
16. Graph
17. TreeNode
18. ListNode
19. Sorting (incl. custom comparators)
20. Binary Search Helpers
21. Bit Manipulation
22. Math / Number Helpers
23. Common Algorithms & Patterns
24. Time Complexity Cheat Sheet
25. Primitive Types & Gotchas

------------------------------------------------------------------------

# 1. Arrays

## Java

``` java
int[] arr = new int[5];        // fixed-size array, all elements default to 0
arr[0] = 10;                   // set index 0 -> O(1)
int x = arr[0];                // read index 0 -> O(1)
arr.length;                    // property (no parens!) giving array size -> O(1)
Arrays.sort(arr);               // sort ascending in place -> O(n log n)
Arrays.fill(arr, 0);             // set every element to 0 -> O(n)
int[][] arr2 = new int[3][4];   // 2D array: 3 rows, 4 cols, all 0
int[] copy = Arrays.copyOf(arr, arr.length); // clone into new array -> O(n)
int[] sub = Arrays.copyOfRange(arr, 1, 3);   // copy indices [1,3) -> O(k)
System.out.println(Arrays.toString(arr));    // human-readable print, e.g. [10,0,0,0,0]
boolean eq = Arrays.equals(arr, copy);        // element-wise equality -> O(n)
```

## C++

``` cpp
int arr[5];                    // fixed-size array, NOT zero-initialized by default
arr[0] = 10;                   // set index 0 -> O(1)
int x = arr[0];                // read index 0 -> O(1)
int n = sizeof(arr)/sizeof(arr[0]); // size trick for raw arrays (only works on real arrays, not pointers)
sort(arr, arr + 5);             // sort ascending -> O(n log n)
fill(arr, arr + 5, 0);          // set every element to 0 -> O(n)
int arr2[3][4] = {};            // 2D array: 3 rows, 4 cols, all 0
int copy[5];
copy(arr, arr+5, copy);         // std::copy into another array -> O(n)
for (int x : arr) cout << x << " "; // print elements
```

> **Tip:** prefer `vector<int>` over raw C++ arrays in interviews — raw arrays can't be resized and don't carry their own length.

------------------------------------------------------------------------

# 2. ArrayList ↔ vector

## Java

``` java
ArrayList<Integer> list = new ArrayList<>();   // dynamic array, auto-resizes
list.add(10);                  // append to end -> O(1) amortized
list.add(1, 20);               // insert 20 at index 1, shifts rest right -> O(n)
list.get(0);                   // read by index -> O(1)
list.set(0, 30);                // overwrite index 0 -> O(1)
list.remove(0);                 // remove BY INDEX, shifts left -> O(n)
list.remove(Integer.valueOf(30)); // remove BY VALUE (first match) -> O(n)
list.contains(20);              // linear search -> O(n)
list.indexOf(20);                // index of first match, -1 if absent -> O(n)
list.size();                     // current element count -> O(1)
list.isEmpty();                  // true if size == 0 -> O(1)
list.clear();                    // remove all elements -> O(n)
Collections.sort(list);           // sort ascending in place -> O(n log n)
Collections.reverse(list);        // reverse order in place -> O(n)
List<Integer> sub = list.subList(1, 3); // VIEW (not copy) of indices [1,3)
Integer[] arrOut = list.toArray(new Integer[0]); // convert to array
```

## C++

``` cpp
vector<int> v;                  // dynamic array, auto-resizes
v.push_back(10);                // append to end -> O(1) amortized
v.emplace_back(20);             // append, constructs in place (avoids a copy) -> O(1) amortized
v[0];                            // read by index, no bounds check -> O(1)
v.at(0);                          // read by index, throws if out of range -> O(1)
v.insert(v.begin() + 1, 20);       // insert 20 at index 1 -> O(n)
v.erase(v.begin());                // remove element at iterator (front here) -> O(n)
v.pop_back();                       // remove last element -> O(1)
v.size();                            // current element count -> O(1)
v.empty();                            // true if size == 0 -> O(1)
v.clear();                             // remove all elements -> O(n)
sort(v.begin(), v.end());               // sort ascending in place -> O(n log n)
reverse(v.begin(), v.end());             // reverse order in place -> O(n)
vector<int> sub(v.begin()+1, v.begin()+3); // COPY of indices [1,3)
v.reserve(100);                            // pre-allocate capacity to avoid reallocations
v.resize(10);                                // grow/shrink to size 10 (new slots = 0)
```

------------------------------------------------------------------------

# 3. LinkedList

## Java

``` java
LinkedList<Integer> list = new LinkedList<>(); // doubly linked list, implements List + Deque
list.addFirst(1);               // insert at head -> O(1)
list.addLast(2);                // insert at tail -> O(1)
list.getFirst();                // peek head value -> O(1)
list.getLast();                 // peek tail value -> O(1)
list.removeFirst();             // remove & return head -> O(1)
list.removeLast();              // remove & return tail -> O(1)
list.get(2);                    // index access — O(n), walks the list!
```

## C++

``` cpp
list<int> lst;                  // doubly linked list
lst.push_front(1);              // insert at head -> O(1)
lst.push_back(2);               // insert at tail -> O(1)
lst.front();                    // peek head value -> O(1)
lst.back();                     // peek tail value -> O(1)
lst.pop_front();                 // remove head -> O(1)
lst.pop_back();                  // remove tail -> O(1)
// no random-access operator[] on list<int> — must use an iterator and advance() -> O(n)
```

------------------------------------------------------------------------

# 4. Stack

## Java

``` java
Deque<Integer> st = new ArrayDeque<>(); // PREFERRED stack in modern Java (Stack class is legacy/synchronized)
st.push(1);                     // push to top -> O(1)
st.peek();                      // view top without removing -> O(1)
st.pop();                       // remove & return top -> O(1)
st.isEmpty();                    // true if empty -> O(1)
st.size();                        // number of elements -> O(1)
```

## C++

``` cpp
stack<int> st;                  // LIFO container (adapter over deque by default)
st.push(1);                     // push to top -> O(1)
st.top();                        // view top without removing -> O(1)
st.pop();                         // remove top (does NOT return value!) -> O(1)
st.empty();                        // true if empty -> O(1)
st.size();                          // number of elements -> O(1)
```

> **Note:** `Stack<Integer>` in Java still works and behaves the same logically, but `ArrayDeque` is faster since `Stack` is internally synchronized (thread-safe overhead you usually don't need).

------------------------------------------------------------------------

# 5. Queue

## Java

``` java
Queue<Integer> q = new LinkedList<>(); // FIFO; ArrayDeque also works and is generally faster
q.offer(1);                     // enqueue (add to back) -> O(1)
q.peek();                        // view front without removing, null if empty -> O(1)
q.poll();                          // dequeue (remove & return front), null if empty -> O(1)
q.size();                            // number of elements -> O(1)
q.isEmpty();                          // true if empty -> O(1)
```

## C++

``` cpp
queue<int> q;                   // FIFO container (adapter over deque by default)
q.push(1);                      // enqueue (add to back) -> O(1)
q.front();                        // view front without removing -> O(1)
q.pop();                           // dequeue (does NOT return value!) -> O(1)
q.size();                            // number of elements -> O(1)
q.empty();                            // true if empty -> O(1)
```

------------------------------------------------------------------------

# 6. Deque

## Java

``` java
Deque<Integer> dq = new ArrayDeque<>(); // double-ended queue, resizable array backed
dq.offerFirst(1);                // push to front -> O(1)
dq.offerLast(2);                  // push to back -> O(1)
dq.peekFirst();                    // view front, null if empty -> O(1)
dq.peekLast();                      // view back, null if empty -> O(1)
dq.pollFirst();                      // remove & return front, null if empty -> O(1)
dq.pollLast();                        // remove & return back, null if empty -> O(1)
```

## C++

``` cpp
deque<int> dq;                  // double-ended queue, segmented array backed
dq.push_front(1);                // push to front -> O(1)
dq.push_back(2);                  // push to back -> O(1)
dq.front();                         // view front -> O(1)
dq.back();                           // view back -> O(1)
dq.pop_front();                       // remove front -> O(1)
dq.pop_back();                         // remove back -> O(1)
dq[2];                                  // random access also supported -> O(1)
```

------------------------------------------------------------------------

# 7. Priority Queue

## Java

``` java
PriorityQueue<Integer> minHeap = new PriorityQueue<>(); // min-heap by default
minHeap.offer(1);                // insert -> O(log n)
minHeap.peek();                    // view smallest, null if empty -> O(1)
minHeap.poll();                      // remove & return smallest, null if empty -> O(log n)

PriorityQueue<Integer> maxHeap =
    new PriorityQueue<>(Collections.reverseOrder()); // max-heap via reverse comparator

// Custom comparator, e.g. min-heap of int[] pairs sorted by 2nd element:
PriorityQueue<int[]> pq = new PriorityQueue<>((a, b) -> a[1] - b[1]);
```

## C++

``` cpp
priority_queue<int> maxHeap;     // max-heap by default
maxHeap.push(1);                  // insert -> O(log n)
maxHeap.top();                      // view largest -> O(1)
maxHeap.pop();                        // remove largest (does NOT return value!) -> O(log n)

priority_queue<int, vector<int>, greater<int>> minHeap; // min-heap via greater<> comparator

// Custom comparator (lambda) needs an explicit decltype:
auto cmp = [](pair<int,int>& a, pair<int,int>& b) { return a.second > b.second; }; // min by 2nd elem
priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
```

------------------------------------------------------------------------

# 8. HashMap ↔ unordered_map

## Java

``` java
HashMap<String,Integer> map = new HashMap<>(); // avg O(1) get/put, no order guarantee
map.put("a", 1);                  // insert or overwrite key -> O(1) avg
map.get("a");                       // value for key, null if absent -> O(1) avg
map.getOrDefault("b", 0);             // value for key, fallback if absent (no null risk) -> O(1) avg
map.containsKey("a");                   // true if key exists -> O(1) avg
map.containsValue(1);                     // true if value exists -> O(n), scans all values
map.remove("a");                            // delete key, returns old value or null -> O(1) avg
map.putIfAbsent("c", 5);                      // insert only if key missing -> O(1) avg
map.merge("a", 1, Integer::sum);                // add 1 to existing value, or insert 1 if absent -> O(1) avg
map.size();                                       // number of entries -> O(1)
map.isEmpty();                                      // true if empty -> O(1)
for (var e : map.entrySet()) { e.getKey(); e.getValue(); } // iterate key-value pairs
for (String k : map.keySet()) { }                     // iterate keys only
for (Integer v : map.values()) { }                       // iterate values only
```

## C++

``` cpp
unordered_map<string,int> mp;       // avg O(1) get/put, no order guarantee
mp["a"] = 1;                          // insert or overwrite key -> O(1) avg (auto-creates if missing!)
mp.insert({"b", 2});                    // insert only if key missing (no-op if present) -> O(1) avg
mp["a"];                                  // read value, auto-creates "a"->0 if missing — careful!
mp.count("a");                              // 1 if key exists, 0 otherwise -> O(1) avg
mp.find("a");                                 // returns iterator, mp.end() if not found -> O(1) avg
mp.erase("a");                                  // delete key -> O(1) avg
mp.size();                                        // number of entries -> O(1)
mp.empty();                                         // true if empty -> O(1)
for (auto &p : mp) { p.first; p.second; }             // iterate key-value pairs
```

> **Gotcha:** in C++, `mp["key"]` on a missing key silently inserts it with a default value (0 for int). Use `mp.find()` or `mp.count()` first if you don't want that side effect. Java's `map.get()` just returns `null` — no insertion.

------------------------------------------------------------------------

# 9. TreeMap ↔ map

## Java

``` java
TreeMap<Integer,String> map = new TreeMap<>(); // sorted by key (Red-Black tree), O(log n) ops
map.put(1, "A");                  // insert/update -> O(log n)
map.firstKey();                     // smallest key -> O(log n)
map.lastKey();                        // largest key -> O(log n)
map.floorKey(5);                        // largest key <= 5, null if none -> O(log n)
map.ceilingKey(5);                        // smallest key >= 5, null if none -> O(log n)
map.lowerKey(5);                            // largest key < 5, null if none -> O(log n)
map.higherKey(5);                             // smallest key > 5, null if none -> O(log n)
map.pollFirstEntry();                           // remove & return smallest entry -> O(log n)
map.pollLastEntry();                              // remove & return largest entry -> O(log n)
```

## C++

``` cpp
map<int,string> mp;               // sorted by key (Red-Black tree), O(log n) ops
mp[1] = "A";                       // insert/update -> O(log n)
mp.lower_bound(5);                   // iterator to first key >= 5 -> O(log n)
mp.upper_bound(5);                     // iterator to first key > 5 -> O(log n)
mp.begin();                              // iterator to smallest key -> O(1)
mp.rbegin();                                // iterator to largest key (reverse begin) -> O(1)
mp.erase(5);                                  // delete key -> O(log n)
mp.find(5);                                     // iterator to key, mp.end() if absent -> O(log n)
```

> No direct `floorKey`/`ceilingKey` in C++; combine `lower_bound`/`upper_bound` and step the iterator with `--it` / `++it` to emulate floor/ceiling/lower/higher.

------------------------------------------------------------------------

# 10. HashSet ↔ unordered_set

## Java

``` java
HashSet<Integer> set = new HashSet<>(); // avg O(1) ops, no order, no duplicates
set.add(1);                       // insert (no-op if already present) -> O(1) avg
set.contains(1);                    // true if present -> O(1) avg
set.remove(1);                         // delete element -> O(1) avg
set.size();                              // number of elements -> O(1)
set.isEmpty();                              // true if empty -> O(1)
for (int x : set) { }                          // iterate (unordered)
```

## C++

``` cpp
unordered_set<int> st;           // avg O(1) ops, no order, no duplicates
st.insert(1);                      // insert (no-op if already present) -> O(1) avg
st.count(1);                          // 1 if present, 0 otherwise -> O(1) avg
st.erase(1);                             // delete element -> O(1) avg
st.size();                                  // number of elements -> O(1)
st.empty();                                    // true if empty -> O(1)
for (int x : st) { }                              // iterate (unordered)
```

------------------------------------------------------------------------

# 11. TreeSet ↔ set

## Java

``` java
TreeSet<Integer> set = new TreeSet<>(); // sorted, no duplicates, O(log n) ops
set.add(1);                       // insert -> O(log n)
set.first();                         // smallest element -> O(log n)
set.last();                            // largest element -> O(log n)
set.floor(5);                            // largest element <= 5, null if none -> O(log n)
set.ceiling(5);                            // smallest element >= 5, null if none -> O(log n)
set.lower(5);                                // largest element < 5, null if none -> O(log n)
set.higher(5);                                 // smallest element > 5, null if none -> O(log n)
set.pollFirst();                                  // remove & return smallest -> O(log n)
set.pollLast();                                     // remove & return largest -> O(log n)
```

## C++

``` cpp
set<int> st;                     // sorted, no duplicates, O(log n) ops
st.insert(1);                      // insert -> O(log n)
st.lower_bound(5);                    // iterator to first element >= 5 -> O(log n)
st.upper_bound(5);                       // iterator to first element > 5 -> O(log n)
*st.begin();                                // smallest element -> O(1)
*st.rbegin();                                  // largest element -> O(1)
st.erase(5);                                      // delete element -> O(log n)
```

------------------------------------------------------------------------

# 12. Pair / Tuple

## Java

``` java
Map.Entry<Integer,Integer> p =
    new AbstractMap.SimpleEntry<>(1, 2);  // simple two-value pair (no native Pair class pre-Java 16)
p.getKey();                       // first value -> O(1)
p.getValue();                       // second value -> O(1)

// Common interview workaround: encode pair as int[] or use a small record/class
record Pair(int first, int second) {} // Java 16+, immutable, auto getters .first()/.second()
```

## C++

``` cpp
pair<int,int> p = {1, 2};         // built-in pair, very common in C++ DSA code
p.first;                           // first value -> O(1)
p.second;                            // second value -> O(1)

tuple<int,int,int> t = {1, 2, 3};      // for 3+ values
get<0>(t); get<1>(t); get<2>(t);          // access tuple elements by index -> O(1)
auto [a, b, c] = t;                          // structured binding (C++17) unpacks tuple
```

------------------------------------------------------------------------

# 13. String

## Java

``` java
String s = "hello";               // immutable — every "modification" creates a new String
s.length();                          // number of characters -> O(1)
s.charAt(0);                           // character at index -> O(1)
s.substring(1, 3);                       // substring [1,3), new String -> O(k)
s.indexOf("e");                             // index of first match, -1 if absent -> O(n*m)
s.contains("el");                              // true if substring exists -> O(n*m)
s.replace("a", "b");                              // replace ALL occurrences, returns new String -> O(n)
s.split(" ");                                        // split into String[] by delimiter regex
s.toCharArray();                                        // convert to char[] -> O(n)
s.equals("hello");                                         // content equality (use this, NOT ==!)
s.equalsIgnoreCase("HELLO");                                  // case-insensitive equality
s.trim();                                                       // remove leading/trailing whitespace
s.toUpperCase(); s.toLowerCase();                                  // case conversion
s.compareTo("world");                                                // lexicographic comparison
String.valueOf(123);                                                    // convert int -> String
Integer.parseInt("123");                                                   // convert String -> int
String joined = String.join(",", "a", "b");                                  // "a,b"
```

## C++

``` cpp
string s = "hello";               // mutable, dynamic-size character buffer
s.length();                          // (same as s.size()) -> O(1)
s[0];                                   // character at index, mutable -> O(1)
s.substr(1, 3);                            // substring starting at 1, length 3, new string -> O(k)
s.find("el");                                 // index of first match, string::npos if absent -> O(n*m)
s.replace(1, 2, "ab");                            // replace 2 chars starting at index 1 -> O(n)
reverse(s.begin(), s.end());                          // reverse in place -> O(n)
s == "hello";                                            // content equality works directly with ==
to_string(123);                                             // convert int -> string
stoi("123");                                                  // convert string -> int (also stol, stod, etc.)
transform(s.begin(), s.end(), s.begin(), ::toupper);              // uppercase conversion
stringstream ss(s);                                                 // split helper:
string token;
while (getline(ss, token, ' ')) { /* token = each space-separated word */ }
```

> **Gotcha:** Java strings compare with `.equals()`, never `==` (which compares object references). C++ strings can safely use `==` directly.

------------------------------------------------------------------------

# 14. StringBuilder

## Java

``` java
StringBuilder sb = new StringBuilder(); // mutable buffer for efficient string building
sb.append("a");                  // add to end -> O(1) amortized
sb.insert(1, "b");                  // insert at index -> O(n)
sb.delete(1, 2);                      // remove chars in range [1,2) -> O(n)
sb.deleteCharAt(0);                       // remove single char -> O(n)
sb.reverse();                                // reverse in place -> O(n)
sb.setCharAt(0, 'x');                           // overwrite char at index -> O(1)
sb.charAt(0);                                      // read char at index -> O(1)
sb.length();                                          // current length -> O(1)
sb.toString();                                           // convert to immutable String -> O(n)
```

## C++

``` cpp
string s;                        // std::string itself is mutable — no separate builder needed
s += "a";                           // append -> O(1) amortized
s.insert(1, "b");                      // insert at index -> O(n)
s.erase(1, 1);                            // remove 1 char starting at index 1 -> O(n)
reverse(s.begin(), s.end());                 // reverse in place -> O(n)
s[0] = 'x';                                     // overwrite char at index -> O(1)
s.length();                                        // current length -> O(1)
```

------------------------------------------------------------------------

# 15. 2D Arrays / Matrix

## Java

``` java
int[][] grid = new int[3][4];     // 3 rows x 4 cols, all 0
grid[0][0] = 1;                      // set cell -> O(1)
int rows = grid.length;                 // number of rows -> O(1)
int cols = grid[0].length;                 // number of cols -> O(1)
for (int[] row : grid) {                      // iterate rows
    for (int val : row) { }                      // iterate cols
}
int[][] copy = new int[3][];                       // deep copy each row (arrays of arrays share refs otherwise!)
for (int i = 0; i < 3; i++) copy[i] = grid[i].clone();
```

## C++

``` cpp
vector<vector<int>> grid(3, vector<int>(4, 0)); // 3 rows x 4 cols, all 0
grid[0][0] = 1;                       // set cell -> O(1)
int rows = grid.size();                  // number of rows -> O(1)
int cols = grid[0].size();                  // number of cols -> O(1)
for (auto &row : grid) {                       // iterate rows (by reference, mutable)
    for (int val : row) { }                       // iterate cols
}
vector<vector<int>> copy = grid;                   // deep copy by value (vectors copy automatically)
```

------------------------------------------------------------------------

# 16. Graph

## Java

``` java
// Adjacency list (unweighted)
List<List<Integer>> g = new ArrayList<>();
for (int i = 0; i < n; i++) g.add(new ArrayList<>());
g.get(u).add(v);                  // add directed edge u -> v -> O(1)
g.get(u).add(v); g.get(v).add(u);    // undirected: add both directions

// Adjacency list (weighted), each entry is [neighbor, weight]
List<List<int[]>> wg = new ArrayList<>();
wg.get(u).add(new int[]{v, weight});

// Adjacency matrix
int[][] adj = new int[n][n];          // adj[u][v] = weight (or 1/0 for unweighted)
```

## C++

``` cpp
// Adjacency list (unweighted)
vector<vector<int>> g(n);
g[u].push_back(v);                // add directed edge u -> v -> O(1)
g[u].push_back(v); g[v].push_back(u); // undirected: add both directions

// Adjacency list (weighted), each entry is {neighbor, weight}
vector<vector<pair<int,int>>> wg(n);
wg[u].push_back({v, weight});

// Adjacency matrix
vector<vector<int>> adj(n, vector<int>(n, 0)); // adj[u][v] = weight (or 1/0 for unweighted)
```

------------------------------------------------------------------------

# 17. TreeNode

## Java

``` java
class TreeNode {
    int val;
    TreeNode left, right;
    TreeNode(int x) { val = x; }   // left/right default to null automatically
}
// usage:
TreeNode root = new TreeNode(5);
root.left = new TreeNode(3);
```

## C++

``` cpp
struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) { val = x; left = right = NULL; } // must set pointers to NULL manually
};
// usage:
TreeNode* root = new TreeNode(5);
root->left = new TreeNode(3);     // use -> for pointer member access
```

------------------------------------------------------------------------

# 18. ListNode

## Java

``` java
class ListNode {
    int val;
    ListNode next;
    ListNode(int x) { val = x; }   // next defaults to null automatically
}
// usage:
ListNode head = new ListNode(1);
head.next = new ListNode(2);
```

## C++

``` cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) { val = x; next = NULL; } // must set pointer to NULL manually
};
// usage:
ListNode* head = new ListNode(1);
head->next = new ListNode(2);     // use -> for pointer member access
```

------------------------------------------------------------------------

# 19. Sorting (incl. custom comparators)

## Java

``` java
Arrays.sort(arr);                  // primitive array, ascending -> O(n log n), in place
Collections.sort(list);               // List<Integer>/etc, ascending -> O(n log n)
Collections.reverse(list);               // reverse order -> O(n)

// Custom comparator on objects (primitives can't use Comparator directly):
Arrays.sort(people, (a, b) -> a.age - b.age);          // ascending by age
Arrays.sort(people, Comparator.comparingInt(p -> p.age)); // same, more idiomatic
Collections.sort(list, Collections.reverseOrder());        // descending

// Sorting 2D array by column 0:
int[][] intervals = {{1,3},{2,6}};
Arrays.sort(intervals, (a, b) -> a[0] - b[0]);
```

## C++

``` cpp
sort(arr, arr + n);                  // raw array, ascending -> O(n log n)
sort(v.begin(), v.end());               // vector, ascending -> O(n log n)
reverse(v.begin(), v.end());               // reverse order -> O(n)

// Custom comparator with a lambda:
sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // descending
sort(people.begin(), people.end(),
     [](Person &a, Person &b) { return a.age < b.age; });        // ascending by age

// Sorting vector<vector<int>> (e.g. intervals) by column 0:
sort(intervals.begin(), intervals.end(),
     [](vector<int>&a, vector<int>&b){ return a[0] < b[0]; });
```

> Both languages: sorting is `O(n log n)` average and worst case (Java's `Arrays.sort` on objects/`Collections.sort` uses a stable mergesort variant; C++'s `sort` is typically introsort, not guaranteed stable — use `stable_sort` if stability matters).

------------------------------------------------------------------------

# 20. Binary Search Helpers

## Java

``` java
Arrays.binarySearch(arr, x);         // index of x in sorted array, or negative if absent -> O(log n)
Collections.binarySearch(list, x);      // same, for a sorted List -> O(log n)

// Manual binary search template (find first index where condition becomes true):
int lo = 0, hi = arr.length - 1;
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (arr[mid] >= target) hi = mid; else lo = mid + 1;
}
```

## C++

``` cpp
binary_search(v.begin(), v.end(), x);   // true/false if x exists in sorted range -> O(log n)
lower_bound(v.begin(), v.end(), x);        // iterator to first element >= x -> O(log n)
upper_bound(v.begin(), v.end(), x);          // iterator to first element > x -> O(log n)
int idx = lower_bound(v.begin(), v.end(), x) - v.begin(); // convert iterator to index

// Manual binary search template (same logic as Java):
int lo = 0, hi = v.size() - 1;
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (v[mid] >= target) hi = mid; else lo = mid + 1;
}
```

------------------------------------------------------------------------

# 21. Bit Manipulation

## Java

``` java
int a = 5, b = 3;
a & b;                       // AND
a | b;                       // OR
a ^ b;                       // XOR
~a;                          // NOT (bitwise complement)
a << 1;                      // left shift (multiply by 2)
a >> 1;                       // arithmetic right shift (sign-preserving, divide by 2)
a >>> 1;                       // unsigned right shift (fills with 0, ignores sign)
Integer.bitCount(a);             // number of set bits (popcount) -> O(1)
Integer.toBinaryString(a);          // binary string representation
a & (a - 1);                          // clears the lowest set bit — common trick
a & 1;                                   // check if odd (1) or even (0)
1 << k;                                     // value with only bit k set
```

## C++

``` cpp
int a = 5, b = 3;
a & b;                       // AND
a | b;                       // OR
a ^ b;                       // XOR
~a;                          // NOT (bitwise complement)
a << 1;                      // left shift (multiply by 2)
a >> 1;                       // right shift (divide by 2; arithmetic for signed ints)
__builtin_popcount(a);           // number of set bits (popcount) -> O(1)
bitset<8>(a).to_string();           // binary string representation
a & (a - 1);                          // clears the lowest set bit — common trick
a & 1;                                   // check if odd (1) or even (0)
1 << k;                                     // value with only bit k set
```

------------------------------------------------------------------------

# 22. Math / Number Helpers

## Java

``` java
Math.max(a, b);                  // larger of two values -> O(1)
Math.min(a, b);                     // smaller of two values -> O(1)
Math.abs(x);                           // absolute value -> O(1)
Math.pow(a, b);                           // a^b as a double -> O(log b)
Math.sqrt(x);                                // square root (double) -> O(1)
Math.ceil(x); Math.floor(x);                    // round up / down (returns double)
Math.floorMod(a, b);                               // always non-negative modulo (unlike %)
long gcdVal = gcd(a, b);                              // no built-in; implement Euclid's algorithm:
// static long gcd(long a, long b){ return b==0 ? a : gcd(b, a%b); }
Integer.MAX_VALUE; Integer.MIN_VALUE;                    // int bounds: 2^31-1, -2^31
Long.MAX_VALUE;                                             // long bound: 2^63-1
```

## C++

``` cpp
min(a, b);                     // smaller of two values -> O(1)
max(a, b);                        // larger of two values -> O(1)
abs(x);                              // absolute value -> O(1)
pow(a, b);                              // a^b as a double -> O(log b)
sqrt(x);                                   // square root (double) -> O(1)
ceil(x); floor(x);                            // round up / down (returns double)
__gcd(a, b);                                     // built-in GCD (Euclid's algorithm) -> O(log min(a,b))
INT_MAX; INT_MIN;                                   // int bounds: 2^31-1, -2^31 (need <climits>)
LLONG_MAX;                                             // long long bound: 2^63-1
```

------------------------------------------------------------------------

# 23. Common Algorithms & Patterns

## Java

``` java
Arrays.fill(arr, 0);                   // fill array with value -> O(n)
Collections.frequency(list, x);           // count occurrences of x -> O(n)
Collections.swap(list, i, j);                // swap two elements by index -> O(1)
Collections.max(list); Collections.min(list);    // find extremes -> O(n)
Collections.unmodifiableList(list);                 // read-only view of a list

// two-pointer template
int l = 0, r = arr.length - 1;
while (l < r) { /* ... */ l++; r--; }

// sliding window template
int left = 0, sum = 0;
for (int right = 0; right < arr.length; right++) {
    sum += arr[right];
    while (/* window invalid */ false) { sum -= arr[left++]; }
}

// prefix sum
int[] prefix = new int[arr.length + 1];
for (int i = 0; i < arr.length; i++) prefix[i+1] = prefix[i] + arr[i];
// sum of arr[i..j] inclusive = prefix[j+1] - prefix[i]
```

## C++

``` cpp
fill(arr, arr + n, 0);                  // fill array with value -> O(n)
count(v.begin(), v.end(), x);              // count occurrences of x -> O(n)
swap(a, b);                                   // swap two values (also works on containers) -> O(1)
*max_element(v.begin(), v.end());                // find max value -> O(n)
*min_element(v.begin(), v.end());                   // find min value -> O(n)
accumulate(v.begin(), v.end(), 0);                     // sum all elements (3rd arg = initial value) -> O(n)
find(v.begin(), v.end(), x);                              // iterator to first match, v.end() if absent -> O(n)
next_permutation(v.begin(), v.end());                        // rearrange to next lexicographic permutation -> O(n)
prev_permutation(v.begin(), v.end());                          // rearrange to previous permutation -> O(n)
unique(v.begin(), v.end());                                      // removes consecutive dupes, returns new logical end -> O(n)
                                                                   // (call v.erase(unique(...), v.end()) to actually shrink)
distance(a, b);                                                      // number of elements between two iterators -> O(n) or O(1)

// two-pointer template
int l = 0, r = v.size() - 1;
while (l < r) { /* ... */ l++; r--; }

// sliding window template
int left = 0, sum = 0;
for (int right = 0; right < v.size(); right++) {
    sum += v[right];
    while (/* window invalid */ false) { sum -= v[left++]; }
}

// prefix sum
vector<int> prefix(v.size() + 1, 0);
for (int i = 0; i < v.size(); i++) prefix[i+1] = prefix[i] + v[i];
// sum of v[i..j] inclusive = prefix[j+1] - prefix[i]
```

------------------------------------------------------------------------

# 24. Time Complexity Cheat Sheet

| Operation                      | ArrayList / vector | LinkedList / list | HashMap / unordered_map | TreeMap / map | Heap (PQ)   |
|---------------------------------|:---:|:---:|:---:|:---:|:---:|
| Access by index                  | O(1) | O(n) | — | — | — |
| Insert/remove at end                | O(1)* | O(1) | — | — | — |
| Insert/remove at front/middle          | O(n) | O(1) | — | — | — |
| Search by value                          | O(n) | O(n) | O(1) avg | O(log n) | — |
| Insert/lookup by key                        | — | — | O(1) avg | O(log n) | — |
| Get min/max                                    | — | — | — | O(log n) | O(1) peek / O(log n) pop |

\* amortized, due to occasional internal resizing.

------------------------------------------------------------------------

# 25. Primitive Types & Gotchas

## Java

``` java
int, long, double, char, boolean, byte, short, float; // primitives — NOT objects, can't be null
Integer, Long, Double, Character, Boolean;               // boxed wrapper classes — CAN be null, used in generics
                                                            // (List<int> is illegal; must use List<Integer>)
int OVERFLOW = Integer.MAX_VALUE + 1;                        // silently wraps to Integer.MIN_VALUE — no error!
long safe = (long) a * b;                                      // cast before multiplying to avoid int overflow
'a' == 'a';                                                        // chars compare fine with ==
Integer x = 200, y = 200;
x == y;                                                                // FALSE for boxed values outside [-128,127]
                                                                         // cache range — always use .equals() for Integer!
```

## C++

``` cpp
int, long, long long, double, char, bool, short, float;  // all are value types
int OVERFLOW = INT_MAX + 1;                  // undefined behavior — commonly wraps, but not guaranteed!
long long safe = (long long) a * b;             // cast before multiplying to avoid int overflow
auto x = 5;                                        // type inferred at compile time (here: int)
const int& ref = x;                                   // reference: alias to existing variable, no copy
int* ptr = &x;                                           // pointer: holds the memory address of x
*ptr;                                                        // dereference: get value pointed to
```

> **Biggest cross-language trap:** integer overflow is silent in both languages for fixed-width types — always widen to `long`/`long long` before multiplying values that might exceed ~2 billion.
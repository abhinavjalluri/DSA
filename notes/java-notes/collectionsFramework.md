# Part 4 – Collections Framework

```
Iterable
└── Collection
    ├── List (ordered, duplicates allowed)
    │   ├── ArrayList
    │   ├── LinkedList
    │   └── Vector -> Stack
    ├── Set (no duplicates)
    │   ├── HashSet
    │   │   └── LinkedHashSet
    │   └── SortedSet -> NavigableSet -> TreeSet
    └── Queue
        ├── PriorityQueue
        └── Deque -> ArrayDeque, LinkedList

Map (separate hierarchy, not a Collection)
├── HashMap
│   └── LinkedHashMap
├── SortedMap -> NavigableMap -> TreeMap
└── ConcurrentHashMap
```

---

## 1. List

Ordered collection, allows duplicates, index-based access.

```java
List<String> list = new ArrayList<>();
list.add("A"); list.add("B"); list.add("A");
list.get(0);
list.remove("A");        // removes first occurrence
```

---

## 2. ArrayList

- Backed by a **resizable array** (default capacity 10, grows by 1.5x when full).
- **O(1)** get/set (index-based), **O(n)** insert/remove in the middle (shifts elements).
- Not synchronized (not thread-safe).

```java
List<Integer> list = new ArrayList<>();
list.add(10);
list.add(0, 5);          // O(n) - shifts elements
System.out.println(list.get(1));   // O(1)
```

**When to use**: frequent random access/reads, infrequent insert/delete in the middle.

---

## 3. LinkedList

- Doubly-linked list implementation — `O(1)` insert/delete at ends (and O(1) once you have the node), but `O(n)` random access (must traverse).
- Implements both `List` and `Deque` — can be used as a stack/queue.

```java
LinkedList<Integer> ll = new LinkedList<>();
ll.addFirst(1);
ll.addLast(2);
ll.removeFirst();
```

**When to use**: frequent insertions/deletions at the beginning/middle, rare random access. In practice, `ArrayDeque` usually outperforms `LinkedList` for queue/stack use cases due to cache locality.

---

## 4. Vector

- Legacy (JDK 1.0), synchronized version of `ArrayList` — thread-safe but slower due to lock overhead on every method.
- Grows by doubling (100%) instead of 1.5x.
- Mostly obsolete — prefer `ArrayList` + explicit synchronization, or `CopyOnWriteArrayList` for concurrent read-heavy scenarios.

---

## 5. Stack

- Legacy class extending `Vector` — LIFO (Last-In-First-Out).

```java
Stack<Integer> stack = new Stack<>();
stack.push(1); stack.push(2);
stack.pop();     // 2
stack.peek();    // 1
```

**Modern alternative**: `Deque<Integer> stack = new ArrayDeque<>();` (faster, not synchronized, recommended by Java docs over `Stack`).

---

## 6. Queue

FIFO (First-In-First-Out) interface.

```java
Queue<Integer> q = new LinkedList<>();
q.offer(1); q.offer(2);
q.poll();     // removes & returns head (1) or null if empty
q.peek();     // views head without removing
```

`add()`/`remove()`/`element()` throw exceptions on failure; `offer()`/`poll()`/`peek()` return special values (`false`/`null`) — prefer the latter for graceful handling.

---

## 7. PriorityQueue

- Min-heap by default (smallest element has highest priority) — `O(log n)` insert/remove, `O(1)` peek.

```java
PriorityQueue<Integer> pq = new PriorityQueue<>();               // min-heap
pq.addAll(List.of(5, 1, 3));
pq.poll();    // 1

PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Collections.reverseOrder());
PriorityQueue<Task> byPriority = new PriorityQueue<>(Comparator.comparingInt(t -> t.priority));
```

Not thread-safe; use `PriorityBlockingQueue` for concurrent scenarios. Common in Dijkstra's algorithm, task scheduling, "top-K" problems.

---

## 8. Deque

Double-ended queue — insert/remove from both ends. `ArrayDeque` is the go-to implementation (faster than `LinkedList` and `Stack`).

```java
Deque<Integer> deque = new ArrayDeque<>();
deque.addFirst(1); deque.addLast(2);
deque.pollFirst(); deque.pollLast();
// Used as Stack: push()/pop() operate on the head
// Used as Queue: offer()/poll() operate tail/head
```

---

## 9. Set

Collection with **no duplicate elements**, based on `equals()`/`hashCode()`.

```java
Set<String> set = new HashSet<>();
set.add("A"); set.add("A");   // second add ignored
System.out.println(set.size());  // 1
```

---

## 10. HashSet

- Backed internally by a `HashMap` (elements stored as keys, dummy value).
- **O(1)** average add/remove/contains, **no ordering guarantee**.

```java
Set<Integer> hs = new HashSet<>();
hs.add(3); hs.add(1); hs.add(2);
System.out.println(hs);   // order not guaranteed (often looks sorted for small ints due to hashing, don't rely on it)
```

Requires correct `equals()`/`hashCode()` implementations for custom objects to work correctly.

---

## 11. LinkedHashSet

- `HashSet` + a doubly-linked list running through it — maintains **insertion order**.

```java
Set<Integer> lhs = new LinkedHashSet<>();
lhs.add(3); lhs.add(1); lhs.add(2);
System.out.println(lhs);   // [3, 1, 2] - insertion order preserved
```

---

## 12. TreeSet

- Implements `NavigableSet`, backed by a **Red-Black Tree** — elements kept in **sorted order**, `O(log n)` operations.

```java
TreeSet<Integer> ts = new TreeSet<>();
ts.add(5); ts.add(1); ts.add(3);
System.out.println(ts);          // [1, 3, 5] sorted
ts.first(); ts.last();
ts.higher(3);    // 5 (smallest element > 3)
ts.lower(3);     // 1 (largest element < 3)
ts.headSet(3);   // elements < 3
```

Requires elements to be `Comparable`, or a `Comparator` passed to the constructor.

---

## 13. Map

Key-value pairs, keys are unique. **Not** a subtype of `Collection`.

```java
Map<String, Integer> map = new HashMap<>();
map.put("a", 1);
map.get("a");
map.getOrDefault("z", 0);
map.computeIfAbsent("b", k -> 2);
map.merge("a", 10, Integer::sum);   // a -> 11
for (Map.Entry<String, Integer> e : map.entrySet()) { ... }
```

---

## 14. HashMap

- Array of **buckets**, each holding a linked list (or, since Java 8, a **Red-Black tree** if a bucket has ≥8 entries — improves worst-case from O(n) to O(log n)).
- Key's `hashCode()` determines the bucket; `equals()` resolves collisions within a bucket.
- **O(1)** average get/put, allows one `null` key and multiple `null` values, **not thread-safe**, **no ordering guarantee**.
- Default capacity 16, load factor 0.75 (resizes/rehashes when 75% full).

```java
Map<String, Integer> map = new HashMap<>();
map.put("apple", 1);
map.put("banana", 2);
map.put(null, 0);     // allowed - one null key

// custom key requires proper equals()/hashCode()
class Point {
    int x, y;
    public boolean equals(Object o) { ... }
    public int hashCode() { return Objects.hash(x, y); }
}
```

**Interview essential** — HashMap internal working (Java 8+):
1. `hash(key)` computed → `hash ^ (hash >>> 16)` (spreads bits to reduce collisions).
2. Bucket index = `hash & (capacity - 1)`.
3. If bucket empty → insert node. If occupied → traverse linked list/tree, compare via `equals()`.
4. When `size > capacity * loadFactor` → **resize** (double capacity) and rehash all entries.

---

## 15. LinkedHashMap

- `HashMap` + insertion-order (or access-order) iteration via internal doubly-linked list.

```java
Map<String, Integer> lhm = new LinkedHashMap<>(16, 0.75f, true);  // access-order = true -> LRU-cache friendly

// Common pattern: simple LRU cache
class LRUCache<K,V> extends LinkedHashMap<K,V> {
    private final int capacity;
    LRUCache(int capacity) { super(16, 0.75f, true); this.capacity = capacity; }
    protected boolean removeEldestEntry(Map.Entry<K,V> eldest) { return size() > capacity; }
}
```

---

## 16. TreeMap

- `NavigableMap`, Red-Black tree backed, keys kept in **sorted order**, `O(log n)` operations.

```java
TreeMap<String, Integer> tm = new TreeMap<>();
tm.put("banana", 2); tm.put("apple", 1);
System.out.println(tm);       // {apple=1, banana=2} sorted by key
tm.firstKey(); tm.lastKey();
tm.ceilingKey("b");    // smallest key >= "b"
tm.floorKey("b");      // largest key <= "b"
```

---

## 17. ConcurrentHashMap

- Thread-safe `HashMap` alternative — since Java 8 uses **CAS (Compare-And-Swap) + fine-grained bucket-level locking** (synchronized only on the specific bin during writes) instead of locking the whole map (unlike the legacy `Hashtable`).
- Does **not** allow `null` keys or values (to avoid ambiguity in concurrent reads).
- Iterators are **weakly consistent** (never throw `ConcurrentModificationException`, may or may not reflect concurrent updates).

```java
ConcurrentHashMap<String, Integer> chm = new ConcurrentHashMap<>();
chm.put("count", 0);
chm.compute("count", (k, v) -> v + 1);      // atomic update
chm.computeIfAbsent("newKey", k -> 100);
```

**vs `Collections.synchronizedMap()`**: the latter locks the entire map for every operation (coarse-grained), while `ConcurrentHashMap` allows much higher concurrent throughput.

**vs `Hashtable`**: `Hashtable` is legacy, synchronizes every method on the whole object, and disallows `null` keys/values too — `ConcurrentHashMap` is the modern, performant replacement.

---

## 18. Comparable

Interface implemented by the class itself to define its **natural ordering** — single method `compareTo()`.

```java
class Employee implements Comparable<Employee> {
    int salary;
    public int compareTo(Employee other) { return Integer.compare(this.salary, other.salary); }
}
List<Employee> list = ...;
Collections.sort(list);   // uses compareTo()
```

---

## 19. Comparator

External, pluggable comparison strategy — allows **multiple** sort orders without modifying the class.

```java
Comparator<Employee> byName = Comparator.comparing(e -> e.name);
Comparator<Employee> bySalaryDesc = Comparator.comparingInt((Employee e) -> e.salary).reversed();
Comparator<Employee> combo = byName.thenComparing(bySalaryDesc);

employees.sort(combo);
Collections.sort(employees, bySalaryDesc);
```

**Comparable vs Comparator**: `Comparable` defines a single natural order inside the class (`compareTo`); `Comparator` defines external, multiple, swappable orders (`compare`) — preferred when you don't own the class or need several sort strategies.

---

## 20. Iterator / Iterable

- `Iterable`: interface with `iterator()` method — implementing it enables the enhanced for-loop (`for (T t : collection)`).
- `Iterator`: cursor object with `hasNext()`, `next()`, `remove()` — allows **safe removal** during iteration.

```java
List<Integer> list = new ArrayList<>(List.of(1,2,3,4,5));
Iterator<Integer> it = list.iterator();
while (it.hasNext()) {
    int val = it.next();
    if (val % 2 == 0) it.remove();     // SAFE removal during iteration
}
```

**Common pitfall**: modifying a collection directly (`list.remove()`) while iterating with a for-each loop throws `ConcurrentModificationException` (fail-fast behavior via an internal `modCount` check) — always use `Iterator.remove()` or `removeIf()` instead.

```java
list.removeIf(val -> val % 2 == 0);   // preferred modern approach
```

**ListIterator** extends `Iterator` — adds `hasPrevious()`, `previous()`, `set()`, `add()` for bidirectional traversal (available only on `List`).
# DSA Java ↔ C++ Reference

> Compact interview reference.

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
12. Pair
13. String
14. StringBuilder
15. Graph
16. TreeNode
17. ListNode
18. Sorting
19. Binary Search Helpers
20. STL/Collections Utilities

------------------------------------------------------------------------

# 1. Arrays

## Java

``` java
int[] arr=new int[5];
arr[0]=10;
int x=arr[0];
arr.length;
Arrays.sort(arr);
Arrays.fill(arr,0);
```

## C++

``` cpp
int arr[5];
arr[0]=10;
int x=arr[0];
sort(arr,arr+5);
fill(arr,arr+5,0);
```

------------------------------------------------------------------------

# 2. ArrayList ↔ vector

## Java

``` java
ArrayList<Integer> list=new ArrayList<>();
list.add(10);
list.add(1,20);
list.get(0);
list.set(0,30);
list.remove(0);
list.remove(Integer.valueOf(30));
list.contains(20);
list.size();
list.isEmpty();
Collections.sort(list);
Collections.reverse(list);
```

## C++

``` cpp
vector<int> v;
v.push_back(10);
v.emplace_back(20);
v[0];
v.at(0);
v.pop_back();
v.size();
v.empty();
sort(v.begin(),v.end());
reverse(v.begin(),v.end());
```

------------------------------------------------------------------------

# 3. LinkedList

## Java

``` java
LinkedList<Integer> list=new LinkedList<>();
list.addFirst(1);
list.addLast(2);
list.getFirst();
list.getLast();
list.removeFirst();
list.removeLast();
```

## C++

``` cpp
list<int> lst;
lst.push_front(1);
lst.push_back(2);
lst.front();
lst.back();
lst.pop_front();
lst.pop_back();
```

------------------------------------------------------------------------

# 4. Stack

## Java

``` java
Stack<Integer> st=new Stack<>();
st.push(1);
st.peek();
st.pop();
st.empty();
st.size();
```

## C++

``` cpp
stack<int> st;
st.push(1);
st.top();
st.pop();
st.empty();
st.size();
```

------------------------------------------------------------------------

# 5. Queue

## Java

``` java
Queue<Integer> q=new LinkedList<>();
q.offer(1);
q.peek();
q.poll();
q.size();
q.isEmpty();
```

## C++

``` cpp
queue<int> q;
q.push(1);
q.front();
q.pop();
q.size();
q.empty();
```

------------------------------------------------------------------------

# 6. Deque

## Java

``` java
Deque<Integer> dq=new ArrayDeque<>();
dq.offerFirst(1);
dq.offerLast(2);
dq.peekFirst();
dq.peekLast();
dq.pollFirst();
dq.pollLast();
```

## C++

``` cpp
deque<int> dq;
dq.push_front(1);
dq.push_back(2);
dq.front();
dq.back();
dq.pop_front();
dq.pop_back();
```

------------------------------------------------------------------------

# 7. Priority Queue

## Java

``` java
PriorityQueue<Integer> minHeap=new PriorityQueue<>();
minHeap.offer(1);
minHeap.peek();
minHeap.poll();

PriorityQueue<Integer> maxHeap=
new PriorityQueue<>(Collections.reverseOrder());
```

## C++

``` cpp
priority_queue<int> maxHeap;
maxHeap.push(1);
maxHeap.top();
maxHeap.pop();

priority_queue<int,vector<int>,greater<int>> minHeap;
```

------------------------------------------------------------------------

# 8. HashMap ↔ unordered_map

## Java

``` java
HashMap<String,Integer> map=new HashMap<>();
map.put("a",1);
map.get("a");
map.getOrDefault("b",0);
map.containsKey("a");
map.containsValue(1);
map.remove("a");
map.size();
map.isEmpty();
for(var e:map.entrySet()){}
```

## C++

``` cpp
unordered_map<string,int> mp;
mp["a"]=1;
mp.insert({"b",2});
mp["a"];
mp.count("a");
mp.find("a");
mp.erase("a");
mp.size();
mp.empty();
for(auto &p:mp){}
```

------------------------------------------------------------------------

# 9. TreeMap ↔ map

## Java

``` java
TreeMap<Integer,String> map=new TreeMap<>();
map.put(1,"A");
map.firstKey();
map.lastKey();
map.floorKey(5);
map.ceilingKey(5);
map.lowerKey(5);
map.higherKey(5);
```

## C++

``` cpp
map<int,string> mp;
mp[1]="A";
mp.lower_bound(5);
mp.upper_bound(5);
mp.begin();
mp.rbegin();
```

------------------------------------------------------------------------

# 10. HashSet ↔ unordered_set

## Java

``` java
HashSet<Integer> set=new HashSet<>();
set.add(1);
set.contains(1);
set.remove(1);
set.size();
set.isEmpty();
```

## C++

``` cpp
unordered_set<int> st;
st.insert(1);
st.count(1);
st.erase(1);
st.size();
st.empty();
```

------------------------------------------------------------------------

# 11. TreeSet ↔ set

## Java

``` java
TreeSet<Integer> set=new TreeSet<>();
set.add(1);
set.first();
set.last();
set.floor(5);
set.ceiling(5);
set.lower(5);
set.higher(5);
```

## C++

``` cpp
set<int> st;
st.insert(1);
st.lower_bound(5);
st.upper_bound(5);
```

------------------------------------------------------------------------

# 12. Pair

## Java

``` java
Map.Entry<Integer,Integer> p=
new AbstractMap.SimpleEntry<>(1,2);
p.getKey();
p.getValue();
```

## C++

``` cpp
pair<int,int> p={1,2};
p.first;
p.second;
```

------------------------------------------------------------------------

# 13. String

## Java

``` java
String s="hello";
s.length();
s.charAt(0);
s.substring(1,3);
s.indexOf("e");
s.contains("el");
s.replace("a","b");
s.split(" ");
s.toCharArray();
```

## C++

``` cpp
string s="hello";
s.length();
s[0];
s.substr(1,3);
s.find("el");
s.replace(1,2,"ab");
reverse(s.begin(),s.end());
```

------------------------------------------------------------------------

# 14. StringBuilder

## Java

``` java
StringBuilder sb=new StringBuilder();
sb.append("a");
sb.insert(1,"b");
sb.delete(1,2);
sb.reverse();
sb.toString();
```

## C++

``` cpp
string s;
s+="a";
s.insert(1,"b");
s.erase(1,1);
```

------------------------------------------------------------------------

# 15. Graph

## Java

``` java
ArrayList<ArrayList<Integer>> g=new ArrayList<>();
g.get(u).add(v);
```

## C++

``` cpp
vector<vector<int>> g(n);
g[u].push_back(v);
```

------------------------------------------------------------------------

# 16. TreeNode

## Java

``` java
class TreeNode{
 int val;
 TreeNode left,right;
 TreeNode(int x){val=x;}
}
```

## C++

``` cpp
struct TreeNode{
 int val;
 TreeNode* left,*right;
 TreeNode(int x){val=x;left=right=NULL;}
};
```

------------------------------------------------------------------------

# 17. ListNode

## Java

``` java
class ListNode{
 int val;
 ListNode next;
 ListNode(int x){val=x;}
}
```

## C++

``` cpp
struct ListNode{
 int val;
 ListNode* next;
 ListNode(int x){val=x;next=NULL;}
};
```

------------------------------------------------------------------------

# 18. Sorting

## Java

``` java
Arrays.sort(arr);
Collections.sort(list);
Collections.reverse(list);
```

## C++

``` cpp
sort(arr,arr+n);
sort(v.begin(),v.end());
reverse(v.begin(),v.end());
```

------------------------------------------------------------------------

# 19. Binary Search Helpers

## Java

``` java
Collections.binarySearch(list,x);
Arrays.binarySearch(arr,x);
```

## C++

``` cpp
binary_search(v.begin(),v.end(),x);
lower_bound(v.begin(),v.end(),x);
upper_bound(v.begin(),v.end(),x);
```

------------------------------------------------------------------------

# 20. Common Algorithms

## Java

``` java
Math.max(a,b);
Math.min(a,b);
Math.abs(x);
Math.pow(a,b);
Arrays.fill(arr,0);
Collections.frequency(list,x);
Collections.swap(list,i,j);
```

## C++

``` cpp
min(a,b);
max(a,b);
swap(a,b);
accumulate(v.begin(),v.end(),0);
count(v.begin(),v.end(),x);
find(v.begin(),v.end(),x);
next_permutation(v.begin(),v.end());
prev_permutation(v.begin(),v.end());
unique(v.begin(),v.end());
distance(a,b);
```

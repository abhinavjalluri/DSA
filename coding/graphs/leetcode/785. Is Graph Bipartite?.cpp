// There is an undirected graph with n nodes, where each node is numbered between 0 and n - 1. You are given a 2D array graph, where graph[u] is an array of nodes that node u is adjacent to. More formally, for each v in graph[u], there is an undirected edge between node u and node v. The graph has the following properties:

// There are no self-edges (graph[u] does not contain u).
// There are no parallel edges (graph[u] does not contain duplicate values).
// If v is in graph[u], then u is in graph[v] (the graph is undirected).
// The graph may not be connected, meaning there may be two nodes u and v such that there is no path between them.
// A graph is bipartite if the nodes can be partitioned into two independent sets A and B such that every edge in the graph connects a node in set A and a node in set B.

// Return true if and only if it is bipartite.

 

// Example 1:


// Input: graph = [[1,2,3],[0,2],[0,1,3],[0,2]]
// Output: false
// Explanation: There is no way to partition the nodes into two independent sets such that every edge connects a node in one and a node in the other.
// Example 2:


// Input: graph = [[1,3],[0,2],[1,3],[0,2]]
// Output: true
// Explanation: We can partition the nodes into two sets: {0, 2} and {1, 3}.
 

// Constraints:

// graph.length == n
// 1 <= n <= 100
// 0 <= graph[u].length < n
// 0 <= graph[u][i] <= n - 1
// graph[u] does not contain u.
// All the values of graph[u] are unique.
// If graph[u] contains v, then graph[v] contains u.

// DFS approach

class Solution {
public:
    bool dfs(vector<vector<int>>&graph, int node, vector<int>& visited, int color){
        visited[node] = color;
        int ans = true;
        for(int neighbour : graph[node]){
            if(visited[neighbour] == -1){
                ans = ans && dfs(graph, neighbour, visited, 1-color);
            }
            if(visited[neighbour] == visited[node]){
                return false;
            }
        }
        return ans;
    }
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int>visited(n, -1);
        bool ans = true;
        for(int i = 0 ; i < n ; i++){
            if(visited[i] == -1){
                ans = ans && dfs(graph, i, visited, 0);
            }
        }
        return ans;
    }
};

// BFS approach

class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        // -1 = unvisited
        //  0 = color 0
        //  1 = color 1
        vector<int> color(n, -1);
        queue<int> q;
        for (int i = 0; i < n; i++) {
            // Handle disconnected components
            if (color[i] != -1){
                continue;
            }
            q.push(i);
            color[i] = 0;
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (int neighbor : graph[node]) {
                    // Not colored yet
                    if (color[neighbor] == -1) {
                        // Give opposite color
                        color[neighbor] = 1 - color[node];
                        q.push(neighbor);
                    }
                    // Same color on both ends -> not bipartite
                    else if (color[neighbor] == color[node]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};
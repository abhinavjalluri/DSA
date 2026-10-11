// There are n cities connected by some number of flights. You are given an array flights where flights[i] = [fromi, toi, pricei] indicates that there is a flight from city fromi to city toi with cost pricei.

// You are also given three integers src, dst, and k, return the cheapest price from src to dst with at most k stops. If there is no such route, return -1.

 

// Example 1:


// Input: n = 4, flights = [[0,1,100],[1,2,100],[2,0,100],[1,3,600],[2,3,200]], src = 0, dst = 3, k = 1
// Output: 700
// Explanation:
// The graph is shown above.
// The optimal path with at most 1 stop from city 0 to 3 is marked in red and has cost 100 + 600 = 700.
// Note that the path through cities [0,1,2,3] is cheaper but is invalid because it uses 2 stops.
// Example 2:


// Input: n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 1
// Output: 200
// Explanation:
// The graph is shown above.
// The optimal path with at most 1 stop from city 0 to 2 is marked in red and has cost 100 + 100 = 200.
// Example 3:


// Input: n = 3, flights = [[0,1,100],[1,2,100],[0,2,500]], src = 0, dst = 2, k = 0
// Output: 500
// Explanation:
// The graph is shown above.
// The optimal path with no stops from city 0 to 2 is marked in red and has cost 500.
 

// Constraints:

// 2 <= n <= 100
// 0 <= flights.length <= (n * (n - 1) / 2)
// flights[i].length == 3
// 0 <= fromi, toi < n
// fromi != toi
// 1 <= pricei <= 104
// There will not be any multiple flights between two cities.
// 0 <= src, dst, k < n
// src != dst



class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {

        vector<vector<pair<int, int>>> adj(n);
        for (auto &f : flights) {
            adj[f[0]].push_back({f[1], f[2]});
        }
        int maxFlights = k + 1;
        const int INF = INT_MAX;
        // dist[city][flightsUsed]
        vector<vector<int>> dist(
            n, vector<int>(maxFlights + 1, INF)
        );
        // {cost, city, flightsUsed}
        priority_queue<
            tuple<int, int, int>,
            vector<tuple<int, int, int>>,
            greater<tuple<int, int, int>>
        > pq;

        dist[src][0] = 0;
        pq.push({0, src, 0});

        while (!pq.empty()) {
            auto [cost, city, flightsUsed] = pq.top();
            pq.pop();

            // Skip outdated states
            if (cost != dist[city][flightsUsed]) {
                continue;
            }

            // Cheapest valid route to destination
            if (city == dst) {
                return cost;
            }

            // Cannot take more than k + 1 flights
            if (flightsUsed == maxFlights) {
                continue;
            }

            for (auto [nextCity, price] : adj[city]) {
                int newCost = cost + price;
                int newFlights = flightsUsed + 1;

                if (newCost < dist[nextCity][newFlights]) {
                    dist[nextCity][newFlights] = newCost;
                    pq.push({
                        newCost, nextCity, newFlights
                    });
                }
            }
        }

        return -1;
    }
};
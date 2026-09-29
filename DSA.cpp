
    void dfs(int node,
             vector<vector<int>>& adj,
             vector<bool>& visited) {

        if (visited[node])
            return;

        visited[node] = true;

        for (auto neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor, adj, visited);
            }
        }
    }

// when we just go through dfs hence visit node check is needed 
// but when we want to detect the cycle no visit check inside neigbours check 

bool dfs(int node,
             int parent,
             vector<vector<int>>& adj,
             vector<bool>& visited) {

        visited[node] = true;

        for (auto neighbor : adj[node]) {

            if (!visited[neighbor]) {

                if (!dfs(neighbor, node, adj, visited))
                    return false;
            }
            else if (neighbor != parent) {

                // Visited neighbor that isn't our parent
                // means we found a cycle.
                return false;
            }
        }

        return true;
    }

We process every edge once
→ O(E)

DFS:
Every node visited once  → O(V)
Every edge examined      → O(E)

So DFS is:
O(V + E)

Then checking visited:
O(V)

Total : (O(E)+O(V+E)+O(V)\)

TREE
 |
 ├── Connected
 |
 └── No cycle


n nodes
+
n-1 edges
+
connected
=
TREE


    == =======
    LAMBDA = small local function

Syntax:

[capture](parameters) {
    body
};

----------------------------

[]       → capture nothing

[&]      → everything by reference
           CAN modify originals

[=]      → everything by value
           COPY

[x]      → x by value

[&x]     → x by reference

[x, &y]  → x copy, y reference

----------------------------

Example:

auto add = [](int a, int b) {
    return a + b;
};

----------------------------

Your BFS:

auto tryMove = [&](int ni, int nj) {
    ...
};

[&]            → use outside variables
ni, nj         → function arguments
return         → exits lambda
tryMove(...)   → calls lambda
    == =======
===========================================================================
Indegree 0 → Queue → Remove edge → New indegree 0 → Count nodes → count == V
Course Schedule — Kahn’s Topological Sort

1. [a, b] means:
   b -> a

2. Build graph:
   adj[b].push_back(a)
   indegree[a]++

3. Push all indegree == 0 nodes into queue.

4. BFS:
   pop course
   count++

   for every neighbor:
      indegree[neighbor]--

      if indegree becomes 0:
         push neighbor

5. Final:
   count == numCourses → no cycle → true
   count < numCourses  → cycle → false

Why?
Cycle nodes never reach indegree 0.

Time:
Build graph = O(E)
Queue all nodes = O(V)
BFS nodes + edges = O(V + E)

TOTAL = O(V + E)

Space = O(V + E)


===========================================================================
NETWORK DELAY TIME = DIJKSTRA

Edge:
[u,v,w] => u -> v with weight w

1. Build adjacency list
2. distance[] = INF
3. distance[k] = 0
4. Min-heap {distance,node}

While heap:
    pop smallest distance

    if outdated:
        continue

    for neighbors:
        newDist = dist + weight

        if newDist < distance[nei]:
            update
            push into heap

Finally:
    any INF -> -1
    otherwise -> max(distance)

Why MAX?
Last node to receive signal determines total time.

Time: O((V+E) log V)
Space: O(V+E)
===========================================================================
    547. NUMBER OF PROVINCES

Think:
"How many separate groups?"

Example:
0 — 1     2 — 3     4

Groups = 3
Answer = 3

DFS/BFS pattern:
for each node:
    if NOT visited:
        count++
        visit whole group

KEY:
NEW unvisited node
      ↓
NEW province
      ↓
count++

Matrix:
isConnected[i][j] == 1
means i ↔ j

Complexity:
n rows × n columns
= O(n²)

DSU memory:
start groups = n

if union(a,b) merges 2 groups:
    groups--

Final groups = provinces
===========================================================================
Clone Graph BFS

MAP start clone
QUEUE start

while queue:
    pop curr

    for nei:
        if nei not cloned:
            clone nei
            map it
            push nei

        connect:
        clone[curr] -> clone[nei]

return clone[start]
    
    class Solution {
public:
    Node* cloneGraph(Node* node) {

        if (node == nullptr) {
            return nullptr;
        }

        unordered_map<Node*, Node*> copies;
        queue<Node*> q;

        // Clone starting node
        copies[node] = new Node(node->val);

        q.push(node);

        while (!q.empty()) {

            Node* current = q.front();
            q.pop();

            for (Node* neighbor : current->neighbors) {

                // Neighbor hasn't been cloned yet
                if (!copies.count(neighbor)) {

                    copies[neighbor] =
                        new Node(neighbor->val);

                    q.push(neighbor);
                }

                // Connect cloned current node
                // with cloned neighbor
                copies[current]->neighbors.push_back(
                    copies[neighbor]
                );
            }
        }

        return copies[node];
    }
};
===========================================================================
787. Cheapest Flights Within K Stops
class Solution {
public:
    int findCheapestPrice(
        int n,
        vector<vector<int>>& flights,
        int src,
        int dst,
        int k) {

        // ---------------------------------------------------------
        // STEP 1: Build adjacency list
        //
        // adj[u] = {v, price}
        //
        // Example:
        // flight = [0, 1, 100]
        // means:
        //
        // 0 ----100----> 1
        // ---------------------------------------------------------

        vector<vector<pair<int, int>>> adj(n);

        for (auto& flight : flights) {

            int from = flight[0];
            int to = flight[1];
            int price = flight[2];

            adj[from].push_back({to, price});
        }


        // ---------------------------------------------------------
        // IMPORTANT:
        //
        // k stops means maximum k + 1 edges/flights.
        //
        // Example:
        //
        // src -> A -> dst
        //
        // Stops = 1
        // Edges = 2
        //
        // Therefore:
        // maxEdges = k + 1
        // ---------------------------------------------------------

        int maxEdges = k + 1;


        // ---------------------------------------------------------
        // best[node][edgesUsed]
        //
        // = cheapest cost to reach "node"
        //   using exactly "edgesUsed" flights.
        //
        // Why do we need edgesUsed in our state?
        //
        // Because:
        // reaching the same node with different numbers of flights
        // represents DIFFERENT states.
        //
        // A slightly more expensive route using fewer flights
        // may later become the final cheapest valid route.
        // ---------------------------------------------------------

        vector<vector<int>> best(
            n,
            vector<int>(maxEdges + 1, INT_MAX)
        );


        // ---------------------------------------------------------
        // BFS queue state:
        //
        // {node, cost, edgesUsed}
        // ---------------------------------------------------------

        queue<tuple<int, int, int>> q;

        q.push({src, 0, 0});

        best[src][0] = 0;


        // ---------------------------------------------------------
        // STEP 2: BFS
        // ---------------------------------------------------------

        while (!q.empty()) {

            auto [currentNode, currentCost, edgesUsed] = q.front();
            q.pop();


            // -----------------------------------------------------
            // If we already used k + 1 edges,
            // we cannot take another flight.
            //
            // Example:
            // k = 1
            //
            // maximum edges = 2
            //
            // src -> A -> dst
            //
            // Once edgesUsed == 2,
            // stop expanding.
            // -----------------------------------------------------

            if (edgesUsed == maxEdges) {
                continue;
            }


            // -----------------------------------------------------
            // Explore all outgoing flights
            // -----------------------------------------------------

            for (auto [neighbor, price] : adj[currentNode]) {

                int newCost = currentCost + price;

                int newEdgesUsed = edgesUsed + 1;


                // -------------------------------------------------
                // Relaxation:
                //
                // Have we reached this neighbor
                // with the SAME number of edges
                // at a cheaper price?
                //
                // If yes:
                // update + push into queue.
                // -------------------------------------------------

                if (newCost < best[neighbor][newEdgesUsed]) {

                    best[neighbor][newEdgesUsed] = newCost;

                    q.push({
                        neighbor,
                        newCost,
                        newEdgesUsed
                    });
                }
            }
        }


        // ---------------------------------------------------------
        // STEP 3:
        //
        // Destination can be reached using:
        //
        // 1 edge
        // 2 edges
        // ...
        // k + 1 edges
        //
        // Take minimum among all valid possibilities.
        // ---------------------------------------------------------

        int answer = INT_MAX;

        for (int edges = 0; edges <= maxEdges; edges++) {

            answer = min(
                answer,
                best[dst][edges]
            );
        }


        // Destination unreachable
        if (answer == INT_MAX) {
            return -1;
        }

        return answer;
    }
};
===========================================================================\class Solution {
public:

    // Expand a string using already resolved replacements.
    string expand(
        string s,
        unordered_map<string, string>& resolved) {

        string result;

        int i = 0;

        while (i < s.size()) {

            // Normal character
            if (s[i] != '%') {
                result += s[i];
                i++;
            }

            else {
                // Placeholder format: %X%
                string key(1, s[i + 1]);

                result += resolved[key];

                // Skip %, X, %
                i += 3;
            }
        }

        return result;
    }


    string applySubstitutions(
        vector<vector<string>>& replacements,
        string text) {

        // raw[key] = original replacement string
        unordered_map<string, string> raw;

        // graph[A] contains keys that depend on A.
        //
        // Example:
        // B = "x%A%"
        //
        // A -> B
        unordered_map<string, vector<string>> graph;

        // indegree[key]
        // = number of unresolved dependencies
        unordered_map<string, int> indegree;


        // --------------------------------------------------
        // Step 1: Store raw replacement strings
        // --------------------------------------------------

        for (auto& r : replacements) {

            string key = r[0];
            string value = r[1];

            raw[key] = value;

            // Make sure key exists in indegree map
            indegree[key] = 0;
        }


        // --------------------------------------------------
        // Step 2: Build dependency graph
        // --------------------------------------------------

        for (auto& r : replacements) {

            string key = r[0];
            string value = r[1];

            int i = 0;

            while (i < value.size()) {

                if (value[i] != '%') {
                    i++;
                    continue;
                }

                // Found %X%
                string dependency(1, value[i + 1]);

                // dependency must be resolved before key
                //
                // dependency -> key
                graph[dependency].push_back(key);

                indegree[key]++;

                i += 3;
            }
        }


        // --------------------------------------------------
        // Step 3: Push all indegree 0 keys
        // --------------------------------------------------

        queue<string> q;

        for (auto& [key, degree] : indegree) {

            if (degree == 0) {
                q.push(key);
            }
        }


        // resolved[key] = fully expanded value
        unordered_map<string, string> resolved;


        // --------------------------------------------------
        // Step 4: Kahn's BFS
        // --------------------------------------------------

        while (!q.empty()) {

            string key = q.front();
            q.pop();


            // At this point all dependencies of key
            // are already resolved.
            resolved[key] =
                expand(raw[key], resolved);


            // Unlock keys that depend on this key
            for (string dependent : graph[key]) {

                indegree[dependent]--;

                if (indegree[dependent] == 0) {
                    q.push(dependent);
                }
            }
        }


        // --------------------------------------------------
        // Step 5: Expand original text
        // --------------------------------------------------

        return expand(text, resolved);
    }
};
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
    

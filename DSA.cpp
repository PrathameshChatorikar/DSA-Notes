
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
    class Solution {
public:

    class DSU {
    public:
        vector<int> parent;
        vector<int> rank;

        DSU(int n) {

            parent.resize(n);
            rank.resize(n, 0);

            // Initially every node is its own parent/component
            for (int i = 0; i < n; i++) {
                parent[i] = i;
            }
        }

        // Find representative/root of the component
        int find(int node) {

            if (parent[node] == node) {
                return node;
            }

            // Path compression
            return parent[node] = find(parent[node]);
        }

        // Merge two components
        void unite(int a, int b) {

            int rootA = find(a);
            int rootB = find(b);

            // Already connected
            if (rootA == rootB) {
                return;
            }

            // Union by rank
            if (rank[rootA] < rank[rootB]) {

                parent[rootA] = rootB;
            }
            else if (rank[rootA] > rank[rootB]) {

                parent[rootB] = rootA;
            }
            else {

                parent[rootB] = rootA;
                rank[rootA]++;
            }
        }
    };


    vector<bool> pathExistenceQueries(
        int n,
        vector<int>& nums,
        int maxDiff,
        vector<vector<int>>& queries) {

        DSU dsu(n);


        // --------------------------------------------------
        // nums is sorted.
        //
        // We only need to compare ADJACENT elements.
        //
        // If adjacent difference <= maxDiff:
        //
        // i-1 -------- i
        //
        // they belong to same connected component.
        //
        // If difference > maxDiff:
        //
        // i-1     |     i
        //         BREAK
        //
        // No node from the left can reach the right.
        // --------------------------------------------------

        for (int i = 1; i < n; i++) {

            if (nums[i] - nums[i - 1] <= maxDiff) {

                dsu.unite(i - 1, i);
            }
        }


        vector<bool> answer;

        // --------------------------------------------------
        // Query:
        //
        // If u and v have same root,
        // they belong to same component.
        //
        // Therefore a path exists.
        // --------------------------------------------------

        for (auto& query : queries) {

            int u = query[0];
            int v = query[1];

            if (dsu.find(u) == dsu.find(v)) {

                answer.push_back(true);
            }
            else {

                answer.push_back(false);
            }
        }

        return answer;
    }
};

3532 PATH EXISTENCE

nums is SORTED

[1,3,5,10]
   2 2  5
   ✅✅  ❌

0 --- 1 --- 2    |    3
                 BREAK

DSU:

for i = 1 → n-1:

    if nums[i] - nums[i-1] <= maxDiff:

        union(i-1, i)


QUERY:

find(u) == find(v)
        ↓
      TRUE


BIG GAP
   ↓
NEW COMPONENT
for (int i = 1; i < n; i++) {

    if (nums[i] - nums[i-1] <= maxDiff) {

        dsu.unite(i-1, i);
    }
}

for (auto q : queries) {

    ans.push_back(
        dsu.find(q[0]) == dsu.find(q[1])
    );
}
===========================================================================
    class Solution {
public:
    vector<int> findOrder(
        int numCourses,
        vector<vector<int>>& prerequisites) {

        // adj[b] contains courses that become available
        // after finishing course b.
        vector<vector<int>> adj(numCourses);

        // indegree[i] = number of prerequisites
        // still required for course i.
        vector<int> indegree(numCourses, 0);

        // --------------------------------------------------
        // Build graph
        //
        // [a, b] means:
        // b -> a
        // --------------------------------------------------
        for (auto& edge : prerequisites) {

            int course = edge[0];
            int prerequisite = edge[1];

            adj[prerequisite].push_back(course);

            indegree[course]++;
        }

        queue<int> q;

        // --------------------------------------------------
        // Courses with indegree 0 have no prerequisites.
        // They can be taken immediately.
        // --------------------------------------------------
        for (int course = 0; course < numCourses; course++) {

            if (indegree[course] == 0) {
                q.push(course);
            }
        }

        vector<int> order;

        // --------------------------------------------------
        // Kahn's BFS
        // --------------------------------------------------
        while (!q.empty()) {

            int course = q.front();
            q.pop();

            // This course can now be taken.
            order.push_back(course);

            // Finishing this course removes one prerequisite
            // from each dependent course.
            for (int neighbor : adj[course]) {

                indegree[neighbor]--;

                // All prerequisites satisfied.
                if (indegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }

        // --------------------------------------------------
        // If we processed every course:
        // valid topological order exists.
        //
        // Otherwise:
        // cycle exists -> impossible.
        // --------------------------------------------------
        if (order.size() != numCourses) {
            return {};
        }

        return order;
    }
};
210 COURSE SCHEDULE II

[a,b]
means:
b -> a

BUILD:
adj[b].push_back(a)
indegree[a]++

QUEUE:
push all indegree == 0

BFS:
while q:
    pop course
    order.push_back(course)

    for nei:
        indegree[nei]--

        if indegree[nei] == 0:
            q.push(nei)

FINAL:
order.size() == n
    return order

else
    return {}

Time = O(V + E)
Space = O(V + E)
===========================================================================
    class Solution {
public:
    vector<int> findMinHeightTrees(
        int n,
        vector<vector<int>>& edges) {

        // Special case:
        // One node itself is the answer.
        if (n == 1) {
            return {0};
        }

        // -----------------------------------------
        // Build adjacency list
        // -----------------------------------------
        vector<vector<int>> adj(n);

        // degree[i] = number of neighbors of node i
        vector<int> degree(n, 0);

        for (auto& edge : edges) {

            int u = edge[0];
            int v = edge[1];

            adj[u].push_back(v);
            adj[v].push_back(u);

            degree[u]++;
            degree[v]++;
        }

        queue<int> q;

        // -----------------------------------------
        // Initial leaves:
        //
        // In a tree, a leaf has degree 1.
        // -----------------------------------------
        for (int node = 0; node < n; node++) {

            if (degree[node] == 1) {
                q.push(node);
            }
        }

        int remainingNodes = n;

        // -----------------------------------------
        // Remove leaves level by level.
        //
        // Stop when only 1 or 2 nodes remain.
        // Those are the center(s).
        // -----------------------------------------
        while (remainingNodes > 2) {

            int leafCount = q.size();

            // Remove this entire outer layer
            remainingNodes -= leafCount;

            while (leafCount--) {

                int leaf = q.front();
                q.pop();

                // "Remove" leaf from graph
                for (int neighbor : adj[leaf]) {

                    degree[neighbor]--;

                    // If neighbor becomes degree 1,
                    // it becomes a new leaf.
                    if (degree[neighbor] == 1) {
                        q.push(neighbor);
                    }
                }
            }
        }

        // Whatever remains in queue
        // is the center: 1 or 2 nodes.
        vector<int> answer;

        while (!q.empty()) {
            answer.push_back(q.front());
            q.pop();
        }

        return answer;
    }
};
310 MINIMUM HEIGHT TREES

TREE CENTER problem

Leaves = degree 1

queue all leaves

while remainingNodes > 2:

    size = q.size()

    remainingNodes -= size

    remove all current leaves

    for neighbor:
        degree[neighbor]--

        if degree[neighbor] == 1:
            push neighbor

remaining 1 or 2 nodes
= answer
Time = O(V)
Space = O(V)
    Minimum Height Tree → trim leaves with BFS until 1 or 2 centers remain.
===========================================================================
    329 LONGEST INCREASING PATH

Think:
MATRIX -> DAG

smaller -> larger

For every cell:
    if neighbor > current:
        indegree[neighbor]++

Queue:
    all indegree == 0

BFS level by level:

while q:
    size = q.size()
    levels++

    while size--:
        pop cell

        for larger neighbor:
            indegree[neighbor]--

            if indegree == 0:
                push

answer = number of BFS levels
    class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {

        int rows = matrix.size();
        int cols = matrix[0].size();

        // indegree[r][c]
        // = number of smaller neighbors pointing to this cell
        vector<vector<int>> indegree(
            rows,
            vector<int>(cols, 0)
        );

        // --------------------------------------------------
        // STEP 1: Build indegree
        //
        // For each cell:
        // if neighbor is larger,
        //
        // current ---> neighbor
        //
        // therefore indegree[neighbor]++
        // --------------------------------------------------

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {

                // Down
                if (r + 1 < rows &&
                    matrix[r + 1][c] > matrix[r][c]) {

                    indegree[r + 1][c]++;
                }

                // Up
                if (r - 1 >= 0 &&
                    matrix[r - 1][c] > matrix[r][c]) {

                    indegree[r - 1][c]++;
                }

                // Right
                if (c + 1 < cols &&
                    matrix[r][c + 1] > matrix[r][c]) {

                    indegree[r][c + 1]++;
                }

                // Left
                if (c - 1 >= 0 &&
                    matrix[r][c - 1] > matrix[r][c]) {

                    indegree[r][c - 1]++;
                }
            }
        }

        queue<pair<int, int>> q;

        // --------------------------------------------------
        // STEP 2:
        // Push all cells with indegree 0.
        //
        // These are starting points of increasing paths.
        // --------------------------------------------------

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {

                if (indegree[r][c] == 0) {
                    q.push({r, c});
                }
            }
        }

        int longestPath = 0;

        // --------------------------------------------------
        // STEP 3: Kahn's BFS level by level
        //
        // Each BFS level corresponds to one additional
        // value in an increasing path.
        // --------------------------------------------------

        while (!q.empty()) {

            int levelSize = q.size();

            // We are processing one topological layer
            longestPath++;

            while (levelSize--) {

                auto [r, c] = q.front();
                q.pop();

                // ------------------------------------------
                // Visit all LARGER neighbors.
                //
                // Since current -> larger neighbor,
                // removing current decreases their indegree.
                // ------------------------------------------

                // Down
                if (r + 1 < rows &&
                    matrix[r + 1][c] > matrix[r][c]) {

                    indegree[r + 1][c]--;

                    if (indegree[r + 1][c] == 0) {
                        q.push({r + 1, c});
                    }
                }

                // Up
                if (r - 1 >= 0 &&
                    matrix[r - 1][c] > matrix[r][c]) {

                    indegree[r - 1][c]--;

                    if (indegree[r - 1][c] == 0) {
                        q.push({r - 1, c});
                    }
                }

                // Right
                if (c + 1 < cols &&
                    matrix[r][c + 1] > matrix[r][c]) {

                    indegree[r][c + 1]--;

                    if (indegree[r][c + 1] == 0) {
                        q.push({r, c + 1});
                    }
                }

                // Left
                if (c - 1 >= 0 &&
                    matrix[r][c - 1] > matrix[r][c]) {

                    indegree[r][c - 1]--;

                    if (indegree[r][c - 1] == 0) {
                        q.push({r, c - 1});
                    }
                }
            }
        }

        return longestPath;
    }
};
Time = O(R * C)
indegree = O(R * C)
queue    = O(R * C)

Space = O(R * C)
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
===========================================================================
    

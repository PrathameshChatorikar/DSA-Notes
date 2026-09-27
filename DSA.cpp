
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

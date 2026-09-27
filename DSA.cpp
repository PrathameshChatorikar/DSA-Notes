
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


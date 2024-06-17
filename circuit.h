#ifndef CIRCUIT_H_
#define CIRCUIT_H_

#include <set>
#include <vector>

class Circuit {
 private:
  // The function checks if a node in the graph has exactly one outgoing edge
  // and exactly one incoming edge.
  bool needs_resistance_adjustment(int node, std::vector<int>& indegree) {
    return adj_ls[node].size() == 1 && indegree[node] == 1;
  }

  // It combines parallel edges by recalculating their weights and updates the
  // graph accordingly.
  void adjust_parallel_connections(int node, std::vector<int>& indegree) {
    std::multiset<std::pair<int, double>>::iterator it = adj_ls[node].begin();

    while (it != adj_ls[node].end()) {
      std::multiset<std::pair<int, double>>::iterator start = it;
      std::multiset<std::pair<int, double>>::iterator end =
          ++std::multiset<std::pair<int, double>>::iterator(it);

      double result = 0;
      bool flag = false;

      while (end != adj_ls[node].end() && it->first == end->first) {
        result += 1.0 / end->second;
        end = adj_ls[node].erase(end);
        indegree[it->first]--;
        flag = true;
      }

      if (flag == true) {
        result += 1.0 / start->second;
        adj_ls[node].erase(start);
        adj_ls[node].insert({start->first, 1.0 / result});
      }

      it = end;
    }
  }

  /**
   * Performs a Depth-First Search (DFS) traversal starting from the specified
   * node.
   *
   * Template Parameters:
   *     - None.
   *
   * Parameters:
   *     - node: The starting node for DFS traversal.
   *     - indegree: Reference to a vector representing the in-degree of nodes.
   *
   * Returns:
   *     - None.
   *
   * Description:
   * This function traverses the graph starting from the node `node` using DFS.
   * During traversal, it checks for nodes where resistance adjustment is needed
   * using `needs_resistance_adjustment`. If adjustment is required, it merges
   * parallel connections by recalculating resistances and updating graph
   * structures. After completing the traversal and potential adjustments, it
   * ensures all parallel connections are properly adjusted by calling
   * `adjust_parallel_connections`.
   */
  void dfs(int node, std::vector<int>& indegree) {
    for (const std::pair<int, double>& it : adj_ls[node]) {
      int adj_node = it.first;
      int resistance = it.second;
      // checking for resistence adjustment
      if (needs_resistance_adjustment(adj_node, indegree)) {
        double new_resistence = resistance + adj_ls[adj_node].begin()->second;
        int node_to_connect = adj_ls[adj_node].begin()->first;

        adj_ls[adj_node].clear();
        indegree[adj_node]--;

        adj_ls[node].erase({adj_node, resistance});
        adj_ls[node].insert({node_to_connect, new_resistence});
      } else {
        dfs(adj_node, indegree);
      }
    }
    adjust_parallel_connections(node, indegree);
  }

  // Calculates the in-degree of nodes in a graph represented by `adj_ls` and
  // updates `indegree` vector accordingly.
  void calculate_indegree(std::vector<int>& indegree) {
    for (int node = 0; node < V; node++) {
      for (const std::pair<int, double>& it : adj_ls[node]) {
        indegree[it.first]++;
      }
    }
  }

 public:
  // Constructs a Circuit with a specified number of junctions `V` and
  // initializes an adjacency list of size `V`.
  Circuit(int V) {
    this->V = V;
    adj_ls.resize(V);
  }

  // Adds a junction from junction `u` to junction `v` with a given resistance
  // `resistance` to the adjacency list `adj_ls`.
  void connect(int u, int v, double resistance) {
    adj_ls[u].insert({v, resistance});
  }

  // Calculates the total resistance of the circuit using DFS traversal and
  // in-degree calculation, returning the resulting total resistance value.

  double calculate_total_resistence() {
    std::vector<int> indegree(V, 0);
    calculate_indegree(indegree);
    while (adj_ls[0].begin()->first != V - 1) {
      dfs(0, indegree);
    }
    return adj_ls[0].begin()->second;
  }

 private:
  int V;  // Number of vertices in the graph
  std::vector<std::multiset<std::pair<int, double>>>
      adj_ls;  // Adjacency list of the graph. {junction, resistance}
};

#endif  // OPEN_STL_ARRAY_

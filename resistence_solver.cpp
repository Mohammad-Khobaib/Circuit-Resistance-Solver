#include <iostream>
#include<set>
#include<vector>

using namespace std;

bool needs_resistance_adjustment(int node, vector<multiset<pair<int, double>>>& adj_ls, vector<int>& indegree) {
    return indegree[node] == 1 && adj_ls[node].size() == 1;
}


void adjust_parallel_connections(int node, vector<multiset<pair<int, double>>>& adj_ls, vector<int>& indegree) {
    multiset<pair<int, double>>::iterator it = adj_ls[node].begin();
    
    while (it != adj_ls[node].end()) {
        auto start = it;
        auto end = ++multiset<pair<int, double>>::iterator(it);
        
        double result = 0;
        bool flag = false;
        
        while (end != adj_ls[node].end() && it -> first == end -> first) {
            result += 1.0 / end -> second;
            end = adj_ls[node].erase(end);
            indegree[it -> first]--;
            flag = true;
        }
        
        if (flag == true) {
            result += 1.0 / start -> second;
            adj_ls[node].erase(start);
            adj_ls[node].insert({start -> first, 1.0 / result});
        }
        
        it = end;
    }
}

void dfs(int node, vector<multiset<pair<int, double>>>& adj_ls, vector<int>& indegree) {
    for (auto& [adj_node, resistence]: adj_ls[node]) {
        // checking for resistence adjustment
        if (needs_resistance_adjustment(adj_node, adj_ls, indegree)) {
            double new_resistence = resistence + adj_ls[adj_node].begin() -> second;
            int node_to_connect = adj_ls[adj_node].begin() -> first;
            
            adj_ls[adj_node].clear();
            indegree[adj_node]--;
            
            adj_ls[node].erase({adj_node, resistence});
            adj_ls[node].insert({node_to_connect, new_resistence});
        } else {
            dfs(adj_node, adj_ls, indegree);
        }
    }
    adjust_parallel_connections(node, adj_ls, indegree);
}

vector<int> calculate_indegree(vector<multiset<pair<int, double>>>& adj_ls) {
    int V = adj_ls.size();
    vector<int> indegree(V, 0);
    for (int node = 0; node < V; node++) {
        for (const auto& [adj_node, resistence]: adj_ls[node]) {
            indegree[adj_node]++;
        }
    }
    return indegree;
}

double calculate_total_resistence(vector<multiset<pair<int, double>>>& adj_ls) {
    int V = adj_ls.size();
    vector<int> indegree = calculate_indegree(adj_ls);
    while (adj_ls[0].begin() -> first != V - 1) {
        dfs(0, adj_ls, indegree);
    }
    return adj_ls[0].begin() -> second;
}

int main()
{
    // Creating multiset of {node, resistence}
    int V = 9;
    vector<multiset<pair<int, double>>> adj_ls(V);
    
    adj_ls[0].insert({1, 2});
    
    adj_ls[1].insert({2, 6});
    
    adj_ls[2].insert({3, 2});
    
    adj_ls[3].insert({4, 10});
    adj_ls[3].insert({7, 20});
    adj_ls[3].insert({8, 20});
    
    adj_ls[4].insert({5, 10});
    adj_ls[4].insert({6, 20});
    adj_ls[4].insert({7, 20});
    
    adj_ls[5].insert({6, 10});
    
    adj_ls[6].insert({7, 10});
    
    adj_ls[7].insert({8, 10});
    
    cout << calculate_total_resistence(adj_ls) << endl;
    return 0;
}

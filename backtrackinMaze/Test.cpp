 #include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

class Graph {
public:
    unordered_map<int, vector<int>> adjList; 

    // Add an edge between two nodes
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // Assuming it's an undirected graph
    }

   
    bool dfs(int current, int end, unordered_map<int, bool>& visited, vector<int>& path) {
        visited[current] = true;  // Mark the current node as visited
        path.push_back(current); // Add the current node to the path

        if (current == end) return true; 

        // Explore neighbors
        for (int neighbor : adjList[current]) {
            if (!visited[neighbor]) { // If the neighbor is not visited
                if (dfs(neighbor, end, visited, path)) { // Recursively visit the neighbor
                    return true; // Path to the end found
                }
            }
        }

        // Backtrack: unmark the node and remove it from the path
        visited[current] = false;
        path.pop_back();
        return false;
    }

    // Public function to check if a path exists and print the route
    bool findRoute(int start, int end) {
        unordered_map<int, bool> visited; // To track visited nodes
        vector<int> path;                 // To store the path
        if (dfs(start, end, visited, path)) {
            cout << "Path from " << start << " to " << end << ": ";
            for (int node : path) {
                cout << node << " ";
            }
            cout << endl;
            return true;
        }
        else {
            cout << "Failure! No path exists from node " << start << " to node " << end << "." << endl;
            return false;
        }
    }
};

int main() {
    Graph maze;

   
    maze.addEdge(0, 1);
    maze.addEdge(1, 2);
    maze.addEdge(2, 3);
    maze.addEdge(2, 7);
    maze.addEdge(3, 4);
    maze.addEdge(3, 5);
    maze.addEdge(3, 6);
    maze.addEdge(5, 6);
    maze.addEdge(7, 8);
    maze.addEdge(7, 9);
    maze.addEdge(9, 10);
    maze.addEdge(9, 11);
    maze.addEdge(11, 12);
    maze.addEdge(11, 13);
    maze.addEdge(11, 14);
    maze.addEdge(14, 15);
    maze.addEdge(15, 16);
    maze.addEdge(15, 17);
   // maze.addEdge(15, 18);
    maze.addEdge(15, 26);
    maze.addEdge(18, 19);
    maze.addEdge(19, 20);
    maze.addEdge(26, 24);
    maze.addEdge(26, 25);
    maze.addEdge(24, 23);
    maze.addEdge(23, 22);
    maze.addEdge(22, 20);
    maze.addEdge(20, 21);
    maze.addEdge(21, 27); // Exit node

    
    int start = 0; 
    int end = 27;  

   
    maze.findRoute(start, end);

    return 0;
}
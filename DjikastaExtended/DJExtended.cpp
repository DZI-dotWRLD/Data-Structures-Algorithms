#include <list>
#include <queue>
#include <iostream>
#include <limits.h>
#include <vector>
#include <iomanip>
using namespace std;

// Define INF as a large value to represent infinity
#define INF INT_MAX

// iPair ==> Integer Pair
typedef pair<int, int> iPair;


class Graph {
    int V; // Number of vertices
    list<iPair>* adj; // Adjacency list
    vector<int> dist; // To store distances
    vector<int> parent; // To store the shortest path tree

public:
    Graph(int V); // Constructor

    void addEdge(int u, int v, int w); // Function to add an edge
    void shortestPath(int s); // Function to compute shortest paths from source
    void printPath(int start, int end); // Function to print path from start to end
};


Graph::Graph(int V) {
    this->V = V;
    adj = new list<iPair>[V];
    dist.resize(V, INF);
    parent.resize(V, -1);
}

// Function to add an edge to the graph
void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w)); // Since the graph is undirected
}

// Function to compute shortest paths from source
void Graph::shortestPath(int src) {
    // Create a priority queue to store vertices being processed
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

    // Insert source into priority queue and initialize its distance as 0
    pq.push(make_pair(0, src));
    dist[src] = 0;

    // Process the priority queue
    while (!pq.empty()) {
        // Get the vertex with the minimum distance
        int u = pq.top().second;
        pq.pop();

        // Iterate through all adjacent vertices of the current vertex
        for (auto& neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            // If a shorter path to v is found
            if (dist[v] > dist[u] + weight) {
                // Update distance and push new distance to the priority queue
                dist[v] = dist[u] + weight;
                parent[v] = u; // Update parent to reconstruct path
                pq.push(make_pair(dist[v], v));
            }
        }
    }
}

// Function to print the path from start to end
void Graph::printPath(int start, int end) {
    if (parent[end] == -1) {
        cout << "No path exists from " << start << " to " << end << "." << endl;
        return;
    }

    vector<int> path;
    for (int v = end; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    cout << "Path from " << start << " to " << end << ": ";
    for (int j = path.size() - 1; j >= 0; --j) {
        cout << path[j];
        if (j != 0) cout << " -> ";
    }
    cout << endl;
}

// Driver's code
int main() {
    int V = 9; // Number of vertices
    Graph g(V);

    // Add edges to the graph
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    // Compute shortest paths from source node 0
    g.shortestPath(0);

    // User input for start and end nodes
    int start, end;
    while (true) {
        cout << "Enter starting node (or -1 to exit): ";
        cin >> start;
        if (start == -1) break; // Exit condition
        cout << "Enter ending node: ";
        cin >> end;

        // Print the path from start to end
        g.printPath(start, end);
    }

    return 0;
}



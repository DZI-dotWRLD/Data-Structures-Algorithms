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

// Class representing a graph using adjacency list representation
class Graph {
    int V; // Number of vertices
    list<iPair>* adj; // Adjacency list

public:
    Graph(int V); // Constructor

    void addEdge(int u, int v, int w); // Function to add an edge
    void shortestPath(int s); // Function to print shortest path from source
};

// Constructor to allocate memory for the adjacency list
Graph::Graph(int V) {
    this->V = V;
    adj = new list<iPair>[V];
}

// Function to add an edge to the graph
void Graph::addEdge(int u, int v, int w) {
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w)); // Since the graph is undirected
}

// Function to print shortest paths from source
void Graph::shortestPath(int src) {
    // Create a priority queue to store vertices being processed
    priority_queue<iPair, vector<iPair>, greater<iPair>> pq;

    // Create a vector to store distances and initialize all distances as INF
    vector<int> dist(V, INF);
    vector<int> parent(V, -1); // To store the shortest path tree

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

    // Print the header for the table
    cout << left << setw(15) << "Destination" << setw(15) << "Distance" << "Path" << endl;
    cout << "---------------------------------------------------------" << endl;

    // Print the shortest distances and paths
    for (int i = 0; i < V; ++i) {
        cout << left << setw(15) << i << setw(15) << dist[i] << " ";

        // Reconstruct the path from source to i
        vector<int> path;
        for (int v = i; v != -1; v = parent[v]) {
            path.push_back(v);
        }

        // Print the path in reverse order
        for (int j = path.size() - 1; j >= 0; --j) {
            cout << path[j];
            if (j != 0) cout << " -> ";
        }
        cout << endl;
    }
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

    // Call the shortestPath function
    g.shortestPath(0);

    return 0;
}


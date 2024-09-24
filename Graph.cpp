#include <iostream>
#include "Graph.hpp"
#include <stack>

Graph::Graph(int num_vertices) {
    this->num_vertices = num_vertices;
    adj.resize(num_vertices);
    adjT.resize(num_vertices);

    #ifdef DEBUG
        std::cout << "Graph initialized with " << num_vertices << " vertices" << std::endl;
    #endif
}

Graph::~Graph() {
    // didn't allocate any memory, so nothing to delete, will just clear the vector
    // adj.clear();
}

void Graph::resetGraph(int num_vertices) {
    this->num_vertices = num_vertices;
    adj.clear();
    adjT.clear();
    adj.resize(num_vertices);
    adjT.resize(num_vertices);
}

void Graph::addEdge(int u, int v, int weight) {
    // DIRECTED GRAPH
    adj[u].push_back(Edge(u, v, weight));
    adjT[v].push_back(Edge(v, u, weight));
}

void Graph::removeEdge(int u, int v) {
    // DIRECTED GRAPH
    for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
        if (it->v == v) {
            adj[u].erase(it);
            break;
        }
    }
}

std::vector<Edge> Graph::getEdges() const {
    std::vector<Edge> edges;
    for (int i = 0; i < num_vertices; ++i) {
        for (Edge edge : adj[i]) {
            edges.push_back(edge);
        }
    }
    return edges;
}

Edge& Graph::getEdge(int u, int v) {
    for (Edge& edge : adj[u]) {
        if (edge.v == v) {
            return edge;
        }
    }
    throw std::out_of_range("Edge does not exist");
}

std::vector<Edge> Graph::getNeighbors(int u) const {
    return adj[u];
}

bool Graph::isConnected() {
    return this->Kosaraju();
}

// DFS function for a graph
void Graph::DFS(int v, std::vector<bool>& visited, std::stack<int>& stack) {
    visited[v] = true;

    // Visit all neighbors of v
    for (const Edge& edge : adj[v]) {
        if (!visited[edge.v]) {
            DFS(edge.v, visited, stack);
        }
    }

    // Store the vertex after the DFS finishes
    stack.push(v);
}

// DFS function for the reversed graph
void Graph::reverseDFS(int v, std::vector<bool>& visited) {
    visited[v] = true;

    // Visit all neighbors in the reversed graph
    
    for (const Edge& edge : adjT[v]) {

        if (!visited[edge.v]) {
            reverseDFS(edge.v, visited);
        }
    }
}

bool Graph::Kosaraju() {
    // Step 1: Perform DFS on the original graph
    std::stack<int> stack;
    std::vector<bool> visited(this->getNumVertices(), false);

    // Perform DFS from the first vertex
    DFS(0, visited, stack);

    // Check if all vertices were visited
    for (int i = 0; i < this->getNumVertices(); i++) {
        if (!visited[i]) {
            return false; // Not strongly connected
        }
    }

    // Step 2: Perform DFS on the reversed graph
    visited.assign(this->getNumVertices(), false); // Reset visited array

    // Perform DFS on the vertex with the highest finishing time in original graph
    reverseDFS(stack.top(), visited);

    // Check if all vertices were visited in the reversed graph
    for (int i = 0; i < this->getNumVertices(); i++) {
        if (!visited[i]) {
            return false; // Not strongly connected
        }
    }

    return true; // Graph is strongly connected
}


void Graph::generateRandomEdges(int num_edges, unsigned int seed) {
    srand(seed);
    
    int edge_count = 0;
    while (edge_count < num_edges) {
        int u = rand() % num_vertices;
        int v = rand() % num_vertices;
        int weight = rand() % 100;

        // make sure we don't add the same edge twice or an edge to itself
        if (u != v && std::find(adj[u].begin(), adj[u].end(), v) == adj[u].end()) {
            addEdge(u, v, weight);
            edge_count++;
        }
    }

    #ifdef DEBUG
        std::cout << "Generated " << num_edges << " random edges" << std::endl;
    #endif
}

int Graph::getNumVertices() const {
    return num_vertices;
}

int Graph::getNumEdges() const {
    int edge_count = 0;
    for (int i = 0; i < num_vertices; ++i) {
        edge_count += adj[i].size();
    }
    return edge_count;
}

void Graph::show(){
    // export the graph to txt and run python file in another thread to show it
    std::ofstream out("graph.txt");
    out << num_vertices << std::endl;
    for (int i = 0; i < num_vertices; ++i) {
        for (Edge edge : adj[i]) {
            out << edge.u << " " << edge.v << " " << edge.weight << std::endl;
        }
    }
    out.close();

    system("python3 show_graph.py"); 

    // remove the file after showing
    remove("graph.txt");
}
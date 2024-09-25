#include <iostream>
#include "Graph.hpp"
#include <stack>

Graph::Graph(int num_vertices) {
    this->num_vertices = num_vertices;
    adj.resize(num_vertices);

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
    adj.resize(num_vertices);
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 0 || u >= num_vertices || v < 0 || v >= num_vertices) {
        return;
    }
    bool found = false;
    for (const Edge& edge : adj[u]) {
        if (edge == v) {
            found = true;
        }
    }
    if (!found) {
        adj[u].push_back(Edge(u, v, weight));
    }
    found = false;
    for (const Edge& edge : adj[v]) {
        if (edge == u) {
            found = true;
        }
    }
    if (!found) {
        adj[v].push_back(Edge(v, u, weight));
    }
}

void Graph::removeEdge(int u, int v) {
    auto it_u = std::remove_if(adj[u].begin(), adj[u].end(), [v](const Edge& edge) {
        return edge == v;
    });
    if (it_u != adj[u].end()) {
        adj[u].erase(it_u, adj[u].end());
    }

    auto it_v = std::remove_if(adj[v].begin(), adj[v].end(), [u](const Edge& edge) {
        return edge == u;
    });
    if (it_v != adj[v].end()) {
        adj[v].erase(it_v, adj[v].end());
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
    // use simple dfs, this is an undirected graph
    std::vector<bool> visited(num_vertices, false);
    DFS(0, visited);

    for (bool v : visited) {
        if (!v) {
            return false;
        }
    }

    return true;
}

// DFS function for a graph
void Graph::DFS(int v, std::vector<bool>& visited) {
    visited[v] = true;

    // Visit all neighbors of v
    for (const Edge& edge : adj[v]) {
        if (!visited[edge.v]) {
            DFS(edge.v, visited);
        }
    }
}

int Graph::getNumVertices() const {
    return num_vertices;
}
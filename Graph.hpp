#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>      // for the show() function

// Edge struct to simplify working with edges - better in terms of readability
// Not really necessary, we could use std::tuple<int, int, int> instead for all 3 values (which might have been better in terms of memory)
struct Edge {
    int u, v, weight;
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
};

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
    os << edge.u << " -> " << edge.v << " (" << edge.weight << ")";
    return os;
}


// Weighted, directed graph class
class Graph {
private:
    int num_vertices;                     // Number of vertices in the graph
    std::vector<std::vector<Edge>> adj;  // Adjacency list for each vertex
    
public:
    // Constructor to init a graph with the given number of vertices (no edges yet)
    Graph(int num_vertices);
    ~Graph();

    // Add an undirected edge between vertices u and v
    void addEdge(int u, int v, int weight);

    // Generate random edges for the graph based on the given number of edges and seed
    void generateRandomEdges(int num_edges, unsigned int seed);

    // Check if the graph is connected
    bool isConnected();

    // Show the graph in a GUI using python script
    // TODO DELETE?
    void show();

private:
    // Helper DFS function to visit all vertices connected to v
    // void DFS(int v, std::vector<bool>& visited);
};


#endif // GRAPH_HPP
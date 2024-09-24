#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>      // for the show() function
#include <stack>

// Edge struct to simplify working with edges - better in terms of readability
// Not really necessary, we could use std::tuple<int, int, int> instead for all 3 values (which might have been better in terms of memory)
struct Edge {
    int u, v, weight;
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}

    // Overload the == operator to compare edges
    bool operator==(const Edge& other) const {
        return u == other.u && v == other.v && weight == other.weight;
    }

    bool operator==(const int& other) const {
        return u == other || v == other;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        os << edge.u << " -> " << edge.v << " (" << edge.weight << ")";
        return os;
    }
};

// Weighted, directed graph class
class Graph {
private:
    int num_vertices;                     // Number of vertices in the graph
    std::vector<std::vector<Edge>> adj;  // Adjacency list for each vertex
    std::vector<std::vector<Edge>> adjT; // Transposed adjacency list for Kosaraju's algorithm
    
public:
    // Constructor to init a graph with the given number of vertices (no edges yet)
    Graph(int num_vertices);
    ~Graph();

    // Reset the graph with the given number of vertices
    void resetGraph(int num_vertices);

    // Add an undirected edge between vertices u and v
    void addEdge(int u, int v, int weight);

    // Remove an edge between vertices u and v
    void removeEdge(int u, int v);

    // Get edges for algorithms
    std::vector<Edge> getEdges() const;

    // Get neighbors of a vertex
    std::vector<Edge> getNeighbors(int u) const;

    Edge& getEdge(int u, int v);

    // Generate random edges for the graph based on the given number of edges and seed
    void generateRandomEdges(int num_edges, unsigned int seed);

    // Check if the graph is connected
    bool isConnected();

    // Get the number of vertices in the graph
    int getNumVertices() const;

    // Get the number of edges in the graph
    int getNumEdges() const;

    // Show the graph in a GUI using python script
    // TODO DELETE?
    void show();

private:
    // Helper DFS functions to visit all vertices in directed graph - using Kosaraju's algorithm
    void DFS(int v, std::vector<bool>& visited, std::stack<int>& stack);
    void reverseDFS(int v, std::vector<bool>& visited);
    bool Kosaraju();
};


#endif // GRAPH_HPP
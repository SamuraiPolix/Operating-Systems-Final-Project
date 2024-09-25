#include "MSTSolver.hpp"
#include <algorithm>
#include <climits>
#include <set>
#include <limits>
#include <tuple>


// ---------------------------- Calculate Metrics ----------------------------
int MSTSolver::totalWeight(Graph& graph) {
    std::vector<Edge> mst = solve(graph);
    return totalWeight(mst);
}

int MSTSolver::longestDistance(Graph& graph) {
    std::vector<Edge> mst = solve(graph);
    return longestDistance(mst);
}

int MSTSolver::shortestDistance(Graph& graph) {
    std::vector<Edge> mst = solve(graph);
    return shortestDistance(mst);
}

double MSTSolver::averageDistance(Graph& graph) {
    std::vector<Edge> mst = solve(graph);
    return totalWeight(mst);
}

int MSTSolver::totalWeight(std::vector<Edge>& mst){
    int totalWeight = 0;
    int numEdges = 0;
    for (const Edge& edge : mst) {
        totalWeight += edge.weight;
        numEdges++;
    }
    return static_cast<double>(totalWeight) / numEdges;
}

int MSTSolver::longestDistance(std::vector<Edge>& mst){
    int longestDistance = 0;
    for (const Edge& edge : mst) {
        longestDistance = std::max(longestDistance, edge.weight);
    }
    return longestDistance;
}

int MSTSolver::shortestDistance(std::vector<Edge>& mst){
    int shortestDistance = INT_MAX;
    for (const Edge& edge : mst) {
        shortestDistance = std::min(shortestDistance, edge.weight);
    }
    return shortestDistance;
}

double MSTSolver::averageDistance(std::vector<Edge>& mst){
    int totalWeight = 0;
    int numEdges = 0;
    for (const Edge& edge : mst) {
        totalWeight += edge.weight;
        numEdges++;
    }
    return static_cast<double>(totalWeight) / numEdges;
}

std::string MSTSolver::printMetrics(Graph& graph){
    std::vector<Edge> mst = solve(graph);
    return printMetrics(mst);
}

std::string MSTSolver::printMetrics(std::vector<Edge>& mst){
    std::string response = "Metrics:\n";
    response += "Total weight: " + std::to_string(totalWeight(mst)) + "\n";
    response += "Longest distance: " + std::to_string(longestDistance(mst)) + "\n";
    response += "Shortest distance: " + std::to_string(shortestDistance(mst)) + "\n";
    response += "Average distance: " + std::to_string(averageDistance(mst)) + "\n";
    return response;
}


// Helper function to find the root of a set in the disjoint-set/union-find structure
int find(std::vector<int>& parent, int i) {
    if (parent[i] != i) {
        parent[i] = find(parent, parent[i]);
    }
    return parent[i];
}

// Helper function to do union of two subsets in disjoint-set/union-find structure
void unionSets(std::vector<int>& parent, std::vector<int>& rank, int u, int v) {
    int rootU = find(parent, u);
    int rootV = find(parent, v);

    if (rank[rootU] < rank[rootV]) {
        parent[rootU] = rootV;
    } else if (rank[rootU] > rank[rootV]) {
        parent[rootV] = rootU;
    } else {
        parent[rootV] = rootU;
        rank[rootU]++;
    }
}

// Boruvka's algorithm implementation
std::vector<Edge> BoruvkaSolver::solve(Graph& graph) {
    if (!graph.isConnected()) {
        return {};
    }
    int numVertices = graph.getNumVertices();
    std::vector<Edge> mstEdges;
    std::vector<int> parent(numVertices);
    std::vector<int> rank(numVertices, 0);

    // Initialize each vertex as its own parent (disjoint sets)
    for (int i = 0; i < numVertices; ++i) {
        parent[i] = i;
    }

    int numComponents = numVertices;

    // Continue until there is only one component
    while (numComponents > 1) {
        // Array to store the cheapest outgoing edge for each component
        std::vector<Edge> cheapestEdge(numVertices, Edge{-1, -1, std::numeric_limits<int>::max()});

        // Traverse all edges and find the cheapest outgoing edge for each component
        for (const Edge& edge : graph.getEdges()) {
            int u = edge.u;
            int v = edge.v;
            int weight = edge.weight;

            // Find the set (component) for both vertices
            int setU = find(parent, u);
            int setV = find(parent, v);

            // We only care about outgoing edges, so ensure u -> v is the correct direction
            if (setU != setV) {
                // Update the cheapest outgoing edge for setU
                if (cheapestEdge[setU].weight > weight) {
                    cheapestEdge[setU] = edge;
                }
                // Update the cheapest outgoing edge for setV
                if (cheapestEdge[setV].weight > weight) {
                    cheapestEdge[setV] = edge;
                }
            }
        }

        // Add the cheapest edges to the MST and perform union of sets
        for (int i = 0; i < numVertices; ++i) {
            const Edge& edge = cheapestEdge[i];

            // If a valid cheapest edge was found for this component
            if (edge.u != -1 && edge.v != -1) {
                int setU = find(parent, edge.u);
                int setV = find(parent, edge.v);

                // If the components are different, include this edge in MST
                if (setU != setV) {
                    mstEdges.push_back(edge);
                    unionSets(parent, rank, setU, setV);
                    numComponents--;  // We've merged two components
                }
            }
        }

        // Reset the cheapest edges for the next iteration
        std::fill(cheapestEdge.begin(), cheapestEdge.end(), Edge{-1, -1, std::numeric_limits<int>::max()});
    }

    return mstEdges;
}

std::vector<Edge> PrimSolver::solve(Graph& graph) {
    if (!graph.isConnected()) {
        return {};
    }
    int numVertices = graph.getNumVertices();
    
    std::vector<int> key(numVertices, INT_MAX);  // Key values to pick the minimum edge weight
    std::vector<bool> inMST(numVertices, false); // To keep track of vertices included in MST
    std::vector<int> parent(numVertices, -1);    // Array to store the MST
    std::vector<Edge> mstEdges;

    key[0] = 0; // Start from vertex 0 (arbitrary choice)

    // Min-heap (or set) to get the vertex with the smallest key
    using Tuple = std::tuple<int, int, int>; // (key, vertex, parent)
    std::set<Tuple> pq;
    pq.insert(std::make_tuple(0, 0, -1)); // Starting with vertex 0

    while (!pq.empty()) {
        // Get the vertex with the smallest key value
        auto [minKey, u, parent_u] = *pq.begin();
        pq.erase(pq.begin());

        if (inMST[u]) continue; // Skip if it's already included
        inMST[u] = true;  // Mark it as included in the MST
        
        // If it's not the starting vertex, add the edge to MST
        if (parent_u != -1) {
            mstEdges.push_back(graph.getEdge(u, parent_u)); // Undirected edge between u and parent_u
        }

        // Loop over all neighbors of u (undirected edges u <-> v)
        for (const Edge& edge : graph.getNeighbors(u)) {
            int v = edge.v;
            int weight = edge.weight;

            // If v is not in MST and weight is smaller, update the key
            if (!inMST[v] && weight < key[v]) {
                pq.erase({key[v], v, u}); // Remove if already in the set
                key[v] = weight;
                pq.insert({key[v], v, u}); // Add updated key with new parent u
                parent[v] = u;
            }
        }
    }

    return mstEdges;
}

#include "Graph.hpp"

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

void Graph::addEdge(int u, int v, int weight) {
    adj[u].push_back(Edge(u, v, weight));
    adj[v].push_back(Edge(u, v, weight));
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
            addEdge(u, v);
            edge_count++;
        }
    }

    #ifdef DEBUG
        std::cout << "Generated " << num_edges << " random edges" << std::endl;
    #endif
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
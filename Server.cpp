#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include "Graph.hpp"
#include "MSTFactory.hpp"

using namespace std;        // TODO make it more specific later

// ---------------------------- Constants and Global vars ----------------------------
#define PORT 9034
#define MAXCONNECTIONS 10
std::mutex graphMutex;
Graph graph(0);

// ---------------------------- Declare Functions ----------------------------
void handle_client_command(int client_socket, const std::string& command);
void handle_client(int client_socket); 

// ---------------------------- Functions ----------------------------
void handle_client(int client_socket) {
    char buffer[1024];
    int bytesReceived;
    int expected_edges = 0;

    while ((bytesReceived = recv(client_socket, buffer, 1024, 0)) > 0) {
        std::istringstream iss(std::string(buffer, bytesReceived));
        std::string cmd;
        iss >> cmd;

        std::unique_lock<std::mutex> lock(graphMutex);

        bool validCommand = false;

        if (cmd == "Newgraph") {
            validCommand = true;
            int vertices, edges;
            if (iss >> vertices >> edges) {
                graph.resetGraph(vertices);
                std::cout << "Graph created with " << vertices << " vertices. Waiting for " << edges << " edges.\n";
                std::string response = "Graph created. Send " + std::to_string(edges) + " edges (u v weight).\n";
                send(client_socket, response.c_str(), response.size(), 0);
                expected_edges = edges;
            } else {
                std::cout << "Error: Invalid graph command format\n";
            }
        }
        
        while (expected_edges > 0) {
            cout << "Waiting for " << expected_edges << " edges...\n";
            bytesReceived = recv(client_socket, buffer, 1024, 0);
            if (bytesReceived <= 0) {
                close(client_socket);
                return;       // end client thread
            }
            std::istringstream iss(std::string(buffer, bytesReceived));
            int u, v, weight;
            iss >> u >> v >> weight;
            if (expected_edges <= 0) {
                break;
            }
            if (u >= 0 && v >= 0 && u < graph.getNumVertices() && v < graph.getNumVertices()) {
                graph.addEdge(u, v, weight);
                expected_edges--;
                std::cout << "Added edge " << u << "->" << v << " [" << weight << "]. " << expected_edges << " edges remaining.\n";
                std::string response = "Edge added. " + std::to_string(expected_edges) + " edges remaining.\n";
                send(client_socket, response.c_str(), response.size(), 0);
            } else {
                std::cout << "Error: Vertex index out of bounds\n";
            }
        }
        lock.unlock();
        
        if (cmd == "Newedge") {
            validCommand = true;
            int u, v, weight;
            if (iss >> u >> v >> weight) {
                if (u >= 0 && v >= 0 && u < graph.getNumVertices() && v < graph.getNumVertices()) {
                    lock.lock();
                    graph.addEdge(u, v, weight);
                    lock.unlock();
                    std::cout << "Added edge " << u << "->" << v << " [" << weight << "].\n";
                } else {
                    std::cout << "Error: Vertex index out of bounds\n";
                }
            } else {
                std::cout << "Error: Invalid edge command format\n";
            }
        }
        else if (cmd == "Removeedge") {
            validCommand = true;
            int u, v;
            if (iss >> u >> v) {
                if (u >= 0 && v >= 0 && u < graph.getNumVertices() && v < graph.getNumVertices()) {
                    lock.lock();
                    graph.removeEdge(u, v);
                    lock.unlock();
                    std::cout << "Removed edge from " << u << " to " << v << ".\n";
                } else {
                    std::cout << "Error: Vertex index out of bounds\n";
                }
            } else {
                std::cout << "Error: Invalid edge command format\n";
            }
        }
        else if (cmd == "Boruvka") {
            validCommand = true;
            std::unique_ptr<MSTSolver> solver = MSTFactory::createSolver(MSTFactory::BORUVKA);
            lock.lock();
            std::vector<Edge> mst = solver->solve(graph);
            lock.unlock();
            std::string response = "Minimum Spanning Tree (Boruvka):\n";
            for (const Edge& edge : mst) {
                response += std::to_string(edge.u) + " -> " + std::to_string(edge.v) + " (" + std::to_string(edge.weight) + ")\n";
            }
            send(client_socket, response.c_str(), response.size(), 0);
        }
        else if (cmd == "Prim") {
            validCommand = true;
            std::unique_ptr<MSTSolver> solver = MSTFactory::createSolver(MSTFactory::PRIM);
            lock.lock();
            std::vector<Edge> mst = solver->solve(graph);
            lock.unlock();
            std::string response = "Minimum Spanning Tree (Prim):\n";
            for (const Edge& edge : mst) {
                response += std::to_string(edge.u) + " -> " + std::to_string(edge.v) + " (" + std::to_string(edge.weight) + ")\n";
            }
            send(client_socket, response.c_str(), response.size(), 0);
        }
        else if (!validCommand) {
            std::cout << "Unknown command.\n";
        }
    }
    close(client_socket);
}


// ---------------------------- Main ----------------------------
int main() {
    // Create a socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        std::cerr << "socket: Could not create socket.\n";
        return 1;
    }

    // Set server address for binding
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Allow reuse of address
    int yes = 1;
    if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        std::cerr << "setsockopt: Could not set socket options.\n";
        return 2;
    }

    // Bind to port
    if (bind(server, (sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "bind: Could not bind to port " << PORT << ".\n";
        return 3;
    }

    // Listen on port for incoming connections
    if (listen(server, MAXCONNECTIONS) == -1) {
        std::cerr << "listen: Could not listen on port " << PORT << ".\n";
        // print error
        std::cout << "Error: " << strerror(errno) << "\n";
        return 4;
    }

    // Accept incoming connections and open a thread for each client
    int client;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    std::cout << "Waiting for connections..." << std::endl;
    while (true) {
        if ((client = accept(server, (struct sockaddr*)&clientAddr, &clientAddrSize)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        std::cout << "Client connected, socket " << client << std::endl;
        std::cout << "Still allowing more connections in the background..." << std::endl;

        // Handle client connection
        std::thread clientThread(handle_client, client);
        clientThread.detach(); // Detach thread to handle multiple clients
    }

    return 0;
}
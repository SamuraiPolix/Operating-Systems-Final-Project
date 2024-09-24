#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <vector>
#include "Graph.hpp"
#include "MSTFactory.hpp"
#include "MSTSolver.hpp"

TEST_CASE ("Test Non-connected graph") {
    // Based on test from https://www.geeksforgeeks.org/boruvkas-algorithm-greedy-algo-9/
    // Create a directed graph with 4 vertices
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);

    CHECK(!g.isConnected());

    // Run Boruvka's algo
    std::vector<Edge> mstEdges = MSTFactory::createSolver(MSTFactory::MSTType::BORUVKA)->solve(g);
    
    // Expected edges in the MST (directed)
    std::vector<Edge> expectedEdges = {};

    // Check the size of the MST
    CHECK(mstEdges.size() == expectedEdges.size());

    // Check the content of the MST
    for (const Edge& edge : mstEdges) {
        bool found = false;
        for (const Edge& expected : expectedEdges) {
            if (expected.u == edge.u && expected.v == edge.v && expected.weight == edge.weight) {
                found = true;
                break;
            }
        }
        std::cout << "Edge: " << edge.u << " -> " << edge.v << " (" << edge.weight << ")\n";        // for debugging
        CHECK(found);
    }

    // Run Prim's algo
    mstEdges = MSTFactory::createSolver(MSTFactory::MSTType::PRIM)->solve(g);

    // Expected edges in the MST (directed)
    expectedEdges = {};

    // Check the size of the MST
    CHECK(mstEdges.size() == expectedEdges.size());
    
    // Check the content of the MST
    for (const Edge& edge : mstEdges) {
        bool found = false;
        for (const Edge& expected : expectedEdges) {
            if (expected.u == edge.u && expected.v == edge.v && expected.weight == edge.weight) {
                found = true;
                break;
            }
        }
        std::cout << "Edge: " << edge.u << " -> " << edge.v << " (" << edge.weight << ")\n";        // for debugging
        CHECK(found);
    }
}

TEST_CASE ("Connected graph") {
    // directed graph but we made each edge bidirectional, connected
    Graph g(5);
    g.addEdge(0, 1, 2);
    g.addEdge(0, 3, 6);
    g.addEdge(1, 0, 2);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 8);
    g.addEdge(1, 4, 5);
    g.addEdge(2, 1, 3);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 0, 6);
    g.addEdge(3, 1, 8);
    g.addEdge(3, 4, 9);
    g.addEdge(4, 1, 5);
    g.addEdge(4, 2, 7);
    g.addEdge(4, 3, 9);

    CHECK(g.isConnected());

    // Run Boruvka's algo
    std::vector<Edge> mstEdges = MSTFactory::createSolver(MSTFactory::MSTType::BORUVKA)->solve(g);

    // Expected edges in the MST (directed)
    std::vector<Edge> expectedEdges = {
        {0, 1, 2},
        {1, 2, 3},
        {0, 3, 6},
        {1, 4, 5}
    };

    // Check the size of the MST
    CHECK(mstEdges.size() == expectedEdges.size());

    // Check the content of the MST
    for (const Edge& edge : mstEdges) {
        bool found = false;
        for (const Edge& expected : expectedEdges) {
            if (expected.u == edge.u && expected.v == edge.v && expected.weight == edge.weight) {
                found = true;
                break;
            }
        }
        std::cout << "Edge: " << edge.u << " -> " << edge.v << " (" << edge.weight << ")\n";        // for debugging
        CHECK(found);
    }

    // Run Prim's algo
    mstEdges = MSTFactory::createSolver(MSTFactory::MSTType::PRIM)->solve(g);

    // Check the size of the MST
    CHECK(mstEdges.size() == expectedEdges.size());

    // Check the content of the MST
    for (const Edge& edge : mstEdges) {
        bool found = false;
        for (const Edge& expected : expectedEdges) {
            if (expected.u == edge.u && expected.v == edge.v && expected.weight == edge.weight) {
                found = true;
                break;
            }
        }
        std::cout << "Edge: " << edge.u << " -> " << edge.v << " (" << edge.weight << ")\n";        // for debugging
        CHECK(found);
    }
}
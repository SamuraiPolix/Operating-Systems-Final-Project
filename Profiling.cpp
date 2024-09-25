#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Graph.hpp"
#include <stack>
#include "MSTFactory.hpp"
#include "MSTSolver.hpp"


#define NUM_VERTICES 50000

int main(){
    std::srand(std::time(0)); // Seed the random number generator

    Graph g(NUM_VERTICES);

    for (int i = 0; i < NUM_VERTICES-1; i++) {
        g.addEdge(i, i+1, std::rand() % 100);
    }

    std::vector<Edge> mstEdges = MSTFactory::createSolver(MSTFactory::MSTType::PRIM)->solve(g);
    std::cout << "MST size: " << mstEdges.size() << std::endl;

    std::vector<Edge> mstEdges2 = MSTFactory::createSolver(MSTFactory::MSTType::BORUVKA)->solve(g);
    std::cout << "MST size: " << mstEdges2.size() << std::endl;

    return 0;
}
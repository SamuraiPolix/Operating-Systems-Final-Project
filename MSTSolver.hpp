#ifndef MST_SOLVER_HPP
#define MST_SOLVER_HPP

#include <vector>
#include "Graph.hpp"

class MSTSolver {
public:
    virtual ~MSTSolver() {}
    // Solve the MST problem for the given graph
    virtual std::vector<Edge> solve(Graph& graph) = 0;
    // Total weight of the MST
    virtual int totalWeight(Graph& graph);
    // Longest distance between two vertices
    virtual int longestDistance(Graph& graph);
    // Shortest distance between two vertices
    virtual int shortestDistance(Graph& graph);
    /*
     * Average distance between two edges in the graph
     * assume distance (x,x)=0 for any X, We are interested in avg of all distances Xi,Xj where i=1..n j≥i.
     */
    virtual double averageDistance(Graph& graph);

    // if we have the MST, we can calculate the metrics without solving the MST again
    virtual int totalWeight(std::vector<Edge>& mst);
    virtual int longestDistance(std::vector<Edge>& mst);
    virtual int shortestDistance(std::vector<Edge>& mst);
    virtual double averageDistance(std::vector<Edge>& mst);

    std::string printMetrics(std::vector<Edge>& mst);
    std::string printMetrics(Graph& graph);

};

class BoruvkaSolver : public MSTSolver {
public:
    std::vector<Edge> solve(Graph& graph) override;
    // virtual int totalWeight(Graph& graph);
};

class PrimSolver : public MSTSolver {
public:
    std::vector<Edge> solve(Graph& graph) override;
    // virtual int totalWeight(Graph& graph);
};

#endif // MST_SOLVER_HPP
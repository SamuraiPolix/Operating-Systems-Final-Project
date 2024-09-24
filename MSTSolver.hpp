#ifndef MST_SOLVER_HPP
#define MST_SOLVER_HPP

#include <vector>
#include "Graph.hpp"

class MSTSolver {
public:
    virtual ~MSTSolver() {}
    virtual std::vector<Edge> solve(Graph& graph) = 0;
    virtual int totalWeight(Graph& graph);
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
#ifndef MSTFACTORY_HPP
#define MSTFACTORY_HPP

#include "Graph.hpp"
#include "MSTSolver.hpp"
#include <memory>

class MSTFactory {
public:
    enum MSTType { BORUVKA, PRIM };
    // using unique_ptr to avoid memory leaks (and some more advantages)
    static std::unique_ptr<MSTSolver> createSolver(MSTType type);
};

#endif // MSTFACTORY_HPP
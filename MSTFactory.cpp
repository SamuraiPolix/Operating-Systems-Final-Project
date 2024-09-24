#include "MSTFactory.hpp"

std::unique_ptr<MSTSolver> MSTFactory::createSolver(MSTType type) {
    switch (type) {
        case BORUVKA:
            return std::unique_ptr<MSTSolver>(new BoruvkaSolver());
        case PRIM:
            return std::unique_ptr<MSTSolver>(new PrimSolver());
        default:
            std::cout << "Invalid MST type" << std::endl;
            return nullptr;
    }
}
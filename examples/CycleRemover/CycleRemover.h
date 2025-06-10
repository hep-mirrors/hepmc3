#ifndef CYCLE_REMOVER_H
#define CYCLE_REMOVER_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"

namespace HepMC3 {

class CycleRemover {
public:
    void mergeCycle(std::shared_ptr<GenEvent> event);

private:
    bool detectCycleDFS(std::shared_ptr<GenVertex> v, std::shared_ptr<GenVertex> parent,
                        std::unordered_set<std::shared_ptr<GenVertex>>& visited,
                        std::unordered_set<std::shared_ptr<GenVertex>>& cycleNodes);
};

} // namespace HepMC3

#endif // CYCLE_REMOVER_H

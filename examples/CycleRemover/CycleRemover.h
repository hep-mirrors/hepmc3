#ifndef CYCLE_REMOVER_H
#define CYCLE_REMOVER_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"

using namespace HepMC3;
using namespace std;

class CycleRemover {
public:
    void mergeCycle(shared_ptr<GenEvent> event);

private:
    bool detectCycleDFS(shared_ptr<GenVertex> v, shared_ptr<GenVertex> parent,
                        unordered_set<shared_ptr<GenVertex>>& visited,
                        unordered_set<shared_ptr<GenVertex>>& cycleNodes);
};

#endif // CYCLE_REMOVER_H

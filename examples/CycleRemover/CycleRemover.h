#ifndef CYCLE_REMOVER_H
#define CYCLE_REMOVER_H

#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"

namespace HepMC3 {

class CycleRemover {

public:
    CycleRemover(const std::shared_ptr<GenEvent>& e) {
        m_original_event = std::make_shared<HepMC3::GenEvent>(*(e.get()));
    }
    CycleRemover(const GenEvent& e) {
        m_original_event = std::make_shared<HepMC3::GenEvent>(e);
    }
    CycleRemover(const GenEvent* e) {
        m_original_event = std::make_shared<HepMC3::GenEvent>(*e);
    }
    std::shared_ptr<GenEvent> mergeCycle();
    ~CycleRemover() {}
private:
    std::shared_ptr<GenEvent> m_original_event = nullptr;
//std::shared_ptr<GenEvent> m_event = nullptr;
    bool detectCycleDFS(HepMC3::GenVertexPtr v, HepMC3::GenVertexPtr parent,
                        std::unordered_set<HepMC3::GenVertexPtr>& visited,
                        std::unordered_set<HepMC3::GenVertexPtr>& cycleNodes);
};

} // namespace HepMC3

#endif // CYCLE_REMOVER_H

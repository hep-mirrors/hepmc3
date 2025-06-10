#include "CycleRemover.h"

namespace HepMC3 {

std::shared_ptr<GenEvent> CycleRemover::mergeCycle() {
    std::shared_ptr<GenEvent> m_event = std::make_shared<HepMC3::GenEvent>(*(m_original_event.get()));


    std::unordered_set<HepMC3::GenVertexPtr> cycleVertices;

    // Detect cycle using DFS
    auto cycleStart = HepMC3::GenVertexPtr{nullptr};
    for (auto v : m_event->vertices()) {
        std::unordered_set<HepMC3::GenVertexPtr> visited;
        if (detectCycleDFS(v, nullptr, visited, cycleVertices)) {
            cycleStart = v;
            break;
        }
    }

    if (!cycleStart || cycleVertices.empty()) {
        std::cout << "No cycle detected." << std::endl;
        return m_event;
    }

    // Create a new merged vertex
    auto mergedVertex = std::make_shared<GenVertex>();
    m_event->add_vertex(mergedVertex);

    // Redirect edges to the new merged vertex
    for (auto v : cycleVertices) {
        for (auto p : v->particles_out()) {
            if (cycleVertices.find(p->end_vertex()) == cycleVertices.end()) {
                mergedVertex->add_particle_in(p);
            }
        }
        for (auto p : v->particles_in()) {
            if (cycleVertices.find(p->production_vertex()) == cycleVertices.end()) {
                mergedVertex->add_particle_out(p);
            }
        }
    }

    // Remove old cycle vertices
    for (auto v : cycleVertices) {
        m_event->remove_vertex(v);
    }

    std::cout << "Merged cycle vertices into a single vertex." << std::endl;
    return m_event;
}

bool CycleRemover::detectCycleDFS(HepMC3::GenVertexPtr v, HepMC3::GenVertexPtr parent,
                                  std::unordered_set<HepMC3::GenVertexPtr>& visited,
                                  std::unordered_set<HepMC3::GenVertexPtr>& cycleNodes) {
    if (!v || visited.find(v) != visited.end()) {
        return false;
    }

    visited.insert(v);
    cycleNodes.insert(v);

    for (auto p : v->particles_out()) {
        auto nextVertex = p->end_vertex();
        if (nextVertex && nextVertex != parent) {
            if (visited.find(nextVertex) != visited.end() ||
                    detectCycleDFS(nextVertex, v, visited, cycleNodes)) {
                return true;
            }
        }
    }

    cycleNodes.erase(v);
    return false;
}

} // namespace HepMC3

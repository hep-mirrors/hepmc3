#include "CycleRemover.h"

namespace HepMC3 {

void CycleRemover::mergeCycle(std::shared_ptr<GenEvent> event) {
    std::unordered_set<std::shared_ptr<GenVertex>> cycleVertices;

    // Detect cycle using DFS
    auto cycleStart = std::shared_ptr<GenVertex>{nullptr};
    for (auto v : event->vertices()) {
        std::unordered_set<std::shared_ptr<GenVertex>> visited;
        if (detectCycleDFS(v, nullptr, visited, cycleVertices)) {
            cycleStart = v;
            break;
        }
    }

    if (!cycleStart || cycleVertices.empty()) {
        std::cout << "No cycle detected." << std::endl;
        return;
    }

    // Create a new merged vertex
    auto mergedVertex = std::make_shared<GenVertex>();
    event->add_vertex(mergedVertex);

    // Redirect edges to the new merged vertex
    for (auto v : cycleVertices) {
        for (auto p : v->particles_out()) {
            if (cycleVertices.find(p->end_vertex()) == cycleVertices.end()) {
                mergedVertex->add_particle_in(p);
                p->set_end_vertex(mergedVertex);
            }
        }
        for (auto p : v->particles_in()) {
            if (cycleVertices.find(p->production_vertex()) == cycleVertices.end()) {
                mergedVertex->add_particle_out(p);
                p->set_production_vertex(mergedVertex);
            }
        }
    }

    // Remove old cycle vertices
    for (auto v : cycleVertices) {
        event->remove_vertex(v);
    }

    std::cout << "Merged cycle vertices into a single vertex." << std::endl;
}

bool CycleRemover::detectCycleDFS(std::shared_ptr<GenVertex> v, std::shared_ptr<GenVertex> parent,
                                  std::unordered_set<std::shared_ptr<GenVertex>>& visited,
                                  std::unordered_set<std::shared_ptr<GenVertex>>& cycleNodes) {
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

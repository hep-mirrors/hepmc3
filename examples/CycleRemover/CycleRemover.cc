#include "CycleRemover.h"

void CycleRemover::mergeCycle(shared_ptr<GenEvent> event) {
    unordered_set<shared_ptr<GenVertex>> cycleVertices;

    // Detect cycle using DFS
    shared_ptr<GenVertex> cycleStart = nullptr;
    for (auto v : event->vertices()) {
        unordered_set<shared_ptr<GenVertex>> visited;
        if (detectCycleDFS(v, nullptr, visited, cycleVertices)) {
            cycleStart = v;
            break;
        }
    }

    if (!cycleStart || cycleVertices.empty()) {
        cout << "No cycle detected." << endl;
        return;
    }

    // Create a new merged vertex
    shared_ptr<GenVertex> mergedVertex = make_shared<GenVertex>();
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

    cout << "Merged cycle vertices into a single vertex." << endl;
}

bool CycleRemover::detectCycleDFS(shared_ptr<GenVertex> v, shared_ptr<GenVertex> parent,
                                  unordered_set<shared_ptr<GenVertex>>& visited,
                                  unordered_set<shared_ptr<GenVertex>>& cycleNodes) {
    if (!v || visited.find(v) != visited.end()) {
        return false;
    }

    visited.insert(v);
    cycleNodes.insert(v);

    for (auto p : v->particles_out()) {
        shared_ptr<GenVertex> nextVertex = p->end_vertex();
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

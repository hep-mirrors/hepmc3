#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"

using namespace HepMC3;
using namespace std;

class GraphProcessor {
public:
    void mergeCycle(shared_ptr<GenEvent> event) {
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

private:
    bool detectCycleDFS(shared_ptr<GenVertex> v, shared_ptr<GenVertex> parent, 
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
};

int main() {
    auto event = make_shared<GenEvent>();

    // Create example vertices using shared_ptr
    auto v0 = make_shared<GenVertex>();
    auto v1 = make_shared<GenVertex>();
    auto v2 = make_shared<GenVertex>();
    auto v3 = make_shared<GenVertex>();
    
    event->add_vertex(v0);
    event->add_vertex(v1);
    event->add_vertex(v2);
    event->add_vertex(v3);

    // Create example particles (edges) using shared_ptr
    auto p01 = make_shared<GenParticle>();
    auto p12 = make_shared<GenParticle>();
    auto p23 = make_shared<GenParticle>();
    auto p30 = make_shared<GenParticle>(); // Introduces a cycle

    p01->set_production_vertex(v0);
    p01->set_end_vertex(v1);
    p12->set_production_vertex(v1);
    p12->set_end_vertex(v2);
    p23->set_production_vertex(v2);
    p23->set_end_vertex(v3);
    p30->set_production_vertex(v3);
    p30->set_end_vertex(v0); // Closing cycle

    v0->add_particle_out(p01);
    v1->add_particle_out(p12);
    v2->add_particle_out(p23);
    v3->add_particle_out(p30);

    cout << "Before merging cycle:" << endl;
    for (auto v : event->vertices()) {
        cout << "Vertex: " << v << " connected to " << v->particles_out().size() << " particles." << endl;
    }

    GraphProcessor processor;
    processor.mergeCycle(event);

    cout << "\nAfter merging cycle:" << endl;
    for (auto v : event->vertices()) {
        cout << "Vertex: " << v << " connected to " << v->particles_out().size() << " particles." << endl;
    }

    return 0;
}

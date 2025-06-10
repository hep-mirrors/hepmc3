#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"
#include "CycleRemover.h"

using namespace HepMC3;
using namespace std;

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

    CycleRemover processor;
    processor.mergeCycle(event);

    cout << "\nAfter merging cycle:" << endl;
    for (auto v : event->vertices()) {
        cout << "Vertex: " << v << " connected to " << v->particles_out().size() << " particles." << endl;
    }

    return 0;
}

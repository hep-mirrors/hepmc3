#include <iostream>
#include <unordered_set>
#include <memory>
#include "HepMC3/GenEvent.h"
#include "CycleRemover.h"

using namespace HepMC3;

int main() {
    auto event = std::make_shared<GenEvent>();

    // Create example vertices using shared_ptr
    auto v0 = std::make_shared<GenVertex>();
    auto v1 = std::make_shared<GenVertex>();
    auto v2 = std::make_shared<GenVertex>();
    auto v3 = std::make_shared<GenVertex>();

    event->add_vertex(v0);
    event->add_vertex(v1);
    event->add_vertex(v2);
    event->add_vertex(v3);

    // Create example particles (edges) using shared_ptr
    auto p01 = std::make_shared<GenParticle>();
    auto p12 = std::make_shared<GenParticle>();
    auto p23 = std::make_shared<GenParticle>();
    auto p30 = std::make_shared<GenParticle>(); // Introduces a cycle

    v0->add_particle_out(p01);
    v1->add_particle_in(p01);

    v1->add_particle_out(p12);
    v2->add_particle_in(p12);

    v2->add_particle_out(p23);
    v3->add_particle_in(p23);

    v3->add_particle_out(p30);
    v0->add_particle_in(p30); // Closing cycle


    v0->add_particle_out(p01);
    v1->add_particle_out(p12);
    v2->add_particle_out(p23);
    v3->add_particle_out(p30);

    cout << "Before merging cycle:" << endl;
    for (auto v : event->vertices()) {
        cout << "Vertex: " << v << " connected to " << v->particles_out().size() << " particles." << endl;
    }

    //CycleRemover processor(event);
    //auto new_event = processor.mergeCycle();
    auto new_event = CycleRemover(event).mergeCycle();

    cout << "\nAfter merging cycle:" << endl;
    for (auto v : new_event->vertices()) {
        cout << "Vertex: " << v << " connected to " << v->particles_out().size() << " particles." << endl;
    }

    return 0;
}

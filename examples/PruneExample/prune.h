#ifndef HEPMC3_PRUNE_H
#define HEPMC3_PRUNE_H
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/ReaderAscii.h>
#include <HepMC3/WriterAscii.h>
#include <list>
namespace HepMC3 {
// Assuming vector starage and allowing momenta noncoservation 
template <typename Select>
void remove_particles1(GenEvent& event, Select select) {
    auto& particles = event.particles();
    std::vector<GenParticlePtr> part_toremove;
    part_toremove.reserve(particles.size());
    std::vector<GenVertexPtr> vert_toremove;
    vert_toremove.reserve(event.vertices_size());
    for (auto& particle : particles) {
        if (select(particle)) continue;
        part_toremove.push_back(particle);
        auto endVtx = particle->end_vertex();
        auto prdVtx = particle->production_vertex();
        if (endVtx) endVtx->remove_particle_in(particle);
        if (prdVtx) prdVtx->remove_particle_out(particle);
        if (endVtx && prdVtx && prdVtx != endVtx && endVtx->particles_in_size() == 0) {
          for (auto& outgoing : endVtx->particles_out()) prdVtx->add_particle_out(outgoing);
        }
    }
    std::sort(part_toremove.begin(),part_toremove.end(), [](const auto& a, consta auto& b) {return a->id()<b->id();});
    for (auto& particle : part_toremove) event.remove_particle(particle);
    for (auto& vtx : event.vertices()) if (vtx->particles_out().empty() && vtx->particles_in().empty()) vert_toremove.push_back(vtx);
    std::sort(vert_toremove.begin(),vert_toremove.end(), [](const auto& a, consta auto& b) {return a->id()>b->id();});
    for (auto& vtx : vert_toremove) event.remove_vertex(vtx);
}

// Assuming list starage and allowing momenta noncoservation
template <typename Select>
void remove_particles2(GenEvent& event, Select select) {
    auto& particles = event.particles();
    std::vector<GenParticlePtr> part_toremove;
    part_toremove.reserve(particles.size());
    std::vector<GenVertexPtr> vert_toremove;
    vert_toremove.reserve(event.vertices_size());
    for (auto& particle : particles) {
        if (select(particle)) continue;
        part_toremove.push_back(particle);
        auto endVtx = particle->end_vertex();
        auto prdVtx = particle->production_vertex();
        if (endVtx) endVtx->remove_particle_in(particle);
        if (prdVtx) prdVtx->remove_particle_out(particle);
        if (endVtx && prdVtx && prdVtx != endVtx && endVtx->particles_in_size() == 0) {
          for (auto& outgoing : endVtx->particles_out()) prdVtx->add_particle_out(outgoing);
        }
    }
    for (auto& particle : part_toremove) event.remove_particle(particle);
    for (auto& vtx : event.vertices()) if (vtx->particles_out().empty() && vtx->particles_in().empty()) vert_toremove.push_back(vtx);
    for (auto& vtx : vert_toremove) event.remove_vertex(vtx);
}
// Assuming vector starage
template <typename Select>
void remove_particles3(GenEvent& event, Select select) {
    auto& particles = event.particles();
    std::vector<GenParticlePtr> part_toremove;
    part_toremove.reserve(particles.size());
    std::vector<GenVertexPtr> vert_toremove;
    vert_toremove.reserve(event.vertices_size());
    for (auto& particle : particles) {
        if (select(particle)) continue;
        part_toremove.push_back(particle);
        auto endVtx = particle->end_vertex();
        auto prdVtx = particle->production_vertex();
        if (endVtx) endVtx->remove_particle_in(particle);
        if (prdVtx) prdVtx->remove_particle_out(particle);
        if (endVtx && prdVtx && prdVtx != endVtx ) {
          for (auto& outgoing : endVtx->particles_out()) prdVtx->add_particle_out(outgoing);
          for (auto& incoming : endVtx->particles_in()) prdVtx->add_particle_in(incoming);
        }
    }
    std::sort(part_toremove.begin(),part_toremove.end(), [](const auto& a, consta auto& b) {return a->id()<b->id();});
    for (auto& particle : part_toremove) event.remove_particle(particle);
    for (auto& vtx : event.vertices()) if (vtx->particles_out().empty() && vtx->particles_in().empty()) vert_toremove.push_back(vtx);
    std::sort(vert_toremove.begin(),vert_toremove.end(), [](const auto& a, consta auto& b) {return a->id()>b->id();});
    for (auto& vtx : vert_toremove) event.remove_vertex(vtx);
}

// Assuming list starage
template <typename Select>
void remove_particles4(GenEvent& event, Select select) {
    auto& particles = event.particles();
    std::vector<GenParticlePtr> part_toremove;
    part_toremove.reserve(particles.size());
    std::vector<GenVertexPtr> vert_toremove;
    vert_toremove.reserve(event.vertices_size());
    for (auto& particle : particles) {
        if (select(particle)) continue;
        part_toremove.push_back(particle);
        auto endVtx = particle->end_vertex();
        auto prdVtx = particle->production_vertex();
        if (endVtx) endVtx->remove_particle_in(particle);
        if (prdVtx) prdVtx->remove_particle_out(particle);
        if (endVtx && prdVtx && prdVtx != endVtx ) {
          for (auto& outgoing : endVtx->particles_out()) prdVtx->add_particle_out(outgoing);
          for (auto& incoming : endVtx->particles_in()) prdVtx->add_particle_in(incoming);
        }
    }
    for (auto& particle : part_toremove) event.remove_particle(particle);
    for (auto& vtx : event.vertices()) if (vtx->particles_out().empty() && vtx->particles_in().empty()) vert_toremove.push_back(vtx);
    for (auto& vtx : vert_toremove) event.remove_vertex(vtx);
}


}
#endif

#ifndef PRUNE_H
#define PRUNE_H
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenParticle.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/ReaderAscii.h>
#include <HepMC3/WriterAscii.h>
#include <list>

#define LOG(ENABLE) \
  if (ENABLE)       \
    std::clog

template <typename Select>
void prune(HepMC3::GenEvent& event,
           Select select,
           int verb = 0)
{
    auto particles = event.particles();
    auto vertices = event.vertices();
    std::list<HepMC3::GenParticlePtr> toRemove;

    LOG(verb > 1) << "HepMC events has " << particles.size()
                  << " particles and " << vertices.size()
                  << " vertices" << std::endl;

    size_t nSelect = 0;

    for (auto particle : particles) {
        if (select(particle)) {
            nSelect++;
            continue;
        }

        // Remove particle from the event
        toRemove.push_back(particle);
        LOG(verb > 2) << "Remove "
                      << std::setw(3) << particle->id() << " " << std::flush;

        auto endVtx = particle->end_vertex();
        auto prdVtx = particle->production_vertex();
        if (endVtx) {
            // Disconnect this particle from its out going vertex
            endVtx->remove_particle_in(particle);
            LOG(verb > 3) << "end " << std::setw(3) << endVtx->id()
                          << " " << std::flush;

            if (prdVtx and prdVtx->id() != endVtx->id()) {
                LOG(verb > 3) << "prd " << std::setw(3) << prdVtx->id() << " "
                              << std::flush;

                auto outbound = endVtx->particles_out();
                LOG(verb > 3) << std::setw(3) << outbound.size() << " out ";

                for (auto outgoing : outbound) {
                    // This should also detach the particle from its old
                    // end-vertex.
                    if (outgoing) {
                        auto ee = outgoing->end_vertex();
                        if (not ee or ee->id() != prdVtx->id())
                            prdVtx->add_particle_out(outgoing);
                        LOG(verb > 4) << std::setw(3) << outgoing->id() << " ";
                    }
                }

                auto inbound = endVtx->particles_in();
                LOG(verb > 3) << std::setw(3) << inbound.size() << " in ";

                for (auto incoming : inbound) {
                    if (incoming) {
                        auto pp = incoming->production_vertex();
                        if (not pp or pp->id() != prdVtx->id())
                            prdVtx->add_particle_in(incoming);

                        LOG(verb > 4) << std::setw(3) << incoming->id() << " ";
                    }
                }
            }
        }
        if (prdVtx) {
            prdVtx->remove_particle_out(particle);
        }
        LOG(verb > 2) << std::endl;
    }

    size_t nOrig = particles.size();

    LOG(verb > 1) << "Selected " << nSelect << " particles\n"
                  << "Removing " << toRemove.size() << " particles" << std::endl;

    // This is unfortunately highly in efficient.  This does a lot of
    // deallocation of std::vector objects which makes it really slow.
    for (auto particle : toRemove)
        event.remove_particle(particle);

    std::list<HepMC3::GenVertexPtr> remVtx;
    for (auto vtx : event.vertices()) {
        if (vtx and
                vtx->id() < 0 and
                (vtx->particles_out().empty() and
                 vtx->particles_in().empty()))
            remVtx.push_back(vtx);
    }

    LOG(verb > 1) << "Removing " << remVtx.size() << " vertexes"
                  << std::endl;
    // This is unfortunately highly in efficient.  This does a lot of
    // deallocation of std::vector objects which makes it really slow.
#ifndef PRUNE_BUG
    for (auto vtx : remVtx)
        event.remove_vertex(vtx);
#endif

    LOG(verb > 0) << "HepMC event originally had "
                  << nOrig << " particles, has been pruned to "
                  << event.particles().size()
                  << " particles and " << event.vertices().size()
                  << " vertices" << std::endl;
}

#endif

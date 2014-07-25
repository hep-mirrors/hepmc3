// -*- C++ -*-
#include "Rivet/Projections/PrimaryHadrons.hh"

namespace Rivet {


  void PrimaryHadrons::project(const Event& e) {
    _theParticles.clear();

    const Particles& unstables = applyProjection<FinalState>(e, "UFS").particles();
    foreach (const Particle& p, unstables) {
      // Exclude taus etc.
      if (!PID::isHadron(p)) continue;
      // A spontaneously appearing hadron: this is weird, but I guess is allowed... and is primary
      if (!p.genParticle() || !p.genParticle()->production_vertex()) {
        MSG_DEBUG("Hadron " << p.pdgId() << " with no GenParticle or parent found: treating as primary");
        _theParticles.push_back(p);
        continue;
      }
      // There are ancestors -- check them for status=2 hadronic content
      const vector<GenParticlePtr> ancestors = particles_in(p.genParticle(), HepMC3::ancestors);
      bool has_hadron_parent = false;
      foreach (const GenParticlePtr pa, ancestors) {
        if (pa->status() != 2) continue;
        /// @todo Are hadrons from tau decays "primary hadrons"? I guess not
        if (PID::isHadron(pa->pdg_id()) || abs(pa->pdg_id()) == PID::TAU) {
          has_hadron_parent = true;
          break;
        }
      }
      // If the particle seems to be a primary hadron, add it to the list
      if (!has_hadron_parent) _theParticles.push_back(p);
    }

    MSG_DEBUG("Number of primary hadrons = " << _theParticles.size());
  }


}

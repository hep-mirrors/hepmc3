// -*- C++ -*-
#include "Rivet/Projections/HeavyHadrons.hh"

namespace Rivet {


  void HeavyHadrons::project(const Event& e) {
    _theParticles.clear();
    _theBs.clear();
    _theCs.clear();

    const Particles& unstables = applyProjection<FinalState>(e, "UFS").particles();
    foreach (const Particle& p, unstables) {
      // Exclude non-b/c-hadrons
      if (!PID::isHadron(p)) continue;
      if (!PID::hasCharm(p) && !PID::hasBottom(p)) continue;
      MSG_DEBUG("Found a heavy (b or c) unstable hadron: " << p.pdgId());

      // An unbound, or undecayed status 2 hadron: this is weird, but I guess is allowed...
      if (!p.genParticle() || !p.genParticle()->end_vertex()) {
        MSG_DEBUG("Heavy hadron " << p.pdgId() << " with no GenParticle or decay found");
        _theParticles.push_back(p);
        if (PID::hasBottom(p)) _theBs.push_back(p); else _theCs.push_back(p);
        continue;
      }
      // There are descendants -- check them for b or c content
      /// @todo What about charm hadrons coming from bottom hadron decays?
      const vector<GenParticlePtr> children = particles_out(p.genParticle(), HepMC3::children);
      if (PID::hasBottom(p)) {
        bool has_b_child = false;
        foreach (const GenParticlePtr p2, children) {
          if (PID::hasBottom(p2->pdg_id())) {
            has_b_child = true;
            break;
          }
        }
        if (!has_b_child) {
          _theParticles.push_back(p);
          _theBs.push_back(p);
        }
      } else if (PID::hasCharm(p)) {
        bool has_c_child = false;
        foreach (const GenParticlePtr p2, children) {
          if (PID::hasCharm(p2->pdg_id())) {
            has_c_child = true;
            break;
          }
        }
        if (!has_c_child) {
          _theParticles.push_back(p);
          _theCs.push_back(p);
        }
      }
    }

    MSG_DEBUG("Num b hadrons = " << _theBs.size() <<
              ", num c hadrons = " << _theCs.size() <<
              ", total = " << _theParticles.size());
  }


}

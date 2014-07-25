// -*- C++ -*-
#include "Rivet/Projections/NonHadronicFinalState.hh"

namespace Rivet {


  int NonHadronicFinalState::compare(const Projection& p) const {
    return FinalState::compare(p);
  }


  bool nonHadronFilter(const Particle& p) {
    return PID::isHadron(p.pdgId());
  }

  void NonHadronicFinalState::project(const Event& e) {
    const FinalState& fs = applyProjection<FinalState>(e, "FS");
    _theParticles.clear();
    std::remove_copy_if(fs.particles().begin(), fs.particles().end(),
                        std::back_inserter(_theParticles), nonHadronFilter);
    MSG_DEBUG("Number of non-hadronic final-state particles = "
             << _theParticles.size());
  }

}

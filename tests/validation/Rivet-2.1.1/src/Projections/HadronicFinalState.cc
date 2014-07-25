// -*- C++ -*-
#include "Rivet/Projections/HadronicFinalState.hh"

namespace Rivet {


  int HadronicFinalState::compare(const Projection& p) const {
    return FinalState::compare(p);
  }


  bool hadronFilter(const Particle& p) {
    return ! PID::isHadron(p.pdgId());
  }


  void HadronicFinalState::project(const Event& e) {
    const FinalState& fs = applyProjection<FinalState>(e, "FS");
    _theParticles.clear();
    std::remove_copy_if(fs.particles().begin(), fs.particles().end(),
                        std::back_inserter(_theParticles), hadronFilter);
    MSG_DEBUG("Number of hadronic final-state particles = "
             << _theParticles.size());
  }

}

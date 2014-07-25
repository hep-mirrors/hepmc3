// -*- C++ -*-
#include "Rivet/Projections/VisibleFinalState.hh"

namespace Rivet {


  int VisibleFinalState::compare(const Projection& p) const {
    return mkNamedPCmp(p, "FS");
  }


  // Since we remove invisibles from the FinalState in project(),
  // we need a filter where invisible --> true
  bool isInvisibleFilter(const Particle& p) {
    // Charged particles are visible
    if ( PID::threeCharge( p.pdgId() ) != 0 )
      return false;

    // Neutral hadrons are visible
    if ( PID::isHadron( p.pdgId() ) )
      return false;

    // Photons are visible
    if ( p.pdgId() == PID::PHOTON )
      return false;

    // Gluons are visible (for parton level analyses)
    if ( p.pdgId() == PID::GLUON )
      return false;

    // Everything else is invisible
    return true;
  }


  void VisibleFinalState::project(const Event& e) {
    const FinalState& fs = applyProjection<FinalState>(e, "FS");
    _theParticles.clear();
    std::remove_copy_if(fs.particles().begin(), fs.particles().end(),
                        std::back_inserter(_theParticles), isInvisibleFilter);
    MSG_DEBUG("Number of visible final-state particles = "
              << _theParticles.size());
  }


}

// -*- C++ -*-
#include "Rivet/Projections/MissingMomentum.hh"

namespace Rivet {


  int MissingMomentum::compare(const Projection& p) const {
    return mkNamedPCmp(p, "VisibleFS");
  }


  void MissingMomentum::clear() {
    _momentum = FourMomentum();
    _set = 0.0;
    _vet = Vector3();
  }


  void MissingMomentum::project(const Event& e) {
    clear();

    // Project into final state
    const FinalState& vfs = applyProjection<FinalState>(e, "VisibleFS");
    foreach (const Particle& p, vfs.particles()) {
      const FourMomentum& mom = p.momentum();
      _momentum += mom;
      _set += mom.Et();
      _vet += mom.Et() * mom.vector3().setZ(0.0).unit();
    }
  }


}

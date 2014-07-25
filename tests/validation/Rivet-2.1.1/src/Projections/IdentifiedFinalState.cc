// -*- C++ -*-
#include "Rivet/Projections/IdentifiedFinalState.hh"

namespace Rivet {


  IdentifiedFinalState::IdentifiedFinalState(const FinalState& fsp) {
    setName("IdentifiedFinalState");
    addProjection(fsp, "FS");
  }


  IdentifiedFinalState::IdentifiedFinalState(double etamin, double etamax, double ptMin)
    : FinalState(etamin, etamax, ptMin)
  {
    setName("IdentifiedFinalState");
    addProjection(FinalState(etamin, etamax, ptMin), "FS");
  }


  IdentifiedFinalState::IdentifiedFinalState(const vector<pair<double, double> >& etaRanges,
                                             double ptMin)
    : FinalState(etaRanges, ptMin)
  {
    setName("IdentifiedFinalState");
    addProjection(FinalState(etaRanges, ptMin), "FS");
  }


  int IdentifiedFinalState::compare(const Projection& p) const {
    const PCmp fscmp = mkNamedPCmp(p, "FS");
    if (fscmp != EQUIVALENT) return fscmp;

    const IdentifiedFinalState& other = dynamic_cast<const IdentifiedFinalState&>(p);
    int pidssize = cmp(_pids.size(), other._pids.size());
    if (pidssize != EQUIVALENT) return pidssize;
    return cmp(_pids, other._pids);
  }


  void IdentifiedFinalState::project(const Event& e) {
    const FinalState& fs = applyProjection<FinalState>(e, "FS");
    _theParticles.clear();
    _theParticles.reserve(fs.particles().size());
    foreach (const Particle& p, fs.particles()) {
      if (acceptedIds().find(p.pdgId()) != acceptedIds().end()) {
        _theParticles.push_back(p);
      }
    }
  }


}

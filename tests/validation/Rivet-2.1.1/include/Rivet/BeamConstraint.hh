// -*- C++ -*-
#ifndef RIVET_BeamConstraint_HH
#define RIVET_BeamConstraint_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/ParticleName.hh"
#include "Rivet/Particle.hh"
#include <iostream>


namespace Rivet {

  /// Find whether ParticleName @a p is compatible with the
  /// template ParticleName @a allowed. Effectively this is
  /// asking whether @a p is a subset of @a allowed.
  inline bool compatible(PdgId p, PdgId allowed) {
    return (allowed == PID::ANY || p == allowed);
  }

  /// Find whether PdgIdPair @a pair is compatible with the template
  /// PdgIdPair @a allowedpair. This assesses whether either of the
  /// two possible pairings of @a pair's constituents is compatible.
  inline bool compatible(const PdgIdPair& pair, const PdgIdPair& allowedpair) {
    bool oneToOne = compatible(pair.first, allowedpair.first);
    bool twoToTwo = compatible(pair.second, allowedpair.second);
    bool oneToTwo = compatible(pair.first, allowedpair.second);
    bool twoToOne = compatible(pair.second, allowedpair.first);
    return (oneToOne && twoToTwo) || (oneToTwo && twoToOne);
  }


  /// Check particle compatibility of Particle pairs
  inline bool compatible(const ParticlePair& ppair,
                         const PdgIdPair& allowedpair) {
    return compatible(PID::make_pdgid_pair(ppair.first.pdgId(),
                                           ppair.second.pdgId()), allowedpair);
  }
  /// Check particle compatibility of Particle pairs (for symmetric completeness)
  inline bool compatible(const PdgIdPair& allowedpair,
                         const ParticlePair& ppair) {
    return compatible(ppair, allowedpair);
  }


  /// Find whether a PdgIdPair @a pair is compatible with at least one template
  /// beam pair in a set @a allowedpairs.
  inline bool compatible(const PdgIdPair& pair, const set<PdgIdPair>& allowedpairs) {
    for (set<PdgIdPair>::const_iterator bp = allowedpairs.begin(); bp != allowedpairs.end(); ++bp) {
      if (compatible(pair, *bp)) return true;
    }
    return false;
  }

  /// Return the intersection of two sets of {PdgIdPair}s.
  inline set<PdgIdPair> intersection(const set<PdgIdPair>& a, const set<PdgIdPair>& b) {
    set<PdgIdPair> ret;
    for (set<PdgIdPair>::const_iterator bp = a.begin(); bp != a.end(); ++bp) {
      if (compatible(*bp, b)) ret.insert(*bp);
    }
    return ret;
  }


}

#endif

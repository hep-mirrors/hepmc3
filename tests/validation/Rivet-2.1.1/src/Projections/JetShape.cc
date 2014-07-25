// -*- C++ -*-
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/JetShape.hh"

namespace Rivet {


  // Constructor.
  JetShape::JetShape(const JetAlg& jetalg,
                     double rmin, double rmax, size_t nbins,
                     double ptmin, double ptmax,
                     double absrapmin, double absrapmax,
                     RapScheme rapscheme)
    : _rapscheme(rapscheme)
  {
    setName("JetShape");
    _binedges = linspace(nbins, rmin, rmax);
    _ptcuts = make_pair(ptmin, ptmax);
    _rapcuts = make_pair(absrapmin, absrapmax);
    addProjection(jetalg, "Jets");
  }


  // Constructor.
  JetShape::JetShape(const JetAlg& jetalg,
                     vector<double> binedges,
                     double ptmin, double ptmax,
                     double absrapmin, double absrapmax,
                     RapScheme rapscheme)
    : _binedges(binedges), _rapscheme(rapscheme)
  {
    setName("JetShape");
    _ptcuts = make_pair(ptmin, ptmax);
    _rapcuts = make_pair(absrapmin, absrapmax);
    addProjection(jetalg, "Jets");
  }


  int JetShape::compare(const Projection& p) const {
    const int jcmp = mkNamedPCmp(p, "Jets");
    if (jcmp != EQUIVALENT) return jcmp;
    const JetShape& other = pcast<JetShape>(p);
    const int ptcmp = cmp(ptMin(), other.ptMin()) || cmp(ptMax(), other.ptMax());
    if (ptcmp != EQUIVALENT) return ptcmp;
    const int rapcmp = cmp(_rapcuts.first, other._rapcuts.first) || cmp(_rapcuts.second, other._rapcuts.second);
    if (rapcmp != EQUIVALENT) return rapcmp;
    int bincmp = cmp(numBins(), other.numBins());
    if (bincmp != EQUIVALENT) return bincmp;
    for (size_t i = 0; i < _binedges.size(); ++i) {
      bincmp = cmp(_binedges[i], other._binedges[i]);
      if (bincmp != EQUIVALENT) return bincmp;
    }
    return EQUIVALENT;
  }


  void JetShape::clear() {
    _diffjetshapes.clear();
  }


  void JetShape::calc(const Jets& jets) {
    clear();

    foreach (const Jet& j, jets) {
      // Apply jet cuts
      const FourMomentum& pj = j.momentum();
      if (!inRange(pj.pT(), _ptcuts)) continue;
      /// @todo Use Cut for better eta/y selection
      if (_rapscheme == PSEUDORAPIDITY && !inRange(fabs(pj.eta()), _rapcuts)) continue;
      if (_rapscheme == RAPIDITY && !inRange(fabs(pj.rapidity()), _rapcuts)) continue;

      // Fill bins
      vector<double> bins(numBins(), 0.0);
      foreach (const Particle& p, j.particles()) {
        const double dR = deltaR(pj, p.momentum(), _rapscheme);
        const int dRindex = binIndex(dR, _binedges);
        if (dRindex == -1) continue; //< Out of histo range
        bins[dRindex] += p.pT();
      }

      // Add bin vector for this jet to the diffjetshapes container
      _diffjetshapes += bins;
    }

    // Normalize to total pT
    foreach (vector<double>& binsref, _diffjetshapes) {
      double integral = 0.0;
      for (size_t i = 0; i < numBins(); ++i) {
        integral += binsref[i];
      }
      if (integral > 0) {
        for (size_t i = 0; i < numBins(); ++i) {
          binsref[i] /= integral;
        }
      } else {
        // It's just-about conceivable that a jet would have no particles in the given Delta(r) range...
        MSG_DEBUG("No pT contributions in jet Delta(r) range: weird!");
      }
    }

  }


  void JetShape::project(const Event& e) {
    const Jets jets = applyProjection<JetAlg>(e, "Jets").jets(_ptcuts.first, _ptcuts.second,
                                                              -_rapcuts.second, _rapcuts.second, _rapscheme);
    calc(jets);
  }


}

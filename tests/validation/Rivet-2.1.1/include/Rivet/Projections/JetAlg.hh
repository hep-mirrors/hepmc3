// -*- C++ -*-
#ifndef RIVET_JetAlg_HH
#define RIVET_JetAlg_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Jet.hh"

namespace Rivet {


  /// Abstract base class for projections which can return a set of {@link Jet}s.
  class JetAlg : public Projection {
  public:

    /// Constructor
    JetAlg(const FinalState& fs);

    JetAlg() {};

    /// Clone on the heap.
    virtual const Projection* clone() const = 0;

    /// Destructor
    virtual ~JetAlg() { }

    /// @brief Include invisible particles in jet construction.
    /// The default behaviour is that jets are only constructed from visible
    /// (i.e. charged under an SM gauge group) particles. Some jet studies,
    /// including those from ATLAS, use a definition in which neutrinos from hadron
    /// decays are included (via MC correction) in the experimental jet definition.
    /// Setting this flag to true avoids the automatic restriction to a VisibleFinalState.
    void useInvisibles(bool useinvis=true) {
      _useInvisibles = useinvis;
    }

    /// Get jets in no guaranteed order, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    virtual Jets jets(double ptmin=0.0, double ptmax=MAXDOUBLE,
                      double rapmin=-MAXDOUBLE, double rapmax=MAXDOUBLE,
                      RapScheme rapscheme=PSEUDORAPIDITY) const {
      const Jets rawjets = _jets(ptmin);
      Jets rtn;
      MSG_DEBUG("Raw jet size (with pTmin cut = " << ptmin/GeV << " GeV) = " << rawjets.size());
      foreach (const Jet& j, rawjets) {
        const FourMomentum pj = j.momentum();
        if (!inRange(pj.pT(), ptmin, ptmax)) continue;
        if (rapscheme == PSEUDORAPIDITY && !inRange(pj.eta(), rapmin, rapmax)) continue;
        if (rapscheme == RAPIDITY && !inRange(pj.rapidity(), rapmin, rapmax)) continue;
        rtn += j;
      }
      return rtn;
    }

    /// Get the jets, ordered by supplied sorting function object, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    template <typename F>
    Jets jets(F sorter, double ptmin, double ptmax,
              double rapmin, double rapmax,
              RapScheme rapscheme) const {
      Jets js = jets(ptmin, ptmax, rapmin, rapmax, rapscheme);
      if (sorter != 0) {
        std::sort(js.begin(), js.end(), sorter);
      }
      return js;
    }

    /// Get the jets, ordered by \f$ p_T \f$, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    Jets jetsByPt(double ptmin=0.0, double ptmax=MAXDOUBLE,
                  double rapmin=-MAXDOUBLE, double rapmax=MAXDOUBLE,
                  RapScheme rapscheme=PSEUDORAPIDITY) const {
      return jets(cmpMomByPt, ptmin, ptmax, rapmin, rapmax, rapscheme);
    }

    /// Get the jets, ordered by \f$ |p| \f$, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    Jets jetsByP(double ptmin=0.0, double ptmax=MAXDOUBLE,
                 double rapmin=-MAXDOUBLE, double rapmax=MAXDOUBLE,
                 RapScheme rapscheme=PSEUDORAPIDITY) const {
      return jets(cmpMomByP, ptmin, ptmax, rapmin, rapmax, rapscheme);
    }

    /// Get the jets, ordered by \f$ E \f$, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    Jets jetsByE(double ptmin=0.0, double ptmax=MAXDOUBLE,
                 double rapmin=-MAXDOUBLE, double rapmax=MAXDOUBLE,
                 RapScheme rapscheme=PSEUDORAPIDITY) const {
      return jets(cmpMomByE, ptmin, ptmax, rapmin, rapmax, rapscheme);
    }

    /// Get the jets, ordered by \f$ E_T \f$, with optional cuts on \f$ p_\perp \f$ and rapidity.
    /// @todo Introduce MomentumFilter objects for pT, ET, eta, y, etc. filtering, to avoid double-arg ambiguities
    Jets jetsByEt(double ptmin=0.0, double ptmax=MAXDOUBLE,
                  double rapmin=-MAXDOUBLE, double rapmax=MAXDOUBLE,
                  RapScheme rapscheme=PSEUDORAPIDITY) const {
      return jets(cmpMomByEt, ptmin, ptmax, rapmin, rapmax, rapscheme);
    }


  protected:

    /// @brief Internal pure virtual method for getting jets in no guaranteed order.
    /// An optional cut on min \f$ p_\perp \f$ is applied in this function, since that is
    /// directly supported by FastJet and it seems a shame to not make use of that. But
    /// all other jet cuts are applied at the @c ::jets() function level.
    virtual Jets _jets(double ptmin) const = 0;


  public:

    /// Number of jets.
    virtual size_t size() const = 0;
    /// Determine if the jet collection is empty.
    bool empty() const { return size() != 0; }

    /// Clear the projection.
    virtual void reset() = 0;

    typedef Jet entity_type;
    typedef Jets collection_type;

    /// Template-usable interface common to FinalState.
    collection_type entities() const { return jets(); }

    /// Do the calculation locally (no caching).
    virtual void calc(const Particles& ps) = 0;


  protected:

    /// Perform the projection on the Event.
    virtual void project(const Event& e) = 0;

    /// Compare projections.
    virtual int compare(const Projection& p) const = 0;


  protected:

    /// Flag to determine whether or not the VFS wrapper is to be used.
    bool _useInvisibles;

  };


}

#endif

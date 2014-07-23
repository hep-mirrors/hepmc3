// -*- C++ -*-
#ifndef RIVET_DressedLeptons_HH
#define RIVET_DressedLeptons_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"

namespace Rivet {


  /// A charged lepton meta-particle created by clustering photons close to the bare lepton
  class ClusteredLepton : public Particle {
  public:

    ClusteredLepton(Particle lepton) :
      Particle(lepton.pdgId(), lepton.momentum()),
      _constituentLepton(lepton) {}

    void addPhoton(const Particle& p, bool cluster) {
      _constituentPhotons.push_back(p);
      if (cluster) setMomentum(momentum() + p.momentum());
    }

    const Particle& constituentLepton() const { return _constituentLepton; }
    const Particles& constituentPhotons() const { return _constituentPhotons; }

  private:

    Particles _constituentPhotons;
    Particle _constituentLepton;
  };


  /// @brief Cluster photons from a given FS to all charged particles (typically leptons)
  ///
  /// This stores the original charged particles and photons as particles()
  /// while the newly created clustered lepton objects are accessible as
  /// clusteredLeptons().
  class DressedLeptons : public FinalState {
  public:

    DressedLeptons(const FinalState& photons, const FinalState& signal,
                   double dRmax, bool cluster,
                   const vector<pair<double, double> >& etaRanges,
                   double pTmin, bool useDecayPhotons=false);

    virtual const Projection* clone() const {
      return new DressedLeptons(*this);
    }

    const vector<ClusteredLepton>& clusteredLeptons() const { return _clusteredLeptons; }


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;


  private:

    /// Maximum cone radius to find photons in
    double _dRmax;
    /// Whether to actually add the photon momenta to clusteredLeptons
    bool _cluster;
    /// Whether to include photons from hadron (particularly pi0) decays
    bool _fromDecay;

    /// Container which stores the clustered lepton objects
    vector<ClusteredLepton> _clusteredLeptons;

  };




}


#endif

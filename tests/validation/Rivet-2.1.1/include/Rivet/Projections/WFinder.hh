// -*- C++ -*-
#ifndef RIVET_WFinder_HH
#define RIVET_WFinder_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"

namespace Rivet {


  /// @brief Convenience finder of leptonically decaying Ws
  ///
  /// Chain together different projections as convenience for finding W's
  /// from two leptons in the final state, including photon clustering.
  class WFinder : public FinalState {
  public:

    enum ClusterPhotons { NOCLUSTER=0, CLUSTERNODECAY=1, CLUSTERALL };
    enum PhotonTracking { NOTRACK=0, TRACK=1 };
    enum MassWindow { MASS=0, TRANSMASS=1 };

    /// @name Constructors
    //@{

    /// Constructor taking single eta/pT bounds
    ///
    /// @param etaMin,etaMax,pTmin charged lepton cuts
    /// @param pid type of the charged lepton
    /// @param minmass,maxmass (transverse) mass window
    /// @param missingET minimal amount of missing ET (neutrinos) required
    /// @param dRmax maximum dR of photons around charged lepton to take into account
    ///  for W reconstruction (only relevant if one of the following are true)
    /// @param clusterPhotons whether such photons are supposed to be
    ///  clustered to the lepton object and thus W mom
    /// @param trackPhotons whether such photons should be added to _theParticles
    /// @param masstype whether mass window should be applied using m or mT
    WFinder(double etaMin, double etaMax,
            double pTmin,
            PdgId pid,
            double minmass, double maxmass,
            double missingET,
            double dRmax=0.1, ClusterPhotons clusterPhotons=CLUSTERNODECAY, PhotonTracking trackPhotons=NOTRACK,
            MassWindow masstype=MASS, double masstarget=80.4*GeV) {
      vector<pair<double, double> > etaRanges;
      etaRanges += std::make_pair(etaMin, etaMax);
      _init(FinalState(), etaRanges, pTmin, pid, minmass, maxmass, missingET,
            dRmax, clusterPhotons, trackPhotons, masstype, masstarget);
    }

    /// Constructor taking single eta/pT bounds and an input FS
    ///
    /// @param inputfs Input final state
    /// @param etaMin,etaMax,pTmin charged lepton cuts
    /// @param pid type of the charged lepton
    /// @param minmass,maxmass (transverse) mass window
    /// @param missingET minimal amount of missing ET (neutrinos) required
    /// @param dRmax maximum dR of photons around charged lepton to take into account
    ///  for W reconstruction (only relevant if one of the following are true)
    /// @param clusterPhotons whether such photons are supposed to be
    ///  clustered to the lepton object and thus W mom
    /// @param trackPhotons whether such photons should be added to _theParticles
    /// @param masstype whether mass window should be applied using m or mT
    WFinder(const FinalState& inputfs,
            double etaMin, double etaMax,
            double pTmin,
            PdgId pid,
            double minmass, double maxmass,
            double missingET,
            double dRmax=0.1, ClusterPhotons clusterPhotons=CLUSTERNODECAY, PhotonTracking trackPhotons=NOTRACK,
            MassWindow masstype=MASS, double masstarget=80.4*GeV) {
      vector<pair<double, double> > etaRanges;
      etaRanges += std::make_pair(etaMin, etaMax);
      _init(inputfs, etaRanges, pTmin, pid, minmass, maxmass, missingET,
            dRmax, clusterPhotons, trackPhotons, masstype, masstarget);
    }


    /// Constructor taking multiple eta/pT bounds
    ///
    /// @param etaRanges,pTmin charged lepton cuts
    /// @param pid type of the charged lepton
    /// @param minmass,maxmass (transverse) mass window
    /// @param missingET minimal amount of missing ET (neutrinos) required
    /// @param dRmax maximum dR of photons around charged lepton to take into account
    ///  for W reconstruction (only relevant if one of the following are true)
    /// @param clusterPhotons whether such photons are supposed to be
    ///  clustered to the lepton object and thus W mom
    /// @param trackPhotons whether such photons should be added to _theParticles
    /// @param masstype whether mass window should be applied using mT
    WFinder(const vector<pair<double, double> >& etaRanges,
            double pTmin,
            PdgId pid,
            double minmass, double maxmass,
            double missingET,
            double dRmax=0.1, ClusterPhotons clusterPhotons=CLUSTERNODECAY, PhotonTracking trackPhotons=NOTRACK,
            MassWindow masstype=MASS, double masstarget=80.4*GeV) {
      _init(FinalState(), etaRanges, pTmin, pid, minmass, maxmass, missingET,
            dRmax, clusterPhotons, trackPhotons, masstype, masstarget);
    }


    /// Constructor taking multiple eta/pT bounds and an input FS
    ///
    /// @param inputfs Input final state
    /// @param etaRanges,pTmin charged lepton cuts
    /// @param pid type of the charged lepton
    /// @param minmass,maxmass (transverse) mass window
    /// @param missingET minimal amount of missing ET (neutrinos) required
    /// @param dRmax maximum dR of photons around charged lepton to take into account
    ///  for W reconstruction (only relevant if one of the following are true)
    /// @param clusterPhotons whether such photons are supposed to be
    ///  clustered to the lepton object and thus W mom
    /// @param trackPhotons whether such photons should be added to _theParticles
    /// @param masstype whether mass window should be applied using mT
    WFinder(const FinalState& inputfs,
            const vector<pair<double, double> >& etaRanges,
            double pTmin,
            PdgId pid,
            double minmass, double maxmass,
            double missingET,
            double dRmax=0.1, ClusterPhotons clusterPhotons=CLUSTERNODECAY, PhotonTracking trackPhotons=NOTRACK,
            MassWindow masstype=MASS, double masstarget=80.4*GeV) {
      _init(inputfs, etaRanges, pTmin, pid, minmass, maxmass, missingET,
            dRmax, clusterPhotons, trackPhotons, masstype, masstarget);
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new WFinder(*this);
    }

    //@}


    /// Access to the found bosons
    ///
    /// @note Currently either 0 or 1 boson can be found.
    const Particles& bosons() const { return _bosons; }

    /// Access to the found boson (assuming it exists)
    const Particle& boson() const { return _bosons[0]; }


    /// Access to the W constituent clustered leptons
    ///
    /// @note Either size 0 if no boson was found or 1 if one boson was found
    const Particles& constituentLeptons() const { return _constituentLeptons; }

    /// Access to the W constituent clustered lepton (assuming it exists)
    const Particle& constituentLepton() const { return _constituentLeptons[0]; }


    /// Access to the W constituent neutrinos
    ///
    /// @note Either size 0 if no boson was found or 1 if one boson was found
    const Particles& constituentNeutrinos() const { return _constituentNeutrinos; }

    /// Access to the W constituent neutrinos
    const Particle& constituentNeutrino() const { return _constituentNeutrinos[0]; }


    /// Access to the particles other than the W leptons and clustered photons
    ///
    /// Useful for e.g. input to a jet finder
    const FinalState& remainingFinalState() const;

    /// @brief Calculate the transverse mass of the W, from the charged lepton and neutrino
    ///
    /// Defined as sqrt(2 pT_l pT_nu (1.0 - cos(dphi_lnu))). Return -1 if no boson found.
    double mT() const {
      if (bosons().empty()) return -1;
      const FourMomentum& l = constituentLeptons()[0].momentum();
      const FourMomentum& nu = constituentNeutrinos()[0].momentum();
      return sqrt( 2 * l.pT() * nu.pT() * (1 - cos(deltaPhi(l, nu))) );
    }


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;


  public:

    /// Clear the projection
    void clear() {
      _theParticles.clear();
      _bosons.clear();
      _constituentLeptons.clear();
      _constituentNeutrinos.clear();
    }


  private:

    /// Common implementation of constructor operation, taking FS params.
    void _init(const FinalState& inputfs,
               const vector<pair<double, double> >& etaRanges,
               double pTmin,  PdgId pid,
               double minmass, double maxmass,
               double missingET,
               double dRmax, ClusterPhotons clusterPhotons=CLUSTERNODECAY, PhotonTracking trackPhotons=NOTRACK,
               MassWindow masstype=MASS, double masstarget=80.4*GeV);


  private:

    /// Transverse mass cuts
    double _minmass, _maxmass, _masstarget;
    bool _useTransverseMass;

    /// Missing ET cut
    double _etMiss;

    /// Switch for tracking of photons (whether to add them to _theParticles)
    /// This is relevant when the ZFinder::_theParticles are to be excluded
    /// from e.g. the input to a jet finder, to specify whether the clustered
    /// photons are to be excluded as well.
    /// (Yes, some experiments make a difference between clusterPhotons and
    /// trackPhotons!)
    PhotonTracking _trackPhotons;

    /// Lepton flavour
    PdgId _pid;

    /// Neutrino flavour
    PdgId _nu_pid;

    /// list of found bosons (currently either 0 or 1)
    Particles _bosons;

    /// Constituent leptons (currently either 0 or 1)
    Particles _constituentLeptons;

    /// Constituent neutrinos (currently either 0 or 1)
    Particles _constituentNeutrinos;

  };


}


#endif

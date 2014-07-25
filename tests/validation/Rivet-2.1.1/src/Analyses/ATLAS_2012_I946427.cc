// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @author Peter Richardson
  class ATLAS_2012_I946427 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I946427()
      : Analysis("ATLAS_2012_I946427")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // photons
      IdentifiedFinalState photonfs(-1.81, 1.81, 25.0*GeV);
      photonfs.acceptId(PID::PHOTON);
      addProjection(photonfs, "Photon");

      //
      FinalState fs;
      addProjection(fs, "FS");

      // Used for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");

      // Book histograms
      _count_SR = bookHisto1D("count_SR", 1, 0., 1.);

      _hist_ET_photon = bookHisto1D("hist_ET_photon", 48 , 20., 500.);
      _hist_met       = bookHisto1D("hist_met"      , 100,  0., 500.);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

      // require at least 2 photons in final state
      Particles photons =
        applyProjection<IdentifiedFinalState>(event, "Photon").particlesByPt();
      if (photons.size() < 2) {
        vetoEvent;
      }

      // Loop over photons and fill vector of isolated ones
      Particles fs = applyProjection<FinalState>(event, "FS").particles();
      Particles isolated_photons;
      foreach (const Particle& photon, photons) {
        // remove photons in crack
        double eta_P = photon.eta();
        if (fabs(eta_P)>=1.37 && fabs(eta_P)<1.52) continue;

        double phi_P = photon.momentum().phi();

        FourMomentum mom_in_EtCone = -photon.momentum();
        foreach (const Particle& p, fs) {
          // check if it's in the cone of .2
          if (deltaR(eta_P, phi_P, p.eta(),
                     p.momentum().phi()) >= 0.2) continue;
          mom_in_EtCone += p.momentum();
        }
        // apply isolation
        if(mom_in_EtCone.Et()>5.) continue;

        // add photon to list of isolated ones
        isolated_photons.push_back(photon);
      }

      // need two isolated photons
      if(isolated_photons.size() < 2 ) {
        vetoEvent;
      }

      // pTmiss
      Particles vfs_particles =
        applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();

      _hist_ET_photon->fill(isolated_photons[0].momentum().Et(),weight);
      _hist_met      ->fill(eTmiss                             ,weight);

      if(eTmiss>125.) _count_SR->fill(0.5,weight);
    }


    void finalize() {

      double norm = crossSection()/femtobarn*1.07/sumOfWeights();
      // these are number of events at 1.07fb^-1 per 10 GeV
      scale( _hist_ET_photon, 10. * norm );
      // these are number of events at 1.07fb^-1 per  5 GeV
      scale( _hist_met, 5. * norm );
      // these are number of events at 1.07fb^-1
      scale(_count_SR,norm);
    }

    //@}


  private:

    Histo1DPtr _count_SR;
    Histo1DPtr _hist_ET_photon;
    Histo1DPtr _hist_met;

  };


  // This global object acts as a hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I946427);

}

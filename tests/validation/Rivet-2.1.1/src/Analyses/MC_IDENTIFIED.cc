// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// Generic analysis looking at various distributions of final state particles
  class MC_IDENTIFIED : public Analysis {
  public:

    /// Constructor
    MC_IDENTIFIED()
      : Analysis("MC_IDENTIFIED")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Projections
      const FinalState cnfs(-5.0, 5.0, 500*MeV);
      addProjection(cnfs, "FS");
      addProjection(UnstableFinalState(-5.0, 5.0, 500*MeV), "UFS");

      // Histograms
      // @todo Choose E/pT ranged based on input energies... can't do anything about kin. cuts, though

      _histStablePIDs  = bookHisto1D("MultsStablePIDs", 3335, -0.5, 3334.5);
      _histDecayedPIDs = bookHisto1D("MultsDecayedPIDs", 3335, -0.5, 3334.5);
      _histAllPIDs     = bookHisto1D("MultsAllPIDs", 3335, -0.5, 3334.5);

      _histEtaPi       = bookHisto1D("EtaPi", 25, 0, 5);
      _histEtaK        = bookHisto1D("EtaK", 25, 0, 5);
      _histEtaLambda   = bookHisto1D("EtaLambda", 25, 0, 5);
    }



    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Unphysical (debug) plotting of all PIDs in the event, physical or otherwise
      foreach (const GenParticlePtr gp, particles(event.genEvent())) {
        _histAllPIDs->fill(abs(gp->pdg_id()), weight);
      }

      // Charged + neutral final state PIDs
      const FinalState& cnfs = applyProjection<FinalState>(event, "FS");
      foreach (const Particle& p, cnfs.particles()) {
        _histStablePIDs->fill(abs(p.pdgId()), weight);
      }

      // Unstable PIDs and identified particle eta spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");
      foreach (const Particle& p, ufs.particles()) {
        _histDecayedPIDs->fill(p.pdgId(), weight);
        const double eta_abs = fabs(p.eta());
        const PdgId pid = abs(p.pdgId()); //if (PID::isMeson(pid) && PID::hasStrange()) {
        if (pid == 211 || pid == 111) _histEtaPi->fill(eta_abs, weight);
        else if (pid == 321 || pid == 130 || pid == 310) _histEtaK->fill(eta_abs, weight);
        else if (pid == 3122) _histEtaLambda->fill(eta_abs, weight);
      }

    }



    /// Finalize
    void finalize() {
      scale(_histStablePIDs, 1/sumOfWeights());
      scale(_histDecayedPIDs, 1/sumOfWeights());
      scale(_histAllPIDs, 1/sumOfWeights());
      scale(_histEtaPi, 1/sumOfWeights());
      scale(_histEtaK, 1/sumOfWeights());
      scale(_histEtaLambda, 1/sumOfWeights());
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _histStablePIDs, _histDecayedPIDs, _histAllPIDs;
    Histo1DPtr _histEtaPi, _histEtaK, _histEtaLambda;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_IDENTIFIED);

}

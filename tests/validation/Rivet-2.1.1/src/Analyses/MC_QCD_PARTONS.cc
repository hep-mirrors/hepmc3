// -*- C++ -*-
#include "Rivet/Analyses/MC_JetSplittings.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// Generic analysis looking at kt splitting scales of partons
  class MC_QCD_PARTONS : public MC_JetSplittings {
  public:

    /// Constructor
    MC_QCD_PARTONS()
      : MC_JetSplittings("MC_QCD_PARTONS", 4, "Jets")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Projections
      IdentifiedFinalState partonfs;
      for (int i=1; i<6; ++i) partonfs.acceptIdPair(i);
      partonfs.acceptId(PID::GLUON);
      addProjection(FastJets(partonfs, FastJets::KT, 0.6), "Jets");

      MC_JetSplittings::init();
    }



    /// Perform the per-event analysis
    void analyze(const Event& event) {
      MC_JetSplittings::analyze(event);
    }



    /// Finalize
    void finalize() {
      MC_JetSplittings::finalize();
    }

    //@}


  private:

    /// @name Histograms
    //@{
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_QCD_PARTONS);

}

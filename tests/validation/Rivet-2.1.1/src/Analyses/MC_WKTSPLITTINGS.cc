// -*- C++ -*-
#include "Rivet/Analyses/MC_JetSplittings.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for kt splitting scales in W + jets events
  class MC_WKTSPLITTINGS : public MC_JetSplittings {
  public:

    /// Default constructor
    MC_WKTSPLITTINGS()
      : MC_JetSplittings("MC_WKTSPLITTINGS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      WFinder wfinder(fs, -3.5, 3.5, 25.0*GeV, PID::ELECTRON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wfinder, "WFinder");
      FastJets jetpro(wfinder.remainingFinalState(), FastJets::KT, 0.6);
      addProjection(jetpro, "Jets");

      MC_JetSplittings::init();
    }


    /// Do the analysis
    void analyze(const Event & e) {

      const WFinder& wfinder = applyProjection<WFinder>(e, "WFinder");
      if (wfinder.bosons().size() != 1) {
        vetoEvent;
      }

      MC_JetSplittings::analyze(e);
    }


    /// Finalize
    void finalize() {
      MC_JetSplittings::finalize();
    }

    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_WKTSPLITTINGS);

}

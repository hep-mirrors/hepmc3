// -*- C++ -*-
#include "Rivet/Analyses/MC_JetSplittings.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief MC validation analysis for W^+[enu]W^-[munu] + jets events
  class MC_WWKTSPLITTINGS : public MC_JetSplittings {
  public:

    /// Default constructor
    MC_WWKTSPLITTINGS()
      : MC_JetSplittings("MC_WWKTSPLITTINGS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      WFinder wenufinder(fs, -3.5, 3.5, 25.0*GeV, PID::ELECTRON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wenufinder, "WenuFinder");

      VetoedFinalState wmnuinput;
      wmnuinput.addVetoOnThisFinalState(wenufinder);
      WFinder wmnufinder(wmnuinput, -3.5, 3.5, 25.0*GeV, PID::MUON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wmnufinder, "WmnuFinder");

      VetoedFinalState jetinput;
      jetinput
          .addVetoOnThisFinalState(wenufinder)
          .addVetoOnThisFinalState(wmnufinder);
      FastJets jetpro(jetinput, FastJets::KT, 0.6);
      addProjection(jetpro, "Jets");

      MC_JetSplittings::init();
    }



    /// Do the analysis
    void analyze(const Event & e) {
      const WFinder& wenufinder = applyProjection<WFinder>(e, "WenuFinder");
      if (wenufinder.bosons().size()!=1) {
        vetoEvent;
      }

      const WFinder& wmnufinder = applyProjection<WFinder>(e, "WmnuFinder");
      if (wmnufinder.bosons().size()!=1) {
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
  DECLARE_RIVET_PLUGIN(MC_WWKTSPLITTINGS);

}

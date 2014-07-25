// -*- C++ -*-
#include "Rivet/Analyses/MC_JetSplittings.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {

  /// @brief MC validation analysis for Z[ee]Z[mumu] + jets events
  class MC_ZZKTSPLITTINGS : public MC_JetSplittings {
  public:

    /// Default constructor
    MC_ZZKTSPLITTINGS()
      : MC_JetSplittings("MC_ZZKTSPLITTINGS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      ZFinder zeefinder(FinalState(), -3.5, 3.5, 25*GeV, PID::ELECTRON, 65*GeV, 115*GeV,
                        0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zeefinder, "ZeeFinder");

      VetoedFinalState zmminput;
      zmminput.addVetoOnThisFinalState(zeefinder);
      ZFinder zmmfinder(zmminput, -3.5, 3.5, 25*GeV, PID::MUON, 65*GeV, 115*GeV,
                        0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zmmfinder, "ZmmFinder");

      VetoedFinalState jetinput;
      jetinput
          .addVetoOnThisFinalState(zeefinder)
          .addVetoOnThisFinalState(zmmfinder);
      FastJets jetpro(jetinput, FastJets::KT, 0.6);
      addProjection(jetpro, "Jets");

      MC_JetSplittings::init();
    }


    /// Do the analysis
    void analyze(const Event & e) {
      const ZFinder& zeefinder = applyProjection<ZFinder>(e, "ZeeFinder");
      if (zeefinder.bosons().size() != 1) vetoEvent;
      const ZFinder& zmmfinder = applyProjection<ZFinder>(e, "ZmmFinder");
      if (zmmfinder.bosons().size() != 1) vetoEvent;
      MC_JetSplittings::analyze(e);
    }


    /// Finalize
    void finalize() {
      MC_JetSplittings::finalize();
    }

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_ZZKTSPLITTINGS);

}

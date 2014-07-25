// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// @brief STAR di-hadron correlations in d-Au at 200 GeV
  class STAR_2008_S7993412 : public Analysis {
  public:

    STAR_2008_S7993412() : Analysis("STAR_2008_S7993412")
    {
    }


    /// @name Analysis methods
    //@{

    /// Book projections and histograms
    void init() {
      ChargedFinalState fs(-1.0, 1.0, 1.0*GeV);
      addProjection(fs, "FS");

      _h_Y_jet_trigger = bookProfile1D(1, 1, 1);
      _h_Y_jet_associated = bookProfile1D(2, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      // Skip if the event is empty
      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      if (fs.empty()) {
        MSG_DEBUG("Skipping event " << numEvents() << " because no final state found ");
        vetoEvent;
      }

      const double weight = event.weight();

      foreach (const Particle& tp, fs.particles()) {
        const double triggerpT = tp.pT();
        if (triggerpT >= 2.0 && triggerpT < 5.0) {
          int n_associated = 0;
          foreach (const Particle& ap, fs.particles()) {
            if (!inRange(ap.pT()/GeV, 1.5, triggerpT)) continue;
            if (deltaPhi(tp.momentum().phi(), ap.momentum().phi()) > 1) continue;
            if (fabs(tp.eta() - ap.eta()) > 1.75) continue;
            n_associated += 1;
          }
          //const double dPhidEta = 2 * 2*1.75;
          //_h_Y_jet_trigger->fill(triggerpT, n_associated/dPhidEta, weight);
          _h_Y_jet_trigger->fill(triggerpT, n_associated, weight);
        }
      }
    }


    /// Finalize
    void finalize() {
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Profile1DPtr _h_Y_jet_trigger;
    Profile1DPtr _h_Y_jet_associated;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2008_S7993412);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerCDFRun0Run1.hh"

namespace Rivet {


  /// @brief CDF pseudorapidity analysis at 630 and 1800 GeV
  /// @author Andy Buckley
  class CDF_1990_S2089246 : public Analysis {
  public:

    /// Constructor
    CDF_1990_S2089246()
      : Analysis("CDF_1990_S2089246")
    {
      _sumWTrig = 0;
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Setup projections
      addProjection(TriggerCDFRun0Run1(), "Trigger");
      addProjection(ChargedFinalState(-3.5, 3.5), "CFS");

      // Book histo
      if (fuzzyEquals(sqrtS()/GeV, 1800, 1E-3)) {
        _hist_eta = bookHisto1D(3, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 630, 1E-3)) {
        _hist_eta = bookHisto1D(4, 1, 1);
      }
    }


    /// Do the analysis
    void analyze(const Event& event) {
      // Trigger
      const bool trigger = applyProjection<TriggerCDFRun0Run1>(event, "Trigger").minBiasDecision();
      if (!trigger) vetoEvent;
      const double weight = event.weight();
      _sumWTrig += weight;

      // Loop over final state charged particles to fill eta histos
      const FinalState& fs = applyProjection<FinalState>(event, "CFS");
      foreach (const Particle& p, fs.particles()) {
        const double eta = p.momentum().pseudorapidity();
        _hist_eta->fill(fabs(eta), weight);
      }
    }


    /// Finalize
    void finalize() {
      // Divide through by num events to get d<N>/d(eta) in bins
      // Factor of 1/2 for |eta| -> eta
      scale(_hist_eta, 0.5/_sumWTrig);
    }

    //@}


  private:

    /// @name Weight counter
    //@{
    double _sumWTrig;
    //@}

    /// @name Histogram collections
    //@{
    Histo1DPtr _hist_eta;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1990_S2089246);

}

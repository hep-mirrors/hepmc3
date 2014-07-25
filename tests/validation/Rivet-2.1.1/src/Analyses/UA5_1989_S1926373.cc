// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerUA5.hh"

namespace Rivet {


  /// @brief UA5 min bias charged multiplicities in central \f$ \eta \f$ ranges
  class UA5_1989_S1926373 : public Analysis {
  public:

    /// Constructor
    UA5_1989_S1926373() : Analysis("UA5_1989_S1926373") {
      _sumWPassed = 0;
    }


    /// @name Analysis methods
    //@{

    /// Book histograms and projections
    void init() {
      addProjection(TriggerUA5(), "Trigger");
      addProjection(ChargedFinalState(-0.5, 0.5), "CFS05");
      addProjection(ChargedFinalState(-1.5, 1.5), "CFS15");
      addProjection(ChargedFinalState(-3.0, 3.0), "CFS30");
      addProjection(ChargedFinalState(-5.0, 5.0), "CFS50");

      // NB. _hist_nch and _hist_ncheta50 use the same data but different binning
      if (fuzzyEquals(sqrtS()/GeV, 200, 1E-3)) {
        _hist_nch        = bookHisto1D(1, 1, 1);
        _hist_nch_eta05  = bookHisto1D(3, 1, 1);
        _hist_nch_eta15  = bookHisto1D(4, 1, 1);
        _hist_nch_eta30  = bookHisto1D(5, 1, 1);
        _hist_nch_eta50  = bookHisto1D(6, 1, 1);
        _hist_mean_nch   = bookHisto1D(11, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) {
        _hist_nch        = bookHisto1D(2, 1, 1);
        _hist_nch_eta05  = bookHisto1D(7, 1, 1);
        _hist_nch_eta15  = bookHisto1D(8, 1, 1);
        _hist_nch_eta30  = bookHisto1D(9, 1, 1);
        _hist_nch_eta50  = bookHisto1D(10, 1, 1);
        _hist_mean_nch   = bookHisto1D(12, 1, 1);
      }

      /// @todo Moments of distributions
    }


    /// Do the analysis
    void analyze(const Event& event) {
      // Trigger
      const TriggerUA5& trigger = applyProjection<TriggerUA5>(event, "Trigger");
      if (!trigger.nsdDecision()) vetoEvent;

      const double weight = event.weight();
      _sumWPassed += weight;

      // Count final state particles in several eta regions
      const int numP05 = applyProjection<ChargedFinalState>(event, "CFS05").size();
      const int numP15 = applyProjection<ChargedFinalState>(event, "CFS15").size();
      const int numP30 = applyProjection<ChargedFinalState>(event, "CFS30").size();
      const int numP50 = applyProjection<ChargedFinalState>(event, "CFS50").size();

      // Fill histograms
      _hist_nch->fill(numP50, weight);
      _hist_nch_eta05->fill(numP05, weight);
      _hist_nch_eta15->fill(numP15, weight);
      _hist_nch_eta30->fill(numP30, weight);
      _hist_nch_eta50->fill(numP50, weight);
      _hist_mean_nch->fill(_hist_mean_nch->bin(0).midpoint(), numP50*weight);
    }


    void finalize() {
      scale(_hist_nch, 1.0/_sumWPassed);
      scale(_hist_nch_eta05, 1.0/_sumWPassed);
      scale(_hist_nch_eta15, 1.0/_sumWPassed);
      scale(_hist_nch_eta30, 1.0/_sumWPassed);
      scale(_hist_nch_eta50, 1.0/_sumWPassed);
      scale(_hist_mean_nch, 1.0/_sumWPassed);
    }

    //@}


  private:

    /// @name Counters
    //@{
    double _sumWPassed;
    //@}

    /// @name Histograms
    //@{
    Histo1DPtr _hist_nch;
    Histo1DPtr _hist_nch_eta05;
    Histo1DPtr _hist_nch_eta15;
    Histo1DPtr _hist_nch_eta30;
    Histo1DPtr _hist_nch_eta50;
    Histo1DPtr _hist_mean_nch;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(UA5_1989_S1926373);

}

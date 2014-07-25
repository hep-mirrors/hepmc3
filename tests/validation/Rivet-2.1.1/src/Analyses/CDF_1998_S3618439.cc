// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF diff cross-section in events with large missing energy
  class CDF_1998_S3618439 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CDF_1998_S3618439()
      : Analysis("CDF_1998_S3618439")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs(-4.2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_sumET_20 = bookHisto1D(1, 1, 1);
      _h_sumET_100 = bookHisto1D(1, 1, 2);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByEt(20.0*GeV);
      double sumET_20(0.0), sumET_100(0.0);
      foreach (const Jet& jet, jets) {
        double ET = jet.momentum().Et()/GeV;
        sumET_20 += ET;
        if (ET>100.0) sumET_100 += ET;
      }
      if (sumET_20>320.0) _h_sumET_20->fill(sumET_20, weight);
      if (sumET_100>320.0) _h_sumET_100->fill(sumET_100, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_sumET_20, crossSection()/picobarn/sumOfWeights());
      scale(_h_sumET_100, crossSection()/picobarn/sumOfWeights());
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_sumET_20, _h_sumET_100;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1998_S3618439);

}

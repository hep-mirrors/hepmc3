// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF Run I inclusive jet cross-section
  class CDF_2001_S4563131 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CDF_2001_S4563131()
      : Analysis("CDF_2001_S4563131")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs(-4,2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_ET = bookHisto1D(1, 1, 1);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByEt(40.0*GeV);
      foreach (const Jet& jet, jets) {
        double eta = fabs(jet.eta());
        if (eta>0.1 && eta<0.7) {
          _h_ET->fill(jet.momentum().Et(), weight);
        }
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      double deta = 1.2;
      scale(_h_ET, crossSection()/sumOfWeights()/deta/nanobarn);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_ET;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2001_S4563131);

}

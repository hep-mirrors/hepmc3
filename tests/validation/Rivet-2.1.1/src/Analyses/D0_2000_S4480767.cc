// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/WFinder.hh"

namespace Rivet {


  class D0_2000_S4480767 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    D0_2000_S4480767()
      : Analysis("D0_2000_S4480767")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs;
      WFinder wf(fs, -5, 5, 0.0*GeV, PID::ELECTRON, 0.0*GeV, 200.0*GeV, 0.0*GeV, 0.2);
      addProjection(wf, "WFinder");

      _h_W_pT = bookHisto1D(1, 1, 1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const WFinder& wf = applyProjection<WFinder>(event, "WFinder");
      if (wf.bosons().size() == 0) vetoEvent;

      _h_W_pT->fill(wf.bosons()[0].pT()/GeV, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      scale(_h_W_pT, crossSection()/sumOfWeights());

    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_W_pT;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2000_S4480767);

}

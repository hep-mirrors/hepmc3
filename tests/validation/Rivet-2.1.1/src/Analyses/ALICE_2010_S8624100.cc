// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class ALICE_2010_S8624100 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ALICE_2010_S8624100()
      : Analysis("ALICE_2010_S8624100")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      ChargedFinalState cfs05(-0.5, 0.5);
      ChargedFinalState cfs10(-1.0, 1.0);
      ChargedFinalState cfs13(-1.3, 1.3);
      addProjection(cfs05, "CFS05");
      addProjection(cfs10, "CFS10");
      addProjection(cfs13, "CFS13");

      if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) {
        _h_dN_dNch_05    = bookHisto1D(11, 1, 1);
        _h_dN_dNch_10    = bookHisto1D(12, 1, 1);
        _h_dN_dNch_13    = bookHisto1D(13, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 2360, 1E-3)) {
        _h_dN_dNch_05    = bookHisto1D(17, 1, 1);
        _h_dN_dNch_10    = bookHisto1D(18, 1, 1);
        _h_dN_dNch_13    = bookHisto1D(19, 1, 1);
      }

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const ChargedFinalState& charged_05 = applyProjection<ChargedFinalState>(event, "CFS05");
      const ChargedFinalState& charged_10 = applyProjection<ChargedFinalState>(event, "CFS10");
      const ChargedFinalState& charged_13 = applyProjection<ChargedFinalState>(event, "CFS13");

      _h_dN_dNch_05->fill(charged_05.size(), weight);
      _h_dN_dNch_10->fill(charged_10.size(), weight);
      _h_dN_dNch_13->fill(charged_13.size(), weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      normalize(_h_dN_dNch_05);
      normalize(_h_dN_dNch_10);
      normalize(_h_dN_dNch_13);

    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_dN_dNch_05;
    Histo1DPtr _h_dN_dNch_10;
    Histo1DPtr _h_dN_dNch_13;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2010_S8624100);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class ALICE_2010_S8625980 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ALICE_2010_S8625980()
      : Analysis("ALICE_2010_S8625980"),
        _Nevt_after_cuts(0.0)
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      ChargedFinalState cfs(-1.0, 1.0);
      addProjection(cfs, "CFS");

      if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) {
        _h_dN_deta    = bookHisto1D(4, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 2360, 1E-3)) {
        _h_dN_deta    = bookHisto1D(5, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 7000, 1E-3)) {
        _h_dN_deta    = bookHisto1D(6, 1, 1);
        _h_dN_dNch    = bookHisto1D(3, 1, 1);
      }

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const ChargedFinalState& charged = applyProjection<ChargedFinalState>(event, "CFS");
      if (charged.size() < 1) {
        vetoEvent;
      }
      _Nevt_after_cuts += weight;


      foreach (const Particle& p, charged.particles()) {
        const double eta = p.momentum().pseudorapidity();
        _h_dN_deta->fill(eta, weight);
      }

      if (fuzzyEquals(sqrtS()/GeV, 7000, 1E-3)) {
        _h_dN_dNch->fill(charged.size(), weight);
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      if (fuzzyEquals(sqrtS()/GeV, 7000, 1E-3)) {
        normalize(_h_dN_dNch);
      }
      scale(_h_dN_deta, 1.0/_Nevt_after_cuts);

    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_dN_deta;
    Histo1DPtr _h_dN_dNch;
    double _Nevt_after_cuts;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2010_S8625980);

}

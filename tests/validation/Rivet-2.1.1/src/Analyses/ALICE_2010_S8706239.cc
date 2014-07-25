// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class ALICE_2010_S8706239 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ALICE_2010_S8706239()
      : Analysis("ALICE_2010_S8706239"),
        _Nevt_after_cuts(0.0)
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      ChargedFinalState cfs(-0.8, 0.8, 0.15);
      addProjection(cfs, "CFS");

      _h_pT = bookHisto1D(4, 1, 1);

      _h_pT_Nch_015 = bookProfile1D(11, 1, 1);
      _h_pT_Nch_05  = bookProfile1D(12, 1, 1);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const ChargedFinalState& charged = applyProjection<ChargedFinalState>(event, "CFS");

      _Nevt_after_cuts += weight;

      // Get number of particles that fulfill certain pT requirements
      int Nch_015 = 0;
      int Nch_05  = 0;
      foreach (const Particle& p, charged.particles()) {
        double pT = p.pT()/GeV;
        if (pT < 4.0) Nch_015++;
        if (pT > 0.5  && pT < 4.0) Nch_05++;
      }

      // Now we can fill histograms
      foreach (const Particle& p, charged.particles()) {
        double pT = p.pT()/GeV;
        if (pT < 4.0) _h_pT_Nch_015 ->fill(Nch_015, pT, weight);
        if (pT > 0.5  && pT < 4.0) _h_pT_Nch_05  ->fill(Nch_05,  pT, weight);

      // To get the Yield, fill appropriate weight 1/(2PI * pT * d eta)
        _h_pT->fill(pT, weight /(TWOPI*pT*1.6) );
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_pT, 1.0/_Nevt_after_cuts);
    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_pT;

    Profile1DPtr _h_pT_Nch_015 ;
    Profile1DPtr _h_pT_Nch_05  ;

    double _Nevt_after_cuts;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2010_S8706239);

}

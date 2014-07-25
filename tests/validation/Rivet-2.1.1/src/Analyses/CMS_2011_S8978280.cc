// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief CMS strange particle spectra (Ks, Lambda, Cascade) in pp at 900 and 7000 GeV
  /// @author Kevin Stenson
  class CMS_2011_S8978280 : public Analysis {
  public:

    /// Constructor
    CMS_2011_S8978280() : Analysis("CMS_2011_S8978280") {}


    void init() {
      // Need wide range of eta because cut on rapidity not pseudorapidity
      UnstableFinalState ufs(-8.0, 8.0, 0.0*GeV);
      addProjection(ufs, "UFS");

      // Particle distributions versus rapidity and transverse momentum
      if (fuzzyEquals(sqrtS()/GeV, 900.0*GeV)){
	_h_dNKshort_dy  = bookHisto1D(1, 1, 1);
	_h_dNKshort_dpT = bookHisto1D(2, 1, 1);
	_h_dNLambda_dy  = bookHisto1D(3, 1, 1);
	_h_dNLambda_dpT = bookHisto1D(4, 1, 1);
	_h_dNXi_dy      = bookHisto1D(5, 1, 1);
	_h_dNXi_dpT     = bookHisto1D(6, 1, 1);

	_h_LampT_KpT 	= bookScatter2D(7, 1, 1);
	_h_XipT_LampT 	= bookScatter2D(8, 1, 1);
	_h_Lamy_Ky 	= bookScatter2D(9, 1, 1);
	_h_Xiy_Lamy 	= bookScatter2D(10, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 7000.0*GeV)){
	_h_dNKshort_dy  = bookHisto1D(1, 1, 2);
	_h_dNKshort_dpT = bookHisto1D(2, 1, 2);
	_h_dNLambda_dy  = bookHisto1D(3, 1, 2);
	_h_dNLambda_dpT = bookHisto1D(4, 1, 2);
	_h_dNXi_dy      = bookHisto1D(5, 1, 2);
	_h_dNXi_dpT     = bookHisto1D(6, 1, 2);

	_h_LampT_KpT 	= bookScatter2D(7, 1, 2);
	_h_XipT_LampT 	= bookScatter2D(8, 1, 2);
	_h_Lamy_Ky 	= bookScatter2D(9, 1, 2);
	_h_Xiy_Lamy 	= bookScatter2D(10, 1, 2);
      }
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      const UnstableFinalState& parts = applyProjection<UnstableFinalState>(event, "UFS");

      foreach (const Particle& p, parts.particles()) {
        const double pT = p.pT();
        const double y = fabs(p.rapidity());
        const PdgId pid = abs(p.pdgId());

        if (y < 2.0) {
          switch (pid) {
          case PID::K0S:
              _h_dNKshort_dy->fill(y, weight);
              _h_dNKshort_dpT->fill(pT, weight);
            break;
          case PID::LAMBDA:
            // Lambda should not have Cascade or Omega ancestors since they should not decay. But just in case...
            if ( !( p.hasAncestor(3322) || p.hasAncestor(-3322) || p.hasAncestor(3312) || p.hasAncestor(-3312) || p.hasAncestor(3334) || p.hasAncestor(-3334) ) ) {
              _h_dNLambda_dy->fill(y, weight);
              _h_dNLambda_dpT->fill(pT, weight);
            }
            break;
          case PID::XIMINUS:
            // Cascade should not have Omega ancestors since it should not decay.  But just in case...
            if ( !( p.hasAncestor(3334) || p.hasAncestor(-3334) ) ) {
              _h_dNXi_dy->fill(y, weight);
              _h_dNXi_dpT->fill(pT, weight);
            }
            break;
          }
        }
      }
    }


    void finalize() {

      divide(_h_dNLambda_dpT,_h_dNKshort_dpT,
	     _h_LampT_KpT);

      divide(_h_dNXi_dpT,_h_dNLambda_dpT,
	     _h_XipT_LampT);

      divide(_h_dNLambda_dy,_h_dNKshort_dy,
	     _h_Lamy_Ky);

      divide(_h_dNXi_dy,_h_dNLambda_dy,
	     _h_Xiy_Lamy);

      double normpT = 1.0/sumOfWeights();
      double normy = 0.5*normpT; // Accounts for using |y| instead of y
      scale(_h_dNKshort_dy, normy);
      scale(_h_dNKshort_dpT, normpT);
      scale(_h_dNLambda_dy, normy);
      scale(_h_dNLambda_dpT, normpT);
      scale(_h_dNXi_dy, normy);
      scale(_h_dNXi_dpT, normpT);
    }


  private:

    // Particle distributions versus rapidity and transverse momentum
    Histo1DPtr _h_dNKshort_dy;
    Histo1DPtr _h_dNKshort_dpT;
    Histo1DPtr _h_dNLambda_dy;
    Histo1DPtr _h_dNLambda_dpT;
    Histo1DPtr _h_dNXi_dy;
    Histo1DPtr _h_dNXi_dpT;

    Scatter2DPtr _h_LampT_KpT;
    Scatter2DPtr _h_XipT_LampT;
    Scatter2DPtr _h_Lamy_Ky;
    Scatter2DPtr _h_Xiy_Lamy;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S8978280);

}

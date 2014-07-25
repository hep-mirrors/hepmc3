// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// @brief ATLAS minimum bias analysis at 900 GeV
  /// @author Frank Siegert
  class ATLAS_2010_S8591806 : public Analysis {
  public:

    ATLAS_2010_S8591806()
      : Analysis("ATLAS_2010_S8591806"),
        _Nevt_after_cuts(0.0)
    {    }


    void init() {
      ChargedFinalState cfs(-2.5, 2.5, 0.5*GeV);
      addProjection(cfs, "CFS");

      _h_dNch_deta = bookHisto1D(2, 1, 1);
      _h_dNch_dpT = bookHisto1D(3, 1, 1);
      _h_dNevt_dNch = bookHisto1D(4, 1, 1);
      _p_meanpT_Nch = bookProfile1D(5, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      const ChargedFinalState& charged = applyProjection<ChargedFinalState>(event, "CFS");
      if (charged.size() < 1) {
        vetoEvent;
      }
      _Nevt_after_cuts += weight;

      _h_dNevt_dNch->fill(charged.size(), weight);
      foreach (const Particle& p, charged.particles()) {
        double pT = p.pT()/GeV;
        _h_dNch_deta->fill(p.eta(), weight);
        _h_dNch_dpT->fill(pT, weight/pT);
        _p_meanpT_Nch->fill(charged.size(), pT, weight);
      }
    }


    void finalize() {
      double deta = 5.0;
      scale(_h_dNch_deta, 1.0/_Nevt_after_cuts);
      scale(_h_dNch_dpT, 1.0/_Nevt_after_cuts/TWOPI/deta);
      scale(_h_dNevt_dNch, 1.0/_Nevt_after_cuts);
    }


  private:

    Histo1DPtr _h_dNch_deta;
    Histo1DPtr _h_dNch_dpT;
    Histo1DPtr _h_dNevt_dNch;
    Profile1DPtr  _p_meanpT_Nch;

    double _Nevt_after_cuts;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2010_S8591806);

}

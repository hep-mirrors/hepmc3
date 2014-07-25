// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Tools/BinnedHistogram.hh"

namespace Rivet {


  class LHCF_2012_I1115479 : public Analysis {
  public:

    LHCF_2012_I1115479()
      : Analysis("LHCF_2012_I1115479")
    {   }


  public:

    void init() {
      addProjection(UnstableFinalState(),"UFS");

      _binnedHistos_y_pT.addHistogram( 8.9,  9.0, bookHisto1D(1, 1, 1));
      _binnedHistos_y_pT.addHistogram( 9.0,  9.2, bookHisto1D(2, 1, 1));
      _binnedHistos_y_pT.addHistogram( 9.2,  9.4, bookHisto1D(3, 1, 1));
      _binnedHistos_y_pT.addHistogram( 9.4,  9.6, bookHisto1D(4, 1, 1));
      _binnedHistos_y_pT.addHistogram( 9.6, 10.0, bookHisto1D(5, 1, 1));
      _binnedHistos_y_pT.addHistogram(10.0, 11.0, bookHisto1D(6, 1, 1));
    }


    void analyze(const Event& event) {
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");

      const double weight = event.weight();
      const double dphi = TWOPI;

      foreach (const Particle& p, ufs.particles()) {
        if (p.pdgId() == 111) {
          double pT = p.pT();
          double y  = p.rapidity();

          if (pT > 0.6*GeV) continue;

          const double scaled_weight = weight/(dphi*pT/GeV);
          _binnedHistos_y_pT.fill(y, pT/GeV, scaled_weight);
        }
      }
    }


    void finalize() {
      _binnedHistos_y_pT.scale( 1./sumOfWeights() , this);
    }

  private:

    BinnedHistogram<double> _binnedHistos_y_pT;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(LHCF_2012_I1115479);

}

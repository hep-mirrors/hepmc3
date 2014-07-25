#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  class ALICE_2011_S8909580 : public Analysis {
  public:

    ALICE_2011_S8909580()
      : Analysis("ALICE_2011_S8909580")
    {}


  public:
    void init() {
      const UnstableFinalState ufs(-15, 15);
      addProjection(ufs, "UFS");

      _histPtK0s        = bookHisto1D(1, 1, 1);
      _histPtLambda     = bookHisto1D(2, 1, 1);
      _histPtAntiLambda = bookHisto1D(3, 1, 1);
      _histPtXi         = bookHisto1D(4, 1, 1);
      _histPtPhi        = bookHisto1D(5, 1, 1);
      _temp_h_Lambdas   = bookHisto1D("TMP/h_Lambdas", refData(6, 1, 1));
      _temp_h_Kzeros    = bookHisto1D("TMP/h_Kzeros", refData(6, 1, 1));
      _h_LamKzero       = bookScatter2D(6, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");

      foreach (const Particle& p, ufs.particles()) {
        const double absrap = fabs(p.rapidity());
        const double pT = p.pT()/GeV;

        if (absrap < 0.8) {
          switch(p.pdgId()) {
          case 3312:
          case -3312:
            if ( !( p.hasAncestor(3334) || p.hasAncestor(-3334) ) ) {
              _histPtXi->fill(pT, weight);
            }
            break;
            if (absrap < 0.75) {
            case 310:
              _histPtK0s->fill(pT, weight);
              _temp_h_Kzeros->fill(pT, 2*weight);
              break;
            case 3122:
              if ( !( p.hasAncestor(3322) || p.hasAncestor(-3322) ||
                      p.hasAncestor(3312) || p.hasAncestor(-3312) ||
                      p.hasAncestor(3334) || p.hasAncestor(-3334) ) ) {
                _histPtLambda->fill(pT, weight);
                _temp_h_Lambdas->fill(pT, weight);
              }
              break;
            case -3122:
              if ( !( p.hasAncestor(3322) || p.hasAncestor(-3322) ||
                      p.hasAncestor(3312) || p.hasAncestor(-3312) ||
                      p.hasAncestor(3334) || p.hasAncestor(-3334) ) ) {
                _histPtAntiLambda->fill(pT, weight);
                _temp_h_Lambdas->fill(pT, weight);
              }
              break;
            }
            if (absrap<0.6) {
            case 333:
              _histPtPhi->fill(pT, weight);
              break;
            }
          }
        }
      }
    }


    void finalize() {
      scale(_histPtK0s,        1./(1.5*sumOfWeights()));
      scale(_histPtLambda,     1./(1.5*sumOfWeights()));
      scale(_histPtAntiLambda, 1./(1.5*sumOfWeights()));
      scale(_histPtXi,         1./(1.6*sumOfWeights()));
      scale(_histPtPhi,        1./(1.2*sumOfWeights()));
      divide(_temp_h_Lambdas, _temp_h_Kzeros, _h_LamKzero);
    }


  private:

    Histo1DPtr _histPtK0s, _histPtLambda, _histPtAntiLambda, _histPtXi, _histPtPhi;
    Histo1DPtr _temp_h_Lambdas, _temp_h_Kzeros;
    Scatter2DPtr _h_LamKzero;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2011_S8909580);

}

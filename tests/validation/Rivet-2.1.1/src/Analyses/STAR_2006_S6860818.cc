// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief STAR strange particle spectra in pp at 200 GeV
  class STAR_2006_S6860818 : public Analysis {
  public:

    /// Constructor
    STAR_2006_S6860818()
      : Analysis("STAR_2006_S6860818"),
        _sumWeightSelected(0.0)
    {
      for (size_t i = 0; i < 4; i++) {
        _nBaryon[i] = 0;
        _nAntiBaryon[i] = 0;
        _nWeightedBaryon[i] = 0.;
        _nWeightedAntiBaryon[i] = 0.;
      }
    }

    /// Book projections and histograms
    void init() {
      ChargedFinalState bbc1(-5.0,-3.5, 0.0*GeV); // beam-beam-counter trigger
      ChargedFinalState bbc2( 3.5, 5.0, 0.0*GeV); // beam-beam-counter trigger
      addProjection(bbc1, "BBC1");
      addProjection(bbc2, "BBC2");

      UnstableFinalState ufs(-2.5, 2.5, 0.0*GeV);
      addProjection(ufs, "UFS");

      _h_pT_k0s        = bookHisto1D(1, 1, 1);
      _h_pT_kminus     = bookHisto1D(1, 2, 1);
      _h_pT_kplus      = bookHisto1D(1, 3, 1);
      _h_pT_lambda     = bookHisto1D(1, 4, 1);
      _h_pT_lambdabar  = bookHisto1D(1, 5, 1);
      _h_pT_ximinus    = bookHisto1D(1, 6, 1);
      _h_pT_xiplus     = bookHisto1D(1, 7, 1);
      //_h_pT_omega      = bookHisto1D(1, 8, 1);
      _h_antibaryon_baryon_ratio = bookScatter2D(2, 1, 1);
      _h_lambar_lam = bookScatter2D(2, 2, 1);
      _h_xiplus_ximinus = bookScatter2D(2, 3, 1);
      _h_pT_vs_mass    = bookProfile1D(3, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      const ChargedFinalState& bbc1 = applyProjection<ChargedFinalState>(event, "BBC1");
      const ChargedFinalState& bbc2 = applyProjection<ChargedFinalState>(event, "BBC2");
      if (bbc1.size()<1 || bbc2.size()<1) {
        MSG_DEBUG("Failed beam-beam-counter trigger");
        vetoEvent;
      }

      const double weight = event.weight();

      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");
      foreach (const Particle& p, ufs.particles()) {
        if (fabs(p.rapidity()) < 0.5) {
          const PdgId pid = p.pdgId();
          const double pT = p.pT() / GeV;
          switch (abs(pid)) {
          case PID::PIPLUS:
            if (pid < 0) _h_pT_vs_mass->fill(0.1396, pT, weight);
            break;
          case PID::PROTON:
            if (pid < 0) _h_pT_vs_mass->fill(0.9383, pT, weight);
            if (pT > 0.4) {
              pid > 0 ? _nBaryon[0]++ : _nAntiBaryon[0]++;
              pid > 0 ? _nWeightedBaryon[0]+=weight : _nWeightedAntiBaryon[0]+=weight;
            }
            break;
          case PID::K0S:
            if (pT > 0.2) {
              _h_pT_k0s->fill(pT, weight/pT);
            }
            _h_pT_vs_mass->fill(0.5056, pT, weight);
            break;
          case PID::K0L:
            _h_pT_vs_mass->fill(0.5056, pT, weight);
            break;
          case 113: // rho0(770)
            _h_pT_vs_mass->fill(0.7755, pT, weight);
            break;
          case 313: // K0*(892)
            _h_pT_vs_mass->fill(0.8960, pT, weight);
            break;
          case 333: // phi(1020)
            _h_pT_vs_mass->fill(1.0190, pT, weight);
            break;
          case 3214: // Sigma(1385)
            _h_pT_vs_mass->fill(1.3840, pT, weight);
            break;
          case 3124: // Lambda(1520)
            _h_pT_vs_mass->fill(1.5200, pT, weight);
            break;
          case PID::KPLUS:
            if (pid < 0) _h_pT_vs_mass->fill(0.4856, pT, weight);
            if (pT > 0.2) {
              pid > 0 ? _h_pT_kplus->fill(pT, weight/pT) : _h_pT_kminus->fill(pT, weight/pT);
            }
            break;
          case PID::LAMBDA:
            pid > 0 ? _h_pT_vs_mass->fill(1.1050, pT, weight) : _h_pT_vs_mass->fill(1.1250, pT, weight);
            if (pT > 0.3) {
              pid > 0 ? _h_pT_lambda->fill(pT, weight/pT) : _h_pT_lambdabar->fill(pT, weight/pT);
              pid > 0 ? _nBaryon[1]++ : _nAntiBaryon[1]++;
              pid > 0 ? _nWeightedBaryon[1]+=weight : _nWeightedAntiBaryon[1]+=weight;
            }
            break;
          case PID::XIMINUS:
            pid > 0 ? _h_pT_vs_mass->fill(1.3120, pT, weight) : _h_pT_vs_mass->fill(1.3320, pT, weight);
            if (pT > 0.5) {
              pid > 0 ? _h_pT_ximinus->fill(pT, weight/pT) : _h_pT_xiplus->fill(pT, weight/pT);
              pid > 0 ? _nBaryon[2]++ : _nAntiBaryon[2]++;
              pid > 0 ? _nWeightedBaryon[2]+=weight : _nWeightedAntiBaryon[2]+=weight;
            }
            break;
          case PID::OMEGAMINUS:
            _h_pT_vs_mass->fill(1.6720, pT, weight);
            if (pT > 0.5) {
              //_h_pT_omega->fill(pT, weight/pT);
              pid > 0 ? _nBaryon[3]++ : _nAntiBaryon[3]++;
              pid > 0 ? _nWeightedBaryon[3]+=weight : _nWeightedAntiBaryon[3]+=weight;
            }
            break;
          }

        }
      }

      _sumWeightSelected += event.weight();
    }


    /// Finalize
    void finalize() {
      std::vector<Point2D> points;
      for (size_t i=0 ; i<4 ; i++) {
        if (_nWeightedBaryon[i]==0 || _nWeightedAntiBaryon[i]==0) {
          points.push_back(Point2D(i,0,0.5,0));
        } else {
          double y  = _nWeightedAntiBaryon[i]/_nWeightedBaryon[i];
          double dy = sqrt( 1./_nAntiBaryon[i] + 1./_nBaryon[i] );
          points.push_back(Point2D(i,y,0.5,y*dy));
        }
      }
      _h_antibaryon_baryon_ratio->addPoints( points );

      divide(_h_pT_lambdabar,_h_pT_lambda, _h_lambar_lam);
      divide(_h_pT_xiplus,_h_pT_ximinus, _h_xiplus_ximinus);

      scale(_h_pT_k0s,       1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_kminus,    1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_kplus,     1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_lambda,    1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_lambdabar, 1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_ximinus,   1./(2*M_PI*_sumWeightSelected));
      scale(_h_pT_xiplus,    1./(2*M_PI*_sumWeightSelected));
      //scale(_h_pT_omega,     1./(2*M_PI*_sumWeightSelected));
      MSG_DEBUG("sumOfWeights()     = " << sumOfWeights());
      MSG_DEBUG("_sumWeightSelected = " << _sumWeightSelected);
    }

  private:

    double _sumWeightSelected;
    int _nBaryon[4];
    int _nAntiBaryon[4];
    double _nWeightedBaryon[4];
    double _nWeightedAntiBaryon[4];

    Histo1DPtr _h_pT_k0s, _h_pT_kminus, _h_pT_kplus, _h_pT_lambda, _h_pT_lambdabar, _h_pT_ximinus, _h_pT_xiplus;
    //Histo1DPtr _h_pT_omega;
    Scatter2DPtr _h_antibaryon_baryon_ratio;
    Profile1DPtr  _h_pT_vs_mass;
    Scatter2DPtr _h_lambar_lam;
    Scatter2DPtr _h_xiplus_ximinus;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2006_S6860818);


}

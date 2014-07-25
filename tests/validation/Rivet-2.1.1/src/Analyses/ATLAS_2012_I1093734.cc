// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Math/MathUtils.hh"

namespace Rivet {


  namespace {

    inline double sumAB(vector<double> vecX, vector<double> vecY, vector<double> vecW) {
      assert(vecX.size() == vecY.size() && vecX.size() == vecW.size());
      double sum(0);
      for (size_t i = 0; i < vecX.size(); i++) sum += vecW[i] * vecX[i] * vecY[i];
      return sum;
    }

    inline double sumA(vector<double> vecX, vector<double> vecW) {
      assert(vecX.size() == vecW.size());
      double sum(0);
      for (size_t i = 0; i < vecX.size(); i++) sum += vecX[i]*vecW[i];
      return sum;
    }

    inline double sumW(vector<double> vecW) {
      double sum(0);
      for (size_t i = 0; i < vecW.size(); i++) sum += vecW[i];
      return sum;
    }

    inline double mean(vector<double>  vecX, vector<double> vecW) {
      return sumA(vecX, vecW) / sumW(vecW);
    }

    inline double standard_deviation(vector<double> vecX, vector<double> vecW) {
      const double x_bar = mean(vecX, vecW);
      double sum(0);
      for (size_t i = 0; i < vecX.size(); i++) {
        sum += vecW[i] * sqr(vecX[i] - x_bar);
      }
      return sqrt( sum / sumW(vecW) );
    }

    inline double a0_regression(vector<double>  vecX, vector<double> vecY, vector<double> vecW) {
      const double numerator   = sumA(vecY, vecW) * sumAB(vecX, vecX, vecW) - sumA(vecX, vecW) * sumAB(vecX, vecY, vecW);
      const double denominator = sumW(vecW) * sumAB(vecX, vecX, vecW) - sumA(vecX, vecW) * sumA(vecX, vecW);
      return numerator / denominator;
    }

    inline double a1_regression(vector<double>  vecX, vector<double> vecY, vector<double> vecW) {
      const double numerator   = sumW(vecW) * sumAB(vecX,vecY,vecW) - sumA(vecX, vecW) * sumA(vecY, vecW);
      const double denominator = sumW(vecW) * sumAB(vecX,vecX,vecW) - sumA(vecX, vecW) * sumA(vecX, vecW);
      return numerator/ denominator;
    }

    inline double a1_regression2(vector<double>  vecX, vector<double> vecY, vector<double> vecW) {
      const double x_bar = mean(vecX, vecW);
      const double y_bar = mean(vecY, vecW);
      double sumXY(0);
      for (size_t i = 0; i < vecX.size(); i++) {
        sumXY += vecW[i] * (vecY[i]-y_bar) * (vecX[i]-x_bar);
      }
      return sumXY / ( standard_deviation(vecX, vecW) * standard_deviation(vecY, vecW) * sumW(vecW) );
    }

    inline double quadra_sum_residual(vector<double> vecX, vector<double> vecY, vector<double> vecW) {
      const double a0 = a0_regression(vecX, vecY, vecW);
      const double a1 = a1_regression(vecX, vecY, vecW);
      double sum(0);
      for (size_t i = 0; i < vecX.size(); i++) {
        const double y_est = a0 + a1*vecX[i];
        sum += vecW[i] * sqr(vecY[i] - y_est);
      }
      return sum;
    }

    inline double error_on_slope(vector<double> vecX, vector<double> vecY, vector<double> vecW) {
      const double quadra_sum_res = quadra_sum_residual(vecX, vecY, vecW);
      const double sqrt_quadra_sum_x   = standard_deviation(vecX, vecW) * sqrt(sumW(vecW));
      return sqrt(quadra_sum_res/(sumW(vecW)-2)) / sqrt_quadra_sum_x;
    }

  }



  /// Forward-backward and azimuthal correlations in minimum bias events
  class ATLAS_2012_I1093734 : public Analysis {
  public:

    /// Constructor
    ATLAS_2012_I1093734()
      : Analysis("ATLAS_2012_I1093734")
    {
      // Stat convergence happens around 20k events, so it doesn't make sense to run this
      // analysis with much less than that. Given that, lets avoid some unnecessary vector
      // resizing by allocating sensible amounts in the first place.
      for (int ipt = 0; ipt < NPTBINS; ++ipt) {
        for (int k = 0; k < NETABINS; ++k) {
          _vecsNchF [ipt][k].reserve(10000);
          _vecsNchB [ipt][k].reserve(10000);
          _vecWeight[ipt][k].reserve(10000);
          if (ipt == 0) {
            _vecsSumptF[k].reserve(10000);
            _vecsSumptB[k].reserve(10000);
          }
        }
      }
    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // FB correlations part

      // Projections
      for (int ipt = 0; ipt < NPTBINS; ++ipt) {
        const double ptmin = PTMINVALUES[ipt]*MeV;
        for (int ieta = 0; ieta < NETABINS; ++ieta) {
          addProjection(ChargedFinalState(-ETAVALUES[ieta],    -ETAVALUES[ieta]+0.5, ptmin), "Tracks"+ETABINNAMES[ieta]+"B"+PTBINNAMES[ipt]);
          addProjection(ChargedFinalState( ETAVALUES[ieta]-0.5, ETAVALUES[ieta],     ptmin), "Tracks"+ETABINNAMES[ieta]+"F"+PTBINNAMES[ipt]);
        }
        addProjection(ChargedFinalState(-2.5, 2.5, ptmin), "CFS" + PTBINNAMES[ipt]);
      }
      // Histos
      if (fuzzyEquals(sqrtS(), 7000*GeV, 1e-3)) {
        for (int ipt  = 0; ipt  < NPTBINS ; ++ipt ) _s_NchCorr_vsEta[ipt]  = bookScatter2D(1+ipt,  2, 1, true);
        for (int ieta = 0; ieta < NETABINS; ++ieta) _s_NchCorr_vsPt [ieta] = bookScatter2D(8+ieta, 2, 1, true);
        _s_PtsumCorr = bookScatter2D(13, 2, 1, true);
      } else if (fuzzyEquals(sqrtS(), 900*GeV, 1e-3)) {
        _s_NchCorr_vsEta[0] = bookScatter2D(14, 2, 1, true);
        _s_PtsumCorr        = bookScatter2D(15, 2, 1, true);
      }


      // Azimuthal correlations part
      // Projections
      const double ptmin = 500*MeV;
      addProjection(ChargedFinalState(-2.5, 2.5, ptmin), "ChargedTracks25");
      addProjection(ChargedFinalState(-2.0, 2.0, ptmin), "ChargedTracks20");
      addProjection(ChargedFinalState(-1.0, 1.0, ptmin), "ChargedTracks10");
      // Histos
      /// @todo Declare/book as temporary
      for (size_t ieta = 0; ieta < 3; ++ieta) {
        if (fuzzyEquals(sqrtS(), 7000*GeV, 1e-3)) {
          _s_dphiMin[ieta] = bookScatter2D(2+2*ieta, 1, 1, true);
          _s_diffSO[ieta] = bookScatter2D(8+2*ieta, 1, 1, true);
          _th_dphi[ieta] = YODA::Histo1D(refData(2+2*ieta, 1, 1));
          _th_same[ieta] = YODA::Histo1D(refData(8+2*ieta, 1, 1));
          _th_oppo[ieta] = YODA::Histo1D(refData(8+2*ieta, 1, 1));
        } else if (fuzzyEquals(sqrtS(), 900*GeV, 1e-3)) {
          _s_dphiMin[ieta] = bookScatter2D(1+2*ieta, 1, 1, true);
          _s_diffSO[ieta] = bookScatter2D(7+2*ieta, 1, 1, true);
          _th_dphi[ieta] = YODA::Histo1D(refData(1+2*ieta, 1, 1));
          _th_same[ieta] = YODA::Histo1D(refData(7+2*ieta, 1, 1));
          _th_oppo[ieta] = YODA::Histo1D(refData(7+2*ieta, 1, 1));
        }
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      for (int ipt = 0; ipt < NPTBINS; ++ipt) {
        const FinalState& charged = applyProjection<FinalState>(event, "CFS" + PTBINNAMES[ipt]);
        if (charged.particles().size() >= 2) {
          for (int ieta = 0; ieta < NETABINS; ++ieta) {
            const string fname = "Tracks" + ETABINNAMES[ieta] + "F" + PTBINNAMES[ipt];
            const string bname = "Tracks" + ETABINNAMES[ieta] + "B" + PTBINNAMES[ipt];
            const ParticleVector particlesF = applyProjection<FinalState>(event, fname).particles();
            const ParticleVector particlesB = applyProjection<FinalState>(event, bname).particles();
            _vecsNchF[ipt][ieta].push_back((double) particlesF.size());
            _vecsNchB[ipt][ieta].push_back((double) particlesB.size());
            _vecWeight[ipt][ieta].push_back(weight);

            // Sum pT only for 100 MeV particles
            if (ipt == 0) {
              double sumptF = 0;
              double sumptB = 0;
              foreach (const Particle& p, particlesF) sumptF += p.momentum().pT();
              foreach (const Particle& p, particlesB) sumptB += p.momentum().pT();
              _vecsSumptF[ieta].push_back(sumptF);
              _vecsSumptB[ieta].push_back(sumptB);
            }

          }
        }
      }


      string etabin[3] = { "10", "20", "25" };
      for (int ieta = 0; ieta < 3; ieta++) {
        const string fname = "ChargedTracks" + etabin[ieta];
        const ParticleVector partTrks = applyProjection<FinalState>(event, fname).particlesByPt();

        // Find the leading track and fill the temp histograms
        const Particle& plead = partTrks[0];
        foreach (const Particle& p, partTrks) {
          if (&plead == &p) continue; //< Don't compare the lead particle to itself
          const double dphi = deltaPhi(p.momentum(), plead.momentum());
          _th_dphi[ieta].fill(dphi, weight);
          const bool sameside = (plead.momentum().eta() * p.momentum().eta() > 0);
          (sameside ? _th_same : _th_oppo)[ieta].fill(dphi, weight);
        }
      }
    }


    /// Finalize
    void finalize() {

      // FB part
      // @todo For 2D plots we will need _vecsNchF[i], _vecsNchB[j]
      for (int ipt = 0; ipt < NPTBINS; ++ipt) {
        for (int ieta = 0; ieta < NETABINS; ++ieta) {
        _s_NchCorr_vsEta[ipt]->point(ieta).setY(a1_regression2(_vecsNchF[ipt][ieta], _vecsNchB[ipt][ieta], _vecWeight[ipt][ieta]));
        _s_NchCorr_vsEta[ipt]->point(ieta).setYErr(error_on_slope(_vecsNchF[ipt][ieta], _vecsNchB[ipt][ieta], _vecWeight[ipt][ieta]));
        }
        // There is just one plot at 900 GeV so exit the loop here
        if (fuzzyEquals(sqrtS(), 900*GeV, 1e-3) && ipt == 0) break;
      }

      if (!fuzzyEquals(sqrtS(),  900*GeV, 1e-3)) { //< No plots at 900 GeV
        for (int ieta = 0; ieta < NETABINS; ++ieta) {
          for (int ipt = 0; ipt < NPTBINS; ++ipt) {
            _s_NchCorr_vsPt[ieta]->point(ipt).setY(a1_regression2(_vecsNchF[ipt][ieta], _vecsNchB[ipt][ieta], _vecWeight[ipt][ieta]));
            _s_NchCorr_vsPt[ieta]->point(ipt).setYErr(error_on_slope(_vecsNchF[ipt][ieta], _vecsNchB[ipt][ieta], _vecWeight[ipt][ieta]));
          }
        }
      }

      // Sum pt only for 100 MeV particles
      for (int ieta = 0; ieta < NETABINS; ++ieta) {
        _s_PtsumCorr->point(ieta).setY(a1_regression2(_vecsSumptF[ieta], _vecsSumptB[ieta], _vecWeight[0][ieta]));
        _s_PtsumCorr->point(ieta).setYErr(error_on_slope(_vecsSumptF[ieta], _vecsSumptB[ieta], _vecWeight[0][ieta]));
      }


      // Azimuthal part
      for (int ieta = 0; ieta < 3; ieta++) {
        /// @note We don't just do a subtraction because of the risk of negative values and negative errors
        /// @todo Should the difference always be shown as positive?, i.e. y -> abs(y), etc.
        /// @todo Should the normalization be done _after_ the -ve value treatment?
        YODA::Histo1D hdiffSO = _th_same[ieta] - _th_oppo[ieta];
        hdiffSO.normalize(hdiffSO.bin(0).width());
        for (size_t i = 0; i < hdiffSO.numBins(); ++i) {
          const double y = hdiffSO.bin(i).height() >= 0 ? hdiffSO.bin(i).height() : 0;
          const double yerr = hdiffSO.bin(i).heightErr() >= 0 ? hdiffSO.bin(i).heightErr() : 0;
          _s_diffSO[ieta]->point(i).setY(y, yerr);
        }

        // Extract minimal value
        double histMin = _th_dphi[ieta].bin(0).height();
        for (size_t iphi = 1; iphi < _th_dphi[ieta].numBins(); ++iphi) {
          histMin = std::min(histMin, _th_dphi[ieta].bin(iphi).height());
        }
        // Build scatter of differences
        double sumDiff = 0;
        for (size_t iphi = 1; iphi < _th_dphi[ieta].numBins(); ++iphi) {
          const double diff = _th_dphi[ieta].bin(iphi).height() - histMin;
          _s_dphiMin[ieta]->point(iphi).setY(diff, _th_dphi[ieta].bin(iphi).heightErr());
          sumDiff += diff;
        }
        // Normalize
        _s_dphiMin[ieta]->scale(1, 1/sumDiff);
      }

    }

    //@}


  private:

    static const int NPTBINS  = 7;
    static const int NETABINS = 5;
    static const double PTMINVALUES[NPTBINS];
    static const string PTBINNAMES[NPTBINS];
    static const double ETAVALUES[NETABINS];
    static const string ETABINNAMES[NETABINS];

    vector<double> _vecWeight[NPTBINS][NETABINS];
    vector<double> _vecsNchF[NPTBINS][NETABINS];
    vector<double> _vecsNchB[NPTBINS][NETABINS];
    vector<double> _vecsSumptF[NETABINS];
    vector<double> _vecsSumptB[NETABINS];

    /// @name Histograms
    //@{
    Scatter2DPtr _s_NchCorr_vsEta[NPTBINS], _s_NchCorr_vsPt[NETABINS], _s_PtsumCorr;
    Scatter2DPtr _s_dphiMin[3], _s_diffSO[3];
    YODA::Histo1D _th_dphi[3], _th_same[3], _th_oppo[3];
    //@}

  };


  /// @todo Initialize these inline at declaration with C++11
  const double ATLAS_2012_I1093734::PTMINVALUES[] = {100, 200, 300, 500, 1000, 1500, 2000 };
  const string ATLAS_2012_I1093734::PTBINNAMES[] = { "100", "200", "300", "500", "1000", "1500", "2000" };
  const double ATLAS_2012_I1093734::ETAVALUES[] = {0.5, 1.0, 1.5, 2.0, 2.5};
  const string ATLAS_2012_I1093734::ETABINNAMES[] = { "05", "10", "15", "20", "25" };



  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1093734);

}

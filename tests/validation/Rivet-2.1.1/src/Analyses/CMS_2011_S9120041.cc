// -*- C++ -*-

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

using namespace std;

namespace Rivet {


  // UE charged particles vs. leading jet
  class CMS_2011_S9120041 : public Analysis {
  public:

    /// Constructor
    CMS_2011_S9120041() : Analysis("CMS_2011_S9120041") {}


    void init() {
      const ChargedFinalState cfs(-2.0, 2.0, 500*MeV);
      addProjection(cfs, "CFS");

      const ChargedFinalState cfsforjet(-2.5, 2.5, 500*MeV);
      const FastJets jetpro(cfsforjet, FastJets::SISCONE, 0.5);
      addProjection(jetpro, "Jets");

      if (fuzzyEquals(sqrtS(), 7.0*TeV)) {
        _h_Nch_vs_pT = bookProfile1D(1, 1, 1); // Nch vs. pT_max
        _h_Sum_vs_pT = bookProfile1D(2, 1, 1); // sum(pT) vs. pT_max
        _h_pT3_Nch   = bookHisto1D(5, 1, 1);   // transverse Nch,     pT_max > 3GeV
        _h_pT3_Sum   = bookHisto1D(6, 1, 1);   // transverse sum(pT), pT_max > 3GeV
        _h_pT3_pT    = bookHisto1D(7, 1, 1);   // transverse pT,      pT_max > 3GeV
        _h_pT20_Nch  = bookHisto1D(8, 1, 1);   // transverse Nch,     pT_max > 20GeV
        _h_pT20_Sum  = bookHisto1D(9, 1, 1);   // transverse sum(pT), pT_max > 20GeV
        _h_pT20_pT   = bookHisto1D(10, 1, 1);  // transverse pT,      pT_max > 20GeV
      }

      if (fuzzyEquals(sqrtS(), 0.9*TeV)) {
        _h_Nch_vs_pT = bookProfile1D(3, 1, 1); // Nch vs. pT_max
        _h_Sum_vs_pT = bookProfile1D(4, 1, 1); // sum(pT) vs. pT_max
        _h_pT3_Nch   = bookHisto1D(11, 1, 1);  // transverse Nch,     pT_max > 3GeV
        _h_pT3_Sum   = bookHisto1D(12, 1, 1);  // transverse sum(pT), pT_max > 3GeV
        _h_pT3_pT    = bookHisto1D(13, 1, 1);  // transverse pT,      pT_max > 3GeV
      }

      sumOfWeights3  = 0.0;
      sumOfWeights20 = 0.0;

      _nch_tot_pT3  = 0.0;
      _nch_tot_pT20 = 0.0;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Find the lead jet, applying a restriction that the jets must be within |eta| < 2.
      FourMomentum p_lead;
      foreach (const Jet& j, applyProjection<FastJets>(event, "Jets").jetsByPt(1.0*GeV)) {
        if (fabs(j.eta()) < 2.0) {
          p_lead = j.momentum();
          break;
        }
      }
      if (p_lead.isZero()) vetoEvent;
      const double philead = p_lead.phi();
      const double pTlead  = p_lead.pT();

      Particles particles = applyProjection<ChargedFinalState>(event, "CFS").particlesByPt();

      int nTransverse = 0;
      double ptSumTransverse = 0.;
      foreach (const Particle& p, particles) {
        double dphi = fabs(deltaPhi(philead, p.momentum().phi()));
        if (dphi>PI/3. && dphi<PI*2./3.) {   // Transverse region
          nTransverse++;

          const double pT = p.pT()/GeV;
          ptSumTransverse += pT;

          if (pTlead > 3.0*GeV) _h_pT3_pT->fill(pT, weight);
          if (fuzzyEquals(sqrtS(), 7.0*TeV) && pTlead > 20.0*GeV) _h_pT20_pT->fill(pT, weight);
        }
      }

      const double area = 8./3. * PI;
      _h_Nch_vs_pT->fill(pTlead/GeV, 1./area*nTransverse, weight);
      _h_Sum_vs_pT->fill(pTlead/GeV, 1./area*ptSumTransverse, weight);
      if(pTlead > 3.0*GeV) {
        _h_pT3_Nch->fill(nTransverse, weight);
        _h_pT3_Sum->fill(ptSumTransverse, weight);
        sumOfWeights3 += weight;
        _nch_tot_pT3  += weight*nTransverse;
      }
      if (fuzzyEquals(sqrtS(), 7.0*TeV) && pTlead > 20.0*GeV) {
        _h_pT20_Nch->fill(nTransverse, weight);
        _h_pT20_Sum->fill(ptSumTransverse, weight);
        sumOfWeights20 += weight;
        _nch_tot_pT20  += weight*nTransverse;
      }
    }



    /// Normalise histograms etc., after the run
    void finalize() {
      normalize(_h_pT3_Nch);
      normalize(_h_pT3_Sum);
      if (sumOfWeights3 != 0.0) normalize(_h_pT3_pT, _nch_tot_pT3 / sumOfWeights3);

      if (fuzzyEquals(sqrtS(), 7.0*TeV)) {
        normalize(_h_pT20_Nch);
        normalize(_h_pT20_Sum);
        if (sumOfWeights20 != 0.0) normalize(_h_pT20_pT, _nch_tot_pT20 / sumOfWeights20);
      }
    }



  private:

    double sumOfWeights3;
    double sumOfWeights20;

    double _nch_tot_pT3;
    double _nch_tot_pT20;

    Profile1DPtr _h_Nch_vs_pT;
    Profile1DPtr _h_Sum_vs_pT;
    Histo1DPtr _h_pT3_Nch;
    Histo1DPtr _h_pT3_Sum;
    Histo1DPtr _h_pT3_pT;
    Histo1DPtr _h_pT20_Nch;
    Histo1DPtr _h_pT20_Sum;
    Histo1DPtr _h_pT20_pT;

  };


  // This global object acts as a hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S9120041);
}


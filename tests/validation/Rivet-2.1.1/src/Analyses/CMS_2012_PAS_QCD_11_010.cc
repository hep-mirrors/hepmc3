// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {

  class CMS_2012_PAS_QCD_11_010 : public Analysis {
  public:

    CMS_2012_PAS_QCD_11_010()
      : Analysis("CMS_2012_PAS_QCD_11_010")
    {  }

    void init() {
      const FastJets jets(ChargedFinalState(-2.5, 2.5, 0.5*GeV), FastJets::ANTIKT, 0.5);
      addProjection(jets, "Jets");

      const UnstableFinalState ufs(-2.0, 2.0, 0.6*GeV);
      addProjection(ufs, "UFS");

      _h_nTrans_Lambda     = bookProfile1D(1, 1, 1);
      _h_nTrans_Kaon       = bookProfile1D(2, 1, 1);
      _h_ptsumTrans_Lambda = bookProfile1D(3, 1, 1);
      _h_ptsumTrans_Kaon   = bookProfile1D(4, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByPt(1.0*GeV);
      if (jets.size() < 1) vetoEvent;

      if (fabs(jets[0].eta()) >= 2) { // cuts on leading jets
        vetoEvent;
      }

      FourMomentum p_lead = jets[0].momentum();
      const double pTlead  = p_lead.pT();

      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");

      int numTrans_Kaon = 0;
      int numTrans_Lambda = 0;
      double ptSumTrans_Kaon = 0.;
      double ptSumTrans_Lambda = 0.;

      foreach (const Particle& p, ufs.particles()) {
        double dphi = deltaPhi(p, p_lead);
        double pT = p.pT();
        const PdgId id = abs(p.pdgId());

        if (dphi > PI/3. && dphi < 2./3.*PI) {
          if (id == 310 && pT > 0.6*GeV) {
            ptSumTrans_Kaon += pT/GeV;
            numTrans_Kaon++;
          }
          else if (id == 3122 && pT > 1.5*GeV) {
            ptSumTrans_Lambda += pT/GeV;
            numTrans_Lambda++;
          }
        }
      }

      _h_nTrans_Kaon->fill(pTlead/GeV, numTrans_Kaon / (8.0 * PI/3.0), weight);
      _h_nTrans_Lambda->fill(pTlead/GeV, numTrans_Lambda / (8.0 * PI/3.0), weight);
      _h_ptsumTrans_Kaon->fill(pTlead/GeV, ptSumTrans_Kaon / (GeV * (8.0 * PI/3.0)), weight);
      _h_ptsumTrans_Lambda->fill(pTlead/GeV, ptSumTrans_Lambda / (GeV * (8.0 * PI/3.0)), weight);
    }


    void finalize() { }

  private:

    Profile1DPtr _h_nTrans_Kaon;
    Profile1DPtr _h_nTrans_Lambda;
    Profile1DPtr _h_ptsumTrans_Kaon;
    Profile1DPtr _h_ptsumTrans_Lambda;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2012_PAS_QCD_11_010);

}

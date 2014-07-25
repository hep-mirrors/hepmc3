// -*- C++ -*-

#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief ATLAS inclusive jet pT spectrum, di-jet Mass and di-jet chi
  class ATLAS_2010_S8817804: public Analysis {
  public:

    ATLAS_2010_S8817804() : Analysis("ATLAS_2010_S8817804")
    {    }


  private:

    enum Alg { AKT4=0, AKT6=1 };


  public:

    void init() {
      FinalState fs;
      addProjection(fs, "FinalState");
      addProjection(FastJets(fs, FastJets::ANTIKT, 0.6), "AntiKT06");
      addProjection(FastJets(fs, FastJets::ANTIKT, 0.4), "AntiKT04");

      double ybins[] = { 0.0, 0.3, 0.8, 1.2, 2.1, 2.8 };
      double massBinsForChi[] = { 340, 520, 800, 1200 };


      size_t ptDsOffset(0), massDsOffset(10), chiDsOffset(20);
      for (size_t alg = 0; alg < 2; ++alg) {
        for (size_t i = 0; i < 5; ++i) {
          _pThistos[alg].addHistogram(ybins[i], ybins[i+1], bookHisto1D(i + 1 + ptDsOffset, 1, 1));
        }
        ptDsOffset += 5;

        for (size_t i = 0; i < 5; ++i) {
          _massVsY[alg].addHistogram(ybins[i], ybins[i+1], bookHisto1D(i + 1 + massDsOffset, 1, 1));
        }
        massDsOffset += 5;

        for (size_t i = 0; i < 3; ++i) {
          _chiVsMass[alg].addHistogram(massBinsForChi[i], massBinsForChi[i+1], bookHisto1D(i + 1 + chiDsOffset, 1, 1));
        }
        chiDsOffset += 3;
      }
    }


    void analyze(const Event& evt) {
      Jets jetAr[2];
      jetAr[AKT6] = applyProjection<FastJets>(evt, "AntiKT06").jetsByPt(30*GeV);
      jetAr[AKT4] = applyProjection<FastJets>(evt, "AntiKT04").jetsByPt(30*GeV);

      // Identify the dijets
      for (size_t alg = 0; alg < 2; ++alg) {
        vector<FourMomentum> leadjets;
        foreach (const Jet& jet, jetAr[alg]) {
          const double pT = jet.pT();
          const double absy = fabs(jet.rapidity());
          _pThistos[alg].fill(absy, pT/GeV, evt.weight());

          if (absy < 2.8 && leadjets.size() < 2) {
            if (leadjets.empty() && pT < 60*GeV) continue;
            leadjets.push_back(jet.momentum());
          }
        }

        // Veto if no acceptable dijet found
        if (leadjets.size() < 2) {
          MSG_DEBUG("Could not find two suitable leading jets");
          continue;
        }

        const double rap1 = leadjets[0].rapidity();
        const double rap2 = leadjets[1].rapidity();
        const double mass = (leadjets[0] + leadjets[1]).mass();
        const double ymax = max(fabs(rap1), fabs(rap2));
        const double chi = exp(fabs(rap1 - rap2));
        if (fabs(rap1 + rap2) < 2.2) {
          _chiVsMass[alg].fill(mass/GeV, chi, evt.weight());
        }
        _massVsY[alg].fill(ymax, mass/GeV, evt.weight());

      }
    }


    void finalize() {
      for (size_t alg = 0; alg < 2; ++alg) {
        // factor 0.5 needed because it is differential in dy and not d|y|
        _pThistos[alg].scale(0.5*crossSectionPerEvent()/picobarn, this);
        _massVsY[alg].scale(crossSectionPerEvent()/picobarn, this);
        _chiVsMass[alg].scale(crossSectionPerEvent()/picobarn, this);
      }
    }


  private:

    /// The inclusive pT spectrum for akt6 and akt4 jets (array index is jet type from enum above)
    BinnedHistogram<double> _pThistos[2];

    /// The di-jet mass spectrum binned in rapidity for akt6 and akt4 jets (array index is jet type from enum above)
    BinnedHistogram<double> _massVsY[2];

    /// The di-jet chi distribution binned in mass for akt6 and akt4 jets (array index is jet type from enum above)
    BinnedHistogram<double> _chiVsMass[2];

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2010_S8817804);

}

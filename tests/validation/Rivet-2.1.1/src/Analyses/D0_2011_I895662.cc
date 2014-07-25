// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  class D0_2011_I895662 : public Analysis {
  public:

    D0_2011_I895662()
      : Analysis("D0_2011_I895662")
    {    }


  public:

    void init() {
      FastJets jets(FinalState(-3.6, 3.6, 0.*GeV), FastJets::D0ILCONE, 0.7);
      jets.useInvisibles();
      addProjection(jets, "Jets");

      _h_m3j_08_40  = bookHisto1D(1, 1, 1);
      _h_m3j_16_40  = bookHisto1D(2, 1, 1);
      _h_m3j_24_40  = bookHisto1D(3, 1, 1);
      _h_m3j_24_70  = bookHisto1D(4, 1, 1);
      _h_m3j_24_100 = bookHisto1D(5, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByPt(40.*GeV);

      // Need three jets, leading jet above 150 GeV
      if (jets.size() < 3 || jets[0].pT() <= 150.*GeV) vetoEvent;

      std::vector<FourMomentum> p;
      for (size_t i=0; i<3; i++) {
        p.push_back(jets[i].momentum());
      }

      // Jets need to be separated by 2*Rcone
      if (deltaR(p[0], p[1], RAPIDITY) < 1.4 ||
          deltaR(p[0], p[2], RAPIDITY) < 1.4 ||
          deltaR(p[1], p[2], RAPIDITY) < 1.4)
        vetoEvent;

      // Leading three jets need to be within |y|<2.4
      double ymax = fabs(p[0].rapidity());
      for (size_t i=1; i<3; i++) {
        if (ymax < fabs(p[i].rapidity())) ymax = fabs(p[i].rapidity());
      }
      if (ymax >= 2.4) vetoEvent;

      double m3jet = (p[0]+p[1]+p[2]).mass()/GeV;

      if (ymax < 0.8) _h_m3j_08_40->fill(m3jet, weight);
      if (ymax < 1.6) _h_m3j_16_40->fill(m3jet, weight);
      if (ymax < 2.4) {
        _h_m3j_24_40->fill(m3jet, weight);
        if (p[2].pT() > 70.*GeV)  _h_m3j_24_70->fill(m3jet, weight);
        if (p[2].pT() > 100.*GeV) _h_m3j_24_100->fill(m3jet, weight);
      }

    }


    void finalize() {
      // Factor of 1000 is based on GeV <-> TeV mismatch between paper and Hepdata table
      scale(_h_m3j_08_40,  1000*crossSection()/picobarn/sumOfWeights());
      scale(_h_m3j_16_40,  1000*crossSection()/picobarn/sumOfWeights());
      scale(_h_m3j_24_40,  1000*crossSection()/picobarn/sumOfWeights());
      scale(_h_m3j_24_70,  1000*crossSection()/picobarn/sumOfWeights());
      scale(_h_m3j_24_100, 1000*crossSection()/picobarn/sumOfWeights());
    }


  private:

    Histo1DPtr _h_m3j_08_40;
    Histo1DPtr _h_m3j_16_40;
    Histo1DPtr _h_m3j_24_40;
    Histo1DPtr _h_m3j_24_70;
    Histo1DPtr _h_m3j_24_100;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2011_I895662);

}

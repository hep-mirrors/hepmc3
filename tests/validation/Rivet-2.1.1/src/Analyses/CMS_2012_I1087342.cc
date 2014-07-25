// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"


namespace Rivet {

  // This analysis is a derived from the class Analysis:
  class CMS_2012_I1087342 : public Analysis {

  public:

    // Constructor
    CMS_2012_I1087342() : Analysis("CMS_2012_I1087342") {
    }

    void init() {
      const FinalState fs;
      addProjection(FastJets(fs, FastJets::ANTIKT, 0.5),"Jets");

      _hist_jetpt_fwdincl = bookHisto1D(1,1,1);
      _hist_jetpt_forward = bookHisto1D(2,1,1);
      _hist_jetpt_central = bookHisto1D(3,1,1);
    }

    void analyze(const Event& event) {
      const double weight = event.weight();

      const FastJets& fj = applyProjection<FastJets>(event,"Jets");
      const Jets jets = fj.jets(35*GeV, 150*GeV, -4.7, 4.7, ETARAP);

      double cjet_pt = 0.0;
      double fjet_pt = 0.0;

      foreach(const Jet& j, jets) {
        double pT = j.pT();
        if (j.eta() > 3.2 || j.eta() < -3.2) {
          _hist_jetpt_fwdincl->fill(j.pT()/GeV, weight);
        }
        if (fabs(j.eta()) < 2.8) {
          if (cjet_pt < pT) cjet_pt = pT;
        }
        if (fabs(j.eta()) < 4.7  && fabs(j.eta()) > 3.2) {
          if (fjet_pt < pT) fjet_pt = pT;
        }
      }

      if (cjet_pt > 35*GeV && fjet_pt > 35*GeV) {
        _hist_jetpt_forward->fill(fjet_pt/GeV, weight);
        _hist_jetpt_central->fill(cjet_pt/GeV, weight);
      }

    }

    void finalize() {
      scale(_hist_jetpt_fwdincl, crossSection() / picobarn / sumOfWeights() / 3.0);
      scale(_hist_jetpt_forward, crossSection() / picobarn / sumOfWeights() / 3.0);
      scale(_hist_jetpt_central, crossSection() / picobarn / sumOfWeights() / 5.6);
    }


  private:

    Histo1DPtr _hist_jetpt_fwdincl;
    Histo1DPtr _hist_jetpt_forward;
    Histo1DPtr _hist_jetpt_central;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2012_I1087342);

}


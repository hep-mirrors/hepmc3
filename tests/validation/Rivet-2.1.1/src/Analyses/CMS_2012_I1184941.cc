// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  class CMS_2012_I1184941 : public Analysis {
  public:

    CMS_2012_I1184941()
      : Analysis("CMS_2012_I1184941")
    {   }

  public:

    void init() {
      FinalState fs;
      addProjection(fs, "FS");

      const FastJets jets(FinalState(-4.9, 4.9, 0.0*GeV), FastJets::ANTIKT, 0.5);
      addProjection(jets, "AntiKtJets05");

      _h_xi = bookHisto1D(1, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      double xiM = 0.;
      double xiP = 0.;

      const Jets jets = applyProjection<FastJets>(event, "AntiKtJets05").jetsByPt(20.*GeV);
      if (jets.size() < 2) vetoEvent;  // require a dijet system with a 20 GeV cut on both jets
      if (fabs(jets[0].eta()) > 4.4 || fabs(jets[1].eta()) > 4.4) vetoEvent;

      const FinalState& fsp = applyProjection<FinalState>(event, "FS");

      foreach (const Particle& p, fsp.particlesByEta()) {
        double eta = p.eta();
        double energy = p.momentum().E();
        double costheta = cos(p.momentum().theta());

        // Yes, they really correct to +/- infinity, using Pythia 8 ...
        if ( eta < 4.9 )  xiP += (energy + energy*costheta);
        if ( -4.9 < eta ) xiM += (energy - energy*costheta);
      }

      xiP = xiP / (sqrtS()/GeV);
      xiM = xiM / (sqrtS()/GeV);

      _h_xi->fill( xiM, weight ); // Fill the histogram both with xiP and xiM, and get the average in the endjob.
      _h_xi->fill( xiP, weight );

    }


    void finalize() {
      scale( _h_xi, crossSection()/microbarn/sumOfWeights() / 2.);
    }

  private:

    Histo1DPtr _h_xi;

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2012_I1184941);

}

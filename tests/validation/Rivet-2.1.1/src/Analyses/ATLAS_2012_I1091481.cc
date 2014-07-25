// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include <iostream>
#include <fstream>

namespace Rivet {


  class ATLAS_2012_I1091481 : public Analysis {
  public:

    /// Constructor
    ATLAS_2012_I1091481()
      : Analysis("ATLAS_2012_I1091481")
    {
    }


  public:

    /// Book histograms and initialise projections before the run
    void init() {

      /// @todo Initialise and register projections here
      ChargedFinalState cfs100(-2.5, 2.5, 0.1*GeV);
      addProjection(cfs100,"CFS100");
      ChargedFinalState cfs500(-2.5, 2.5, 0.5*GeV);
      addProjection(cfs500,"CFS500");


      // collision energy
      int isqrts = -1;
      if (fuzzyEquals(sqrtS(), 900*GeV)) isqrts = 2;
      else if (fuzzyEquals(sqrtS(), 7*TeV)) isqrts = 1;
      assert(isqrts >= 0);

      _sE_10_100   = bookHisto1D(isqrts, 1, 1);
      _sE_1_100    = bookHisto1D(isqrts, 1, 2);
      _sE_10_500   = bookHisto1D(isqrts, 1, 3);

      _sEta_10_100 = bookHisto1D(isqrts, 2, 1);
      _sEta_1_100  = bookHisto1D(isqrts, 2, 2);
      _sEta_10_500 = bookHisto1D(isqrts, 2, 3);
    }

    // Recalculate particle energy assuming pion mass
    double getPionEnergy(const Particle& p) {
      double m_pi = 0.1396*GeV;
      double p2  = p.momentum().vector3().mod2()/(GeV*GeV);
      return sqrt(pow(m_pi,2) + p2);
    }

    // S_eta core for one event
    //
    //  -1 + 1/Nch * |sum_j^Nch exp[i*(xi eta_j - Phi_j)]|^2
    //
    double getSeta(const Particles& part, double xi) {
      std::complex<double> c_eta (0.0, 0.0);
      foreach (const Particle& p, part) {
        double eta = p.eta();
        double phi = p.momentum().phi();
        double arg = xi*eta-phi;
         std::complex<double> temp(cos(arg), sin(arg));
         c_eta += temp;
      }
      // Not 100% sure about the -1 here
      return std::norm(c_eta)/part.size() - 1.0;
    }

    // S_E core for one event
    //
    //  -1 + 1/Nch * |sum_j^Nch exp[i*(omega X_j - Phi_j)]|^2
    double getSE(const Particles& part, double omega) {
      double Xj = 0.0;
      std::complex<double> c_E (0.0, 0.0);
      for (unsigned int i=0; i<part.size(); i++) {
        Xj += 0.5*getPionEnergy(part[i]);
        double phi = part[i].momentum().phi();
        double arg = omega*Xj - phi;
         std::complex<double> temp(cos(arg), sin(arg));
         c_E += temp;
        Xj += 0.5*getPionEnergy(part[i]);
      }
      // Not 100% sure about the -1 here
      return std::norm(c_E)/part.size() - 1.0;
    }

    // Convenient fill function
    void fillS(Histo1DPtr h, const Particles& part, double weight, bool SE=true) {
      // Loop over bins, take bin centers as parameter values
      for (size_t i=0; i< h->numBins(); i++) {
        double x = h->bin(i).midpoint();
        double y;
        if (SE) y = getSE(part, x);
        else    y = getSeta(part, x);
        h->fill(x, y*weight);
      }
    }

    /// Perform the per-event analysis
    void analyze(const Event& event) {
      double weight = event.weight();

      // Charged fs
      const ChargedFinalState& cfs100 = applyProjection<ChargedFinalState>(event, "CFS100");
      const Particles    part100 = cfs100.particlesByEta();
      const ChargedFinalState& cfs500 = applyProjection<ChargedFinalState>(event, "CFS500");
      const Particles&   part500 = cfs500.particlesByEta();

      // Veto event if the most inclusive phase space has less than 10 particles and the max pT is > 10 GeV
      if (part100.size() < 11) vetoEvent;
      double ptmax = cfs100.particlesByPt()[0].pT()/GeV;
      if (ptmax > 10.0) vetoEvent;

      // Fill the pt>100, pTmax<10 GeV histos
      fillS(_sE_10_100, part100, weight, true);
      fillS(_sEta_10_100, part100, weight, false);

      // Fill the pt>100, pTmax<1 GeV histos
      if (ptmax < 1.0) {
        fillS(_sE_1_100,   part100, weight, true);
        fillS(_sEta_1_100, part100, weight, false);
      }

      // Fill the pt>500, pTmax<10 GeV histos
      if (part500.size() > 10) {
        fillS(_sE_10_500, part500, weight, true);
        fillS(_sEta_10_500, part500, weight, false);
      }
    }

    /// Normalise histograms etc., after the run
    void finalize() {
      // The scaling takes the multiple fills per event into account
      // --- not sure about the normalisation
      scale(_sE_10_100, 1.0/(sumOfWeights()*_sE_10_100->numBins()));
      scale(_sE_1_100 , 1.0/(sumOfWeights()*_sE_1_100 ->numBins()));
      scale(_sE_10_500, 1.0/(sumOfWeights()*_sE_10_500->numBins()));

      scale(_sEta_10_100, 1.0/(sumOfWeights()*_sEta_10_100->numBins()));
      scale(_sEta_1_100 , 1.0/(sumOfWeights()*_sEta_1_100 ->numBins()));
      scale(_sEta_10_500, 1.0/(sumOfWeights()*_sEta_10_500->numBins()));
    }

    //@}

  private:

    Histo1DPtr _sE_10_100;
    Histo1DPtr _sE_1_100;
    Histo1DPtr _sE_10_500;

    Histo1DPtr _sEta_10_100;
    Histo1DPtr _sEta_1_100;
    Histo1DPtr _sEta_10_500;
  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1091481);
}

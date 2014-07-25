// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Particle.hh"

namespace Rivet {


  class CMS_2011_S8941262 : public Analysis {
  public:

    /// Constructor
    CMS_2011_S8941262() : Analysis("CMS_2011_S8941262") {  }


    /// Book histograms and initialise projections before the run
    void init() {
      _h_total = bookHisto1D(1, 1, 1);
      _h_mupt  = bookHisto1D(2, 1, 1);
      _h_mueta = bookHisto1D(3, 1, 1);
      nbtot=0.;   nbmutot=0.;

      IdentifiedFinalState ifs(-2.1, 2.1, 6.0*GeV);
      ifs.acceptIdPair(PID::MUON);
      addProjection(ifs, "IFS");
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // a b-quark must have been produced
      int nb = 0;
      foreach (const GenParticlePtr p, particles(event.genEvent())) {
        if (abs(p->pdg_id()) == PID::BQUARK) nb += 1;
      }
      if (nb == 0) vetoEvent;
      nbtot += weight;

      // Event must contain a muon
      Particles muons = applyProjection<IdentifiedFinalState>(event, "IFS").particlesByPt();
      if (muons.size() < 1) vetoEvent;
      nbmutot += weight;

      FourMomentum pmu = muons[0].momentum();
      _h_total->fill(      7000/GeV, weight);
      _h_mupt->fill(   pmu.pT()/GeV, weight);
      _h_mueta->fill( pmu.eta()/GeV, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_total, crossSection()/microbarn/sumOfWeights());
      scale(_h_mupt,  crossSection()/nanobarn/sumOfWeights());
      scale(_h_mueta, crossSection()/nanobarn/sumOfWeights());
    }


  private:

    double nbtot, nbmutot;

    Histo1DPtr _h_total;
    Histo1DPtr _h_mupt;
    Histo1DPtr _h_mueta;

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S8941262);

}

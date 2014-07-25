// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {

  class ALICE_2012_I1181770 : public Analysis {
  public:

    ALICE_2012_I1181770()
      : Analysis("ALICE_2012_I1181770")
    {    }


    void init() {
      // Projection setup
      addProjection(ChargedFinalState(), "CFS");

      // Book (energy-specific) histograms
      int isqrts = -1;
      if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) isqrts = 1;
      else if (fuzzyEquals(sqrtS()/GeV, 2760, 1E-3)) isqrts = 2;
      else if (fuzzyEquals(sqrtS()/GeV, 7000, 1E-3)) isqrts = 3;
      assert(isqrts > 0);

      _h_frac_sd_inel = bookScatter2D(1, 1, isqrts);
      _h_frac_dd_inel = bookScatter2D(2, 1, isqrts);
      _h_xsec_sd      = bookHisto1D  (3, 1, isqrts);
      _h_xsec_dd      = bookHisto1D  (4, 1, isqrts);
      _h_xsec_inel    = bookHisto1D  (5, 1, isqrts);
    }


    void analyze(const Event& event) {
      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");
      if (cfs.size() < 2) vetoEvent; // need at least two particles to calculate gaps

      const double weight = event.weight();

      // Fill INEL plots for each event
      _h_xsec_inel->fill(sqrtS()/GeV, weight);

      // Identify particles with most positive/most negative rapidities
      const Particles particlesByRap = cfs.particlesByRapidity();
      const Particle pslowest = particlesByRap.front();
      const Particle pfastest = particlesByRap.back();

      // Find gap sizes
      const Particles particlesByEta = cfs.particlesByEta(); // sorted from minus to plus
      const size_t num_particles = particlesByEta.size();
      vector<double> gaps;
      for (size_t ip = 1; ip < num_particles; ++ip) {
        const Particle& p1 = particlesByEta[ip-1];
        const Particle& p2 = particlesByEta[ip];
        const double gap = p2.eta() - p1.eta();
        assert(gap >= 0);
        gaps.push_back(gap);
      }

      // First, last, and largest gaps
      const double gapmax = *max_element(gaps.begin(), gaps.end());
      const double gapbwd = gaps.front();
      const double gapfwd = gaps.back();

      // Mx calculation
      FourMomentum p4lead;
      if (pslowest.pdgId() == PID::PROTON && pfastest.pdgId() == PID::PROTON) {
        p4lead = (fabs(pslowest.rapidity()) > fabs(pfastest.rapidity())) ? pslowest.momentum() : pfastest.momentum();
      } else if (pslowest.pdgId() == PID::PROTON) {
        p4lead = pslowest.momentum();
      } else if (pfastest.pdgId() == PID::PROTON) {
        p4lead = pfastest.momentum();
      }
      const double Mx = sqrt( (sqrtS()-p4lead.E()-p4lead.vector3().mod()) * (sqrtS()-p4lead.E()+p4lead.vector3().mod()) );

      // Fill SD (and escape) if Mx is sufficiently low
      if (Mx < 200*GeV) {
        _h_xsec_sd->fill(sqrtS()/GeV, weight);
        return;
      }

      // Also remove SD-like events in NSD events
      if (fuzzyEquals(gapbwd, gapmax) || fuzzyEquals(gapfwd, gapmax)) vetoEvent;

      // Fill DD plots
      if (gapmax > 3) _h_xsec_dd->fill(sqrtS()/GeV, weight);
    }


    void finalize() {

      // get the ratio plots: SD/inel, DD/inel
      divide(_h_xsec_sd , _h_xsec_inel, _h_frac_sd_inel);
      divide(_h_xsec_sd , _h_xsec_inel, _h_frac_dd_inel);

      scale(_h_xsec_sd,   crossSection()/millibarn/sumOfWeights());
      scale(_h_xsec_dd,   crossSection()/millibarn/sumOfWeights());
      scale(_h_xsec_inel, crossSection()/millibarn/sumOfWeights());

    }

  private:

    Scatter2DPtr _h_frac_sd_inel;
    Scatter2DPtr _h_frac_dd_inel;
    Histo1DPtr   _h_xsec_sd;
    Histo1DPtr   _h_xsec_dd;
    Histo1DPtr   _h_xsec_inel;

  };

  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2012_I1181770);

}

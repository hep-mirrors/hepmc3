// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @author Jinlong Zhang
  class ATLAS_2011_S8994773 : public Analysis {
  public:

    ATLAS_2011_S8994773()
      : Analysis("ATLAS_2011_S8994773") {    }


    void init() {
      const FinalState fs500(-2.5, 2.5, 500*MeV);
      addProjection(fs500, "FS500");
      const FinalState fslead(-2.5, 2.5, 1.0*GeV);
      addProjection(fslead, "FSlead");

      // Get an index for the beam energy
      isqrts = -1;
      if (fuzzyEquals(sqrtS(), 900*GeV)) isqrts = 0;
      else if (fuzzyEquals(sqrtS(), 7*TeV)) isqrts = 1;
      assert(isqrts >= 0);

      // N profiles, 500 MeV pT cut
      _hist_N_transverse_500 = bookProfile1D(1+isqrts, 1, 1);
      // pTsum profiles, 500 MeV pT cut
      _hist_ptsum_transverse_500 = bookProfile1D(3+isqrts, 1, 1);
      // N vs. Delta(phi) profiles, 500 MeV pT cut
      _hist_N_vs_dPhi_1_500 = bookProfile1D(13+isqrts, 1, 1);
      _hist_N_vs_dPhi_2_500 = bookProfile1D(13+isqrts, 1, 2);
      _hist_N_vs_dPhi_3_500 = bookProfile1D(13+isqrts, 1, 3);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      // Require at least one cluster in the event with pT >= 1 GeV
      const FinalState& fslead = applyProjection<FinalState>(event, "FSlead");
      if (fslead.size() < 1) {
        vetoEvent;
      }

      // These are the particles  with pT > 500 MeV
      const FinalState& chargedNeutral500 = applyProjection<FinalState>(event, "FS500");

      // Identify leading object and its phi and pT
      Particles particles500 = chargedNeutral500.particlesByPt();
      Particle p_lead = particles500[0];
      const double philead = p_lead.momentum().phi();
      const double etalead = p_lead.eta();
      const double pTlead  = p_lead.pT();
      MSG_DEBUG("Leading object: pT = " << pTlead << ", eta = " << etalead << ", phi = " << philead);

      // Iterate over all > 500 MeV particles and count particles and scalar pTsum in the three regions
      vector<double> num500(3, 0), ptSum500(3, 0.0);
      // Temporary histos that bin N in dPhi.
      // NB. Only one of each needed since binnings are the same for the energies and pT cuts
      Histo1D hist_num_dphi_500(refData(13+isqrts,1,1));
      foreach (const Particle& p, particles500) {
        const double pT = p.pT();
        const double dPhi = deltaPhi(philead, p.momentum().phi());
        const int ir = region_index(dPhi);
        num500[ir] += 1;
        ptSum500[ir] += pT;

        // Fill temp histos to bin N in dPhi
        if (p.genParticle() != p_lead.genParticle()) { // We don't want to fill all those zeros from the leading track...
          hist_num_dphi_500.fill(dPhi, 1);
        }
      }


      // Now fill underlying event histograms
      // The densities are calculated by dividing the UE properties by dEta*dPhi
      // -- each region has a dPhi of 2*PI/3 and dEta is two times 2.5
      const double dEtadPhi = (2*2.5 * 2*PI/3.0);
      _hist_N_transverse_500->fill(pTlead/GeV,  num500[1]/dEtadPhi, weight);
      _hist_ptsum_transverse_500->fill(pTlead/GeV, ptSum500[1]/GeV/dEtadPhi, weight);

      // Update the "proper" dphi profile histograms
      // Note that we fill dN/dEtadPhi: dEta = 2*2.5, dPhi = 2*PI/nBins
      // The values tabulated in the note are for an (undefined) signed Delta(phi) rather than
      // |Delta(phi)| and so differ by a factor of 2: we have to actually norm for angular range = 2pi
      const size_t nbins = refData(13+isqrts,1,1).numPoints();
      for (size_t i = 0; i < nbins; ++i) {
        double mean = hist_num_dphi_500.bin(i).midpoint();
        double value = 0.;
        if (hist_num_dphi_500.bin(i).numEntries() > 0) {
          mean = hist_num_dphi_500.bin(i).xMean();
          value = hist_num_dphi_500.bin(i).area()/hist_num_dphi_500.bin(i).width()/10.0;
        }
        if (pTlead/GeV >= 1.0) _hist_N_vs_dPhi_1_500->fill(mean, value, weight);
        if (pTlead/GeV >= 2.0) _hist_N_vs_dPhi_2_500->fill(mean, value, weight);
        if (pTlead/GeV >= 3.0) _hist_N_vs_dPhi_3_500->fill(mean, value, weight);
      }

    }


    void finalize() {
    }


  private:

    // Little helper function to identify Delta(phi) regions
    inline int region_index(double dphi) {
      assert(inRange(dphi, 0.0, PI, CLOSED, CLOSED));
      if (dphi < PI/3.0) return 0;
      if (dphi < 2*PI/3.0) return 1;
      return 2;
    }


  private:
    int isqrts;

    Profile1DPtr _hist_N_transverse_500;

    Profile1DPtr _hist_ptsum_transverse_500;

    Profile1DPtr _hist_N_vs_dPhi_1_500;
    Profile1DPtr _hist_N_vs_dPhi_2_500;
    Profile1DPtr _hist_N_vs_dPhi_3_500;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S8994773);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class ATLAS_2010_S8894728 : public Analysis {
  public:

    ATLAS_2010_S8894728() : Analysis("ATLAS_2010_S8894728") {    }


    void init() {
      const ChargedFinalState cfs100(-2.5, 2.5, 100*MeV);
      addProjection(cfs100, "CFS100");
      const ChargedFinalState cfs500(-2.5, 2.5, 500*MeV);
      addProjection(cfs500, "CFS500");
      const ChargedFinalState cfslead(-2.5, 2.5, 1.0*GeV);
      addProjection(cfslead, "CFSlead");

      // Get an index for the beam energy
      int isqrts = -1;
      if (fuzzyEquals(sqrtS(), 900*GeV)) isqrts = 0;
      else if (fuzzyEquals(sqrtS(), 7*TeV)) isqrts = 1;
      assert(isqrts >= 0);

      // Nch profiles, 500 MeV track pT cut
      _hist_nch_transverse_500[0] = bookProfile1D(1+isqrts, 1, 1);
      _hist_nch_toward_500        = bookProfile1D(1+isqrts, 1, 2);
      _hist_nch_away_500          = bookProfile1D(1+isqrts, 1, 3);

      // pTsum profiles, 500 MeV track pT cut
      _hist_ptsum_transverse_500[0] = bookProfile1D(3+isqrts, 1, 1);
      _hist_ptsum_toward_500        = bookProfile1D(3+isqrts, 1, 2);
      _hist_ptsum_away_500          = bookProfile1D(3+isqrts, 1, 3);

      // Standard deviation profiles
      // First the higher moments of main profiles to calculate variance and error on variance...
      for (size_t i = 1; i < 4; ++i) {
        _hist_nch_transverse_500[i].reset(new Profile1D(refData(1+isqrts, 1, 1)));
        _hist_ptsum_transverse_500[i].reset(new Profile1D(refData(3+isqrts, 1, 1)));
      }
      // Then the data point sets into which the results will be inserted
      _dps_sdnch_transverse_500   = bookScatter2D(5+isqrts, 1, 1);
      _dps_sdptsum_transverse_500 = bookScatter2D(7+isqrts, 1, 1);

      // <pT> profiles, 500 MeV track pT cut
      _hist_ptavg_transverse_500 = bookProfile1D(9+isqrts, 1, 1);
      _hist_ptavg_toward_500     = bookProfile1D(9+isqrts, 1, 2);
      _hist_ptavg_away_500       = bookProfile1D(9+isqrts, 1, 3);

      // <pT> vs. Nch profiles, 500 MeV track pT cut
      _hist_dn_dpt_transverse_500 = bookProfile1D(11+isqrts, 1, 1);
      _hist_dn_dpt_toward_500     = bookProfile1D(11+isqrts, 1, 2);
      _hist_dn_dpt_away_500       = bookProfile1D(11+isqrts, 1, 3);

      // Nch vs. Delta(phi) profiles, 500 MeV track pT cut
      _hist_N_vs_dPhi_1_500 = bookProfile1D(13+isqrts, 1, 1);
      _hist_N_vs_dPhi_2_500 = bookProfile1D(13+isqrts, 1, 2);
      _hist_N_vs_dPhi_3_500 = bookProfile1D(13+isqrts, 1, 3);
      _hist_N_vs_dPhi_5_500 = bookProfile1D(13+isqrts, 1, 4);
      // pT vs. Delta(phi) profiles, 500 MeV track pT cut
      _hist_pT_vs_dPhi_1_500 = bookProfile1D(15+isqrts, 1, 1);
      _hist_pT_vs_dPhi_2_500 = bookProfile1D(15+isqrts, 1, 2);
      _hist_pT_vs_dPhi_3_500 = bookProfile1D(15+isqrts, 1, 3);
      _hist_pT_vs_dPhi_5_500 = bookProfile1D(15+isqrts, 1, 4);

      // Nch and pTsum profiles, 100 MeV track pT cut
      _hist_nch_transverse_100   = bookProfile1D(17+isqrts, 1, 1);
      _hist_nch_toward_100       = bookProfile1D(17+isqrts, 1, 2);
      _hist_nch_away_100         = bookProfile1D(17+isqrts, 1, 3);
      _hist_ptsum_transverse_100 = bookProfile1D(19+isqrts, 1, 1);
      _hist_ptsum_toward_100     = bookProfile1D(19+isqrts, 1, 2);
      _hist_ptsum_away_100       = bookProfile1D(19+isqrts, 1, 3);

      // Profiles vs. eta (7 TeV only)
      if (isqrts == 1) {
        _hist_nch_vs_eta_transverse_100   = bookProfile1D(21, 1, 1);
        _hist_ptsum_vs_eta_transverse_100 = bookProfile1D(22, 1, 1);
      }

    }


    // Little helper function to identify Delta(phi) regions
    inline int region_index(double dphi) {
      assert(inRange(dphi, 0.0, PI, CLOSED, CLOSED));
      if (dphi < PI/3.0) return 0;
      if (dphi < 2*PI/3.0) return 1;
      return 2;
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      // Require at least one track in the event with pT >= 1 GeV
      const ChargedFinalState& cfslead = applyProjection<ChargedFinalState>(event, "CFSlead");
      if (cfslead.size() < 1) {
        vetoEvent;
      }

      // These are the charged particles (tracks) with pT > 500 MeV
      const ChargedFinalState& charged500 = applyProjection<ChargedFinalState>(event, "CFS500");

      // Identify leading track and its phi and pT (this is the same for both the 100 MeV and 500 MeV track cuts)
      Particles particles500 = charged500.particlesByPt();
      Particle p_lead = particles500[0];
      const double philead = p_lead.momentum().phi();
      const double etalead = p_lead.eta();
      const double pTlead  = p_lead.momentum().perp();
      MSG_DEBUG("Leading track: pT = " << pTlead << ", eta = " << etalead << ", phi = " << philead);

      // Iterate over all > 500 MeV particles and count particles and scalar pTsum in the three regions
      vector<double> num500(3, 0), ptSum500(3, 0.0);
      // Temporary histos that bin Nch and pT in dPhi.
      // NB. Only one of each needed since binnings are the same for the energies and pT cuts
      Histo1D hist_num_dphi_500(refData(13,1,1));
      Histo1D hist_pt_dphi_500(refData(15,1,1));
      foreach (const Particle& p, particles500) {
        const double pT = p.pT();
        const double dPhi = deltaPhi(philead, p.momentum().phi());
        const int ir = region_index(dPhi);
        num500[ir] += 1;
        ptSum500[ir] += pT;

        // Fill temp histos to bin Nch and pT in dPhi
        if (p.genParticle() != p_lead.genParticle()) { // We don't want to fill all those zeros from the leading track...
          hist_num_dphi_500.fill(dPhi, 1);
          hist_pt_dphi_500.fill(dPhi, pT);
        }
      }


      // Iterate over charged particles again for profiles against Nch
      // This is necessary since the Nch are region-specific and so are only known after the first loop
      foreach (const Particle& p, particles500) {
        const double pT = p.pT();
        const double dPhi = deltaPhi(philead, p.momentum().phi());
        const int ir = region_index(dPhi);
        switch (ir) {
        case 0:
          _hist_dn_dpt_toward_500->fill(num500[0], pT, weight);
          break;
        case 1:
          _hist_dn_dpt_transverse_500->fill(num500[1], pT, weight);
          break;
        case 2:
          _hist_dn_dpt_away_500->fill(num500[2], pT, weight);
          break;
        default:
          assert(false && "How did we get here?");
        }
      }


      // Now fill underlying event histograms
      // The densities are calculated by dividing the UE properties by dEta*dPhi
      // -- each region has a dPhi of 2*PI/3 and dEta is two times 2.5
      const double dEtadPhi = (2*2.5 * 2*PI/3.0);
      // Transverse profiles need 4 orders of moments for stddev with errors
      for (int i = 0; i < 4; ++i) {
        _hist_nch_transverse_500[i]->fill(pTlead/GeV, intpow(num500[1]/dEtadPhi, i+1), weight);
        _hist_ptsum_transverse_500[i]->fill(pTlead/GeV, intpow(ptSum500[1]/GeV/dEtadPhi, i+1), weight);
      }
      // Toward and away profiles only need the first moment (the mean)
      _hist_nch_toward_500->fill(pTlead/GeV, num500[0]/dEtadPhi, weight);
      _hist_nch_away_500->fill(pTlead/GeV, num500[2]/dEtadPhi, weight);
      _hist_ptsum_toward_500->fill(pTlead/GeV, ptSum500[0]/GeV/dEtadPhi, weight);
      _hist_ptsum_away_500->fill(pTlead/GeV, ptSum500[2]/GeV/dEtadPhi, weight);
      // <pT> profiles
      //MSG_INFO("Trans pT1, pTsum, Nch, <pT>" << pTlead/GeV << ", " <<  ptSum500[1]/GeV << ", " << num500[1] << ", " << ptSum500[1]/GeV/num500[1]);
      if (num500[1] > 0) _hist_ptavg_transverse_500->fill(pTlead/GeV, ptSum500[1]/GeV/num500[1], weight);
      if (num500[0] > 0) _hist_ptavg_toward_500->fill(pTlead/GeV, ptSum500[0]/GeV/num500[0], weight);
      if (num500[2] > 0) _hist_ptavg_away_500->fill(pTlead/GeV, ptSum500[2]/GeV/num500[2], weight);


      // Update the "proper" dphi profile histograms
      // Note that we fill dN/dEtadPhi: dEta = 2*2.5, dPhi = 2*PI/nBins
      // The values tabulated in the note are for an (undefined) signed Delta(phi) rather than
      // |Delta(phi)| and so differ by a factor of 2: we have to actually norm for angular range = 2pi
      const size_t nbins = refData(13,1,1).numPoints();
      std::vector<double> ptcut;
      if (fuzzyEquals(sqrtS(), 900*GeV)) {
        ptcut += 1.0; ptcut += 1.5; ptcut += 2.0; ptcut += 2.5;
      }
      else if (fuzzyEquals(sqrtS(), 7*TeV)) {
        ptcut += 1.0; ptcut += 2.0; ptcut += 3.0; ptcut += 5.0;
      }
      assert(ptcut.size() == 4);
      for (size_t i = 0; i < nbins; ++i) {
        // First Nch
        double mean = hist_num_dphi_500.bin(i).midpoint();
        double value = 0.;
        if (hist_num_dphi_500.bin(i).numEntries() > 0) {
          mean = hist_num_dphi_500.bin(i).xMean();
          value = hist_num_dphi_500.bin(i).area()/hist_num_dphi_500.bin(i).width()/10.0;
        }
        if (pTlead/GeV >= ptcut[0]) _hist_N_vs_dPhi_1_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[1]) _hist_N_vs_dPhi_2_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[2]) _hist_N_vs_dPhi_3_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[3]) _hist_N_vs_dPhi_5_500->fill(mean, value, weight);

        // Then pT
        mean = hist_pt_dphi_500.bin(i).midpoint();
        value = 0.;
        if (hist_pt_dphi_500.bin(i).numEntries() > 0) {
          mean = hist_pt_dphi_500.bin(i).xMean();
          value = hist_pt_dphi_500.bin(i).area()/hist_pt_dphi_500.bin(i).width()/10.0;
        }
        if (pTlead/GeV >= ptcut[0]) _hist_pT_vs_dPhi_1_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[1]) _hist_pT_vs_dPhi_2_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[2]) _hist_pT_vs_dPhi_3_500->fill(mean, value, weight);
        if (pTlead/GeV >= ptcut[3]) _hist_pT_vs_dPhi_5_500->fill(mean, value, weight);
      }


      //////////////////////


      // These are the charged particles (tracks) with pT > 100 MeV
      const ChargedFinalState& charged100 = applyProjection<ChargedFinalState>(event, "CFS100");

      // Iterate over all > 100 MeV particles and count particles and scalar pTsum in the three regions
      vector<double> num100(3, 0), ptSum100(3, 0.0);
      foreach (const Particle& p, charged100.particles()) {
        const double pT = p.pT();
        const double dPhi = deltaPhi(philead, p.momentum().phi());
        const int ir = region_index(dPhi);
        num100[ir] += 1;
        ptSum100[ir] += pT;
      }

      // Now fill the two sets of 100 MeV underlying event histograms
      _hist_nch_transverse_100->fill(pTlead/GeV, num100[1]/dEtadPhi, weight);
      _hist_nch_toward_100->fill(pTlead/GeV, num100[0]/dEtadPhi, weight);
      _hist_nch_away_100->fill(pTlead/GeV, num100[2]/dEtadPhi, weight);
      _hist_ptsum_transverse_100->fill(pTlead/GeV, ptSum100[1]/GeV/dEtadPhi, weight);
      _hist_ptsum_toward_100->fill(pTlead/GeV, ptSum100[0]/GeV/dEtadPhi, weight);
      _hist_ptsum_away_100->fill(pTlead/GeV, ptSum100[2]/GeV/dEtadPhi, weight);

      // And finally the Nch and pT vs eta_lead profiles (again from > 100 MeV tracks, and only at 7 TeV)
      if (fuzzyEquals(sqrtS(), 7*TeV) && pTlead > 5*GeV) {
        // MSG_INFO(sqrtS() << " " << pTlead << " " << ptSum100[1]/dEtadPhi << " " << num100[1]/dEtadPhi);
        _hist_nch_vs_eta_transverse_100->fill(etalead, num100[1]/dEtadPhi, weight);
        _hist_ptsum_vs_eta_transverse_100->fill(etalead, ptSum100[1]/GeV/dEtadPhi, weight);
      }

    }


    void finalize() {
      // Convert the various moments of the 500 MeV trans pT and Nch distributions to std devs with correct error
      _moments_to_stddev(_hist_nch_transverse_500, _dps_sdnch_transverse_500);
      _moments_to_stddev(_hist_ptsum_transverse_500, _dps_sdptsum_transverse_500);
    }


  private:


    inline void _moments_to_stddev(Profile1DPtr moment_profiles[], Scatter2DPtr target_dps) {
      for (size_t b = 0; b < moment_profiles[0]->numBins(); ++b) { // loop over points
        /// @todo Assuming unit weights here! Should use N_effective = sumW**2/sumW2?
        const double numentries = moment_profiles[0]->bin(b).numEntries();
        const double x = moment_profiles[0]->bin(b).midpoint();
        const double ex = moment_profiles[0]->bin(b).width()/2.;
        double var = 0.;
        double sd = 0.;
        if (numentries > 0) {
          var = moment_profiles[1]->bin(b).mean() - intpow(moment_profiles[0]->bin(b).mean(), 2);
          sd = fuzzyLessEquals(var,0.) ? 0 : sqrt(var); //< Numerical safety check
        }
        if (sd == 0 || numentries < 3) {
          MSG_WARNING("Need at least 3 bin entries and a non-zero central value to calculate "
                      << "an error on standard deviation profiles (bin " << b << ")");
          target_dps->addPoint(x, sd, ex, 0);
          continue;
        }
        // c2(y) = m4(x) - 4 m3(x) m1(x) - m2(x)^2 + 8 m2(x) m1(x)^2 - 4 m1(x)^4
        const double var_on_var = moment_profiles[3]->bin(b).mean()
          - 4 * moment_profiles[2]->bin(b).mean() * moment_profiles[0]->bin(b).mean()
          - intpow(moment_profiles[1]->bin(b).mean(), 2)
          + 8 * moment_profiles[1]->bin(b).mean() * intpow(moment_profiles[0]->bin(b).mean(), 2)
          - 4 * intpow(moment_profiles[0]->bin(b).mean(), 4);
        const double stderr_on_var = sqrt(var_on_var/(numentries-2.0));
        const double stderr_on_sd = stderr_on_var / (2.0*sd);
        target_dps->addPoint(x, sd, ex, stderr_on_sd);
      }
    }


  private:

    Profile1DPtr _hist_nch_transverse_500[4];
    Profile1DPtr _hist_nch_toward_500;
    Profile1DPtr _hist_nch_away_500;

    Profile1DPtr _hist_ptsum_transverse_500[4];
    Profile1DPtr _hist_ptsum_toward_500;
    Profile1DPtr _hist_ptsum_away_500;

    Scatter2DPtr  _dps_sdnch_transverse_500;
    Scatter2DPtr  _dps_sdptsum_transverse_500;

    Profile1DPtr _hist_ptavg_transverse_500;
    Profile1DPtr _hist_ptavg_toward_500;
    Profile1DPtr _hist_ptavg_away_500;

    Profile1DPtr _hist_dn_dpt_transverse_500;
    Profile1DPtr _hist_dn_dpt_toward_500;
    Profile1DPtr _hist_dn_dpt_away_500;

    Profile1DPtr _hist_N_vs_dPhi_1_500;
    Profile1DPtr _hist_N_vs_dPhi_2_500;
    Profile1DPtr _hist_N_vs_dPhi_3_500;
    Profile1DPtr _hist_N_vs_dPhi_5_500;

    Profile1DPtr _hist_pT_vs_dPhi_1_500;
    Profile1DPtr _hist_pT_vs_dPhi_2_500;
    Profile1DPtr _hist_pT_vs_dPhi_3_500;
    Profile1DPtr _hist_pT_vs_dPhi_5_500;

    Profile1DPtr _hist_nch_transverse_100;
    Profile1DPtr _hist_nch_toward_100;
    Profile1DPtr _hist_nch_away_100;
    Profile1DPtr _hist_ptsum_transverse_100;
    Profile1DPtr _hist_ptsum_toward_100;
    Profile1DPtr _hist_ptsum_away_100;

    Profile1DPtr _hist_nch_vs_eta_transverse_100;
    Profile1DPtr _hist_ptsum_vs_eta_transverse_100;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2010_S8894728);

}

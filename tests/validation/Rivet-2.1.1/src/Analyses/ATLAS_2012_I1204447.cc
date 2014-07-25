// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  class ATLAS_2012_I1204447 : public Analysis {
  public:

    /// Constructor
    ATLAS_2012_I1204447()
      : Analysis("ATLAS_2012_I1204447") {  }


    /// Book histograms and initialise projections before the run
    void init() {

      // To calculate the acceptance without having the fiducial lepton efficiencies included, this part can be turned off
      _use_fiducial_lepton_efficiency = true;

      // Random numbers for simulation of ATLAS detector reconstruction efficiency
      srand(160385);

      // Read in all signal regions
      _signal_regions = getSignalRegions();

      // Set number of events per signal region to 0
      for (size_t i = 0; i < _signal_regions.size(); i++)
        _eventCountsPerSR[_signal_regions[i]] = 0.0;

      // Final state including all charged and neutral particles
      const FinalState fs(-5.0, 5.0, 1*GeV);
      addProjection(fs, "FS");

      // Final state including all charged particles
      addProjection(ChargedFinalState(-2.5,2.5, 1*GeV), "CFS");

      // Final state including all visible particles (to calculate MET, Jets etc.)
      addProjection(VisibleFinalState(-5.0,5.0), "VFS");

      // Final state including all AntiKt 04 Jets
      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4), "AntiKtJets04");

      // Final state including all unstable particles (including taus)
      addProjection(UnstableFinalState(-5.0,5.0, 5*GeV),"UFS");

      // Final state including all electrons
      IdentifiedFinalState elecs(-2.47,2.47, 10*GeV);
      elecs.acceptIdPair(PID::ELECTRON);
      addProjection(elecs, "elecs");

      // Final state including all muons
      IdentifiedFinalState muons(-2.5,2.5, 10*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");

      // Book histograms
      _h_HTlep_all  = bookHisto1D("HTlep_all" , 30, 0, 1500);
      _h_HTjets_all = bookHisto1D("HTjets_all", 30, 0, 1500);
      _h_MET_all    = bookHisto1D("MET_all"   , 20, 0, 1000);
      _h_Meff_all   = bookHisto1D("Meff_all"  , 30, 0, 3000);

      _h_e_n        = bookHisto1D("e_n"  , 10, -0.5, 9.5);
      _h_mu_n       = bookHisto1D("mu_n" , 10, -0.5, 9.5);
      _h_tau_n      = bookHisto1D("tau_n", 10, -0.5, 9.5);

      _h_pt_1_3l    = bookHisto1D("pt_1_3l", 100, 0, 2000);
      _h_pt_2_3l    = bookHisto1D("pt_2_3l", 100, 0, 2000);
      _h_pt_3_3l    = bookHisto1D("pt_3_3l", 100, 0, 2000);
      _h_pt_1_2ltau = bookHisto1D("pt_1_2ltau", 100, 0, 2000);
      _h_pt_2_2ltau = bookHisto1D("pt_2_2ltau", 100, 0, 2000);
      _h_pt_3_2ltau = bookHisto1D("pt_3_2ltau", 100, 0, 2000);

      _h_excluded   = bookHisto1D("excluded", 2, -0.5, 1.5);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      // Muons
      Particles muon_candidates;
      const Particles charged_tracks    = applyProjection<ChargedFinalState>(event, "CFS").particles();
      const Particles visible_particles = applyProjection<VisibleFinalState>(event, "VFS").particles();
      foreach (const Particle& mu, applyProjection<IdentifiedFinalState>(event, "muons").particlesByPt()) {
        // Calculate pTCone30 variable (pT of all tracks within dR<0.3 - pT of muon itself)
        double pTinCone = -mu.momentum().pT();
        foreach (const Particle& track, charged_tracks) {
          if (deltaR(mu.momentum(), track.momentum()) < 0.3)
            pTinCone += track.momentum().pT();
        }

        // Calculate eTCone30 variable (pT of all visible particles within dR<0.3)
        double eTinCone = 0.;
        foreach (const Particle& visible_particle, visible_particles) {
          if (visible_particle.abspid() != PID::MUON && inRange(deltaR(mu.momentum(), visible_particle.momentum()), 0.1, 0.3))
            eTinCone += visible_particle.momentum().pT();
        }

        // Apply reconstruction efficiency and simulate reco
        int muon_id = 13;
        if ( mu.hasAncestor(15) || mu.hasAncestor(-15)) muon_id = 14;
        const double eff = (_use_fiducial_lepton_efficiency) ? apply_reco_eff(muon_id, mu) : 1.0;
        const bool keep_muon = rand()/static_cast<double>(RAND_MAX) <= eff;

        // Keep muon if pTCone30/pT < 0.15 and eTCone30/pT < 0.2 and reconstructed
        if (keep_muon && pTinCone/mu.pT() <= 0.15 && eTinCone/mu.pT() < 0.2)
          muon_candidates.push_back(mu);
      }


      // Electrons
      Particles electron_candidates;
      foreach (const Particle& e, applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt()) {
        // Neglect electrons in crack regions
        if (inRange(e.abseta(), 1.37, 1.52)) continue;

        // Calculate pTCone30 variable (pT of all tracks within dR<0.3 - pT of electron itself)
        double pTinCone = -e.momentum().pT();
        foreach (const Particle& track, charged_tracks) {
          if (deltaR(e.momentum(), track.momentum()) < 0.3) pTinCone += track.momentum().pT();
        }

        // Calculate eTCone30 variable (pT of all visible particles (except muons) within dR<0.3)
        double eTinCone = 0.;
        foreach (const Particle& visible_particle, visible_particles) {
          if (visible_particle.abspid() != PID::MUON && inRange(deltaR(e.momentum(), visible_particle.momentum()), 0.1, 0.3))
            eTinCone += visible_particle.momentum().pT();
        }

        // Apply reconstruction efficiency and simulate reco
        int elec_id = 11;
        if (e.hasAncestor(15) || e.hasAncestor(-15)) elec_id = 12;
        const double eff = (_use_fiducial_lepton_efficiency) ? apply_reco_eff(elec_id, e) : 1.0;
        const bool keep_elec = rand()/static_cast<double>(RAND_MAX) <= eff;

        // Keep electron if pTCone30/pT < 0.13 and eTCone30/pT < 0.2 and reconstructed
        if (keep_elec && pTinCone/e.pT() <= 0.13 && eTinCone/e.pT() < 0.2)
          electron_candidates.push_back(e);
      }


      // Taus
      /// @todo This could benefit from a tau finder projection
      Particles tau_candidates;
      foreach (const Particle& tau, applyProjection<UnstableFinalState>(event, "UFS").particlesByPt()) {
        // Only pick taus out of all unstable particles
        if (tau.abspid() != PID::TAU) continue;

        // Check that tau has decayed into daughter particles
        /// @todo Huh? Unstable taus with no decay vtx? Can use Particle.isStable()? But why in this situation?
        if ( !tau.genParticle()->end_vertex() ) continue;

        // Calculate visible tau pT from pT of tau neutrino in tau decay for pT and |eta| cuts
        const FourMomentum tau_neutrino_mom = get_tau_neutrino_mom(tau);
        const FourMomentum visible_tau_mom = tau.momentum() - tau_neutrino_mom;
        if (visible_tau_mom.pT() < 15*GeV || visible_tau_mom.abseta() > 2.5) continue;

        // Get prong number (number of tracks) in tau decay and check if tau decays leptonically
        unsigned int nprong = 0;
        bool lep_decaying_tau = false;
        get_prong_number(tau.genParticle(), nprong, lep_decaying_tau);

        // Apply reconstruction efficiency
        int tau_id = 15;
        if (nprong == 1) tau_id = 15;
        else if (nprong == 3) tau_id = 16;

        // Get fiducial lepton efficiency simulate reco efficiency
        const double eff = (_use_fiducial_lepton_efficiency) ? apply_reco_eff(tau_id, tau) : 1.0;
        const bool keep_tau = rand()/static_cast<double>(RAND_MAX) <= eff;

        // Keep tau if nprong = 1, it decays hadronically, and it's reconstructed by the detector
        if ( !lep_decaying_tau && nprong == 1 && keep_tau) tau_candidates.push_back(tau);
      }


      // Jets (all anti-kt R=0.4 jets with pT > 25 GeV and eta < 4.9)
      Jets jet_candidates;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(25*GeV)) {
        if (jet.abseta() < 4.9) jet_candidates.push_back(jet);
      }


      // ETmiss
      Particles vfs_particles = applyProjection<VisibleFinalState>(event, "VFS").particles();
      FourMomentum pTmiss;
      foreach (const Particle& p, vfs_particles) pTmiss -= p.momentum();
      double eTmiss = pTmiss.pT()/GeV;


      //------------------
      // Overlap removal

      // electron - electron
      Particles electron_candidates_2;
      for (size_t ie = 0; ie < electron_candidates.size(); ++ie) {
        const Particle & e = electron_candidates[ie];
        bool away = true;
        // If electron pair within dR < 0.1: remove electron with lower pT
        for (size_t ie2=0; ie2 < electron_candidates_2.size(); ++ie2) {
          if ( deltaR( e.momentum(), electron_candidates_2[ie2].momentum()) < 0.1 ) {
            away = false;
            break;
          }
        }
        // If isolated keep it
        if ( away )
          electron_candidates_2.push_back( e );
      }
      // jet - electron
      Jets recon_jets;
      foreach (const Jet& jet, jet_candidates) {
        bool away = true;
        // if jet within dR < 0.2 of electron: remove jet
        foreach (const Particle& e, electron_candidates_2) {
          if (deltaR(e.momentum(), jet.momentum()) < 0.2) {
            away = false;
            break;
          }
        }
        // jet - tau
        if (away)  {
          // If jet within dR < 0.2 of tau: remove jet
          foreach (const Particle& tau, tau_candidates) {
            if (deltaR(tau.momentum(), jet.momentum()) < 0.2) {
              away = false;
              break;
            }
          }
        }
        // If isolated keep it
        if ( away )
          recon_jets.push_back( jet );
      }


      // electron - jet
      Particles recon_leptons, recon_e;
      for (size_t ie = 0; ie < electron_candidates_2.size(); ++ie) {
        const Particle& e = electron_candidates_2[ie];
        // If electron within 0.2 < dR < 0.4 from any jets: remove electron
        bool away = true;
        foreach (const Jet& jet, recon_jets) {
          if (deltaR(e.momentum(), jet.momentum()) < 0.4) {
            away = false;
            break;
          }
        }
        // electron - muon
        // if electron within dR < 0.1 of a muon: remove electron
        if (away) {
          foreach (const Particle& mu, muon_candidates) {
            if (deltaR(mu.momentum(), e.momentum()) < 0.1) {
              away = false;
              break;
            }
          }
        }
        // If isolated keep it
        if (away)  {
          recon_e += e;
          recon_leptons += e;
        }
      }


      // tau - electron
      Particles recon_tau;
      foreach ( const Particle& tau, tau_candidates ) {
        bool away = true;
        // If tau within dR < 0.2 of an electron: remove tau
        foreach ( const Particle& e, recon_e ) {
          if (deltaR( tau.momentum(), e.momentum()) < 0.2) {
            away = false;
            break;
          }
        }
        // tau - muon
        // If tau within dR < 0.2 of a muon: remove tau
        if (away)  {
          foreach (const Particle& mu, muon_candidates) {
            if (deltaR(tau.momentum(), mu.momentum()) < 0.2) {
              away = false;
              break;
            }
          }
        }
        // If isolated keep it
        if (away) recon_tau.push_back( tau );
      }

      // Muon - jet isolation
      Particles recon_mu, trigger_mu;
      // If muon within dR < 0.4 of a jet, remove muon
      foreach (const Particle& mu, muon_candidates) {
        bool away = true;
        foreach (const Jet& jet, recon_jets) {
          if ( deltaR( mu.momentum(), jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        if (away) {
          recon_mu.push_back( mu );
          recon_leptons.push_back( mu );
          if (mu.abseta() < 2.4) trigger_mu.push_back( mu );
        }
      }

      // End overlap removal
      //------------------


      // Jet cleaning
      if (rand()/static_cast<double>(RAND_MAX) <= 0.42) {
        foreach (const Jet& jet, recon_jets) {
          const double eta = jet.rapidity();
          const double phi = jet.momentum().azimuthalAngle(MINUSPI_PLUSPI);
          if (jet.pT() > 25*GeV && inRange(eta, -0.1, 1.5) && inRange(phi, -0.9, -0.5)) vetoEvent;
        }
      }


      // Post-isolation event cuts
      // Require at least 3 charged tracks in event
      if (charged_tracks.size() < 3) vetoEvent;

      // And at least one e/mu passing trigger
      if (!( !recon_e   .empty() && recon_e[0]   .pT() > 25*GeV)  &&
          !( !trigger_mu.empty() && trigger_mu[0].pT() > 25*GeV) ) {
        MSG_DEBUG("Hardest lepton fails trigger");
        vetoEvent;
      }

      // And only accept events with at least 2 electrons and muons and at least 3 leptons in total
      if (recon_mu.size() + recon_e.size() + recon_tau.size() < 3 || recon_leptons.size() < 2) vetoEvent;

      // Now it's worth getting the event weight
      const double weight = event.weight();

      // Sort leptons by decreasing pT
      sortByPt(recon_leptons);
      sortByPt(recon_tau);

      // Calculate HTlep, fill lepton pT histograms & store chosen combination of 3 leptons
      double HTlep = 0.;
      Particles chosen_leptons;
      if ( recon_leptons.size() > 2 ) {
        _h_pt_1_3l->fill(recon_leptons[0].momentum().perp()/GeV, weight);
        _h_pt_2_3l->fill(recon_leptons[1].momentum().perp()/GeV, weight);
        _h_pt_3_3l->fill(recon_leptons[2].momentum().perp()/GeV, weight);
        HTlep = (recon_leptons[0].pT() + recon_leptons[1].pT() + recon_leptons[2].pT())/GeV;
        chosen_leptons.push_back( recon_leptons[0] );
        chosen_leptons.push_back( recon_leptons[1] );
        chosen_leptons.push_back( recon_leptons[2] );
      }
      else {
        _h_pt_1_2ltau->fill(recon_leptons[0].momentum().perp()/GeV, weight);
        _h_pt_2_2ltau->fill(recon_leptons[1].momentum().perp()/GeV, weight);
        _h_pt_3_2ltau->fill(recon_tau[0].momentum().perp()/GeV,     weight);
        HTlep = (recon_leptons[0].pT() + recon_leptons[1].pT() + recon_tau[0].pT())/GeV ;
        chosen_leptons.push_back( recon_leptons[0] );
        chosen_leptons.push_back( recon_leptons[1] );
        chosen_leptons.push_back( recon_tau[0] );
      }

      // Number of prompt e/mu and had taus
      _h_e_n  ->fill(recon_e.size()  , weight);
      _h_mu_n ->fill(recon_mu.size() , weight);
      _h_tau_n->fill(recon_tau.size(), weight);

      // Calculate HTjets
      double HTjets = 0.;
      foreach ( const Jet & jet, recon_jets )
        HTjets += jet.momentum().perp()/GeV;

      // Calculate meff
      double meff = eTmiss + HTjets;
      Particles all_leptons;
      foreach ( const Particle & e , recon_e  )  {
        meff += e.momentum().perp()/GeV;
        all_leptons.push_back( e );
      }
      foreach ( const Particle & mu, recon_mu )  {
        meff += mu.momentum().perp()/GeV;
        all_leptons.push_back( mu );
      }
      foreach ( const Particle & tau, recon_tau )  {
        meff += tau.momentum().perp()/GeV;
        all_leptons.push_back( tau );
      }

      // Fill histogram of kinematic variables
      _h_HTlep_all ->fill(HTlep , weight);
      _h_HTjets_all->fill(HTjets, weight);
      _h_MET_all   ->fill(eTmiss, weight);
      _h_Meff_all  ->fill(meff  , weight);

      // Determine signal region (3l/2ltau, onZ/offZ)
      string basic_signal_region;
      if ( recon_mu.size() + recon_e.size() > 2 )
        basic_signal_region += "3l_";
      else if ( (recon_mu.size() + recon_e.size() == 2) && (recon_tau.size() > 0))
        basic_signal_region += "2ltau_";
      // Is there an OSSF pair or a three lepton combination with an invariant mass close to the Z mass
      int onZ = isonZ(chosen_leptons);
      if      (onZ == 1)   basic_signal_region += "onZ";
      else if (onZ == 0)   basic_signal_region += "offZ";
      // Check in which signal regions this event falls and adjust event counters
      fillEventCountsPerSR(basic_signal_region, onZ, HTlep, eTmiss, HTjets, meff, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      // Normalize to an integrated luminosity of 1 fb-1
      double norm = crossSection()/femtobarn/sumOfWeights();
      string best_signal_region = "";
      double ratio_best_SR = 0.;

      // Loop over all signal regions and find signal region with best sensitivity (ratio signal events/visible cross-section)
      for (size_t i = 0; i < _signal_regions.size(); i++)  {
        double signal_events = _eventCountsPerSR[_signal_regions[i]] * norm;
        // Use expected upper limits to find best signal region
        double UL95  = getUpperLimit(_signal_regions[i], false);
        double ratio = signal_events / UL95;
        if (ratio > ratio_best_SR)  {
          best_signal_region = _signal_regions[i];
          ratio_best_SR = ratio;
        }
      }

      double signal_events_best_SR = _eventCountsPerSR[best_signal_region] * norm;
      double exp_UL_best_SR = getUpperLimit(best_signal_region, false);
      double obs_UL_best_SR = getUpperLimit(best_signal_region, true);

      // Print out result
      cout << "----------------------------------------------------------------------------------------" << endl;
      cout << "Best signal region: " << best_signal_region << endl;
      cout << "Normalized number of signal events in this best signal region (per fb-1): " << signal_events_best_SR << endl;
      cout << "Efficiency*Acceptance: " <<  _eventCountsPerSR[best_signal_region]/sumOfWeights() << endl;
      cout << "Cross-section [fb]: " << crossSection()/femtobarn << endl;
      cout << "Expected visible cross-section (per fb-1): " << exp_UL_best_SR << endl;
      cout << "Ratio (signal events / expected visible cross-section): " << ratio_best_SR << endl;
      cout << "Observed visible cross-section (per fb-1): " << obs_UL_best_SR << endl;
      cout << "Ratio (signal events / observed visible cross-section): " <<  signal_events_best_SR/obs_UL_best_SR << endl;
      cout << "----------------------------------------------------------------------------------------" << endl;

      cout << "Using the EXPECTED limits (visible cross-section) of the analysis: " << endl;
      if (signal_events_best_SR > exp_UL_best_SR)  {
        cout << "Since the number of signal events > the visible cross-section, this model/grid point is EXCLUDED with 95% CL." << endl;
        _h_excluded->fill(1);
      }
      else  {
        cout << "Since the number of signal events < the visible cross-section, this model/grid point is NOT EXCLUDED." << endl;
        _h_excluded->fill(0);
      }
      cout << "----------------------------------------------------------------------------------------" << endl;

      cout << "Using the OBSERVED limits (visible cross-section) of the analysis: " << endl;
      if (signal_events_best_SR > obs_UL_best_SR)  {
        cout << "Since the number of signal events > the visible cross-section, this model/grid point is EXCLUDED with 95% CL." << endl;
        _h_excluded->fill(1);
      }
      else  {
        cout << "Since the number of signal events < the visible cross-section, this model/grid point is NOT EXCLUDED." << endl;
        _h_excluded->fill(0);
      }
      cout << "----------------------------------------------------------------------------------------" << endl;


      // Normalize to cross section
      if (norm != 0)  {
        scale(_h_HTlep_all,  norm);
        scale(_h_HTjets_all, norm);
        scale(_h_MET_all,    norm);
        scale(_h_Meff_all,   norm);

        scale(_h_pt_1_3l,    norm);
        scale(_h_pt_2_3l,    norm);
        scale(_h_pt_3_3l,    norm);
        scale(_h_pt_1_2ltau, norm);
        scale(_h_pt_2_2ltau, norm);
        scale(_h_pt_3_2ltau, norm);

        scale(_h_e_n,        norm);
        scale(_h_mu_n,       norm);
        scale(_h_tau_n,      norm);

        scale(_h_excluded,   signal_events_best_SR);
      }
    }


    /// Helper functions
    //@{

    /// Function giving a list of all signal regions
    vector<string> getSignalRegions()  {

      // List of basic signal regions
      vector<string> basic_signal_regions;
      basic_signal_regions.push_back("3l_offZ");
      basic_signal_regions.push_back("3l_onZ");
      basic_signal_regions.push_back("2ltau_offZ");
      basic_signal_regions.push_back("2ltau_onZ");

      // List of kinematic variables
      vector<string> kinematic_variables;
      kinematic_variables.push_back("HTlep");
      kinematic_variables.push_back("METStrong");
      kinematic_variables.push_back("METWeak");
      kinematic_variables.push_back("Meff");
      kinematic_variables.push_back("MeffStrong");

      vector<string> signal_regions;
      // Loop over all kinematic variables and basic signal regions
      for (size_t i0 = 0; i0 < kinematic_variables.size(); i0++)  {
        for (size_t i1 = 0; i1 < basic_signal_regions.size(); i1++)  {
          // Is signal region onZ?
          int onZ = (basic_signal_regions[i1].find("onZ") != string::npos) ? 1 : 0;
          // Get cut values for this kinematic variable
          vector<int> cut_values = getCutsPerSignalRegion(kinematic_variables[i0], onZ);
          // Loop over all cut values
          for (size_t i2 = 0; i2 < cut_values.size(); i2++)  {
            // push signal region into vector
            signal_regions.push_back( (kinematic_variables[i0] + "_" + basic_signal_regions[i1] + "_cut_" + toString(i2)) );
          }
        }
      }
      return signal_regions;
    }


    /// Function giving all cut vales per kinematic variable (taking onZ for MET into account)
    vector<int> getCutsPerSignalRegion(const string& signal_region, int onZ=0)  {
      vector<int> cutValues;

      // Cut values for HTlep
      if (signal_region.compare("HTlep") == 0)  {
        cutValues.push_back(0);
        cutValues.push_back(100);
        cutValues.push_back(150);
        cutValues.push_back(200);
        cutValues.push_back(300);
      }
      // Cut values for METStrong (HTjets > 100 GeV) and METWeak (HTjets < 100 GeV)
      else if (signal_region.compare("METStrong") == 0 || signal_region.compare("METWeak") == 0)  {
        if      (onZ == 0) cutValues.push_back(0);
        else if (onZ == 1) cutValues.push_back(20);
        cutValues.push_back(50);
        cutValues.push_back(75);
      }
      // Cut values for Meff and MeffStrong (MET > 75 GeV)
      if (signal_region.compare("Meff") == 0 || signal_region.compare("MeffStrong") == 0)  {
        cutValues.push_back(0);
        cutValues.push_back(150);
        cutValues.push_back(300);
        cutValues.push_back(500);
      }

      return cutValues;
    }


    /// function fills map EventCountsPerSR by looping over all signal regions
    /// and looking if the event falls into this signal region
    void fillEventCountsPerSR(const string& basic_signal_region, int onZ,
                              double HTlep, double eTmiss,
                              double HTjets, double meff,
                              double weight)  {

      // Get cut values for HTlep, loop over them and add event if cut is passed
      vector<int> cut_values = getCutsPerSignalRegion("HTlep", onZ);
      for (size_t i = 0; i < cut_values.size(); i++)  {
        if (HTlep > cut_values[i])
          _eventCountsPerSR[("HTlep_" + basic_signal_region + "_cut_" + toString(cut_values[i]))] += weight;
      }

      // Get cut values for METStrong, loop over them and add event if cut is passed
      cut_values = getCutsPerSignalRegion("METStrong", onZ);
      for (size_t i = 0; i < cut_values.size(); i++)  {
        if (eTmiss > cut_values[i] && HTjets > 100.)
          _eventCountsPerSR[("METStrong_" + basic_signal_region + "_cut_" + toString(cut_values[i]))] += weight;
      }

      // Get cut values for METWeak, loop over them and add event if cut is passed
      cut_values = getCutsPerSignalRegion("METWeak", onZ);
      for (size_t i = 0; i < cut_values.size(); i++)  {
        if (eTmiss > cut_values[i] && HTjets <= 100.)
          _eventCountsPerSR[("METWeak_" + basic_signal_region + "_cut_" + toString(cut_values[i]))] += weight;
      }

      // Get cut values for Meff, loop over them and add event if cut is passed
      cut_values = getCutsPerSignalRegion("Meff", onZ);
      for (size_t i = 0; i < cut_values.size(); i++)  {
        if (meff > cut_values[i])
          _eventCountsPerSR[("Meff_" + basic_signal_region + "_cut_" + toString(cut_values[i]))] += weight;
      }

      // Get cut values for MeffStrong, loop over them and add event if cut is passed
      cut_values = getCutsPerSignalRegion("MeffStrong", onZ);
      for (size_t i = 0; i < cut_values.size(); i++)  {
        if (meff > cut_values[i] && eTmiss > 75.)
          _eventCountsPerSR[("MeffStrong_" + basic_signal_region + "_cut_" + toString(cut_values[i]))] += weight;
      }
    }


    /// Function returning 4-vector of daughter-particle if it is a tau neutrino
    /// @todo Move to TauFinder and make less HepMC-ish
    FourMomentum get_tau_neutrino_mom(const Particle& p)  {
      assert(p.abspid() == PID::TAU);
      const GenVertexPtr dv = p.genParticle()->end_vertex();
      assert(dv);
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        if ((*pp)->pdg_id() == PID::NU_TAU) return FourMomentum((*pp)->momentum());
      }
      return FourMomentum();
    }


    /// Function calculating the prong number of taus
    /// @todo Move to TauFinder and make less HepMC-ish
    void get_prong_number(const GenParticlePtr p, unsigned int& nprong, bool& lep_decaying_tau) {
      assert(p != NULL);
      //const int tau_barcode = p->barcode();
      const GenVertexPtr dv = p->end_vertex();
      assert(dv != NULL);
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        // If they have status 1 and are charged they will produce a track and the prong number is +1
        if ((*pp)->status() == 1 )  {
          const int id = (*pp)->pdg_id();
          if (Rivet::PID::charge(id) != 0 ) ++nprong;
          // Check if tau decays leptonically
          // @todo Can a tau decay include a tau in its decay daughters?!
          if ((abs(id) == PID::ELECTRON || abs(id) == PID::MUON || abs(id) == PID::TAU) && abs(p->pdg_id()) == PID::TAU) lep_decaying_tau = true;
        }
        // If the status of the daughter particle is 2 it is unstable and the further decays are checked
        else if ((*pp)->status() == 2 )  {
          get_prong_number(*pp, nprong, lep_decaying_tau);
        }
      }
    }


    /// Function giving fiducial lepton efficiency
    double apply_reco_eff(int flavor, const Particle& p) {
      float pt = p.pT()/GeV;
      float eta = p.eta();

      double eff = 0.;
      //double err = 0.;

      if (flavor == 11) { // weight prompt electron -- now including data/MC ID SF in eff.
        //float rho = 0.820;
        float p0 = 7.34;  float p1 = 0.8977;
        //float ep0= 0.5 ;  float ep1= 0.0087;
        eff = p1 - p0/pt;

        //double err0 = ep0/pt; // d(eff)/dp0
        //double err1 = ep1;    // d(eff)/dp1
        //err = sqrt(err0*err0 + err1*err1 - 2*rho*err0*err1);

        double avgrate = 0.6867;
        float wz_ele_eta[] = {0.588717,0.603674,0.666135,0.747493,0.762202,0.675051,0.751606,0.745569,0.665333,0.610432,0.592693,};
        //float ewz_ele_eta[] ={0.00292902,0.002476,0.00241209,0.00182319,0.00194339,0.00299785,0.00197339,0.00182004,0.00241793,0.00245997,0.00290394,};
        int ibin = 3;

        if (eta >= -2.5 && eta < -2.0) ibin = 0;
        if (eta >= -2.0 && eta < -1.5) ibin = 1;
        if (eta >= -1.5 && eta < -1.0) ibin = 2;
        if (eta >= -1.0 && eta < -0.5) ibin = 3;
        if (eta >= -0.5 && eta < -0.1) ibin = 4;
        if (eta >= -0.1 && eta <  0.1) ibin = 5;
        if (eta >=  0.1 && eta <  0.5) ibin = 6;
        if (eta >=  0.5 && eta <  1.0) ibin = 7;
        if (eta >=  1.0 && eta <  1.5) ibin = 8;
        if (eta >=  1.5 && eta <  2.0) ibin = 9;
        if (eta >=  2.0 && eta <  2.5) ibin = 10;

        double eff_eta = wz_ele_eta[ibin];
        //double err_eta = ewz_ele_eta[ibin];

        eff = (eff*eff_eta)/avgrate;
      }

      if (flavor == 12)  { // weight electron from tau
        //float rho = 0.884;
        float p0 = 6.799;  float p1 = 0.842;
        //float ep0= 0.664;  float ep1= 0.016;
        eff = p1 - p0/pt;

        //double err0 = ep0/pt; // d(eff)/dp0
        //double err1 = ep1;    // d(eff)/dp1
        //err = sqrt(err0*err0 + err1*err1 - 2*rho*err0*err1);

        double avgrate = 0.5319;
        float wz_elet_eta[] = {0.468945,0.465953,0.489545,0.58709,0.59669,0.515829,0.59284,0.575828,0.498181,0.463536,0.481738,};
        //float ewz_elet_eta[] ={0.00933795,0.00780868,0.00792679,0.00642083,0.00692652,0.0101568,0.00698452,0.00643524,0.0080002,0.00776238,0.0094699,};
        int ibin = 3;

        if (eta >= -2.5 && eta < -2.0) ibin = 0;
        if (eta >= -2.0 && eta < -1.5) ibin = 1;
        if (eta >= -1.5 && eta < -1.0) ibin = 2;
        if (eta >= -1.0 && eta < -0.5) ibin = 3;
        if (eta >= -0.5 && eta < -0.1) ibin = 4;
        if (eta >= -0.1 && eta <  0.1) ibin = 5;
        if (eta >=  0.1 && eta <  0.5) ibin = 6;
        if (eta >=  0.5 && eta <  1.0) ibin = 7;
        if (eta >=  1.0 && eta <  1.5) ibin = 8;
        if (eta >=  1.5 && eta <  2.0) ibin = 9;
        if (eta >=  2.0 && eta <  2.5) ibin = 10;

        double eff_eta = wz_elet_eta[ibin];
        //double err_eta = ewz_elet_eta[ibin];

        eff = (eff*eff_eta)/avgrate;

      }

      if (flavor == 13)  {// weight prompt muon

        //if eta>0.1
        float p0 = -18.21;  float p1 = 14.83;  float p2 = 0.9312;
        //float ep0= 5.06;    float ep1= 1.9;    float ep2=0.00069;

        if ( fabs(eta) < 0.1)  {
          p0  = 7.459; p1 = 2.615; p2  = 0.5138;
          //ep0 = 10.4; ep1 = 4.934; ep2 = 0.0034;
        }

        double arg = ( pt-p0 )/( 2.*p1 ) ;
        eff = 0.5 * p2 * (1.+erf(arg));
        //err = 0.1*eff;
      }

      if (flavor == 14)  {// weight muon from tau

        if (fabs(eta) < 0.1) {
          float p0 = -1.756;  float p1 = 12.38;  float p2 = 0.4441;
          //float ep0= 10.39;   float ep1= 7.9;  float ep2=0.022;
          double arg = ( pt-p0 )/( 2.*p1 ) ;
          eff = 0.5 * p2 * (1.+erf(arg));
          //err = 0.1*eff;
        }
        else {
          float p0 = 2.102;  float p1 = 0.8293;
          //float ep0= 0.271;  float ep1= 0.0083;
          eff = p1 - p0/pt;
          //double err0 = ep0/pt; // d(eff)/dp0
          //double err1 = ep1;    // d(eff)/dp1
          //err = sqrt(err0*err0 + err1*err1 - 2*rho*err0*err1);
        }
      }

      if (flavor == 15)  {// weight hadronic tau 1p

        float wz_tau1p[] = {0.0249278,0.146978,0.225049,0.229212,0.21519,0.206152,0.201559,0.197917,0.209249,0.228336,0.193548,};
        //float ewz_tau1p[] ={0.00178577,0.00425252,0.00535052,0.00592126,0.00484684,0.00612941,0.00792099,0.0083006,0.0138307,0.015568,0.0501751,};
        int ibin = 0;
        if (pt > 15)  ibin = 1;
        if (pt > 20)  ibin = 2;
        if (pt > 25)  ibin = 3;
        if (pt > 30)  ibin = 4;
        if (pt > 40)  ibin = 5;
        if (pt > 50)  ibin = 6;
        if (pt > 60)  ibin = 7;
        if (pt > 80)  ibin = 8;
        if (pt > 100) ibin = 9;
        if (pt > 200) ibin = 10;

        eff = wz_tau1p[ibin];
        //err = ewz_tau1p[ibin];


        double avgrate = 0.1718;
        float wz_tau1p_eta[] = {0.162132,0.176393,0.139619,0.178813,0.185144,0.210027,0.203937,0.178688,0.137034,0.164216,0.163713,};
        //float ewz_tau1p_eta[] ={0.00706705,0.00617989,0.00506798,0.00525172,0.00581865,0.00865675,0.00599245,0.00529877,0.00506368,0.00617025,0.00726219,};

        ibin = 3;
        if (eta >= -2.5 && eta < -2.0) ibin = 0;
        if (eta >= -2.0 && eta < -1.5) ibin = 1;
        if (eta >= -1.5 && eta < -1.0) ibin = 2;
        if (eta >= -1.0 && eta < -0.5) ibin = 3;
        if (eta >= -0.5 && eta < -0.1) ibin = 4;
        if (eta >= -0.1 && eta <  0.1) ibin = 5;
        if (eta >=  0.1 && eta <  0.5) ibin = 6;
        if (eta >=  0.5 && eta <  1.0) ibin = 7;
        if (eta >=  1.0 && eta <  1.5) ibin = 8;
        if (eta >=  1.5 && eta <  2.0) ibin = 9;
        if (eta >=  2.0 && eta <  2.5) ibin = 10;

        double eff_eta = wz_tau1p_eta[ibin];
        //double err_eta = ewz_tau1p_eta[ibin];

        eff = (eff*eff_eta)/avgrate;
      }

      if (flavor == 16)  { //weight hadronic tau 3p

        float wz_tau3p[] = {0.000587199,0.00247181,0.0013031,0.00280112,};
        //float ewz_tau3p[] ={0.000415091,0.000617187,0.000582385,0.00197792,};

        int ibin = 0;
        if (pt > 15) ibin = 1;
        if (pt > 20) ibin = 2;
        if (pt > 40) ibin = 3;
        if (pt > 80) ibin = 4;

        eff = wz_tau3p[ibin];
        //err = ewz_tau3p[ibin];
      }

      return eff;
    }


    /// Function giving observed upper limit (visible cross-section)
    double getUpperLimit(const string& signal_region, bool observed)  {
      map<string,double> upperLimitsObserved;
      upperLimitsObserved["HTlep_3l_offZ_cut_0"] = 11.;
      upperLimitsObserved["HTlep_3l_offZ_cut_100"] = 8.7;
      upperLimitsObserved["HTlep_3l_offZ_cut_150"] = 4.0;
      upperLimitsObserved["HTlep_3l_offZ_cut_200"] = 4.4;
      upperLimitsObserved["HTlep_3l_offZ_cut_300"] = 1.6;
      upperLimitsObserved["HTlep_2ltau_offZ_cut_0"] = 25.;
      upperLimitsObserved["HTlep_2ltau_offZ_cut_100"] = 14.;
      upperLimitsObserved["HTlep_2ltau_offZ_cut_150"] = 6.1;
      upperLimitsObserved["HTlep_2ltau_offZ_cut_200"] = 3.3;
      upperLimitsObserved["HTlep_2ltau_offZ_cut_300"] = 1.2;
      upperLimitsObserved["HTlep_3l_onZ_cut_0"] = 48.;
      upperLimitsObserved["HTlep_3l_onZ_cut_100"] = 38.;
      upperLimitsObserved["HTlep_3l_onZ_cut_150"] = 14.;
      upperLimitsObserved["HTlep_3l_onZ_cut_200"] = 7.2;
      upperLimitsObserved["HTlep_3l_onZ_cut_300"] = 4.5;
      upperLimitsObserved["HTlep_2ltau_onZ_cut_0"] = 85.;
      upperLimitsObserved["HTlep_2ltau_onZ_cut_100"] = 53.;
      upperLimitsObserved["HTlep_2ltau_onZ_cut_150"] = 11.0;
      upperLimitsObserved["HTlep_2ltau_onZ_cut_200"] = 5.2;
      upperLimitsObserved["HTlep_2ltau_onZ_cut_300"] = 3.0;
      upperLimitsObserved["METStrong_3l_offZ_cut_0"] = 2.6;
      upperLimitsObserved["METStrong_3l_offZ_cut_50"] = 2.1;
      upperLimitsObserved["METStrong_3l_offZ_cut_75"] = 2.1;
      upperLimitsObserved["METStrong_2ltau_offZ_cut_0"] = 4.2;
      upperLimitsObserved["METStrong_2ltau_offZ_cut_50"] = 3.1;
      upperLimitsObserved["METStrong_2ltau_offZ_cut_75"] = 2.6;
      upperLimitsObserved["METStrong_3l_onZ_cut_20"] = 11.0;
      upperLimitsObserved["METStrong_3l_onZ_cut_50"] = 6.4;
      upperLimitsObserved["METStrong_3l_onZ_cut_75"] = 5.1;
      upperLimitsObserved["METStrong_2ltau_onZ_cut_20"] = 5.9;
      upperLimitsObserved["METStrong_2ltau_onZ_cut_50"] = 3.4;
      upperLimitsObserved["METStrong_2ltau_onZ_cut_75"] = 1.2;
      upperLimitsObserved["METWeak_3l_offZ_cut_0"] = 11.;
      upperLimitsObserved["METWeak_3l_offZ_cut_50"] = 5.3;
      upperLimitsObserved["METWeak_3l_offZ_cut_75"] = 3.1;
      upperLimitsObserved["METWeak_2ltau_offZ_cut_0"] = 23.;
      upperLimitsObserved["METWeak_2ltau_offZ_cut_50"] = 4.3;
      upperLimitsObserved["METWeak_2ltau_offZ_cut_75"] = 3.1;
      upperLimitsObserved["METWeak_3l_onZ_cut_20"] = 41.;
      upperLimitsObserved["METWeak_3l_onZ_cut_50"] = 16.;
      upperLimitsObserved["METWeak_3l_onZ_cut_75"] = 8.0;
      upperLimitsObserved["METWeak_2ltau_onZ_cut_20"] = 80.;
      upperLimitsObserved["METWeak_2ltau_onZ_cut_50"] = 4.4;
      upperLimitsObserved["METWeak_2ltau_onZ_cut_75"] = 1.8;
      upperLimitsObserved["Meff_3l_offZ_cut_0"] = 11.;
      upperLimitsObserved["Meff_3l_offZ_cut_150"] = 8.1;
      upperLimitsObserved["Meff_3l_offZ_cut_300"] = 3.1;
      upperLimitsObserved["Meff_3l_offZ_cut_500"] = 2.1;
      upperLimitsObserved["Meff_2ltau_offZ_cut_0"] = 25.;
      upperLimitsObserved["Meff_2ltau_offZ_cut_150"] = 12.;
      upperLimitsObserved["Meff_2ltau_offZ_cut_300"] = 3.9;
      upperLimitsObserved["Meff_2ltau_offZ_cut_500"] = 2.2;
      upperLimitsObserved["Meff_3l_onZ_cut_0"] = 48.;
      upperLimitsObserved["Meff_3l_onZ_cut_150"] = 37.;
      upperLimitsObserved["Meff_3l_onZ_cut_300"] = 11.;
      upperLimitsObserved["Meff_3l_onZ_cut_500"] = 4.8;
      upperLimitsObserved["Meff_2ltau_onZ_cut_0"] = 85.;
      upperLimitsObserved["Meff_2ltau_onZ_cut_150"] = 28.;
      upperLimitsObserved["Meff_2ltau_onZ_cut_300"] = 5.9;
      upperLimitsObserved["Meff_2ltau_onZ_cut_500"] = 1.9;
      upperLimitsObserved["MeffStrong_3l_offZ_cut_0"] = 3.8;
      upperLimitsObserved["MeffStrong_3l_offZ_cut_150"] = 3.8;
      upperLimitsObserved["MeffStrong_3l_offZ_cut_300"] = 2.8;
      upperLimitsObserved["MeffStrong_3l_offZ_cut_500"] = 2.1;
      upperLimitsObserved["MeffStrong_2ltau_offZ_cut_0"] = 3.9;
      upperLimitsObserved["MeffStrong_2ltau_offZ_cut_150"] = 4.0;
      upperLimitsObserved["MeffStrong_2ltau_offZ_cut_300"] = 2.9;
      upperLimitsObserved["MeffStrong_2ltau_offZ_cut_500"] = 1.5;
      upperLimitsObserved["MeffStrong_3l_onZ_cut_0"] = 10.0;
      upperLimitsObserved["MeffStrong_3l_onZ_cut_150"] = 10.0;
      upperLimitsObserved["MeffStrong_3l_onZ_cut_300"] = 6.8;
      upperLimitsObserved["MeffStrong_3l_onZ_cut_500"] = 3.9;
      upperLimitsObserved["MeffStrong_2ltau_onZ_cut_0"] = 1.6;
      upperLimitsObserved["MeffStrong_2ltau_onZ_cut_150"] = 1.4;
      upperLimitsObserved["MeffStrong_2ltau_onZ_cut_300"] = 1.5;
      upperLimitsObserved["MeffStrong_2ltau_onZ_cut_500"] = 0.9;

      // Expected upper limits are also given but not used in this analysis
      map<string,double> upperLimitsExpected;
      upperLimitsExpected["HTlep_3l_offZ_cut_0"] = 11.;
      upperLimitsExpected["HTlep_3l_offZ_cut_100"] = 8.5;
      upperLimitsExpected["HTlep_3l_offZ_cut_150"] = 4.6;
      upperLimitsExpected["HTlep_3l_offZ_cut_200"] = 3.6;
      upperLimitsExpected["HTlep_3l_offZ_cut_300"] = 1.9;
      upperLimitsExpected["HTlep_2ltau_offZ_cut_0"] = 23.;
      upperLimitsExpected["HTlep_2ltau_offZ_cut_100"] = 14.;
      upperLimitsExpected["HTlep_2ltau_offZ_cut_150"] = 6.4;
      upperLimitsExpected["HTlep_2ltau_offZ_cut_200"] = 3.6;
      upperLimitsExpected["HTlep_2ltau_offZ_cut_300"] = 1.5;
      upperLimitsExpected["HTlep_3l_onZ_cut_0"] = 33.;
      upperLimitsExpected["HTlep_3l_onZ_cut_100"] = 25.;
      upperLimitsExpected["HTlep_3l_onZ_cut_150"] = 12.;
      upperLimitsExpected["HTlep_3l_onZ_cut_200"] = 6.5;
      upperLimitsExpected["HTlep_3l_onZ_cut_300"] = 3.1;
      upperLimitsExpected["HTlep_2ltau_onZ_cut_0"] = 94.;
      upperLimitsExpected["HTlep_2ltau_onZ_cut_100"] = 61.;
      upperLimitsExpected["HTlep_2ltau_onZ_cut_150"] = 9.9;
      upperLimitsExpected["HTlep_2ltau_onZ_cut_200"] = 4.5;
      upperLimitsExpected["HTlep_2ltau_onZ_cut_300"] = 1.9;
      upperLimitsExpected["METStrong_3l_offZ_cut_0"] = 3.1;
      upperLimitsExpected["METStrong_3l_offZ_cut_50"] = 2.4;
      upperLimitsExpected["METStrong_3l_offZ_cut_75"] = 2.3;
      upperLimitsExpected["METStrong_2ltau_offZ_cut_0"] = 4.8;
      upperLimitsExpected["METStrong_2ltau_offZ_cut_50"] = 3.3;
      upperLimitsExpected["METStrong_2ltau_offZ_cut_75"] = 2.1;
      upperLimitsExpected["METStrong_3l_onZ_cut_20"] = 8.7;
      upperLimitsExpected["METStrong_3l_onZ_cut_50"] = 4.9;
      upperLimitsExpected["METStrong_3l_onZ_cut_75"] = 3.8;
      upperLimitsExpected["METStrong_2ltau_onZ_cut_20"] = 7.3;
      upperLimitsExpected["METStrong_2ltau_onZ_cut_50"] = 2.8;
      upperLimitsExpected["METStrong_2ltau_onZ_cut_75"] = 1.5;
      upperLimitsExpected["METWeak_3l_offZ_cut_0"] = 10.;
      upperLimitsExpected["METWeak_3l_offZ_cut_50"] = 4.7;
      upperLimitsExpected["METWeak_3l_offZ_cut_75"] = 3.0;
      upperLimitsExpected["METWeak_2ltau_offZ_cut_0"] = 21.;
      upperLimitsExpected["METWeak_2ltau_offZ_cut_50"] = 4.0;
      upperLimitsExpected["METWeak_2ltau_offZ_cut_75"] = 2.6;
      upperLimitsExpected["METWeak_3l_onZ_cut_20"] = 30.;
      upperLimitsExpected["METWeak_3l_onZ_cut_50"] = 10.;
      upperLimitsExpected["METWeak_3l_onZ_cut_75"] = 5.4;
      upperLimitsExpected["METWeak_2ltau_onZ_cut_20"] = 88.;
      upperLimitsExpected["METWeak_2ltau_onZ_cut_50"] = 5.5;
      upperLimitsExpected["METWeak_2ltau_onZ_cut_75"] = 2.2;
      upperLimitsExpected["Meff_3l_offZ_cut_0"] = 11.;
      upperLimitsExpected["Meff_3l_offZ_cut_150"] = 8.8;
      upperLimitsExpected["Meff_3l_offZ_cut_300"] = 3.7;
      upperLimitsExpected["Meff_3l_offZ_cut_500"] = 2.1;
      upperLimitsExpected["Meff_2ltau_offZ_cut_0"] = 23.;
      upperLimitsExpected["Meff_2ltau_offZ_cut_150"] = 13.;
      upperLimitsExpected["Meff_2ltau_offZ_cut_300"] = 4.9;
      upperLimitsExpected["Meff_2ltau_offZ_cut_500"] = 2.4;
      upperLimitsExpected["Meff_3l_onZ_cut_0"] = 33.;
      upperLimitsExpected["Meff_3l_onZ_cut_150"] = 25.;
      upperLimitsExpected["Meff_3l_onZ_cut_300"] = 9.;
      upperLimitsExpected["Meff_3l_onZ_cut_500"] = 3.9;
      upperLimitsExpected["Meff_2ltau_onZ_cut_0"] = 94.;
      upperLimitsExpected["Meff_2ltau_onZ_cut_150"] = 35.;
      upperLimitsExpected["Meff_2ltau_onZ_cut_300"] = 6.8;
      upperLimitsExpected["Meff_2ltau_onZ_cut_500"] = 2.5;
      upperLimitsExpected["MeffStrong_3l_offZ_cut_0"] = 3.9;
      upperLimitsExpected["MeffStrong_3l_offZ_cut_150"] = 3.9;
      upperLimitsExpected["MeffStrong_3l_offZ_cut_300"] = 3.0;
      upperLimitsExpected["MeffStrong_3l_offZ_cut_500"] = 2.0;
      upperLimitsExpected["MeffStrong_2ltau_offZ_cut_0"] = 3.8;
      upperLimitsExpected["MeffStrong_2ltau_offZ_cut_150"] = 3.9;
      upperLimitsExpected["MeffStrong_2ltau_offZ_cut_300"] = 3.1;
      upperLimitsExpected["MeffStrong_2ltau_offZ_cut_500"] = 1.6;
      upperLimitsExpected["MeffStrong_3l_onZ_cut_0"] = 6.9;
      upperLimitsExpected["MeffStrong_3l_onZ_cut_150"] = 7.1;
      upperLimitsExpected["MeffStrong_3l_onZ_cut_300"] = 4.9;
      upperLimitsExpected["MeffStrong_3l_onZ_cut_500"] = 3.0;
      upperLimitsExpected["MeffStrong_2ltau_onZ_cut_0"] = 2.4;
      upperLimitsExpected["MeffStrong_2ltau_onZ_cut_150"] = 2.5;
      upperLimitsExpected["MeffStrong_2ltau_onZ_cut_300"] = 2.0;
      upperLimitsExpected["MeffStrong_2ltau_onZ_cut_500"] = 1.1;

      if (observed) return upperLimitsObserved[signal_region];
      else          return upperLimitsExpected[signal_region];
    }


    /// Function checking if there is an OSSF lepton pair or a combination of 3 leptons with an invariant mass close to the Z mass
    /// @todo Should the reference Z mass be 91.2?
    int isonZ (const Particles& particles)  {
      int onZ = 0;
      double best_mass_2 = 999.;
      double best_mass_3 = 999.;

      // Loop over all 2 particle combinations to find invariant mass of OSSF pair closest to Z mass
      foreach ( const Particle& p1, particles  )  {
        foreach ( const Particle& p2, particles  )  {
          double mass_difference_2_old = fabs(91.0 - best_mass_2);
          double mass_difference_2_new = fabs(91.0 - (p1.momentum() + p2.momentum()).mass()/GeV);

          // If particle combination is OSSF pair calculate mass difference to Z mass
          if ( (p1.pdgId()*p2.pdgId() == -121 || p1.pdgId()*p2.pdgId() == -169) )  {

            // Get invariant mass closest to Z mass
            if (mass_difference_2_new < mass_difference_2_old)
              best_mass_2 = (p1.momentum() + p2.momentum()).mass()/GeV;

            // In case there is an OSSF pair take also 3rd lepton into account (e.g. from FSR and photon to electron conversion)
            foreach ( const Particle & p3 , particles  )  {
              double mass_difference_3_old = fabs(91.0 - best_mass_3);
              double mass_difference_3_new = fabs(91.0 - (p1.momentum() + p2.momentum() + p3.momentum()).mass()/GeV);
              if (mass_difference_3_new < mass_difference_3_old)
                best_mass_3 = (p1.momentum() + p2.momentum() + p3.momentum()).mass()/GeV;
            }
          }
        }
      }

      // Pick the minimum invariant mass of the best OSSF pair combination and the best 3 lepton combination
      // If this mass is in a 20 GeV window around the Z mass, the event is classified as onZ
      double best_mass = min(best_mass_2, best_mass_3);
      if (fabs(91.0 - best_mass) < 20) onZ = 1;
      return onZ;
    }

    //@}


  private:

    /// Histograms
    //@{
    Histo1DPtr _h_HTlep_all, _h_HTjets_all, _h_MET_all, _h_Meff_all;
    Histo1DPtr _h_pt_1_3l, _h_pt_2_3l, _h_pt_3_3l, _h_pt_1_2ltau, _h_pt_2_2ltau, _h_pt_3_2ltau;
    Histo1DPtr _h_e_n, _h_mu_n, _h_tau_n;
    Histo1DPtr _h_excluded;
    //@}

    /// Fiducial efficiencies to model the effects of the ATLAS detector
    bool _use_fiducial_lepton_efficiency;

    /// List of signal regions and event counts per signal region
    vector<string> _signal_regions;
    map<string, double> _eventCountsPerSR;

  };



  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1204447);

}

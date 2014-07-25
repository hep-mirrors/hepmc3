// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/HadronicFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  struct ATLAS_2012_I1094568_Plots {
    // Track which veto region this is, to match the autobooked histograms
    int region_index;

    // Lower rapidity boundary or veto region
    double y_low;
    // Upper rapidity boundary or veto region
    double y_high;

    double vetoJetPt_Q0;
    double vetoJetPt_Qsum;

    // Histograms to store the veto jet pT and sum(veto jet pT) histograms.
    Histo1DPtr _h_vetoJetPt_Q0;
    Histo1DPtr _h_vetoJetPt_Qsum;

    // Scatter2Ds for the gap fractions
    Scatter2DPtr _d_gapFraction_Q0;
    Scatter2DPtr _d_gapFraction_Qsum;
  };



  /// Top pair production with central jet veto
  class ATLAS_2012_I1094568 : public Analysis {
  public:

    /// Constructor
    ATLAS_2012_I1094568()
      : Analysis("ATLAS_2012_I1094568")
    {   }


    /// Book histograms and initialise projections before the run
    void init() {

      const FinalState fs(-4.5, 4.5);
      addProjection(fs, "ALL_FS");

      /// Get electrons from truth record
      IdentifiedFinalState elec_fs(-2.47, 2.47, 25.0*GeV);
      elec_fs.acceptIdPair(PID::ELECTRON);
      addProjection(elec_fs, "ELEC_FS");

      /// Get muons which pass the initial kinematic cuts:
      IdentifiedFinalState muon_fs(-2.5, 2.5, 20.0*GeV);
      muon_fs.acceptIdPair(PID::MUON);
      addProjection(muon_fs, "MUON_FS");

      /// Get all neutrinos. These will not be used to form jets.
      /// We'll use the highest 2 pT neutrinos to calculate the MET
      IdentifiedFinalState neutrino_fs(-4.5, 4.5, 0.0*GeV);
      neutrino_fs.acceptNeutrinos();
      addProjection(neutrino_fs, "NEUTRINO_FS");

      // Final state used as input for jet-finding.
      // We include everything except the muons and neutrinos
      VetoedFinalState jet_input(fs);
      jet_input.vetoNeutrinos();
      jet_input.addVetoPairId(PID::MUON);
      addProjection(jet_input, "JET_INPUT");

      // Get the jets
      FastJets jets(jet_input, FastJets::ANTIKT, 0.4);
      addProjection(jets, "JETS");

      // Initialise weight counter
      m_total_weight = 0.0;

      // Init histogramming for the various regions
      m_plots[0].region_index = 1;
      m_plots[0].y_low = 0.0;
      m_plots[0].y_high = 0.8;
      initializePlots(m_plots[0]);
      //
      m_plots[1].region_index = 2;
      m_plots[1].y_low = 0.8;
      m_plots[1].y_high = 1.5;
      initializePlots(m_plots[1]);
      //
      m_plots[2].region_index = 3;
      m_plots[2].y_low = 1.5;
      m_plots[2].y_high = 2.1;
      initializePlots(m_plots[2]);
      //
      m_plots[3].region_index = 4;
      m_plots[3].y_low = 0.0;
      m_plots[3].y_high = 2.1;
      initializePlots(m_plots[3]);
    }


    void initializePlots(ATLAS_2012_I1094568_Plots& plots) {
      const string vetoPt_Q0_name = "vetoJetPt_Q0_" + to_str(plots.region_index);
      plots.vetoJetPt_Q0 = 0.0;
      plots._h_vetoJetPt_Q0   = bookHisto1D(vetoPt_Q0_name, 200, 0.0, 1000.0);
      plots._d_gapFraction_Q0 = bookScatter2D(plots.region_index, 1, 1);
      foreach (Point2D p, refData(plots.region_index, 1, 1).points()) {
        p.setY(0, 0);
        plots._d_gapFraction_Q0->addPoint(p);
      }

      const string vetoPt_Qsum_name = "vetoJetPt_Qsum_" + to_str(plots.region_index);
      plots._h_vetoJetPt_Qsum   = bookHisto1D(vetoPt_Qsum_name, 200, 0.0, 1000.0);
      plots._d_gapFraction_Qsum = bookScatter2D(plots.region_index, 2, 1);
      plots.vetoJetPt_Qsum = 0.0;
      foreach (Point2D p, refData(plots.region_index, 2, 1).points()) {
        p.setY(0, 0);
        plots._d_gapFraction_Qsum->addPoint(p);
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

      /// Get the various sets of final state particles
      const Particles& elecFS = applyProjection<IdentifiedFinalState>(event, "ELEC_FS").particlesByPt();
      const Particles& muonFS = applyProjection<IdentifiedFinalState>(event, "MUON_FS").particlesByPt();
      const Particles& neutrinoFS = applyProjection<IdentifiedFinalState>(event, "NEUTRINO_FS").particlesByPt();

      // Get all jets with pT > 25 GeV
      const Jets& jets = applyProjection<FastJets>(event, "JETS").jetsByPt(25.0*GeV);

      // Keep any jets that pass the initial rapidity cut
      vector<const Jet*> central_jets;
      foreach(const Jet& j, jets) {
        if (fabs(j.rapidity()) < 2.4) central_jets.push_back(&j);
      }

      // For each of the jets that pass the rapidity cut, only keep those that are not
      // too close to any leptons
      vector<const Jet*> good_jets;
      foreach(const Jet* j, central_jets) {
        bool goodJet = true;

        foreach (const Particle& e, elecFS) {
          double elec_jet_dR = deltaR(e.momentum(), j->momentum());
          if (elec_jet_dR < 0.4) { goodJet = false; break; }
        }
        if (!goodJet) continue;
        if (!goodJet) continue;

        foreach (const Particle& m, muonFS) {
          double muon_jet_dR = deltaR(m.momentum(), j->momentum());
          if (muon_jet_dR < 0.4) { goodJet = false; break; }
        }
        if (!goodJet) continue;

        good_jets.push_back(j);
      }

      // Get b hadrons with pT > 5 GeV
      /// @todo This is a hack -- replace with UnstableFinalState
      vector<HepMC3::GenParticlePtr> B_hadrons;
      vector<HepMC3::GenParticlePtr> allParticles = particles(event.genEvent());
      for (size_t i = 0; i < allParticles.size(); i++) {
        GenParticlePtr p = allParticles[i];
        if (!PID::isHadron(p->pdg_id()) || !PID::hasBottom(p->pdg_id())) continue;
        if (p->momentum().perp() < 5*GeV) continue;
        B_hadrons.push_back(p);
      }

      // For each of the good jets, check whether any are b-jets (via dR matching)
      vector<const Jet*> b_jets;
      foreach(const Jet* j, good_jets) {
        bool isbJet = false;
        foreach(HepMC3::GenParticlePtr b, B_hadrons) {
          if (deltaR(j->momentum(), FourMomentum(b->momentum())) < 0.3) isbJet = true;
        }
        if (isbJet) b_jets.push_back(j);
      }


      // Check the good jets again and keep track of the "additional jets"
      // i.e. those which are not either of the 2 highest pT b-jets
      vector<const Jet*> veto_jets;
      int n_bjets_matched = 0;
      foreach(const Jet* j, good_jets) {
        bool isBJet = false;
        foreach(const Jet* b, b_jets) {
          if (n_bjets_matched == 2) break;
          if (b == j){isBJet = true; ++ n_bjets_matched;}
        }
        if (!isBJet) veto_jets.push_back(j);
      }


      // Get the MET by taking the vector sum of all neutrinos
      /// @todo Use MissingMomentum instead?
      double MET = 0;
      FourMomentum p_MET;
      foreach (const Particle& p, neutrinoFS) {
        p_MET = p_MET + p.momentum();
      }
      MET = p_MET.pT();

      // Now we have everything we need to start doing the event selections
      bool passed_ee = false;
      vector<const Jet*> vetoJets_ee;

      // We want exactly 2 electrons...
      if (elecFS.size() == 2) {
        // ... with opposite sign charges.
        if (PID::charge(elecFS[0]) != PID::charge(elecFS[1])) {
          // Check the MET
          if (MET >= 40*GeV) {
            // Do some dilepton mass cuts
            const double dilepton_mass = (elecFS[0].momentum() + elecFS[1].momentum()).mass();
            if (dilepton_mass >= 15*GeV) {
              if (fabs(dilepton_mass - 91.0*GeV) >= 10.0*GeV) {
                // We need at least 2 b-jets
                if (b_jets.size() > 1) {
                  // This event has passed all the cuts;
                  passed_ee = true;
                }
              }
            }
          }
        }
      }

      bool passed_mumu = false;
      // Now do the same checks for the mumu channel
      vector<const Jet*> vetoJets_mumu;
      // So we now want 2 good muons...
      if (muonFS.size() == 2) {
        // ...with opposite sign charges.
        if (PID::charge(muonFS.at(0)) != PID::charge(muonFS.at(1))) {
          // Check the MET
          if (MET >= 40*GeV) {
            // and do some di-muon mass cuts
            const double dilepton_mass = (muonFS.at(0).momentum() + muonFS.at(1).momentum()).mass();
            if (dilepton_mass >= 15*GeV) {
              if (fabs(dilepton_mass - 91.0*GeV) >= 10.0*GeV) {
                // Need at least 2 b-jets
                if (b_jets.size() > 1) {
                  // This event has passed all mumu-channel cuts
                  passed_mumu = true;
                }
              }
            }
          }
        }
      }

      bool passed_emu = false;
      // Finally, the same again with the emu channel
      vector<const Jet*> vetoJets_emu;
      // We want exactly 1 electron and 1 muon
      if (elecFS.size() == 1 && muonFS.size() == 1) {
        // With opposite sign charges
        if (PID::charge(elecFS.at(0)) != PID::charge(muonFS.at(0))) {
          // Calculate HT: scalar sum of the pTs of the leptons and all good jets
          double HT = 0;
          HT += elecFS[0].pT();
          HT += muonFS[0].pT();
          foreach (const Jet* j, good_jets)
            HT += fabs(j->pT());
          // Keep events with HT > 130 GeV
          if (HT > 130.0*GeV) {
            // And again we want 2 or more b-jets
            if (b_jets.size() > 1) {
              passed_emu = true;
            }
          }
        }
      }

      if (passed_ee == true || passed_mumu == true || passed_emu == true) {
        // If the event passes the selection, we use it for all gap fractions
        m_total_weight += weight;

        // Loop over each veto jet
        foreach (const Jet* j, veto_jets) {
          const double pt = j->pT();
          const double rapidity = fabs(j->rapidity());
          // Loop over each region
          for (size_t i = 0; i < 4; ++i) {
            // If the jet falls into this region, get its pT and increment sum(pT)
            if (inRange(rapidity, m_plots[i].y_low, m_plots[i].y_high)) {
              m_plots[i].vetoJetPt_Qsum += pt;
              // If we've already got a veto jet, don't replace it
              if (m_plots[i].vetoJetPt_Q0 == 0.0) m_plots[i].vetoJetPt_Q0 = pt;
            }
          }
        }
        for (size_t i = 0; i < 4; ++i) {
          m_plots[i]._h_vetoJetPt_Q0->fill(m_plots[i].vetoJetPt_Q0, weight);
          m_plots[i]._h_vetoJetPt_Qsum->fill(m_plots[i].vetoJetPt_Qsum, weight);
          m_plots[i].vetoJetPt_Q0 = 0.0;
          m_plots[i].vetoJetPt_Qsum = 0.0;
        }
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      for (size_t i = 0; i < 4; ++i) {
        finalizeGapFraction(m_total_weight, m_plots[i]._d_gapFraction_Q0, m_plots[i]._h_vetoJetPt_Q0);
        finalizeGapFraction(m_total_weight, m_plots[i]._d_gapFraction_Qsum, m_plots[i]._h_vetoJetPt_Qsum);
      }
    }


    /// Convert temporary histos to cumulative efficiency scatters
    /// @todo Should be possible to replace this with a couple of YODA one-lines for diff -> integral and "efficiency division"
    void finalizeGapFraction(double total_weight, Scatter2DPtr gapFraction, Histo1DPtr vetoPt) {
     // Stores the cumulative frequency of the veto jet pT histogram
     double vetoPtWeightSum = 0.0;

     // Keep track of which gap fraction point we're currently populating (#final_points != #tmp_bins)
     size_t fgap_point = 0;
     for (size_t i = 0; i < vetoPt->numBins(); ++i) {
       // If we've done the last "final" point, stop
       if (fgap_point == gapFraction->numPoints()) break;

       // Increment the cumulative vetoPt counter for this temp histo bin
       /// @todo Get rid of this and use vetoPt->integral(i+1) when points and bins line up?
       vetoPtWeightSum += vetoPt->bin(i).sumW();

       // If this temp histo bin's upper edge doesn't correspond to the reference point, don't finalise the scatter.
       // Note that points are ON the bin edges and have no width: they represent the integral up to exactly that point.
       if ( !fuzzyEquals(vetoPt->bin(i).xMax(), gapFraction->point(fgap_point).x()) ) continue;

       // Calculate the gap fraction and its uncertainty
       const double frac = (total_weight != 0.0) ? vetoPtWeightSum/total_weight : 0;
       const double fracErr = (total_weight != 0.0) ? sqrt(frac*(1-frac)/total_weight) : 0;
       gapFraction->point(fgap_point).setY(frac, fracErr);

       ++fgap_point;
     }
     /// @todo Delete vetoPt temp histos: use /TMP system
    }


  private:

    // Weight counter
    double m_total_weight;

    // Structs containing all the plots, for each event selection
    ATLAS_2012_I1094568_Plots m_plots[4];

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1094568);

}

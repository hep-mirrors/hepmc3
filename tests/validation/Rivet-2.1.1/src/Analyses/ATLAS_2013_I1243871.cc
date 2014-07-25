// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Particle.hh"

namespace Rivet {


  class ATLAS_2013_I1243871 : public Analysis {
  public:

    /// Constructor
    ATLAS_2013_I1243871()
      : Analysis("ATLAS_2013_I1243871")
    {    }


    /// Book histograms and initialise projections before the run
    void init() {
      // Set up projections
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

      // Final state used as input for jet-finding.
      // We include everything except the muons and neutrinos
      VetoedFinalState jet_input(fs);
      jet_input.vetoNeutrinos();
      jet_input.addVetoPairId(PID::MUON);
      addProjection(jet_input, "JET_INPUT");

      // Get the jets
      FastJets jets(jet_input, FastJets::ANTIKT, 0.4);
      addProjection(jets, "JETS");

      // Book histograms
      for (size_t d = 0; d < 5; ++d) {
        _p_b_rho[d] = bookProfile1D(d+1, 1, 1);
        _p_l_rho[d] = bookProfile1D(d+1, 2, 1);
        _p_b_Psi[d] = bookProfile1D(d+1, 1, 2);
        _p_l_Psi[d] = bookProfile1D(d+1, 2, 2);
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      /// Get the various sets of final state particles
      const ParticleVector& elecFS = applyProjection<IdentifiedFinalState>(event, "ELEC_FS").particlesByPt();
      const ParticleVector& muonFS = applyProjection<IdentifiedFinalState>(event, "MUON_FS").particlesByPt();

      // Get all jets with pT > 7 GeV (ATLAS standard jet collection)
      /// @todo Why rewrite the jets collection as a vector of pointers?
      const Jets& jets = applyProjection<FastJets>(event, "JETS").jetsByPt(7*GeV);
      vector<const Jet*> allJets;
      foreach(const Jet& j, jets) allJets.push_back(&j);

      // Keep any jets that pass the pt cut
      vector<const Jet*> pt_jets;
      foreach (const Jet* j, allJets) {
        /// @todo Use direct kinematics access
        const double pt = j->momentum().pT();
        const double eta = j->momentum().eta();
        if (pt > 25*GeV && fabs(eta) < 2.5) pt_jets.push_back(j);
      }

      // Remove jets too close to an electron
      vector<const Jet*> good_jets;
      foreach (const Jet* j, pt_jets) {
        bool isElectron = 0;
        foreach (const Particle& e, elecFS) {
          const double elec_jet_dR = deltaR(e.momentum(), j->momentum());
          if (elec_jet_dR < 0.2) { isElectron = true; break; }
        }
        if (!isElectron) good_jets.push_back(j);
      }

      // Classify the event type
      const size_t nElec = elecFS.size();
      const size_t nMuon = muonFS.size();
      bool isSemilepton = false, isDilepton = false;
      if (nElec == 1 && nMuon == 0) {
        isSemilepton = true;
      } else if (nElec == 0 && nMuon == 1) {
        isSemilepton = true;
      } else if (nElec == 2 && nMuon == 0) {
        if (PID::charge(elecFS[0]) != PID::charge(elecFS[1])) isDilepton = true;
      } else if (nElec == 1 && nMuon == 1) {
        if (PID::charge(elecFS[0]) != PID::charge(muonFS[0])) isDilepton = true;
      } else if (nElec == 0 && nMuon == 2) {
        if (PID::charge(muonFS[0]) != PID::charge(muonFS[1])) isDilepton = true;
      }
      const bool isGoodEvent = (isSemilepton && good_jets.size() >= 4) || (isDilepton && good_jets.size() >= 2);
      if (!isGoodEvent) vetoEvent;

      // Select b-hadrons
      /// @todo Use built-in identification on Particle, avoid HepMC3
      vector<HepMC3::GenParticlePtr> b_hadrons;
      vector<HepMC3::GenParticlePtr> allParticles = particles(event.genEvent());
      for(unsigned int i = 0; i < allParticles.size(); i++) {
        GenParticlePtr p = allParticles.at(i);
        if ( !(Rivet::PID::isHadron( p->pdg_id() ) && Rivet::PID::hasBottom( p->pdg_id() )) ) continue;
        if (p->momentum().perp() < 5*GeV) continue;
        b_hadrons.push_back(p);
      }

      // Select b-jets as those containing a b-hadron
      /// @todo Use built-in dR < 0.3 Jet tagging, avoid HepMC3
      vector<const Jet*> b_jets;
      foreach (const Jet* j, good_jets) {
        bool isbJet = false;
        foreach (HepMC3::GenParticlePtr b, b_hadrons) {
          /// @todo Use direct momentum accessor / delta functions
          const FourMomentum hadron = b->momentum();
          const double hadron_jet_dR = deltaR(j->momentum(), hadron);
          if (hadron_jet_dR < 0.3) { isbJet = true; break; }
        }
        // Check if it is overlapped to any other jet
        bool isOverlapped = false;
        foreach (const Jet* k, allJets) {
          if (j == k) continue;
          double dRjj = deltaR(j->momentum(), k->momentum());
          if (dRjj < 0.8) { isOverlapped = true; break; }
        }
        if (isbJet && !isOverlapped) b_jets.push_back(j);
      }
      MSG_DEBUG(b_jets.size() << " b-jets selected");


      // Select light-jets as the pair of non-b-jets with invariant mass closest to the W mass
      /// @todo Use built-in b-tagging (dR < 0.3 defn), avoid HepMC3
      const double nominalW = 80.4*GeV;
      double deltaM = 500*GeV;
      const Jet* light1 = NULL; const Jet* light2 = NULL; // NB: const Jets, not const pointers!
      foreach (const Jet* i, good_jets) {
        bool isbJet1 = false;
        foreach (HepMC3::GenParticlePtr b, b_hadrons) {
          /// @todo Use direct momentum accessor / delta functions
          const FourMomentum hadron = b->momentum();
          const double hadron_jet_dR = deltaR(i->momentum(), hadron);
          if (hadron_jet_dR < 0.3) { isbJet1 = true; break; }
        }
        if (isbJet1) continue;
        foreach (const Jet* j, good_jets) {
          bool isbJet2 = false;
          foreach (HepMC3::GenParticlePtr b, b_hadrons) {
            FourMomentum hadron = b->momentum();
            double hadron_jet_dR = deltaR(j->momentum(), hadron);
            if (hadron_jet_dR < 0.3) { isbJet2 = true; break; }
          }
          if (isbJet2) continue;
          double invMass = (i->momentum()+j->momentum()).mass();
          if (fabs(invMass-nominalW) < deltaM){
            deltaM = fabs(invMass - nominalW);
            light1 = i;
            light2 = j;
          }
        }
      }

      // Check that both jets are not overlapped, and populate the light jets list
      vector<const Jet*> light_jets;
      const bool hasGoodLight = light1 != NULL && light2 != NULL && light1 != light2;
      if (hasGoodLight) {
        bool isOverlap1 = false, isOverlap2 = false;
        foreach (const Jet* j, allJets) {
          if (light1 == j) continue;
          const double dR1j = deltaR(light1->momentum(), j->momentum());
          if (dR1j < 0.8) { isOverlap1 = true; break; }
        }
        foreach (const Jet* j, allJets) {
          if (light2 == j) continue;
          const double dR2j = deltaR(light2->momentum(), j->momentum());
          if (dR2j < 0.8) { isOverlap2 = true; break; }
        }
        if (!isOverlap1 && !isOverlap2) {
          light_jets.push_back(light1);
          light_jets.push_back(light2);
        }
      }
      MSG_DEBUG(light_jets.size() << " light jets selected");


      // Calculate the jet shapes
      /// @todo Use C++11 vector/array initialization
      const double binWidth = 0.04; // -> 10 bins from 0.0-0.4
      vector<double> ptEdges; ptEdges += 30, 40, 50, 70, 100, 150;

      // b-jet shapes
      MSG_DEBUG("Filling b-jet shapes");
      foreach (const Jet* bJet, b_jets) {
        // Work out jet pT bin and skip this jet if out of range
        const double jetPt = bJet->momentum().pT();
        MSG_DEBUG("Jet pT = " << jetPt/GeV << " GeV");
        if (!inRange(jetPt/GeV, 30., 150.)) continue;
        /// @todo Use YODA bin index lookup tools
        size_t ipt; for (ipt = 0; ipt < 5; ++ipt) if (inRange(jetPt/GeV, ptEdges[ipt], ptEdges[ipt+1])) break;
        MSG_DEBUG("Jet pT index = " << ipt);

        // Calculate jet shape
        vector<double> rings(10, 0);
        foreach (const Particle& p, bJet->particles()) {
          const double dR = deltaR(bJet->momentum(), p.momentum());
          const size_t idR = (size_t) floor(dR/binWidth);
          for (size_t i = idR; i < 10; ++i) rings[i] += p.momentum().pT();
        }

        // Fill each dR bin of the histos for this jet pT
        for (int iBin = 0; iBin < 10; ++iBin) {
          const double rcenter = 0.02 + iBin*binWidth;
          const double rhoval = (iBin != 0 ? (rings[iBin]-rings[iBin-1]) : rings[iBin]) / binWidth / rings[9];
          const double psival = rings[iBin] / rings[9];
          MSG_DEBUG(rcenter << ", " << rhoval << ", " << psival);
          _p_b_rho[ipt]->fill(rcenter, rhoval, weight);
          _p_b_Psi[ipt]->fill(rcenter, psival, weight);
        }
      }

      // Light jet shapes
      MSG_DEBUG("Filling light jet shapes");
      foreach (const Jet* lJet, light_jets) {
        // Work out jet pT bin and skip this jet if out of range
        const double jetPt = lJet->momentum().pT();
        MSG_DEBUG("Jet pT = " << jetPt/GeV << " GeV");
        if (!inRange(jetPt/GeV, 30., 150.)) continue;
        /// @todo Use YODA bin index lookup tools
        size_t ipt; for (ipt = 0; ipt < 5; ++ipt) if (inRange(jetPt/GeV, ptEdges[ipt], ptEdges[ipt+1])) break;
        MSG_DEBUG("Jet pT index = " << ipt);

        // Calculate jet shape
        vector<double> rings(10, 0);
        foreach (const Particle& p, lJet->particles()) {
          const double dR = deltaR(lJet->momentum(), p.momentum());
          const size_t idR = (size_t) floor(dR/binWidth);
          for (size_t i = idR; i < 10; ++i) rings[i] += p.momentum().pT();
        }

        // Fill each dR bin of the histos for this jet pT
        for (int iBin = 0; iBin < 10; ++iBin) {
          const double rcenter = 0.02 + iBin*binWidth;
          const double rhoval = (iBin != 0 ? (rings[iBin]-rings[iBin-1]) : rings[iBin]) / binWidth / rings[9];
          const double psival = rings[iBin] / rings[9];
          _p_l_rho[ipt]->fill(rcenter, rhoval, weight);
          _p_l_Psi[ipt]->fill(rcenter, psival, weight);
        }
      }

    }


  private:

    Profile1DPtr _p_b_rho[5];
    Profile1DPtr _p_l_rho[5];
    Profile1DPtr _p_b_Psi[5];
    Profile1DPtr _p_l_Psi[5];

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2013_I1243871);

}

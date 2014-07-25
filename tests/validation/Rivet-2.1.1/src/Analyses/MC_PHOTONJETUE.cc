// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for underlying event in jet + isolated photon events
  /// @author Andy Buckley
  class MC_PHOTONJETUE : public Analysis {
  public:

    /// Constructor
    MC_PHOTONJETUE()
      : Analysis("MC_PHOTONJETUE")
    {    }


    /// @name Analysis methods
    //@{

    // Book histograms and projections
    void init() {
      // Final state for the jet finding
      const FinalState fsj(-4.0, 4.0, 0.1*GeV);
      addProjection(fsj, "FSJ");
      addProjection(FastJets(fsj, FastJets::ANTIKT, 0.7), "Jets");

      // Charged final state for the distributions
      const ChargedFinalState cfs(-2.0, 2.0, 0.2*GeV);
      addProjection(cfs, "Tracks");

      // Photons (for isolation)
      const FinalState fsp(-2.0, 2.0, 20.0*GeV);
      IdentifiedFinalState photonfs(fsp);
      photonfs.acceptId(PID::PHOTON);
      addProjection(photonfs, "Photons");

      // Histograms
      _hist_jetgamma_dR   = bookHisto1D("gammajet-dR", 52, 0.0, 5.2);
      _hist_jetgamma_dphi = bookHisto1D("gammajet-dphi", 50, 0.0, PI);
      //
      const double MAXPT1 = 50.0;
      _hist_pnchg_jet      = bookProfile1D("trans-nchg-jet",     50, 0.0, MAXPT1);
      _hist_pmaxnchg_jet   = bookProfile1D("trans-maxnchg-jet",  50, 0.0, MAXPT1);
      _hist_pminnchg_jet   = bookProfile1D("trans-minnchg-jet",  50, 0.0, MAXPT1);
      _hist_pcptsum_jet    = bookProfile1D("trans-ptsum-jet",    50, 0.0, MAXPT1);
      _hist_pmaxcptsum_jet = bookProfile1D("trans-maxptsum-jet", 50, 0.0, MAXPT1);
      _hist_pmincptsum_jet = bookProfile1D("trans-minptsum-jet", 50, 0.0, MAXPT1);
      _hist_pcptave_jet    = bookProfile1D("trans-ptavg-jet",    50, 0.0, MAXPT1);
      //
      _hist_pnchg_gamma      = bookProfile1D("trans-nchg-gamma",     50, 0.0, MAXPT1);
      _hist_pmaxnchg_gamma   = bookProfile1D("trans-maxnchg-gamma",  50, 0.0, MAXPT1);
      _hist_pminnchg_gamma   = bookProfile1D("trans-minnchg-gamma",  50, 0.0, MAXPT1);
      _hist_pcptsum_gamma    = bookProfile1D("trans-ptsum-gamma",    50, 0.0, MAXPT1);
      _hist_pmaxcptsum_gamma = bookProfile1D("trans-maxptsum-gamma", 50, 0.0, MAXPT1);
      _hist_pmincptsum_gamma = bookProfile1D("trans-minptsum-gamma", 50, 0.0, MAXPT1);
      _hist_pcptave_gamma    = bookProfile1D("trans-ptavg-gamma",    50, 0.0, MAXPT1);
    }


    // Do the analysis
    void analyze(const Event& evt) {

      // Get jets
      const Jets jets = applyProjection<FastJets>(evt, "Jets").jetsByPt();
      MSG_DEBUG("Jet multiplicity = " << jets.size());
      if (jets.size() < 1) {
        MSG_DEBUG("No jets found");
        vetoEvent;
      }

      // Get leading jet properties
      const FourMomentum pjet = jets.front().momentum();
      const double jeteta = pjet.eta();
      const double jetphi = pjet.phi();
      const double jetpT  = pjet.pT();
      MSG_DEBUG("Leading jet: pT = " << jetpT/GeV << " GeV"
               << ", eta = " << jeteta
               << ", phi = " << jetphi);

      // Require the leading jet to be within |eta| < 2
      if (fabs(jeteta) > 2) {
        MSG_DEBUG("Failed leading jet eta cut");
        vetoEvent;
      }

      // Get the leading photon
      const FinalState& photonfs = applyProjection<FinalState>(evt, "Photons");
      if (photonfs.size() < 1) {
        MSG_DEBUG("No hard photons found");
        vetoEvent;
      }
      const FourMomentum pgamma = photonfs.particlesByPt().front().momentum();

      // Check that leading photon is isolated from jets
      bool isolated = true;
      foreach (const Jet& j, jets) {
        if (deltaR(j.momentum(), pgamma) < 0.1) {
          isolated = false;
          break;
        }
      }
      if (!isolated) {
        MSG_DEBUG("Leading photon is not isolated from jets");
        vetoEvent;
      }

      // Get leading photon properties
      const double gammaeta = pgamma.eta();
      const double gammaphi = pgamma.phi();
      const double gammapT  = pgamma.pT();
      MSG_DEBUG("Leading photon: pT = " << gammapT/GeV << " GeV"
                << ", eta = " << gammaeta
                << ", phi = " << gammaphi);

      // Get the event weight
      const double weight = evt.weight();

      // Fill jet1-photon separation histos
      const double dR_jetgamma = deltaR(pgamma, pjet);
      _hist_jetgamma_dR->fill(dR_jetgamma, weight);
      const double dPhi_jetgamma = deltaPhi(gammaphi, jetphi);
      _hist_jetgamma_dphi->fill(dPhi_jetgamma, weight);

      /// Cut on back-to-backness of jet-photon
      if (dPhi_jetgamma < 0.5) vetoEvent;

      /// @todo Plot evolution of UE as a function of jet-photon angle
      /// @todo Plot evolution of UE as a function of photon pT

      // Get the final states to work with for filling the distributions
      const FinalState& cfs = applyProjection<ChargedFinalState>(evt, "Tracks");

      // Whole-event counters
      unsigned int   numOverall(0);
      double ptSumOverall(0.0), ptMaxOverall(0.0);
      // Jet-oriented counters
      unsigned int   numToward_jet(0),     numTrans1_jet(0),     numTrans2_jet(0),     numAway_jet(0);
      double ptSumToward_jet(0.0), ptSumTrans1_jet(0.0), ptSumTrans2_jet(0.0), ptSumAway_jet(0.0);
      double ptMaxToward_jet(0.0), ptMaxTrans1_jet(0.0), ptMaxTrans2_jet(0.0), ptMaxAway_jet(0.0);
      // Photon-oriented counters
      unsigned int   numToward_gamma(0),     numTrans1_gamma(0),     numTrans2_gamma(0),     numAway_gamma(0);
      double ptSumToward_gamma(0.0), ptSumTrans1_gamma(0.0), ptSumTrans2_gamma(0.0), ptSumAway_gamma(0.0);
      double ptMaxToward_gamma(0.0), ptMaxTrans1_gamma(0.0), ptMaxTrans2_gamma(0.0), ptMaxAway_gamma(0.0);

      // Calculate all the charged stuff
      foreach (const Particle& p, cfs.particles()) {
        ++numOverall;
        const double pT = p.pT();
        ptSumOverall += pT;
        if (pT > ptMaxOverall) ptMaxOverall = pT;

        // Increment jet-oriented variables
        const double dPhi_jet = jetphi - p.momentum().phi();
        if (fabs(dPhi_jet) < PI/3.0) {
          ptSumToward_jet += pT;
          ++numToward_jet;
          if (pT > ptMaxToward_jet) ptMaxToward_jet = pT;
        }
        else if (fabs(dPhi_jet) < 2*PI/3.0) {
          if (sign(dPhi_jet) == MINUS) {
            ptSumTrans1_jet += pT;
            ++numTrans1_jet;
            if (pT > ptMaxTrans1_jet) ptMaxTrans1_jet = pT;
          } else {
            ptSumTrans2_jet += pT;
            ++numTrans2_jet;
            if (pT > ptMaxTrans2_jet) ptMaxTrans2_jet = pT;
          }
        }
        else {
          ptSumAway_jet += pT;
          ++numAway_jet;
          if (pT > ptMaxAway_jet) ptMaxAway_jet = pT;
        }


        // Increment photon-oriented variables
        const double dPhi_gamma = gammaphi - p.momentum().phi();
        if (fabs(dPhi_gamma) < PI/3.0) {
          ptSumToward_gamma += pT;
          ++numToward_gamma;
          if (pT > ptMaxToward_gamma) ptMaxToward_gamma = pT;
        }
        else if (fabs(dPhi_gamma) < 2*PI/3.0) {
          if (sign(dPhi_gamma) == MINUS) {
            ptSumTrans1_gamma += pT;
            ++numTrans1_gamma;
            if (pT > ptMaxTrans1_gamma) ptMaxTrans1_gamma = pT;
          } else {
            ptSumTrans2_gamma += pT;
            ++numTrans2_gamma;
            if (pT > ptMaxTrans2_gamma) ptMaxTrans2_gamma = pT;
          }
        }
        else {
          ptSumAway_gamma += pT;
          ++numAway_gamma;
          if (pT > ptMaxAway_gamma) ptMaxAway_gamma = pT;
        }


      }


      // Fill the histograms
      _hist_pnchg_jet->fill(jetpT/GeV, (numTrans1_jet+numTrans2_jet)/(4*PI/3), weight);
      _hist_pmaxnchg_jet->fill(jetpT/GeV, (numTrans1_jet>numTrans2_jet ? numTrans1_jet : numTrans2_jet)/(2*PI/3), weight);
      _hist_pminnchg_jet->fill(jetpT/GeV, (numTrans1_jet<numTrans2_jet ? numTrans1_jet : numTrans2_jet)/(2*PI/3), weight);
      _hist_pcptsum_jet->fill(jetpT/GeV, (ptSumTrans1_jet+ptSumTrans2_jet)/GeV/(4*PI/3), weight);
      _hist_pmaxcptsum_jet->fill(jetpT/GeV, (ptSumTrans1_jet>ptSumTrans2_jet ? ptSumTrans1_jet : ptSumTrans2_jet)/GeV/(2*PI/3), weight);
      _hist_pmincptsum_jet->fill(jetpT/GeV, (ptSumTrans1_jet<ptSumTrans2_jet ? ptSumTrans1_jet : ptSumTrans2_jet)/GeV/(2*PI/3), weight);
      if ((numTrans1_jet+numTrans2_jet) > 0) {
        _hist_pcptave_jet->fill(jetpT/GeV, (ptSumTrans1_jet+ptSumTrans2_jet)/GeV/(numTrans1_jet+numTrans2_jet), weight);
      }
      //
      _hist_pnchg_gamma->fill(gammapT/GeV, (numTrans1_gamma+numTrans2_gamma)/(4*PI/3), weight);
      _hist_pmaxnchg_gamma->fill(gammapT/GeV, (numTrans1_gamma>numTrans2_gamma ? numTrans1_gamma : numTrans2_gamma)/(2*PI/3), weight);
      _hist_pminnchg_gamma->fill(gammapT/GeV, (numTrans1_gamma<numTrans2_gamma ? numTrans1_gamma : numTrans2_gamma)/(2*PI/3), weight);
      _hist_pcptsum_gamma->fill(gammapT/GeV, (ptSumTrans1_gamma+ptSumTrans2_gamma)/GeV/(4*PI/3), weight);
      _hist_pmaxcptsum_gamma->fill(gammapT/GeV, (ptSumTrans1_gamma>ptSumTrans2_gamma ? ptSumTrans1_gamma : ptSumTrans2_gamma)/GeV/(2*PI/3), weight);
      _hist_pmincptsum_gamma->fill(gammapT/GeV, (ptSumTrans1_gamma<ptSumTrans2_gamma ? ptSumTrans1_gamma : ptSumTrans2_gamma)/GeV/(2*PI/3), weight);
      if ((numTrans1_gamma+numTrans2_gamma) > 0) {
        _hist_pcptave_gamma->fill(gammapT/GeV, (ptSumTrans1_gamma+ptSumTrans2_gamma)/GeV/(numTrans1_gamma+numTrans2_gamma), weight);
      }

    }


    void finalize() {
      //
    }


  private:

    Histo1DPtr _hist_jetgamma_dR;
    Histo1DPtr _hist_jetgamma_dphi;

    Profile1DPtr _hist_pnchg_jet, _hist_pnchg_gamma;
    Profile1DPtr _hist_pmaxnchg_jet, _hist_pmaxnchg_gamma;
    Profile1DPtr _hist_pminnchg_jet, _hist_pminnchg_gamma;
    Profile1DPtr _hist_pcptsum_jet, _hist_pcptsum_gamma;
    Profile1DPtr _hist_pmaxcptsum_jet, _hist_pmaxcptsum_gamma;
    Profile1DPtr _hist_pmincptsum_jet, _hist_pmincptsum_gamma;
    Profile1DPtr _hist_pcptave_jet, _hist_pcptave_gamma;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_PHOTONJETUE);

}

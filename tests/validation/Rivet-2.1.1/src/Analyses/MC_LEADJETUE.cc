// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for underlying event in jet events
  /// @author Andy Buckley
  class MC_LEADJETUE : public Analysis {
  public:

    /// Constructor
    MC_LEADJETUE()
      : Analysis("MC_LEADJETUE")
    {    }


    /// @name Analysis methods
    //@{

    // Book histograms
    void init() {
      // Final state for the jet finding
      const FinalState fsj(-4.0, 4.0, 0.0*GeV);
      addProjection(fsj, "FSJ");
      addProjection(FastJets(fsj, FastJets::KT, 0.7), "Jets");

      // Charged final state for the distributions
      const ChargedFinalState cfs(-1.0, 1.0, 0.5*GeV);
      addProjection(cfs, "CFS");

      const double maxpt1 = 500.0;
      _hist_pnchg      = bookProfile1D("trans-nchg", 50, 0.0, maxpt1);
      _hist_pmaxnchg   = bookProfile1D("trans-maxnchg", 50, 0.0, maxpt1);
      _hist_pminnchg   = bookProfile1D("trans-minnchg", 50, 0.0, maxpt1);
      _hist_pcptsum    = bookProfile1D("trans-ptsum", 50, 0.0, maxpt1);
      _hist_pmaxcptsum = bookProfile1D("trans-maxptsum", 50, 0.0, maxpt1);
      _hist_pmincptsum = bookProfile1D("trans-minptsum", 50, 0.0, maxpt1);
      _hist_pcptave    = bookProfile1D("trans-ptavg", 50, 0.0, maxpt1);
    }


    // Do the analysis
    void analyze(const Event& e) {

      const FinalState& fsj = applyProjection<FinalState>(e, "FSJ");
      if (fsj.particles().empty()) {
        MSG_DEBUG("Failed multiplicity cut");
        vetoEvent;
      }

      const FastJets& jetpro = applyProjection<FastJets>(e, "Jets");
      const Jets jets = jetpro.jetsByPt();
      MSG_DEBUG("Jet multiplicity = " << jets.size());

      // Require the leading jet to be within |eta| < 2
      if (jets.size() < 1 || fabs(jets[0].momentum().pseudorapidity()) > 2) {
        MSG_DEBUG("Failed jet cut");
        vetoEvent;
      }

      const double jetphi = jets[0].momentum().phi();
      const double jetpT  = jets[0].pT();
      MSG_DEBUG("Leading jet: pT = " << jetpT/GeV << " GeV"
                << ", eta = " << jets[0].momentum().pseudorapidity()
                << ", phi = " << jetphi);

      // Get the event weight
      const double weight = e.weight();

      // Get the final states to work with for filling the distributions
      const FinalState& cfs = applyProjection<ChargedFinalState>(e, "CFS");

      size_t   numOverall(0),     numToward(0),     numTrans1(0),     numTrans2(0),     numAway(0);
      double ptSumOverall(0.0), ptSumToward(0.0), ptSumTrans1(0.0), ptSumTrans2(0.0), ptSumAway(0.0);
      double ptMaxOverall(0.0), ptMaxToward(0.0), ptMaxTrans1(0.0), ptMaxTrans2(0.0), ptMaxAway(0.0);

      // Calculate all the charged stuff
      foreach (const Particle& p, cfs.particles()) {
        const double dPhi = deltaPhi(p.momentum().phi(), jetphi);
        const double pT = p.pT();
        const double phi = p.momentum().azimuthalAngle();
        const double rotatedphi = phi - jetphi;

        ptSumOverall += pT;
        ++numOverall;
        if (pT > ptMaxOverall) ptMaxOverall = pT;

        if (dPhi < PI/3.0) {
          ptSumToward += pT;
          ++numToward;
          if (pT > ptMaxToward) ptMaxToward = pT;
        }
        else if (dPhi < 2*PI/3.0) {
          if (rotatedphi <= PI) {
            ptSumTrans1 += pT;
            ++numTrans1;
            if (pT > ptMaxTrans1) ptMaxTrans1 = pT;
          } else {
            ptSumTrans2 += pT;
            ++numTrans2;
            if (pT > ptMaxTrans2) ptMaxTrans2 = pT;
          }
        }
        else {
          ptSumAway += pT;
          ++numAway;
          if (pT > ptMaxAway) ptMaxAway = pT;
        }
      }


      // Fill the histograms
      //_hist_tnchg->fill(jetpT/GeV, numToward/(4*PI/3), weight);
      _hist_pnchg->fill(jetpT/GeV, (numTrans1+numTrans2)/(4*PI/3), weight);
      _hist_pmaxnchg->fill(jetpT/GeV, (numTrans1>numTrans2 ? numTrans1 : numTrans2)/(2*PI/3), weight);
      _hist_pminnchg->fill(jetpT/GeV, (numTrans1<numTrans2 ? numTrans1 : numTrans2)/(2*PI/3), weight);
      //_hist_pdifnchg->fill(jetpT/GeV, abs(numTrans1-numTrans2)/(2*PI/3), weight);
      //_hist_anchg->fill(jetpT/GeV, numAway/(4*PI/3), weight);

      //_hist_tcptsum->fill(jetpT/GeV, ptSumToward/GeV/(4*PI/3), weight);
      _hist_pcptsum->fill(jetpT/GeV, (ptSumTrans1+ptSumTrans2)/GeV/(4*PI/3), weight);
      _hist_pmaxcptsum->fill(jetpT/GeV, (ptSumTrans1>ptSumTrans2 ? ptSumTrans1 : ptSumTrans2)/GeV/(2*PI/3), weight);
      _hist_pmincptsum->fill(jetpT/GeV, (ptSumTrans1<ptSumTrans2 ? ptSumTrans1 : ptSumTrans2)/GeV/(2*PI/3), weight);
      //_hist_pdifcptsum->fill(jetpT/GeV, fabs(ptSumTrans1-ptSumTrans2)/GeV/(2*PI/3), weight);
      //_hist_acptsum->fill(jetpT/GeV, ptSumAway/GeV/(4*PI/3), weight);

      //if (numToward > 0) {
      //  _hist_tcptave->fill(jetpT/GeV, ptSumToward/GeV/numToward, weight);
      //  _hist_tcptmax->fill(jetpT/GeV, ptMaxToward/GeV, weight);
      //}
      if ((numTrans1+numTrans2) > 0) {
        _hist_pcptave->fill(jetpT/GeV, (ptSumTrans1+ptSumTrans2)/GeV/(numTrans1+numTrans2), weight);
        //_hist_pcptmax->fill(jetpT/GeV, (ptMaxTrans1 > ptMaxTrans2 ? ptMaxTrans1 : ptMaxTrans2)/GeV, weight);
      }
      //if (numAway > 0) {
      //  _hist_acptave->fill(jetpT/GeV, ptSumAway/GeV/numAway, weight);
      //  _hist_acptmax->fill(jetpT/GeV, ptMaxAway/GeV, weight);
      //}
    }


    void finalize() {
      //
    }


  private:

    Profile1DPtr _hist_pnchg;
    Profile1DPtr _hist_pmaxnchg;
    Profile1DPtr _hist_pminnchg;
    Profile1DPtr _hist_pcptsum;
    Profile1DPtr _hist_pmaxcptsum;
    Profile1DPtr _hist_pmincptsum;
    Profile1DPtr _hist_pcptave;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_LEADJETUE);

}

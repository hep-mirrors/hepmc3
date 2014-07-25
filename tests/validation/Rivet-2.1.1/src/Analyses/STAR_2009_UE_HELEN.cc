// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/NeutralFinalState.hh"
#include "Rivet/Projections/MergedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "fastjet/SISConePlugin.hh"

namespace Rivet {


  /// @brief STAR underlying event
  /// @author Hendrik Hoeth
  class STAR_2009_UE_HELEN : public Analysis {
  public:

    /// Constructor
    STAR_2009_UE_HELEN()
      : Analysis("STAR_2009_UE_HELEN")
    {
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Charged final state, |eta|<1, pT>0.2GeV
      const ChargedFinalState cfs(-1.0, 1.0, 0.2*GeV);
      addProjection(cfs, "CFS");

      // Neutral final state, |eta|<1, ET>0.2GeV (needed for the jets)
      const NeutralFinalState nfs(-1.0, 1.0, 0.2*GeV);
      addProjection(nfs, "NFS");

      // STAR can't see neutrons and K^0_L
      VetoedFinalState vfs(nfs);
      vfs.vetoNeutrinos();
      vfs.addVetoPairId(PID::K0L);
      vfs.addVetoPairId(PID::NEUTRON);
      addProjection(vfs, "VFS");

      // Jets are reconstructed from charged and neutral particles,
      // and the cuts are different (pT vs. ET), so we need to merge them.
      const MergedFinalState jfs(cfs, vfs);
      addProjection(jfs, "JFS");

      // SISCone, R = 0.7, overlap_threshold = 0.75
      addProjection(FastJets(jfs, FastJets::SISCONE, 0.7), "AllJets");

      // Book histograms
      _hist_pmaxnchg   = bookProfile1D( 1, 1, 1);
      _hist_pminnchg   = bookProfile1D( 2, 1, 1);
      _hist_anchg      = bookProfile1D( 3, 1, 1);
    }


    // Do the analysis
    void analyze(const Event& e) {
      const FinalState& cfs = applyProjection<ChargedFinalState>(e, "CFS");
      if (cfs.particles().size() < 1) {
        MSG_DEBUG("Failed multiplicity cut");
        vetoEvent;
      }

      const Jets& alljets = applyProjection<FastJets>(e, "AllJets").jetsByPt();
      MSG_DEBUG("Total jet multiplicity = " << alljets.size());

      // The jet acceptance region is |eta|<(1-R)=0.3  (with R = jet radius)
      // Jets also must have a neutral energy fraction of < 0.7
      Jets jets;
      foreach (const Jet jet, alljets) {
        if (jet.neutralEnergy()/jet.totalEnergy() < 0.7 &&
	    fabs(jet.eta()) < 0.3)
          jets.push_back(jet);
      }

      // This analysis requires a di-jet like event.
      // WARNING: There is more data in preparation, some of which
      //          does _not_ have this constraint!
      if (jets.size() != 2) {
        MSG_DEBUG("Failed jet multiplicity cut");
        vetoEvent;
      }

      // The di-jet constraints in this analysis are:
      // - 2 and only 2 jets in the acceptance region
      // - delta(Phi) between the jets is > 150 degrees
      // - Pt_awayjet/Pt_towards_jet > 0.7
      if (deltaPhi(jets[0].momentum().phi(), jets[1].momentum().phi()) <= 5*PI/6 ||
          jets[1].pT()/jets[0].pT() <= 0.7)
      {
        MSG_DEBUG("Failed di-jet criteria");
        vetoEvent;
      }

      // Now lets start ...
      const double jetphi = jets[0].momentum().phi();
      const double jetpT  = jets[0].pT();

      // Get the event weight
      const double weight = e.weight();

      size_t numTrans1(0), numTrans2(0), numAway(0);

      // Calculate all the charged stuff
      foreach (const Particle& p, cfs.particles()) {
        const double dPhi = deltaPhi(p.momentum().phi(), jetphi);
        const double pT = p.pT();
        const double phi = p.momentum().phi();
        double rotatedphi = phi - jetphi;
        while (rotatedphi < 0) rotatedphi += 2*PI;

        // @TODO: WARNING: The following lines are a hack to correct
        //        for the STAR tracking efficiency. Once we have the
        //        final numbers (corrected to hadron level), we need
        //        to remove this!!!!
        if (1.0*rand()/static_cast<double>(RAND_MAX) > 0.87834-exp(-1.48994-0.788432*pT)) {
          continue;
        }
        // -------- end of efficiency hack -------

        if (dPhi < PI/3.0) {
          // toward
        }
        else if (dPhi < 2*PI/3.0) {
          if (rotatedphi <= PI) {
            ++numTrans1;
          }
          else {
            ++numTrans2;
          }
        }
        else {
          ++numAway;
        }
      } // end charged particle loop

      // Fill the histograms
      _hist_pmaxnchg->fill(jetpT, (numTrans1>numTrans2 ? numTrans1 : numTrans2)/(2*PI/3), weight);
      _hist_pminnchg->fill(jetpT, (numTrans1<numTrans2 ? numTrans1 : numTrans2)/(2*PI/3), weight);
      _hist_anchg->fill(jetpT, numAway/(PI*0.7*0.7), weight); // jet area = pi*R^2

    }


    void finalize() {
      //
    }

    //@}


  private:

    Profile1DPtr _hist_pmaxnchg;
    Profile1DPtr _hist_pminnchg;
    Profile1DPtr _hist_anchg;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2009_UE_HELEN);

}

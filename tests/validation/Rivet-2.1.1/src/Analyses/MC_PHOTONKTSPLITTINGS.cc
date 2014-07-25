// -*- C++ -*-
#include "Rivet/Analyses/MC_JetSplittings.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for photon + jets events
  class MC_PHOTONKTSPLITTINGS : public MC_JetSplittings {
  public:

    /// Default constructor
    MC_PHOTONKTSPLITTINGS()
      : MC_JetSplittings("MC_PHOTONKTSPLITTINGS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      // General FS
      FinalState fs(-5.0, 5.0);
      addProjection(fs, "FS");

      // Get leading photon
      LeadingParticlesFinalState photonfs(FinalState(-1.0, 1.0, 30.0*GeV));
      photonfs.addParticleId(PID::PHOTON);
      addProjection(photonfs, "LeadingPhoton");

      // FS for jets excludes the leading photon
      VetoedFinalState vfs(fs);
      vfs.addVetoOnThisFinalState(photonfs);
      addProjection(vfs, "JetFS");
      FastJets jetpro(vfs, FastJets::KT, 0.6);
      addProjection(jetpro, "Jets");

      MC_JetSplittings::init();
    }


    /// Do the analysis
    void analyze(const Event& e) {
      // Get the photon
      const Particles photons = applyProjection<FinalState>(e, "LeadingPhoton").particles();
      if (photons.size() != 1) {
        vetoEvent;
      }
      const FourMomentum photon = photons.front().momentum();

      // Get all charged particles
      const FinalState& fs = applyProjection<FinalState>(e, "JetFS");
      if (fs.empty()) {
        vetoEvent;
      }

      // Isolate photon by ensuring that a 0.4 cone around it contains less than 7% of the photon's energy
      const double egamma = photon.E();
      double econe = 0.0;
      foreach (const Particle& p, fs.particles()) {
        if (deltaR(photon, p.momentum()) < 0.4) {
          econe += p.momentum().E();
          // Veto as soon as E_cone gets larger
          if (econe/egamma > 0.07) {
            vetoEvent;
          }
        }
      }

      MC_JetSplittings::analyze(e);
    }


    // Finalize
    void finalize() {
      MC_JetSplittings::finalize();
    }

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_PHOTONKTSPLITTINGS);

}

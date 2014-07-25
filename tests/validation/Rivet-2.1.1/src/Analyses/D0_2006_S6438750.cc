// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief D0 inclusive isolated photon cross-section vs. \f$ p_\perp(gamma) \f$.
  /// @author Andy Buckley
  /// @author Gavin Hesketh
  class D0_2006_S6438750 : public Analysis {

  public:

    /// @name Constructors etc.
    //@{

    /// Default constructor.
    D0_2006_S6438750()
      : Analysis("D0_2006_S6438750")
    {    }

    //@}


    /// @name Analysis methods
    //@{

    void init() {
      // General FS for photon isolation
      FinalState fs;
      addProjection(fs, "AllFS");

      // Get leading photon
      LeadingParticlesFinalState photonfs(FinalState(-0.9, 0.9, 23.0*GeV));
      photonfs.addParticleId(PID::PHOTON);
      addProjection(photonfs, "LeadingPhoton");

      // Book histograms
      _h_pTgamma = bookHisto1D(1, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {

      // Get the photon
      const FinalState& photonfs = applyProjection<FinalState>(event, "LeadingPhoton");
      if (photonfs.particles().size() != 1) {
        vetoEvent;
      }
      const FourMomentum photon = photonfs.particles().front().momentum();

      // Isolate photon by ensuring that a 0.4 cone around it contains less than 10% of the photon's energy
      double E_P   = photon.E();
      double eta_P = photon.pseudorapidity();
      double phi_P = photon.azimuthalAngle();
      double econe = 0.0;
      foreach (const Particle& p, applyProjection<FinalState>(event, "AllFS").particles()) {
        if (deltaR(eta_P, phi_P,
                   p.momentum().pseudorapidity(), p.momentum().azimuthalAngle()) < 0.4) {
          econe += p.momentum().E();
          if (econe/E_P > 1.1) {
            vetoEvent;
          }
        }
      }

      // Fill histo
      const double weight = event.weight();
      _h_pTgamma->fill(photon.pT(), weight);
    }



    // Finalize
    void finalize() {
      const double lumi_gen = sumOfWeights()/crossSection();
      // Divide by effective lumi, plus rapidity bin width of 1.8
      scale(_h_pTgamma, 1/lumi_gen * 1/1.8);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_pTgamma;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2006_S6438750);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief CDF <what is this analysis doing?>
  class CDF_1993_S2742446 : public Analysis {
  public:

    CDF_1993_S2742446()
      : Analysis("CDF_1993_S2742446")
    {    }


  public:

    void init() {

      // The photon selection has been corrected to pTmin=22 GeV (vs. 23 in the trigger)
      LeadingParticlesFinalState photonfs(FinalState(-0.9, 0.9, 22.0*GeV));
      photonfs.addParticleId(PID::PHOTON);
      addProjection(photonfs, "LeadingPhoton");

      // FS excluding the leading photon
      VetoedFinalState vfs(FinalState(-4.2, 4.2));
      vfs.addVetoOnThisFinalState(photonfs);
      addProjection(vfs, "VFS");

      // Jets
      addProjection(FastJets(vfs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_costheta = bookHisto1D(1, 1, 1);

    }


    void analyze(const Event& event) {

      const double weight = event.weight();

      Particles photons = applyProjection<LeadingParticlesFinalState>(event, "LeadingPhoton").particles();
      if (photons.size()!=1 || photons[0].pT()>45.0*GeV) {
        vetoEvent;
      }
      FourMomentum leadingPhoton = photons[0].momentum();
      double eta_P = leadingPhoton.eta();
      double phi_P = leadingPhoton.phi();

      // photon isolation: less than 2 GeV EM E_T
      double Etsum=0.0;
      foreach (const Particle& p, applyProjection<VetoedFinalState>(event, "VFS").particles()) {
        if (PID::threeCharge(p.pdgId())!=0 &&
            deltaR(eta_P, phi_P, p.eta(), p.momentum().phi()) < 0.7) {
          Etsum += p.momentum().Et();
        }
      }
      if (Etsum > 2.0*GeV) {
        vetoEvent;
      }

      // sum all jets in the opposite hemisphere in phi from the photon
      FourMomentum jetsum;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "Jets").jets(10.0*GeV)) {
        if (fabs(jet.momentum().phi()-phi_P) > M_PI) {
          jetsum+=jet.momentum();
        }
      }

      double costheta = fabs(tanh((eta_P-jetsum.eta())/2.0));

      _h_costheta->fill(costheta, weight);

    }


    void finalize() {

      normalize(_h_costheta, 1.4271); // fixed norm ok

    }


  private:

    Histo1DPtr _h_costheta;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1993_S2742446);

}

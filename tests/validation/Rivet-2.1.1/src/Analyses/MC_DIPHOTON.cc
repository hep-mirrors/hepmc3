// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"

namespace Rivet {


  /// @brief MC validation analysis for isolated di-photon events
  class MC_DIPHOTON : public Analysis {
  public:

    /// Constructor
    MC_DIPHOTON()
      : Analysis("MC_DIPHOTON")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      FinalState fs;
      addProjection(fs, "FS");

      IdentifiedFinalState ifs(-2.0, 2.0, 20.0*GeV);
      ifs.acceptId(PID::PHOTON);
      addProjection(ifs, "IFS");

      _h_m_PP = bookHisto1D("m_PP", logspace(50, 1.0, 0.25*sqrtS()));
      _h_pT_PP = bookHisto1D("pT_PP", logspace(50, 1.0, 0.25*sqrtS()));
      _h_pT_P1 = bookHisto1D("pT_P1", 50, 0.0, 70.0);
      _h_pT_P2 = bookHisto1D("pT_P2", 50, 0.0, 70.0);
      _h_dphi_PP = bookHisto1D("dphi_PP", 20, 0.0, M_PI);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Particles photons = applyProjection<IdentifiedFinalState>(event, "IFS").particles();

      if (photons.size() < 2) {
        vetoEvent;
      }

      // Isolate photons with ET_sum in cone
      Particles isolated_photons;
      Particles fs = applyProjection<FinalState>(event, "FS").particlesByPt();
      foreach (const Particle& photon, photons) {
        FourMomentum mom_in_cone;
        double eta_P = photon.eta();
        double phi_P = photon.momentum().phi();
        foreach (const Particle& p, fs) {
          if (deltaR(eta_P, phi_P, p.eta(), p.momentum().phi()) < 0.4) {
            mom_in_cone += p.momentum();
          }
        }
        if (mom_in_cone.Et()-photon.momentum().Et() < 4.0*GeV) {
          isolated_photons.push_back(photon);
        }
      }

      if (isolated_photons.size() != 2) {
        vetoEvent;
      }

      _h_pT_P1->fill(isolated_photons[0].pT(), weight);
      _h_pT_P2->fill(isolated_photons[1].pT(), weight);
      FourMomentum mom_PP = isolated_photons[0].momentum() + isolated_photons[1].momentum();
      _h_m_PP->fill(mom_PP.mass(), weight);
      _h_pT_PP->fill(mom_PP.pT(), weight);
      _h_dphi_PP->fill(deltaPhi(isolated_photons[0].momentum().phi(),
                                isolated_photons[1].momentum().phi()), weight);
    }


    void finalize() {
      scale(_h_m_PP, crossSection()/sumOfWeights());
      scale(_h_pT_PP, crossSection()/sumOfWeights());
      scale(_h_pT_P1, crossSection()/sumOfWeights());
      scale(_h_pT_P2, crossSection()/sumOfWeights());
      scale(_h_dphi_PP, crossSection()/sumOfWeights());
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_m_PP;
    Histo1DPtr _h_pT_PP;
    Histo1DPtr _h_pT_P1;
    Histo1DPtr _h_pT_P2;
    Histo1DPtr _h_dphi_PP;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_DIPHOTON);

}


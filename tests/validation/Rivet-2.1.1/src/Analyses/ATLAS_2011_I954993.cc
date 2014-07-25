// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief WZ fiducial cross-section measurement
  class ATLAS_2011_I954993 : public Analysis {
  public:

    /// Default constructor
    ATLAS_2011_I954993()
      : Analysis("ATLAS_2011_I954993")
    {
      setNeedsCrossSection(true);
    }


    /// @name Analysis methods
    //@{

    /// Projection and histogram setup
    void init() {

      ZFinder zfinder_e(-2.5, 2.5, 15*GeV, PID::ELECTRON, 81.1876*GeV, 101.1876*GeV, 0.1, ZFinder::CLUSTERNODECAY);
      addProjection(zfinder_e, "ZFinder_e");
      ZFinder zfinder_mu(-2.5, 2.5, 15*GeV, PID::MUON, 81.1876*GeV, 101.1876*GeV, 0.1, ZFinder::CLUSTERNODECAY);
      addProjection(zfinder_mu, "ZFinder_mu");

      VetoedFinalState weinput;
      weinput.addVetoOnThisFinalState(zfinder_e);
      WFinder wfinder_e(weinput, -2.5, 2.5, 15*GeV, PID::ELECTRON, 0*GeV, 1000*GeV, 25*GeV, 0.1, WFinder::CLUSTERNODECAY);
      addProjection(wfinder_e, "WFinder_e");

      VetoedFinalState wminput;
      wminput.addVetoOnThisFinalState(zfinder_mu);
      WFinder wfinder_mu(wminput,-2.5, 2.5, 15*GeV, PID::MUON, 0*GeV, 1000*GeV, 25*GeV, 0.1, WFinder::CLUSTERNODECAY);
      addProjection(wfinder_mu, "WFinder_mu");

      // Histograms
      _h_fiducial = bookHisto1D(1,1,1);
    }


    /// Do the analysis
    void analyze(const Event& e) {
      const ZFinder& zfinder_e = applyProjection<ZFinder>(e, "ZFinder_e");
      const ZFinder& zfinder_mu = applyProjection<ZFinder>(e, "ZFinder_mu");
      const WFinder& wfinder_e = applyProjection<WFinder>(e, "WFinder_e");
      const WFinder& wfinder_mu = applyProjection<WFinder>(e, "WFinder_mu");

      // Looking for a Z, exit if not found
      if (zfinder_e.bosons().size() != 1 && zfinder_mu.bosons().size() != 1) {
        MSG_DEBUG("No Z boson found, vetoing event");
        vetoEvent;
      }

      // Looking for a W, exit if not found
      if (wfinder_e.bosons().size()!= 1 && wfinder_mu.bosons().size() != 1) {
        MSG_DEBUG("No W boson found, vetoing event");
        vetoEvent;
      }

      // If we find a W, make fiducial acceptance cuts and exit if not found
      if (wfinder_e.bosons().size() == 1) {
        const FourMomentum& We = wfinder_e.constituentLeptons()[0].momentum();
        const FourMomentum& Wenu = wfinder_e.constituentNeutrinos()[0].momentum();
        const double mT = wfinder_e.mT();
        if (Wenu.pT() < 25*GeV || We.pT() < 20*GeV || mT < 20*GeV) {
          MSG_DEBUG("Wnu pT = " << Wenu.pT()/GeV << " GeV, Wl pT = " << We.pT()/GeV << " GeV, mT = " << mT/GeV << " GeV");
          vetoEvent;
        }
      } else if (wfinder_mu.bosons().size() == 1) {
        const FourMomentum& Wmu = wfinder_mu.constituentLeptons()[0].momentum();
        const FourMomentum& Wmunu = wfinder_mu.constituentNeutrinos()[0].momentum();
        const double mT = wfinder_mu.mT();
        if (Wmunu.pT() < 25*GeV || Wmu.pT() < 20*GeV || mT < 20*GeV) {
          MSG_DEBUG("Wnu pT = " << Wmunu.pT()/GeV << ", Wl pT = " << Wmu.pT()/GeV << " GeV, mT = " << mT/GeV << " GeV");
          vetoEvent;
        }
      } else {
        MSG_DEBUG("No W boson found: vetoing event");
        vetoEvent;
      }

      // Update the fiducial cross-section histogram
      _h_fiducial->fill(7000, e.weight());
    }


    /// Finalize
    void finalize() {
      normalize(_h_fiducial, crossSection()/femtobarn);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_fiducial;
    //@}

  };


  //// The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_I954993);

}

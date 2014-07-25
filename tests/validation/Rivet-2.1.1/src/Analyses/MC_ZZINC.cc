// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {

  /// @brief MC validation analysis for Z[ee]Z[mumu] events
  class MC_ZZINC : public Analysis {
  public:

    /// Default constructor
    MC_ZZINC()
      : Analysis("MC_ZZINC")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      ZFinder zeefinder(FinalState(), -3.5, 3.5, 25*GeV, PID::ELECTRON, 65*GeV, 115*GeV,
                        0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zeefinder, "ZeeFinder");

      VetoedFinalState zmminput;
      zmminput.addVetoOnThisFinalState(zeefinder);
      ZFinder zmmfinder(zmminput, -3.5, 3.5, 25*GeV, PID::MUON, 65*GeV, 115*GeV,
                        0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zmmfinder, "ZmmFinder");

      // Properties of the pair momentum
      _h_ZZ_pT = bookHisto1D("ZZ_pT", logspace(100, 1.0, 0.5*sqrtS()/GeV));
      _h_ZZ_pT_peak = bookHisto1D("ZZ_pT_peak", 25, 0.0, 25.0);
      _h_ZZ_eta = bookHisto1D("ZZ_eta", 40, -7.0, 7.0);
      _h_ZZ_phi = bookHisto1D("ZZ_phi", 25, 0.0, TWOPI);
      _h_ZZ_m = bookHisto1D("ZZ_m", logspace(100, 150.0, 180.0 + 0.25*sqrtS()/GeV));

      // Correlations between the ZZ
      _h_ZZ_dphi = bookHisto1D("ZZ_dphi", 25, 0.0, PI);  /// @todo non-linear?
      _h_ZZ_deta = bookHisto1D("ZZ_deta", 25, -7.0, 7.0);
      _h_ZZ_dR = bookHisto1D("ZZ_dR", 25, 0.5, 7.0);
      _h_ZZ_dpT = bookHisto1D("ZZ_dpT", logspace(100, 1.0, 0.5*sqrtS()/GeV));
      _h_ZZ_costheta_planes = bookHisto1D("ZZ_costheta_planes", 25, -1.0, 1.0);

      // Properties of the Z bosons
      _h_Z_pT = bookHisto1D("Z_pT", logspace(100, 10.0, 0.25*sqrtS()/GeV));
      _h_Z_eta = bookHisto1D("Z_eta", 70, -7.0, 7.0);

      // Properties of the leptons
      _h_Zl_pT = bookHisto1D("Zl_pT", logspace(100, 30.0, 0.1*sqrtS()/GeV));
      _h_Zl_eta = bookHisto1D("Zl_eta", 40, -3.5, 3.5);

      // Correlations between the opposite charge leptons
      _h_ZeZm_dphi = bookHisto1D("ZeZm_dphi", 25, 0.0, PI);
      _h_ZeZm_deta = bookHisto1D("ZeZm_deta", 25, -5.0, 5.0);
      _h_ZeZm_dR = bookHisto1D("ZeZm_dR", 25, 0.5, 5.0);
      _h_ZeZm_m = bookHisto1D("ZeZm_m", 100, 0.0, 300.0);

    }



    /// Do the analysis
    void analyze(const Event& e) {
      const ZFinder& zeefinder = applyProjection<ZFinder>(e, "ZeeFinder");
      if (zeefinder.bosons().size() != 1) vetoEvent;
      const ZFinder& zmmfinder = applyProjection<ZFinder>(e, "ZmmFinder");
      if (zmmfinder.bosons().size() != 1) vetoEvent;

      // Z momenta
      const FourMomentum& zee = zeefinder.bosons()[0].momentum();
      const FourMomentum& zmm = zmmfinder.bosons()[0].momentum();
      const FourMomentum zz = zee + zmm;
      // Lepton momenta
      const FourMomentum& ep = zeefinder.constituents()[0].momentum();
      const FourMomentum& em = zeefinder.constituents()[1].momentum();
      const FourMomentum& mp = zmmfinder.constituents()[0].momentum();
      const FourMomentum& mm = zmmfinder.constituents()[1].momentum();

      const double weight = e.weight();
      _h_ZZ_pT->fill(zz.pT()/GeV, weight);
      _h_ZZ_pT_peak->fill(zz.pT()/GeV, weight);
      _h_ZZ_eta->fill(zz.eta(), weight);
      _h_ZZ_phi->fill(zz.azimuthalAngle(), weight);
      if (zz.mass2() > 0.0) //< @todo Protection still needed?
        _h_ZZ_m->fill(zz.mass()/GeV, weight);

      _h_ZZ_dphi->fill(deltaPhi(zee, zmm), weight);
      _h_ZZ_deta->fill(zee.eta()-zmm.eta(), weight);
      _h_ZZ_dR->fill(deltaR(zee,zmm), weight);
      _h_ZZ_dpT->fill(fabs(zee.pT()-zmm.pT()), weight);

      const Vector3 crossZee = ep.vector3().cross(em.vector3());
      const Vector3 crossZmm = mp.vector3().cross(mm.vector3());
      const double costheta = crossZee.dot(crossZmm)/crossZee.mod()/crossZmm.mod();
      _h_ZZ_costheta_planes->fill(costheta, weight);

      _h_Z_pT->fill(zee.pT()/GeV, weight);
      _h_Z_pT->fill(zmm.pT()/GeV, weight);
      _h_Z_eta->fill(zee.eta(), weight);
      _h_Z_eta->fill(zmm.eta(), weight);

      _h_Zl_pT->fill(ep.pT()/GeV, weight);
      _h_Zl_pT->fill(em.pT()/GeV, weight);
      _h_Zl_pT->fill(mp.pT()/GeV, weight);
      _h_Zl_pT->fill(mm.pT()/GeV, weight);
      _h_Zl_eta->fill(ep.eta(), weight);
      _h_Zl_eta->fill(em.eta(), weight);
      _h_Zl_eta->fill(mp.eta(), weight);
      _h_Zl_eta->fill(mm.eta(), weight);

      _h_ZeZm_dphi->fill(deltaPhi(ep, mm), weight);
      _h_ZeZm_deta->fill(ep.eta()-mm.eta(), weight);
      _h_ZeZm_dR->fill(deltaR(ep, mm), weight);
      const FourMomentum epmm = ep + mm;
      const double m_epmm = (epmm.mass2() > 0) ? epmm.mass() : 0; //< @todo Protection still needed?
      _h_ZeZm_m->fill(m_epmm/GeV, weight);
    }


    /// Finalize
    void finalize() {
      const double norm = crossSection()/picobarn;
      normalize(_h_ZZ_pT, norm);
      normalize(_h_ZZ_pT_peak, norm);
      normalize(_h_ZZ_eta, norm);
      normalize(_h_ZZ_phi, norm);
      normalize(_h_ZZ_m, norm);
      normalize(_h_ZZ_dphi, norm);
      normalize(_h_ZZ_deta, norm);
      normalize(_h_ZZ_dR, norm);
      normalize(_h_ZZ_dpT, norm);
      normalize(_h_ZZ_costheta_planes, norm);
      normalize(_h_Z_pT, norm);
      normalize(_h_Z_eta, norm);
      normalize(_h_Zl_pT, norm);
      normalize(_h_Zl_eta, norm);
      normalize(_h_ZeZm_dphi, norm);
      normalize(_h_ZeZm_deta, norm);
      normalize(_h_ZeZm_dR, norm);
      normalize(_h_ZeZm_m, norm);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_ZZ_pT;
    Histo1DPtr _h_ZZ_pT_peak;
    Histo1DPtr _h_ZZ_eta;
    Histo1DPtr _h_ZZ_phi;
    Histo1DPtr _h_ZZ_m;
    Histo1DPtr _h_ZZ_dphi;
    Histo1DPtr _h_ZZ_deta;
    Histo1DPtr _h_ZZ_dR;
    Histo1DPtr _h_ZZ_dpT;
    Histo1DPtr _h_ZZ_costheta_planes;
    Histo1DPtr _h_Z_pT;
    Histo1DPtr _h_Z_eta;
    Histo1DPtr _h_Zl_pT;
    Histo1DPtr _h_Zl_eta;
    Histo1DPtr _h_ZeZm_dphi;
    Histo1DPtr _h_ZeZm_deta;
    Histo1DPtr _h_ZeZm_dR;
    Histo1DPtr _h_ZeZm_m;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_ZZINC);

}

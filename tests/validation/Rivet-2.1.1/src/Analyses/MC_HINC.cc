// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"

namespace Rivet {


  /// @brief MC validation analysis for higgs [-> tau tau] events
  class MC_HINC : public Analysis {
  public:

    /// Default constructor
    MC_HINC()
      : Analysis("MC_HINC")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      /// @todo Urk, abuse! Need explicit HiggsFinder (and TauFinder?)
      ZFinder hfinder(FinalState(), -3.5, 3.5, 25*GeV, PID::TAU, 115*GeV, 125*GeV, 0.0, ZFinder::NOCLUSTER);
      addProjection(hfinder, "Hfinder");
      _h_H_mass = bookHisto1D("H_mass", 50, 119.7, 120.3);
      _h_H_pT = bookHisto1D("H_pT", logspace(100, 1.0, 0.5*sqrtS()/GeV));
      _h_H_pT_peak = bookHisto1D("H_pT_peak", 25, 0.0, 25.0);
      _h_H_y = bookHisto1D("H_y", 40, -4, 4);
      _h_H_phi = bookHisto1D("H_phi", 25, 0.0, TWOPI);
      _h_lepton_pT = bookHisto1D("lepton_pT", logspace(100, 10.0, 0.25*sqrtS()/GeV));
      _h_lepton_eta = bookHisto1D("lepton_eta", 40, -4, 4);
    }


    /// Do the analysis
    void analyze(const Event & e) {
      const ZFinder& hfinder = applyProjection<ZFinder>(e, "Hfinder");
      if (hfinder.bosons().size() != 1) vetoEvent;
      const double weight = e.weight();

      FourMomentum hmom(hfinder.bosons()[0].momentum());
      _h_H_mass->fill(hmom.mass()/GeV, weight);
      _h_H_pT->fill(hmom.pT()/GeV, weight);
      _h_H_pT_peak->fill(hmom.pT()/GeV, weight);
      _h_H_y->fill(hmom.rapidity(), weight);
      _h_H_phi->fill(hmom.azimuthalAngle(), weight);
      foreach (const Particle& l, hfinder.constituents()) {
        _h_lepton_pT->fill(l.pT()/GeV, weight);
        _h_lepton_eta->fill(l.eta(), weight);
      }
    }


    /// Finalize
    void finalize() {
      const double xsec = crossSection()/picobarn;
      normalize(_h_H_mass, xsec);
      normalize(_h_H_pT, xsec);
      normalize(_h_H_pT_peak, xsec);
      normalize(_h_H_y, xsec);
      normalize(_h_H_phi, xsec);
      normalize(_h_lepton_pT, xsec);
      normalize(_h_lepton_eta, xsec);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_H_mass;
    Histo1DPtr _h_H_pT;
    Histo1DPtr _h_H_pT_peak;
    Histo1DPtr _h_H_y;
    Histo1DPtr _h_H_phi;
    Histo1DPtr _h_lepton_pT;
    Histo1DPtr _h_lepton_eta;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_HINC);

}

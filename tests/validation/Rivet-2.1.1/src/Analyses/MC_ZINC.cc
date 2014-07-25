// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"

namespace Rivet {


  /// @brief MC validation analysis for Z events
  class MC_ZINC : public Analysis {
  public:

    /// Default constructor
    MC_ZINC()
      : Analysis("MC_ZINC")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      ZFinder zfinder(fs, -3.5, 3.5, 25.0*GeV, PID::ELECTRON, 65.0*GeV, 115.0*GeV, 0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");

      _h_Z_mass = bookHisto1D("Z_mass", 50, 66.0, 116.0);
      _h_Z_pT = bookHisto1D("Z_pT", logspace(100, 1.0, 0.5*sqrtS()/GeV));
      _h_Z_pT_peak = bookHisto1D("Z_pT_peak", 25, 0.0, 25.0);
      _h_Z_y = bookHisto1D("Z_y", 40, -4.0, 4.0);
      _h_Z_phi = bookHisto1D("Z_phi", 25, 0.0, TWOPI);
      _h_lepton_pT = bookHisto1D("lepton_pT", logspace(100, 10.0, 0.25*sqrtS()/GeV));
      _h_lepton_eta = bookHisto1D("lepton_eta", 40, -4.0, 4.0);

    }



    /// Do the analysis
    void analyze(const Event & e) {
      const ZFinder& zfinder = applyProjection<ZFinder>(e, "ZFinder");
      if (zfinder.bosons().size()!=1) {
        vetoEvent;
      }
      const double weight = e.weight();

      FourMomentum zmom(zfinder.bosons()[0].momentum());
      _h_Z_mass->fill(zmom.mass()/GeV, weight);
      _h_Z_pT->fill(zmom.pT()/GeV, weight);
      _h_Z_pT_peak->fill(zmom.pT()/GeV, weight);
      _h_Z_y->fill(zmom.rapidity(), weight);
      _h_Z_phi->fill(zmom.azimuthalAngle(), weight);
      foreach (const Particle& l, zfinder.constituents()) {
        _h_lepton_pT->fill(l.pT()/GeV, weight);
        _h_lepton_eta->fill(l.eta(), weight);
      }
    }


    /// Finalize
    void finalize() {
      const double xsec = crossSection()/picobarn;
      normalize(_h_Z_mass, xsec);
      normalize(_h_Z_pT, xsec);
      normalize(_h_Z_pT_peak, xsec);
      normalize(_h_Z_y, xsec);
      normalize(_h_Z_phi, xsec);
      normalize(_h_lepton_pT, xsec);
      normalize(_h_lepton_eta, xsec);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_Z_mass;
    Histo1DPtr _h_Z_pT;
    Histo1DPtr _h_Z_pT_peak;
    Histo1DPtr _h_Z_y;
    Histo1DPtr _h_Z_phi;
    Histo1DPtr _h_lepton_pT;
    Histo1DPtr _h_lepton_eta;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_ZINC);

}

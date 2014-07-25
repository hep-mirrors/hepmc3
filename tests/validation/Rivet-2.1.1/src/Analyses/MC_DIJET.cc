// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for di-jet events
  class MC_DIJET : public Analysis {
  public:

    /// Default constructor
    MC_DIJET() : Analysis("MC_DIJET")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      FinalState fs(-4, 4, 0.5*GeV);
      ChargedFinalState cfs(fs);
      addProjection(fs, "FS");
      addProjection(cfs, "CFS");
      addProjection(FastJets(fs, FastJets::ANTIKT, 0.7), "Jets");
      addProjection(FastJets(cfs, FastJets::ANTIKT, 0.7), "ChargedJets");

      _hist_jetcount = bookHisto1D("d01-x01-y01", 5, 0., 10.);
      _hist_jetpt = bookHisto1D("d02-x01-y01", 30, 30.,100.);
      _hist_jetptlog = bookHisto1D("d03-x01-y01", 20, 0.,8.);
      _hist_leadingjetpt = bookHisto1D("d04-x01-y01", 25, 30.,100.);
      _hist_secondleadingjetpt = bookHisto1D("d05-x01-y01", 25, 30.,100.);
      _hist_jetphi = bookHisto1D("d06-x01-y01",24, 0., 6.4);
      _hist_jeteta = bookHisto1D("d07-x01-y01", 30, -6., 6.);
      _hist_jetdphi = bookHisto1D("d08-x01-y01", 24, 0., 6.4);
      _hist_jetdeta = bookHisto1D("d09-x01-y01", 24, 0., 6.);
      _hist_chargemultiplicity = bookHisto1D("d10-x01-y01",30, 0.5, 250.5);
      _hist_chargemeanpt = bookHisto1D("d11-x01-y01", 25, 0., 10.);
      _hist_chargept = bookHisto1D("d12-x01-y01", 32, 0., 25.);
      _hist_chargelogpt = bookHisto1D("d13-x01-y01", 32, 0., 6.);
      _hist_chargermspt = bookHisto1D("d14-x01-y01", 32, 0., 10.);
    }


    void analyze(const Event& event) {
      const FastJets& fastjets = applyProjection<FastJets>(event, "Jets");
      const Jets jets = fastjets.jetsByPt(20.*GeV);
      const double weight = event.weight();

      if (jets.size() < 2 || jets.size() >= 3) vetoEvent;
      const double angle = fabs(jets[1].momentum().azimuthalAngle() - jets[0].momentum().azimuthalAngle());
      const double prapidity = fabs(jets[1].momentum().pseudorapidity() - jets[0].momentum().pseudorapidity());
      _hist_jetcount->fill(jets.size(), weight);
      _hist_leadingjetpt->fill(jets[0].pT(), weight);
      _hist_secondleadingjetpt->fill(jets[1].pT(), weight);
      _hist_jetdphi->fill(angle , weight);
      _hist_jetdeta->fill(prapidity, weight);

      foreach(Jet j, fastjets.jetsByPt(20.*GeV)) {
        _hist_jetpt->fill(j.pT(), weight);
        _hist_jetptlog->fill(log(j.pT()), weight);
        _hist_jetphi->fill(j.momentum().azimuthalAngle(), weight);
        _hist_jeteta->fill(j.momentum().pseudorapidity(), weight);
      }

      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");
      // const FastJets& cfastjets = applyProjection<FastJets>(event, "ChargedJets");
      double meanpt(0), rmspt(0);
      /// @todo Add jets
      // foreach(Jet cj, cfastjets.jetsByPt(20.*GeV)){
      _hist_chargemultiplicity->fill(cfs.particles().size(), weight);
      foreach(Particle cp, cfs.particles()) {
        meanpt= meanpt + cp.pT();
        rmspt = rmspt + (cp.pT()*cp.pT());
        _hist_chargept->fill(cp.pT(), weight);
        _hist_chargelogpt->fill(log(cp.pT()), weight);
      }
      meanpt = meanpt / cfs.particles().size();
      _hist_chargemeanpt->fill(meanpt, weight);
      rmspt = sqrt(rmspt / cfs.particles().size());
      _hist_chargermspt->fill(rmspt, weight);
      // }
    }


    void finalize() {
      /// @todo Normalise!
    }

    //@}


  private:

    Histo1DPtr _hist_jetcount;
    Histo1DPtr _hist_jetpt;
    Histo1DPtr _hist_jetptlog;
    Histo1DPtr _hist_leadingjetpt;
    Histo1DPtr _hist_secondleadingjetpt;
    Histo1DPtr _hist_jetphi;
    Histo1DPtr _hist_jetdphi;
    Histo1DPtr _hist_jeteta;
    Histo1DPtr _hist_jetdeta;
    Histo1DPtr _hist_chargemultiplicity;
    Histo1DPtr _hist_chargemeanpt;
    Histo1DPtr _hist_chargept;
    Histo1DPtr _hist_chargelogpt;
    Histo1DPtr _hist_chargermspt;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_DIJET);

}

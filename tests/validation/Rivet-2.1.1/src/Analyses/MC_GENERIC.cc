// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// Generic analysis looking at various distributions of final state particles
  class MC_GENERIC : public Analysis {
  public:

    /// Constructor
    MC_GENERIC()
      : Analysis("MC_GENERIC")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Projections
      const FinalState cnfs(-5.0, 5.0, 500*MeV);
      addProjection(cnfs, "FS");
      addProjection(ChargedFinalState(-5.0, 5.0, 500*MeV), "CFS");

      // Histograms
      // @todo Choose E/pT ranged based on input energies... can't do anything about kin. cuts, though
      _histMult   = bookHisto1D("Mult", 100, -0.5, 199.5);
      _histMultCh = bookHisto1D("MultCh", 100, -0.5, 199.5);

      _histPt   = bookHisto1D("Pt", 300, 0, 30);
      _histPtCh = bookHisto1D("PtCh", 300, 0, 30);

      _histE   = bookHisto1D("E", 100, 0, 200);
      _histECh = bookHisto1D("ECh", 100, 0, 200);

      _histEtaSumEt = bookProfile1D("EtaSumEt", 25, 0, 5);

      _histEta    = bookHisto1D("Eta", 50, -5, 5);
      _histEtaCh  = bookHisto1D("EtaCh", 50, -5, 5);
      _tmphistEtaPlus = Histo1D(25, 0, 5);
      _tmphistEtaMinus = Histo1D(25, 0, 5);
      _tmphistEtaChPlus = Histo1D(25, 0, 5);
      _tmphistEtaChMinus = Histo1D(25, 0, 5);

      _histRapidity    = bookHisto1D("Rapidity", 50, -5, 5);
      _histRapidityCh  = bookHisto1D("RapidityCh", 50, -5, 5);
      _tmphistRapPlus = Histo1D(25, 0, 5);
      _tmphistRapMinus = Histo1D(25, 0, 5);
      _tmphistRapChPlus = Histo1D(25, 0, 5);
      _tmphistRapChMinus = Histo1D(25, 0, 5);

      _histPhi    = bookHisto1D("Phi", 50, 0, TWOPI);
      _histPhiCh  = bookHisto1D("PhiCh", 50, 0, TWOPI);

      _histEtaPMRatio = bookScatter2D("EtaPMRatio");
      _histEtaChPMRatio = bookScatter2D("EtaChPMRatio");
      _histRapidityPMRatio = bookScatter2D("RapidityPMRatio");
      _histRapidityChPMRatio = bookScatter2D("RapidityChPMRatio");
    }



    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Charged + neutral final state
      const FinalState& cnfs = applyProjection<FinalState>(event, "FS");
      MSG_DEBUG("Total multiplicity = " << cnfs.size());
      _histMult->fill(cnfs.size(), weight);
      foreach (const Particle& p, cnfs.particles()) {
        const double eta = p.eta();
        _histEta->fill(eta, weight);
        _histEtaSumEt->fill(fabs(eta), p.momentum().Et(), weight);
        if (eta > 0) _tmphistEtaPlus.fill(fabs(eta), weight);
        else _tmphistEtaMinus.fill(fabs(eta), weight);
        //
        const double rapidity = p.rapidity();
        _histRapidity->fill(rapidity, weight);
        if (rapidity > 0) _tmphistRapPlus.fill(fabs(rapidity), weight);
        else _tmphistRapMinus.fill(fabs(rapidity), weight);
        //
        _histPt->fill(p.pT()/GeV, weight);
        _histE->fill(p.momentum().E()/GeV, weight);
        _histPhi->fill(p.momentum().phi(), weight);
      }

      const FinalState& cfs = applyProjection<FinalState>(event, "CFS");
      MSG_DEBUG("Total charged multiplicity = " << cfs.size());
      _histMultCh->fill(cfs.size(), weight);
      foreach (const Particle& p, cfs.particles()) {
        const double eta = p.eta();
        _histEtaCh->fill(eta, weight);
        if (eta > 0) {
          _tmphistEtaChPlus.fill(fabs(eta), weight);
        } else {
          _tmphistEtaChMinus.fill(fabs(eta), weight);
        }
        const double rapidity = p.rapidity();
        _histRapidityCh->fill(rapidity, weight);
        if (rapidity > 0) {
          _tmphistRapChPlus.fill(fabs(rapidity), weight);
        } else {
          _tmphistRapChMinus.fill(fabs(rapidity), weight);
        }
        _histPtCh->fill(p.pT()/GeV, weight);
        _histECh->fill(p.momentum().E()/GeV, weight);
        _histPhiCh->fill(p.momentum().phi(), weight);
      }

    }


    /// Finalize
    void finalize() {
      normalize(_histMult);
      normalize(_histMultCh);
      normalize(_histEta);
      normalize(_histEtaCh);
      normalize(_histRapidity);
      normalize(_histRapidityCh);
      normalize(_histPt);
      normalize(_histPtCh);
      normalize(_histE);
      normalize(_histECh);
      normalize(_histPhi);
      normalize(_histPhiCh);
      divide(_tmphistEtaPlus, _tmphistEtaMinus, _histEtaPMRatio);
      divide(_tmphistEtaChPlus, _tmphistEtaChMinus, _histEtaChPMRatio);
      divide(_tmphistRapPlus, _tmphistRapMinus, _histRapidityPMRatio);
      divide(_tmphistRapChPlus, _tmphistRapChMinus, _histRapidityChPMRatio);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _histMult, _histMultCh;
    Profile1DPtr _histEtaSumEt;
    Histo1DPtr _histEta, _histEtaCh;
    Histo1DPtr _histRapidity, _histRapidityCh;
    Histo1DPtr _histPt, _histPtCh;
    Histo1DPtr _histE, _histECh;
    Histo1DPtr _histPhi, _histPhiCh;
    Scatter2DPtr _histEtaPMRatio;
    Scatter2DPtr _histEtaChPMRatio;
    Scatter2DPtr _histRapidityPMRatio;
    Scatter2DPtr _histRapidityChPMRatio;
    //@}

    /// @name Temporary histos used to calculate eta+/eta- ratio plots
    //@{
    Histo1D _tmphistEtaPlus, _tmphistEtaMinus;
    Histo1D _tmphistEtaChPlus, _tmphistEtaChMinus;
    Histo1D _tmphistRapPlus, _tmphistRapMinus;
    Histo1D _tmphistRapChPlus, _tmphistRapChMinus;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_GENERIC);

}

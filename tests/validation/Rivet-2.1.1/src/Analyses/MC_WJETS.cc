// -*- C++ -*-
#include "Rivet/Analyses/MC_JetAnalysis.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Analysis.hh"

namespace Rivet {


  /// @brief MC validation analysis for W + jets events
  class MC_WJETS : public MC_JetAnalysis {
  public:

    /// Default constructor
    MC_WJETS()
      : MC_JetAnalysis("MC_WJETS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      WFinder wfinder(fs, -3.5, 3.5, 25.0*GeV, PID::ELECTRON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wfinder, "WFinder");
      FastJets jetpro(wfinder.remainingFinalState(), FastJets::ANTIKT, 0.4);
      addProjection(jetpro, "Jets");

      _h_W_jet1_deta = bookHisto1D("W_jet1_deta", 50, -5.0, 5.0);
      _h_W_jet1_dR = bookHisto1D("W_jet1_dR", 25, 0.5, 7.0);

      MC_JetAnalysis::init();
    }



    /// Do the analysis
    void analyze(const Event & e) {
      const double weight = e.weight();

      const WFinder& wfinder = applyProjection<WFinder>(e, "WFinder");
      if (wfinder.bosons().size() != 1) {
        vetoEvent;
      }
      FourMomentum wmom(wfinder.bosons().front().momentum());

      const Jets& jets = applyProjection<FastJets>(e, "Jets").jetsByPt(m_jetptcut);
      if (jets.size() > 0) {
        _h_W_jet1_deta->fill(wmom.eta()-jets[0].eta(), weight);
        _h_W_jet1_dR->fill(deltaR(wmom, jets[0].momentum()), weight);
      }

      MC_JetAnalysis::analyze(e);
    }


    /// Finalize
    void finalize() {
      scale(_h_W_jet1_deta, crossSection()/sumOfWeights());
      scale(_h_W_jet1_dR, crossSection()/sumOfWeights());

      MC_JetAnalysis::finalize();
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_W_jet1_deta;
    Histo1DPtr _h_W_jet1_dR;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_WJETS);

}

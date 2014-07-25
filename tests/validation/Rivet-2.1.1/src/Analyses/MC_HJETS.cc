// -*- C++ -*-
#include "Rivet/Analyses/MC_JetAnalysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for higgs [-> tau tau] + jets events
  class MC_HJETS : public MC_JetAnalysis {
  public:

    /// Default constructor
    MC_HJETS()
      : MC_JetAnalysis("MC_HJETS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      /// @todo Urk, abuse! Need explicit HiggsFinder (and TauFinder?)
      ZFinder hfinder(FinalState(), -3.5, 3.5, 25*GeV, PID::TAU, 115*GeV, 125*GeV, 0.0, ZFinder::NOCLUSTER);
      addProjection(hfinder, "Hfinder");
      FastJets jetpro(hfinder.remainingFinalState(), FastJets::ANTIKT, 0.4);
      addProjection(jetpro, "Jets");

      _h_H_jet1_deta = bookHisto1D("H_jet1_deta", 50, -5.0, 5.0);
      _h_H_jet1_dR = bookHisto1D("H_jet1_dR", 25, 0.5, 7.0);

      MC_JetAnalysis::init();
    }



    /// Do the analysis
    void analyze(const Event & e) {
      const ZFinder& hfinder = applyProjection<ZFinder>(e, "Hfinder");
      if (hfinder.bosons().size() != 1) vetoEvent;
      const double weight = e.weight();

      FourMomentum hmom(hfinder.bosons()[0].momentum());
      const Jets& jets = applyProjection<FastJets>(e, "Jets").jetsByPt(m_jetptcut);
      if (jets.size() > 0) {
        _h_H_jet1_deta->fill(hmom.eta()-jets[0].eta(), weight);
        _h_H_jet1_dR->fill(deltaR(hmom, jets[0].momentum()), weight);
      }

      MC_JetAnalysis::analyze(e);
    }


    /// Finalize
    void finalize() {
      normalize(_h_H_jet1_deta, crossSection()/picobarn);
      normalize(_h_H_jet1_dR, crossSection()/picobarn);
      MC_JetAnalysis::finalize();
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_H_jet1_deta;
    Histo1DPtr _h_H_jet1_dR;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_HJETS);

}

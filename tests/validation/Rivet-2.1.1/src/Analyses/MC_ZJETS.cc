// -*- C++ -*-
#include "Rivet/Analyses/MC_JetAnalysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief MC validation analysis for Z + jets events
  class MC_ZJETS : public MC_JetAnalysis {
  public:

    /// Default constructor
    MC_ZJETS()
      : MC_JetAnalysis("MC_ZJETS", 4, "Jets")
    {    }


    /// @name Analysis methods
    //@{

    /// Initialize
    void init() {
      FinalState fs;
      ZFinder zfinder(fs, -3.5, 3.5, 25*GeV, PID::ELECTRON, 65*GeV, 115*GeV, 0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");
      FastJets jetpro(zfinder.remainingFinalState(), FastJets::ANTIKT, 0.4);
      addProjection(jetpro, "Jets");

      _h_Z_jet1_deta = bookHisto1D("Z_jet1_deta", 50, -5, 5);
      _h_Z_jet1_dR = bookHisto1D("Z_jet1_dR", 25, 0.5, 7.0);

      MC_JetAnalysis::init();
    }



    /// Do the analysis
    void analyze(const Event & e) {
      const ZFinder& zfinder = applyProjection<ZFinder>(e, "ZFinder");
      if (zfinder.bosons().size() != 1) vetoEvent;
      const FourMomentum& zmom = zfinder.bosons()[0].momentum();

      const Jets& jets = applyProjection<FastJets>(e, "Jets").jetsByPt(m_jetptcut);
      if (jets.size() > 0) {
        const double weight = e.weight();
        _h_Z_jet1_deta->fill(zmom.eta()-jets[0].eta(), weight);
        _h_Z_jet1_dR->fill(deltaR(zmom, jets[0].momentum()), weight);
      }

      MC_JetAnalysis::analyze(e);
    }


    /// Finalize
    void finalize() {
      normalize(_h_Z_jet1_deta, crossSection()/picobarn);
      normalize(_h_Z_jet1_dR, crossSection()/picobarn);
      MC_JetAnalysis::finalize();
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_Z_jet1_deta;
    Histo1DPtr _h_Z_jet1_dR;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_ZJETS);

}

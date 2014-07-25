// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"

namespace Rivet {


  /* @brief D0 Run II angular correlations in di-jet events
   * @author Lars Sonnenschein
   *
   * Measurement of angular correlations in di-jet events.
   *
   * @par Run conditions
   *
   * @arg \f$ \sqrt{s} = \f$ 1960 GeV
   * @arg Run with generic QCD events.
   * @arg Several \f$ p_\perp^\text{min} \f$ cutoffs are probably required to fill the histograms:
   *   @arg \f$ p_\perp^\text{min} = \f$ 50, 75, 100, 150 GeV for the four pT ranges respecively
   *
   */
  class D0_2004_S5992206 : public Analysis {

  public:

    /// @name Constructors etc.
    //@{

    /// Constructor.
    D0_2004_S5992206()
      : Analysis("D0_2004_S5992206")
    {  }

    //@}


    /// @name Analysis methods
    //@{

    void init() {
      // Final state for jets, mET etc.
      const FinalState fs(-3.0, 3.0);
      addProjection(fs, "FS");
      // Veto neutrinos, and muons with pT above 1.0 GeV
      VetoedFinalState vfs(fs);
      vfs.vetoNeutrinos();
      vfs.addVetoPairDetail(PID::MUON, 1.0*GeV, MAXDOUBLE);
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::D0ILCONE, 0.7), "Jets");
      addProjection(MissingMomentum(vfs), "CalMET");

      // Book histograms
      _histJetAzimuth_pTmax75_100  = bookHisto1D(1, 2, 1);
      _histJetAzimuth_pTmax100_130 = bookHisto1D(2, 2, 1);
      _histJetAzimuth_pTmax130_180 = bookHisto1D(3, 2, 1);
      _histJetAzimuth_pTmax180_    = bookHisto1D(4, 2, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {

      // Analyse and print some info
      const JetAlg& jetpro = applyProjection<JetAlg>(event, "Jets");
      MSG_DEBUG("Jet multiplicity before any pT cut = " << jetpro.size());

      const Jets jets  = jetpro.jetsByPt(40.0*GeV);
      if (jets.size() >= 2) {
        MSG_DEBUG("Jet multiplicity after pT > 40 GeV cut = " << jets.size());
      } else {
        vetoEvent;
      }
      const double rap1 = jets[0].rapidity();
      const double rap2 = jets[1].rapidity();
      if (fabs(rap1) > 0.5 || fabs(rap2) > 0.5) {
        vetoEvent;
      }
      MSG_DEBUG("Jet eta and pT requirements fulfilled");
      const double pT1 = jets[0].pT();

      const MissingMomentum& caloMissEt = applyProjection<MissingMomentum>(event, "CalMET");
      MSG_DEBUG("Missing vector Et = " << caloMissEt.vectorEt()/GeV << " GeV");
      if (caloMissEt.vectorEt().mod() > 0.7*pT1) {
        MSG_DEBUG("Vetoing event with too much missing ET: "
                  << caloMissEt.vectorEt()/GeV << " GeV > "
                  << 0.7*pT1/GeV << " GeV");
        vetoEvent;
      }

      if (pT1/GeV >= 75.0) {
        const double weight = event.weight();
        const double dphi = deltaPhi(jets[0].momentum().phi(), jets[1].momentum().phi());
        if (inRange(pT1/GeV, 75.0, 100.0)) {
          _histJetAzimuth_pTmax75_100->fill(dphi, weight);
        } else if (inRange(pT1/GeV, 100.0, 130.0)) {
          _histJetAzimuth_pTmax100_130->fill(dphi, weight);
        } else if (inRange(pT1/GeV, 130.0, 180.0)) {
          _histJetAzimuth_pTmax130_180->fill(dphi, weight);
        } else if (pT1/GeV > 180.0) {
          _histJetAzimuth_pTmax180_->fill(dphi, weight);
        }
      }

    }


    // Finalize
    void finalize() {
      // Normalize histograms to unit area
      normalize(_histJetAzimuth_pTmax75_100);
      normalize(_histJetAzimuth_pTmax100_130);
      normalize(_histJetAzimuth_pTmax130_180);
      normalize(_histJetAzimuth_pTmax180_);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _histJetAzimuth_pTmax75_100;
    Histo1DPtr _histJetAzimuth_pTmax100_130;
    Histo1DPtr _histJetAzimuth_pTmax130_180;
    Histo1DPtr _histJetAzimuth_pTmax180_;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2004_S5992206);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief STAR inclusive jet cross-section in pp at 200 GeV
  class STAR_2006_S6870392 : public Analysis {
  public:

    /// Constructor
    STAR_2006_S6870392()
      : Analysis("STAR_2006_S6870392")
    {    }


    /// @name Analysis methods
    //@{

    /// Book projections and histograms
    void init() {
      FinalState fs(-2.0, 2.0);
      addProjection(fs, "FS");
      // R=0.4, pTmin=0, seed_threshold=0.5:
      addProjection(FastJets(fs, FastJets::CDFMIDPOINT, 0.4, 0.5), "MidpointJets");

      _h_jet_pT_MB = bookHisto1D(1, 1, 1);
      _h_jet_pT_HT = bookHisto1D(2, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Skip if the event is empty
      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      if (fs.empty()) {
        MSG_DEBUG("Skipping event " << numEvents() << " because no final state found ");
        vetoEvent;
      }

      // Find jets
      const FastJets& jetpro = applyProjection<FastJets>(event, "MidpointJets");
      const Jets& jets = jetpro.jetsByPt();
      if (!jets.empty()) {
        const Jet& j1 = jets.front();
        if (inRange(fabs(j1.eta()), 0.2, 0.8)) {
          foreach (const Jet& j, jets) {
            const FourMomentum pj = j.momentum();
            _h_jet_pT_MB->fill(pj.pT(), weight);
            _h_jet_pT_HT->fill(pj.pT(), weight);
          }
        }
      }
    }



    /// Finalize
    void finalize() {
      double normalisation = crossSection()/picobarn/sumOfWeights()/(2*0.6*2*M_PI);
      scale(_h_jet_pT_MB, normalisation);
      scale(_h_jet_pT_HT, normalisation);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_jet_pT_MB;
    Histo1DPtr _h_jet_pT_HT;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2006_S6870392);

}

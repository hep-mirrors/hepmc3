// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF dijet cross-section, differential in dijet mass
  class CDF_2000_S4266730 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CDF_2000_S4266730()
      : Analysis("CDF_2000_S4266730")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs(-4.2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_mjj = bookHisto1D(1, 1, 1);

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByEt();
      if (jets.size()<2) {
        vetoEvent;
      }
      FourMomentum jet1 = jets[0].momentum();
      FourMomentum jet2 = jets[1].momentum();
      double eta1 = jet1.eta();
      double eta2 = jet2.eta();
      if (fabs(eta1)>2.0 || fabs(eta2)>2.0) {
        vetoEvent;
      }
      if (fabs(tanh((eta1-eta2)/2))>2.0/3.0) {
        vetoEvent;
      }
      double mjj = FourMomentum(jet1+jet2).mass()/GeV;
      if (mjj<180.0) {
        vetoEvent;
      }
      _h_mjj->fill(mjj, weight);

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_mjj, crossSection()/picobarn/sumOfWeights());
    }

    //@}
  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_mjj;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2000_S4266730);

}

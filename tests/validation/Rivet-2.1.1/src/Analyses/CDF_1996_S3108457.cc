// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF properties of high-mass multi-jet events
  class CDF_1996_S3108457 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CDF_1996_S3108457()
      : Analysis("CDF_1996_S3108457")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      /// Initialise and register projections here
      const FinalState fs(-4.2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");


      /// Book histograms here, e.g.:
      for (size_t i=0; i<5; ++i) {
        _h_m[i] = bookHisto1D(1+i, 1, 1);
        _h_costheta[i] = bookHisto1D(10+i, 1, 1);
        _h_pT[i] = bookHisto1D(15+i, 1, 1);
      }
      /// @todo Ratios of mass histograms left out: Binning doesn't work out
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      /// Do the event by event analysis here
      Jets jets;
      double sumEt = 0.0;
      FourMomentum jetsystem(0.0, 0.0, 0.0, 0.0);
      foreach (const Jet& jet, applyProjection<FastJets>(event, "Jets").jetsByEt()) {
        double Et = jet.momentum().Et();
        if (Et > 20.0*GeV) {
          jets.push_back(jet);
          sumEt += Et;
          jetsystem += jet.momentum();
        }
      }
      /// @todo include gaussian jet energy resolution smearing?

      if (jets.size() < 2 || jets.size() > 6) {
        vetoEvent;
      }

      if (sumEt < 420.0*GeV) {
        vetoEvent;
      }

      LorentzTransform cms_boost(-jetsystem.boostVector());
      FourMomentum jet0boosted(cms_boost.transform(jets[0].momentum()));

      double mass = jetsystem.mass();
      double costheta0 = fabs(cos(jet0boosted.theta()));

      if (costheta0 < 2.0/3.0) {
        _h_m[jets.size()-2]->fill(mass, weight);
      }

      if (mass > 600.0*GeV) {
        _h_costheta[jets.size()-2]->fill(costheta0, weight);
      }

      if (costheta0 < 2.0/3.0 && mass > 600.0*GeV) {
        foreach (const Jet jet, jets) {
          _h_pT[jets.size()-2]->fill(jet.pT(), weight);
        }
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      /// Normalise, scale and otherwise manipulate histograms here
      for (size_t i=0; i<5; ++i) {
        normalize(_h_m[i], 40.0);
        normalize(_h_costheta[i], 2.0);
        normalize(_h_pT[i], 20.0);
      }

    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_m[5];
    Histo1DPtr _h_costheta[5];
    Histo1DPtr _h_pT[5];

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1996_S3108457);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  class ATLAS_2011_S8971293 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2011_S8971293()
      : Analysis("ATLAS_2011_S8971293")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      /// Initialise and register projections
      addProjection(FastJets(FinalState(), FastJets::ANTIKT, 0.6), "AntiKtJets06");

      /// Book histograms
      _h_deltaPhi.addHistogram(110., 160., bookHisto1D(1, 1, 1));
      _h_deltaPhi.addHistogram(160., 210., bookHisto1D(1, 1, 2));
      _h_deltaPhi.addHistogram(210., 260., bookHisto1D(1, 1, 3));
      _h_deltaPhi.addHistogram(260., 310., bookHisto1D(1, 1, 4));
      _h_deltaPhi.addHistogram(310., 400., bookHisto1D(1, 1, 5));
      _h_deltaPhi.addHistogram(400., 500., bookHisto1D(1, 1, 6));
      _h_deltaPhi.addHistogram(500., 600., bookHisto1D(1, 1, 7));
      _h_deltaPhi.addHistogram(600., 800., bookHisto1D(1, 1, 8));
      _h_deltaPhi.addHistogram(800., 10000., bookHisto1D(1, 1, 9));
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets06;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "AntiKtJets06").jetsByPt(100.0*GeV)) {
        if (fabs(jet.rapidity()) < 2.8) {
          jets06.push_back(jet);
        }
      }
      if (jets06.size()>1){
        if (fabs(jets06[0].rapidity())<0.8 && fabs(jets06[1].rapidity())<0.8) {
          double observable = mapAngle0ToPi(jets06[0].momentum().phi()-jets06[1].momentum().phi()) / M_PI;
          _h_deltaPhi.fill(jets06[0].pT(), observable, weight);
        }
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      foreach (Histo1DPtr hist, _h_deltaPhi.getHistograms()) {
        normalize(hist, 1/M_PI);
      }
    }

    //@}


  private:

    /// @name Histograms
    //@{
    BinnedHistogram<double> _h_deltaPhi;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S8971293);

}

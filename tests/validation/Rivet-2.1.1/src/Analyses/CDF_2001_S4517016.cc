// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF two jet tripply-differential cross-section
  class CDF_2001_S4517016 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CDF_2001_S4517016()
      : Analysis("CDF_2001_S4517016")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs(-4.2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_ET.addHistogram(0.1, 0.7, bookHisto1D(1, 1, 1));
      _h_ET.addHistogram(0.7, 1.4, bookHisto1D(2, 1, 1));
      _h_ET.addHistogram(1.4, 2.1, bookHisto1D(3, 1, 1));
      _h_ET.addHistogram(2.1, 3.0, bookHisto1D(4, 1, 1));
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets = applyProjection<FastJets>(event, "Jets").jetsByEt(10.0*GeV);
      if (jets.size()<2) {
        vetoEvent;
      }
      FourMomentum jet1 = jets[0].momentum();
      FourMomentum jet2 = jets[1].momentum();
      double eta1 = fabs(jet1.eta());
      double eta2 = fabs(jet2.eta());
      double ET1 = jet1.Et();
      double ET2 = jet2.Et();
      if (eta1<0.1 || eta1>0.7 || ET1<40.0*GeV) {
        vetoEvent;
      }
      if (eta2<0.1 || eta2>3.0) {
        vetoEvent;
      }
      _h_ET.fill(eta2, ET1, weight);
      if (eta2<0.7 && ET2>40.0*GeV) _h_ET.fill(eta1, ET2, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      double deta1 = 1.2;
      _h_ET.scale(crossSection()/nanobarn/sumOfWeights()/deta1 / 2.0, this);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    BinnedHistogram<double> _h_ET;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2001_S4517016);

}

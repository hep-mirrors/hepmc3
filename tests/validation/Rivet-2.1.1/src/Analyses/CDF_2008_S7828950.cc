// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF Run II inclusive jet cross-section using the Midpoint algorithm.
  ///
  /// The analysis includes 1.1fb^-1 of CDF data and is the first with a
  /// cone algorithm to include the forward region of the detector.
  /// arXiv:0807.2204 to be published in PRD
  class CDF_2008_S7828950 : public Analysis {
  public:

    /// Constructor
    CDF_2008_S7828950()
      : Analysis("CDF_2008_S7828950")
    {    }


    /// @name Analysis methods
    //@{

    // Book histos and set counters for number of events passed in each one
    void init() {
      const FinalState fs;
      addProjection(FastJets(fs, FastJets::CDFMIDPOINT, 0.7), "JetsM07");

      _binnedHistosR07.addHistogram(  0, 0.1, bookHisto1D(1, 1, 1));
      _binnedHistosR07.addHistogram(0.1, 0.7, bookHisto1D(2, 1, 1));
      _binnedHistosR07.addHistogram(0.7, 1.1, bookHisto1D(3, 1, 1));
      _binnedHistosR07.addHistogram(1.1, 1.6, bookHisto1D(4, 1, 1));
      _binnedHistosR07.addHistogram(1.6, 2.1, bookHisto1D(5, 1, 1));

    }


    // Do the analysis
    void analyze(const Event& event) {
      const double weight = event.weight();
      foreach (const Jet& jet, applyProjection<FastJets>(event, "JetsM07").jets(62.0*GeV)) {
        _binnedHistosR07.fill(fabs(jet.rapidity()), jet.pT(), weight);
      }
    }


    // Normalise histograms to cross-section
    void finalize() {
      _binnedHistosR07.scale(crossSection()/nanobarn/sumOfWeights()/2.0, this);
    }

    //@}


  private:

    /// Histograms in different eta regions
    BinnedHistogram<double> _binnedHistosR07;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2008_S7828950);

}

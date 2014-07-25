// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF inclusive jet cross-section using the \f$ k_\perp \f$ algorithm
  class CDF_2007_S7057202 : public Analysis {
  public:

    CDF_2007_S7057202()
      : Analysis("CDF_2007_S7057202")
    {    }


    void init() {
      // Set up projections
      const FinalState fs;
      addProjection(FastJets(fs, FastJets::KT, 0.5), "JetsD05");
      addProjection(FastJets(fs, FastJets::KT, 0.7), "JetsD07");
      addProjection(FastJets(fs, FastJets::KT, 1.0), "JetsD10");

      // Book histos
      _binnedHistosD07.addHistogram(  0, 0.1, bookHisto1D(1, 1, 1));
      _binnedHistosD07.addHistogram(0.1, 0.7, bookHisto1D(2, 1, 1));
      _binnedHistosD07.addHistogram(0.7, 1.1, bookHisto1D(3, 1, 1));
      _binnedHistosD07.addHistogram(1.1, 1.6, bookHisto1D(4, 1, 1));
      _binnedHistosD07.addHistogram(1.6, 2.1, bookHisto1D(5, 1, 1));
      _histoD05 = bookHisto1D(6, 1, 1);
      _histoD10 = bookHisto1D(7, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      foreach (const Jet& jet, applyProjection<JetAlg>(event, "JetsD07").jets(54.0*GeV)) {
        double y = fabs(jet.rapidity());
        _binnedHistosD07.fill(y, jet.pT(), weight);
      }

      foreach (const Jet& jet, applyProjection<JetAlg>(event, "JetsD05").jets(54.0*GeV)) {
        double y = fabs(jet.rapidity());
        if (y >= 0.1 && y < 0.7) _histoD05->fill(jet.pT(), weight);
      }

      foreach (const Jet& jet, applyProjection<JetAlg>(event, "JetsD10").jets(54.0*GeV)) {
        double y = fabs(jet.rapidity());
        if (y >= 0.1 && y < 0.7) _histoD10->fill(jet.pT(), weight);
      }
    }


    // Normalise histograms to cross-section
    void finalize() {
      const double xSec = crossSectionPerEvent()/nanobarn;

      scale(_histoD05, xSec);
      scale(_histoD10, xSec);
      // scale to xSec/yBinWidth and take into account the double yBinWidth due
      // to the absolute value of y
      _binnedHistosD07.scale(xSec/2.0, this);
    }

  private:

    BinnedHistogram<double> _binnedHistosD07;

    // Single histogram for the \f$R=0.5\f$ \f$k_\perp\f$ jets
    Histo1DPtr _histoD05;

    // Single histogram for the \f$R=1.0\f$ \f$k_\perp\f$ jets
    Histo1DPtr _histoD10;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2007_S7057202);

}

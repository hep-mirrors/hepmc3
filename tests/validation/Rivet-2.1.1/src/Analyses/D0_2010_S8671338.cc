// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ZFinder.hh"

namespace Rivet {


  /// @brief Measurement of Z(->muon muon) pT differential cross-section
  /// @author Flavia Dias
  class D0_2010_S8671338 : public Analysis {
  public:

    /// Constructor
    D0_2010_S8671338()
      : Analysis("D0_2010_S8671338")
    {    }


    ///@name Analysis methods
    //@{

    /// Add projections and book histograms
    void init() {
      ZFinder zfinder(FinalState(), -1.7, 1.7, 15*GeV, PID::MUON, 65*GeV, 115*GeV,
                      0.2, ZFinder::NOCLUSTER, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");

      _h_Z_pT_normalised = bookHisto1D(1, 1, 1);
      _h_Z_pT_xs = bookHisto1D(2, 1, 1);
    }


    // Do the analysis
    void analyze(const Event& e) {
      const double weight = e.weight();
      const ZFinder& zfinder = applyProjection<ZFinder>(e, "ZFinder");
      if (zfinder.bosons().size()==1) {
        double ZpT = zfinder.bosons()[0].pT()/GeV;
        _h_Z_pT_normalised->fill(ZpT, weight);
        _h_Z_pT_xs->fill(ZpT, weight);
      }
    }


    /// Finalize
    void finalize() {
      normalize(_h_Z_pT_normalised);
      normalize(_h_Z_pT_xs, crossSection());
    }

    //@}


  private:

    /// @name Histogram
    Histo1DPtr _h_Z_pT_normalised;
    Histo1DPtr _h_Z_pT_xs;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2010_S8671338);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief D0 dijet angular distributions
  class D0_2009_S8320160 : public Analysis {
  public:

    /// @name Construction
    //@{

    /// Constructor
    D0_2009_S8320160() : Analysis("D0_2009_S8320160")
    {
    }

    //@}


    /// @name Analysis methods
    //@{

    // Book histograms
    void init() {
      FinalState fs;
      FastJets conefinder(fs, FastJets::D0ILCONE, 0.7);
      addProjection(conefinder, "ConeFinder");

      _h_chi_dijet.addHistogram(250., 300., bookHisto1D(1, 1, 1));
      _h_chi_dijet.addHistogram(300., 400., bookHisto1D(2, 1, 1));
      _h_chi_dijet.addHistogram(400., 500., bookHisto1D(3, 1, 1));
      _h_chi_dijet.addHistogram(500., 600., bookHisto1D(4, 1, 1));
      _h_chi_dijet.addHistogram(600., 700., bookHisto1D(5, 1, 1));
      _h_chi_dijet.addHistogram(700., 800., bookHisto1D(6, 1, 1));
      _h_chi_dijet.addHistogram(800., 900., bookHisto1D(7, 1, 1));
      _h_chi_dijet.addHistogram(900., 1000., bookHisto1D(8, 1, 1));
      _h_chi_dijet.addHistogram(1000., 1100., bookHisto1D(9, 1, 1));
      _h_chi_dijet.addHistogram(1100., 1960, bookHisto1D(10, 1, 1));
    }



    /// Do the analysis
    void analyze(const Event & e) {
      const double weight = e.weight();

      const Jets& jets = applyProjection<JetAlg>(e, "ConeFinder").jetsByPt();
      if (jets.size() < 2) vetoEvent;

      FourMomentum j0(jets[0].momentum());
      FourMomentum j1(jets[1].momentum());
      double y0 = j0.rapidity();
      double y1 = j1.rapidity();

      if (fabs(y0+y1)>2) vetoEvent;

      double mjj = FourMomentum(j0+j1).mass();
      double chi = exp(fabs(y0-y1));
      _h_chi_dijet.fill(mjj, chi, weight);
    }



    /// Finalize
    void finalize() {
      foreach (Histo1DPtr hist, _h_chi_dijet.getHistograms()) {
        normalize(hist);
      }
    }

    //@}


  private:

    /// @name Histograms
    //@{
    BinnedHistogram<double> _h_chi_dijet;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2009_S8320160);

}

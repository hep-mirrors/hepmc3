// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/BinnedHistogram.hh"

namespace Rivet {


  class CMS_2011_S8968497 : public Analysis {
  public:

    CMS_2011_S8968497()
      : Analysis("CMS_2011_S8968497")
    { }


    void init() {
      FinalState fs;
      FastJets antikt(fs, FastJets::ANTIKT, 0.5);
      addProjection(antikt, "ANTIKT");
      _h_chi_dijet.addHistogram(2200., 7000., bookHisto1D(1, 1, 1));
      _h_chi_dijet.addHistogram(1800., 2200., bookHisto1D(2, 1, 1));
      _h_chi_dijet.addHistogram(1400., 1800., bookHisto1D(3, 1, 1));
      _h_chi_dijet.addHistogram(1100., 1400., bookHisto1D(4, 1, 1));
      _h_chi_dijet.addHistogram( 850., 1100., bookHisto1D(5, 1, 1));
      _h_chi_dijet.addHistogram( 650.,  850., bookHisto1D(6, 1, 1));
      _h_chi_dijet.addHistogram( 500.,  650., bookHisto1D(7, 1, 1));
      _h_chi_dijet.addHistogram( 350.,  500., bookHisto1D(8, 1, 1));
      _h_chi_dijet.addHistogram( 250.,  350., bookHisto1D(9, 1, 1));
    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      const Jets& jets = applyProjection<JetAlg>(event, "ANTIKT").jetsByPt();
      if (jets.size() < 2) vetoEvent;
      FourMomentum j0(jets[0].momentum());
      FourMomentum j1(jets[1].momentum());
      double y0 = j0.rapidity();
      double y1 = j1.rapidity();
      if (fabs(y0+y1)/2. > 1.11) vetoEvent;
      double mjj = FourMomentum(j0+j1).mass();
      double chi = exp(fabs(y0-y1));
      if(chi<16.)  _h_chi_dijet.fill(mjj, chi, weight);
    }


    void finalize() {
      foreach (Histo1DPtr hist, _h_chi_dijet.getHistograms()) {
        normalize(hist);
      }
    }


  private:

    BinnedHistogram<double> _h_chi_dijet;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S8968497);

}

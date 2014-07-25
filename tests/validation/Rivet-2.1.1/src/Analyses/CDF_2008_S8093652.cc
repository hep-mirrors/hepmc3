// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF dijet mass spectrum
  class CDF_2008_S8093652 : public Analysis {
  public:

    /// Constructor
    CDF_2008_S8093652()
      : Analysis("CDF_2008_S8093652")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      FastJets conefinder(fs, FastJets::CDFMIDPOINT, 0.7);
      addProjection(conefinder, "ConeFinder");

      _h_m_dijet = bookHisto1D(1, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event & e) {
      const double weight = e.weight();

      const JetAlg& jetpro = applyProjection<JetAlg>(e, "ConeFinder");
      const Jets& jets = jetpro.jetsByPt();

      if (jets.size() < 2) vetoEvent;

      const FourMomentum j0(jets[0].momentum());
      const FourMomentum j1(jets[1].momentum());
      if (fabs(j1.rapidity()) > 1.0 || fabs(j0.rapidity()) > 1.0) {
        vetoEvent;
      }

      double mjj = FourMomentum(j0+j1).mass();
      _h_m_dijet->fill(mjj, weight);
    }


    /// Finalize
    void finalize() {
      scale(_h_m_dijet, crossSection()/sumOfWeights());
    }
    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_m_dijet;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2008_S8093652);

}

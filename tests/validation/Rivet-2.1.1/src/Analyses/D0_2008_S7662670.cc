// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief D0 differential jet cross sections
  /// @author Andy Buckley
  /// @author Gavin Hesketh
  class D0_2008_S7662670 : public Analysis {

  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    D0_2008_S7662670()
      : Analysis("D0_2008_S7662670")
    {    }

    //@}


    /// @name Analysis methods
    //@{

    void init()
    {

      // Full final state
      FinalState fs;
      addProjection(fs, "FS");

      // Jets
      FastJets jetpro(fs, FastJets::D0ILCONE, 0.7);
      addProjection(jetpro, "Jets");

      // Book histograms
      _h_dsigdptdy_y00_04 = bookHisto1D(1, 1, 1);
      _h_dsigdptdy_y04_08 = bookHisto1D(2, 1, 1);
      _h_dsigdptdy_y08_12 = bookHisto1D(3, 1, 1);
      _h_dsigdptdy_y12_16 = bookHisto1D(4, 1, 1);
      _h_dsigdptdy_y16_20 = bookHisto1D(5, 1, 1);
      _h_dsigdptdy_y20_24 = bookHisto1D(6, 1, 1);
    }



    /// Do the analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Skip if the event is empty
      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      if (fs.empty()) {
        MSG_DEBUG("Empty event!");
        vetoEvent;
      }

      // Find the jets
      const JetAlg& jetpro = applyProjection<JetAlg>(event, "Jets");
      // Fill histo for each jet
      foreach (const Jet& j, jetpro.jets(50.0*GeV)) {
        const double pt = j.pT();
        const double y = fabs(j.rapidity());
        MSG_TRACE("Filling histos: pT = " << pt/GeV
            << ", |y| = " << y);
        if (y < 0.4) {
          _h_dsigdptdy_y00_04->fill(pt/GeV, weight);
        } else if (y < 0.8) {
          _h_dsigdptdy_y04_08->fill(pt/GeV, weight);
        } else if (y < 1.2) {
          _h_dsigdptdy_y08_12->fill(pt/GeV, weight);
        } else if (y < 1.6) {
          _h_dsigdptdy_y12_16->fill(pt/GeV, weight);
        } else if (y < 2.0) {
          _h_dsigdptdy_y16_20->fill(pt/GeV, weight);
        } else if (y < 2.4) {
          _h_dsigdptdy_y20_24->fill(pt/GeV, weight);
        }
      }

    }


    /// Finalize
    void finalize() {
      /// Scale by L_eff = sig_MC * L_exp / num_MC
      const double lumi_mc = sumOfWeights() / crossSection();
      const double scalefactor =  1 / lumi_mc;
      scale(_h_dsigdptdy_y00_04, scalefactor);
      scale(_h_dsigdptdy_y04_08, scalefactor);
      scale(_h_dsigdptdy_y08_12, scalefactor);
      scale(_h_dsigdptdy_y12_16, scalefactor);
      scale(_h_dsigdptdy_y16_20, scalefactor);
      scale(_h_dsigdptdy_y20_24, scalefactor);
    }

    //@}

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_dsigdptdy_y00_04;
    Histo1DPtr _h_dsigdptdy_y04_08;
    Histo1DPtr _h_dsigdptdy_y08_12;
    Histo1DPtr _h_dsigdptdy_y12_16;
    Histo1DPtr _h_dsigdptdy_y16_20;
    Histo1DPtr _h_dsigdptdy_y20_24;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2008_S7662670);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerCDFRun0Run1.hh"

namespace Rivet {


  /// @brief CDF Run I charged multiplicity measurement
  /// @author Hendrik Hoeth
  ///
  /// This analysis measures the charged multiplicity distribution
  /// in minimum bias events at two different center-of-mass energies:
  /// \f$ \sqrt{s} = \f$ 630 and 1800 GeV.
  ///
  /// Particles with c*tau > 10 mm are considered stable, i.e. they
  /// are reconstructed and their decay products removed. Selection
  /// cuts are |eta|<1 and pT>0.4 GeV.
  ///
  /// @par Run conditions
  ///
  /// @arg Two different beam energies: \f$ \sqrt{s} = \$f 630 & 1800 GeV
  /// @arg Run with generic QCD events.
  /// @arg Set particles with c*tau > 10 mm stable
  class CDF_2002_S4796047 : public Analysis {
  public:

    /// Constructor
    CDF_2002_S4796047()
      : Analysis("CDF_2002_S4796047")
    {
      _sumWTrig = 0;
    }


    /// @name Analysis methods
    //@{

    /// Book projections and histograms
    void init() {
      addProjection(TriggerCDFRun0Run1(), "Trigger");
      const ChargedFinalState cfs(-1.0, 1.0, 0.4*GeV);
      addProjection(cfs, "FS");

      // Histos
      if (fuzzyEquals(sqrtS()/GeV, 630)) {
        _hist_multiplicity  = bookHisto1D(1, 1, 1);
        _hist_pt_vs_multiplicity  = bookProfile1D(3, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 1800)) {
        _hist_multiplicity = bookHisto1D(2, 1, 1);
        _hist_pt_vs_multiplicity = bookProfile1D(4, 1, 1);
      }
    }


    /// Do the analysis
    void analyze(const Event& evt) {
      // Trigger
      const bool trigger = applyProjection<TriggerCDFRun0Run1>(evt, "Trigger").minBiasDecision();
      if (!trigger) vetoEvent;
      const double weight = evt.weight();
      _sumWTrig += weight;

      // Get beam energy and tracks
      const ChargedFinalState& fs = applyProjection<ChargedFinalState>(evt, "FS");
      const size_t numParticles = fs.particles().size();

      // Fill histos of charged multiplicity distributions
      _hist_multiplicity->fill(numParticles, weight);

      // Fill histos for <pT> vs. charged multiplicity
      foreach (const Particle& p, fs.particles()) {
        const double pT = p.pT();
        _hist_pt_vs_multiplicity->fill(numParticles, pT/GeV, weight);
      }

    }


    void finalize() {
      // This normalisation is NOT a cross-section.
      // In the paper the x-axes (!) of the histograms are
      // scaled such that they can put both energies in the
      // same plot. Of course this affects the area, too.
      // Since we want to plot the actual multiplicity, we
      // scale the x-axes back and have to adjust the areas
      // accordingly. The scale factors are given in the
      // legend of the plot in the paper. Have a look at
      // figure 1 and everything immediately becomes clear.
      // DON'T TRY TO REPAIR THIS, YOU WILL BREAK IT.
      if (fuzzyEquals(sqrtS()/GeV, 630)) {
        normalize(_hist_multiplicity, 3.21167); // fixed norm OK
      } else if (fuzzyEquals(sqrtS()/GeV, 1800)) {
        normalize(_hist_multiplicity, 4.19121); // fixed norm OK
      }
    }

    //@}


  private:

    /// @name Counter
    //@{
    double _sumWTrig;
    //@}

    /// @name Histos
    //@{
    Histo1DPtr _hist_multiplicity;
    Profile1DPtr _hist_pt_vs_multiplicity;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2002_S4796047);

}

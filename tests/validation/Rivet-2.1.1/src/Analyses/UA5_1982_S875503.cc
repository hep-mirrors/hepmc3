// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerUA5.hh"

namespace Rivet {


  /// @brief UA5 multiplicity and \f$ \eta \f$ distributions
  class UA5_1982_S875503 : public Analysis {
  public:

    /// Default constructor
    UA5_1982_S875503() : Analysis("UA5_1982_S875503") {
      _sumWTrig = 0;
    }


    /// @name Analysis methods
    //@{

    /// Set up projections and book histos
    void init() {
      addProjection(TriggerUA5(), "Trigger");
      addProjection(ChargedFinalState(-3.5, 3.5), "CFS");

      // Book histos based on pp or ppbar beams
      if (beamIds().first == beamIds().second) {
        _hist_nch = bookHisto1D(2,1,1);
        _hist_eta = bookHisto1D(3,1,1);
      } else {
        _hist_nch = bookHisto1D(2,1,2);
        _hist_eta = bookHisto1D(4,1,1);
      }
    }


    void analyze(const Event& event) {
      // Trigger
      const TriggerUA5& trigger = applyProjection<TriggerUA5>(event, "Trigger");
      if (!trigger.nsdDecision()) vetoEvent;
      const double weight = event.weight();
      _sumWTrig += weight;

      // Get tracks
      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");

      // Fill mean charged multiplicity histos
      _hist_nch->fill(_hist_nch->bin(0).midpoint(), cfs.size()*weight);

      // Iterate over all tracks and fill eta histograms
      foreach (const Particle& p, cfs.particles()) {
        const double eta = fabs(p.momentum().pseudorapidity());
        _hist_eta->fill(eta, weight);
      }

    }


    void finalize() {
      /// @todo Why the factor of 2 on Nch for ppbar?
      if (beamIds().first == beamIds().second) {
        scale(_hist_nch, 1.0/_sumWTrig);
      } else {
        scale(_hist_nch, 0.5/_sumWTrig);
      }
      scale(_hist_eta, 0.5/_sumWTrig);
    }

    //@}


  private:

    /// @name Counters
    //@{
    double _sumWTrig;
    //@}

    /// @name Histogram collections
    //@{
    Histo1DPtr _hist_nch;
    Histo1DPtr _hist_eta;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(UA5_1982_S875503);

}

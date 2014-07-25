// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/TriggerUA5.hh"

namespace Rivet {


  /// @brief UA5 \f$ \eta \f$ distributions at 200 and 900 GeV
  class UA5_1986_S1583476 : public Analysis {
  public:

    /// Constructor
    UA5_1986_S1583476() : Analysis("UA5_1986_S1583476") {
      _sumWTrig = 0;
      _sumWTrigNSD = 0;
    }


    /// @name Analysis methods
    //@{

    /// Set up projections and histograms
    void init() {
      addProjection(TriggerUA5(), "Trigger");
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(-5.0, 5.0), "CFS50");

      // Histograms
      if (fuzzyEquals(sqrtS()/GeV, 200.0, 1E-4)) {
        _hist_eta_nsd       = bookHisto1D(1,1,1);
        _hist_eta_inelastic = bookHisto1D(1,1,2);
        for (int i = 1; i <= 6; ++i) {
          _sumWn += 0.0;
          _hists_eta_nsd += bookHisto1D(2,1,i);
        }
      } else if (fuzzyEquals(sqrtS()/GeV, 900.0, 1E-4)) {
        _hist_eta_nsd       = bookHisto1D(1,1,3);
        _hist_eta_inelastic = bookHisto1D(1,1,4);
        for (int i = 1; i <= 9; ++i) {
          _sumWn += 0.0;
          _hists_eta_nsd += bookHisto1D(3,1,i);
        }
      }
    }


    /// Fill eta histograms (in Nch bins)
    void analyze(const Event& event) {
      // Trigger
      const TriggerUA5& trigger = applyProjection<TriggerUA5>(event, "Trigger");
      if (!trigger.sdDecision()) vetoEvent;
      const bool isNSD = trigger.nsdDecision();

      // Get the index corresponding to the max Nch range histo/sum(w) vector index
      const ChargedFinalState& cfs50 = applyProjection<ChargedFinalState>(event, "CFS50");
      const int numP = cfs50.size();
      const int ni = (int)floor(static_cast<float>(numP-2)/10.0);
      const int num_idx = min(ni, (int)_sumWn.size()-1);
      MSG_TRACE("Multiplicity index: " << numP << " charged particles -> #" << num_idx);

      // Update weights
      const double weight = event.weight();
      _sumWTrig += weight;
      if (isNSD) {
        _sumWTrigNSD += weight;
        if (num_idx >= 0) _sumWn[num_idx] += weight;
      }

      // Fill histos
      foreach (const Particle& p, cfs50.particles()) {
        const double eta = fabs(p.momentum().pseudorapidity());
        _hist_eta_inelastic->fill(eta, weight);
        if (isNSD) {
          _hist_eta_nsd->fill(eta, weight);
          if (num_idx >= 0) _hists_eta_nsd[num_idx]->fill(eta, weight);
        }
      }
    }


    /// Scale histos
    void finalize() {
      MSG_DEBUG("sumW_NSD,inel = " << _sumWTrigNSD << ", " << _sumWTrig);
      scale(_hist_eta_nsd, 0.5/_sumWTrigNSD);
      scale(_hist_eta_inelastic, 0.5/_sumWTrig);
      //
      MSG_DEBUG("sumW[n] = " << _sumWn);
      for (size_t i = 0; i < _hists_eta_nsd.size(); ++i) {
        scale(_hists_eta_nsd[i], 0.5/_sumWn[i]);
      }
    }


  private:

    /// @name Weight counters
    //@{
    double _sumWTrig;
    double _sumWTrigNSD;
    vector<double> _sumWn;
    //@}

    /// @name Histograms
    //@{
    Histo1DPtr _hist_eta_nsd;
    Histo1DPtr _hist_eta_inelastic;
    vector<Histo1DPtr> _hists_eta_nsd;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(UA5_1986_S1583476);

}

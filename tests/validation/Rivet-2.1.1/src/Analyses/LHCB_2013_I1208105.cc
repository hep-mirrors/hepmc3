// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class LHCB_2013_I1208105 : public Analysis {
  public:

    LHCB_2013_I1208105()
      : Analysis("LHCB_2013_I1208105")
    {   }


    void init() {
      // Projections
      addProjection(FinalState(1.9, 4.9), "forwardFS");
      addProjection(FinalState(-3.5,-1.5), "backwardFS");
      addProjection(ChargedFinalState(1.9, 4.9), "forwardCFS");
      addProjection(ChargedFinalState(-3.5,-1.5), "backwardCFS");

      // Histos
      _s_chEF_minbias = bookScatter2D(1, 1, 1, true);
      _s_chEF_hard = bookScatter2D(2, 1, 1, true);
      _s_chEF_diff = bookScatter2D(3, 1, 1, true);
      _s_chEF_nondiff = bookScatter2D(4, 1, 1, true);
      _s_totEF_minbias = bookScatter2D(5, 1, 1, true);
      _s_totEF_hard = bookScatter2D(6, 1, 1, true);
      _s_totEF_diff = bookScatter2D(7, 1, 1, true);
      _s_totEF_nondiff = bookScatter2D(8, 1, 1, true);

      // Temporary profiles and histos
      /// @todo Convert to declared/registered temp histos
      _tp_chEF_minbias.reset(new YODA::Profile1D(refData(1,1,1)));
      _tp_chEF_hard.reset(new YODA::Profile1D(refData(2,1,1)));
      _tp_chEF_diff.reset(new YODA::Profile1D(refData(3,1,1)));
      _tp_chEF_nondiff.reset(new YODA::Profile1D(refData(4,1,1)));
      _tp_totEF_minbias.reset(new YODA::Profile1D(refData(5,1,1)));
      _tp_totEF_hard.reset(new YODA::Profile1D(refData(6,1,1)));
      _tp_totEF_diff.reset(new YODA::Profile1D(refData(7,1,1)));
      _tp_totEF_nondiff.reset(new YODA::Profile1D(refData(8,1,1)));
      //
      _th_chN_minbias.reset(new YODA::Histo1D(refData(1,1,1)));
      _th_chN_hard.reset(new YODA::Histo1D(refData(2,1,1)));
      _th_chN_diff.reset(new YODA::Histo1D(refData(3,1,1)));
      _th_chN_nondiff.reset(new YODA::Histo1D(refData(4,1,1)));
      _th_totN_minbias.reset(new YODA::Histo1D(refData(5,1,1)));
      _th_totN_hard.reset(new YODA::Histo1D(refData(6,1,1)));
      _th_totN_diff.reset(new YODA::Histo1D(refData(7,1,1)));
      _th_totN_nondiff.reset(new YODA::Histo1D(refData(8,1,1)));

      // Counters
      _mbSumW = 0.0; _hdSumW = 0.0; _dfSumW = 0.0; _ndSumW = 0.0;
      _mbchSumW = 0.0; _hdchSumW = 0.0; _dfchSumW = 0.0; _ndchSumW = 0.0;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const FinalState& ffs = applyProjection<FinalState>(event, "forwardFS");
      const FinalState& bfs = applyProjection<FinalState>(event, "backwardFS");
      const ChargedFinalState& fcfs = applyProjection<ChargedFinalState>(event, "forwardCFS");
      const ChargedFinalState& bcfs = applyProjection<ChargedFinalState>(event, "backwardCFS");

      // Veto this event completely if there are no forward *charged* particles
      if (fcfs.empty()) vetoEvent;

      // Charged and neutral version
      {
        // Decide empirically if this is a "hard" or "diffractive" event
        bool ishardEvt = false;
        foreach (const Particle& p, ffs.particles()) {
          if (p.momentum().pT() > 3.0*GeV) { ishardEvt = true; break; }
        }
        // Decide empirically if this is a "diffractive" event
        /// @todo Can be "diffractive" *and* "hard"?
        bool isdiffEvt = (bfs.size() == 0);

        // Update event-type weight counters
        _mbSumW += weight;
        (isdiffEvt ? _dfSumW : _ndSumW) += weight;
        if (ishardEvt) _hdSumW += weight;

        // Plot energy flow
        foreach (const Particle& p, ffs.particles()) {
          const double eta = p.momentum().eta();
          const double energy = p.momentum().E();
          _tp_totEF_minbias->fill(eta, energy, weight);
          _th_totN_minbias->fill(eta, weight);
          if (ishardEvt) {
            _tp_totEF_hard->fill(eta, energy, weight);
            _th_totN_hard->fill(eta, weight);
          }
          if (isdiffEvt) {
            _tp_totEF_diff->fill(eta, energy, weight);
            _th_totN_diff->fill(eta, weight);
          } else {
            _tp_totEF_nondiff->fill(eta, energy, weight);
            _th_totN_nondiff->fill(eta, weight);
          }
        }
      }


      // Charged-only version
      {
        bool ishardEvt = false;
        foreach (const Particle& p, fcfs.particles()) {
          if (p.momentum().pT() > 3.0*GeV) { ishardEvt = true; break; }
        }
        // Decide empirically if this is a "diffractive" event
        /// @todo Can be "diffractive" *and* "hard"?
        bool isdiffEvt = (bcfs.size() == 0);

        // Update event-type weight counters
        _mbchSumW += weight;
        (isdiffEvt ? _dfchSumW : _ndchSumW) += weight;
        if (ishardEvt) _hdchSumW += weight;

        // Plot energy flow
        foreach (const Particle& p, fcfs.particles()) {
          const double eta = p.momentum().eta();
          const double energy = p.momentum().E();
          _tp_chEF_minbias->fill(eta, energy, weight);
          _th_chN_minbias->fill(eta, weight);
          if (ishardEvt) {
            _tp_chEF_hard->fill(eta, energy, weight);
            _th_chN_hard->fill(eta, weight);
          }
          if (isdiffEvt) {
            _tp_chEF_diff->fill(eta, energy, weight);
            _th_chN_diff->fill(eta, weight);
          } else {
            _tp_chEF_nondiff->fill(eta, energy, weight);
            _th_chN_nondiff->fill(eta, weight);
          }
        }
      }

    }


    void finalize() {
      for (size_t i = 0; i < _s_totEF_minbias->numPoints(); ++i) {
        const double val = _tp_totEF_minbias->bin(i).mean() * _th_totN_minbias->bin(i).height();
        const double err = (_tp_totEF_minbias->bin(i).mean() * _th_totN_minbias->bin(i).heightErr() +
                            _tp_totEF_minbias->bin(i).stdErr() * _th_totN_minbias->bin(i).height());
        _s_totEF_minbias->point(i).setY(val/_mbSumW, err/_mbSumW);
      }
      for (size_t i = 0; i < _s_totEF_hard->numPoints(); ++i) {
        const double val = _tp_totEF_hard->bin(i).mean() * _th_totN_hard->bin(i).height();
        const double err = (_tp_totEF_hard->bin(i).mean() * _th_totN_hard->bin(i).heightErr() +
                            _tp_totEF_hard->bin(i).stdErr() * _th_totN_hard->bin(i).height());
        _s_totEF_hard->point(i).setY(val/_hdSumW, err/_hdSumW);
      }
      for (size_t i = 0; i < _s_totEF_diff->numPoints(); ++i) {
        const double val = _tp_totEF_diff->bin(i).mean() * _th_totN_diff->bin(i).height();
        const double err = (_tp_totEF_diff->bin(i).mean() * _th_totN_diff->bin(i).heightErr() +
                                   _tp_totEF_diff->bin(i).stdErr() * _th_totN_diff->bin(i).height());
        _s_totEF_diff->point(i).setY(val/_dfSumW, err/_dfSumW);
      }
      for (size_t i = 0; i < _s_totEF_nondiff->numPoints(); ++i) {
        const double val = _tp_totEF_nondiff->bin(i).mean() * _th_totN_nondiff->bin(i).height();
        const double err = (_tp_totEF_nondiff->bin(i).mean() * _th_totN_nondiff->bin(i).heightErr() +
                            _tp_totEF_nondiff->bin(i).stdErr() * _th_totN_nondiff->bin(i).height());
        _s_totEF_nondiff->point(i).setY(val/_ndSumW, err/_ndSumW);
      }
      for (size_t i = 0; i < _s_chEF_minbias->numPoints(); ++i) {
        const double val = _tp_chEF_minbias->bin(i).mean() * _th_chN_minbias->bin(i).height();
        const double err = (_tp_chEF_minbias->bin(i).mean() * _th_chN_minbias->bin(i).heightErr() +
                            _tp_chEF_minbias->bin(i).stdErr() * _th_chN_minbias->bin(i).height());
        _s_chEF_minbias->point(i).setY(val/_mbchSumW, err/_mbchSumW);
      }
      for (size_t i = 0; i < _s_chEF_hard->numPoints(); ++i) {
        const double val = _tp_chEF_hard->bin(i).mean() * _th_chN_hard->bin(i).height();
        const double err = (_tp_chEF_hard->bin(i).mean() * _th_chN_hard->bin(i).heightErr() +
                            _tp_chEF_hard->bin(i).stdErr() * _th_chN_hard->bin(i).height());
        _s_chEF_hard->point(i).setY(val/_hdchSumW, err/_hdchSumW);
      }
      for (size_t i = 0; i < _s_chEF_diff->numPoints(); ++i) {
        const double val = _tp_chEF_diff->bin(i).mean() * _th_chN_diff->bin(i).height();
        const double err = (_tp_chEF_diff->bin(i).mean() * _th_chN_diff->bin(i).heightErr() +
                            _tp_chEF_diff->bin(i).stdErr() * _th_chN_diff->bin(i).height());
        _s_chEF_diff->point(i).setY(val/_dfchSumW, err/_dfchSumW);
      }
      for (size_t i = 0; i < _s_chEF_nondiff->numPoints(); ++i) {
        const double val = _tp_chEF_nondiff->bin(i).mean() * _th_chN_nondiff->bin(i).height();
        const double err = (_tp_chEF_nondiff->bin(i).mean() * _th_chN_nondiff->bin(i).heightErr() +
                            _tp_chEF_nondiff->bin(i).stdErr() * _th_chN_nondiff->bin(i).height());
        _s_chEF_nondiff->point(i).setY(val/_ndchSumW, err/_ndchSumW);
      }
    }


  private:

    /// @name Histograms and counters
    ///
    /// @note Histograms correspond to charged and total EF for each class of events:
    ///  minimum bias, hard scattering, diffractive enriched and non-diffractive enriched.
    //@{

    // Scatters to be filled in finalize with 1/d_eta <N(eta)><E(eta)>
    Scatter2DPtr _s_totEF_minbias, _s_totEF_hard, _s_totEF_diff, _s_totEF_nondiff;
    Scatter2DPtr _s_chEF_minbias, _s_chEF_hard, _s_chEF_diff, _s_chEF_nondiff;

    // Temp profiles containing <E(eta)>
    shared_ptr<YODA::Profile1D> _tp_totEF_minbias, _tp_totEF_hard, _tp_totEF_diff, _tp_totEF_nondiff;
    shared_ptr<YODA::Profile1D> _tp_chEF_minbias, _tp_chEF_hard, _tp_chEF_diff, _tp_chEF_nondiff;

    // Temp profiles containing <N(eta)>
    shared_ptr<YODA::Histo1D> _th_totN_minbias, _th_totN_hard, _th_totN_diff, _th_totN_nondiff;
    shared_ptr<YODA::Histo1D> _th_chN_minbias, _th_chN_hard, _th_chN_diff, _th_chN_nondiff;

    // Sums of weights (~ #events) in each event class
    double _mbSumW, _hdSumW, _dfSumW, _ndSumW;
    double _mbchSumW, _hdchSumW, _dfchSumW, _ndchSumW;

    //@}

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(LHCB_2013_I1208105);

}

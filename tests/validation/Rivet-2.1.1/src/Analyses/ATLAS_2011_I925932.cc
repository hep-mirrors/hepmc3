// -*- C++ -*-
// ATLAS W pT analysis

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/WFinder.hh"

namespace Rivet {


  namespace {
    /// @todo Correct this in Rivet
    inline double calcMT(const FourMomentum& a, const FourMomentum& b) {
      return sqrt(2.0 * a.pT() * b.pT() * (1.0 - cos(a.phi() - b.phi())) );
    }
  }


  class ATLAS_2011_I925932 : public Analysis {
  public:

    /// Constructor
    ATLAS_2011_I925932() : Analysis("ATLAS_2011_I925932") {  }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      FinalState fs;
      WFinder wfinder_dressed_el(fs, -2.4, 2.4, 20*GeV, PID::ELECTRON, 0*GeV, 1000*GeV, 25*GeV, 0.2, WFinder::CLUSTERNODECAY);
      addProjection(wfinder_dressed_el, "WFinder_dressed_el");
      WFinder wfinder_bare_el(fs, -2.4, 2.4, 20*GeV, PID::ELECTRON, 0*GeV, 1000*GeV, 25*GeV, 0.0, WFinder::NOCLUSTER);
      addProjection(wfinder_bare_el, "WFinder_bare_el");
      WFinder wfinder_dressed_mu(fs, -2.4, 2.4, 20*GeV, PID::MUON, 0*GeV, 1000*GeV, 25*GeV, 0.2, WFinder::CLUSTERNODECAY);
      addProjection(wfinder_dressed_mu, "WFinder_dressed_mu");
      WFinder wfinder_bare_mu(fs, -2.4, 2.4, 20*GeV, PID::MUON, 0*GeV, 1000*GeV, 25*GeV, 0.0, WFinder::NOCLUSTER);
      addProjection(wfinder_bare_mu, "WFinder_bare_mu");

      // Book histograms
      _hist_wpt_dressed_el  = bookHisto1D(1, 1, 1);
      _hist_wpt_bare_el     = bookHisto1D(1, 1, 2);
      _hist_wpt_dressed_mu  = bookHisto1D(2, 1, 1);
      _hist_wpt_bare_mu     = bookHisto1D(2, 1, 2);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const WFinder& wfinder_dressed_el = applyProjection<WFinder>(event, "WFinder_dressed_el");
      const WFinder& wfinder_bare_el    = applyProjection<WFinder>(event, "WFinder_bare_el");
      const WFinder& wfinder_dressed_mu = applyProjection<WFinder>(event, "WFinder_dressed_mu");
      const WFinder& wfinder_bare_mu    = applyProjection<WFinder>(event, "WFinder_bare_mu");

      if (wfinder_dressed_el.empty() && wfinder_bare_el.empty() &&
          wfinder_dressed_mu.empty() && wfinder_bare_mu.empty()) {
        MSG_DEBUG("No W bosons found");
        vetoEvent;
      }

      // "Dressed" electron
      if (!wfinder_dressed_el.particles().empty()) {
	    const FourMomentum& nu = wfinder_dressed_el.constituentNeutrinos()[0].momentum();
	    if (wfinder_dressed_el.mT() > 40*GeV && nu.pT() > 25*GeV) {
          _hist_wpt_dressed_el->fill(wfinder_dressed_el.bosons()[0].pT()/GeV, weight);
	    }
      }

      // "Bare" electron
      if (!wfinder_bare_el.particles().empty()) {
	    const FourMomentum& nu = wfinder_bare_el.constituentNeutrinos()[0].momentum();
	    if (wfinder_bare_el.mT() > 40*GeV && nu.pT() > 25*GeV) {
          _hist_wpt_bare_el->fill(wfinder_bare_el.bosons()[0].pT()/GeV, weight);
	    }
      }

      // "Dressed" muon
      if (!wfinder_dressed_mu.particles().empty()) {
	    const FourMomentum& nu = wfinder_dressed_mu.constituentNeutrinos()[0].momentum();
	    if (wfinder_dressed_mu.mT() > 40*GeV && nu.pT() > 25*GeV) {
          _hist_wpt_dressed_mu->fill(wfinder_dressed_mu.bosons()[0].pT()/GeV, weight);
	    }
      }

      // "Bare" muon
      if (!wfinder_bare_mu.particles().empty()) {
	    const FourMomentum& nu = wfinder_bare_mu.constituentNeutrinos()[0].momentum();
	    if (wfinder_bare_mu.mT() > 40*GeV && nu.pT() > 25*GeV) {
          _hist_wpt_bare_mu->fill(wfinder_bare_mu.bosons()[0].pT()/GeV, weight);
	    }
      }

    }


    // Normalize histos
    void finalize() {
      normalize(_hist_wpt_dressed_el);
      normalize(_hist_wpt_bare_el);
      normalize(_hist_wpt_dressed_mu);
      normalize(_hist_wpt_bare_mu);
    }

    //@}


  private:

	Histo1DPtr _hist_wpt_dressed_el;
	Histo1DPtr _hist_wpt_bare_el;
	Histo1DPtr _hist_wpt_dressed_mu;
	Histo1DPtr _hist_wpt_bare_mu;

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_I925932);

}

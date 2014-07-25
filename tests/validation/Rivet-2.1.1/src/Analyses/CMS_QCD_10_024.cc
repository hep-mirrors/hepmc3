// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Particle.hh"

namespace Rivet {


  class CMS_QCD_10_024 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CMS_QCD_10_024() : Analysis("CMS_QCD_10_024"),
		       _weight_pt05_eta08(0.), _weight_pt10_eta08(0.),
		       _weight_pt05_eta24(0.), _weight_pt10_eta24(0.) {  }


    void init() {
      addProjection(ChargedFinalState(-0.8, 0.8, 0.5*GeV), "CFS_08_05");
      addProjection(ChargedFinalState(-0.8, 0.8, 1.0*GeV), "CFS_08_10");
      addProjection(ChargedFinalState(-2.4, 2.4, 0.5*GeV), "CFS_24_05");
      addProjection(ChargedFinalState(-2.4, 2.4, 1.0*GeV), "CFS_24_10");

      size_t offset = 0;
      if (fuzzyEquals(sqrtS()/GeV, 7000, 1E-3)) offset = 0;
      if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) offset = 4;
      _hist_dNch_deta_pt05_eta08 = bookHisto1D(1+offset, 1, 1);
      _hist_dNch_deta_pt10_eta08 = bookHisto1D(2+offset, 1, 1);
      _hist_dNch_deta_pt05_eta24 = bookHisto1D(3+offset, 1, 1);
      _hist_dNch_deta_pt10_eta24 = bookHisto1D(4+offset, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      const ChargedFinalState& cfs_08_05 = applyProjection<ChargedFinalState>(event, "CFS_08_05");
      const ChargedFinalState& cfs_08_10 = applyProjection<ChargedFinalState>(event, "CFS_08_10");
      const ChargedFinalState& cfs_24_05 = applyProjection<ChargedFinalState>(event, "CFS_24_05");
      const ChargedFinalState& cfs_24_10 = applyProjection<ChargedFinalState>(event, "CFS_24_10");

      // Plot distributions
      if(!cfs_08_05.particles().empty()) _weight_pt05_eta08 += weight;
      if(!cfs_24_05.particles().empty()) _weight_pt05_eta24 += weight;
      foreach (const Particle& p, cfs_24_05.particles()) {
        _hist_dNch_deta_pt05_eta24->fill(p.momentum().pseudorapidity(), weight);
        if(!cfs_08_05.particles().empty())
	  _hist_dNch_deta_pt05_eta08->fill(p.momentum().pseudorapidity(), weight);
      }
      if(!cfs_08_10.particles().empty()) _weight_pt10_eta08 += weight;
      if(!cfs_24_10.particles().empty()) _weight_pt10_eta24 += weight;
      foreach (const Particle& p, cfs_24_10.particles()) {
        _hist_dNch_deta_pt10_eta24->fill(p.momentum().pseudorapidity(), weight);
	if(!cfs_08_10.particles().empty())
	  _hist_dNch_deta_pt10_eta08->fill(p.momentum().pseudorapidity(), weight);
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_hist_dNch_deta_pt05_eta08,1./_weight_pt05_eta08);
      scale(_hist_dNch_deta_pt10_eta08,1./_weight_pt10_eta08);
      scale(_hist_dNch_deta_pt05_eta24,1./_weight_pt05_eta24);
      scale(_hist_dNch_deta_pt10_eta24,1./_weight_pt10_eta24);
    }


  private:

    Histo1DPtr _hist_dNch_deta_pt05_eta08;
    Histo1DPtr _hist_dNch_deta_pt10_eta08;
    Histo1DPtr _hist_dNch_deta_pt05_eta24;
    Histo1DPtr _hist_dNch_deta_pt10_eta24;
    double _weight_pt05_eta08,_weight_pt10_eta08,_weight_pt05_eta24,_weight_pt10_eta24;
  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_QCD_10_024);

}

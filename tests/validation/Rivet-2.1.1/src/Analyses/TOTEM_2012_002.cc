// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// TOTEM elastic and total cross-section measurement
  class TOTEM_2012_002 : public Analysis {
  public:

    TOTEM_2012_002()
      : Analysis("TOTEM_2012_002")
    {    }


  public:

    void init() {
      addProjection(ChargedFinalState(), "CFS");
      _hist_tlow  = bookHisto1D(1, 1, 1);
      _hist_thigh = bookHisto1D(2, 1, 1);
      _hist_sigma = bookHisto1D(3, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");
      if (cfs.size() > 2) MSG_WARNING("Final state includes more than two charged particles !");
      _hist_sigma->fill(sqrtS()/GeV, weight);

      foreach (const Particle& p, cfs.particles()) {
        if (p.eta() > 0. && p.pdgId() == PID::PROTON) {
          double t = sqr(p.pT());
          _hist_tlow->fill(t, weight);
          _hist_thigh->fill(t, weight);
        }
      }
    }


    void finalize() {
      normalize(_hist_tlow, crossSection()/millibarn);
      normalize(_hist_thigh, crossSection()/millibarn);
      normalize(_hist_sigma, crossSection()/millibarn);
    }


  private:

    Histo1DPtr _hist_tlow;
    Histo1DPtr _hist_thigh;
    Histo1DPtr _hist_sigma;

  };


  DECLARE_RIVET_PLUGIN(TOTEM_2012_002);

}

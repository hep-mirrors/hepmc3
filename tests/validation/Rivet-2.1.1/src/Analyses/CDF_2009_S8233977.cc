// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerCDFRun2.hh"

namespace Rivet {


  /// @brief CDF Run II min-bias cross-section
  /// @author Hendrik Hoeth
  ///
  /// Measurement of \f$ \langle p_T \rangle \f$ vs. \f$ n_\text{ch} \f$,
  /// the track \f$ p_T \f$ distribution, and the \f$ \sum E_T \f$ distribution.
  /// Particles are selected within |eta|<1 and with pT>0.4 GeV.
  /// There is no pT cut for the \f$ \sum E_T \f$ measurement.
  ///
  /// @par Run conditions
  ///
  /// @arg \f$ \sqrt{s} = \f$ 1960 GeV
  /// @arg Run with generic QCD events.
  /// @arg Set particles with c*tau > 10 mm stable
  class CDF_2009_S8233977 : public Analysis {
  public:

    /// Constructor
    CDF_2009_S8233977()
      : Analysis("CDF_2009_S8233977"),
        _sumWeightSelected(0.0)
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms and projections
    void init() {
      addProjection(TriggerCDFRun2(), "Trigger");
      addProjection(FinalState(-1.0, 1.0, 0.0*GeV), "EtFS");
      addProjection(ChargedFinalState(-1.0, 1.0, 0.4*GeV), "CFS");

      _hist_pt = bookHisto1D(1, 1, 1);
      _hist_pt_vs_multiplicity = bookProfile1D(2, 1, 1);
      _hist_sumEt = bookHisto1D(3, 1, 1);
    }



    /// Do the analysis
    void analyze(const Event& evt) {
      // MinBias Trigger
      const bool trigger = applyProjection<TriggerCDFRun2>(evt, "Trigger").minBiasDecision();
      if (!trigger) vetoEvent;

      // Get the event weight
      const double weight = evt.weight();

      /// @todo The pT and sum(ET) distributions look slightly different from
      ///       Niccolo's Monte Carlo plots. Still waiting for his answer.

      const ChargedFinalState& trackfs = applyProjection<ChargedFinalState>(evt, "CFS");
      const size_t numParticles = trackfs.size();
      foreach (const Particle& p, trackfs.particles()) {
        const double pT = p.pT() / GeV;
        _hist_pt_vs_multiplicity->fill(numParticles, pT, weight);

        // The weight for entries in the pT distribution should be weight/(pT*dPhi*dy).
        //
        // - dPhi = 2*PI
        //
        // - dy depends on the pT: They calculate y assuming the particle has the
        //   pion mass and assuming that eta=1:
        //   dy = 2 * 1/2 * ln [(sqrt(m^2 + (a+1)*pT^2) + a*pT) / (sqrt(m^2 + (a+1)*pT^2) - a*pT)]
        //   with a = sinh(1).
        //
        // sinh(1) = 1.1752012
        // m(charged pion)^2 = (139.57 MeV)^2 = 0.019479785 GeV^2
        const double sinh1 = 1.1752012;
        const double apT  = sinh1 * pT;
        const double mPi = 139.57*MeV;
        const double root = sqrt(mPi*mPi + (1+sinh1)*pT*pT);
        const double dy = std::log((root+apT)/(root-apT));
        const double dphi = TWOPI;
        _hist_pt->fill(pT, weight/(pT*dphi*dy));
      }

      // Calc sum(Et) from calo particles
      const FinalState& etfs = applyProjection<FinalState>(evt, "EtFS");
      double sumEt = 0.0;
      foreach (const Particle& p, etfs.particles()) {
        sumEt += p.momentum().Et();
      }
      _hist_sumEt->fill(sumEt, weight);
      _sumWeightSelected += evt.weight();
    }



    /// Normalize histos
    void finalize() {
      scale(_hist_sumEt, crossSection()/millibarn/(4*M_PI*_sumWeightSelected));
      scale(_hist_pt, crossSection()/millibarn/_sumWeightSelected);
      MSG_DEBUG("sumOfWeights()     = " << sumOfWeights());
      MSG_DEBUG("_sumWeightSelected = " << _sumWeightSelected);
    }

    //@}


  private:

    double _sumWeightSelected;
    Profile1DPtr _hist_pt_vs_multiplicity;
    Histo1DPtr _hist_pt;
    Histo1DPtr _hist_sumEt;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2009_S8233977);

}

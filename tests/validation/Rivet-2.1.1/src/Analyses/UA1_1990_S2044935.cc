// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"

namespace Rivet {


  /// @brief UA1 minbias track multiplicities, \f$ p_\perp \f$ and \f$ E_\perp \f$
  class UA1_1990_S2044935 : public Analysis {
  public:

    /// Constructor
    UA1_1990_S2044935() : Analysis("UA1_1990_S2044935") {
      _sumwTrig = 0;
      _sumwTrig08 = 0;
      _sumwTrig40 = 0;
      _sumwTrig80 = 0;
    }


    /// @name Analysis methods
    //@{

    /// Book projections and histograms
    void init() {
      addProjection(ChargedFinalState(-5.5, 5.5), "TriggerFS");
      addProjection(ChargedFinalState(-2.5, 2.5), "TrackFS");
      const FinalState trkcalofs(-2.5, 2.5);
      addProjection(MissingMomentum(trkcalofs), "MET25");
      const FinalState calofs(-6.0, 6.0);
      addProjection(MissingMomentum(calofs), "MET60");

      if (fuzzyEquals(sqrtS()/GeV, 63)) {
        _hist_Pt = bookProfile1D(8,1,1);
      } else if (fuzzyEquals(sqrtS()/GeV, 200)) {
        _hist_Nch = bookHisto1D(1,1,1);
        _hist_Esigd3p = bookHisto1D(2,1,1);
        _hist_Pt = bookProfile1D(6,1,1);
        _hist_Et = bookHisto1D(9,1,1);
        _hist_Etavg = bookProfile1D(12,1,1);
      } else if (fuzzyEquals(sqrtS()/GeV, 500)) {
        _hist_Nch = bookHisto1D(1,1,2);
        _hist_Esigd3p = bookHisto1D(2,1,2);
        _hist_Et = bookHisto1D(10,1,1);
        _hist_Etavg = bookProfile1D(12,1,2);
      } else if (fuzzyEquals(sqrtS()/GeV, 900)) {
        _hist_Nch = bookHisto1D(1,1,3);
        _hist_Esigd3p = bookHisto1D(2,1,3);
        _hist_Pt = bookProfile1D(7,1,1);
        _hist_Et = bookHisto1D(11,1,1);
        _hist_Etavg = bookProfile1D(12,1,3);
        _hist_Esigd3p08 = bookHisto1D(3,1,1);
        _hist_Esigd3p40 = bookHisto1D(4,1,1);
        _hist_Esigd3p80 = bookHisto1D(5,1,1);
      }

    }


    void analyze(const Event& event) {
      // Trigger
      const FinalState& trigfs = applyProjection<FinalState>(event, "TriggerFS");
      unsigned int n_minus(0), n_plus(0);
      foreach (const Particle& p, trigfs.particles()) {
        const double eta = p.eta();
        if (inRange(eta, -5.5, -1.5)) n_minus++;
        else if (inRange(eta, 1.5, 5.5)) n_plus++;
      }
      MSG_DEBUG("Trigger -: " << n_minus << ", Trigger +: " << n_plus);
      if (n_plus == 0 || n_minus == 0) vetoEvent;
      const double weight = event.weight();
      _sumwTrig += weight;

      // Use good central detector tracks
      const FinalState& cfs = applyProjection<FinalState>(event, "TrackFS");
      const double Et25 = applyProjection<MissingMomentum>(event, "MET25").scalarEt();
      const double Et60 = applyProjection<MissingMomentum>(event, "MET60").scalarEt();
      const unsigned int nch = cfs.size();

      // Event level histos
      if (!fuzzyEquals(sqrtS()/GeV, 63, 1E-3)) {
        _hist_Nch->fill(nch, weight);
        _hist_Et->fill(Et60/GeV, weight);
        _hist_Etavg->fill(nch, Et25/GeV, weight);
      }

      // Particle/track level histos
      const double deta = 2 * 5.0;
      const double dphi = TWOPI;
      const double dnch_deta = nch/deta;
      foreach (const Particle& p, cfs.particles()) {
        const double pt = p.pT();
        const double scaled_weight = weight/(deta*dphi*pt/GeV);
        if (!fuzzyEquals(sqrtS()/GeV, 500, 1E-3)) {
          _hist_Pt->fill(nch, pt/GeV, weight);
        }
        if (!fuzzyEquals(sqrtS()/GeV, 63, 1E-3)) {
          _hist_Esigd3p->fill(pt/GeV, scaled_weight);
        }
        // Also fill for specific dn/deta ranges at 900 GeV
        if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) {
          if (inRange(dnch_deta, 0.8, 4.0)) {
            _sumwTrig08 += weight;
            _hist_Esigd3p08->fill(pt/GeV, scaled_weight);
          } else if (inRange(dnch_deta, 4.0, 8.0)) {
            _sumwTrig40 += weight;
            _hist_Esigd3p40->fill(pt/GeV, scaled_weight);
          } else {
            //MSG_WARNING(dnch_deta);
            if (dnch_deta > 8.0) {
              _sumwTrig80 += weight;
              _hist_Esigd3p80->fill(pt/GeV, scaled_weight);
            }
          }
        }
      }

    }


    void finalize() {
      if (_sumwTrig <= 0) {
        MSG_WARNING("No events passed the trigger!");
        return;
      }
      const double xsec = crossSectionPerEvent();
      if (!fuzzyEquals(sqrtS()/GeV, 63, 1E-3)) {
        scale(_hist_Nch, 2*xsec/millibarn); //< Factor of 2 for Nch bin widths?
        scale(_hist_Esigd3p, xsec/millibarn);
        scale(_hist_Et, xsec/millibarn);
      }
      if (fuzzyEquals(sqrtS()/GeV, 900, 1E-3)) {
        // NB. Ref data is normalised to a fixed value not reproducible from MC.
        const double scale08 =  (_hist_Esigd3p08->bin(0).area() > 0) ?
          0.933e5/_hist_Esigd3p08->bin(0).height() : 0;
        scale(_hist_Esigd3p08, scale08);
        const double scale40 = (_hist_Esigd3p40->bin(0).area() > 0) ?
          1.369e5/_hist_Esigd3p40->bin(0).height() : 0;
        scale(_hist_Esigd3p40, scale40);
        const double scale80 = (_hist_Esigd3p80->bin(0).area() > 0) ?
          1.657e5/_hist_Esigd3p80->bin(0).height() : 0;
        scale(_hist_Esigd3p80, scale80);
      }
    }

    //@}


  private:

    /// @name Weight counters
    //@{
    double _sumwTrig, _sumwTrig08, _sumwTrig40, _sumwTrig80;
    //@}

    /// @name Histogram collections
    //@{
    Histo1DPtr _hist_Nch;
    Histo1DPtr _hist_Esigd3p;
    Histo1DPtr _hist_Esigd3p08;
    Histo1DPtr _hist_Esigd3p40;
    Histo1DPtr _hist_Esigd3p80;
    Profile1DPtr _hist_Pt;
    Profile1DPtr _hist_Etavg;
    Histo1DPtr _hist_Et;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(UA1_1990_S2044935);

}

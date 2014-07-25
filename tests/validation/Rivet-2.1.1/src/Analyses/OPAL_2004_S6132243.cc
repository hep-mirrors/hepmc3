// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ParisiTensor.hh"
#include "Rivet/Projections/Hemispheres.hh"
#include <cmath>

namespace Rivet {


  /// @brief OPAL event shapes and moments at 91, 133, 177, and 197 GeV
  /// @author Andy Buckley
  class OPAL_2004_S6132243 : public Analysis {
  public:

    /// Constructor
    OPAL_2004_S6132243()
      : Analysis("OPAL_2004_S6132243"),
        _isqrts(-1), _sumWTrack2(0.0), _sumWJet3(0.0)
    {
      //
    }


    /// @name Analysis methods
    //@{

    /// Energies: 91, 133, 177 (161-183), 197 (189-209) => index 0..4
    int getHistIndex(double sqrts) {
      int ih = -1;
      if (inRange(sqrts/GeV, 89.9, 91.5)) {
        ih = 0;
      } else if (fuzzyEquals(sqrts/GeV, 133)) {
        ih = 1;
      } else if (fuzzyEquals(sqrts/GeV, 177)) { // (161-183)
        ih = 2;
      } else if (fuzzyEquals(sqrts/GeV, 197)) { // (189-209)
        ih = 3;
      } else {
        stringstream ss;
        ss << "Invalid energy for OPAL_2004 analysis: "
           << sqrts/GeV << " GeV != 91, 133, 177, or 197 GeV";
        throw Error(ss.str());
      }
      assert(ih >= 0);
      return ih;
    }


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      const FinalState fs;
      addProjection(fs, "FS");
      const ChargedFinalState cfs;
      addProjection(cfs, "CFS");
      addProjection(FastJets(fs, FastJets::DURHAM, 0.7), "DurhamJets");
      addProjection(Sphericity(fs), "Sphericity");
      addProjection(ParisiTensor(fs), "Parisi");
      const Thrust thrust(fs);
      addProjection(thrust, "Thrust");
      addProjection(Hemispheres(thrust), "Hemispheres");

      // Get beam energy index
      _isqrts = getHistIndex(sqrtS());

      // Book histograms
      _hist1MinusT[_isqrts]    = bookHisto1D(1, 1, _isqrts+1);
      _histHemiMassH[_isqrts]  = bookHisto1D(2, 1, _isqrts+1);
      _histCParam[_isqrts]     = bookHisto1D(3, 1, _isqrts+1);
      _histHemiBroadT[_isqrts] = bookHisto1D(4, 1, _isqrts+1);
      _histHemiBroadW[_isqrts] = bookHisto1D(5, 1, _isqrts+1);
      _histY23Durham[_isqrts]  = bookHisto1D(6, 1, _isqrts+1);
      _histTMajor[_isqrts]     = bookHisto1D(7, 1, _isqrts+1);
      _histTMinor[_isqrts]     = bookHisto1D(8, 1, _isqrts+1);
      _histAplanarity[_isqrts] = bookHisto1D(9, 1, _isqrts+1);
      _histSphericity[_isqrts] = bookHisto1D(10, 1, _isqrts+1);
      _histOblateness[_isqrts] = bookHisto1D(11, 1, _isqrts+1);
      _histHemiMassL[_isqrts]  = bookHisto1D(12, 1, _isqrts+1);
      _histHemiBroadN[_isqrts] = bookHisto1D(13, 1, _isqrts+1);
      _histDParam[_isqrts]     = bookHisto1D(14, 1, _isqrts+1);
      //
      _hist1MinusTMom[_isqrts]    = bookHisto1D(15, 1, _isqrts+1);
      _histHemiMassHMom[_isqrts]  = bookHisto1D(16, 1, _isqrts+1);
      _histCParamMom[_isqrts]     = bookHisto1D(17, 1, _isqrts+1);
      _histHemiBroadTMom[_isqrts] = bookHisto1D(18, 1, _isqrts+1);
      _histHemiBroadWMom[_isqrts] = bookHisto1D(19, 1, _isqrts+1);
      _histY23DurhamMom[_isqrts]  = bookHisto1D(20, 1, _isqrts+1);
      _histTMajorMom[_isqrts]     = bookHisto1D(21, 1, _isqrts+1);
      _histTMinorMom[_isqrts]     = bookHisto1D(22, 1, _isqrts+1);
      _histSphericityMom[_isqrts] = bookHisto1D(23, 1, _isqrts+1);
      _histOblatenessMom[_isqrts] = bookHisto1D(24, 1, _isqrts+1);
      _histHemiMassLMom[_isqrts]  = bookHisto1D(25, 1, _isqrts+1);
      _histHemiBroadNMom[_isqrts] = bookHisto1D(26, 1, _isqrts+1);
    }


    void analyze(const Event& event) {
      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      const FinalState& cfs = applyProjection<FinalState>(event, "CFS");
      if (cfs.size() < 2) vetoEvent;

      // Increment passed-cuts weight sum
      const double weight = event.weight();
      _sumWTrack2 += weight;

      // Thrusts
      const Thrust& thrust = applyProjection<Thrust>(event, "Thrust");
      _hist1MinusT[_isqrts]->fill(1-thrust.thrust(), weight);
      _histTMajor[_isqrts]->fill(thrust.thrustMajor(), weight);
      _histTMinor[_isqrts]->fill(thrust.thrustMinor(), weight);
      _histOblateness[_isqrts]->fill(thrust.oblateness(), weight);
      for (int n = 1; n <= 5; ++n) {
        _hist1MinusTMom[_isqrts]->fill(n, pow(1-thrust.thrust(), n)*weight);
        _histTMajorMom[_isqrts]->fill(n, pow(thrust.thrustMajor(), n)*weight);
        _histTMinorMom[_isqrts]->fill(n, pow(thrust.thrustMinor(), n)*weight);
        _histOblatenessMom[_isqrts]->fill(n, pow(thrust.oblateness(), n)*weight);
      }

      // Jets
      const FastJets& durjet = applyProjection<FastJets>(event, "DurhamJets");
      if (durjet.clusterSeq()) {
        _sumWJet3 += weight;
        const double y23 = durjet.clusterSeq()->exclusive_ymerge_max(2);
        if (y23>0.0) {
          _histY23Durham[_isqrts]->fill(y23, weight);
          for (int n = 1; n <= 5; ++n) {
            _histY23DurhamMom[_isqrts]->fill(n, pow(y23, n)*weight);
          }
        }
      }

      // Sphericities
      const Sphericity& sphericity = applyProjection<Sphericity>(event, "Sphericity");
      const double sph = sphericity.sphericity();
      const double apl = sphericity.aplanarity();
      _histSphericity[_isqrts]->fill(sph, weight);
      _histAplanarity[_isqrts]->fill(apl, weight);
      for (int n = 1; n <= 5; ++n) {
        _histSphericityMom[_isqrts]->fill(n, pow(sph, n)*weight);
      }

      // C & D params
      const ParisiTensor& parisi = applyProjection<ParisiTensor>(event, "Parisi");
      const double cparam = parisi.C();
      const double dparam = parisi.D();
      _histCParam[_isqrts]->fill(cparam, weight);
      _histDParam[_isqrts]->fill(dparam, weight);
      for (int n = 1; n <= 5; ++n) {
        _histCParamMom[_isqrts]->fill(n, pow(cparam, n)*weight);
      }

      // Hemispheres
      const Hemispheres& hemi = applyProjection<Hemispheres>(event, "Hemispheres");
      // The paper says that M_H/L are scaled by sqrt(s), but scaling by E_vis is the way that fits the data...
      const double hemi_mh = hemi.scaledMhigh();
      const double hemi_ml = hemi.scaledMlow();
      /// @todo This shouldn't be necessary... what's going on? Memory corruption suspected :(
      // if (std::isnan(hemi_ml)) {
      //   MSG_ERROR("NaN in HemiL! Event = " << numEvents());
      //   MSG_ERROR(hemi.M2low() << ", " << hemi.E2vis());
      // }
      if (!std::isnan(hemi_mh) && !std::isnan(hemi_ml)) {
        const double hemi_bmax = hemi.Bmax();
        const double hemi_bmin = hemi.Bmin();
        const double hemi_bsum = hemi.Bsum();
        _histHemiMassH[_isqrts]->fill(hemi_mh, weight);
        _histHemiMassL[_isqrts]->fill(hemi_ml, weight);
        _histHemiBroadW[_isqrts]->fill(hemi_bmax, weight);
        _histHemiBroadN[_isqrts]->fill(hemi_bmin, weight);
        _histHemiBroadT[_isqrts]->fill(hemi_bsum, weight);
        for (int n = 1; n <= 5; ++n) {
          // if (std::isnan(pow(hemi_ml, n))) MSG_ERROR("NaN in HemiL moment! Event = " << numEvents());
          _histHemiMassHMom[_isqrts]->fill(n, pow(hemi_mh, n)*weight);
          _histHemiMassLMom[_isqrts]->fill(n, pow(hemi_ml, n)*weight);
          _histHemiBroadWMom[_isqrts]->fill(n, pow(hemi_bmax, n)*weight);
          _histHemiBroadNMom[_isqrts]->fill(n, pow(hemi_bmin, n)*weight);
          _histHemiBroadTMom[_isqrts]->fill(n, pow(hemi_bsum, n)*weight);
        }
      }
    }


    void finalize() {
      scale(_hist1MinusT[_isqrts], 1.0/_sumWTrack2);
      scale(_histTMajor[_isqrts], 1.0/_sumWTrack2);
      scale(_histTMinor[_isqrts], 1.0/_sumWTrack2);
      scale(_histOblateness[_isqrts], 1.0/_sumWTrack2);
      scale(_histSphericity[_isqrts], 1.0/_sumWTrack2);
      scale(_histAplanarity[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiMassH[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiMassL[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadW[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadN[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadT[_isqrts], 1.0/_sumWTrack2);
      scale(_histCParam[_isqrts], 1.0/_sumWTrack2);
      scale(_histDParam[_isqrts], 1.0/_sumWTrack2);
      scale(_histY23Durham[_isqrts], 1.0/_sumWJet3);
      //
      scale(_hist1MinusTMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histTMajorMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histTMinorMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histOblatenessMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histSphericityMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiMassHMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiMassLMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadWMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadNMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histHemiBroadTMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histCParamMom[_isqrts], 1.0/_sumWTrack2);
      scale(_histY23DurhamMom[_isqrts], 1.0/_sumWJet3);
    }

    //@}


  private:

    /// Beam energy index for histograms
    int _isqrts;

    /// @name Counters of event weights passing the cuts
    //@{
    double _sumWTrack2, _sumWJet3;
    //@}

    /// @name Event shape histos at 4 energies
    //@{
    Histo1DPtr _hist1MinusT[4];
    Histo1DPtr _histHemiMassH[4];
    Histo1DPtr _histCParam[4];
    Histo1DPtr _histHemiBroadT[4];
    Histo1DPtr _histHemiBroadW[4];
    Histo1DPtr _histY23Durham[4];
    Histo1DPtr _histTMajor[4];
    Histo1DPtr _histTMinor[4];
    Histo1DPtr _histAplanarity[4];
    Histo1DPtr _histSphericity[4];
    Histo1DPtr _histOblateness[4];
    Histo1DPtr _histHemiMassL[4];
    Histo1DPtr _histHemiBroadN[4];
    Histo1DPtr _histDParam[4];
    //@}

    /// @name Event shape moment histos at 4 energies
    //@{
    Histo1DPtr _hist1MinusTMom[4];
    Histo1DPtr _histHemiMassHMom[4];
    Histo1DPtr _histCParamMom[4];
    Histo1DPtr _histHemiBroadTMom[4];
    Histo1DPtr _histHemiBroadWMom[4];
    Histo1DPtr _histY23DurhamMom[4];
    Histo1DPtr _histTMajorMom[4];
    Histo1DPtr _histTMinorMom[4];
    Histo1DPtr _histSphericityMom[4];
    Histo1DPtr _histOblatenessMom[4];
    Histo1DPtr _histHemiMassLMom[4];
    Histo1DPtr _histHemiBroadNMom[4];
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_2004_S6132243);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  namespace {

    /// @name Jet angle calculator functions
    //@{
    /// @todo Move to utils?

    /// @todo Use Jet or FourMomentum interface rather than PseudoJet
    /// @todo Move to utils?
    double calc_BZ(const vector<fastjet::PseudoJet>& jets) {
      assert(jets.size() == 4);
      Vector3 p12 = cross( momentum3(jets[0]), momentum3(jets[1]));
      Vector3 p34 = cross( momentum3(jets[2]), momentum3(jets[3]));
      return dot(p12,p34) / (p12.mod()*p34.mod());
    }


    /// @todo Use Jet or FourMomentum interface rather than PseudoJet
    /// @todo Move to utils?
    double calc_KSW(const vector<fastjet::PseudoJet>& jets) {
      assert(jets.size() == 4);
      Vector3 p13 = cross( momentum3(jets[0]), momentum3(jets[2]));
      Vector3 p24 = cross( momentum3(jets[1]), momentum3(jets[3]));
      Vector3 p14 = cross( momentum3(jets[0]), momentum3(jets[3]));
      Vector3 p23 = cross( momentum3(jets[1]), momentum3(jets[2]));
      return cos (0.5*( acos (dot(p14,p23) / (p14.mod()*p23.mod())) +
                        acos (dot(p13,p24) / (p13.mod()*p24.mod())) ));
    }


    /// @todo Use Jet or FourMomentum interface rather than PseudoJet
    /// @todo Move to utils?
    double calc_NR(const vector<fastjet::PseudoJet>& jets) {
      assert(jets.size() == 4);
      Vector3 p12 = momentum3(jets[0]) - momentum3(jets[1]);
      Vector3 p34 = momentum3(jets[2]) - momentum3(jets[3]);
      return dot(p12,p34) / (p12.mod()*p34.mod());
    }

    /// @todo Use Jet or FourMomentum interface rather than PseudoJet
    /// @todo Move to utils?
    double calc_ALPHA34(const vector<fastjet::PseudoJet>& jets) {
      assert(jets.size() == 4);
      Vector3 p3 = momentum3(jets[2]);
      Vector3 p4 = momentum3(jets[3]);
      return dot(p3,p4) / (p3.mod()*p4.mod());
    }

    //@}

  }


  /**
   * @brief DELPHI 4-jet angular distributions
   * @author Hendrik Hoeth
   *
   * This is Hendrik Hoeth's Diploma thesis, measuring the 4-jet angular
   * distributions at LEP-1.
   *
   * @par Run conditions
   *
   * @arg LEP1 beam energy: \f$ \sqrt{s} = \f$ 91.2 GeV
   * @arg Run with generic QCD events.
   * @arg No \f$ p_\perp^\text{min} \f$ cutoff is required
   */
  class DELPHI_2003_WUD_03_11 : public Analysis {
  public:

    /// Constructor
    DELPHI_2003_WUD_03_11()
      : Analysis("DELPHI_2003_WUD_03_11")
    {
      _numdurjets = 0;
      _numjadejets = 0;
    }


    /// @name Analysis methods
    //@{

    void init() {
      const ChargedFinalState cfs;
      addProjection(cfs, "FS");
      addProjection(FastJets(cfs, FastJets::JADE, 0.7), "JadeJets");
      addProjection(FastJets(cfs, FastJets::DURHAM, 0.7), "DurhamJets");

      _histDurhamBZ      = bookHisto1D(1, 1, 1);
      _histDurhamKSW     = bookHisto1D(2, 1, 1);
      _histDurhamNR      = bookHisto1D(3, 1, 1);
      _histDurhamALPHA34 = bookHisto1D(4, 1, 1);
      _histJadeBZ        = bookHisto1D(1, 2, 1);
      _histJadeKSW       = bookHisto1D(2, 2, 1);
      _histJadeNR        = bookHisto1D(3, 2, 1);
      _histJadeALPHA34   = bookHisto1D(4, 2, 1);
    }


    void analyze(const Event& e) {
      // First, veto on leptonic events by requiring at least 4 charged FS particles
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      const size_t numParticles = fs.particles().size();

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      if (numParticles < 2) {
        MSG_DEBUG("Failed multiplicity cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed multiplicity cut");

      // Get event weight for histo filling
      const double weight = e.weight();

      // Jets
      const FastJets& durjet = applyProjection<FastJets>(e, "DurhamJets");
      vector<fastjet::PseudoJet> jets_durham;
      if (durjet.clusterSeq()) {
        jets_durham = fastjet::sorted_by_E(durjet.clusterSeq()->exclusive_jets_ycut(0.008));
        if (jets_durham.size() == 4) {
          _histDurhamBZ->fill(fabs(calc_BZ(jets_durham)), weight);
          _histDurhamKSW->fill(calc_KSW(jets_durham), weight);
          _histDurhamNR->fill(fabs(calc_NR(jets_durham)), weight);
          _histDurhamALPHA34->fill(calc_ALPHA34(jets_durham), weight);
        }
        if (durjet.clusterSeq()->exclusive_ymerge_max(3) > 0.008 &&
            durjet.clusterSeq()->exclusive_ymerge_max(4) < 0.008) {
          _numdurjets++;
        }
      }

      const FastJets& jadejet = applyProjection<FastJets>(e, "JadeJets");
      vector<fastjet::PseudoJet> jets_jade;
      if (jadejet.clusterSeq()) {
        jets_jade = fastjet::sorted_by_E(jadejet.clusterSeq()->exclusive_jets_ycut(0.015));
        if (jets_jade.size() == 4) {
          _histJadeBZ->fill(fabs(calc_BZ(jets_jade)), weight);
          _histJadeKSW->fill(calc_KSW(jets_jade), weight);
          _histJadeNR->fill(fabs(calc_NR(jets_jade)), weight);
          _histJadeALPHA34->fill(calc_ALPHA34(jets_jade), weight);
        }
        if (jadejet.clusterSeq()->exclusive_ymerge_max(3) > 0.015 &&
            jadejet.clusterSeq()->exclusive_ymerge_max(4) < 0.015) {
          _numjadejets++;
        }
      }

    }


    // Finalize
    void finalize() {
      // Normalize inclusive single particle distributions to the average number
      // of charged particles per event.

      MSG_INFO("Number of Durham jets = " << _numdurjets);
      MSG_INFO("Number of Jade jets   = " << _numjadejets);

      /// @todo Scale rather than normalize?
      normalize(_histDurhamBZ      , 0.0785);
      normalize(_histDurhamKSW     , 0.0785);
      normalize(_histDurhamNR      , 0.0785);
      normalize(_histDurhamALPHA34 , 0.0785);
      normalize(_histJadeBZ        , 0.0277);
      normalize(_histJadeKSW       , 0.0277);
      normalize(_histJadeNR        , 0.0277);
      normalize(_histJadeALPHA34   , 0.0277);
    }

    //@}


  private:

    int _numdurjets;
    int _numjadejets;

    /// @name Histograms
    //@{
    Histo1DPtr _histDurhamBZ;
    Histo1DPtr _histDurhamKSW;
    Histo1DPtr _histDurhamNR;
    Histo1DPtr _histDurhamALPHA34;
    Histo1DPtr _histJadeBZ;
    Histo1DPtr _histJadeKSW;
    Histo1DPtr _histJadeNR;
    Histo1DPtr _histJadeALPHA34;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(DELPHI_2003_WUD_03_11);

}

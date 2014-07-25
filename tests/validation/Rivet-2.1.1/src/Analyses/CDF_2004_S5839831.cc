// -*- C++ -*-
// "Acosta" underlying event analysis at CDF, inc. "Swiss Cheese"

#include "Rivet/Analysis.hh"
#include "Rivet/Jet.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/TriggerCDFRun0Run1.hh"

namespace Rivet {


  /// @brief CDF calo jet underlying event analysis at 630 and 1800 GeV
  ///
  /// CDF measurement of underlying event using calorimeter jet scales and
  /// alignment, particle flow activity in transverse cones, and the Swiss
  /// Cheese analysis method, where cones are excluded around the 2 and 3
  /// hardest jets.
  ///
  /// @author Andy Buckley
  class CDF_2004_S5839831 : public Analysis {
  public:

    /// Constructor: cuts on charged final state are \f$ -1 < \eta < 1 \f$
    /// and \f$ p_T > 0.4 \f$ GeV.
    CDF_2004_S5839831()
      : Analysis("CDF_2004_S5839831")
    {
    }


  private:


    /// @cond CONEUE_DETAIL

    struct ConesInfo {
      ConesInfo() : numMax(0), numMin(0), ptMax(0), ptMin(0), ptDiff(0) {}
      unsigned int numMax, numMin;
      double ptMax, ptMin, ptDiff;
    };

    /// @endcond

    ConesInfo _calcTransCones(const double etaLead, const double phiLead,
                              const Particles& tracks) {
      const double phiTransPlus = mapAngle0To2Pi(phiLead + PI/2.0);
      const double phiTransMinus = mapAngle0To2Pi(phiLead - PI/2.0);
      MSG_DEBUG("phi_lead = " << phiLead
               << " -> trans = (" << phiTransPlus
               << ", " << phiTransMinus << ")");

      unsigned int numPlus(0), numMinus(0);
      double ptPlus(0), ptMinus(0);
      // Run over all charged tracks
      foreach (const Particle& t, tracks) {
        FourMomentum trackMom = t.momentum();
        const double pt = trackMom.pT();

        // Find if track mom is in either transverse cone
        if (deltaR(trackMom, etaLead, phiTransPlus) < 0.7) {
          ptPlus += pt;
          numPlus += 1;
        } else if (deltaR(trackMom, etaLead, phiTransMinus) < 0.7) {
          ptMinus += pt;
          numMinus += 1;
        }
      }

      ConesInfo rtn;
      // Assign N_{min,max} from N_{plus,minus}
      rtn.numMax = (ptPlus >= ptMinus) ? numPlus : numMinus;
      rtn.numMin = (ptPlus >= ptMinus) ? numMinus : numPlus;
      // Assign pT_{min,max} from pT_{plus,minus}
      rtn.ptMax = (ptPlus >= ptMinus) ? ptPlus : ptMinus;
      rtn.ptMin = (ptPlus >= ptMinus) ? ptMinus : ptPlus;
      rtn.ptDiff = fabs(rtn.ptMax - rtn.ptMin);

      MSG_DEBUG("Min cone has " << rtn.numMin << " tracks -> "
               << "pT_min = " << rtn.ptMin/GeV << " GeV");
      MSG_DEBUG("Max cone has " << rtn.numMax << " tracks -> "
               << "pT_max = " << rtn.ptMax/GeV << " GeV");

      return rtn;
    }


    ConesInfo _calcTransCones(const FourMomentum& leadvec,
                              const Particles& tracks) {
      const double etaLead = leadvec.pseudorapidity();
      const double phiLead = leadvec.azimuthalAngle();
      return _calcTransCones(etaLead, phiLead, tracks);
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      addProjection(TriggerCDFRun0Run1(), "Trigger");
      addProjection(Beam(), "Beam");
      const FinalState calofs(-1.2, 1.2);
      addProjection(calofs, "CaloFS");
      addProjection(FastJets(calofs, FastJets::CDFJETCLU, 0.7), "Jets");
      const ChargedFinalState trackfs(-1.2, 1.2, 0.4*GeV);
      addProjection(trackfs, "TrackFS");
      // Restrict tracks to |eta| < 0.7 for the min bias part.
      const ChargedFinalState mbfs(-0.7, 0.7, 0.4*GeV);
      addProjection(mbfs, "MBFS");
      // Restrict tracks to |eta| < 1 for the Swiss-Cheese part.
      const ChargedFinalState cheesefs(-1.0, 1.0, 0.4*GeV);
      addProjection(cheesefs, "CheeseFS");
      addProjection(FastJets(cheesefs, FastJets::CDFJETCLU, 0.7), "CheeseJets");

      // Book histograms
      if (fuzzyEquals(sqrtS()/GeV, 1800, 1E-3)) {
        _pt90MaxAvg1800 = bookProfile1D(1, 1, 1);
        _pt90MinAvg1800 = bookProfile1D(1, 1, 2);
        _pt90Max1800 = bookProfile1D(2, 1, 1);
        _pt90Min1800 = bookProfile1D(2, 1, 2);
        _pt90Diff1800 = bookProfile1D(2, 1, 3);
        _num90Max1800 = bookProfile1D(4, 1, 1);
        _num90Min1800 = bookProfile1D(4, 1, 2);
        _pTSum1800_2Jet = bookProfile1D(7, 1, 1);
        _pTSum1800_3Jet = bookProfile1D(7, 1, 2);

        _pt90Dbn1800Et40 = bookHisto1D(3, 1, 1);
        _pt90Dbn1800Et80 = bookHisto1D(3, 1, 2);
        _pt90Dbn1800Et120 = bookHisto1D(3, 1, 3);
        _pt90Dbn1800Et160 = bookHisto1D(3, 1, 4);
        _pt90Dbn1800Et200 = bookHisto1D(3, 1, 5);
        _numTracksDbn1800MB = bookHisto1D(5, 1, 1);
        _ptDbn1800MB = bookHisto1D(6, 1, 1);
      } else if (fuzzyEquals(sqrtS()/GeV, 630, 1E-3)) {
        _pt90Max630 = bookProfile1D(8, 1, 1);
        _pt90Min630 = bookProfile1D(8, 1, 2);
        _pt90Diff630 = bookProfile1D(8, 1, 3);
        _pTSum630_2Jet = bookProfile1D(9, 1, 1);
        _pTSum630_3Jet = bookProfile1D(9, 1, 2);

        _numTracksDbn630MB = bookHisto1D(10, 1, 1);
        _ptDbn630MB = bookHisto1D(11, 1, 1);
      }
    }


    /// Do the analysis
    void analyze(const Event& event) {
      // Trigger
      const bool trigger = applyProjection<TriggerCDFRun0Run1>(event, "Trigger").minBiasDecision();
      if (!trigger) vetoEvent;

      // Get sqrt(s) and event weight
      const double sqrtS = applyProjection<Beam>(event, "Beam").sqrtS();
      const double weight = event.weight();

      {
        MSG_DEBUG("Running max/min analysis");
        vector<Jet> jets = applyProjection<JetAlg>(event, "Jets").jetsByE();
        if (!jets.empty()) {
          // Leading jet must be in central |eta| < 0.5 region
          const Jet leadingjet = jets.front();
          const double etaLead = leadingjet.eta();
          // Get Et of the leading jet: used to bin histograms
          const double ETlead = leadingjet.Et();
          MSG_DEBUG("Leading Et = " << ETlead/GeV << " GeV");
          if (fabs(etaLead) > 0.5 && ETlead < 15*GeV) {
            MSG_DEBUG("Leading jet eta = " << etaLead
                     << " not in |eta| < 0.5 & pT > 15 GeV");
          } else {
            // Multiplicity & pT distributions for sqrt(s) = 630 GeV, 1800 GeV
            const Particles tracks = applyProjection<FinalState>(event, "TrackFS").particles();
            const ConesInfo cones = _calcTransCones(leadingjet.momentum(), tracks);
            if (fuzzyEquals(sqrtS/GeV, 630)) {
              _pt90Max630->fill(ETlead/GeV, cones.ptMax/GeV, weight);
              _pt90Min630->fill(ETlead/GeV, cones.ptMin/GeV, weight);
              _pt90Diff630->fill(ETlead/GeV, cones.ptDiff/GeV, weight);
            } else if (fuzzyEquals(sqrtS/GeV, 1800)) {
              _num90Max1800->fill(ETlead/GeV, cones.numMax, weight);
              _num90Min1800->fill(ETlead/GeV, cones.numMin, weight);
              _pt90Max1800->fill(ETlead/GeV, cones.ptMax/GeV, weight);
              _pt90Min1800->fill(ETlead/GeV, cones.ptMin/GeV, weight);
              _pt90Diff1800->fill(ETlead/GeV, cones.ptDiff/GeV, weight);
              _pt90MaxAvg1800->fill(ETlead/GeV, cones.ptMax/GeV, weight); // /numMax
              _pt90MinAvg1800->fill(ETlead/GeV, cones.ptMin/GeV, weight); // /numMin
              //
              const double ptTransTotal = cones.ptMax + cones.ptMin;
              if (inRange(ETlead/GeV, 40., 80.)) {
                _pt90Dbn1800Et40->fill(ptTransTotal/GeV, weight);
              } else if (inRange(ETlead/GeV, 80., 120.)) {
                _pt90Dbn1800Et80->fill(ptTransTotal/GeV, weight);
              } else if (inRange(ETlead/GeV, 120., 160.)) {
                _pt90Dbn1800Et120->fill(ptTransTotal/GeV, weight);
              } else if (inRange(ETlead/GeV, 160., 200.)) {
                _pt90Dbn1800Et160->fill(ptTransTotal/GeV, weight);
              } else if (inRange(ETlead/GeV, 200., 270.)) {
                _pt90Dbn1800Et200->fill(ptTransTotal/GeV, weight);
              }
            }

          }
        }
      }


      // Fill min bias total track multiplicity histos
      {
        MSG_DEBUG("Running min bias multiplicity analysis");
        const Particles mbtracks = applyProjection<FinalState>(event, "MBFS").particles();
        if (fuzzyEquals(sqrtS/GeV, 1800)) {
          _numTracksDbn1800MB->fill(mbtracks.size(), weight);
        } else if (fuzzyEquals(sqrtS/GeV, 630)) {
          _numTracksDbn630MB->fill(mbtracks.size(), weight);
        }
        // Run over all charged tracks
        foreach (const Particle& t, mbtracks) {
          FourMomentum trackMom = t.momentum();
          const double pt = trackMom.pT();
          // Plot total pT distribution for min bias
          if (fuzzyEquals(sqrtS/GeV, 1800)) {
            _ptDbn1800MB->fill(pt/GeV, weight);
          } else if (fuzzyEquals(sqrtS/GeV, 630)) {
            _ptDbn630MB->fill(pt/GeV, weight);
          }
        }
      }



      // Construct "Swiss Cheese" pT distributions, with pT contributions from
      // tracks within R = 0.7 of the 1st, 2nd (and 3rd) jets being ignored. A
      // different set of charged tracks, with |eta| < 1.0, is used here, and all
      // the removed jets must have Et > 5 GeV.
      {
        MSG_DEBUG("Running Swiss Cheese analysis");
        const Particles cheesetracks = applyProjection<FinalState>(event, "CheeseFS").particles();
        vector<Jet> cheesejets = applyProjection<JetAlg>(event, "Jets").jetsByE();
        if (cheesejets.empty()) {
          MSG_DEBUG("No 'cheese' jets found in event");
          return;
        }
        if (cheesejets.size() > 1 &&
            fabs(cheesejets[0].momentum().pseudorapidity()) <= 0.5 &&
            cheesejets[0].momentum().Et()/GeV > 5.0 &&
            cheesejets[1].momentum().Et()/GeV > 5.0) {

          const double cheeseETlead = cheesejets[0].momentum().Et();

          const double eta1 = cheesejets[0].momentum().pseudorapidity();
          const double phi1 = cheesejets[0].momentum().azimuthalAngle();
          const double eta2 = cheesejets[1].momentum().pseudorapidity();
          const double phi2 = cheesejets[1].momentum().azimuthalAngle();

          double ptSumSub2(0), ptSumSub3(0);
          foreach (const Particle& t, cheesetracks) {
            FourMomentum trackMom = t.momentum();
            const double pt = trackMom.pT();

            // Subtracting 2 leading jets
            const double deltaR1 = deltaR(trackMom, eta1, phi1);
            const double deltaR2 = deltaR(trackMom, eta2, phi2);
            MSG_TRACE("Track vs jet(1): "
                     << "|(" << trackMom.pseudorapidity() << ", " << trackMom.azimuthalAngle() << ") - "
                     << "|(" << eta1 << ", " << phi1 << ")| = " << deltaR1);
            MSG_TRACE("Track vs jet(2): "
                     << "|(" << trackMom.pseudorapidity() << ", " << trackMom.azimuthalAngle() << ") - "
                     << "|(" << eta2 << ", " << phi2 << ")| = " << deltaR2);
            if (deltaR1 > 0.7 && deltaR2 > 0.7) {
              ptSumSub2 += pt;

              // Subtracting 3rd leading jet
              if (cheesejets.size() > 2 &&
                  cheesejets[2].momentum().Et()/GeV > 5.0) {
                const double eta3 = cheesejets[2].momentum().pseudorapidity();
                const double phi3 = cheesejets[2].momentum().azimuthalAngle();
                const double deltaR3 = deltaR(trackMom, eta3, phi3);
                MSG_TRACE("Track vs jet(3): "
                         << "|(" << trackMom.pseudorapidity() << ", " << trackMom.azimuthalAngle() << ") - "
                         << "|(" << eta3 << ", " << phi3 << ")| = " << deltaR3);
                if (deltaR3 > 0.7) {
                  ptSumSub3 += pt;
                }
              }
            }
          }

          // Swiss Cheese sub 2,3 jets distributions for sqrt(s) = 630 GeV, 1800 GeV
          if (fuzzyEquals(sqrtS/GeV, 630)) {
            if (!isZero(ptSumSub2)) _pTSum630_2Jet->fill(cheeseETlead/GeV, ptSumSub2/GeV, weight);
            if (!isZero(ptSumSub3))_pTSum630_3Jet->fill(cheeseETlead/GeV, ptSumSub3/GeV, weight);
          } else if (fuzzyEquals(sqrtS/GeV, 1800)) {
            if (!isZero(ptSumSub2))_pTSum1800_2Jet->fill(cheeseETlead/GeV, ptSumSub2/GeV, weight);
            if (!isZero(ptSumSub3))_pTSum1800_3Jet->fill(cheeseETlead/GeV, ptSumSub3/GeV, weight);
          }

        }
      }

    }


    void finalize() {
      /// @todo Take these normalisations from the data histo (it can't come from just the MC)

      if (fuzzyEquals(sqrtS()/GeV, 1800, 1E-3)) {
        // Normalize to actual number of entries in pT dbn histos...
        normalize(_pt90Dbn1800Et40,  1656.75); // norm OK
        normalize(_pt90Dbn1800Et80,  4657.5); // norm OK
        normalize(_pt90Dbn1800Et120, 5395.5); // norm OK
        normalize(_pt90Dbn1800Et160, 7248.75); // norm OK
        normalize(_pt90Dbn1800Et200, 2442.0); // norm OK
      }

      // ...and for min bias distributions:
      if (fuzzyEquals(sqrtS()/GeV, 1800, 1E-3)) {
        normalize(_numTracksDbn1800MB, 309718.25); // norm OK
        normalize(_ptDbn1800MB, 33600.0); // norm OK
      } else if (fuzzyEquals(sqrtS()/GeV, 630, 1E-3)) {
        normalize(_numTracksDbn630MB, 1101024.0); // norm OK
        normalize(_ptDbn630MB, 105088.0); // norm OK
      }
    }

    //@}


  private:

    /// @name Histogram collections
    //@{
    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the average \f$ p_T \f$ in the toward, transverse and away regions at
    /// \f$ \sqrt{s} = 1800 \text{GeV} \f$.
    /// Corresponds to Table 1, and HepData table 1.
    Profile1DPtr _pt90MaxAvg1800, _pt90MinAvg1800;

    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the \f$ p_T \f$ sum in the toward, transverse and away regions at
    /// \f$ \sqrt{s} = 1800 \text{GeV} \f$.
    /// Corresponds to figure 2/3, and HepData table 2.
    Profile1DPtr _pt90Max1800, _pt90Min1800, _pt90Diff1800;

    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the \f$ p_T \f$ sum in the toward, transverse and away regions at
    /// at \f$ \sqrt{s} = 630 \text{GeV} \f$.
    /// Corresponds to figure 8, and HepData table 8.
    Profile1DPtr _pt90Max630, _pt90Min630, _pt90Diff630;

    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the cone track multiplicity at \f$ \sqrt{s} = 1800 \text{GeV} \f$.
    /// Corresponds to figure 5, and HepData table 4.
    Profile1DPtr _num90Max1800, _num90Min1800;

    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the \f$ p_T \f$ sum at \f$ \sqrt{s} = 1800 \text{GeV} \f$.
    /// Corresponds to figure 7, and HepData table 7.
    Profile1DPtr _pTSum1800_2Jet, _pTSum1800_3Jet;

    /// Profile histograms, binned in the \f$ E_T \f$ of the leading jet, for
    /// the \f$ p_T \f$ sum at \f$ \sqrt{s} = 630 \text{GeV} \f$.
    /// Corresponds to figure 9, and HepData table 9.
    Profile1DPtr _pTSum630_2Jet, _pTSum630_3Jet;

    /// Histogram of \f$ p_{T\text{sum}} \f$ distribution for 5 different
    /// \f$ E_{T1} \f$ bins.
    /// Corresponds to figure 4, and HepData table 3.
    Histo1DPtr _pt90Dbn1800Et40, _pt90Dbn1800Et80, _pt90Dbn1800Et120,
      _pt90Dbn1800Et160, _pt90Dbn1800Et200;

    /// Histograms of track multiplicity and \f$ p_T \f$ distributions for
    /// minimum bias events.
    /// Figure 6, and HepData tables 5 & 6.
    /// Figure 10, and HepData tables 10 & 11.
    Histo1DPtr _numTracksDbn1800MB, _ptDbn1800MB;
    Histo1DPtr _numTracksDbn630MB, _ptDbn630MB;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2004_S5839831);

}

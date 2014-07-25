// -*- C++ -*-

// Field & Stuart underlying event analysis at CDF.
// Phys.Rev.D65:092002,2002 - no arXiv code.
// FNAL-PUB 01/211-E

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/ConstLossyFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/TriggerCDFRun0Run1.hh"

namespace Rivet {


  /// @brief "Field-Stuart" CDF Run I track-jet underlying event analysis
  ///
  /// @author Andy Buckley
  ///
  /// The "original" underlying event analysis, using a non-standard track-jet algorithm.
  ///
  /// @par Run conditions
  ///
  /// @arg \f$ \sqrt{s} = \f$ 1800 GeV
  /// @arg Run with generic QCD events.
  /// @arg Several \f$ p_\perp^\text{min} \f$ cutoffs are probably required to fill the profile histograms:
  ///   @arg \f$ p_\perp^\text{min} = \f$ 0 (min bias), 10, 20 GeV
  class CDF_2001_S4751469 : public Analysis {
  public:

    /// Constructor: cuts on final state are \f$ -1 < \eta < 1 \f$
    /// and \f$ p_T > 0.5 \f$ GeV.
    CDF_2001_S4751469()
      : Analysis("CDF_2001_S4751469"),
        _totalNumTrans2(0), _totalNumTrans5(0), _totalNumTrans30(0),
        _sumWeightsPtLead2(0),_sumWeightsPtLead5(0), _sumWeightsPtLead30(0)
    {
    }


    /// @name Analysis methods
    //@{

    // Book histograms
    void init() {
      addProjection(TriggerCDFRun0Run1(), "Trigger");
      // Randomly discard 8% of charged particles as a kind of hacky detector correction.
      const ChargedFinalState cfs(-1.0, 1.0, 0.5*GeV);
      const ConstLossyFinalState lfs(cfs, 0.08);

      addProjection(lfs, "FS");
      addProjection(FastJets(lfs, FastJets::TRACKJET, 0.7), "TrackJet");

      _numvsDeltaPhi2 =  bookProfile1D(1, 1, 1);
      _numvsDeltaPhi5 =  bookProfile1D(1, 1, 2);
      _numvsDeltaPhi30 = bookProfile1D(1, 1, 3);
      _pTvsDeltaPhi2 =   bookProfile1D(2, 1, 1);
      _pTvsDeltaPhi5 =   bookProfile1D(2, 1, 2);
      _pTvsDeltaPhi30 =  bookProfile1D(2, 1, 3);

      _numTowardMB = bookProfile1D(3, 1, 1);
      _numTransMB = bookProfile1D(3, 1, 2);
      _numAwayMB = bookProfile1D(3, 1, 3);
      _numTowardJ20 = bookProfile1D(4, 1, 1);
      _numTransJ20 = bookProfile1D(4, 1, 2);
      _numAwayJ20 = bookProfile1D(4, 1, 3);

      _ptsumTowardMB = bookProfile1D(5, 1, 1);
      _ptsumTransMB = bookProfile1D(5, 1, 2);
      _ptsumAwayMB = bookProfile1D(5, 1, 3);
      _ptsumTowardJ20 = bookProfile1D(6, 1, 1);
      _ptsumTransJ20 = bookProfile1D(6, 1, 2);
      _ptsumAwayJ20 = bookProfile1D(6, 1, 3);

      _ptTrans2 = bookHisto1D(7, 1, 1);
      _ptTrans5 = bookHisto1D(7, 1, 2);
      _ptTrans30 = bookHisto1D(7, 1, 3);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      // Trigger
      const bool trigger = applyProjection<TriggerCDFRun0Run1>(event, "Trigger").minBiasDecision();
      if (!trigger) vetoEvent;

      // Analyse, with pT > 0.5 GeV AND |eta| < 1
      const JetAlg& tj = applyProjection<JetAlg>(event, "TrackJet");

      // Final state (lossy) charged particles
      const FinalState& fs = applyProjection<FinalState>(event, "FS");

      // Get jets, sorted by pT
      const Jets jets = tj.jetsByPt();
      if (jets.empty()) {
        vetoEvent;
      }

      Jet leadingJet = jets.front();
      const double phiLead = leadingJet.phi();
      const double ptLead = leadingJet.pT();

      // Cut on highest pT jet: combined 0.5 GeV < pT(lead) < 50 GeV
      if (ptLead/GeV < 0.5) vetoEvent;
      if (ptLead/GeV > 50.0) vetoEvent;

      // Get the event weight
      const double weight = event.weight();

      // Count sum of all event weights in three pT_lead regions
      if (ptLead/GeV > 2.0) {
        _sumWeightsPtLead2 += weight;
      }
      if (ptLead/GeV > 5.0) {
        _sumWeightsPtLead5 += weight;
      }
      if (ptLead/GeV > 30.0) {
        _sumWeightsPtLead30 += weight;
      }

      // Run over tracks
      double ptSumToward(0.0), ptSumAway(0.0), ptSumTrans(0.0);
      size_t numToward(0), numTrans(0), numAway(0);

      // Temporary histos that bin N and pT in dphi
      Profile1D htmp_num_dphi_2(refData(1, 1, 1)), htmp_num_dphi_5(refData(1, 1, 2)), htmp_num_dphi_30(refData(1, 1, 3));
      Profile1D htmp_pt_dphi_2(refData(2, 1, 1)), htmp_pt_dphi_5(refData(2, 1, 2)), htmp_pt_dphi_30(refData(2, 1, 3));

      foreach (const Particle& p, fs.particles()) {
        // Calculate DeltaPhi(p,leadingJet)
        const double dPhi = deltaPhi(p.momentum().phi(), phiLead);
        const double pT = p.pT();

        if (dPhi < PI/3.0) {
          ptSumToward += pT;
          ++numToward;
        }
        else if (dPhi < 2*PI/3.0) {
          ptSumTrans += pT;
          ++numTrans;
          // Fill transverse pT distributions
          if (ptLead/GeV > 2.0) {
            _ptTrans2->fill(pT/GeV, weight);
            _totalNumTrans2 += weight;
          }
          if (ptLead/GeV > 5.0) {
            _ptTrans5->fill(pT/GeV, weight);
            _totalNumTrans5 += weight;
          }
          if (ptLead/GeV > 30.0) {
            _ptTrans30->fill(pT/GeV, weight);
            _totalNumTrans30 += weight;
          }
        }
        else {
          ptSumAway += pT;
          ++numAway;
        }

        // Fill tmp histos to bin event's track Nch & pT in dphi
        const double dPhideg = 180*dPhi/PI;
        if (ptLead/GeV > 2.0) {
          htmp_num_dphi_2.fill(dPhideg, 1);
          htmp_pt_dphi_2.fill (dPhideg, pT/GeV);
        }
        if (ptLead/GeV > 5.0) {
          htmp_num_dphi_5.fill(dPhideg, 1);
          htmp_pt_dphi_5.fill (dPhideg, pT/GeV);
        }
        if (ptLead/GeV > 30.0) {
          htmp_num_dphi_30.fill(dPhideg, 1);
          htmp_pt_dphi_30.fill (dPhideg, pT/GeV);
        }
      }

      // Update the "proper" dphi profile histograms
      for (int i = 0; i < 50; i++) { //< @todo Should really explicitly iterate over nbins for each temp histo
        if (ptLead/GeV > 2.0) {
          const double x2 = htmp_pt_dphi_2.bin(i).midpoint();
          const double num2 = (htmp_num_dphi_2.bin(i).numEntries() > 0) ? htmp_num_dphi_2.bin(i).mean() : 0.0;
          const double pt2 = (htmp_num_dphi_2.bin(i).numEntries() > 0) ? htmp_pt_dphi_2.bin(i).mean() : 0.0;
          _numvsDeltaPhi2->fill(x2, num2, weight);
          _pTvsDeltaPhi2->fill(x2, pt2, weight);
        }
        if (ptLead/GeV > 5.0) {
          const double x5 = htmp_pt_dphi_5.bin(i).midpoint();
          const double num5 = (htmp_num_dphi_5.bin(i).numEntries() > 0) ? htmp_num_dphi_5.bin(i).mean() : 0.0;
          const double pt5 = (htmp_num_dphi_5.bin(i).numEntries() > 0) ? htmp_pt_dphi_5.bin(i).mean() : 0.0;
          _numvsDeltaPhi5->fill(x5, num5, weight);
          _pTvsDeltaPhi5->fill(x5, pt5, weight);
        }
        if (ptLead/GeV > 30.0) {
          const double x30 = htmp_pt_dphi_30.bin(i).midpoint();
          const double num30 = (htmp_num_dphi_30.bin(i).numEntries() > 0) ? htmp_num_dphi_30.bin(i).mean() : 0.0;
          const double pt30 = (htmp_num_dphi_30.bin(i).numEntries() > 0) ? htmp_pt_dphi_30.bin(i).mean() : 0.0;
          _numvsDeltaPhi30->fill(x30, num30, weight);
          _pTvsDeltaPhi30->fill(x30, pt30, weight);
        }
      }

      // Log some event details about pT
      MSG_DEBUG("pT [lead; twd, away, trans] = [" << ptLead << "; "
                << ptSumToward << ", " << ptSumAway << ", " << ptSumTrans << "]");

      // Update the pT profile histograms
      _ptsumTowardMB->fill(ptLead/GeV, ptSumToward/GeV, weight);
      _ptsumTowardJ20->fill(ptLead/GeV, ptSumToward/GeV, weight);

      _ptsumTransMB->fill(ptLead/GeV, ptSumTrans/GeV, weight);
      _ptsumTransJ20->fill(ptLead/GeV, ptSumTrans/GeV, weight);

      _ptsumAwayMB->fill(ptLead/GeV, ptSumAway/GeV, weight);
      _ptsumAwayJ20->fill(ptLead/GeV, ptSumAway/GeV, weight);

      // Log some event details about Nch
      MSG_DEBUG("N [twd, away, trans] = [" << ptLead << "; "
                << numToward << ", " << numTrans << ", " << numAway << "]");

      // Update the N_track profile histograms
      _numTowardMB->fill(ptLead/GeV, numToward, weight);
      _numTowardJ20->fill(ptLead/GeV, numToward, weight);

      _numTransMB->fill(ptLead/GeV, numTrans, weight);
      _numTransJ20->fill(ptLead/GeV, numTrans, weight);

      _numAwayMB->fill(ptLead/GeV, numAway, weight);
      _numAwayJ20->fill(ptLead/GeV, numAway, weight);
    }


    /// Normalize histos
    void finalize() {
      normalize(_ptTrans2, _totalNumTrans2 / _sumWeightsPtLead2);
      normalize(_ptTrans5, _totalNumTrans5 / _sumWeightsPtLead5);
      normalize(_ptTrans30, _totalNumTrans30 / _sumWeightsPtLead30);
    }

    //@}


  private:

    /// Sum total number of charged particles in the trans region, in 3 \f$ p_\perp^\text{lead} \f$ bins.
    double _totalNumTrans2, _totalNumTrans5, _totalNumTrans30;

    /// Sum the total number of events in 3 \f$ p_\perp^\text{lead} \f$ bins.
    double _sumWeightsPtLead2,_sumWeightsPtLead5, _sumWeightsPtLead30;


    /// @name Histogram collections
    //@{

    // The sumpt vs. dphi and Nch vs. dphi histos
    Profile1DPtr _numvsDeltaPhi2, _numvsDeltaPhi5, _numvsDeltaPhi30;
    Profile1DPtr _pTvsDeltaPhi2, _pTvsDeltaPhi5, _pTvsDeltaPhi30;

    /// Profile histograms, binned in the \f$ p_T \f$ of the leading jet, for
    /// the \f$ p_T \f$ sum in the toward, transverse and away regions.
    Profile1DPtr _ptsumTowardMB,  _ptsumTransMB,  _ptsumAwayMB;
    Profile1DPtr _ptsumTowardJ20, _ptsumTransJ20, _ptsumAwayJ20;

    /// Profile histograms, binned in the \f$ p_T \f$ of the leading jet, for
    /// the number of charged particles per jet in the toward, transverse and
    /// away regions.
    Profile1DPtr _numTowardMB,  _numTransMB,  _numAwayMB;
    Profile1DPtr _numTowardJ20, _numTransJ20, _numAwayJ20;

    /// Histogram of \f$ p_T \f$ distribution for 3 different \f$ p_{T1} \f$ IR cutoffs.
    Histo1DPtr _ptTrans2, _ptTrans5, _ptTrans30;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2001_S4751469);

}

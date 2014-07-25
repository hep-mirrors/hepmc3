// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/BinnedHistogram.hh"

namespace Rivet {


  /// ATLAS charged particle jet underlying event and jet radius dependence
  class ATLAS_2012_I1125575 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I1125575()
      : Analysis("ATLAS_2012_I1125575")
    {    }

    //@}


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      const ChargedFinalState jet_input(-2.5, 2.5, 0.5*GeV);
      addProjection(jet_input, "JET_INPUT");

      const ChargedFinalState track_input(-1.5, 1.5, 0.5*GeV);
      addProjection(track_input, "TRACK_INPUT");

      const FastJets jets02(jet_input, FastJets::ANTIKT, 0.2);
      addProjection(jets02, "JETS_02");

      const FastJets jets04(jet_input, FastJets::ANTIKT, 0.4);
      addProjection(jets04, "JETS_04");

      const FastJets jets06(jet_input, FastJets::ANTIKT, 0.6);
      addProjection(jets06, "JETS_06");

      const FastJets jets08(jet_input, FastJets::ANTIKT, 0.8);
      addProjection(jets08, "JETS_08");

      const FastJets jets10(jet_input, FastJets::ANTIKT, 1.0);
      addProjection(jets10, "JETS_10");

      // Mean number of tracks
      initializeProfiles(_h_meanNch, 1);

      // Mean of the average track pT in each region
      initializeProfiles(_h_meanPtAvg, 2);

      // Mean of the scalar sum of track pT in each region
      initializeProfiles(_h_meanPtSum, 3);

      // Distribution of Nch, in bins of leading track-jet pT
      initializeHistograms(_h_Nch, 4);

      // Distribution of average track-jet pT, in bins of leading track-jet pT
      initializeHistograms(_h_PtAvg, 5);

      // Distribution of sum of track-jet pT, in bins of leading track-jet pT
      initializeHistograms(_h_PtSum, 6);

      for (int i = 0; i < 5; ++i)
        _nEvents[i] = 0.0;
    }


    void initializeProfiles(Profile1DPtr plots[5][2], int distribution) {
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
          plots[i][j] = bookProfile1D(distribution, i+1, j+1);
        }
      }
    }


    void initializeHistograms(BinnedHistogram<double> plots[5][2], int distribution) {
      Scatter2D refscatter = refData(1, 1, 1);
      for (int i = 0; i < 5; ++i) {
        for (int y = 0; y < 2; ++y) {
          for (size_t j = 0; j < refscatter.numPoints(); ++j) {
            int histogram_number = ((j+1)*2)-((y+1)%2);
            double low_edge = refscatter.point(j).xMin();
            double high_edge = refscatter.point(j).xMax();
            plots[i][y].addHistogram(low_edge, high_edge, bookHisto1D(distribution, i+1, histogram_number));
          }
        }
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      vector<Jets*> all_jets;
      Jets jets_02 = applyProjection<FastJets>(event, "JETS_02").jetsByPt(4.0*GeV, MAXDOUBLE, -1.5, 1.5, PSEUDORAPIDITY);
      all_jets.push_back(&jets_02);
      Jets jets_04 = applyProjection<FastJets>(event, "JETS_04").jetsByPt(4.0*GeV, MAXDOUBLE, -1.5, 1.5, PSEUDORAPIDITY);
      all_jets.push_back(&jets_04);
      Jets jets_06 = applyProjection<FastJets>(event, "JETS_06").jetsByPt(4.0*GeV, MAXDOUBLE, -1.5, 1.5, PSEUDORAPIDITY);
      all_jets.push_back(&jets_06);
      Jets jets_08 = applyProjection<FastJets>(event, "JETS_08").jetsByPt(4.0*GeV, MAXDOUBLE, -1.5, 1.5, PSEUDORAPIDITY);
      all_jets.push_back(&jets_08);
      Jets jets_10 = applyProjection<FastJets>(event, "JETS_10").jetsByPt(4.0*GeV, MAXDOUBLE, -1.5, 1.5, PSEUDORAPIDITY);
      all_jets.push_back(&jets_10);

      // Count the number of tracks in the away and transverse regions, for each set of jets
      double n_ch[5][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
      // Also add up the sum pT
      double sumpt[5][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
      // ptmean = sumpt / n_ch
      double ptavg[5][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
      // lead jet pT defines which bin we want to fill
      double lead_jet_pts[5] = {0.0};

      // Loop over each of the jet radii:
      for (int i = 0; i < 5; ++i) {
        if (all_jets[i]->size() < 1) continue;

        // Find the lead jet pT
        lead_jet_pts[i] = all_jets[i]->at(0).pT();

        // Loop over each of the charged particles
        const Particles& tracks = applyProjection<ChargedFinalState>(event, "TRACK_INPUT").particlesByPt();
        foreach(const Particle& t, tracks) {

          // Get the delta-phi between the track and the leading jet
          double dphi = deltaPhi(all_jets[i]->at(0), t);

          // Find out which region this puts it in.
          // 0 = away region, 1 = transverse region, 2 = toward region
          int region = region_index(dphi);

          // If the track is in the toward region, ignore it.
          if (region == 2) continue;

          // Otherwise, increment the relevant counters
          ++n_ch[i][region];
          sumpt[i][region] += t.pT();

        }
        // Calculate the pT_avg for the away and transverse regions.
        // (And make sure we don't try to divide by zero.)
        ptavg[i][0] = (n_ch[i][0] == 0 ? 0.0 : sumpt[i][0] / n_ch[i][0]);
        ptavg[i][1] = (n_ch[i][1] == 0 ? 0.0 : sumpt[i][1] / n_ch[i][1]);

        _nEvents[i] += weight;
      }

      fillProfiles(_h_meanNch,    n_ch, lead_jet_pts, weight, 1.0 / (2*PI));
      fillProfiles(_h_meanPtAvg, ptavg, lead_jet_pts, weight, 1.0);
      fillProfiles(_h_meanPtSum, sumpt, lead_jet_pts, weight, 1.0 / (2*PI));

      fillHistograms(_h_Nch,    n_ch, lead_jet_pts, weight);
      fillHistograms(_h_PtAvg, ptavg, lead_jet_pts, weight);
      fillHistograms(_h_PtSum, sumpt, lead_jet_pts, weight);
    }


    void fillProfiles(Profile1DPtr plots[5][2], double var[5][2], double lead_pt[5], double weight, double scale) {
      for (int i=0; i<5; ++i) {
        double pt = lead_pt[i];
        for (int j=0; j<2; ++j) {
          double v = var[i][j];
          plots[i][j]->fill(pt, v*scale, weight);
        }
      }
    }


    void fillHistograms(BinnedHistogram<double> plots[5][2], double var[5][2], double lead_pt[5], double weight) {
      for (int i=0; i<5; ++i) {
        double pt = lead_pt[i];
        for (int j=0; j<2; ++j) {
          double v = var[i][j];
          plots[i][j].fill(pt, v, weight);
        }
      }
    }


    int region_index(double dphi) {
      assert(inRange(dphi, 0.0, PI, CLOSED, CLOSED));
      if (dphi < PI/3.0) return 2;
      if (dphi < 2*PI/3.0) return 1;
      return 0;
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      finalizeHistograms(_h_Nch);
      finalizeHistograms(_h_PtAvg);
      finalizeHistograms(_h_PtSum);
    }


    void finalizeHistograms(BinnedHistogram<double> plots[5][2]) {
      for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 2; ++j) {
          vector<Histo1DPtr> histos = plots[i][j].getHistograms();
          foreach(Histo1DPtr h, histos) {
            scale(h, 1.0/_nEvents[i]);
          }
        }
      }
    }

    //@}


  private:

    // Data members like post-cuts event weight counters go here
    double _nEvents[5];

    Profile1DPtr _h_meanNch[5][2];
    Profile1DPtr _h_meanPtAvg[5][2];
    Profile1DPtr _h_meanPtSum[5][2];

    BinnedHistogram<double> _h_Nch[5][2];
    BinnedHistogram<double> _h_PtAvg[5][2];
    BinnedHistogram<double> _h_PtSum[5][2];

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1125575);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

#include "Rivet/Tools/BinnedHistogram.hh"

namespace Rivet {


  struct ATLAS_2011_S9126244_Plots {

    int selectionType; //< The HepData y-axis code
    string intermediateHistName;

    // Gap fraction vs DeltaY plot setup
    int _gapFractionDeltaYHistIndex;
    vector<double> _gapFractionDeltaYSlices;
    BinnedHistogram<double> _h_gapVsDeltaYVeto;
    BinnedHistogram<double> _h_gapVsDeltaYInc;

    // Gap fraction vs ptBar plot setup
    int _gapFractionPtBarHistIndex;
    vector<double> _gapFractionPtBarSlices;
    BinnedHistogram<double> _h_gapVsPtBarVeto;
    BinnedHistogram<double> _h_gapVsPtBarInc;

    // Gap fraction vs Q0 plot setup
    int _gapFractionQ0HistIndex;
    vector<double> _gapFractionQ0SlicesPtBar;
    vector<double> _gapFractionQ0SlicesDeltaY;
    vector<Histo1DPtr> _h_vetoPt;
    vector<Scatter2DPtr> _d_vetoPtGapFraction;
    vector<double> _vetoPtTotalSum; //< @todo Can this just be replaced with _h_vetoPt.integral()?

    // Average njet vs DeltaY setup
    int _avgNJetDeltaYHistIndex;
    vector<double> _avgNJetDeltaYSlices;
    vector<Profile1DPtr> _p_avgJetVsDeltaY;

    // Average njet vs PptBar setup
    int _avgNJetPtBarHistIndex;
    vector<double> _avgNJetPtBarSlices;
    vector<Profile1DPtr> _p_avgJetVsPtBar;
  };



  /// ATLAS dijet production with central jet veto
  /// @todo Make sure that temp histos are removed
  class ATLAS_2011_S9126244 : public Analysis {
  public:

    /// Constructor
    ATLAS_2011_S9126244()
      : Analysis("ATLAS_2011_S9126244")
    {    }


  public:

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialize the lone projection required
      addProjection(FastJets(FinalState(), FastJets::ANTIKT, 0.6), "AntiKtJets06");

      // Initialize plots for each selection type
      _selectionPlots[0].intermediateHistName = "highestPt";
      _selectionPlots[0].selectionType = 1;
      _selectionPlots[0]._gapFractionDeltaYHistIndex = 6;
      _selectionPlots[0]._gapFractionPtBarHistIndex = 1;
      _selectionPlots[0]._gapFractionQ0HistIndex = 13;
      _selectionPlots[0]._avgNJetDeltaYHistIndex = 37;
      _selectionPlots[0]._avgNJetPtBarHistIndex = 26;
      _selectionPlots[0]._gapFractionDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      _selectionPlots[0]._gapFractionPtBarSlices += 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
      _selectionPlots[0]._gapFractionQ0SlicesPtBar += 70.0, 90.0, 120.0, 150.0, 210.0, 240.0;
      _selectionPlots[0]._gapFractionQ0SlicesDeltaY += 2.0, 3.0, 4.0, 5.0;
      _selectionPlots[0]._avgNJetPtBarSlices += 1.0, 2.0, 3.0, 4.0, 5.0;
      _selectionPlots[0]._avgNJetDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      initializePlots(_selectionPlots[0]);

      _selectionPlots[1].intermediateHistName = "forwardBackward";
      _selectionPlots[1].selectionType = 2;
      _selectionPlots[1]._gapFractionDeltaYHistIndex = 6;
      _selectionPlots[1]._gapFractionPtBarHistIndex = 1;
      _selectionPlots[1]._gapFractionQ0HistIndex = 13;
      _selectionPlots[1]._avgNJetDeltaYHistIndex = 37;
      _selectionPlots[1]._avgNJetPtBarHistIndex = 26;
      _selectionPlots[1]._gapFractionDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      _selectionPlots[1]._gapFractionPtBarSlices += 1.0, 2.0, 3.0, 4.0, 5.0, 6.0;
      _selectionPlots[1]._gapFractionQ0SlicesPtBar += 70.0, 90.0, 120.0, 150.0, 210.0, 240.0;
      _selectionPlots[1]._gapFractionQ0SlicesDeltaY += 2.0, 3.0, 4.0, 5.0;
      _selectionPlots[1]._avgNJetPtBarSlices += 1.0, 2.0, 3.0, 4.0, 5.0;
      _selectionPlots[1]._avgNJetDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      initializePlots(_selectionPlots[1]);

      _selectionPlots[2].intermediateHistName = "forwardBackward_PtBarVeto";
      _selectionPlots[2].selectionType = 1;
      _selectionPlots[2]._gapFractionDeltaYHistIndex = 19;
      _selectionPlots[2]._avgNJetDeltaYHistIndex = 30;
      _selectionPlots[2]._gapFractionDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      _selectionPlots[2]._avgNJetDeltaYSlices += 70.0, 90.0, 120.0, 150.0, 180.0, 210.0, 240.0, 270.0;
      initializePlots(_selectionPlots[2]);
    }


    void initializePlots(ATLAS_2011_S9126244_Plots& plots) {

      // Gap fraction vs DeltaY
      if (!plots._gapFractionDeltaYSlices.empty()) {
        for (size_t x = 0; x < plots._gapFractionDeltaYSlices.size()-1; x++) {
          const string vetoHistName = "gapDeltaYVeto_" + plots.intermediateHistName + "_" + to_str(x);
          const string inclusiveHistName = "gapDeltaYInclusive_" + plots.intermediateHistName + "_" + to_str(x);
          plots._h_gapVsDeltaYVeto.addHistogram(plots._gapFractionDeltaYSlices[x], plots._gapFractionDeltaYSlices[x+1],
                                                bookHisto1D(plots._gapFractionDeltaYHistIndex+x, 1, plots.selectionType, vetoHistName));
          plots._h_gapVsDeltaYInc.addHistogram(plots._gapFractionDeltaYSlices[x], plots._gapFractionDeltaYSlices[x+1],
                                               bookHisto1D(plots._gapFractionDeltaYHistIndex+x, 1, plots.selectionType, inclusiveHistName));
        }
      }

      // Average njet vs DeltaY
      if (!plots._avgNJetDeltaYSlices.empty()) {
        for (size_t x = 0; x < plots._avgNJetDeltaYSlices.size()-1; x++) {
          plots._p_avgJetVsDeltaY += bookProfile1D(plots._avgNJetDeltaYHistIndex+x, 1, plots.selectionType);
        }
      }

      // Gap fraction vs PtBar
      if (!plots._gapFractionPtBarSlices.empty()) {
        for (size_t x = 0; x < plots._gapFractionPtBarSlices.size()-1; x++) {
          const string vetoHistName = "gapPtBarVeto_" + plots.intermediateHistName + "_" + to_str(x);
          const string inclusiveHistName = "gapPtBarInclusive_" + plots.intermediateHistName + "_" + to_str(x);
          plots._h_gapVsPtBarVeto.addHistogram(plots._gapFractionPtBarSlices[x], plots._gapFractionPtBarSlices[x+1],
                                               bookHisto1D(plots._gapFractionPtBarHistIndex+x, 1, plots.selectionType, vetoHistName));
          plots._h_gapVsPtBarInc.addHistogram(plots._gapFractionPtBarSlices[x], plots._gapFractionPtBarSlices[x+1],
                                              bookHisto1D(plots._gapFractionPtBarHistIndex+x, 1, plots.selectionType, inclusiveHistName));
        }
      }

      // Average njet vs PtBar
      if (!plots._avgNJetPtBarSlices.empty()) {
        for (size_t x=0; x<plots._avgNJetPtBarSlices.size()-1; x++) {
          plots._p_avgJetVsPtBar += bookProfile1D(plots._avgNJetPtBarHistIndex+x, 1, plots.selectionType);
        }
      }

      // Gap fraction vs Q0
      int q0PlotCount = 0;
      for (size_t x = 0; x < plots._gapFractionQ0SlicesPtBar.size()/2; x++) {
        for (size_t y = 0; y < plots._gapFractionQ0SlicesDeltaY.size()/2; y++) {
          const string vetoPtHistName = "vetoPt_" + plots.intermediateHistName + "_" + to_str(q0PlotCount);
          plots._h_vetoPt += bookHisto1D(vetoPtHistName, refData(plots._gapFractionQ0HistIndex + q0PlotCount, 1, plots.selectionType));
          plots._d_vetoPtGapFraction += bookScatter2D(plots._gapFractionQ0HistIndex + q0PlotCount, 1, plots.selectionType);
          plots._vetoPtTotalSum += 0.0; //< @todo Can this just be replaced with _h_vetoPt.integral()?
          q0PlotCount += 1;
        }
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Get minimal list of jets needed to be considered
      double minimumJetPtBar = 50.0*GeV; // of interval defining jets

      vector<FourMomentum> acceptJets;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "AntiKtJets06").jetsByPt(20.0*GeV)) {
        if (fabs(jet.rapidity()) < 4.4) {
          acceptJets.push_back(jet.momentum());
        }
      }

      // If we can't form an interval, drop out of the analysis early
      if (acceptJets.size() < 2) vetoEvent;

      // Analyze leading jet case
      if (acceptJets[0].pT() + acceptJets[1].pT() > 2*minimumJetPtBar) {
        analyzeJets(acceptJets, _selectionPlots[0], weight, 20.0*GeV);
      }

      // Find the most forward-backward jets
      size_t minRapidityJet = 0, maxRapidityJet = 0;
      for (size_t j = 1; j < acceptJets.size(); j++) {
        if (acceptJets[j].rapidity() > acceptJets[maxRapidityJet].rapidity()) maxRapidityJet = j;
        if (acceptJets[j].rapidity() < acceptJets[minRapidityJet].rapidity()) minRapidityJet = j;
      }

      // Make a container of jet momenta with the extreme f/b jets at the front
      vector<FourMomentum> fwdBkwdJets;
      fwdBkwdJets.push_back(acceptJets[maxRapidityJet]);
      fwdBkwdJets.push_back(acceptJets[minRapidityJet]);
      for (size_t j = 0; j < acceptJets.size(); j++) {
        if (j == minRapidityJet || j == maxRapidityJet) continue;
        fwdBkwdJets.push_back(acceptJets[j]);
      }

      if (fwdBkwdJets[0].pT() + fwdBkwdJets[1].pT() > 2*minimumJetPtBar) {
        // Use most forward/backward jets in rapidity to define the interval
        analyzeJets(fwdBkwdJets, _selectionPlots[1], weight, 20.0*GeV);
        // As before but now using PtBar of interval to define veto threshold
        analyzeJets(fwdBkwdJets, _selectionPlots[2], weight, (fwdBkwdJets[0].pT()+fwdBkwdJets[1].pT())/2.0);
      }
    }


    /// Fill plots!
    void analyzeJets(vector<FourMomentum>& jets, ATLAS_2011_S9126244_Plots& plots,
                     const double weight, double vetoPtThreshold) {

      // Calculate the interval size, ptBar and veto Pt (if any)
      const double intervalSize = fabs(jets[0].rapidity()-jets[1].rapidity());
      const double ptBar = (jets[0].pT()+jets[1].pT())/2.0;

      const double minY = min(jets[0].rapidity(), jets[1].rapidity());
      const double maxY = max(jets[0].rapidity(), jets[1].rapidity());

      double vetoPt = 0.0*GeV;
      for (size_t j = 2; j < jets.size(); j++) {
        if (inRange(jets[j].rapidity(), minY, maxY)) vetoPt = max(jets[j].pT(), vetoPt);
      }

      // Fill the gap fraction vs delta Y histograms
      plots._h_gapVsDeltaYInc.fill(ptBar/GeV, intervalSize, weight);
      if (vetoPt < vetoPtThreshold) {
        plots._h_gapVsDeltaYVeto.fill(ptBar/GeV, intervalSize, weight);
      }

      // Fill the gap fraction vs ptBar histograms
      plots._h_gapVsPtBarInc.fill(intervalSize, ptBar/GeV,  weight);
      if (vetoPt < vetoPtThreshold) {
        plots._h_gapVsPtBarVeto.fill(intervalSize, ptBar/GeV, weight);
      }

      // Count the number of veto jets present
      int vetoJetsCount = 0;
      for (size_t j = 2; j < jets.size(); j++) {
        if (inRange(jets[j].rapidity(), minY, maxY) && jets[j].pT() > vetoPtThreshold) {
          vetoJetsCount += 1;
        }
      }

      // Fill the avg NJet, deltaY slices
      if (!plots._avgNJetPtBarSlices.empty()) {
      for (size_t i = 0; i < plots._avgNJetPtBarSlices.size()-1; i++) {
        if (inRange(intervalSize, plots._avgNJetPtBarSlices[i], plots._avgNJetPtBarSlices[i+1])) {
          plots._p_avgJetVsPtBar[i]->fill(ptBar/GeV, vetoJetsCount, weight);
        }
      }
      }

      // Fill the avg NJet, ptBar slices
      if (!plots._avgNJetDeltaYSlices.empty()) {
      for (size_t i = 0; i < plots._avgNJetDeltaYSlices.size()-1; i++) {
        if (inRange(ptBar/GeV, plots._avgNJetDeltaYSlices[i], plots._avgNJetDeltaYSlices[i+1])) {
          plots._p_avgJetVsDeltaY[i]->fill(intervalSize, vetoJetsCount, weight);
        }
      }
      }

      // Fill the veto pt plots
      int q0PlotCount = 0;
      for (size_t x = 0; x < plots._gapFractionQ0SlicesPtBar.size()/2; x++) {
        for (size_t y = 0; y < plots._gapFractionQ0SlicesDeltaY.size()/2; y++) {
          // Check if it should be filled
          if ( ptBar/GeV < plots._gapFractionQ0SlicesPtBar[x*2] ||
               ptBar/GeV >= plots._gapFractionQ0SlicesPtBar[x*2+1] ) {
            q0PlotCount++;
            continue;
          }

          if ( intervalSize < plots._gapFractionQ0SlicesDeltaY[y*2] ||
               intervalSize >= plots._gapFractionQ0SlicesDeltaY[y*2+1] ) {
            q0PlotCount++;
            continue;
          }

          plots._h_vetoPt[q0PlotCount]->fill(vetoPt, weight);
          plots._vetoPtTotalSum[q0PlotCount] += weight;

          q0PlotCount++;
        }
      }
    }


    /// Derive final distributions for each selection
    void finalize() {
      foreach (const ATLAS_2011_S9126244_Plots& plots, _selectionPlots) {

        /// @todo Clean up temp histos -- requires restructuring the temp histo struct

        for (size_t x = 0; x < plots._h_gapVsDeltaYVeto.getHistograms().size(); x++) {
          divide(plots._h_gapVsDeltaYVeto.getHistograms()[x], plots._h_gapVsDeltaYInc.getHistograms()[x],
                 bookScatter2D(plots._gapFractionDeltaYHistIndex+x, 1, plots.selectionType));
        }
        for (size_t x = 0; x < plots._h_gapVsPtBarVeto.getHistograms().size(); x++) {
          divide(plots._h_gapVsPtBarVeto.getHistograms()[x], plots._h_gapVsPtBarInc.getHistograms()[x],
                 bookScatter2D(plots._gapFractionPtBarHistIndex+x, 1, plots.selectionType));
        }

        for (size_t h = 0; h < plots._d_vetoPtGapFraction.size(); h++) {
          finalizeQ0GapFraction(plots._vetoPtTotalSum[h], plots._d_vetoPtGapFraction[h], plots._h_vetoPt[h]);
        }
      }
    }


    /// Convert the differential histograms to an integral histo and assign binomial errors as a efficiency
    /// @todo Should be convertible to a YODA ~one-liner using toIntegralEfficiencyHisto
    void finalizeQ0GapFraction(double totalWeightSum, Scatter2DPtr gapFractionDP, Histo1DPtr vetoPtHist) {
      for (size_t i = 0; i < vetoPtHist->numBins(); ++i) {
        const double vetoPtWeightSum = vetoPtHist->integral(i); //< Integral (with underflow) up to but not including bin i
        // Calculate the efficiency & binomial uncertainty
        const double eff = (totalWeightSum != 0) ? vetoPtWeightSum/totalWeightSum : 0;
        const double effErr = (totalWeightSum != 0) ? sqrt( eff*(1.0-eff)/totalWeightSum ) : 0;
        gapFractionDP->addPoint(eff, effErr);
      }
    }


  private:

    // Struct containing the complete set of plots, times 3 for the different selections
    ATLAS_2011_S9126244_Plots _selectionPlots[3];

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9126244);

}

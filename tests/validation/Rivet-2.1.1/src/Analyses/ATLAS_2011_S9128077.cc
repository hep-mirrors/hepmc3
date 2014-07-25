// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  class ATLAS_2011_S9128077 : public Analysis {
  public:

    /// Constructor
    ATLAS_2011_S9128077()
      : Analysis("ATLAS_2011_S9128077")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Projections
      const FinalState fs;
      FastJets j4(fs, FastJets::ANTIKT, 0.4);
      j4.useInvisibles();
      addProjection(j4, "AntiKtJets04");
      FastJets j6(fs, FastJets::ANTIKT, 0.6);
      j6.useInvisibles();
      addProjection(j6, "AntiKtJets06");

      // Persistent histograms
      _h_jet_multi_inclusive = bookHisto1D(1, 1, 1);
      _h_jet_multi_ratio = bookScatter2D(2, 1, 1);
      _h_jet_pT.resize(4);
      _h_jet_pT[0] = bookHisto1D(3, 1, 1);
      _h_jet_pT[1] = bookHisto1D(4, 1, 1);
      _h_jet_pT[2] = bookHisto1D(5, 1, 1);
      _h_jet_pT[3] = bookHisto1D(6, 1, 1);
      _h_HT_2 = bookHisto1D(7, 1, 1);
      _h_HT_3 = bookHisto1D(8, 1, 1);
      _h_HT_4 = bookHisto1D(9, 1, 1);
      //
      _h_pTlead_R06_60_ratio = bookScatter2D(10, 1, 1);
      _h_pTlead_R06_80_ratio = bookScatter2D(11, 1, 1);
      _h_pTlead_R06_110_ratio = bookScatter2D(12, 1, 1);
      _h_pTlead_R04_60_ratio = bookScatter2D(13, 1, 1);
      _h_pTlead_R04_80_ratio = bookScatter2D(14, 1, 1);
      _h_pTlead_R04_110_ratio = bookScatter2D(15, 1, 1);
      _h_HT2_R06_ratio = bookScatter2D(16, 1, 1);
      _h_HT2_R04_ratio = bookScatter2D(17, 1, 1);

      // Temporary histograms to be divided for the dsigma3/dsigma2 ratios
      _h_tmp_pTlead_R06_60_2  = Histo1D(refData(10, 1, 1));
      _h_tmp_pTlead_R06_80_2  = Histo1D(refData(11, 1, 1));
      _h_tmp_pTlead_R06_110_2 = Histo1D(refData(12, 1, 1));
      _h_tmp_pTlead_R06_60_3  = Histo1D(refData(10, 1, 1));
      _h_tmp_pTlead_R06_80_3  = Histo1D(refData(11, 1, 1));
      _h_tmp_pTlead_R06_110_3 = Histo1D(refData(12, 1, 1));
      //
      _h_tmp_pTlead_R04_60_2  = Histo1D(refData(13, 1, 1));
      _h_tmp_pTlead_R04_80_2  = Histo1D(refData(14, 1, 1));
      _h_tmp_pTlead_R04_110_2 = Histo1D(refData(15, 1, 1));
      _h_tmp_pTlead_R04_60_3  = Histo1D(refData(13, 1, 1));
      _h_tmp_pTlead_R04_80_3  = Histo1D(refData(14, 1, 1));
      _h_tmp_pTlead_R04_110_3 = Histo1D(refData(15, 1, 1));
      //
      _h_tmp_HT2_R06_2 = Histo1D(refData(16, 1, 1));
      _h_tmp_HT2_R06_3 = Histo1D(refData(16, 1, 1));
      _h_tmp_HT2_R04_2 = Histo1D(refData(17, 1, 1));
      _h_tmp_HT2_R04_3 = Histo1D(refData(17, 1, 1));
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      vector<FourMomentum> jets04;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(60.0*GeV)) {
        if (fabs(jet.eta()) < 2.8) {
          jets04.push_back(jet.momentum());
        }
      }

      if (jets04.size() > 1 && jets04[0].pT() > 80.0*GeV) {
        for (size_t i = 2; i <= jets04.size(); ++i) {
          _h_jet_multi_inclusive->fill(i, weight);
        }

        double HT = 0.0;
        for (size_t i = 0; i < jets04.size(); ++i) {
          if (i < _h_jet_pT.size()) _h_jet_pT[i]->fill(jets04[i].pT(), weight);
          HT += jets04[i].pT();
        }

        if (jets04.size() >= 2) _h_HT_2->fill(HT, weight);
        if (jets04.size() >= 3) _h_HT_3->fill(HT, weight);
        if (jets04.size() >= 4) _h_HT_4->fill(HT, weight);

        double pT1(jets04[0].pT()), pT2(jets04[1].pT());
        double HT2 = pT1 + pT2;
        if (jets04.size() >= 2) {
          _h_tmp_HT2_R04_2.fill(HT2, weight);
          _h_tmp_pTlead_R04_60_2.fill(pT1, weight);
          if (pT2 > 80.0*GeV) _h_tmp_pTlead_R04_80_2.fill(pT1, weight);
          if (pT2 > 110.0*GeV) _h_tmp_pTlead_R04_110_2.fill(pT1, weight);
        }
        if (jets04.size() >= 3) {
          double pT3(jets04[2].pT());
          _h_tmp_HT2_R04_3.fill(HT2, weight);
          _h_tmp_pTlead_R04_60_3.fill(pT1, weight);
          if (pT3 > 80.0*GeV) _h_tmp_pTlead_R04_80_3.fill(pT1, weight);
          if (pT3 > 110.0*GeV) _h_tmp_pTlead_R04_110_3.fill(pT1, weight);
        }
      }

      /// @todo It'd be better to avoid duplicating 95% of the code!
      vector<FourMomentum> jets06;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "AntiKtJets06").jetsByPt(60.0*GeV)) {
        if (fabs(jet.eta()) < 2.8) {
          jets06.push_back(jet.momentum());
        }
      }
      if (jets06.size() > 1 && jets06[0].pT() > 80.0*GeV) {
        double pT1(jets06[0].pT()), pT2(jets06[1].pT());
        double HT2 = pT1 + pT2;
        if (jets06.size() >= 2) {
          _h_tmp_HT2_R06_2.fill(HT2, weight);
          _h_tmp_pTlead_R06_60_2.fill(pT1, weight);
          if (pT2 > 80.0*GeV) _h_tmp_pTlead_R06_80_2.fill(pT1, weight);
          if (pT2 > 110.0*GeV) _h_tmp_pTlead_R06_110_2.fill(pT1, weight);
        }
        if (jets06.size() >= 3) {
          double pT3(jets06[2].pT());
          _h_tmp_HT2_R06_3.fill(HT2, weight);
          _h_tmp_pTlead_R06_60_3.fill(pT1, weight);
          if (pT3 > 80.0*GeV) _h_tmp_pTlead_R06_80_3.fill(pT1, weight);
          if (pT3 > 110.0*GeV) _h_tmp_pTlead_R06_110_3.fill(pT1, weight);
        }
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      // Fill inclusive jet multiplicity ratio
      Histo1D temphisto(refData(2, 1, 1));
      for (size_t b = 0; b < temphisto.numBins(); ++b) {
        if (_h_jet_multi_inclusive->bin(b).sumW() != 0) {
          const double x   = temphisto.bin(b).midpoint();
          const double ex  = temphisto.bin(b).width()/2.;
          const double val = _h_jet_multi_inclusive->bin(b+1).sumW() / _h_jet_multi_inclusive->bin(b).sumW();
          const double err = ( _h_jet_multi_inclusive->bin(b+1).relErr() + _h_jet_multi_inclusive->bin(b).relErr() ) * val;
          _h_jet_multi_ratio->addPoint(x, val, ex, err);
        }
      }

      // Normalize std histos
      scale(_h_jet_multi_inclusive, crossSectionPerEvent());
      scale(_h_jet_pT[0], crossSectionPerEvent());
      scale(_h_jet_pT[1], crossSectionPerEvent());
      scale(_h_jet_pT[2], crossSectionPerEvent());
      scale(_h_jet_pT[3], crossSectionPerEvent());
      scale(_h_HT_2, crossSectionPerEvent());
      scale(_h_HT_3, crossSectionPerEvent());
      scale(_h_HT_4, crossSectionPerEvent());

      /// Create ratio histograms
      divide(_h_tmp_pTlead_R06_60_3,_h_tmp_pTlead_R06_60_2, _h_pTlead_R06_60_ratio);
      divide(_h_tmp_pTlead_R06_80_3,_h_tmp_pTlead_R06_80_2, _h_pTlead_R06_80_ratio);
      divide(_h_tmp_pTlead_R06_110_3,_h_tmp_pTlead_R06_110_2, _h_pTlead_R06_110_ratio);
      divide(_h_tmp_pTlead_R04_60_3,_h_tmp_pTlead_R04_60_2, _h_pTlead_R04_60_ratio);
      divide(_h_tmp_pTlead_R04_80_3,_h_tmp_pTlead_R04_80_2, _h_pTlead_R04_80_ratio);
      divide(_h_tmp_pTlead_R04_110_3,_h_tmp_pTlead_R04_110_2, _h_pTlead_R04_110_ratio);
      divide(_h_tmp_HT2_R06_3,_h_tmp_HT2_R06_2, _h_HT2_R06_ratio);
      divide(_h_tmp_HT2_R04_3,_h_tmp_HT2_R04_2, _h_HT2_R04_ratio);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_jet_multi_inclusive;
    Scatter2DPtr _h_jet_multi_ratio;
    vector<Histo1DPtr> _h_jet_pT;
    Histo1DPtr _h_HT_2;
    Histo1DPtr _h_HT_3;
    Histo1DPtr _h_HT_4;
    //@}

    /// @name Ratio histograms
    //@{
    Scatter2DPtr _h_pTlead_R06_60_ratio, _h_pTlead_R06_80_ratio, _h_pTlead_R06_110_ratio;
    Scatter2DPtr _h_pTlead_R04_60_ratio, _h_pTlead_R04_80_ratio, _h_pTlead_R04_110_ratio;
    Scatter2DPtr _h_HT2_R06_ratio, _h_HT2_R04_ratio;
    //@}

    /// @name Temporary histograms to be divided for the dsigma3/dsigma2 ratios
    //@{
    Histo1D _h_tmp_pTlead_R06_60_2, _h_tmp_pTlead_R06_80_2, _h_tmp_pTlead_R06_110_2;
    Histo1D _h_tmp_pTlead_R06_60_3, _h_tmp_pTlead_R06_80_3, _h_tmp_pTlead_R06_110_3;
    Histo1D _h_tmp_pTlead_R04_60_2, _h_tmp_pTlead_R04_80_2, _h_tmp_pTlead_R04_110_2;
    Histo1D _h_tmp_pTlead_R04_60_3, _h_tmp_pTlead_R04_80_3, _h_tmp_pTlead_R04_110_3;
    Histo1D _h_tmp_HT2_R06_2, _h_tmp_HT2_R06_3, _h_tmp_HT2_R04_2, _h_tmp_HT2_R04_3;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9128077);


}

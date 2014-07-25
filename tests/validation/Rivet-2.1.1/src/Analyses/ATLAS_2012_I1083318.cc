// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"

namespace Rivet {


  /// ATLAS W + jets production at 7 TeV
  class ATLAS_2012_I1083318 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I1083318()
      : Analysis("ATLAS_2012_I1083318")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      FinalState fs;
      IdentifiedFinalState allleptons;
      allleptons.acceptIdPair(PID::ELECTRON);
      allleptons.acceptIdPair(PID::MUON);
      vector< pair<double, double> > etaRanges;
      etaRanges.push_back(make_pair(-2.5, 2.5));
      DressedLeptons leptons(fs, allleptons, 0.1, true, etaRanges, 20.0*GeV);
      addProjection(leptons, "leptons");

      // Leading neutrinos for Etmiss
      LeadingParticlesFinalState neutrinos(fs);
      neutrinos.addParticleIdPair(PID::NU_E);
      neutrinos.addParticleIdPair(PID::NU_MU);
      neutrinos.setLeadingOnly(true);
      addProjection(neutrinos, "neutrinos");

      // Input for the jets: "Neutrinos, electrons, and muons from decays of the
      // massive W boson were not used"
      VetoedFinalState veto;
      veto.addVetoOnThisFinalState(leptons);
      veto.addVetoOnThisFinalState(neutrinos);
      FastJets jets(veto, FastJets::ANTIKT, 0.4);
      jets.useInvisibles(true);
      addProjection(jets, "jets");

      for (size_t i = 0; i < 2; ++i) {
        _h_NjetIncl[i] = bookHisto1D(1, 1, i+1);
        _h_RatioNjetIncl[i] = bookScatter2D(2, 1, i+1);
        _h_FirstJetPt_1jet[i] = bookHisto1D(3, 1, i+1);
        _h_FirstJetPt_2jet[i] = bookHisto1D(4, 1, i+1);
        _h_FirstJetPt_3jet[i] = bookHisto1D(5, 1, i+1);
        _h_FirstJetPt_4jet[i] = bookHisto1D(6, 1, i+1);
        _h_SecondJetPt_2jet[i] = bookHisto1D(7, 1, i+1);
        _h_SecondJetPt_3jet[i] = bookHisto1D(8, 1, i+1);
        _h_SecondJetPt_4jet[i] = bookHisto1D(9, 1, i+1);
        _h_ThirdJetPt_3jet[i] = bookHisto1D(10, 1, i+1);
        _h_ThirdJetPt_4jet[i] = bookHisto1D(11, 1, i+1);
        _h_FourthJetPt_4jet[i] = bookHisto1D(12, 1, i+1);
        _h_Ht_1jet[i] = bookHisto1D(13, 1, i+1);
        _h_Ht_2jet[i] = bookHisto1D(14, 1, i+1);
        _h_Ht_3jet[i] = bookHisto1D(15, 1, i+1);
        _h_Ht_4jet[i] = bookHisto1D(16, 1, i+1);
        _h_Minv_2jet[i] = bookHisto1D(17, 1, i+1);
        _h_Minv_3jet[i] = bookHisto1D(18, 1, i+1);
        _h_Minv_4jet[i] = bookHisto1D(19, 1, i+1);
        _h_JetRapidity[i] = bookHisto1D(20, 1, i+1);
        _h_DeltaYElecJet[i] = bookHisto1D(21, 1, i+1);
        _h_SumYElecJet[i] = bookHisto1D(22, 1, i+1);
        _h_DeltaR_2jet[i] = bookHisto1D(23, 1, i+1);
        _h_DeltaY_2jet[i] = bookHisto1D(24, 1, i+1);
        _h_DeltaPhi_2jet[i] = bookHisto1D(25, 1, i+1);
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const vector<ClusteredLepton>& leptons = applyProjection<DressedLeptons>(event, "leptons").clusteredLeptons();
      Particles neutrinos = applyProjection<FinalState>(event, "neutrinos").particlesByPt();

      if (leptons.size() != 1 || (neutrinos.size() == 0)) {
        vetoEvent;
      }

      FourMomentum lepton = leptons[0].momentum();
      FourMomentum p_miss = neutrinos[0].momentum();
      if (p_miss.Et() < 25.0*GeV) {
        vetoEvent;
      }

      double mT = sqrt(2.0 * lepton.pT() * p_miss.Et() * (1.0 - cos( lepton.phi()-p_miss.phi()) ) );
      if (mT < 40.0*GeV) {
        vetoEvent;
      }

      double jetcuts[] = { 30.0*GeV, 20.0*GeV };
      const FastJets& jetpro = applyProjection<FastJets>(event, "jets");

      for (size_t i = 0; i < 2; ++i) {
        vector<FourMomentum> jets;
        double HT = lepton.pT() + p_miss.pT();
        foreach (const Jet& jet, jetpro.jetsByPt(jetcuts[i])) {
          if (fabs(jet.rapidity()) < 4.4 && deltaR(lepton, jet.momentum()) > 0.5) {
            jets.push_back(jet.momentum());
            HT += jet.pT();
          }
        }

        _h_NjetIncl[i]->fill(0.0, weight);

        // Njet>=1 observables
        if (jets.size() < 1) continue;
        _h_NjetIncl[i]->fill(1.0, weight);
        _h_FirstJetPt_1jet[i]->fill(jets[0].pT(), weight);
        _h_JetRapidity[i]->fill(jets[0].rapidity(), weight);
        _h_Ht_1jet[i]->fill(HT, weight);
        _h_DeltaYElecJet[i]->fill(lepton.rapidity()-jets[0].rapidity(), weight);
        _h_SumYElecJet[i]->fill(lepton.rapidity()+jets[0].rapidity(), weight);

        // Njet>=2 observables
        if (jets.size() < 2) continue;
        _h_NjetIncl[i]->fill(2.0, weight);
        _h_FirstJetPt_2jet[i]->fill(jets[0].pT(), weight);
        _h_SecondJetPt_2jet[i]->fill(jets[1].pT(), weight);
        _h_Ht_2jet[i]->fill(HT, weight);
        double m2_2jet = FourMomentum(jets[0]+jets[1]).mass2();
        _h_Minv_2jet[i]->fill(m2_2jet>0.0 ? sqrt(m2_2jet) : 0.0, weight);
        _h_DeltaR_2jet[i]->fill(deltaR(jets[0], jets[1]), weight);
        _h_DeltaY_2jet[i]->fill(jets[0].rapidity()-jets[1].rapidity(), weight);
        _h_DeltaPhi_2jet[i]->fill(deltaPhi(jets[0], jets[1]), weight);

        // Njet>=3 observables
        if (jets.size() < 3) continue;
        _h_NjetIncl[i]->fill(3.0, weight);
        _h_FirstJetPt_3jet[i]->fill(jets[0].pT(), weight);
        _h_SecondJetPt_3jet[i]->fill(jets[1].pT(), weight);
        _h_ThirdJetPt_3jet[i]->fill(jets[2].pT(), weight);
        _h_Ht_3jet[i]->fill(HT, weight);
        double m2_3jet = FourMomentum(jets[0]+jets[1]+jets[2]).mass2();
        _h_Minv_3jet[i]->fill(m2_3jet>0.0 ? sqrt(m2_3jet) : 0.0, weight);

        // Njet>=4 observables
        if (jets.size() < 4) continue;
        _h_NjetIncl[i]->fill(4.0, weight);
        _h_FirstJetPt_4jet[i]->fill(jets[0].pT(), weight);
        _h_SecondJetPt_4jet[i]->fill(jets[1].pT(), weight);
        _h_ThirdJetPt_4jet[i]->fill(jets[2].pT(), weight);
        _h_FourthJetPt_4jet[i]->fill(jets[3].pT(), weight);
        _h_Ht_4jet[i]->fill(HT, weight);
        double m2_4jet = FourMomentum(jets[0]+jets[1]+jets[2]+jets[3]).mass2();
        _h_Minv_4jet[i]->fill(m2_4jet>0.0 ? sqrt(m2_4jet) : 0.0, weight);

        // Njet>=5 observables
        if (jets.size() < 5) continue;
        _h_NjetIncl[i]->fill(5.0, weight);
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      for (size_t i = 0; i < 2; ++i) {

        // Construct jet multiplicity ratio
        for (size_t n = 1; n < _h_NjetIncl[i]->numBins(); ++n) {
          YODA::HistoBin1D& b0 = _h_NjetIncl[i]->bin(n-1);
          YODA::HistoBin1D& b1 = _h_NjetIncl[i]->bin(n);
          if (b0.height() == 0.0 || b1.height() == 0.0) continue;
          _h_RatioNjetIncl[i]->addPoint(n, b1.height()/b0.height(), 0,
                                        b1.height()/b0.height() * (b0.relErr() + b1.relErr()));
        }

        // Scale all histos to the cross section
        const double factor = crossSection()/sumOfWeights();
        scale(_h_DeltaPhi_2jet[i], factor);
        scale(_h_DeltaR_2jet[i], factor);
        scale(_h_DeltaY_2jet[i], factor);
        scale(_h_DeltaYElecJet[i], factor);
        scale(_h_FirstJetPt_1jet[i], factor);
        scale(_h_FirstJetPt_2jet[i], factor);
        scale(_h_FirstJetPt_3jet[i], factor);
        scale(_h_FirstJetPt_4jet[i], factor);
        scale(_h_FourthJetPt_4jet[i], factor);
        scale(_h_Ht_1jet[i], factor);
        scale(_h_Ht_2jet[i], factor);
        scale(_h_Ht_3jet[i], factor);
        scale(_h_Ht_4jet[i], factor);
        scale(_h_JetRapidity[i], factor);
        scale(_h_Minv_2jet[i], factor);
        scale(_h_Minv_3jet[i], factor);
        scale(_h_Minv_4jet[i], factor);
        scale(_h_NjetIncl[i], factor);
        scale(_h_SecondJetPt_2jet[i], factor);
        scale(_h_SecondJetPt_3jet[i], factor);
        scale(_h_SecondJetPt_4jet[i], factor);
        scale(_h_SumYElecJet[i], factor);
        scale(_h_ThirdJetPt_3jet[i], factor);
        scale(_h_ThirdJetPt_4jet[i], factor);
      }
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_DeltaPhi_2jet[2];
    Histo1DPtr _h_DeltaR_2jet[2];
    Histo1DPtr _h_DeltaY_2jet[2];
    Histo1DPtr _h_DeltaYElecJet[2];
    Histo1DPtr _h_FirstJetPt_1jet[2];
    Histo1DPtr _h_FirstJetPt_2jet[2];
    Histo1DPtr _h_FirstJetPt_3jet[2];
    Histo1DPtr _h_FirstJetPt_4jet[2];
    Histo1DPtr _h_FourthJetPt_4jet[2];
    Histo1DPtr _h_Ht_1jet[2];
    Histo1DPtr _h_Ht_2jet[2];
    Histo1DPtr _h_Ht_3jet[2];
    Histo1DPtr _h_Ht_4jet[2];
    Histo1DPtr _h_JetRapidity[2];
    Histo1DPtr _h_Minv_2jet[2];
    Histo1DPtr _h_Minv_3jet[2];
    Histo1DPtr _h_Minv_4jet[2];
    Histo1DPtr _h_NjetIncl[2];
    Scatter2DPtr _h_RatioNjetIncl[2];
    Histo1DPtr _h_SecondJetPt_2jet[2];
    Histo1DPtr _h_SecondJetPt_3jet[2];
    Histo1DPtr _h_SecondJetPt_4jet[2];
    Histo1DPtr _h_SumYElecJet[2];
    Histo1DPtr _h_ThirdJetPt_3jet[2];
    Histo1DPtr _h_ThirdJetPt_4jet[2];
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1083318);

}

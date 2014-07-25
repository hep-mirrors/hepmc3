// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// CMS 4-jet production at 7 TeV
  class CMS_2013_I1273574 : public Analysis {
  public:

    /// Constructor
    CMS_2013_I1273574()
      : Analysis("CMS_2013_I1273574")
    {    }


    /// Book histograms and initialise projections before the run
    void init() {
      const FinalState cnfs(-4.7, 4.7);
      addProjection(FastJets(cnfs, FastJets::ANTIKT, 0.5), "Jets");

      _h_jetetas[0]     = bookHisto1D(1,1,1);
      _h_jetetas[0]     = bookHisto1D(2,1,1);
      _h_DeltaS         = bookHisto1D(3,1,1);
      _h_DeltaPhiSoft   = bookHisto1D(4,1,1);
      _h_DeltaPtRelSoft = bookHisto1D(5,1,1);
      _h_jetetas[2]     = bookHisto1D(6,1,1);
      _h_jetpts[2]      = bookHisto1D(7,1,1);
      _h_jetetas[3]     = bookHisto1D(8,1,1);
      _h_jetpts[3]      = bookHisto1D(9,1,1);
      _h_jetetas[1]     = bookHisto1D(10,1,1);
      _h_jetpts[1]      = bookHisto1D(11,1,1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      /// @todo Use jetsByPt(ptGtr(20*GeV) & absetaIn(4.7)), then no need for the lower loop;
      const Jets jets = applyProjection<FastJets>(event, "Jets").jetsByPt(20*GeV);
      if (jets.size() < 4) vetoEvent;

      // Ensure that there are exactly 4 jets > 20 GeV, with two above 50 GeV
      Jets hardjets, alljets;
      foreach (const Jet& j, jets) {
        if (j.abseta() > 4.7) continue;
        if (j.pT() > 50*GeV) hardjets.push_back(j);
        if (j.pT() > 20*GeV) alljets.push_back(j);
      }
      if (hardjets.size() < 2 || alljets.size() != 4) vetoEvent;
      const double weight = event.weight();

      // Histogram pT and eta of all 4 jets
      for (size_t i = 0; i < 4; ++i) {
        _h_jetpts[i]->fill(alljets[i].pT()/GeV, weight);
        _h_jetetas[i]->fill(alljets[i].eta(), weight);
      }

      // Create vector sums of the hard and soft pairs of jets
      const FourMomentum p12 = alljets[0].momentum() + alljets[1].momentum();
      const FourMomentum p34 = alljets[2].momentum() + alljets[3].momentum();

      // Fill the delta(phi) between the soft jets
      const double dphisoft = deltaPhi(alljets[2], alljets[3]);
      _h_DeltaPhiSoft->fill(dphisoft, weight);

      // Fill the pT balance between the soft jets
      const double ptbalanceSoft = p34.pT() / (alljets[2].pT() + alljets[3].pT());
      _h_DeltaPtRelSoft->fill(ptbalanceSoft, weight);

      // Fill the azimuthal angle difference between the two jet pairs
      const double p12p34_trans = p12.px()*p34.px() + p12.py()*p34.py();
      const double DeltaS = acos( p12p34_trans / p12.pT() / p34.pT() );
      _h_DeltaS->fill(DeltaS, weight);
    }


    /// Normalise histograms (mostly to cross-section)
    void finalize() {
      const double invlumi = crossSection()/picobarn/sumOfWeights();
      for (size_t i = 0; i < 4; ++i) {
        scale(_h_jetpts[i], invlumi);
        scale(_h_jetetas[i], invlumi);
      }
      normalize(_h_DeltaPtRelSoft);
      normalize(_h_DeltaPhiSoft);
      normalize(_h_DeltaS);
    }


  private:

    Histo1DPtr _h_jetpts[4], _h_jetetas[4];
    Histo1DPtr _h_DeltaS, _h_DeltaPhiSoft, _h_DeltaPtRelSoft;

  };


  DECLARE_RIVET_PLUGIN(CMS_2013_I1273574);

}

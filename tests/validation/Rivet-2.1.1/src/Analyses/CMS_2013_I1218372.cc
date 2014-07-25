// Samantha Dooling DESY
// February 2012
//
// -*- C++ -*-
// =============================
//
// Ratio of the energy deposited in the pseudorapditiy range
// -6.6 < eta < -5.2 for events with a charged particle jet
//
// =============================
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  class CMS_2013_I1218372 : public Analysis {
  public:

  /// Constructor
  CMS_2013_I1218372()
    : Analysis("CMS_2013_I1218372")
    { }

    void init() {

      // gives the range of eta and min pT for the final state from which I get the jets
      FastJets jetpro (ChargedFinalState(-2.5, 2.5, 0.3*GeV), FastJets::ANTIKT, 0.5);
      addProjection(jetpro, "Jets");

      // skip Neutrinos and Muons
      VetoedFinalState fsv(FinalState(-7.0, -4.0, 0.*GeV));
      fsv.vetoNeutrinos();
      fsv.addVetoPairId(PID::MUON);
      addProjection(fsv, "fsv");

      // for the hadron level selection
      VetoedFinalState sfsv(FinalState(-MAXDOUBLE, MAXDOUBLE, 0.*GeV));
      sfsv.vetoNeutrinos();
      sfsv.addVetoPairId(PID::MUON);
      addProjection(sfsv, "sfsv");

      //counters
      passedSumOfWeights = 0.;
      inclEflow = 0.;

      // Temporary histograms to fill the energy flow for leading jet events.
      // Ratios are calculated in finalyze().
      int id = 0;
      if (fuzzyEquals(sqrtS()/GeV,  900, 1e-3)) id=1;
      if (fuzzyEquals(sqrtS()/GeV, 2760, 1e-3)) id=2;
      if (fuzzyEquals(sqrtS()/GeV, 7000, 1e-3)) id=3;
      _h_ratio  = bookScatter2D(id, 1, 1);
      _tmp_jet  = bookHisto1D  (id, 1, 1, "eflow_jet");  // Leading jet energy flow in pt
      _tmp_njet = bookHisto1D  (id, 1, 1, "number_jet"); // Number of events in pt
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

      // Skip if the event is empty
      const FinalState& fsv = applyProjection<FinalState>(event, "fsv");
      if (fsv.empty()) vetoEvent;

      // ====================== Minimum Bias selection

      const FinalState& sfsv = applyProjection<FinalState>(event, "sfsv");
      Particles parts = sfsv.particlesByRapidity();
      if (parts.empty()) vetoEvent;

      // find dymax
      double dymax = 0;
      int gap_pos  = -1;
      for (size_t i=0; i < parts.size()-1; ++i) {
        double dy = parts[i+1].rapidity() - parts[i].rapidity();
        if (dy > dymax) {
          dymax     = dy;
          gap_pos = i;
        }
      }

      // calculate mx2 and my2
      FourMomentum xmom;
      for (int i=0; i<=gap_pos; ++i) {
        xmom += parts[i].momentum();
      }
      double mx2 = xmom.mass2();
      if (mx2<0) vetoEvent;

      FourMomentum ymom;
      for (size_t i=gap_pos+1; i<parts.size(); ++i) {
        ymom += parts[i].momentum();
      }
      double my2 = ymom.mass2();
      if (my2<0) vetoEvent;

      // calculate xix and xiy and xidd
      double xix  = mx2 / sqr(sqrtS());
      double xiy  = my2 / sqr(sqrtS());
      double xidd = mx2*my2 / sqr(sqrtS()*0.938*GeV);

      // combine the selection: xi cuts
      bool passedHadronCuts = false;
      if (fuzzyEquals(sqrtS()/GeV,  900, 1e-3) && (xix > 0.1  || xiy > 0.4 || xidd > 0.5)) passedHadronCuts = true;
      if (fuzzyEquals(sqrtS()/GeV, 2760, 1e-3) && (xix > 0.07 || xiy > 0.2 || xidd > 0.5)) passedHadronCuts = true;
      if (fuzzyEquals(sqrtS()/GeV, 7000, 1e-3) && (xix > 0.04 || xiy > 0.1 || xidd > 0.5)) passedHadronCuts = true;
      if (!passedHadronCuts) vetoEvent;

      //  ============================== MINIMUM BIAS EVENTS

      // loop over particles to calculate the energy
      passedSumOfWeights += weight;

      foreach (const Particle& p, fsv.particles()) {
        if (-5.2 > p.eta() && p.eta() > -6.6) inclEflow += weight*p.momentum().E()/GeV;
      }

      //  ============================== JET EVENTS

      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      const Jets& jets = jetpro.jetsByPt(1.0*GeV);
      if (jets.size()<1) vetoEvent;

      if (fabs(jets[0].eta()) < 2.0) {
        _tmp_njet->fill(jets[0].pT()/GeV, weight);

        // energy flow
        foreach (const Particle& p, fsv.particles()) {
          if (p.eta() > -6.6 && p.eta() < -5.2) {  // ask for the CASTOR region
            _tmp_jet->fill(jets[0].pT()/GeV, weight * p.momentum().E()/GeV);
          }
        }
      }

    }// analysis

    void finalize() {
      scale(_tmp_jet, passedSumOfWeights/inclEflow);
      divide(_tmp_jet, _tmp_njet, _h_ratio);
    }

  private:
    // counters
    double passedSumOfWeights;
    double inclEflow;

    // histograms
    Scatter2DPtr _h_ratio;
    Histo1DPtr   _tmp_jet;
    Histo1DPtr   _tmp_njet;
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2013_I1218372);

}

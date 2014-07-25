// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

namespace Rivet {


  class CMS_2013_I1224539_WJET : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    CMS_2013_I1224539_WJET()
      : Analysis("CMS_2013_I1224539_WJET"),
        _filter(fastjet::Filter(fastjet::JetDefinition(fastjet::cambridge_algorithm, 0.3), fastjet::SelectorNHardest(3))),
        _trimmer(fastjet::Filter(fastjet::JetDefinition(fastjet::kt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.03))),
        _pruner(fastjet::Pruner(fastjet::cambridge_algorithm, 0.1, 0.5))
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs(-2.4, 2.4, 0*GeV);
      addProjection(fs, "FS");

      // Find W's with pT > 120, MET > 50
      WFinder wfinder(fs, -2.4, 2.4, 80.0*GeV, PID::ELECTRON, 50*GeV, 1000*GeV, 50.0*GeV,
                      0.2, WFinder::CLUSTERNODECAY, WFinder::NOTRACK, WFinder::TRANSMASS);
      addProjection(wfinder, "WFinder");

      // W+jet jet collections
      addProjection(FastJets(wfinder.remainingFinalState(), FastJets::ANTIKT, 0.7), "JetsAK7_wj");
      addProjection(FastJets(wfinder.remainingFinalState(), FastJets::CAM, 0.8), "JetsCA8_wj");
      addProjection(FastJets(wfinder.remainingFinalState(), FastJets::CAM, 1.2), "JetsCA12_wj");

      // Histograms
      /// @note These are 2D histos rendered into slices
      const int wjetsOffset = 51;
      for (size_t i = 0; i < N_PT_BINS_vj; ++i) {
        _h_ungroomedJetMass_AK7_wj[i] = bookHisto1D(wjetsOffset+i+1+0*N_PT_BINS_vj, 1, 1);
        _h_filteredJetMass_AK7_wj[i] = bookHisto1D(wjetsOffset+i+1+1*N_PT_BINS_vj, 1, 1);
        _h_trimmedJetMass_AK7_wj[i] = bookHisto1D(wjetsOffset+i+1+2*N_PT_BINS_vj, 1, 1);
        _h_prunedJetMass_AK7_wj[i] = bookHisto1D(wjetsOffset+i+1+3*N_PT_BINS_vj, 1, 1);
        _h_prunedJetMass_CA8_wj[i] = bookHisto1D(wjetsOffset+i+1+4*N_PT_BINS_vj, 1, 1);
        if (i > 0) _h_filteredJetMass_CA12_wj[i] = bookHisto1D(wjetsOffset+i+5*N_PT_BINS_vj, 1, 1);
      }
    }


    bool isBackToBack_wj(const WFinder& wf, const fastjet::PseudoJet& psjet) {
      const FourMomentum& w = wf.bosons()[0].momentum();
      const FourMomentum& l1 = wf.constituentLeptons()[0].momentum();
      const FourMomentum& l2 = wf.constituentNeutrinos()[0].momentum();
      /// @todo We should make FourMomentum know how to construct itself from a PseudoJet
      const FourMomentum jmom(psjet.e(), psjet.px(), psjet.py(), psjet.pz());
      return (deltaPhi(w, jmom) > 2.0 && deltaR(l1, jmom) > 1.0 && deltaPhi(l2, jmom) > 0.4);
    }


    // Find the pT histogram bin index for value pt (in GeV), to hack a 2D histogram equivalent
    /// @todo Use a YODA axis/finder alg when available
    size_t findPtBin(double ptJ) {
      const double ptBins_vj[N_PT_BINS_vj+1] = { 125.0, 150.0, 220.0, 300.0, 450.0 };
      for (size_t ibin = 0; ibin < N_PT_BINS_vj; ++ibin) {
        if (inRange(ptJ, ptBins_vj[ibin], ptBins_vj[ibin+1])) return ibin;
      }
      return N_PT_BINS_vj;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Get the W
      const WFinder& wfinder = applyProjection<WFinder>(event, "WFinder");
      if (wfinder.bosons().size() != 1) vetoEvent;
      const Particle& w = wfinder.bosons()[0];
      const Particle& l = wfinder.constituentLeptons()[0];

      // Require a fairly high-pT W and charged lepton
      if (l.momentum().pT() < 80*GeV || w.momentum().pT() < 120*GeV) vetoEvent;

      // Get the pseudojets.
      const PseudoJets& psjetsCA8_wj = applyProjection<FastJets>(event, "JetsCA8_wj").pseudoJetsByPt( 50.0*GeV );
      const PseudoJets& psjetsCA12_wj = applyProjection<FastJets>(event, "JetsCA12_wj").pseudoJetsByPt( 50.0*GeV );

      // AK7 jets
      const PseudoJets& psjetsAK7_wj = applyProjection<FastJets>(event, "JetsAK7_wj").pseudoJetsByPt( 50.0*GeV );
      if (!psjetsAK7_wj.empty()) {
        // Get the leading jet and make sure it's back-to-back with the W
        const fastjet::PseudoJet& j0 = psjetsAK7_wj[0];
        if (isBackToBack_wj(wfinder, j0)) {
          const size_t njetBin = findPtBin(j0.pt()/GeV);
          if (njetBin < N_PT_BINS_vj) {
            fastjet::PseudoJet filtered0 = _filter(j0);
            fastjet::PseudoJet trimmed0 = _trimmer(j0);
            fastjet::PseudoJet pruned0 = _pruner(j0);
            _h_ungroomedJetMass_AK7_wj[njetBin]->fill(j0.m()/GeV, weight);
            _h_filteredJetMass_AK7_wj[njetBin]->fill(filtered0.m()/GeV, weight);
            _h_trimmedJetMass_AK7_wj[njetBin]->fill(trimmed0.m()/GeV, weight);
            _h_prunedJetMass_AK7_wj[njetBin]->fill(pruned0.m()/GeV, weight);
          }
        }
      }

      // CA8 jets
      if (!psjetsCA8_wj.empty()) {
        // Get the leading jet and make sure it's back-to-back with the W
        const fastjet::PseudoJet& j0 = psjetsCA8_wj[0];
        if (isBackToBack_wj(wfinder, j0)) {
          const size_t njetBin = findPtBin(j0.pt()/GeV);
          if (njetBin < N_PT_BINS_vj) {
            fastjet::PseudoJet pruned0 = _pruner(j0);
            _h_prunedJetMass_CA8_wj[njetBin]->fill(pruned0.m()/GeV, weight);
          }
        }
      }

      // CA12 jets
      if (!psjetsCA12_wj.empty()) {
        // Get the leading jet and make sure it's back-to-back with the W
        const fastjet::PseudoJet& j0 = psjetsCA12_wj[0];
        if (isBackToBack_wj(wfinder, j0)) {
          const size_t njetBin = findPtBin(j0.pt()/GeV);
          if (njetBin < N_PT_BINS_vj&&njetBin>0) {
            fastjet::PseudoJet filtered0 = _filter(j0);
            _h_filteredJetMass_CA12_wj[njetBin]->fill( filtered0.m() / GeV, weight);
          }
        }
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      const double normalizationVal = 1000;
      for (size_t i = 0; i < N_PT_BINS_vj; ++i) {
        normalize(_h_ungroomedJetMass_AK7_wj[i], normalizationVal);
        normalize(_h_filteredJetMass_AK7_wj[i], normalizationVal);
        normalize(_h_trimmedJetMass_AK7_wj[i], normalizationVal);
        normalize(_h_prunedJetMass_AK7_wj[i], normalizationVal);
        normalize(_h_prunedJetMass_CA8_wj[i], normalizationVal);
        if (i > 0) normalize( _h_filteredJetMass_CA12_wj[i], normalizationVal);
      }
    }

    //@}


  private:

    /// @name FastJet grooming tools (configured in constructor init list)
    //@{
    const fastjet::Filter _filter;
    const fastjet::Filter _trimmer;
    const fastjet::Pruner _pruner;
    //@}


    /// @name Histograms
    //@{
    enum { PT_125_150_vj=0, PT_150_220_vj, PT_220_300_vj, PT_300_450_vj, N_PT_BINS_vj } BINS_vj;
    Histo1DPtr _h_ungroomedJetMass_AK7_wj[N_PT_BINS_vj];
    Histo1DPtr _h_filteredJetMass_AK7_wj[N_PT_BINS_vj];
    Histo1DPtr _h_trimmedJetMass_AK7_wj[N_PT_BINS_vj];
    Histo1DPtr _h_prunedJetMass_AK7_wj[N_PT_BINS_vj];
    Histo1DPtr _h_prunedJetMass_CA8_wj[N_PT_BINS_vj];
    Histo1DPtr _h_filteredJetMass_CA12_wj[N_PT_BINS_vj-1];
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2013_I1224539_WJET);

}

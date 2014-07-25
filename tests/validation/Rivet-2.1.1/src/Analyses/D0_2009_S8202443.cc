// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief D0 Z + jet + \f$ X \f$ cross-section / \f$ p_\perp \f$ distributions
  class D0_2009_S8202443 : public Analysis {
  public:

    /// Constructor
    D0_2009_S8202443()
      : Analysis("D0_2009_S8202443"),
        _sum_of_weights(0), _sum_of_weights_constrained(0)
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      FinalState fs;
      // Leptons in constrained tracking acceptance
      vector<pair<double, double> > etaRanges;
      etaRanges.push_back(make_pair(-2.5, -1.5));
      etaRanges.push_back(make_pair(-1.1, 1.1));
      etaRanges.push_back(make_pair(1.5, 2.5));
      ZFinder zfinder_constrained(fs, etaRanges, 25*GeV, PID::ELECTRON,
                                  65*GeV, 115*GeV, 0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zfinder_constrained, "ZFinderConstrained");
      FastJets conefinder_constrained(zfinder_constrained.remainingFinalState(),
                                      FastJets::D0ILCONE, 0.5);
      addProjection(conefinder_constrained, "ConeFinderConstrained");

      // Unconstrained leptons
      ZFinder zfinder(fs, -MAXDOUBLE, MAXDOUBLE, 0*GeV, PID::ELECTRON,
                      65*GeV, 115*GeV, 0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");
      FastJets conefinder(zfinder.remainingFinalState(), FastJets::D0ILCONE, 0.5);
      addProjection(conefinder, "ConeFinder");

      _h_jet1_pT_constrained = bookHisto1D(1, 1, 1);
      _h_jet2_pT_constrained = bookHisto1D(3, 1, 1);
      _h_jet3_pT_constrained = bookHisto1D(5, 1, 1);
      _h_jet1_pT = bookHisto1D(2, 1, 1);
      _h_jet2_pT = bookHisto1D(4, 1, 1);
      _h_jet3_pT = bookHisto1D(6, 1, 1);
    }



    // Do the analysis
    void analyze(const Event& e) {
      double weight = e.weight();

      // Unconstrained electrons
      const ZFinder& zfinder = applyProjection<ZFinder>(e, "ZFinder");
      if (zfinder.bosons().size() == 0) {
        MSG_DEBUG("No unique lepton pair found.");
        vetoEvent;
      }
      _sum_of_weights += weight;
      const Jets& jets = applyProjection<JetAlg>(e, "ConeFinder").jetsByPt(20*GeV);
      /// @todo Replace this explicit selection with a Cut
      Jets jets_cut;
      foreach (const Jet& j, jets) {
        if (fabs(j.eta()) < 2.5) jets_cut.push_back(j);
      }
      if (jets_cut.size() > 0)
        _h_jet1_pT->fill(jets_cut[0].pT()/GeV, weight);
      if (jets_cut.size() > 1)
        _h_jet2_pT->fill(jets_cut[1].pT()/GeV, weight);
      if (jets_cut.size() > 2)
        _h_jet3_pT->fill(jets_cut[2].pT()/GeV, weight);


      // Constrained electrons
      const ZFinder& zfinder_constrained = applyProjection<ZFinder>(e, "ZFinderConstrained");
      if (zfinder_constrained.bosons().size() == 0) {
        MSG_DEBUG("No unique constrained lepton pair found.");
        return; // Not really a "veto", since if we got this far there is an unconstrained Z
      }
      _sum_of_weights_constrained += weight;
      const Jets& jets_constrained = applyProjection<JetAlg>(e, "ConeFinderConstrained").jetsByPt(20*GeV);
      /// @todo Replace this explicit selection with a Cut
      Jets jets_cut_constrained;
      foreach (const Jet& j, jets_constrained) {
        if (fabs(j.eta()) < 2.5) jets_cut_constrained.push_back(j);
      }
      if (jets_cut_constrained.size() > 0)
        _h_jet1_pT_constrained->fill(jets_cut_constrained[0].pT()/GeV, weight);
      if (jets_cut_constrained.size() > 1)
        _h_jet2_pT_constrained->fill(jets_cut_constrained[1].pT()/GeV, weight);
      if (jets_cut_constrained.size() > 2)
        _h_jet3_pT_constrained->fill(jets_cut_constrained[2].pT()/GeV, weight);
    }


    // Finalize
    void finalize() {
      scale(_h_jet1_pT, 1/_sum_of_weights);
      scale(_h_jet2_pT, 1/_sum_of_weights);
      scale(_h_jet3_pT, 1/_sum_of_weights);
      scale(_h_jet1_pT_constrained, 1/_sum_of_weights_constrained);
      scale(_h_jet2_pT_constrained, 1/_sum_of_weights_constrained);
      scale(_h_jet3_pT_constrained, 1/_sum_of_weights_constrained);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_jet1_pT;
    Histo1DPtr _h_jet2_pT;
    Histo1DPtr _h_jet3_pT;
    Histo1DPtr _h_jet1_pT_constrained;
    Histo1DPtr _h_jet2_pT_constrained;
    Histo1DPtr _h_jet3_pT_constrained;
    //@}

    double _sum_of_weights, _sum_of_weights_constrained;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2009_S8202443);

}

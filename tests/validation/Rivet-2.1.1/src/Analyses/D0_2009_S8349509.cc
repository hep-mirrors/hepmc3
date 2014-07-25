// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief D0 Z+jets angular distributions
  class D0_2009_S8349509 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    D0_2009_S8349509()
      : Analysis("D0_2009_S8349509"),
        _inclusive_Z_sumofweights(0)
    {    }

    //@}


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      ZFinder zfinder(FinalState(), -1.7, 1.7, 15*GeV, PID::MUON, 65*GeV, 115*GeV, 0.2, ZFinder::NOCLUSTER, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");

      FastJets conefinder(zfinder.remainingFinalState(), FastJets::D0ILCONE, 0.5);
      addProjection(conefinder, "ConeFinder");

      _h_dphi_jet_Z25 = bookHisto1D(1, 1, 1);
      _h_dphi_jet_Z45 = bookHisto1D(2, 1, 1);

      _h_dy_jet_Z25 = bookHisto1D(3, 1, 1);
      _h_dy_jet_Z45 = bookHisto1D(4, 1, 1);

      _h_yboost_jet_Z25 = bookHisto1D(5, 1, 1);
      _h_yboost_jet_Z45 = bookHisto1D(6, 1, 1);

      _h_dphi_jet_Z25_xs = bookHisto1D(1, 1, 2);
      _h_dphi_jet_Z45_xs = bookHisto1D(2, 1, 2);

      _h_dy_jet_Z25_xs = bookHisto1D(3, 1, 2);
      _h_dy_jet_Z45_xs = bookHisto1D(4, 1, 2);

      _h_yboost_jet_Z25_xs = bookHisto1D(5, 1, 2);
      _h_yboost_jet_Z45_xs = bookHisto1D(6, 1, 2);

      _inclusive_Z_sumofweights = 0;
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      const ZFinder& zfinder = applyProjection<ZFinder>(event, "ZFinder");
      if (zfinder.bosons().size() == 1) {
        // count inclusive sum of weights for histogram normalisation
        _inclusive_Z_sumofweights += weight;

        const FourMomentum& zmom = zfinder.bosons()[0].momentum();
        if (zmom.pT() < 25*GeV) vetoEvent;

        Jets jets;
        foreach (const Jet& j, applyProjection<JetAlg>(event, "ConeFinder").jetsByPt(20*GeV)) {
          if (fabs(j.momentum().pseudorapidity()) < 2.8) {
            jets.push_back(j);
            break;
          }
        }

        // Return if there are no jets:
        if (jets.size() < 1) {
          MSG_DEBUG("Skipping event " << numEvents() << " because no jets pass cuts ");
          vetoEvent;
        }

        const FourMomentum& jetmom = jets[0].momentum();
        const double yZ = zmom.rapidity();
        const double yjet = jetmom.rapidity();
        const double dphi = deltaPhi(zmom, jetmom);
        const double dy = deltaRap(zmom, jetmom);
        const double yboost = fabs(yZ+yjet)/2;

        if (zmom.pT() > 25*GeV) {
          _h_dphi_jet_Z25->fill(dphi, weight);
          _h_dy_jet_Z25->fill(dy, weight);
          _h_yboost_jet_Z25->fill(yboost, weight);
          _h_dphi_jet_Z25_xs->fill(dphi, weight);
          _h_dy_jet_Z25_xs->fill(dy, weight);
          _h_yboost_jet_Z25_xs->fill(yboost, weight);
        }
        if (zmom.pT() > 45*GeV) {
          _h_dphi_jet_Z45->fill(dphi, weight);
          _h_dy_jet_Z45->fill(dy, weight);
          _h_yboost_jet_Z45->fill(yboost, weight);
          _h_dphi_jet_Z45_xs->fill(dphi, weight);
          _h_dy_jet_Z45_xs->fill(dy, weight);
          _h_yboost_jet_Z45_xs->fill(yboost, weight);
        }
      }

    }


    void finalize() {
      if (_inclusive_Z_sumofweights == 0) return;
      scale(_h_dphi_jet_Z25, 1/_inclusive_Z_sumofweights);
      scale(_h_dphi_jet_Z45, 1/_inclusive_Z_sumofweights);
      scale(_h_dy_jet_Z25, 1/_inclusive_Z_sumofweights);
      scale(_h_dy_jet_Z45, 1/_inclusive_Z_sumofweights);
      scale(_h_yboost_jet_Z25, 1/_inclusive_Z_sumofweights);
      scale(_h_yboost_jet_Z45, 1/_inclusive_Z_sumofweights);

      scale(_h_dphi_jet_Z25_xs, crossSectionPerEvent());
      scale(_h_dphi_jet_Z45_xs, crossSectionPerEvent());
      scale(_h_dy_jet_Z25_xs, crossSectionPerEvent());
      scale(_h_dy_jet_Z45_xs, crossSectionPerEvent());
      scale(_h_yboost_jet_Z25_xs, crossSectionPerEvent());
      scale(_h_yboost_jet_Z45_xs, crossSectionPerEvent());
    }

    //@}

  private:

    // Data members like post-cuts event weight counters go here

  private:

    /// @name Histograms (normalised)
    //@{
    Histo1DPtr _h_dphi_jet_Z25;
    Histo1DPtr _h_dphi_jet_Z45;

    Histo1DPtr _h_dy_jet_Z25;
    Histo1DPtr _h_dy_jet_Z45;

    Histo1DPtr _h_yboost_jet_Z25;
    Histo1DPtr _h_yboost_jet_Z45;
    //@}

    /// @name Histograms (absolute cross sections)
    //@{
    Histo1DPtr _h_dphi_jet_Z25_xs;
    Histo1DPtr _h_dphi_jet_Z45_xs;

    Histo1DPtr _h_dy_jet_Z25_xs;
    Histo1DPtr _h_dy_jet_Z45_xs;

    Histo1DPtr _h_yboost_jet_Z25_xs;
    Histo1DPtr _h_yboost_jet_Z45_xs;
    //@}

    double _inclusive_Z_sumofweights;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2009_S8349509);

}

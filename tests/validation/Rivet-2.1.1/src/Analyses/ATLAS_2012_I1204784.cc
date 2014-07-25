// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Tools/BinnedHistogram.hh"

namespace Rivet {


  /// ATLAS Z phi* measurement
  class ATLAS_2012_I1204784 : public Analysis {
    public:

      /// Constructor
      ATLAS_2012_I1204784()
        : Analysis("ATLAS_2012_I1204784")
      {      }


    public:

      /// Book histograms and initialise projections before the run
      void init() {

        ZFinder zfinder_dressed_el(-2.4, 2.4, 20, PID::ELECTRON, 66*GeV, 116*GeV, 0.1, ZFinder::CLUSTERNODECAY);
        addProjection(zfinder_dressed_el, "ZFinder_dressed_el");
        ZFinder zfinder_bare_el(-2.4, 2.4, 20, PID::ELECTRON, 66*GeV, 116*GeV, 0.0, ZFinder::NOCLUSTER);
        addProjection(zfinder_bare_el, "ZFinder_bare_el");
        ZFinder zfinder_dressed_mu(-2.4, 2.4, 20, PID::MUON, 66*GeV, 116*GeV, 0.1, ZFinder::CLUSTERNODECAY);
        addProjection(zfinder_dressed_mu, "ZFinder_dressed_mu");
        ZFinder zfinder_bare_mu(-2.4, 2.4, 20, PID::MUON, 66*GeV, 116*GeV, 0.0, ZFinder::NOCLUSTER);
        addProjection(zfinder_bare_mu, "ZFinder_bare_mu");

        // Book histograms
        // Single-differential plots
        _hist_zphistar_el_bare = bookHisto1D(1, 1, 1);
        _hist_zphistar_mu_bare = bookHisto1D(1, 1, 2);
        _hist_zphistar_el_dressed = bookHisto1D(2, 1, 1);
        _hist_zphistar_mu_dressed = bookHisto1D(2, 1, 2);

        // Double-differential plots
        _h_phistar_el_bare.addHistogram(0.0, 0.8, bookHisto1D(3, 1, 1));
        _h_phistar_el_bare.addHistogram(0.8, 1.6, bookHisto1D(3, 1, 2));
        _h_phistar_el_bare.addHistogram(1.6, 10.0, bookHisto1D(3, 1, 3));

        _h_phistar_el_dressed.addHistogram(0.0, 0.8, bookHisto1D(3, 2, 1));
        _h_phistar_el_dressed.addHistogram(0.8, 1.6, bookHisto1D(3, 2, 2));
        _h_phistar_el_dressed.addHistogram(1.6, 10.0, bookHisto1D(3, 2, 3));

        _h_phistar_mu_bare.addHistogram(0.0, 0.8, bookHisto1D(4, 1, 1));
        _h_phistar_mu_bare.addHistogram(0.8, 1.6, bookHisto1D(4, 1, 2));
        _h_phistar_mu_bare.addHistogram(1.6, 10.0, bookHisto1D(4, 1, 3));

        _h_phistar_mu_dressed.addHistogram(0.0, 0.8, bookHisto1D(4, 2, 1));
        _h_phistar_mu_dressed.addHistogram(0.8, 1.6, bookHisto1D(4, 2, 2));
        _h_phistar_mu_dressed.addHistogram(1.6, 10.0, bookHisto1D(4, 2, 3));
      }


      /// Perform the per-event analysis
      void analyze(const Event& event) {
        const double weight = event.weight();

        const ZFinder& zfinder_dressed_el = applyProjection<ZFinder>(event, "ZFinder_dressed_el");
        const ZFinder& zfinder_bare_el = applyProjection<ZFinder>(event, "ZFinder_bare_el");
        const ZFinder& zfinder_dressed_mu = applyProjection<ZFinder>(event, "ZFinder_dressed_mu");
        const ZFinder& zfinder_bare_mu = applyProjection<ZFinder>(event, "ZFinder_bare_mu");

        fillPlots(zfinder_dressed_el, _hist_zphistar_el_dressed, _h_phistar_el_dressed, weight);
        fillPlots(zfinder_bare_el, _hist_zphistar_el_bare, _h_phistar_el_bare, weight);
        fillPlots(zfinder_dressed_mu, _hist_zphistar_mu_dressed, _h_phistar_mu_dressed, weight);
        fillPlots(zfinder_bare_mu, _hist_zphistar_mu_bare, _h_phistar_mu_bare, weight);
      }


      void fillPlots(const ZFinder& zfind, Histo1DPtr hist, BinnedHistogram<double>& binnedHist, double weight) {
        if (zfind.bosons().size() != 1) return;
        Particles leptons = zfind.constituents(cmpMomByPt);

        const FourMomentum lminus = leptons[0].charge() < 0 ? leptons[0].momentum() : leptons[1].momentum();
        const FourMomentum lplus = leptons[0].charge() < 0 ? leptons[1].momentum() : leptons[0].momentum();

        const double phi_acop = M_PI - deltaPhi(lminus, lplus);
        const double costhetastar = tanh((lminus.eta()-lplus.eta())/2.0);
        const double sin2thetastar = (costhetastar <= 1) ? 1.0 - sqr(costhetastar) : 0;
        const double phistar = tan(phi_acop/2.0) * sqrt(sin2thetastar);
        hist->fill(phistar, weight);

        binnedHist.fill(zfind.bosons()[0].absrap(), phistar, weight);
      }


      /// Normalise histograms etc., after the run
      void finalize() {
        normalize(_hist_zphistar_el_dressed);
        normalize(_hist_zphistar_el_bare);
        normalize(_hist_zphistar_mu_dressed);
        normalize(_hist_zphistar_mu_bare);

        foreach (Histo1DPtr hist, _h_phistar_mu_dressed.getHistograms()) { normalize(hist); }
        foreach (Histo1DPtr hist, _h_phistar_mu_bare.getHistograms()) { normalize(hist); }
        foreach (Histo1DPtr hist, _h_phistar_el_bare.getHistograms()) { normalize(hist); }
        foreach (Histo1DPtr hist, _h_phistar_el_dressed.getHistograms()) { normalize(hist); }
      }

      //@}


    private:

      BinnedHistogram<double> _h_phistar_mu_dressed;
      BinnedHistogram<double> _h_phistar_mu_bare;
      BinnedHistogram<double> _h_phistar_el_dressed;
      BinnedHistogram<double> _h_phistar_el_bare;

      Histo1DPtr _hist_zphistar_el_dressed;
      Histo1DPtr _hist_zphistar_el_bare;

      Histo1DPtr _hist_zphistar_mu_dressed;
      Histo1DPtr _hist_zphistar_mu_bare;

      Histo1DPtr _hist_zphistar_el_bare_1;
      Histo1DPtr _hist_zphistar_el_bare_2;
      Histo1DPtr _hist_zphistar_el_bare_3;

      Histo1DPtr _hist_zphistar_el_dressed_1;
      Histo1DPtr _hist_zphistar_el_dressed_2;
      Histo1DPtr _hist_zphistar_el_dressed_3;

      Histo1DPtr _hist_zphistar_mu_bare_1;
      Histo1DPtr _hist_zphistar_mu_bare_2;
      Histo1DPtr _hist_zphistar_mu_bare_3;

      Histo1DPtr _hist_zphistar_mu_dressed_1;
      Histo1DPtr _hist_zphistar_mu_dressed_2;
      Histo1DPtr _hist_zphistar_mu_dressed_3;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1204784);

}

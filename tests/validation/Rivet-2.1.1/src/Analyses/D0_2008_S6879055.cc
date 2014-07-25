// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief D0 measurement of the ratio \f$ \sigma(Z/\gamma^* + n \text{ jets})/\sigma(Z/\gamma^*) \f$
  class D0_2008_S6879055 : public Analysis {
  public:

    /// Default constructor.
    D0_2008_S6879055()
      : Analysis("D0_2008_S6879055")
    {    }
    // DEFAULT_RIVET_ANA_CONSTRUCTOR(D0_2008_S6879055);

    /// @name Analysis methods
    //@{

    // Book histograms
    void init() {
      FinalState fs;
      ZFinder zfinder(fs, -MAXDOUBLE, MAXDOUBLE, 0*GeV, PID::ELECTRON,
                      40*GeV, 200*GeV, 0.2, ZFinder::CLUSTERNODECAY, ZFinder::TRACK);
      addProjection(zfinder, "ZFinder");

      FastJets conefinder(zfinder.remainingFinalState(), FastJets::D0ILCONE, 0.5);
      addProjection(conefinder, "ConeFinder");

      _crossSectionRatio = bookHisto1D(1, 1, 1);
      _pTjet1 = bookHisto1D(2, 1, 1);
      _pTjet2 = bookHisto1D(3, 1, 1);
      _pTjet3 = bookHisto1D(4, 1, 1);
    }



    /// Do the analysis
    void analyze(const Event& event) {
      const double weight = event.weight();



      const ZFinder& zfinder = applyProjection<ZFinder>(event, "ZFinder");
      if (zfinder.bosons().size()!=1) {
        vetoEvent;
      }

      FourMomentum e0 = zfinder.constituents()[0].momentum();
      FourMomentum e1 = zfinder.constituents()[1].momentum();
      const double e0eta = e0.eta();
      const double e0phi = e0.phi();
      const double e1eta = e1.eta();
      const double e1phi = e1.phi();

      vector<FourMomentum> finaljet_list;
      foreach (const Jet& j, applyProjection<JetAlg>(event, "ConeFinder").jetsByPt(20*GeV)) {
        const double jeta = j.eta();
        const double jphi = j.momentum().phi();
        if (fabs(jeta) < 2.5) {
          if (deltaR(e0eta, e0phi, jeta, jphi) > 0.4 &&
              deltaR(e1eta, e1phi, jeta, jphi) > 0.4) {
            finaljet_list.push_back(j.momentum());
          }
        }
      }

      // For normalisation of crossSection data (includes events with no jets passing cuts)
      _crossSectionRatio->fill(0, weight);

      // Fill jet pT and multiplicities
      if (finaljet_list.size() >= 1) {
        _crossSectionRatio->fill(1, weight);
        _pTjet1->fill(finaljet_list[0].pT(), weight);
      }
      if (finaljet_list.size() >= 2) {
        _crossSectionRatio->fill(2, weight);
        _pTjet2->fill(finaljet_list[1].pT(), weight);
      }
      if (finaljet_list.size() >= 3) {
        _crossSectionRatio->fill(3, weight);
        _pTjet3->fill(finaljet_list[2].pT(), weight);
      }
      if (finaljet_list.size() >= 4) {
        _crossSectionRatio->fill(4, weight);
      }
    }



    /// Finalize
    void finalize() {
      // Now divide by the inclusive result
      scale(_crossSectionRatio,1/_crossSectionRatio->bin(0).area());

      // Normalise jet pTs to integrals of data
      // @note There is no other way to do this, because these quantities are not detector-corrected
      /// @todo Use integrals of refData()?
      normalize(_pTjet1, 10439); // fixed norm OK
      normalize(_pTjet2, 1461.5); // fixed norm OK
      normalize(_pTjet3, 217); // fixed norm OK
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _crossSectionRatio;
    Histo1DPtr _pTjet1;
    Histo1DPtr _pTjet2;
    Histo1DPtr _pTjet3;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2008_S6879055);

}

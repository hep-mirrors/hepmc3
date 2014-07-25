// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ZFinder.hh"

namespace Rivet {


  /// @brief CDF Run I Z \f$ p_\perp \f$ in Drell-Yan events
  /// @author Hendrik Hoeth
  class CDF_2000_S4155203 : public Analysis {
  public:

    /// Constructor
    CDF_2000_S4155203()
      : Analysis("CDF_2000_S4155203")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      ZFinder zfinder(FinalState(), -MAXDOUBLE, MAXDOUBLE, 0*GeV, PID::ELECTRON,
                      66*GeV, 116*GeV, 0.0, ZFinder::NOCLUSTER);
      addProjection(zfinder, "ZFinder");

      // Book histogram
      _hist_zpt = bookHisto1D(1, 1, 1);
    }


    /// Do the analysis
    void analyze(const Event& e) {
      const ZFinder& zfinder = applyProjection<ZFinder>(e, "ZFinder");
      if (zfinder.bosons().size() != 1) {
        MSG_DEBUG("Num e+ e- pairs found = " << zfinder.bosons().size());
        vetoEvent;
      }

      FourMomentum pZ = zfinder.bosons()[0].momentum();
      if (pZ.mass2() < 0) {
        MSG_DEBUG("Negative Z mass**2 = " << pZ.mass2()/GeV2 << "!");
        vetoEvent;
      }

      MSG_DEBUG("Dilepton mass = " << pZ.mass()/GeV << " GeV");
      MSG_DEBUG("Dilepton pT   = " << pZ.pT()/GeV << " GeV");
      _hist_zpt->fill(pZ.pT()/GeV, e.weight());
    }


    void finalize() {
      scale(_hist_zpt, crossSection()/picobarn/sumOfWeights());
    }

    //@}


  private:

    Histo1DPtr _hist_zpt;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2000_S4155203);

}

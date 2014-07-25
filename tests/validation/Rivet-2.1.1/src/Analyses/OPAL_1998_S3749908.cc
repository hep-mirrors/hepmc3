// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief OPAL photon/light meson paper
  /// @author Peter Richardson
  class OPAL_1998_S3749908 : public Analysis {
  public:

    /// Constructor
    OPAL_1998_S3749908()
      : Analysis("OPAL_1998_S3749908")
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXePhoton   = bookHisto1D( 2, 1, 1);
      _histXiPhoton   = bookHisto1D( 3, 1, 1);
      _histXePi       = bookHisto1D( 4, 1, 1);
      _histXiPi       = bookHisto1D( 5, 1, 1);
      _histXeEta      = bookHisto1D( 6, 1, 1);
      _histXiEta      = bookHisto1D( 7, 1, 1);
      _histXeRho      = bookHisto1D( 8, 1, 1);
      _histXiRho      = bookHisto1D( 9, 1, 1);
      _histXeOmega    = bookHisto1D(10, 1, 1);
      _histXiOmega    = bookHisto1D(11, 1, 1);
      _histXeEtaPrime = bookHisto1D(12, 1, 1);
      _histXiEtaPrime = bookHisto1D(13, 1, 1);
      _histXeA0       = bookHisto1D(14, 1, 1);
      _histXiA0       = bookHisto1D(15, 1, 1);
    }


    void analyze(const Event& e) {
      // First, veto on leptonic events by requiring at least 4 charged FS particles
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      const size_t numParticles = fs.particles().size();

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      if (numParticles < 2) {
        MSG_DEBUG("Failed leptonic event cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed leptonic event cut");

      // Get event weight for histo filling
      const double weight = e.weight();

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);

      // Final state of unstable particles to get particle spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      foreach (const Particle& p, ufs.particles()) {
        const int id = abs(p.pdgId());
        double xi = -log(p.momentum().vector3().mod()/meanBeamMom);
        double xE = p.momentum().t()/meanBeamMom;
        switch (id) {
        case 22:
          _histXePhoton->fill(xE, weight);
          _histXiPhoton->fill(xi, weight);
          break;
        case 111:
          _histXePi->fill(xE, weight);
          _histXiPi->fill(xi, weight);
          break;
        case 211:
          _histXeEta->fill(xE, weight);
          _histXiEta->fill(xi, weight);
          break;
        case 213:
          _histXeRho->fill(xE, weight);
          _histXiRho->fill(xi, weight);
          break;
        case 223:
          _histXeOmega->fill(xE, weight);
          _histXiOmega->fill(xi, weight);
          break;
        case 331:
          _histXeEtaPrime->fill(xE, weight);
          _histXiEtaPrime->fill(xi, weight);
          break;
        case 9000111:
          _histXeA0->fill(xE, weight);
          _histXiA0->fill(xi, weight);
          break;
        }
      }
    }


    /// Finalize
    void finalize() {
      scale(_histXePhoton  , 1./sumOfWeights());
      scale(_histXiPhoton  , 1./sumOfWeights());
      scale(_histXePi      , 1./sumOfWeights());
      scale(_histXiPi      , 1./sumOfWeights());
      scale(_histXeEta     , 1./sumOfWeights());
      scale(_histXiEta     , 1./sumOfWeights());
      scale(_histXeRho     , 1./sumOfWeights());
      scale(_histXiRho     , 1./sumOfWeights());
      scale(_histXeOmega   , 1./sumOfWeights());
      scale(_histXiOmega   , 1./sumOfWeights());
      scale(_histXeEtaPrime, 1./sumOfWeights());
      scale(_histXiEtaPrime, 1./sumOfWeights());
      scale(_histXeA0      , 1./sumOfWeights());
      scale(_histXiA0      , 1./sumOfWeights());
    }

    //@}


  private:

      Histo1DPtr _histXePhoton  ;
      Histo1DPtr _histXiPhoton  ;
      Histo1DPtr _histXePi      ;
      Histo1DPtr _histXiPi      ;
      Histo1DPtr _histXeEta     ;
      Histo1DPtr _histXiEta     ;
      Histo1DPtr _histXeRho     ;
      Histo1DPtr _histXiRho     ;
      Histo1DPtr _histXeOmega   ;
      Histo1DPtr _histXiOmega   ;
      Histo1DPtr _histXeEtaPrime;
      Histo1DPtr _histXiEtaPrime;
      Histo1DPtr _histXeA0      ;
      Histo1DPtr _histXiA0      ;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1998_S3749908);

}

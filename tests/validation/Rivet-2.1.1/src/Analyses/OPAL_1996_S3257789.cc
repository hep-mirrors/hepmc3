// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief OPAL J/Psi fragmentation function paper
  /// @author Peter Richardson
  class OPAL_1996_S3257789 : public Analysis {
  public:

    /// Constructor
    OPAL_1996_S3257789()
      : Analysis("OPAL_1996_S3257789"), _weightSum(0.)
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXpJPsi   = bookHisto1D( 1, 1, 1);
      _multJPsi     = bookHisto1D( 2, 1, 1);
      _multPsiPrime = bookHisto1D( 2, 1, 2);
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
        if(abs(p.pdgId())==443) {
          double xp = p.momentum().vector3().mod()/meanBeamMom;
          _histXpJPsi->fill(xp, weight);
          _multJPsi->fill(91.2,weight);
          _weightSum += weight;
        }
        else if(abs(p.pdgId())==100443) {
          _multPsiPrime->fill(91.2,weight);
        }
      }
    }


    /// Finalize
    void finalize() {
      if(_weightSum>0.)
        scale(_histXpJPsi  , 0.1/_weightSum);
      scale(_multJPsi    , 1./sumOfWeights());
      scale(_multPsiPrime, 1./sumOfWeights());
    }

    //@}


  private:

    double _weightSum;
    Histo1DPtr _histXpJPsi;
    Histo1DPtr _multJPsi;
    Histo1DPtr _multPsiPrime;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1996_S3257789);

}

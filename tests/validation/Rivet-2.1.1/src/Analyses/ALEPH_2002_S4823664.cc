// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief ALEPH eta/omega fragmentation function paper
  /// @author Peter Richardson
  class ALEPH_2002_S4823664 : public Analysis {
  public:

    /// Constructor
    ALEPH_2002_S4823664()
      : Analysis("ALEPH_2002_S4823664")
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXpEta   = bookHisto1D( 2, 1, 2);
      _histXpOmega = bookHisto1D( 3, 1, 2);
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
        if(abs(p.pdgId())==221) {
          double xp = p.momentum().vector3().mod()/meanBeamMom;
          _histXpEta->fill(xp, weight);
        }
        else if(abs(p.pdgId())==223) {
          double xp = p.momentum().vector3().mod()/meanBeamMom;
          _histXpOmega->fill(xp, weight);
        }
      }
    }


    /// Finalize
    void finalize() {
      scale(_histXpEta  , 1./sumOfWeights());
      scale(_histXpOmega, 1./sumOfWeights());
    }

    //@}


  private:

      Histo1DPtr _histXpEta;
      Histo1DPtr _histXpOmega;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALEPH_2002_S4823664);

}

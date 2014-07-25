// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief OPAL K0 fragmentation function paper
  /// @author Peter Richardson
  class OPAL_2000_S4418603 : public Analysis {
  public:

    /// Constructor
    OPAL_2000_S4418603()
      : Analysis("OPAL_2000_S4418603")
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXeK0   = bookHisto1D( 3, 1, 1);
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
        if (id == PID::K0S || id == PID::K0L) {
          double xE = p.momentum().t()/meanBeamMom;
          _histXeK0->fill(xE, weight);
        }
      }
    }


    /// Finalize
    void finalize() {
      scale(_histXeK0, 1./sumOfWeights());
    }

    //@}


  private:

      Histo1DPtr _histXeK0;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_2000_S4418603);

}

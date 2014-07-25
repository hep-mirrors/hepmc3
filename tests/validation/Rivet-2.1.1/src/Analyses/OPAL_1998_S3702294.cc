// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief OPAL f0,f2 and phi fragmentation function paper
  /// @author Peter Richardson
  class OPAL_1998_S3702294 : public Analysis {
  public:

    /// Constructor
    OPAL_1998_S3702294()
      : Analysis("OPAL_1998_S3702294")
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXpf0  = bookHisto1D( 2, 1, 1);
      _histXpf2  = bookHisto1D( 2, 1, 2);
      _histXpPhi = bookHisto1D( 2, 1, 3);
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
        double xp = p.momentum().vector3().mod()/meanBeamMom;
        switch (id) {
        case 9010221:
          _histXpf0->fill(xp, weight);
          break;
        case 225:
          _histXpf2->fill(xp, weight);
          break;
        case 333:
          _histXpPhi->fill(xp, weight);
          break;
        }
      }
    }


    /// Finalize
    void finalize() {
      scale(_histXpf0 , 1./sumOfWeights());
      scale(_histXpf2 , 1./sumOfWeights());
      scale(_histXpPhi, 1./sumOfWeights());
    }

    //@}


  private:

      Histo1DPtr _histXpf0;
      Histo1DPtr _histXpf2;
      Histo1DPtr _histXpPhi;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1998_S3702294);

}

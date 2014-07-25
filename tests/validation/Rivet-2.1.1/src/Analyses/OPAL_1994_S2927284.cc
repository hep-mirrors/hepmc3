// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// @brief OPAL charged particle fragmentation functions
  /// @author Peter Richardson
  class OPAL_1994_S2927284 : public Analysis {
  public:

    /// Constructor
    OPAL_1994_S2927284()
      : Analysis("OPAL_1994_S2927284")
    {   }


    /// @name Analysis methods
    //@{

    void analyze(const Event& e) {

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      if (fs.particles().size() < 2) {
        MSG_DEBUG("Failed ncharged cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed ncharged cut");

      // Get event weight for histo filling
      const double weight = e.weight();

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);

      foreach (const Particle& p, fs.particles()) {
        int id = abs(p.pdgId());
        // charged pions
        if (id == PID::PIPLUS) {
          _histXpPiPlus->fill(p.momentum().vector3().mod(), weight);
        } else if(id == PID::KPLUS) {
          _histXpKPlus->fill(p.momentum().vector3().mod(), weight);
        } else if(id == PID::PROTON) {
          _histXpProton->fill(p.momentum().vector3().mod(), weight);
        }
      }
    }


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");

      _histXpPiPlus = bookHisto1D( 1, 1, 1);
      _histXpKPlus  = bookHisto1D( 2, 1, 1);
      _histXpProton = bookHisto1D( 3, 1, 1);
    }


    /// Finalize
    void finalize() {
      scale(_histXpPiPlus,1./sumOfWeights());
      scale(_histXpKPlus ,1./sumOfWeights());
      scale(_histXpProton,1./sumOfWeights());
    }

    //@}

  private:

    Histo1DPtr _histXpPiPlus;
    Histo1DPtr _histXpKPlus;
    Histo1DPtr _histXpProton;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1994_S2927284);

}

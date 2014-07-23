// -*- C++ -*-
#ifndef RIVET_ChargedLeptons_HH
#define RIVET_ChargedLeptons_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /// @brief Get charged final-state leptons
  ///
  /// NB. This is just electrons and muons, unless you set taus stable!
  class ChargedLeptons : public FinalState {
  public:

    /// Constructor
    ChargedLeptons(const FinalState& fsp)
    {
      setName("ChargedLeptons");
      addProjection(ChargedFinalState(fsp), "ChFS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new ChargedLeptons(*this);
    }

  protected:

    /// Apply the projection to the event.
    void project(const Event& evt);

    /// Compare projections.
    int compare(const Projection& other) const;

  public:

    /// Access the projected leptons.
    const Particles& chargedLeptons() const {
      return _theParticles;
    }

  };


}

#endif

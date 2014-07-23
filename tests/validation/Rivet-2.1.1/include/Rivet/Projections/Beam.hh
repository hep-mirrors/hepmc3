// -*- C++ -*-
#ifndef RIVET_Beam_HH
#define RIVET_Beam_HH

#include "Rivet/Projection.hh"
#include "Rivet/Event.hh"
#include "Rivet/Particle.hh"

namespace Rivet {


  /// @name Stand-alone functions
  //@{

  /// Function to get beam particles from an event
  ParticlePair beams(const Event& e);

  /// Function to get beam particle IDs from an event
  PdgIdPair beamIds(const Event& e);

  /// Function to get beam particle IDs from a pair of particles
  PdgIdPair beamIds(const ParticlePair& beams);

  /// Function to get beam centre of mass energy from an event
  double sqrtS(const Event& e);

  /// Function to get beam centre of mass energy from a pair of particles
  double sqrtS(const ParticlePair& beams);

  /// Function to get beam centre of mass energy from a pair of beam momenta
  double sqrtS(const FourMomentum& pa, const FourMomentum& pb);

  //@}




  /// @brief Project out the incoming beams
  class Beam : public Projection {
  public:

    /// The default constructor.
    Beam() {
      setName("Beam");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new Beam(*this);
    }


  public:

    /// The pair of beam particles in the current collision.
    const ParticlePair& beams() const {
      return _theBeams;
    }

    /// The pair of beam particle PDG codes in the current collision.
    const PdgIdPair beamIds() const {
      return Rivet::beamIds(beams());
    }

    /// Get centre of mass energy, \f$ \sqrt{s} \f$.
    double sqrtS() const;


  public:

    /// Project on to the Event
    virtual void project(const Event& e);


  protected:

    /// Compare with other projections.
    virtual int compare(const Projection& UNUSED(p)) const {
      return EQUIVALENT;
    }


  private:

    /// The beam particles in the current collision
    ParticlePair _theBeams;

  };


}

#endif

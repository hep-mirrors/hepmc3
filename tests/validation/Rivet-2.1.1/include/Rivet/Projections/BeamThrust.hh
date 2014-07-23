// -*- C++ -*-
#ifndef RIVET_BeamThrust_HH
#define RIVET_BeamThrust_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Event.hh"

namespace Rivet {

  class BeamThrust : public Projection {
  public:

    /// Constructor.
    BeamThrust() {}

    BeamThrust(const FinalState& fsp) {
      setName("BeamThrust");
      addProjection(fsp, "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new BeamThrust(*this);
    }

  protected:

    /// Perform the projection on the Event
    void project(const Event& e) {
      const vector<Particle> ps
        = applyProjection<FinalState>(e, "FS").particles();
      calc(ps);
    }

    /// Compare projections
    int compare(const Projection& p) const {
      return mkNamedPCmp(p, "FS");
    }


  public:

    double beamthrust() const { return _beamthrust; }

  public:

    /// @name Direct methods
    /// Ways to do the calculation directly, without engaging the caching system
    //@{

    /// Manually calculate the beamthrust, without engaging the caching system
    void calc(const FinalState& fs);

    /// Manually calculate the beamthrust, without engaging the caching system
    void calc(const vector<Particle>& fsparticles);

    /// Manually calculate the beamthrust, without engaging the caching system
    void calc(const vector<FourMomentum>& fsmomenta);

    //@}


  private:

    /// The beamthrust scalar.
    double _beamthrust;

  private:

    /// Explicitly calculate the beamthrust values.
    void _calcBeamThrust(const vector<FourMomentum>& fsmomenta);

  };

}

#endif

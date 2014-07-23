// -*- C++ -*-
#ifndef RIVET_DISLepton_HH
#define RIVET_DISLepton_HH

#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /// @brief Get the incoming and outgoing leptons in a DIS event.
  class DISLepton : public Projection {

  public:

    /// @name Constructors.
    //@{

    DISLepton(){
      setName("DISLepton");
      addProjection(Beam(), "Beam");
      addProjection(FinalState(), "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new DISLepton(*this);
    }
    //@}


  protected:

    /// Perform the projection operation on the supplied event.
    virtual void project(const Event& e);

    /// Compare with other projections.
    virtual int compare(const Projection& p) const;

  public:

    /// The incoming lepton.
    const Particle& in() const { return _incoming; }

    /// The outgoing lepton.
    const Particle& out() const { return _outgoing; }

    const double &pzSign() const { return _sign; }

  private:

    /// The incoming lepton.
    Particle _incoming;

    /// The outgoing lepton.
    Particle _outgoing;

    /// The sign of the PZ of the incoming lepton
    double _sign;

  };

}


#endif

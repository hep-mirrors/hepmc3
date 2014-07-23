// -*- C++ -*-
#ifndef RIVET_NeutralFinalState_HH
#define RIVET_NeutralFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Project only neutral final state particles.
  class NeutralFinalState : public FinalState {

  public:

    /// @name Constructors
    //@{
    NeutralFinalState(const FinalState& fsp)  : _Etmin(0.0*GeV) {
      setName("NeutralFinalState");
      addProjection(fsp, "FS");
    }

    NeutralFinalState(double mineta = -MAXDOUBLE,
                      double maxeta =  MAXDOUBLE,
                      double minEt  =  0.0*GeV) : _Etmin(minEt)
    {
      setName("NeutralFinalState");
      addProjection(FinalState(mineta, maxeta, 0.0*GeV), "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new NeutralFinalState(*this);
    }
    //@}

  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// The minimum allowed transverse energy.
    double _Etmin;

    /// Compare projections.
    int compare(const Projection& p) const;
  };


}


#endif

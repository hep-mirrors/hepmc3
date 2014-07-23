// -*- C++ -*-
#ifndef RIVET_MergedFinalState_HH
#define RIVET_MergedFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Get final state particles merged from two FinalState projections.
  class MergedFinalState : public FinalState {

  public:

    /// @name Constructors
    //@{
    MergedFinalState(const FinalState& fspa, const FinalState& fspb) {
      setName("MergedFinalState");
      addProjection(fspa, "FSA");
      addProjection(fspb, "FSB");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new MergedFinalState(*this);
    }
    //@}

  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;
  };


}


#endif

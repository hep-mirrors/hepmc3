// -*- C++ -*-
#ifndef RIVET_PrimaryHadrons_HH
#define RIVET_PrimaryHadrons_HH

#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /// @brief Project out the first hadrons from hadronisation.
  ///
  /// @todo Also be able to return taus? Prefer a separate tau finder.
  /// @todo This assumes that the primary hadrons are unstable... should we also look for stable primary hadrons?
  class PrimaryHadrons : public FinalState {
  public:

    /// @name Constructors and destructors.
    //@{

    /// Constructor with specification of the minimum and maximum pseudorapidity
    /// \f$ \eta \f$ and the min \f$ p_T \f$ (in GeV).
    PrimaryHadrons(double mineta = -MAXDOUBLE,
                   double maxeta =  MAXDOUBLE,
                   double minpt  =  0.0*GeV) {
      setName("PrimaryHadrons");
      addProjection(UnstableFinalState(mineta, maxeta, minpt), "UFS");
    }


    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new PrimaryHadrons(*this);
    }

    //@}

  protected:

    /// Apply the projection to the event.
    virtual void project(const Event& e);

  };


}


#endif

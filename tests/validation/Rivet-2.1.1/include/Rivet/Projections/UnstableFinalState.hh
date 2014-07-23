// -*- C++ -*-
#ifndef RIVET_UnstableFinalState_HH
#define RIVET_UnstableFinalState_HH

#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /// @brief Project out all physical-but-decayed particles in an event.
  ///
  /// The particles returned by the UFS are unique unstable particles, such as
  /// hadrons which are decayed by the generator. If, for example, you set Ks
  /// and Lambda particles stable in the generator, they will not be returned by
  /// the UFS. Also, you should be aware that all unstable particles in a decay
  /// chain are returned: if you are looking for something like the number of B
  /// hadrons in an event and there is a decay chain from e.g. B** -> B, you
  /// will count both B mesons unless you are careful to check for
  /// ancestor/descendent relations between the particles. Duplicate particles
  /// in the event record, i.e. those which differ only in bookkeeping details
  /// or photon emissions, are stripped from the returned particles collection.
  class UnstableFinalState : public FinalState {
  public:

    /// @name Standard constructors and destructors.
    //@{

    /// The default constructor. May specify the minimum and maximum
    /// pseudorapidity \f$ \eta \f$ and the min \f$ p_T \f$ (in GeV).
    UnstableFinalState(double mineta = -MAXDOUBLE,
                       double maxeta =  MAXDOUBLE,
                       double minpt  =  0.0*GeV)
      : FinalState(mineta,maxeta,minpt)
    {
      setName("UnstableFinalState");
    }


    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new UnstableFinalState(*this);
    }

    //@}

  protected:

    /// Apply the projection to the event.
    virtual void project(const Event& e);

  };


}


#endif

// -*- C++ -*-
#ifndef RIVET_DISFinalState_HH
#define RIVET_DISFinalState_HH

#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/DISKinematics.hh"

namespace Rivet {


  /// @brief Final state particles boosted to the hadronic center of mass system.
  ///
  /// NB. The DIS scattered lepton is not included in the final state particles.
  class DISFinalState: public FinalState {
  public:

    /// Type of DIS boost to apply
    enum BoostType { HCM, BREIT };


    /// @name Constructors
    //@{

    /// Constructor
    DISFinalState(const DISKinematics& kinematicsp, BoostType boosttype)
      : _boosttype(boosttype)
    {
      setName("DISFinalState");
      addProjection(kinematicsp, "Kinematics");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new DISFinalState(*this);
    }

    //@}


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const {
      const DISFinalState& other = dynamic_cast<const DISFinalState&>(p);
      return mkNamedPCmp(p, "Kinematics") || cmp(_boosttype, other._boosttype);
    }


  private:

    BoostType _boosttype;

  };


}

#endif

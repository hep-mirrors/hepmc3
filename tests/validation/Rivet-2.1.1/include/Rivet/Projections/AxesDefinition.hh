// -*- C++ -*-
#ifndef RIVET_AxesDefinition_HH
#define RIVET_AxesDefinition_HH

#include "Rivet/Projection.hh"
#include "Rivet/Event.hh"

namespace Rivet {

  /**
    @brief Base class for projections which define a spatial basis.

    A pure virtual interface for projections which define a set of 3
    basis vectors. This allows e.g. Thrust and Sphericity to be used
    interchangeably as defining bases for e.g. hemisphere mass and broadening
    calculations.

    @author Andy Buckley
   */
  class AxesDefinition : public Projection {
  public:

    /// Virtual destructor.
    virtual ~AxesDefinition() { }

    /// Clone on the heap.
    virtual const Projection* clone() const = 0;

    ///@{ Axis accessors, in decreasing order of significance.
    /// The main axis.
    virtual const Vector3& axis1() const = 0;
    /// The 2nd most significant ("major") axis.
    virtual const Vector3& axis2() const = 0;
    /// The least significant ("minor") axis.
    virtual const Vector3& axis3() const = 0;
    ///@}

  };

}

#endif

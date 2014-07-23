// -*- C++ -*-
#ifndef RIVET_ConstLossyFinalState_HH
#define RIVET_ConstLossyFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/LossyFinalState.hh"

namespace Rivet {


  /// Functor used to implement constant random lossiness.
  class ConstRandomFilter {
  public:

    ConstRandomFilter(double lossFraction)
      : _lossFraction(lossFraction)
    {
      assert(_lossFraction >= 0);
    }

    // If operator() returns true, particle is deleted ("lost")
    bool operator()(const Particle&) {
      /// @todo Use a better RNG
      return (rand()/static_cast<double>(RAND_MAX) < _lossFraction);
    }

    int compare(const ConstRandomFilter& other) const {
      return cmp(_lossFraction, other._lossFraction);
    }

  private:

    double _lossFraction;

  };



  /// @brief Randomly lose a constant fraction of particles.
  class ConstLossyFinalState : public LossyFinalState<ConstRandomFilter> {
  public:

    /// @name Constructors
    //@{

    /// Constructor from a FinalState.
    ConstLossyFinalState(const FinalState& fsp, double lossfraction)
      : LossyFinalState<ConstRandomFilter>(fsp, ConstRandomFilter(lossfraction))
    {
      setName("ConstLossyFinalState");
    }

    /// Stand-alone constructor. Initialises the base FinalState projection.
    ConstLossyFinalState(double lossfraction,
                         double mineta = -MAXDOUBLE,
                         double maxeta = MAXDOUBLE,
                         double minpt = 0.0)
      : LossyFinalState<ConstRandomFilter>(ConstRandomFilter(lossfraction), mineta, maxeta, minpt)
    {
      setName("ConstLossyFinalState");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new ConstLossyFinalState(*this);
    }

    //@}

  };


}

#endif

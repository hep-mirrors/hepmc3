// -*- C++ -*-
#ifndef RIVET_CentralEtHCM_HH
#define RIVET_CentralEtHCM_HH

#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projections/DISFinalState.hh"

namespace Rivet {


  /// @brief Summed \f$ E_\perp \f$ of central particles in HCM system.
  ///
  /// Sum up \f$ E_\perp \f$ of all particles in the hadronic final state in the
  /// central rapidity bin of the HCM system.
  class CentralEtHCM : public Projection {
  public:

    /// The default constructor. Must specify a FinalStateHCM projection
    /// object which is guaranteed to live throughout the run.
    CentralEtHCM(const DISFinalState& fs)
    {
      setName("CentralEtHCM");
      addProjection(fs, "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new CentralEtHCM(*this);
    }


  protected:

    /// Apply the projection on to the Event.
    void project(const Event& e);

    /// Compare with other projections
    int compare(const Projection& p) const {
      return mkNamedPCmp(p, "FS");
    }


  public:

    /// The sum of the Et in the central rapidity bin.
    double sumEt() const { return _sumet; }


  private:

    /// The sum of the Et in the central rapidity bin.
    double _sumet;

  };

}


#endif

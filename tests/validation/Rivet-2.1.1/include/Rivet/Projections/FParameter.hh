// -*- C++ -*-
#ifndef RIVET_FParameter_HH
#define RIVET_FParameter_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Event.hh"


namespace Rivet {

  class FParameter : public Projection {

  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    FParameter(const FinalState& fsp);

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new FParameter(*this);
    }

    //@}


  protected:

    /// Perform the projection on the Event
    void project(const Event& e);

    /// Compare with other projections
    //int compare(const Projection& p) const;
    // Taken from Thrust.hh
    int compare(const Projection& p) const {
      return mkNamedPCmp(p, "FS");
    }
  public:

    /// Reset the projection
    void clear();

    /// @name Access the event shapes by name
    /// @{
    /// F-Parametr
    double F() const { return lambda1() >= lambda2() ? lambda2()/lambda1() : lambda1()/lambda2(); }
    /// @}

    /// @name Access the linearised transverse momentum tensor eigenvalues
    /// @{
    double lambda1() const { return _lambdas[0]; }
    double lambda2() const { return _lambdas[1]; }
    /// @}


    /// @name Direct methods
    /// Ways to do the calculation directly, without engaging the caching system
    //@{
 
    /// Manually calculate the sphericity, without engaging the caching system
    void calc(const FinalState& fs);

    /// Manually calculate the sphericity, without engaging the caching system
    void calc(const vector<Particle>& fsparticles);

    /// Manually calculate the sphericity, without engaging the caching system
    void calc(const vector<FourMomentum>& fsmomenta);

    /// Manually calculate the sphericity, without engaging the caching system
    void calc(const vector<Vector3>& fsmomenta);

    //@}
  private:
    /// Eigenvalues.
    vector<double> _lambdas;

  private:

    /// Actually do the calculation
    void _calcFParameter(const vector<Vector3>& fsmomenta);

  };
}


#endif

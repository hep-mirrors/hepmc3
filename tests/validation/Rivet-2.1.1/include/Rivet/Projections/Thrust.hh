// -*- C++ -*-
#ifndef RIVET_Thrust_HH
#define RIVET_Thrust_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/AxesDefinition.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /**
    @brief Get the e+ e- thrust basis and the thrust, thrust major and thrust minor scalars.

    @author Andy Buckley

    The scalar (maximum) thrust is defined as
    \f[
    T = \mathrm{max}_{\vec{n}} \frac{\sum_i \left|\vec{p}_i \cdot \vec{n} \right|}{\sum_i |\vec{p}_i|}
    \f],
    with the direction of the unit vector \f$ \vec{n} \f$ which maximises \f$ T \f$
    being identified as the thrust axis. The unit vector which maximises the thrust
    scalar in the plane perpendicular to \f$ \vec{n} \f$ is the "thrust major"
    direction, and the vector perpendicular to both the thrust and thrust major directions
    is the thrust minor. Both the major and minor directions have associated thrust
    scalars.

    Thrust calculations have particularly simple forms for less than 4 particles, and
    in those cases this projection is computationally minimal. For 4 or more particles,
    a more general calculation must be carried out, based on the Brandt/Dahmen method
    from Z. Phys. C1 (1978). While a polynomial improvement on the exponential scaling
    of the naive method, this algorithm scales asymptotically as
    \f$ \mathcal{O}\left( n^3 \right) \f$. Be aware that the thrust may easily be the
    most computationally demanding projection in Rivet for large events!

    The Rivet implementation of thrust is based heavily on Stefan Gieseke's Herwig++
    re-coding of the 'tasso' code from HERWIG.

    NB. special case with >= 4 coplanar particles will still fail.
    NB. Thrust assumes all momenta are in the CoM system: no explicit boost is performed.
      This can be dealt with by appropriate choice of the supplied FinalState.
   */
  class Thrust : public AxesDefinition {
  public:

    /// Constructor.
    Thrust() {}

    Thrust(const FinalState& fsp) {
      setName("Thrust");
      addProjection(fsp, "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new Thrust(*this);
    }

  protected:

    /// Perform the projection on the Event
    void project(const Event& e) {
      const vector<Particle> ps
        = applyProjection<FinalState>(e, "FS").particles();
      calc(ps);
    }

    /// Compare projections
    int compare(const Projection& p) const {
      return mkNamedPCmp(p, "FS");
    }


  public:

    ///@{ Thrust scalar accessors
    /// The thrust scalar, \f$ T \f$, (maximum thrust).
    double thrust() const { return _thrusts[0]; }
    /// The thrust major scalar, \f$ M \f$, (thrust along thrust major axis).
    double thrustMajor() const { return _thrusts[1]; }
    /// The thrust minor scalar, \f$ m \f$, (thrust along thrust minor axis).
    double thrustMinor() const { return _thrusts[2]; }
    /// The oblateness, \f$ O = M - m \f$ .
    double oblateness() const { return _thrusts[1] - _thrusts[2]; }
    ///@}

    ///@{ Thrust axis accessors
    /// The thrust axis.
    const Vector3& thrustAxis() const { return _thrustAxes[0]; }
    /// The thrust major axis (axis of max thrust perpendicular to thrust axis).
    const Vector3& thrustMajorAxis() const { return _thrustAxes[1]; }
    /// The thrust minor axis (axis perpendicular to thrust and thrust major).
    const Vector3& thrustMinorAxis() const { return _thrustAxes[2]; }
    ///@}

    ///@{ AxesDefinition axis accessors.
    const Vector3& axis1() const { return thrustAxis(); }
    const Vector3& axis2() const { return thrustMajorAxis(); }
    const Vector3& axis3() const { return thrustMinorAxis(); }
    ///@}


  public:

    /// @name Direct methods
    /// Ways to do the calculation directly, without engaging the caching system
    //@{

    /// Manually calculate the thrust, without engaging the caching system
    void calc(const FinalState& fs);

    /// Manually calculate the thrust, without engaging the caching system
    void calc(const vector<Particle>& fsparticles);

    /// Manually calculate the thrust, without engaging the caching system
    void calc(const vector<FourMomentum>& fsmomenta);

    /// Manually calculate the thrust, without engaging the caching system
    void calc(const vector<Vector3>& threeMomenta);

    //@}


  private:

    /// The thrust scalars.
    vector<double> _thrusts;

    /// The thrust axes.
    vector<Vector3> _thrustAxes;

  private:

    /// Explicitly calculate the thrust values.
    void _calcThrust(const vector<Vector3>& fsmomenta);

  };

}

#endif

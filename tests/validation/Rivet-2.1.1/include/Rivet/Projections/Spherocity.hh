// -*- C++ -*-
#ifndef RIVET_Spherocity_HH
#define RIVET_Spherocity_HH

#include "Rivet/Projection.hh"
#include "Rivet/Projections/AxesDefinition.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Event.hh"

namespace Rivet {


  /**
    @brief Get the transverse spherocity scalars for hadron-colliders.

    @author Holger Schulz

    The scalar (maximum) transverse spherocity is defined as
    \f[
    T = \mathrm{max}_{\vec{n_\perp}} \frac{\sum_i \left|\vec{p}_{\perp,i} \cdot \vec{n} \right|}{\sum_i |\vec{p}_{\perp,i}|}
    \f],
    with the direction of the unit vector \f$ \vec{n_\perp} \f$ which maximises \f$ T \f$
    being identified as the spherocity axis. The unit vector which maximises the spherocity
    scalar in the plane perpendicular to \f$ \vec{n} \f$ is the "spherocity major"
    direction, and the vector perpendicular to both the spherocity and spherocity major directions
    is the spherocity minor. Both the major and minor directions have associated spherocity
    scalars.

    Care must be taken in the case of Drell-Yan processes - there we should use the
    newly proposed observable \f$ a_T \f$.

   */
  class Spherocity : public AxesDefinition {
  public:

    // Default Constructor
    Spherocity() {}

    /// Constructor.
    Spherocity(const FinalState& fsp)
      : _calculatedSpherocity(false)
    {
      setName("Spherocity");
      addProjection(fsp, "FS");
    }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new Spherocity(*this);
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

    ///@{ Spherocity scalar accessors
    /// The spherocity scalar, \f$ S \f$, (minimum spherocity).
    double spherocity() const { return _spherocities[0]; }
    ///@}


    ///@{ Spherocity axis accessors
    /// The spherocity axis.
    const Vector3& spherocityAxis() const { return _spherocityAxes[0]; }
    /// The spherocity major axis (axis of max spherocity perpendicular to spherocity axis).
    const Vector3& spherocityMajorAxis() const { return _spherocityAxes[1]; }
    /// The spherocity minor axis (axis perpendicular to spherocity and spherocity major).
    const Vector3& spherocityMinorAxis() const { return _spherocityAxes[2]; }
    ///@}


    ///@{ AxesDefinition axis accessors.
    const Vector3& axis1() const { return spherocityAxis(); }
    const Vector3& axis2() const { return spherocityMajorAxis(); }
    const Vector3& axis3() const { return spherocityMinorAxis(); }
    ///@}


  public:

    /// @name Direct methods
    /// Ways to do the calculation directly, without engaging the caching system
    //@{

    /// Manually calculate the spherocity, without engaging the caching system
    void calc(const FinalState& fs);

    /// Manually calculate the spherocity, without engaging the caching system
    void calc(const vector<Particle>& fsparticles);

    /// Manually calculate the spherocity, without engaging the caching system
    void calc(const vector<FourMomentum>& fsmomenta);

    /// Manually calculate the spherocity, without engaging the caching system
    void calc(const vector<Vector3>& threeMomenta);

    //@}


  private:

    /// The spherocity scalars.
    vector<double> _spherocities;

    /// The spherocity axes.
    vector<Vector3> _spherocityAxes;

    /// Caching flag to avoid costly recalculations.
    bool _calculatedSpherocity;


  private:

    /// Explicitly calculate the spherocity values.
    void _calcSpherocity(const vector<Vector3>& fsmomenta);

  };

}

#endif

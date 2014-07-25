// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Tools/Logging.hh"


namespace Rivet {


  void Thrust::calc(const FinalState& fs) {
    calc(fs.particles());
  }

  void Thrust::calc(const vector<Particle>& fsparticles) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsparticles.size());
    foreach (const Particle& p, fsparticles) {
      threeMomenta.push_back( p.momentum().vector3() );
    }
    _calcThrust(threeMomenta);
  }

  void Thrust::calc(const vector<FourMomentum>& fsmomenta) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsmomenta.size());
    foreach (const FourMomentum& v, fsmomenta) {
      threeMomenta.push_back(v.vector3());
    }
    _calcThrust(threeMomenta);
  }

  void Thrust::calc(const vector<Vector3>& fsmomenta) {
    _calcThrust(fsmomenta);
  }


  /////////////////////////////////////////////////



  inline bool mod2Cmp(const Vector3& a, const Vector3& b) {
    return a.mod2() > b.mod2();
  }


  // Do the general case thrust calculation
  void _calcT(const vector<Vector3>& momenta, double& t, Vector3& taxis) {
    // This function implements the iterative algorithm as described in the
    // Pythia manual. We take eight (four) different starting vectors
    // constructed from the four (three) leading particles to make sure that
    // we don't find a local maximum.
    vector<Vector3> p = momenta;
    assert(p.size() >= 3);
    unsigned int n = 3;
    if (p.size() == 3) n = 3;
    vector<Vector3> tvec;
    vector<double> tval;
    std::sort(p.begin(), p.end(), mod2Cmp);
    for (int i = 0 ; i < intpow(2, n-1); ++i) {
      // Create an initial vector from the leading four jets
      Vector3 foo(0,0,0);
      int sign = i;
      for (unsigned int k = 0 ; k < n ; ++k) {
        (sign % 2) == 1 ? foo += p[k] : foo -= p[k];
        sign /= 2;
      }
      foo=foo.unit();

      // Iterate
      double diff=999.;
      while (diff>1e-5) {
        Vector3 foobar(0,0,0);
        for (unsigned int k=0 ; k<p.size() ; k++)
          foo.dot(p[k])>0 ? foobar+=p[k] : foobar-=p[k];
        diff=(foo-foobar.unit()).mod();
        foo=foobar.unit();
      }

      // Calculate the thrust value for the vector we found
      t=0.;
      for (unsigned int k=0 ; k<p.size() ; k++)
        t+=fabs(foo.dot(p[k]));

      // Store everything
      tval.push_back(t);
      tvec.push_back(foo);
    }

    // Pick the solution with the largest thrust
    t=0.;
    for (unsigned int i=0 ; i<tvec.size() ; i++)
      if (tval[i]>t){
        t=tval[i];
        taxis=tvec[i];
      }
  }



  // Do the full calculation
  void Thrust::_calcThrust(const vector<Vector3>& fsmomenta) {
    // Make a vector of the three-momenta in the final state
    double momentumSum(0.0);
    foreach (const Vector3& p3, fsmomenta) {
      momentumSum += mod(p3);
    }
    MSG_DEBUG("Number of particles = " << fsmomenta.size());


    // Clear the caches
    _thrusts.clear();
    _thrustAxes.clear();


    // If there are fewer than 2 visible particles, we can't do much
    if (fsmomenta.size() < 2) {
      for (int i = 0; i < 3; ++i) {
        _thrusts.push_back(-1);
        _thrustAxes.push_back(Vector3(0,0,0));
      }
      return;
    }


    // Handle special case of thrust = 1 if there are only 2 particles
    if (fsmomenta.size() == 2) {
      Vector3 axis(0,0,0);
      _thrusts.push_back(1.0);
      _thrusts.push_back(0.0);
      _thrusts.push_back(0.0);
      axis = fsmomenta[0].unit();
      if (axis.z() < 0) axis = -axis;
      _thrustAxes.push_back(axis);
      /// @todo Improve this --- special directions bad...
      /// (a,b,c) _|_ 1/(a^2+b^2) (b,-a,0) etc., but which combination minimises error?
      if (axis.z() < 0.75)
        _thrustAxes.push_back( (axis.cross(Vector3(0,0,1))).unit() );
      else
        _thrustAxes.push_back( (axis.cross(Vector3(0,1,0))).unit() );
      _thrustAxes.push_back( _thrustAxes[0].cross(_thrustAxes[1]) );
      return;
    }



    // Temporary variables for calcs
    Vector3 axis(0,0,0);
    double val = 0.;

    // Get thrust
    _calcT(fsmomenta, val, axis);
    MSG_DEBUG("Mom sum = " << momentumSum);
    _thrusts.push_back(val / momentumSum);
    // Make sure that thrust always points along the +ve z-axis.
    if (axis.z() < 0) axis = -axis;
    axis = axis.unit();
    MSG_DEBUG("Axis = " << axis);
    _thrustAxes.push_back(axis);

    // Get thrust major
    vector<Vector3> threeMomenta;
    foreach (const Vector3& v, fsmomenta) {
      // Get the part of each 3-momentum which is perpendicular to the thrust axis
      const Vector3 vpar = dot(v, axis.unit()) * axis.unit();
      threeMomenta.push_back(v - vpar);
    }
    _calcT(threeMomenta, val, axis);
    _thrusts.push_back(val / momentumSum);
    if (axis.x() < 0) axis = -axis;
    axis = axis.unit();
    _thrustAxes.push_back(axis);

    // Get thrust minor
    if (_thrustAxes[0].dot(_thrustAxes[1]) < 1e-10) {
      axis = _thrustAxes[0].cross(_thrustAxes[1]);
      _thrustAxes.push_back(axis);
      val = 0.0;
      foreach (const Vector3& v, fsmomenta) {
        val += fabs(dot(axis, v));
      }
      _thrusts.push_back(val / momentumSum);
    } else {
      _thrusts.push_back(-1.0);
      _thrustAxes.push_back(Vector3(0,0,0));
    }

  }



}

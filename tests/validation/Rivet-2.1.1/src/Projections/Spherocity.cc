// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projections/Spherocity.hh"
#include "Rivet/Tools/Logging.hh"


namespace Rivet {


  void Spherocity::calc(const FinalState& fs) {
    calc(fs.particles());
  }


  void Spherocity::calc(const vector<Particle>& fsparticles) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsparticles.size());
    foreach (const Particle& p, fsparticles) {
      threeMomenta.push_back( p.momentum().vector3() );
    }
    _calcSpherocity(threeMomenta);
  }


  void Spherocity::calc(const vector<FourMomentum>& fsmomenta) {
    vector<Vector3> threeMomenta;
    threeMomenta.reserve(fsmomenta.size());
    foreach (const FourMomentum& v, fsmomenta) {
      threeMomenta.push_back(v.vector3());
    }
    _calcSpherocity(threeMomenta);
  }


  void Spherocity::calc(const vector<Vector3>& fsmomenta) {
    _calcSpherocity(fsmomenta);
  }


  /////////////////////////////////////////////////


  //// Do the general case spherocity calculation
  void _calcS(const vector<Vector3 >& perpmomenta, double& sphero, Vector3& saxis) {
    // According to the Salam paper, p5, footnote 4, the
    // axis n that minimises the Spherocity value ALWAYS coincides with the
    // direction of one of the transverse momentum vectors of the events particles.
    // Thus we carry out the calculation of Sphero for all particles and pick the
    // one that yields the smallerst values

    vector<Vector3> p = perpmomenta;
    vector<double> sval;

    // Prepare vector to store unit vector representation of all particle momenta
    vector<Vector3> units;
    foreach (const Vector3& p, perpmomenta) {
      units.push_back(Vector3(p.x(), p.y(), 0.0).unit());
    }

    // Spherocity calculation
    //
    // Pick the solution with the smallest spherocity
    sphero = 99999.;
    // The outer loop is for iteration over all unit vectors
    foreach (const Vector3& u, units){
      double s =0.0;
      for (unsigned int k=0 ; k<p.size() ; k++) {
        s += fabs( p[k].cross(u).mod() );
      }
      if (s < sphero) {
        sphero = s;
        saxis = u;
      }
    }
  }

  // Do the full calculation
  void Spherocity::_calcSpherocity(const vector<Vector3>& fsmomenta) {

    // Make a vector of the three-momenta in the final state
    // Explicitly set the z-component (parallel to beam axis) to zero
    // This creates a 3D-vector representation of the transverse momentum
    // but takes the full information momentum vectors as input

    // A small iteration over full momenta but set z-coord. to 0.0 to get transverse momenta
    vector<Vector3> fsperpmomenta;
    foreach (const Vector3& p, fsmomenta) {
      fsperpmomenta.push_back(Vector3(p.x(), p.y(), 0.0));
    }

    // This returns the scalar sum of (transverse) momenta
    double perpmomentumSum(0.0);
    foreach (const Vector3& p, fsperpmomenta) {
      perpmomentumSum += mod(p);
    }

    // Clear the caches
    _spherocities.clear();
    _spherocityAxes.clear();


    // Temporary variables for calcs
    Vector3 axis(0,0,0);
    double val = 0.;

    // Get spherocity
    _calcS(fsperpmomenta, val, axis);
    MSG_DEBUG("Mom sum = " << perpmomentumSum);
    double spherocity = 0.25 * PI * PI * val * val / (perpmomentumSum * perpmomentumSum);
    _spherocities.push_back(spherocity);

    // See if calculated spherocity value makes sense
    if (spherocity < 0.0 || spherocity > 1.0) {
      MSG_WARNING("Spherocity = " << spherocity);
    }

    MSG_DEBUG("Spherocity value = " << spherocity);

    MSG_DEBUG("Sperocity axis = " << axis);

    _spherocityAxes.push_back(axis);


  }
}

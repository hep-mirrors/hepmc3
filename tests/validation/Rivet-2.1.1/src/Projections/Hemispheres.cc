// -*- C++ -*-
#include "Rivet/Projections/Hemispheres.hh"

namespace Rivet {


  void Hemispheres::project(const Event& e) {
    clear();

    // Get thrust axes.
    const AxesDefinition& ax = applyProjection<AxesDefinition>(e, "Axes");
    const Vector3 n = ax.axis1();
    MSG_DEBUG("Thrust axis = " << n);

    FourMomentum p4With, p4Against;
    double Evis(0), broadWith(0), broadAgainst(0), broadDenom(0);
    const FinalState& fs = applyProjection<FinalState>(e, ax.getProjection("FS"));
    const Particles& particles = fs.particles();
    MSG_DEBUG("Number of particles = " << particles.size());
    foreach (const Particle& p, particles) {
      const FourMomentum p4 = p.momentum();
      const Vector3 p3 = p4.vector3();
      const double p3Mag = mod(p3);
      const double p3Para = dot(p3, n);
      const double p3Trans = mod(p3 - p3Para * n);

      // Update normalisations
      Evis += p4.E();
      broadDenom += 2.0 * p3Mag;

      // Update the mass and broadening variables
      if (p3Para > 0) {
        p4With += p4;
        broadWith += p3Trans;
      } else if (p3Para < 0) {
        p4Against += p4;
        broadAgainst += p3Trans;
      } else {
        // In the incredibly unlikely event that a particle goes exactly along the
        // thrust plane, add half to each hemisphere.
        MSG_WARNING("Particle split between hemispheres");
        p4With += 0.5 * p4;
        p4Against += 0.5 * p4;
        broadWith += 0.5 * p3Trans;
        broadAgainst += 0.5 * p3Trans;
      }
    }

    // Visible energy squared.
    _E2vis = Evis * Evis;

    // Calculate masses.
    const double mass2With = p4With.mass2();
    const double mass2Against = p4Against.mass2();
    _M2high = max(mass2With, mass2Against);
    _M2low = min(mass2With, mass2Against);

    // Calculate broadenings.
    broadWith /= broadDenom;
    broadAgainst /= broadDenom;
    _Bmax = max(broadWith, broadAgainst);
    _Bmin = min(broadWith, broadAgainst);

    // Calculate high-max correlation flag.
    const int maxMassID = (mass2With >= mass2Against);
    const int maxBroadID = (broadWith >= broadAgainst);
    _highMassEqMaxBroad = (maxMassID == maxBroadID);
  }


}

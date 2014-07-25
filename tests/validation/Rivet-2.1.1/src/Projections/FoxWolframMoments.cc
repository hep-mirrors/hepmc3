// -*- C++ -*-
#include "Rivet/Projections/FoxWolframMoments.hh"

namespace Rivet {

    int FoxWolframMoments::compare(const Projection& p) const {
        return mkNamedPCmp(p, "FS");
    }


    void FoxWolframMoments::project(const Event& e) {
      // Project into final state and get total visible momentum
      const FinalState& fs = applyProjection<FinalState>(e, "VFS");

      // remember: # pairs = N! / ( r! * (N-r)! )
      // N.B.: Autocorrelations are included! Treat them separately as diagonal elements.
      // see: http://cepa.fnal.gov/psm/simulation/mcgen/lund/pythia_manual/pythia6.3/pythia6301/node215.html

      double sumEnergy = 0.0;
      for (Particles::const_iterator pi = fs.particles().begin(); pi != fs.particles().end(); ++pi) {
        sumEnergy += pi->momentum().E();
        const FourMomentum pi_4 = pi->momentum();
        for (Particles::const_iterator pj = pi+1; pj != fs.particles().end(); ++pj) {
          const FourMomentum pj_4 = pj->momentum();

          // Calculate x_ij = cos(theta_ij)
          double x_ij = 1.0;
          if ( pi != pj ) {
            double denom =  pi_4.vector3().mod() * pj_4.vector3().mod();
            x_ij = pi_4.vector3().dot( pj_4.vector3() ) / denom;
          }

          //const double core = fabs( pi_4 * pj_4 ); //  / sumet2 ;
          const double core = pi_4.vector3().mod() * pi_4.vector3().mod();

          for (int order = 0; order < MAXMOMENT; ++order) {
            // enter a factor 2.0 because ij = ji. Use symmetry to speed up!
            _fwmoments[order] += 2.0 * core * gsl_sf_legendre_Pl( order, x_ij ) ;
          }
        } // end loop over p_j

        // Now add autocorrelations
        // Obviously cos(theta_ij) = 1.0
        // Note that P_l(1) == 1 for each l
        for (int order = 0; order < MAXMOMENT; ++order) {
          _fwmoments[order] += fabs( pi_4 * pi_4 );
        }
      } // end loop over p_i

      MSG_DEBUG("sumEnergy = " << sumEnergy);

      for (int order = 0; order < MAXMOMENT; ++order) {
        _fwmoments[order] /= (sumEnergy*sumEnergy);
      }

      // Normalize to H0
      for (int order = 1; order < MAXMOMENT; ++order) {
        _fwmoments[order] /= _fwmoments[0];
      }
    }

}

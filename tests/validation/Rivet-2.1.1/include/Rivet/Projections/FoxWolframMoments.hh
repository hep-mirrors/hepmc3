// -*- C++ -*-
#ifndef RIVET_FoxWolframMoments_HH
#define RIVET_FoxWolframMoments_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"

#include <gsl/gsl_sf_legendre.h>

#define MAXMOMENT 5

namespace Rivet {


  /// @brief Calculate Fox-Wolfram moments
  class FoxWolframMoments : public Projection {
  public:

    /// Constructor.
    FoxWolframMoments(const FinalState& fsp)
    {
      setName("FoxWolframMoments");
      addProjection(fsp, "FS");

      /// @todo Let the user supply any projection they like?
      VisibleFinalState vfs(fsp);
      addProjection(vfs, "VFS");

      // Initialize moments vector
      for (int i = 0; i < MAXMOMENT ; ++i) {
        _fwmoments.push_back(0.0);
      }
    }


    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new FoxWolframMoments(*this);
    }


  public:

    /// The projected Fox-Wolfram Moment of order l
    double getFoxWolframMoment(unsigned int l) const {
      if (l < MAXMOMENT) {
        return _fwmoments[l];
      }
      /// @todo What?!?
      return -666.0;
    }


  protected:

    /// Apply the projection to the event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;


  private:

    vector<double> _fwmoments;

  };


}


#endif

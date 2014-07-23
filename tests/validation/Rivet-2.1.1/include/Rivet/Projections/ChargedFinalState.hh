// -*- C++ -*-
#ifndef RIVET_ChargedFinalState_HH
#define RIVET_ChargedFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Project only charged final state particles.
  class ChargedFinalState : public FinalState {
  public:

    /// @name Constructors
    //@{
    ChargedFinalState(const FinalState& fsp);

    /// Single eta-range constructor.
    ChargedFinalState(double mineta = -MAXDOUBLE,
                      double maxeta =  MAXDOUBLE,
                      double minpt  =  0.0*GeV);

    /// A constructor which allows to specify multiple eta ranges
    /// and the min \f$ p_T \f$.
    ChargedFinalState(const vector<pair<double, double> >& etaRanges,
                      double minpt = 0.0*GeV);

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new ChargedFinalState(*this);
    }
    //@}


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;
  };


}


#endif

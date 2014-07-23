// -*- C++ -*-
#ifndef RIVET_TriggerUA5_HH
#define RIVET_TriggerUA5_HH

#include "Rivet/Projection.hh"
#include "Rivet/Event.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Projections/Beam.hh"

namespace Rivet {


  /// @brief Access to the min bias triggers used by UA5
  class TriggerUA5 : public Projection {
  public:

    /// Default constructor.
    TriggerUA5();

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new TriggerUA5(*this);
    }


  public:

    /// The trigger result for non-single diffractive (2 arm) trigger
    bool sdDecision() const {
      return _decision_sd;
    }

    /// The trigger result for non-single diffractive (2 arm) trigger
    bool nsdDecision() const {
      return _decision_nsd_1;
    }

    /// The trigger result for non-single diffractive (2 arm) trigger
    /// with special ">= 2" trigger for ppbar bg rejection
    bool nsd2Decision() const {
      return _decision_nsd_2;
    }

    /// The trigger result
    bool samebeams() const {
      return _samebeams;
    }

    /// Number of hits in <-,+> eta hodoscopes
    pair<unsigned int, unsigned int> numHits() {
      return make_pair(_n_plus, _n_minus);
    }

    /// Project on to the event
    void project(const Event& evt);


  protected:

    /// Compare with other projections.
    virtual int compare(const Projection& UNUSED(p)) const {
      return EQUIVALENT;
    }


  private:

    /// The min bias trigger decisions
    bool _decision_sd, _decision_nsd_1, _decision_nsd_2;

    /// Is it a pp collision?
    bool _samebeams;

    /// Number of hits in hodoscopes
    unsigned int _n_plus, _n_minus;

  };


}

#endif

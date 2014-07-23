// -*- C++ -*-
#ifndef RIVET_LossyFinalState_HH
#define RIVET_LossyFinalState_HH

#include "Rivet/Tools/Logging.hh"
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Particle.hh"
#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Templated FS projection which can lose some of the supplied particles.
  template <typename FILTER>
  class LossyFinalState : public FinalState {
  public:

    /// @name Constructors
    //@{

    /// Constructor from FinalState.
    LossyFinalState(const FinalState& fsp, FILTER filter)
      : _filter(filter)
    {
      setName("LossyFinalState");
      addProjection(fsp, "FS");
    }

    /// Stand-alone constructor. Initialises the base FinalState projection.
    LossyFinalState(FILTER filter,
                    double mineta = -MAXDOUBLE,
                    double maxeta = MAXDOUBLE,
                    double minpt = 0.0)
      : _filter(filter)
    {
      setName("LossyFinalState");
      addProjection(FinalState(mineta, maxeta, minpt), "FS");
    }

    /// Virtual destructor, to allow subclassing
    virtual ~LossyFinalState() { }

    /// Clone on the heap.
    virtual const Projection* clone() const {
      return new LossyFinalState<FILTER>(*this);
    }

    //@}


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e) {
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      getLog() << Log::DEBUG << "Pre-loss number of FS particles = " << fs.particles().size() << endl;
      _theParticles.clear();
      std::remove_copy_if(fs.particles().begin(), fs.particles().end(),
                          std::back_inserter(_theParticles), _filter);
      getLog() << Log::DEBUG << "Filtered number of FS particles = " << _theParticles.size() << endl;
    }


    /// Compare projections.
    int compare(const Projection& p) const {
      const LossyFinalState<FILTER>& other = pcast< LossyFinalState<FILTER> >(p);
      const int fscmp = mkNamedPCmp(other, "FS");
      if (fscmp) return fscmp;
      return _filter.compare(other._filter);
    }


  protected:

    /// Filtering object: must support operator(const Particle&) and compare(const Filter&)
    FILTER _filter;

  };


}

#endif

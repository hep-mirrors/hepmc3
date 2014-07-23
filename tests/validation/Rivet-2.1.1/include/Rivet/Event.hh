// -*- C++ -*-
#ifndef RIVET_Event_HH
#define RIVET_Event_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Tools/RivetBoost.hh"

namespace Rivet {


  /// Rivet wrapper for HepMC event and Projection references.
  ///
  /// Event is a concrete class representing an generated event in
  /// Rivet. It is constructed given a HepMC::GenEvent, a pointer to
  /// which is kept by the Event object throughout its lifetime. The user
  /// must therefore make sure that the corresponding HepMC::GenEvent
  /// will persist at least as long as the Event object.
  ///
  /// In addition to the HepMC::GenEvent object the Event also keeps
  /// track of all Projections object which have been applied to the
  /// Event so far.
  class Event {
  public:

    /// @name Standard constructors and destructors.
    //@{

    /// Constructor from a HepMC GenEvent reference
    Event(const GenEvent& ge)
      : _originalGenEvent(&ge), _genEvent(ge)
    { _init(ge); }

    /// Constructor from a HepMC GenEvent pointer
    Event(const GenEvent* ge)
      : _originalGenEvent(ge), _genEvent(*ge)
    { assert(ge); _init(*ge); }

    /// Copy constructor
    Event(const Event& e)
      : _originalGenEvent(e._originalGenEvent), _genEvent(e._genEvent)
    {  }

    //@}


  public:

    /// The generated event obtained from an external event generator
    const GenEvent* genEvent() const {
      return &_genEvent;
    }

    /// @brief The generation weight associated with the event
    ///
    /// @todo This needs to be revisited when we finally add the mechanism to
    /// support NLO counter-events and weight vectors.
    double weight() const {
      return (!_genEvent.weights().empty()) ? _genEvent.weights()[0] : 1.0;
    }


  public:

    /// @brief Add a projection @a p to this Event.
    ///
    /// If an equivalent Projection has been applied before, the
    /// Projection::project(const Event&) of @a p is not called and a reference
    /// to the previous equivalent projection is returned. If no previous
    /// Projection was found, the Projection::project(const Event&) of @a p is
    /// called and a reference to @a p is returned.
    template <typename PROJ>
    const PROJ& applyProjection(PROJ& p) const {
      const Projection* cpp(&p);
      std::set<const Projection*>::const_iterator old = _projections.find(cpp);
      if (old != _projections.end()) {
        const Projection& pRef = **old;
        return pcast<PROJ>(pRef);
      }
      // Add the projection via the Projection base class (only
      // possible because Event is a friend of Projection)
      Projection* pp = const_cast<Projection*>(cpp);
      pp->project(*this);
      _projections.insert(pp);
      return p;
    }


    template <typename PROJ>
    const PROJ& applyProjection(PROJ* pp) const {
      if (!pp) throw Error("Event::applyProjection(PROJ*): Projection pointer is null.");
      return applyProjection(*pp);
    }


  private:

    /// @brief Actual (shared) implementation of the constructors from GenEvents
    ///
    /// @todo When we can require C++11, constructors can call each other and
    /// this can be removed.
    void _init(const GenEvent& ge);

    /// @brief Convert the GenEvent to use conventional alignment
    ///
    /// For example, FHerwig only produces DIS events in the unconventional
    /// hadron-lepton orientation and has to be corrected for DIS analysis
    /// portability.
    void _geNormAlignment();

    /// @brief The generated event, as obtained from an external generator.
    ///
    /// This is the original GenEvent. In practise the version seen by users
    /// will often/always be a modified one.
    ///
    /// @todo Provide access to this via an Event::originalGenEvent() method? If requested...
    const GenEvent* _originalGenEvent;

    /// @brief The GenEvent used by Rivet analysis projections etc.
    ///
    /// This version may be rotated to a "normal" alignment, have
    /// generator-specific particles stripped out, etc.  If an analysis is
    /// affected by these modifications, it is probably an unphysical analysis!
    ///
    /// Stored as a non-pointer since it may get overwritten, and memory for
    /// copying and cleanup is much neater this way.
    GenEvent _genEvent;

    /// The set of Projection objects applied so far.
    mutable std::set<ConstProjectionPtr> _projections;

  };


}

#endif

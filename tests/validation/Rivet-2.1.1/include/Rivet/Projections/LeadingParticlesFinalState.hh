// -*- C++ -*-
#ifndef RIVET_LeadingParticlesFinalState_HH
#define RIVET_LeadingParticlesFinalState_HH

#include "Rivet/Event.hh"
#include "Rivet/Projection.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {

  class Particle;

  /// @brief Get the highest-pT occurrences of FS particles with the specified PDG IDs.
  class LeadingParticlesFinalState : public FinalState {

  public:

    /// Constructor: the supplied FinalState projection is assumed to live through the run.
    LeadingParticlesFinalState(const FinalState& fsp)
      :  FinalState(), _leading_only(false)
    {
      setName("LeadingParticlesFinalState");
      addProjection(fsp, "FS");
    }

    /// Clone on the heap.
    virtual const Projection *clone() const {
      return new LeadingParticlesFinalState(*this);
    }

    /// Add a particle ID to the list of leading particles selected
    LeadingParticlesFinalState& addParticleId(long id) {
      _ids.insert(id);
      return *this;
    }

    /// Add a particle ID to the list of leading particles selected
    LeadingParticlesFinalState& addParticleIdPair(long id) {
      _ids.insert(id);
      _ids.insert(-id);
      return *this;
    }

    /// Toggle whether to keep track only of the leading particle of any ID,
    /// or the leading particle of all IDs separately
    /// Default is the latter (=false)
    void setLeadingOnly(const bool& leadingonly) {
      _leading_only = leadingonly;
    }

    // /// Check if a particle of a particular ID was found in the current event
    // bool hasParticleId(const PdgId pid) const;

    // /// Get a particle of a particular ID (check it exists first)
    // bool get(const PdgId pid) const;


  protected:

    /// Apply the projection on the supplied event.
    void project(const Event& e);

    /// Compare projections.
    int compare(const Projection& p) const;

    /// Check if the particle's ID is in the list
    bool inList(const Particle& particle) const;

  private:

    /// IDs of the leading particles to be selected
    std::set<long>_ids;
    bool _leading_only;

  };

}

#endif

// -*- C++ -*-
#ifndef RIVET_Jet_HH
#define RIVET_Jet_HH

#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Jet.fhh"
#include "Rivet/Particle.hh"
#include "fastjet/PseudoJet.hh"
#include <numeric>

namespace Rivet {


  /// @brief Representation of a clustered jet of particles.
  class Jet : public ParticleBase {
  public:

    /// @name Constructors
    //@{

    Jet() : ParticleBase() { clear(); }

    /// Set all the jet data, with full particle information.
    Jet(const vector<Particle>& particles, const FourMomentum& pjet)
      : ParticleBase() {
      setState(particles, pjet);
    }

    /// @todo Add a constructor from PseudoJet
    // operator Jet(const PseudoJet&) { ... }

    //@}


    /// @name Access jet constituents
    //@{

    /// Number of particles in this jet.
    size_t size() const { return _particles.size(); }

    /// Get the particles in this jet.
    vector<Particle>& particles() { return _particles; }

    /// Get the particles in this jet (const version)
    const vector<Particle>& particles() const { return _particles; }

    /// Check whether this jet contains a particular particle.
    bool containsParticle(const Particle& particle) const;

    /// Check whether this jet contains a certain particle type.
    bool containsParticleId(PdgId pid) const;

    /// Check whether this jet contains at least one of certain particle types.
    bool containsParticleId(const vector<PdgId>& pids) const;

    /// Check whether this jet contains a charm-flavoured hadron (or decay products from one).
    bool containsCharm() const;

    /// Check whether this jet contains a bottom-flavoured hadron (or decay products from one).
    bool containsBottom() const;

    //@}


    /// @name Access additional effective jet 4-vector properties
    //@{

    /// Get equivalent single momentum four-vector.
    const FourMomentum& momentum() const { return _momentum; }

    /// Get the total energy of this jet.
    double totalEnergy() const { return momentum().E(); }

    /// Get the energy carried in this jet by neutral particles.
    double neutralEnergy() const;

    /// Get the energy carried in this jet by hadrons.
    double hadronicEnergy() const;

    //@}


    // /// @name Interaction with FastJet
    // //@{

    // /// @todo Add a cast operator to FJ3 PseudoJet
    // operator const PseudoJet& () const { return pseudojet(); }

    // /// @todo Add a cast operator to FJ3 PseudoJet
    // const PseudoJet& pseudojet() const { return _pseudojet; }

    // //@}


    /// @name Set the jet constituents and properties
    //@{

    /// Set all the jet data, with full particle information.
    Jet& setState(const vector<Particle>& particles, const FourMomentum& pjet);

    /// Set the effective 4-momentum of the jet.
    Jet& setMomentum(const FourMomentum& momentum);

    /// Set the particles collection with full particle information.
    Jet& setParticles(const vector<Particle>& particles);

    /// Reset this jet as empty.
    Jet& clear();

    //@}


  private:

    /// @todo Add a FJ3 PseudoJet member to unify PseudoJet and Jet
    // PseudoJet _pseudojet;

    /// Full particle information including tracks, ID etc. (caching PseudoJet properties)
    Particles _particles;

    /// Effective jet 4-vector (caching PseudoJet properties)
    FourMomentum _momentum;

  };


}

#endif

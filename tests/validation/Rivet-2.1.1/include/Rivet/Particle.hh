// -*- C++ -*-
#ifndef RIVET_Particle_HH
#define RIVET_Particle_HH

#include "Rivet/Particle.fhh"
#include "Rivet/ParticleBase.hh"
#include "Rivet/Config/RivetCommon.hh"

namespace Rivet {


  /// Representation of particles from a HepMC3::GenEvent.
  class Particle : public ParticleBase {
  public:

    /// Default constructor.
    /// @deprecated A particle without info is useless. This only exists to keep STL containers happy.
    Particle()
      : ParticleBase(),
        _original(0), _id(0), _momentum()
    { }

    /// Constructor without GenParticle.
    Particle(PdgId pid, const FourMomentum& mom)
      : ParticleBase(),
        _original(0), _id(pid), _momentum(mom)
    { }
/*
    /// Constructor from a HepMC3 GenParticle.
    Particle(const GenParticle& gp)
      : ParticleBase(),
        _original(&gp), _id(gp.pdg_id()),
        _momentum(gp.momentum())
    { }
*/

    /// Constructor from a HepMC3 GenParticle pointer.
    Particle(const GenParticlePtr gp)
      : ParticleBase(),
        _original(gp), _id(gp->pdg_id()),
        _momentum(gp->momentum())
    { }


  public:

    /// @name Basic particle specific properties
    //@{

    /// Get a const reference to the original GenParticle.
    const GenParticlePtr genParticle() const {
      return _original;
    }

    /// Cast operator for conversion to GenParticlePtr
    operator const GenParticlePtr () const { return genParticle(); }

    /// The momentum.
    const FourMomentum& momentum() const {
      return _momentum;
    }
    /// Set the momentum.
    Particle& setMomentum(const FourMomentum& momentum) {
      _momentum = momentum;
      return *this;
    }

    //@}


    /// @name Particle ID code accessors
    //@{

    /// This Particle's PDG ID code.
    PdgId pid() const { return _id; }
    /// Absolute value of the PDG ID code.
    PdgId abspid() const { return _id; }
    /// This Particle's PDG ID code (alias).
    /// @deprecatedThe pid/abspid form is nicer (don't need to remember
    ///    lower/upper case, doesn't visually stick out in the code, etc. ...)
    PdgId pdgId() const { return _id; }

    //@}


    /// @name Particle species properties
    //@{

    /// The charge of this Particle.
    double charge() const {
      return PID::charge(pdgId());
    }
    /// Three times the charge of this Particle (i.e. integer multiple of smallest quark charge).
    int threeCharge() const {
      return PID::threeCharge(pdgId());
    }

    /// Is this a hadron?
    bool isHadron() const { return PID::isHadron(pdgId()); }

    /// Is this a meson?
    bool isMeson() const { return PID::isMeson(pdgId()); }

    /// Is this a baryon?
    bool isBaryon() const { return PID::isBaryon(pdgId()); }

    /// Is this a lepton?
    bool isLepton() const { return PID::isLepton(pdgId()); }

    /// Is this a neutrino?
    bool isNeutrino() const { return PID::isNeutrino(pdgId()); }

    /// Does this (hadron) contain a b quark?
    bool hasBottom() const { return PID::hasBottom(pdgId()); }

    /// Does this (hadron) contain a c quark?
    bool hasCharm() const { return PID::hasCharm(pdgId()); }

    // /// Does this (hadron) contain an s quark?
    // bool hasStrange() const { return PID::hasStrange(pdgId()); }

    //@}


    /// @name Ancestry properties
    //@{

    /// Check whether a given PID is found in the GenParticle's ancestor list
    ///
    /// @note This question is valid in MC, but may not be answerable
    /// experimentally -- use this function with care when replicating
    /// experimental analyses!
    bool hasAncestor(PdgId pdg_id) const;

    /// @brief Determine whether the particle is from a hadron or tau decay
    ///
    /// Specifically, walk up the ancestor chain until a status 2 hadron or
    /// tau is found, if at all.
    ///
    /// @note This question is valid in MC, but may not be perfectly answerable
    /// experimentally -- use this function with care when replicating
    /// experimental analyses!
    bool fromDecay() const;

    /// @brief Determine whether the particle is from a b-hadron decay
    ///
    /// @note This question is valid in MC, but may not be perfectly answerable
    /// experimentally -- use this function with care when replicating
    /// experimental analyses!
    bool fromBottom() const;

    /// @brief Determine whether the particle is from a c-hadron decay
    ///
    /// @note If a hadron contains b and c quarks it is considered a bottom
    /// hadron and NOT a charm hadron.
    ///
    /// @note This question is valid in MC, but may not be perfectly answerable
    /// experimentally -- use this function with care when replicating
    /// experimental analyses!
    bool fromCharm() const;

    // /// @brief Determine whether the particle is from a s-hadron decay
    // ///
    // /// @note If a hadron contains b or c quarks as well as strange it is
    // /// considered a b or c hadron, but NOT a strange hadron.
    // ///
    // /// @note This question is valid in MC, but may not be perfectly answerable
    // /// experimentally -- use this function with care when replicating
    // /// experimental analyses!
    // bool fromStrange() const;

    /// @brief Determine whether the particle is from a tau decay
    ///
    /// @note This question is valid in MC, but may not be perfectly answerable
    /// experimentally -- use this function with care when replicating
    /// experimental analyses!
    bool fromTau() const;

    //@}


    /// @name Decay info
    //@{

    /// Whether this particle is stable according to the generator
    bool isStable() const {
      return genParticle() && genParticle()->status() == 1 && !genParticle()->end_vertex();
    }

    /// Get a list of the direct descendants from the current particle
    vector<Particle> children() const {
      vector<Particle> rtn;
      if (isStable()) return rtn;
      /// @todo Remove this const mess crap when HepMC3 doesn't suck
      HepMC3::GenVertexPtr gv = genParticle()->end_vertex();
      /// @todo Would like to do this, but the range objects are broken
      // foreach (const GenParticlePtr gp, gv->particles(HepMC3::children))
      //   rtn += Particle(gp);
      for (GenVertex::particle_iterator it = gv->particles_begin(HepMC3::children); it != gv->particles_end(HepMC3::children); ++it)
        rtn += Particle(*it);
      return rtn;
    }

    /// Get a list of all the descendants (including duplication of parents and children) from the current particle
    /// @todo Use recursion through replica-avoiding MCUtils functions to avoid bookkeeping duplicates
    /// @todo Insist that the current particle is post-hadronization, otherwise throw an exception?
    vector<Particle> allDescendants() const {
      vector<Particle> rtn;
      if (isStable()) return rtn;
      /// @todo Remove this const mess crap when HepMC3 doesn't suck
      HepMC3::GenVertexPtr gv = genParticle()->end_vertex();
      /// @todo Would like to do this, but the range objects are broken
      // foreach (const GenParticlePtr gp, gv->particles(HepMC3::descendants))
      //   rtn += Particle(gp);
      for (GenVertex::particle_iterator it = gv->particles_begin(HepMC3::descendants); it != gv->particles_end(HepMC3::descendants); ++it)
        rtn += Particle(*it);
      return rtn;
    }

    /// Get a list of all the stable descendants from the current particle
    /// @todo Use recursion through replica-avoiding MCUtils functions to avoid bookkeeping duplicates
    /// @todo Insist that the current particle is post-hadronization, otherwise throw an exception?
    vector<Particle> stableDescendants() const {
      vector<Particle> rtn;
      if (isStable()) return rtn;
      /// @todo Remove this const mess crap when HepMC3 doesn't suck
      HepMC3::GenVertexPtr gv = genParticle()->end_vertex();
      /// @todo Would like to do this, but the range objects are broken
      // foreach (const GenParticlePtr gp, gv->particles(HepMC3::descendants))
      //   if (gp->status() == 1 && gp->end_vertex() == NULL)
      //     rtn += Particle(gp);
      for (GenVertex::particle_iterator it = gv->particles_begin(HepMC3::descendants); it != gv->particles_end(HepMC3::descendants); ++it)
        if ((*it)->status() == 1 && !(*it)->end_vertex())
          rtn += Particle(*it);
      return rtn;
    }

    /// Flight length (divide by mm or cm to get the appropriate units)
    double flightLength() const {
      if (isStable()) return -1;
      if (!genParticle()) return 0;
      if (!genParticle()->production_vertex()) return 0;
      const HepMC3::FourVector v1 = genParticle()->production_vertex()->position();
      const HepMC3::FourVector v2 = genParticle()->end_vertex()->position();
      return sqrt(sqr(v2.x()-v1.x()) + sqr(v2.y()-v1.y()) + sqr(v2.z()-v1.z()));
    }

    //@}


  private:

    /// A pointer to the original GenParticle from which this Particle is projected.
    GenParticlePtr _original;

    /// The PDG ID code for this Particle.
    PdgId _id;

    /// The momentum of this projection of the Particle.
    FourMomentum _momentum;

  };


  /// @name String representation
  //@{

  /// Print a ParticlePair as a string.
  inline std::string to_str(const ParticlePair& pair) {
    stringstream out;
    out << "["
        << PID::toParticleName(pair.first.pdgId()) << " @ "
        << pair.first.momentum().E()/GeV << " GeV, "
        << PID::toParticleName(pair.second.pdgId()) << " @ "
        << pair.second.momentum().E()/GeV << " GeV]";
    return out.str();
  }

  /// Allow ParticlePair to be passed to an ostream.
  inline std::ostream& operator<<(std::ostream& os, const ParticlePair& pp) {
    os << to_str(pp);
    return os;
  }

  //@}


}

#endif

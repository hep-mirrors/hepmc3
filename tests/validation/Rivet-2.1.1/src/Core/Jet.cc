#include "Rivet/Jet.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Tools/RivetBoost.hh"
#include "Rivet/ParticleName.hh"

namespace Rivet {


  Jet& Jet::setState(const vector<Particle>& particles, const FourMomentum& pjet) {
    setParticles(particles);
    setMomentum(pjet);
    return *this;
  }


  Jet& Jet::setMomentum(const FourMomentum& momentum) {
    _momentum = momentum;
    return *this;
  }


  Jet& Jet::setParticles(const vector<Particle>& particles) {
    _particles = particles;
    return *this;
  }


  bool Jet::containsParticle(const Particle& particle) const {
    const int barcode = particle.genParticle()->barcode();
    foreach (const Particle& p, particles()) {
      if (p.genParticle()->barcode() == barcode) return true;
    }
    return false;
  }


  bool Jet::containsParticleId(PdgId pid) const {
    foreach (const Particle& p, particles()) {
      if (p.pdgId() == pid) return true;
    }
    return false;
  }


  bool Jet::containsParticleId(const vector<PdgId>& pids) const {
    foreach (const Particle& p, particles()) {
      foreach (PdgId pid, pids) {
        if (p.pdgId() == pid) return true;
      }
    }
    return false;
  }


  /// @todo Jet::containsMatch(Matcher m) { ... if m(pid) return true; ... }


  double Jet::neutralEnergy() const {
    double e_neutral = 0.0;
    foreach (const Particle& p, particles()) {
      const PdgId pid = p.pdgId();
      if (PID::threeCharge(pid) == 0) {
        e_neutral += p.E();
      }
    }
    return e_neutral;
  }


  double Jet::hadronicEnergy() const {
    double e_hadr = 0.0;
    foreach (const Particle& p, particles()) {
      const PdgId pid = p.pdgId();
      if (PID::isHadron(pid)) {
        e_hadr += p.E();
      }
    }
    return e_hadr;
  }


  bool Jet::containsCharm() const {
    foreach (const Particle& p, particles()) {
      const PdgId pid = p.pdgId();
      if (abs(pid) == PID::CQUARK) return true;
      if (PID::isHadron(pid) && PID::hasCharm(pid)) return true;
      HepMC3::GenVertexPtr gv = p.genParticle()->production_vertex();
      if (gv) {
        foreach (const GenParticlePtr pi, Rivet::particles(gv, HepMC3::ancestors)) {
          const PdgId pid2 = pi->pdg_id();
          if (PID::isHadron(pid2) && PID::hasCharm(pid2)) return true;
        }
      }
    }
    return false;
  }


  bool Jet::containsBottom() const {
    foreach (const Particle& p, particles()) {
      const PdgId pid = p.pdgId();
      if (abs(pid) == PID::BQUARK) return true;
      if (PID::isHadron(pid) && PID::hasBottom(pid)) return true;
      HepMC3::GenVertexPtr gv = p.genParticle()->production_vertex();
      if (gv) {
        foreach (const GenParticlePtr pi, Rivet::particles(gv, HepMC3::ancestors)) {
          const PdgId pid2 = pi->pdg_id();
          if (PID::isHadron(pid2) && PID::hasBottom(pid2)) return true;
        }
      }
    }
    return false;
  }


  Jet& Jet::clear() {
    //_momenta.clear();
    _particles.clear();
    _momentum = FourMomentum();
    return *this;
  }


}

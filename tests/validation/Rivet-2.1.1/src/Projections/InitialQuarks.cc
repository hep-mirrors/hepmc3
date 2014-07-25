// -*- C++ -*-
#include "Rivet/Projections/InitialQuarks.hh"


#define IS_PARTON_PDGID(id) ( abs(id) <= 100 && abs(id) != 22 && (abs(id) < 11 || abs(id) > 18) )


namespace Rivet {


  int InitialQuarks::compare(const Projection& p) const {
    return EQUIVALENT;
  }


  void InitialQuarks::project(const Event& e) {
    _theParticles.clear();

    foreach (const GenParticlePtr p, Rivet::particles(e.genEvent())) {
      const GenVertexPtr pv = p->production_vertex();
      const GenVertexPtr dv = p->end_vertex();
      const PdgId pid = abs(p->pdg_id());
      bool passed = inRange((long)pid, 1, 5);
      if (passed) {
        if (pv) {
          foreach (const GenParticlePtr pp, particles_in(pv)) {
            // Only accept if parent is electron or Z0
            const PdgId pid = abs(pp->pdg_id());
            passed = (pid == PID::ELECTRON || abs(pp->pdg_id()) == PID::ZBOSON);
          }
        } else {
          passed = false;
        }
      }

      if (getLog().isActive(Log::TRACE)) {
        const int st = p->status();
        const double pT = p->momentum().perp();
        const double eta = p->momentum().eta();
        MSG_TRACE(std::boolalpha
                  << "ID = " << p->pdg_id() << ", status = " << st << ", pT = " << pT
                  << ", eta = " << eta << ": result = " << passed);
        if (pv) {
          foreach (const GenParticlePtr pp, particles_in(pv)) {
            MSG_TRACE(std::boolalpha << " parent ID = " << pp->pdg_id());
          }
        }
        if (dv) {
          foreach (const GenParticlePtr pp, particles_out(dv)) {
            MSG_TRACE(std::boolalpha << " child ID  = " << pp->pdg_id());
          }
        }
      }
      if (passed) _theParticles.push_back(Particle(*p));
    }
    MSG_DEBUG("Number of initial quarks = " << _theParticles.size());
    if (!_theParticles.empty()) {
      for (size_t i = 0; i < _theParticles.size(); i++) {
        MSG_DEBUG("Initial quark[" << i << "] = " << _theParticles[i].pdgId());
      }
    }
  }


}

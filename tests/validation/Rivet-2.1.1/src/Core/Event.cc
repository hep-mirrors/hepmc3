#include "Rivet/Event.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/BeamConstraint.hh"
#include "HepMC3/GenEvent.h"

namespace Rivet {


  void Event::_init(const GenEvent& ge) {
    // Use Rivet's preferred units if possible
    #ifdef HEPMC_HAS_UNITS
    _genEvent.use_units(HepMC3::Units::GEV, HepMC3::Units::MM);
    #endif

    // Use the conventional alignment
    _geNormAlignment();

    // @todo Filter the event to remove generator-specific particles: optional
    // behaviour? Maybe disableable in an inconvenient way, e.g. with an env
    // var, to communicate the appropriate distaste for this sort of truth
    // analysis ;-)

    // Debug printout to check that copying/mangling has worked
    /// @todo Enable this when HepMC3 has been fixed to allow printing to a stream like the Rivet logger.
    //_genEvent.print();
  }


  namespace { // unnamed namespace for hiding

    void _geRot180x(GenEvent& ge) {
      /// @todo Use nicer iterators over HepMC3 particles
      for (HepMC3::GenEvent::particle_iterator ip = ge.particles_begin(); ip != ge.particles_end(); ++ip) {
        const HepMC3::FourVector& mom = (*ip)->momentum();
        (*ip)->set_momentum(HepMC3::FourVector(mom.px(), -mom.py(), -mom.pz(), mom.e()));
      }
      /// @todo Use nicer iterators over HepMC3 vertices
      for (HepMC3::GenEvent::vertex_iterator iv = ge.vertices_begin(); iv != ge.vertices_end(); ++iv) {
        const HepMC3::FourVector& pos = (*iv)->position();
        (*iv)->set_position(HepMC3::FourVector(pos.x(), -pos.y(), -pos.z(), pos.t()));
      }
    }

  }


  void Event::_geNormAlignment() {
    if (!_genEvent.valid_beam_particles()) return;
    typedef pair<HepMC3::GenParticlePtr, HepMC3::GenParticlePtr> GPPair;
    GPPair bps = _genEvent.beam_particles();

    // Rotate e+- p and ppbar to put p along +z
    /// @todo Is there an e+ e- convention for longitudinal boosting, e.g. at B-factories? Different from LEP?
    // if (compatible(beamids, make_pdgid_pair(ELECTRON, PROTON)) ||
    //     compatible(beamids, make_pdgid_pair(POSITRON, PROTON)) ||
    //     compatible(beamids, make_pdgid_pair(ANTIPROTON, PROTON)) ) {
    //   Log::getLog("Rivet.Event") << Log::TRACE << "May need to rotate event..." << endl;
    bool rot = false;
    HepMC3::GenParticlePtr plusgp = 0;
    if (bps.first->pdg_id() != PID::PROTON || bps.second->pdg_id() != PID::PROTON) {
      if (bps.first->pdg_id() == PID::PROTON) {
        plusgp = bps.first;
      } else if (bps.second->pdg_id() == PID::PROTON) {
        plusgp = bps.second;
      }
      if (plusgp && plusgp->momentum().pz() < 0) {
        rot = true;
      }
    }

    // Do the rotation
    if (rot) {
      if (Log::getLog("Rivet.Event").isActive(Log::TRACE)) {
        Log::getLog("Rivet.Event") << Log::TRACE << "Rotating event\n"
                                   << "Before rotation: "
                                   << bps.first->pdg_id() << "@pz=" << bps.first->momentum().pz()/GeV << ", "
                                   << bps.second->pdg_id() << "@pz=" << bps.second->momentum().pz()/GeV << endl;
      }
      _geRot180x(_genEvent);
    }
  }


}

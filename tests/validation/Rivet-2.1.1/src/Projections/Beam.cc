// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/Beam.hh"

namespace Rivet {


  ParticlePair beams(const Event& e) {
    Beam beamproj;
    beamproj.project(e);
    return beamproj.beams();
  }

  PdgIdPair beamIds(const Event& e) {
    Beam beamproj;
    beamproj.project(e);
    return beamproj.beamIds();
  }

  PdgIdPair beamIds(const ParticlePair& beams) {
    return make_pair(beams.first.pdgId(), beams.second.pdgId());
  }

  double sqrtS(const Event& e) {
    Beam beamproj;
    beamproj.project(e);
    return beamproj.sqrtS();
  }

  double sqrtS(const ParticlePair& beams) {
    return sqrtS(beams.first.momentum(), beams.second.momentum());
  }

  double sqrtS(const FourMomentum& pa, const FourMomentum& pb) {
    const double mom1 = pa.pz();
    const double e1 = pa.E();
    const double mom2 = pb.pz();
    const double e2 = pb.E();
    double sqrts = sqrt( sqr(e1+e2) - sqr(mom1+mom2) );
    return sqrts;
  }



  /////////////////////////////////////////////



  void Beam::project(const Event& e) {
    assert(e.genEvent()->particles().size() >= 2);
    if (e.genEvent()->valid_beam_particles()) {
      pair<HepMC3::GenParticlePtr, HepMC3::GenParticlePtr> beams = e.genEvent()->beam_particles();
      assert(beams.first && beams.second);
      _theBeams.first  = Particle(beams.first);
      _theBeams.second = Particle(beams.second);
    } else if(e.genEvent()->barcode_to_particle(1) && e.genEvent()->barcode_to_particle(2)) {
      _theBeams.first  = e.genEvent()->barcode_to_particle(1);
      _theBeams.second = e.genEvent()->barcode_to_particle(2);
    }
    else {
      _theBeams.first = Particle(PID::ANY, FourMomentum());
      _theBeams.second = Particle(PID::ANY, FourMomentum());
    }
    //MSG_DEBUG("Beam particle IDs = " << beamIds());
  }


  double Beam::sqrtS() const {
    double sqrts = Rivet::sqrtS(beams());
    //MSG_DEBUG("sqrt(s) = " << sqrts/GeV << " GeV");
    return sqrts;
  }



}

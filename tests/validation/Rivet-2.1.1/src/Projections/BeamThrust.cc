// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Projections/BeamThrust.hh"
#include "Rivet/Tools/Logging.hh"


namespace Rivet {


  void BeamThrust::calc(const FinalState& fs) {
    calc(fs.particles());
  }

  void BeamThrust::calc(const vector<Particle>& fsparticles) {
    vector<FourMomentum> fsmomenta;
    fsmomenta.reserve(fsparticles.size());
    foreach (const Particle& p, fsparticles) {
      const FourMomentum p4 = p.momentum();
      fsmomenta.push_back(p4);
    }
    _calcBeamThrust(fsmomenta);
  }

  void BeamThrust::calc(const vector<FourMomentum>& fsmomenta) {
    _calcBeamThrust(fsmomenta);
  }

  /////////////////////////////////////////////////

  // Do the full calculation
  void BeamThrust::_calcBeamThrust(const vector<FourMomentum>& fsmomenta) {
    _beamthrust = 0.;
    foreach (const FourMomentum& p, fsmomenta) {
      _beamthrust += p.E() - fabs(p.z());
    }
  }

}

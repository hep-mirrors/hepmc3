// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerUA5.hh"

namespace Rivet {


  TriggerUA5::TriggerUA5() {
    setName("TriggerUA5");

    addProjection(Beam(), "Beam");
    addProjection(ChargedFinalState(-5.6, 5.6), "CFS");
  }


  void TriggerUA5::project(const Event& evt) {
    _n_plus = 0;
    _n_minus = 0;

    // Start with the assumption that the trigger fails
    _decision_sd = false;
    _decision_nsd_1 = false;
    _decision_nsd_2 = false;

    // Triggers can be different for pp and ppbar running
    const Beam& b = applyProjection<Beam>(evt, "Beam");
    _samebeams = (b.beams().first.pdgId() == b.beams().second.pdgId());

    // Count hodoscope hits
    const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(evt, "CFS");
    foreach (const Particle& p, cfs.particles()) {
      if (inRange(p.eta(), -5.6, -2.0)) _n_minus++;
      else if (inRange(p.eta(), 2.0, 5.6)) _n_plus++;
    }
    MSG_DEBUG("Trigger -: " << _n_minus << ", Trigger +: " << _n_plus);

    // Common SD/NSD trigger requirement: must activate at least one hodoscope
    if (_n_minus == 0 && _n_plus == 0) return;
    _decision_sd = true;

    // Extra NSD trigger requirements
    if (_n_minus == 0 || _n_plus == 0) return;
    _decision_nsd_1 = true;
    if (_n_minus < 2 || _n_plus < 2) return;
    _decision_nsd_2 = true;
  }


}

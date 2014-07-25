// -*- C++ -*-
#include "Rivet/Config/RivetCommon.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/TriggerCDFRun0Run1.hh"

namespace Rivet {


  void TriggerCDFRun0Run1::project(const Event& evt) {
    // Start with the assumption that the trigger fails
    _decision_mb = false;

    // Minimum Bias trigger requirements from the BBC counters
    int n_trig_1 = 0;
    int n_trig_2 = 0;
    // Event selection based on tracks in VTPC (time projection chambers)
    int n_backward = 0;
    int n_forward = 0;

    const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(evt, "CFS");
    foreach (const Particle& p, cfs.particles()) {
      if (inRange(p.eta(), -5.9, -3.2)) n_trig_1++;
      else if (inRange(p.eta(), 3.2, 5.9)) n_trig_2++;
      if (inRange(p.eta(), -3.0, 0.0)) n_backward++;
      else if (inRange(p.eta(), 0.0, 3.0)) n_forward++;
    }

    // Require at least 4 tracks with at least one in each of the forward
    // and backward hemispheres:
    if (n_trig_1 == 0 || n_trig_2 == 0) return;
    MSG_DEBUG("Trigger 1: " << n_trig_1 << " Trigger 2: " << n_trig_2);
    if (n_backward + n_forward < 4 || n_backward == 0 || n_forward == 0) return;
    MSG_DEBUG(" Num. forward: " << n_forward  << ", Num. backward: " << n_backward);

    // Trigger success:
    _decision_mb = true;
  }


}

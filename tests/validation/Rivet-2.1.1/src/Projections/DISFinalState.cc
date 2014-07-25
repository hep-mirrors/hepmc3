// -*- C++ -*-
#include "Rivet/Projections/DISFinalState.hh"

namespace Rivet {


  void DISFinalState::project(const Event& e) {
    const DISKinematics& diskin = applyProjection<DISKinematics>(e, "Kinematics");
    const LorentzTransform hcmboost = (_boosttype == HCM) ? diskin.boostHCM() : diskin.boostBreit();
    const DISLepton& dislep = diskin.applyProjection<DISLepton>(e, "Lepton");
    const FinalState& fs = dislep.applyProjection<FinalState>(e, "FS");

    // Fill the particle list with all particles _other_ than the DIS scattered
    // lepton, with momenta boosted into the appropriate frame.
    _theParticles.clear();
    _theParticles.reserve(fs.particles().size()-1);
    const GenParticlePtr dislepGP = dislep.out().genParticle();
    foreach (const Particle& p, fs.particles()) {
      if (p.genParticle() != dislepGP) { //< Ensure that we skip the DIS lepton
        Particle temp(p);
        temp.setMomentum(hcmboost.transform(temp.momentum()));
        _theParticles.push_back(temp);
      }
    }
  }


}

// -*- C++ -*-
#include "Rivet/Projections/DISKinematics.hh"
#include "Rivet/Math/Constants.hh"

namespace Rivet {


  void DISKinematics::project(const Event& e) {
    // Identify beam hadron
    const ParticlePair& inc = applyProjection<Beam>(e, "Beam").beams();
    bool firstIsHadron  = PID::isHadron(inc.first.pdgId());
    bool secondIsHadron = PID::isHadron(inc.second.pdgId());
    if (firstIsHadron && !secondIsHadron) {
      _inHadron = inc.first;
    } else if (!firstIsHadron && secondIsHadron) {
      _inHadron = inc.second;
    } else {
      //help!
      throw Error("DISKinematics projector could not find the correct beam hadron");
    }

    // Get the DIS lepton and store some of its properties
    const DISLepton& dislep = applyProjection<DISLepton>(e, "Lepton");
    const FourMomentum pLepIn = dislep.in().momentum();
    const FourMomentum pLepOut = dislep.out().momentum();
    const FourMomentum pHad = _inHadron.momentum();
    const FourMomentum pGamma = pLepIn - pLepOut;
    const FourMomentum tothad = pGamma + pHad;
    _theQ2 = -pGamma.mass2();
    _theW2 = tothad.mass2();
    _theX = Q2()/(2.0 * pGamma * pHad);
    _theY = (pGamma * pHad) / (pLepIn * pHad);
    _theS = invariant(pLepIn + pHad);

    // Calculate boost vector for boost into HCM-system
    LorentzTransform tmp;
    tmp.setBoost(-tothad.boostVector());

    // Rotate so the photon is in x-z plane in HCM rest frame
    FourMomentum pGammaHCM = tmp.transform(pGamma);
    tmp.preMult(Matrix3(Vector3::mkZ(), -pGammaHCM.azimuthalAngle()));
    pGammaHCM = tmp.transform(pGamma);
    assert(isZero(dot(pGammaHCM.vector3(), Vector3::mkY())));

    // Rotate so the photon is along the positive z-axis
    const double rot_angle = pGammaHCM.polarAngle() * (pGammaHCM.px() >= 0 ? -1 : 1);
    tmp.preMult(Matrix3(Vector3::mkY(), rot_angle));
    // Check that final HCM photon lies along +ve z as expected
    pGammaHCM = tmp.transform(pGamma);
    assert(isZero(dot(pGammaHCM.vector3(), Vector3::mkX())));
    assert(isZero(dot(pGammaHCM.vector3(), Vector3::mkY())));
    assert(isZero(angle(pGammaHCM.vector3(), Vector3::mkZ())));

    // Finally rotate so outgoing lepton at phi = 0
    FourMomentum pLepOutHCM = tmp.transform(pLepOut);
    tmp.preMult(Matrix3(Vector3::mkZ(), -pLepOutHCM.azimuthalAngle()));
    assert(isZero(tmp.transform(pLepOut).azimuthalAngle()));
    _hcm = tmp;

    // Boost to Breit frame (use opposite convention for photon --- along *minus* z)
    tmp.preMult(Matrix3(Vector3::mkX(), PI));
    const double bz = 1 - 2*x();
    _breit = LorentzTransform(Vector3::mkZ() * bz).combine(tmp);
    assert(isZero(angle(_breit.transform(pGamma).vector3(), -Vector3::mkZ())));
    assert(isZero(_breit.transform(pLepOut).azimuthalAngle()));
  }


  int DISKinematics::compare(const Projection & p) const {
    const DISKinematics& other = pcast<DISKinematics>(p);
    return mkNamedPCmp(other, "Lepton");
  }


}

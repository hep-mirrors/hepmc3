// -*- C++ -*-
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/InvMassFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @todo Not necessary in C++11, where constructors can be chained
  void ZFinder::_init(const FinalState& inputfs,
                      const vector<pair<double, double> >& etaRanges,
                      double pTmin,  PdgId pid,
                      double minmass, double maxmass,
                      double dRmax,
                      ClusterPhotons clusterPhotons,
                      PhotonTracking trackPhotons,
                      double masstarget)
  {
    setName("ZFinder");

    _minmass = minmass;
    _maxmass = maxmass;
    _masstarget = masstarget;
    _pid = pid;
    _trackPhotons = trackPhotons;

    IdentifiedFinalState bareleptons(inputfs);
    bareleptons.acceptIdPair(pid);
    const bool doClustering = (clusterPhotons != NOCLUSTER);
    const bool useDecayPhotons = (clusterPhotons == CLUSTERALL);
    DressedLeptons leptons(inputfs, bareleptons, dRmax, doClustering, etaRanges, pTmin, useDecayPhotons);
    addProjection(leptons, "DressedLeptons");

    VetoedFinalState remainingFS;
    remainingFS.addVetoOnThisFinalState(*this);
    addProjection(remainingFS, "RFS");
  }


  /////////////////////////////////////////////////////


  const FinalState& ZFinder::remainingFinalState() const {
    return getProjection<FinalState>("RFS");
  }


  int ZFinder::compare(const Projection& p) const {
    PCmp LCcmp = mkNamedPCmp(p, "DressedLeptons");
    if (LCcmp != EQUIVALENT) return LCcmp;

    const ZFinder& other = dynamic_cast<const ZFinder&>(p);
    return (cmp(_minmass, other._minmass) || cmp(_maxmass, other._maxmass) ||
            cmp(_pid, other._pid) || cmp(_trackPhotons, other._trackPhotons));
  }


  void ZFinder::project(const Event& e) {
    clear();

    const DressedLeptons& leptons = applyProjection<DressedLeptons>(e, "DressedLeptons");

    InvMassFinalState imfs(std::make_pair(_pid, -_pid), _minmass, _maxmass, _masstarget);
    Particles tmp;
    tmp.insert(tmp.end(), leptons.clusteredLeptons().begin(), leptons.clusteredLeptons().end());
    imfs.calc(tmp);

    if (imfs.particlePairs().size() < 1) return;
    ParticlePair Zconstituents(imfs.particlePairs()[0]);
    Particle l1(Zconstituents.first), l2(Zconstituents.second);
    if (PID::threeCharge(l1)>0.0) {
      _constituents += l1, l2;
    }
    else {
      _constituents += l2, l1;
    }
    FourMomentum pZ = l1.momentum() + l2.momentum();
    assert(PID::threeCharge(l1.pdgId()) + PID::threeCharge(l2.pdgId()) == 0);

    stringstream msg;
    msg << "Z reconstructed from: \n"
        << "   " << l1.momentum() << " " << l1.pdgId() << "\n"
        << " + " << l2.momentum() << " " << l2.pdgId();
    MSG_DEBUG(msg.str());
    _bosons.push_back(Particle(PID::ZBOSON, pZ));

    // Find the DressedLeptons which survived the IMFS cut such that we can
    // extract their original particles
    foreach (const Particle& p, _constituents) {
      foreach (const ClusteredLepton& l, leptons.clusteredLeptons()) {
        if (p.pdgId()==l.pdgId() && p.momentum()==l.momentum()) {
          _theParticles.push_back(l.constituentLepton());
          if (_trackPhotons) {
            _theParticles.insert(_theParticles.end(),
                                 l.constituentPhotons().begin(), l.constituentPhotons().end());
          }
        }
      }
    }
  }


}

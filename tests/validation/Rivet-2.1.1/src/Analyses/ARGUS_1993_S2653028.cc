// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/ParticleName.hh"

namespace Rivet {


  /// @brief BELLE pi+/-, K+/- and proton/antiproton spectrum at Upsilon(4S)
  /// @author Peter Richardson
  class ARGUS_1993_S2653028 : public Analysis {
  public:

    ARGUS_1993_S2653028()
      : Analysis("ARGUS_1993_S2653028"), _weightSum(0.)
    { }


    void analyze(const Event& e) {
      const double weight = e.weight();

      // Find the upsilons
      Particles upsilons;
      // First in unstable final state
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      foreach (const Particle& p, ufs.particles()) {
        if (p.pdgId() == 300553) upsilons.push_back(p);
      }
      // Then in whole event if that failed
      if (upsilons.empty()) {
        foreach (GenParticlePtr p, Rivet::particles(e.genEvent())) {
          if (p->pdg_id() != 300553) continue;
          const GenVertexPtr pv = p->production_vertex();
          bool passed = true;
          if (pv) {
            /// @todo Use better looping
            for (GenVertex::particles_in_const_iterator pp = pv->particles_in_const_begin(); pp != pv->particles_in_const_end() ; ++pp) {
              if ( p->pdg_id() == (*pp)->pdg_id() ) {
                passed = false;
                break;
              }
            }
          }
          if (passed) upsilons.push_back(Particle(*p));
        }
      }

      // Find an upsilon
      foreach (const Particle& p, upsilons) {
        _weightSum += weight;
        vector<GenParticlePtr> pionsA,pionsB,protonsA,protonsB,kaons;
        // Find the decay products we want
        findDecayProducts(p.genParticle(), pionsA, pionsB, protonsA, protonsB, kaons);
        LorentzTransform cms_boost;
        if (p.momentum().vector3().mod() > 1*MeV)
          cms_boost = LorentzTransform(-p.momentum().boostVector());
        for (size_t ix = 0; ix < pionsA.size(); ++ix) {
          FourMomentum ptemp(pionsA[ix]->momentum());
          FourMomentum p2 = cms_boost.transform(ptemp);
          double pcm = cms_boost.transform(ptemp).vector3().mod();
          _histPiA->fill(pcm,weight);
        }
        _multPiA->fill(10.58,double(pionsA.size())*weight);
        for (size_t ix = 0; ix < pionsB.size(); ++ix) {
          double pcm = cms_boost.transform(FourMomentum(pionsB[ix]->momentum())).vector3().mod();
          _histPiB->fill(pcm,weight);
        }
        _multPiB->fill(10.58,double(pionsB.size())*weight);
        for (size_t ix = 0; ix < protonsA.size(); ++ix) {
          double pcm = cms_boost.transform(FourMomentum(protonsA[ix]->momentum())).vector3().mod();
          _histpA->fill(pcm,weight);
        }
        _multpA->fill(10.58,double(protonsA.size())*weight);
        for (size_t ix = 0; ix < protonsB.size(); ++ix) {
          double pcm = cms_boost.transform(FourMomentum(protonsB[ix]->momentum())).vector3().mod();
          _histpB->fill(pcm,weight);
        }
        _multpB->fill(10.58,double(protonsB.size())*weight);
        for (size_t ix = 0 ;ix < kaons.size(); ++ix) {
          double pcm = cms_boost.transform(FourMomentum(kaons[ix]->momentum())).vector3().mod();
          _histKA->fill(pcm,weight);
          _histKB->fill(pcm,weight);
        }
        _multK->fill(10.58,double(kaons.size())*weight);
      }
    }


    void finalize() {
      if (_weightSum > 0.) {
        scale(_histPiA, 1./_weightSum);
        scale(_histPiB, 1./_weightSum);
        scale(_histKA , 1./_weightSum);
        scale(_histKB , 1./_weightSum);
        scale(_histpA , 1./_weightSum);
        scale(_histpB , 1./_weightSum);
        scale(_multPiA, 1./_weightSum);
        scale(_multPiB, 1./_weightSum);
        scale(_multK  , 1./_weightSum);
        scale(_multpA , 1./_weightSum);
        scale(_multpB , 1./_weightSum);
      }
    }


    void init() {
      addProjection(UnstableFinalState(), "UFS");

      // spectra
      _histPiA = bookHisto1D(1, 1, 1);
      _histPiB = bookHisto1D(2, 1, 1);
      _histKA  = bookHisto1D(3, 1, 1);
      _histKB  = bookHisto1D(6, 1, 1);
      _histpA  = bookHisto1D(4, 1, 1);
      _histpB  = bookHisto1D(5, 1, 1);
      // multiplicities
      _multPiA = bookHisto1D( 7, 1, 1);
      _multPiB = bookHisto1D( 8, 1, 1);
      _multK   = bookHisto1D( 9, 1, 1);
      _multpA  = bookHisto1D(10, 1, 1);
      _multpB  = bookHisto1D(11, 1, 1);
    } // init


  private:

    //@{
    /// Count of weights
    double _weightSum;
    /// Spectra
    Histo1DPtr _histPiA, _histPiB, _histKA, _histKB, _histpA, _histpB;
    /// Multiplicities
    Histo1DPtr _multPiA, _multPiB, _multK, _multpA, _multpB;
    //@}

    void findDecayProducts(const GenParticlePtr p,
                           vector<GenParticlePtr>& pionsA, vector<GenParticlePtr>& pionsB,
                           vector<GenParticlePtr>& protonsA, vector<GenParticlePtr>& protonsB,
                           vector<GenParticlePtr>& kaons)
    {
      int parentId = p->pdg_id();
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = abs((*pp)->pdg_id());
        if (id == PID::PIPLUS) {
          if (parentId != PID::LAMBDA && parentId != PID::K0S) {
            pionsA.push_back(*pp);
            pionsB.push_back(*pp);
          }
          else
            pionsB.push_back(*pp);
        }
        else if (id == PID::PROTON) {
          if (parentId != PID::LAMBDA && parentId != PID::K0S) {
            protonsA.push_back(*pp);
            protonsB.push_back(*pp);
          }
          else
            protonsB.push_back(*pp);
        }
        else if (id == PID::KPLUS) {
          kaons.push_back(*pp);
        }
        else if ((*pp)->end_vertex())
          findDecayProducts(*pp, pionsA, pionsB, protonsA, protonsB, kaons);
      }
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ARGUS_1993_S2653028);

}

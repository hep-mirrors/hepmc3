// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/ParticleName.hh"

namespace Rivet {


  /// @brief Production of the $\eta'(958)$ and $f_0(980)$ in $e^+e^-$ annihilation in the Upsilon region
  /// @author Peter Richardson
  class ARGUS_1993_S2669951 : public Analysis {
  public:

    ARGUS_1993_S2669951()
      : Analysis("ARGUS_1993_S2669951"),
        _count_etaPrime_highZ(2, 0.), _count_etaPrime_allZ(3, 0.), _count_f0(3, 0.),
        _weightSum_cont(0.), _weightSum_Ups1(0.), _weightSum_Ups2(0.)
    {   }


    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");

      _hist_cont_f0 = bookHisto1D(2, 1, 1);
      _hist_Ups1_f0 = bookHisto1D(3, 1, 1);
      _hist_Ups2_f0 = bookHisto1D(4, 1, 1);
    }


    void analyze(const Event& e) {
      const double weight = e.weight();

      const Beam beamproj = applyProjection<Beam>(e, "Beams");
      const double s = sqr(beamproj.sqrtS());
      const double roots = sqrt(s);
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      // Find the upsilons
      Particles upsilons;
      // First in unstable final state
      foreach (const Particle& p, ufs.particles())
        if (p.pdgId() == 553 || p.pdgId() == 100553 ) upsilons.push_back(p);
      // Then in whole event if fails
      if (upsilons.empty()) {
        foreach (GenParticlePtr p, Rivet::particles(e.genEvent())) {
          if ( p->pdg_id() != 553 && p->pdg_id() != 100553 ) continue;
          const GenVertexPtr pv = p->production_vertex();
          bool passed = true;
          if (pv) {
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
      // Continuum
      if (upsilons.empty()) {
        _weightSum_cont += weight;
        unsigned int nEtaA(0),nEtaB(0),nf0(0);
        foreach (const Particle& p, ufs.particles()) {
          int id = abs(p.pdgId());
          double xp = 2.*p.momentum().t()/roots;
          double beta = p.momentum().vector3().mod() / p.momentum().t();
          if (id == 9010221) {
            _hist_cont_f0->fill(xp, weight/beta);
            ++nf0;
          } else if (id == 331) {
            if (xp > 0.35) ++nEtaA;
            ++nEtaB;
          }
        }
        _count_f0[2]             += nf0*weight;
        _count_etaPrime_highZ[1] += nEtaA*weight;
        _count_etaPrime_allZ[2]  += nEtaB*weight;
      }
      else {
        // Find an Upsilon
        foreach (const Particle& ups, upsilons) {
          int parentId = ups.pdgId();
          ((parentId == 553) ? _weightSum_Ups1 : _weightSum_Ups2) += weight;
          Particles unstable;
          // Find the decay products we want
          findDecayProducts(ups.genParticle(), unstable);
          LorentzTransform cms_boost;
          if (ups.momentum().vector3().mod() > 1*MeV)
            cms_boost = LorentzTransform(-ups.momentum().boostVector());
          double mass = ups.momentum().mass();
          unsigned int nEtaA(0), nEtaB(0), nf0(0);
          foreach(const Particle& p , unstable) {
            const int id = abs(p.pdgId());
            FourMomentum p2 = cms_boost.transform(p.momentum());
            double xp = 2.*p2.t()/mass;
            double beta = p2.vector3().mod()/p2.t();
            if (id == 9010221) {
              ((parentId == 553) ? _hist_Ups1_f0 : _hist_Ups2_f0)->fill(xp, weight/beta);
              ++nf0;
            } else if (id == 331) {
              if (xp > 0.35) ++nEtaA;
              ++nEtaB;
            }
          }
          if (parentId == 553) {
            _count_f0[0]             +=   nf0*weight;
            _count_etaPrime_highZ[0] += nEtaA*weight;
            _count_etaPrime_allZ[0]  += nEtaB*weight;
          } else {
            _count_f0[1] += nf0*weight;
            _count_etaPrime_allZ[1]  += nEtaB*weight;
          }
        }
      }
    }


    void finalize() {

      /// @todo Better to group these by coherent 'if (weightSum_X)' statements?

      // High-Z eta' multiplicity
      Scatter2DPtr s111 = bookScatter2D(1, 1, 1, true);
      if (_weightSum_Ups1 > 0) // Point at 9.460
        s111->point(0).setY(_count_etaPrime_highZ[0] / _weightSum_Ups1, 0);
      if (_weightSum_cont > 0) // Point at 9.905
        s111->point(1).setY(_count_etaPrime_highZ[1] / _weightSum_cont, 0);


      // All-Z eta' multiplicity
      Scatter2DPtr s112 = bookScatter2D(1, 1, 2, true);
      if (_weightSum_Ups1 > 0) // Point at 9.460
        s112->point(0).setY(_count_etaPrime_allZ[0] / _weightSum_Ups1, 0);
      if (_weightSum_cont > 0) // Point at 9.905
        s112->point(1).setY(_count_etaPrime_allZ[2] / _weightSum_cont, 0);
      if (_weightSum_Ups2 > 0) // Point at 10.02
        s112->point(2).setY(_count_etaPrime_allZ[1] / _weightSum_Ups2, 0);

      // f0 multiplicity
      Scatter2DPtr s511 = bookScatter2D(5, 1, 1, true);
      if (_weightSum_Ups1 > 0) // Point at 9.46
        s112->point(0).setY(_count_f0[0] / _weightSum_Ups1, 0);
      if (_weightSum_Ups2 > 0) // Point at 10.02
        s112->point(1).setY(_count_f0[1] / _weightSum_Ups2, 0);
      if (_weightSum_cont > 0) // Point at 10.45
        s112->point(2).setY(_count_f0[2] / _weightSum_cont, 0);

      if (_weightSum_cont > 0.) scale(_hist_cont_f0, 1./_weightSum_cont);
      if (_weightSum_Ups1 > 0.) scale(_hist_Ups1_f0, 1./_weightSum_Ups1);
      if (_weightSum_Ups2 > 0.) scale(_hist_Ups2_f0, 1./_weightSum_Ups2);
    }


  private:

    /// @name Counters
    //@{
    vector<double> _count_etaPrime_highZ, _count_etaPrime_allZ, _count_f0;
    double _weightSum_cont,_weightSum_Ups1,_weightSum_Ups2;
    //@}


    /// Histos
    Histo1DPtr _hist_cont_f0, _hist_Ups1_f0, _hist_Ups2_f0;


    /// Recursively walk the HepMC tree to find decay products of @a p
    void findDecayProducts(const GenParticlePtr p, Particles& unstable) {
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        const int id = abs((*pp)->pdg_id());
        if (id == 331 || id == 9010221) unstable.push_back(Particle(*pp));
        else if ((*pp)->end_vertex()) findDecayProducts(*pp, unstable);
      }
    }


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ARGUS_1993_S2669951);

}

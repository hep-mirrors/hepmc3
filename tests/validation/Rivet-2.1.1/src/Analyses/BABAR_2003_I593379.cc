// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief Babar charmonium spectra
  /// @author Peter Richardson
  class BABAR_2003_I593379 : public Analysis {
  public:

    BABAR_2003_I593379()
      : Analysis("BABAR_2003_I593379"), _weightSum(0.)
    { }


    void analyze(const Event& e) {
      const double weight = e.weight();

      // Find the charmonia
      Particles upsilons;
      // First in unstable final state
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      foreach (const Particle& p, ufs.particles())
        if (p.pdgId()==300553) upsilons.push_back(p);
      // Then in whole event if fails
      if (upsilons.empty()) {
        foreach (GenParticlePtr p, Rivet::particles(e.genEvent())) {
          if (p->pdg_id()!=300553) continue;
          const GenVertexPtr pv = p->production_vertex();
          bool passed = true;
          if (pv) {
            for (GenVertex::particles_in_const_iterator pp = pv->particles_in_const_begin(); pp != pv->particles_in_const_end(); ++pp) {
              if ( p->pdg_id() == (*pp)->pdg_id() ) {
                passed = false;
                break;
              }
            }
          }
          if (passed) upsilons.push_back(Particle(p*));
        }
      }

      // find an upsilons
      foreach (const Particle& p, upsilons) {
        _weightSum += weight;
        // find the charmonium resonances
        vector<GenParticlePtr> allJpsi, primaryJpsi, Psiprime,
          all_chi_c1, all_chi_c2, primary_chi_c1, primary_chi_c2;
        findDecayProducts(p.genParticle(), allJpsi, primaryJpsi, Psiprime,
                          all_chi_c1, all_chi_c2, primary_chi_c1, primary_chi_c2);
        LorentzTransform cms_boost(-p.momentum().boostVector());
        for (size_t i = 0; i < allJpsi.size(); i++) {
          double pcm = cms_boost.transform(FourMomentum(allJpsi[i]->momentum())).vector3().mod();
          _hist_all_Jpsi->fill(pcm, weight);
        }
        _mult_JPsi->fill(10.58, weight*double(allJpsi.size()));
        for (size_t i = 0; i < primaryJpsi.size(); i++) {
          double pcm = cms_boost.transform(FourMomentum(primaryJpsi[i]->momentum())).vector3().mod();
          _hist_primary_Jpsi->fill(pcm, weight);
        }
        _mult_JPsi_direct->fill(10.58, weight*double(primaryJpsi.size()));
        for (size_t i=0; i<Psiprime.size(); i++) {
          double pcm = cms_boost.transform(FourMomentum(Psiprime[i]->momentum())).vector3().mod();
          _hist_Psi_prime->fill(pcm, weight);
        }
        _mult_Psi2S->fill(10.58, weight*double(Psiprime.size()));
        for (size_t i = 0; i < all_chi_c1.size(); i++) {
          double pcm = cms_boost.transform(FourMomentum(all_chi_c1[i]->momentum())).vector3().mod();
          _hist_chi_c1->fill(pcm, weight);
        }
        _mult_chi_c1->fill(10.58, weight*double(all_chi_c1.size()));
        _mult_chi_c1_direct->fill(10.58, weight*double(primary_chi_c1.size()));
        for (size_t i = 0; i < all_chi_c2.size(); i++) {
          double pcm = cms_boost.transform(FourMomentum(all_chi_c2[i]->momentum())).vector3().mod();
          _hist_chi_c2->fill(pcm, weight);
        }
        _mult_chi_c2->fill(10.58, weight*double(all_chi_c2.size()));
        _mult_chi_c2_direct->fill(10.58, weight*double(primary_chi_c2.size()));
      }
    } // analyze


    void finalize() {
      scale(_hist_all_Jpsi     , 0.5*0.1/_weightSum);
      scale(_hist_chi_c1       , 0.5*0.1/_weightSum);
      scale(_hist_chi_c2       , 0.5*0.1/_weightSum);
      scale(_hist_Psi_prime    , 0.5*0.1/_weightSum);
      scale(_hist_primary_Jpsi , 0.5*0.1/_weightSum);
      scale(_mult_JPsi         , 0.5*100./_weightSum);
      scale(_mult_JPsi_direct  , 0.5*100./_weightSum);
      scale(_mult_chi_c1       , 0.5*100./_weightSum);
      scale(_mult_chi_c1_direct, 0.5*100./_weightSum);
      scale(_mult_chi_c2       , 0.5*100./_weightSum);
      scale(_mult_chi_c2_direct, 0.5*100./_weightSum);
      scale(_mult_Psi2S        , 0.5*100./_weightSum);
    } // finalize


    void init() {
      addProjection(UnstableFinalState(), "UFS");

      _mult_JPsi          = bookHisto1D(1, 1, 1);
      _mult_JPsi_direct   = bookHisto1D(1, 1, 2);
      _mult_chi_c1        = bookHisto1D(1, 1, 3);
      _mult_chi_c1_direct = bookHisto1D(1, 1, 4);
      _mult_chi_c2        = bookHisto1D(1, 1, 5);
      _mult_chi_c2_direct = bookHisto1D(1, 1, 6);
      _mult_Psi2S         = bookHisto1D(1, 1, 7);
      _hist_all_Jpsi      = bookHisto1D(6, 1, 1);
      _hist_chi_c1        = bookHisto1D(7, 1, 1);
      _hist_chi_c2        = bookHisto1D(7, 1, 2);
      _hist_Psi_prime     = bookHisto1D(8, 1, 1);
      _hist_primary_Jpsi  = bookHisto1D(10, 1, 1);
    } // init

  private:

    //@{
    // count of weights
    double _weightSum;
    /// Histograms
    Histo1DPtr _hist_all_Jpsi;
    Histo1DPtr _hist_chi_c1;
    Histo1DPtr _hist_chi_c2;
    Histo1DPtr _hist_Psi_prime;
    Histo1DPtr _hist_primary_Jpsi;

    Histo1DPtr _mult_JPsi;
    Histo1DPtr _mult_JPsi_direct;
    Histo1DPtr _mult_chi_c1;
    Histo1DPtr _mult_chi_c1_direct;
    Histo1DPtr _mult_chi_c2;
    Histo1DPtr _mult_chi_c2_direct;
    Histo1DPtr _mult_Psi2S;
    //@}

    void findDecayProducts(const GenParticlePtr p,
                           vector<GenParticlePtr>& allJpsi,
                           vector<GenParticlePtr>& primaryJpsi,
                           vector<GenParticlePtr>& Psiprime,
                           vector<GenParticlePtr>& all_chi_c1, vector<GenParticlePtr>& all_chi_c2,
                           vector<GenParticlePtr>& primary_chi_c1, vector<GenParticlePtr>& primary_chi_c2) {
      const GenVertexPtr dv = p->end_vertex();
      bool isOnium = false;
      /// @todo Use better looping
      for (GenVertex::particles_in_const_iterator pp = dv->particles_in_const_begin() ; pp != dv->particles_in_const_end() ; ++pp) {
        int id = (*pp)->pdg_id();
        id = id%1000;
        id -= id%10;
        id /= 10;
        if (id==44) isOnium = true;
      }
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = (*pp)->pdg_id();
        if (id==100443) {
          Psiprime.push_back(*pp);
        }
        else if (id==20443) {
          all_chi_c1.push_back(*pp);
          if (!isOnium) primary_chi_c1.push_back(*pp);
        }
        else if (id==445) {
          all_chi_c2.push_back(*pp);
          if (!isOnium) primary_chi_c2.push_back(*pp);
        }
        else if (id==443) {
          allJpsi.push_back(*pp);
          if (!isOnium) primaryJpsi.push_back(*pp);
        }
        if ((*pp)->end_vertex()) {
          findDecayProducts(*pp, allJpsi, primaryJpsi, Psiprime, all_chi_c1, all_chi_c2, primary_chi_c1, primary_chi_c2);
        }
      }
    }

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BABAR_2003_I593379);

}

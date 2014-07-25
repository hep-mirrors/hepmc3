// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief BELLE pi0 spectrum at Upsilon(4S)
  /// @author Peter Richardson
  class BELLE_2001_S4598261 : public Analysis {
  public:

    BELLE_2001_S4598261()
      : Analysis("BELLE_2001_S4598261"), _weightSum(0.)
    { }


    void analyze(const Event& e) {
      const double weight = e.weight();


      // find the upsilons
      Particles upsilons;
      // first in unstable final state
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      foreach (const Particle& p, ufs.particles())
        if (p.pdgId()==300553) upsilons.push_back(p);
      // then in whole event if fails
      if (upsilons.empty()) {
        foreach (const GenParticlePtr p, Rivet::particles(e.genEvent())) {
          if (p->pdg_id() != 300553) continue;
          const GenVertexPtr pv = p->production_vertex();
          bool passed = true;
          if (pv) {
            /// @todo Use better looping
            for (GenVertex::particles_in_const_iterator pp = pv->particles_in_const_begin() ; pp != pv->particles_in_const_end() ; ++pp) {
              if ( p->pdg_id() == (*pp)->pdg_id() ) {
                passed = false;
                break;
              }
            }
          }
          if (passed) upsilons.push_back(Particle(p));
        }
      }

      // find an upsilons
      foreach (const Particle& p, upsilons) {
        _weightSum += weight;
        // find the neutral pions from the decay
        vector<GenParticlePtr> pions;
        findDecayProducts(p.genParticle(), pions);
        LorentzTransform cms_boost(-p.momentum().boostVector());
        for(unsigned int ix=0;ix<pions.size();++ix) {
          double pcm =
            cms_boost.transform(FourMomentum(pions[ix]->momentum())).vector3().mod();
          _histdSigDp->fill(pcm,weight);
        }
        _histMult->fill(0.,double(pions.size())*weight);
      }
    } // analyze

    void finalize() {

      scale(_histdSigDp, 1./_weightSum);
      scale(_histMult  , 1./_weightSum);
    } // finalize


    void init() {
      addProjection(UnstableFinalState(), "UFS");

      // spectrum
      _histdSigDp = bookHisto1D(1, 1, 1);
      // multiplicity
      _histMult   = bookHisto1D(2, 1, 1);
    } // init

  private:

    //@{
    // count of weights
    double _weightSum;
    /// Histograms
    Histo1DPtr _histdSigDp;
    Histo1DPtr _histMult;
    //@}

    void findDecayProducts(const GenParticlePtr p, vector<GenParticlePtr>& pions) {
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = (*pp)->pdg_id();
        if (id==111) {
          pions.push_back(*pp);
        } else if((*pp)->end_vertex())
          findDecayProducts(*pp, pions);
      }
    }


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BELLE_2001_S4598261);

}

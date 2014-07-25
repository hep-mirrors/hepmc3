// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief BABAR Xi_c baryons from fragmentation
  /// @author Peter Richardson
  class BABAR_2005_S6181155 : public Analysis {
  public:

    BABAR_2005_S6181155()
      : Analysis("BABAR_2005_S6181155")
    { }


    void analyze(const Event& e) {
      const double weight = e.weight();

      // Loop through unstable FS particles and look for charmed mesons/baryons
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      const Beam beamproj = applyProjection<Beam>(e, "Beams");
      const ParticlePair& beams = beamproj.beams();
      FourMomentum mom_tot = beams.first.momentum() + beams.second.momentum();
      LorentzTransform cms_boost(-mom_tot.boostVector());
      const double s = sqr(beamproj.sqrtS());

      const bool onresonance = fuzzyEquals(beamproj.sqrtS(), 10.58, 2E-3);

      foreach (const Particle& p, ufs.particles()) {
        // 3-momentum in CMS frame
        const double mom = cms_boost.transform(p.momentum()).vector3().mod();
        // only looking at Xi_c^0
        if(abs(p.pdgId()) != 4132 ) continue;
        if (onresonance) {
          _histOnResonanceA_norm->fill(mom,weight);
          _histOnResonanceB_norm->fill(mom,weight);
        }
        else {
          _histOffResonance_norm->fill(mom,s/sqr(10.58)*weight);
        }
        MSG_DEBUG("mom = " << mom);
        // off-resonance cross section
        if(checkDecay(p.genParticle())) {
          if (onresonance) {
            _histOnResonanceA->fill(mom,weight);
            _histOnResonanceB->fill(mom,weight);
          }
          else {
            _histOffResonance->fill(mom,s/sqr(10.58)*weight);
            _sigma->fill(10.6,weight);
          }
        }
      }
    } // analyze

    void finalize() {
      scale(_histOnResonanceA, crossSection()/femtobarn/sumOfWeights());
      scale(_histOnResonanceB, crossSection()/femtobarn/sumOfWeights());
      scale(_histOffResonance, crossSection()/femtobarn/sumOfWeights());
      scale(_sigma           , crossSection()/femtobarn/sumOfWeights());
      normalize(_histOnResonanceA_norm);
      normalize(_histOnResonanceB_norm);
      normalize(_histOffResonance_norm);
    } // finalize


    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");

      _histOnResonanceA = bookHisto1D(1,1,1);
      _histOnResonanceB = bookHisto1D(2,1,1);
      _histOffResonance = bookHisto1D(2,1,2);
      _sigma            = bookHisto1D(3,1,1);
      _histOnResonanceA_norm = bookHisto1D(4,1,1);
      _histOnResonanceB_norm = bookHisto1D(5,1,1);
      _histOffResonance_norm = bookHisto1D(5,1,2);

    } // init

  private:

    //@{
    /// Histograms
    Histo1DPtr _histOnResonanceA;
    Histo1DPtr _histOnResonanceB;
    Histo1DPtr _histOffResonance;
    Histo1DPtr _sigma           ;
    Histo1DPtr _histOnResonanceA_norm;
    Histo1DPtr _histOnResonanceB_norm;
    Histo1DPtr _histOffResonance_norm;
    //@}

    bool checkDecay(const GenParticlePtr p) {
      unsigned int nstable = 0, npip = 0, npim = 0;
      unsigned int nXim = 0, nXip = 0;
      findDecayProducts(p, nstable, npip, npim, nXip, nXim);
      int id = p->pdg_id();
      // Xi_c
      if (id==4132) {
        if (nstable==2 && nXim==1 && npip==1) return true;
      }
      else if (id==-4132) {
        if (nstable==2 && nXip==1 && npim==1) return true;
      }
      return false;
    }

    void findDecayProducts(const GenParticlePtr p,
                           unsigned int& nstable,
                           unsigned int& npip, unsigned int& npim,
                           unsigned int& nXip, unsigned int& nXim) {
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = (*pp)->pdg_id();
        if (id==3312) {
          ++nXim;
          ++nstable;
        } else if (id==-3312) {
          ++nXip;
          ++nstable;
        } else if(id==111||id==221) {
          ++nstable;
        } else if ((*pp)->end_vertex()) {
          findDecayProducts(*pp, nstable, npip, npim, nXip, nXim);
        } else {
          if     (id !=    22) ++nstable;
          if     (id ==   211) ++npip;
          else if(id ==  -211) ++npim;
        }
      }
    }

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BABAR_2005_S6181155);

}

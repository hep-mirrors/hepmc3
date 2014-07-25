// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief BELLE charmed mesons and baryons from fragmentation
  /// @author Eike von Seggern
  class BELLE_2006_S6265367 : public Analysis {
  public:

    BELLE_2006_S6265367()
      : Analysis("BELLE_2006_S6265367")
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
      const bool onresonance = fuzzyEquals(beamproj.sqrtS(), 10.58, 5E-3);

      // Particle masses from PDGlive (accessed online 16. Nov. 2009).
      foreach (const Particle& p, ufs.particles()) {

        double xp = 0.0;
        double mH2 = 0.0;
        // 3-momentum in CMS frame
        const double mom = cms_boost.transform(p.momentum()).vector3().mod();

        const int PdgId = abs(p.pdgId());
        MSG_DEBUG("pdgID = " << PdgId << "  mom = " << mom);
        switch (PdgId) {

          case 421:
            MSG_DEBUG("D0 found");
            mH2 = 3.47763; // 1.86484^2
            xp = mom/sqrt(s/4.0 - mH2);
            // off-resonance cross section
            if(!onresonance) _sigmaD0->fill(10.6,weight);
            if(checkDecay(p.genParticle())) {
              if (onresonance)
                _histXpD0_R->fill(xp, s*weight);
              else
                _histXpD0_C->fill(xp, s*weight);
            }
            if (onresonance)
              _histXpD0_R_N->fill(xp, weight);
            else
              _histXpD0_C_N->fill(xp, weight);
            break;

          case 411:
            MSG_DEBUG("D+ found");
            mH2 = 3.49547; // 1.86962^2
            xp = mom/sqrt(s/4.0 - mH2);
            if(!onresonance) _sigmaDPlus->fill(10.6,weight);
            if(checkDecay(p.genParticle())) {
              if (onresonance)
                _histXpDplus_R->fill(xp, s*weight);
              else
                _histXpDplus_C->fill(xp, s*weight);
            }
            if (onresonance)
              _histXpDplus_R_N->fill(xp, weight);
            else
              _histXpDplus_C_N->fill(xp, weight);
            break;
          case 431:
            MSG_DEBUG("D+_s found");
            mH2 = 3.87495; // 1.96849^2
            xp = mom/sqrt(s/4.0 - mH2);
            if(!onresonance) _sigmaDs->fill(10.6,weight);
            if(checkDecay(p.genParticle())) {
              if (onresonance)
                _histXpDplus_s_R->fill(xp, s*weight);
              else
                _histXpDplus_s_C->fill(xp, s*weight);
            }
            if (onresonance)
              _histXpDplus_s_R_N->fill(xp, weight);
            else
              _histXpDplus_s_C_N->fill(xp, weight);
            break;

          case 4122:
            MSG_DEBUG("Lambda_c found");
            mH2 = 5.22780; // 2.28646^2
            xp = mom/sqrt(s/4.0 - mH2);
            if(!onresonance) _sigmaLambdac   ->fill(10.6,weight);
            if(checkDecay(p.genParticle())) {
              if (onresonance)
                _histXpLambda_c_R->fill(xp, s*weight);
              else
                _histXpLambda_c_C->fill(xp, s*weight);
            }
            if (onresonance)
              _histXpLambda_c_R_N->fill(xp, weight);
            else
              _histXpLambda_c_C_N->fill(xp, weight);
            break;


          case 413: {
            MSG_DEBUG("D*+ found");
            mH2 = 4.04119; // 2.01027^2
            xp = mom/sqrt(s/4.0 - mH2);
            if(!onresonance) {
              _sigmaDStarPlusA->fill(10.6,weight);
              _sigmaDStarPlusB->fill(10.6,weight);
              _sigmaDStarPlusC->fill(10.6,weight);
            }
            GenParticlePtr Dmeson = p.genParticle();
            const GenVertexPtr dv = p.genParticle()->end_vertex();
            bool D0decay(false), Pi0decay(false), Piplusdecay(false), Dplusdecay(false);

            for (GenVertex::particles_out_const_iterator
                   pp = dv->particles_out_const_begin();
                 pp != dv->particles_out_const_end(); ++pp) {
              if (abs((*pp)->pdg_id()) == 421) {
                Dmeson = *pp;
                D0decay = true;
              } else if (abs((*pp)->pdg_id()) == 411) {
                Dmeson = *pp;
                Dplusdecay = true;
              } else if (abs((*pp)->pdg_id()) == 111) {
                Pi0decay = true;
              } else if (abs((*pp)->pdg_id()) == 211) {
                Piplusdecay = true;
              }
            }
            if (D0decay && Piplusdecay && checkDecay(Dmeson)) {
              if (onresonance)
                _histXpDstarplus2D0_R->fill(xp, s*weight);
              else
                _histXpDstarplus2D0_C->fill(xp, s*weight);
            }
            else if (Dplusdecay && Pi0decay && checkDecay(Dmeson)) {
              if (onresonance)
                _histXpDstarplus2Dplus_R->fill(xp, s*weight);
              else
                _histXpDstarplus2Dplus_C->fill(xp, s*weight);
            }
            else {
              MSG_WARNING("Unexpected D* decay!");
            }
            if (onresonance) {
              _histXpDstarplus2D0_R_N->fill(xp, weight);
              _histXpDstarplus2Dplus_R_N->fill(xp, weight);
            }
            else {
              _histXpDstarplus2D0_C_N->fill(xp, weight);
              _histXpDstarplus2Dplus_C_N->fill(xp, weight);
            }
            break;
            }

          case 423:
            MSG_DEBUG("D*0 found");
            mH2 = 4.02793; // 2.00697**2
            xp = mom/sqrt(s/4.0 - mH2);
            if(!onresonance) _sigmaDStar0    ->fill(10.6,weight);
            MSG_DEBUG("xp = " << xp);
            GenParticlePtr Dmeson = p.genParticle();
            const GenVertexPtr dv = p.genParticle()->end_vertex();
            bool D0decay(false), Pi0decay(false);
            /// @todo Use better looping
            for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
              if (abs((*pp)->pdg_id()) == 421) {
                Dmeson = *pp;
                D0decay = true;
              }
              else if (abs((*pp)->pdg_id()) == 111) {
                Pi0decay = true;
              }
            }
            if (D0decay && Pi0decay && checkDecay(Dmeson)) {
              if (onresonance)
                _histXpDstar0_R->fill(xp, s*weight);
              else {
                _histXpDstar0_C->fill(xp, s*weight);
              }
            }
            if (onresonance)
              _histXpDstar0_R_N->fill(xp, weight);
            else {
              _histXpDstar0_C_N->fill(xp, weight);
            }
            break;
        }
      }
    } // analyze


    void finalize() {
      scale(_histXpDstarplus2D0_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpD0_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDplus_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDplus_s_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpLambda_c_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDstarplus2Dplus_R, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDstar0_R, crossSection()/nanobarn/sumOfWeights());

      scale(_histXpDstarplus2D0_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpD0_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDplus_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDplus_s_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpLambda_c_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDstarplus2Dplus_C, crossSection()/nanobarn/sumOfWeights());
      scale(_histXpDstar0_C, crossSection()/nanobarn/sumOfWeights());

      normalize(_histXpDstarplus2D0_R_N);
      normalize(_histXpD0_R_N);
      normalize(_histXpDplus_R_N);
      normalize(_histXpDplus_s_R_N);
      normalize(_histXpLambda_c_R_N);
      normalize(_histXpDstarplus2Dplus_R_N);
      normalize(_histXpDstar0_R_N);

      normalize(_histXpDstarplus2D0_C_N);
      normalize(_histXpD0_C_N);
      normalize(_histXpDplus_C_N);
      normalize(_histXpDplus_s_C_N);
      normalize(_histXpLambda_c_C_N);
      normalize(_histXpDstarplus2Dplus_C_N);
      normalize(_histXpDstar0_C_N);

      scale(_sigmaD0, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDPlus, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDs, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaLambdac, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStar0, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStarPlusA, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStarPlusB, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStarPlusC, crossSection()/picobarn/sumOfWeights());
    } // finalize


    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");

      // continuum cross sections
      _sigmaD0         = bookHisto1D(1,1,1);
      _sigmaDPlus      = bookHisto1D(1,1,2);
      _sigmaDs         = bookHisto1D(1,1,3);
      _sigmaLambdac    = bookHisto1D(1,1,4);
      _sigmaDStar0     = bookHisto1D(1,1,5);
      _sigmaDStarPlusA = bookHisto1D(1,1,6);
      _sigmaDStarPlusB = bookHisto1D(1,1,7);
      _sigmaDStarPlusC = bookHisto1D(1,1,8);

      // histograms for continuum data (sqrt(s) = 10.52 GeV)
      _histXpDstarplus2D0_C    = bookHisto1D(2, 1, 1);
      _histXpD0_C              = bookHisto1D(3, 1, 1);
      _histXpDplus_C           = bookHisto1D(4, 1, 1);
      _histXpDplus_s_C         = bookHisto1D(5, 1, 1);
      _histXpLambda_c_C        = bookHisto1D(6, 1, 1);
      _histXpDstarplus2Dplus_C = bookHisto1D(7, 1, 1);
      _histXpDstar0_C          = bookHisto1D(8, 1, 1);

      // histograms for on-resonance data (sqrt(s) = 10.58 GeV)
      _histXpDstarplus2D0_R    = bookHisto1D(9, 1, 1);
      _histXpD0_R              = bookHisto1D(10, 1, 1);
      _histXpDplus_R           = bookHisto1D(11, 1, 1);
      _histXpDplus_s_R         = bookHisto1D(12, 1, 1);
      _histXpLambda_c_R        = bookHisto1D(13, 1, 1);
      _histXpDstarplus2Dplus_R = bookHisto1D(14, 1, 1);
      _histXpDstar0_R          = bookHisto1D(15, 1, 1);

      // histograms for continuum data (sqrt(s) = 10.52 GeV)
      _histXpDstarplus2D0_C_N    = bookHisto1D(2, 1, 2);
      _histXpD0_C_N              = bookHisto1D(3, 1, 2);
      _histXpDplus_C_N           = bookHisto1D(4, 1, 2);
      _histXpDplus_s_C_N         = bookHisto1D(5, 1, 2);
      _histXpLambda_c_C_N        = bookHisto1D(6, 1, 2);
      _histXpDstarplus2Dplus_C_N = bookHisto1D(7, 1, 2);
      _histXpDstar0_C_N          = bookHisto1D(8, 1, 2);

      // histograms for on-resonance data (sqrt(s) = 10.58 GeV)
      _histXpDstarplus2D0_R_N    = bookHisto1D(9, 1, 2);
      _histXpD0_R_N              = bookHisto1D(10, 1, 2);
      _histXpDplus_R_N           = bookHisto1D(11, 1, 2);
      _histXpDplus_s_R_N         = bookHisto1D(12, 1, 2);
      _histXpLambda_c_R_N        = bookHisto1D(13, 1, 2);
      _histXpDstarplus2Dplus_R_N = bookHisto1D(14, 1, 2);
      _histXpDstar0_R_N          = bookHisto1D(15, 1, 2);

    } // init

  private:

    //@{
    /// Histograms

    // Histograms for the continuum cross sections
    Histo1DPtr _sigmaD0;
    Histo1DPtr _sigmaDPlus;
    Histo1DPtr _sigmaDs;
    Histo1DPtr _sigmaLambdac;
    Histo1DPtr _sigmaDStar0;
    Histo1DPtr _sigmaDStarPlusA;
    Histo1DPtr _sigmaDStarPlusB;
    Histo1DPtr _sigmaDStarPlusC;

    // histograms for continuum data (sqrt(s) = 10.52 GeV)
    Histo1DPtr _histXpDstarplus2D0_C;
    Histo1DPtr _histXpD0_C;
    Histo1DPtr _histXpDplus_C;
    Histo1DPtr _histXpDplus_s_C;
    Histo1DPtr _histXpLambda_c_C;
    Histo1DPtr _histXpDstarplus2Dplus_C;
    Histo1DPtr _histXpDstar0_C;
    Histo1DPtr _histXpDstarplus2D0_C_N;
    Histo1DPtr _histXpD0_C_N;
    Histo1DPtr _histXpDplus_C_N;
    Histo1DPtr _histXpDplus_s_C_N;
    Histo1DPtr _histXpLambda_c_C_N;
    Histo1DPtr _histXpDstarplus2Dplus_C_N;
    Histo1DPtr _histXpDstar0_C_N;

    // histograms for on-resonance data (sqrt(s) = 10.58 GeV)
    Histo1DPtr _histXpDstarplus2D0_R;
    Histo1DPtr _histXpD0_R;
    Histo1DPtr _histXpDplus_R;
    Histo1DPtr _histXpDplus_s_R;
    Histo1DPtr _histXpLambda_c_R;
    Histo1DPtr _histXpDstarplus2Dplus_R;
    Histo1DPtr _histXpDstar0_R;
    Histo1DPtr _histXpDstarplus2D0_R_N;
    Histo1DPtr _histXpD0_R_N;
    Histo1DPtr _histXpDplus_R_N;
    Histo1DPtr _histXpDplus_s_R_N;
    Histo1DPtr _histXpLambda_c_R_N;
    Histo1DPtr _histXpDstarplus2Dplus_R_N;
    Histo1DPtr _histXpDstar0_R_N;
    //@}

    bool checkDecay(const GenParticlePtr p) {
      unsigned int nstable=0,npip=0,npim=0;
      unsigned int np=0,nap=0,nKp=0,nKm=0,nPhi=0;
      findDecayProducts(p, nstable, npip, npim, np, nap, nKp, nKm, nPhi);
      int id = p->pdg_id();
      //D0
      if(id==421) {
        if(nstable==2&&nKm==1&&npip==1) return true;
      }
      //Dbar0
      else if(id==-421) {
        if(nstable==2&&nKp==1&&npim==1) return true;
      }
      // D+
      else if(id==411) {
        if(nstable==3&&nKm==1&&npip==2) return true;
      }
      // D-
      else if(id==-411) {
        if(nstable==3&&nKp==1&&npim==2) return true;
      }
      // D_s+
      else if(id==431) {
        if(nstable==1&&nPhi==1&&npip==1) return true;
      }
      // D_s-
      else if(id==-431) {
        if(nstable==1&&nPhi==1&&npim==1) return true;
      }
      // Lambda_c
      else if(id==4122) {
        if(nstable==3&&np==1&&npip==1&&nKm==1) return true;
      }
      // Lambda_c bar
      else if(id==-4122) {
        if(nstable==3&&nap==1&&npim==1&&nKp==1) return true;
      }
      return false;
    }

    void findDecayProducts(const GenParticlePtr p,
                           unsigned int & nstable, unsigned int & npip,
                           unsigned int & npim   , unsigned int & np,
                           unsigned int & nap    , unsigned int & nKp,
                           unsigned int & nKm    , unsigned int & nPhi) {
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = (*pp)->pdg_id();
        if(id==333)
          ++nPhi;
        else if(id==111||id==221)
          ++nstable;
        else if((*pp)->end_vertex())
          findDecayProducts(*pp, nstable, npip, npim, np, nap, nKp, nKm, nPhi);
        else {
          if     (id !=    22) ++nstable;
          if     (id ==   211) ++npip;
          else if(id ==  -211) ++npim;
          else if(id ==  2212) ++np;
          else if(id == -2212) ++nap;
          else if(id ==   321) ++nKp;
          else if(id ==  -321) ++nKm;
        }
      }
    }

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BELLE_2006_S6265367);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/InitialQuarks.hh"
#include "Rivet/Projections/Thrust.hh"

namespace Rivet {


  /// @brief SLD flavour-dependent fragmentation paper
  /// @author Peter Richardson
  class SLD_1999_S3743934 : public Analysis {
  public:

    /// Constructor
    SLD_1999_S3743934()
      : Analysis("SLD_1999_S3743934"),
        _SumOfudsWeights(0.), _SumOfcWeights(0.),
        _SumOfbWeights(0.),
        _multPiPlus(4,0.),_multKPlus(4,0.),_multK0(4,0.),
        _multKStar0(4,0.),_multPhi(4,0.),
        _multProton(4,0.),_multLambda(4,0.)
    {    }


    /// @name Analysis methods
    //@{

    void analyze(const Event& e) {
      // First, veto on leptonic events by requiring at least 4 charged FS particles
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      const size_t numParticles = fs.particles().size();

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      if (numParticles < 2) {
        MSG_DEBUG("Failed ncharged cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed ncharged cut");
      // Get event weight for histo filling
      const double weight = e.weight();

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);
      int flavour = 0;
      const InitialQuarks& iqf = applyProjection<InitialQuarks>(e, "IQF");

      // If we only have two quarks (qqbar), just take the flavour.
      // If we have more than two quarks, look for the highest energetic q-qbar pair.
      /// @todo Can we make this based on hadron flavour instead?
      Particles quarks;
      if (iqf.particles().size() == 2) {
        flavour = abs( iqf.particles().front().pdgId() );
        quarks = iqf.particles();
      } else {
        map<int, Particle > quarkmap;
        foreach (const Particle& p, iqf.particles()) {
          if (quarkmap.find(p.pdgId()) == quarkmap.end()) quarkmap[p.pdgId()] = p;
          else if (quarkmap[p.pdgId()].momentum().E() < p.momentum().E()) quarkmap[p.pdgId()] = p;
        }
        double maxenergy = 0.;
        for (int i = 1; i <= 5; ++i) {
          double energy(0.);
          if (quarkmap.find( i) != quarkmap.end())
            energy += quarkmap[ i].momentum().E();
          if (quarkmap.find(-i) != quarkmap.end())
            energy += quarkmap[-i].momentum().E();
          if (energy > maxenergy)
            flavour = i;
        }
        if (quarkmap.find(flavour) != quarkmap.end())
          quarks.push_back(quarkmap[flavour]);
        if (quarkmap.find(-flavour) != quarkmap.end())
          quarks.push_back(quarkmap[-flavour]);
      }
      switch (flavour) {
      case PID::DQUARK:
      case PID::UQUARK:
      case PID::SQUARK:
        _SumOfudsWeights += weight;
        break;
      case PID::CQUARK:
        _SumOfcWeights += weight;
        break;
      case PID::BQUARK:
        _SumOfbWeights += weight;
        break;
      }
      // thrust axis for projections
      Vector3 axis = applyProjection<Thrust>(e, "Thrust").thrustAxis();
      double dot(0.);
      if (!quarks.empty()) {
        dot = quarks[0].momentum().vector3().dot(axis);
        if (quarks[0].pdgId() < 0) dot *= -1;
      }

      foreach (const Particle& p, fs.particles()) {
        const double xp = p.momentum().vector3().mod()/meanBeamMom;
        // if in quark or antiquark hemisphere
        bool quark = p.momentum().vector3().dot(axis)*dot > 0.;
        _h_XpChargedN->fill(xp, weight);
        _temp_XpChargedN1->fill(xp, weight);
        _temp_XpChargedN2->fill(xp, weight);
        _temp_XpChargedN3->fill(xp, weight);
        int id = abs(p.pdgId());
        // charged pions
        if (id == PID::PIPLUS) {
          _h_XpPiPlusN->fill(xp, weight);
          _multPiPlus[0] += weight;
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multPiPlus[1] += weight;
            _h_XpPiPlusLight->fill(xp, weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RPiPlus->fill(xp, weight);
            else
              _h_RPiMinus->fill(xp, weight);
            break;
          case PID::CQUARK:
            _multPiPlus[2] += weight;
            _h_XpPiPlusCharm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multPiPlus[3] += weight;
            _h_XpPiPlusBottom->fill(xp, weight);
            break;
          }
        }
        else if (id == PID::KPLUS) {
          _h_XpKPlusN->fill(xp, weight);
          _multKPlus[0] += weight;
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multKPlus[1] += weight;
            _temp_XpKPlusLight->fill(xp, weight);
            _h_XpKPlusLight->fill(xp, weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RKPlus->fill(xp, weight);
            else
              _h_RKMinus->fill(xp, weight);
            break;
         break;
          case PID::CQUARK:
            _multKPlus[2] += weight;
            _h_XpKPlusCharm->fill(xp, weight);
            _temp_XpKPlusCharm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multKPlus[3] += weight;
            _h_XpKPlusBottom->fill(xp, weight);
            break;
          }
        }
        else if (id == PID::PROTON) {
          _h_XpProtonN->fill(xp, weight);
          _multProton[0] += weight;
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multProton[1] += weight;
            _temp_XpProtonLight->fill(xp, weight);
            _h_XpProtonLight->fill(xp, weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RProton->fill(xp, weight);
            else
              _h_RPBar  ->fill(xp, weight);
            break;
         break;
          case PID::CQUARK:
            _multProton[2] += weight;
            _temp_XpProtonCharm->fill(xp, weight);
            _h_XpProtonCharm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multProton[3] += weight;
            _h_XpProtonBottom->fill(xp, weight);
            break;
          }
        }
      }

      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      foreach (const Particle& p, ufs.particles()) {
        const double xp = p.momentum().vector3().mod()/meanBeamMom;
        // if in quark or antiquark hemisphere
        bool quark = p.momentum().vector3().dot(axis)*dot>0.;
        int id = abs(p.pdgId());
        if (id == PID::LAMBDA) {
          _multLambda[0] += weight;
          _h_XpLambdaN->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multLambda[1] += weight;
            _h_XpLambdaLight->fill(xp, weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RLambda->fill(xp, weight);
            else
              _h_RLBar  ->fill(xp, weight);
            break;
          case PID::CQUARK:
            _multLambda[2] += weight;
            _h_XpLambdaCharm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multLambda[3] += weight;
            _h_XpLambdaBottom->fill(xp, weight);
            break;
          }
        }
        else if (id == 313) {
          _multKStar0[0] += weight;
          _h_XpKStar0N->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multKStar0[1] += weight;
            _temp_XpKStar0Light->fill(xp, weight);
            _h_XpKStar0Light->fill(xp, weight);
            if ( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RKS0   ->fill(xp, weight);
            else
              _h_RKSBar0->fill(xp, weight);
            break;
            break;
          case PID::CQUARK:
            _multKStar0[2] += weight;
            _temp_XpKStar0Charm->fill(xp, weight);
            _h_XpKStar0Charm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multKStar0[3] += weight;
            _h_XpKStar0Bottom->fill(xp, weight);
            break;
          }
        }
        else if (id == 333) {
          _multPhi[0] += weight;
          _h_XpPhiN->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multPhi[1] += weight;
            _h_XpPhiLight->fill(xp, weight);
            break;
          case PID::CQUARK:
            _multPhi[2] += weight;
            _h_XpPhiCharm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multPhi[3] += weight;
            _h_XpPhiBottom->fill(xp, weight);
            break;
          }
        }
        else if (id == PID::K0S || id == PID::K0L) {
          _multK0[0] += weight;
          _h_XpK0N->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _multK0[1] += weight;
            _h_XpK0Light->fill(xp, weight);
            break;
          case PID::CQUARK:
            _multK0[2] += weight;
            _h_XpK0Charm->fill(xp, weight);
            break;
          case PID::BQUARK:
            _multK0[3] += weight;
            _h_XpK0Bottom->fill(xp, weight);
            break;
          }
        }
      }
    }


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      addProjection(InitialQuarks(), "IQF");
      addProjection(Thrust(FinalState()), "Thrust");

      _temp_XpChargedN1 = bookHisto1D( 1, 1, 1);
      _temp_XpChargedN2 = bookHisto1D( 2, 1, 1);
      _temp_XpChargedN3 = bookHisto1D( 3, 1, 1);

      _h_XpPiPlusN      = bookHisto1D( 1, 1, 2);
      _h_XpKPlusN       = bookHisto1D( 2, 1, 2);
      _h_XpProtonN      = bookHisto1D( 3, 1, 2);
      _h_XpChargedN     = bookHisto1D( 4, 1, 1);
      _h_XpK0N          = bookHisto1D( 5, 1, 1);
      _h_XpLambdaN      = bookHisto1D( 7, 1, 1);
      _h_XpKStar0N      = bookHisto1D( 8, 1, 1);
      _h_XpPhiN         = bookHisto1D( 9, 1, 1);

      _h_XpPiPlusLight  = bookHisto1D(10, 1, 1);
      _h_XpPiPlusCharm  = bookHisto1D(10, 1, 2);
      _h_XpPiPlusBottom = bookHisto1D(10, 1, 3);
      _h_XpKPlusLight   = bookHisto1D(12, 1, 1);
      _h_XpKPlusCharm   = bookHisto1D(12, 1, 2);
      _h_XpKPlusBottom  = bookHisto1D(12, 1, 3);
      _h_XpKStar0Light  = bookHisto1D(14, 1, 1);
      _h_XpKStar0Charm  = bookHisto1D(14, 1, 2);
      _h_XpKStar0Bottom = bookHisto1D(14, 1, 3);
      _h_XpProtonLight  = bookHisto1D(16, 1, 1);
      _h_XpProtonCharm  = bookHisto1D(16, 1, 2);
      _h_XpProtonBottom = bookHisto1D(16, 1, 3);
      _h_XpLambdaLight  = bookHisto1D(18, 1, 1);
      _h_XpLambdaCharm  = bookHisto1D(18, 1, 2);
      _h_XpLambdaBottom = bookHisto1D(18, 1, 3);
      _h_XpK0Light      = bookHisto1D(20, 1, 1);
      _h_XpK0Charm      = bookHisto1D(20, 1, 2);
      _h_XpK0Bottom     = bookHisto1D(20, 1, 3);
      _h_XpPhiLight     = bookHisto1D(22, 1, 1);
      _h_XpPhiCharm     = bookHisto1D(22, 1, 2);
      _h_XpPhiBottom    = bookHisto1D(22, 1, 3);

      _temp_XpKPlusCharm   = bookHisto1D("TMP/XpKPlusCharm", refData(13, 1, 1));
      _temp_XpKPlusLight   = bookHisto1D("TMP/XpKPlusLight", refData(13, 1, 1));
      _temp_XpKStar0Charm  = bookHisto1D("TMP/XpKStar0Charm", refData(15, 1, 1));
      _temp_XpKStar0Light  = bookHisto1D("TMP/XpKStar0Light", refData(15, 1, 1));
      _temp_XpProtonCharm  = bookHisto1D("TMP/XpProtonCharm", refData(17, 1, 1));
      _temp_XpProtonLight  = bookHisto1D("TMP/XpProtonLight", refData(17, 1, 1));

      _h_RPiPlus  = bookHisto1D( 26, 1, 1);
      _h_RPiMinus = bookHisto1D( 26, 1, 2);
      _h_RKS0     = bookHisto1D( 28, 1, 1);
      _h_RKSBar0  = bookHisto1D( 28, 1, 2);
      _h_RKPlus   = bookHisto1D( 30, 1, 1);
      _h_RKMinus  = bookHisto1D( 30, 1, 2);
      _h_RProton  = bookHisto1D( 32, 1, 1);
      _h_RPBar    = bookHisto1D( 32, 1, 2);
      _h_RLambda  = bookHisto1D( 34, 1, 1);
      _h_RLBar    = bookHisto1D( 34, 1, 2);

      _s_Xp_PiPl_Ch        = bookScatter2D(1, 1, 1);
      _s_Xp_KPl_Ch         = bookScatter2D(2, 1, 1);
      _s_Xp_Pr_Ch          = bookScatter2D(3, 1, 1);
      _s_Xp_PiPlCh_PiPlLi  = bookScatter2D(11, 1, 1);
      _s_Xp_PiPlBo_PiPlLi  = bookScatter2D(11, 1, 2);
      _s_Xp_KPlCh_KPlLi    = bookScatter2D(13, 1, 1);
      _s_Xp_KPlBo_KPlLi    = bookScatter2D(13, 1, 2);
      _s_Xp_KS0Ch_KS0Li    = bookScatter2D(15, 1, 1);
      _s_Xp_KS0Bo_KS0Li    = bookScatter2D(15, 1, 2);
      _s_Xp_PrCh_PrLi      = bookScatter2D(17, 1, 1);
      _s_Xp_PrBo_PrLi      = bookScatter2D(17, 1, 2);
      _s_Xp_LaCh_LaLi      = bookScatter2D(19, 1, 1);
      _s_Xp_LaBo_LaLi      = bookScatter2D(19, 1, 2);
      _s_Xp_K0Ch_K0Li      = bookScatter2D(21, 1, 1);
      _s_Xp_K0Bo_K0Li      = bookScatter2D(21, 1, 2);
      _s_Xp_PhiCh_PhiLi    = bookScatter2D(23, 1, 1);
      _s_Xp_PhiBo_PhiLi    = bookScatter2D(23, 1, 2);

      _s_PiM_PiP    = bookScatter2D(27, 1, 1);
      _s_KSBar0_KS0 = bookScatter2D(29, 1, 1);
      _s_KM_KP      = bookScatter2D(31, 1, 1);
      _s_Pr_PBar    = bookScatter2D(33, 1, 1);
      _s_Lam_LBar   = bookScatter2D(35, 1, 1);
    }


    /// Finalize
    void finalize() {
      // Get the ratio plots sorted out first
      divide(_h_XpPiPlusN, _temp_XpChargedN1, _s_Xp_PiPl_Ch);
      divide(_h_XpKPlusN, _temp_XpChargedN2, _s_Xp_KPl_Ch);
      divide(_h_XpProtonN, _temp_XpChargedN3, _s_Xp_Pr_Ch);
      divide(_h_XpPiPlusCharm, _h_XpPiPlusLight, _s_Xp_PiPlCh_PiPlLi);
      divide(_h_XpPiPlusBottom, _h_XpPiPlusLight, _s_Xp_PiPlBo_PiPlLi);
      divide(_temp_XpKPlusCharm , _temp_XpKPlusLight, _s_Xp_KPlCh_KPlLi);
      divide(_h_XpKPlusBottom, _h_XpKPlusLight, _s_Xp_KPlBo_KPlLi);
      divide(_temp_XpKStar0Charm, _temp_XpKStar0Light, _s_Xp_KS0Ch_KS0Li);
      divide(_h_XpKStar0Bottom, _h_XpKStar0Light, _s_Xp_KS0Bo_KS0Li);
      divide(_temp_XpProtonCharm, _temp_XpProtonLight, _s_Xp_PrCh_PrLi);
      divide(_h_XpProtonBottom, _h_XpProtonLight, _s_Xp_PrBo_PrLi);
      divide(_h_XpLambdaCharm, _h_XpLambdaLight, _s_Xp_LaCh_LaLi);
      divide(_h_XpLambdaBottom, _h_XpLambdaLight, _s_Xp_LaBo_LaLi);
      divide(_h_XpK0Charm, _h_XpK0Light, _s_Xp_K0Ch_K0Li);
      divide(_h_XpK0Bottom, _h_XpK0Light, _s_Xp_K0Bo_K0Li);
      divide(_h_XpPhiCharm, _h_XpPhiLight, _s_Xp_PhiCh_PhiLi);
      divide(_h_XpPhiBottom, _h_XpPhiLight, _s_Xp_PhiBo_PhiLi);

      // Then the leading particles
      divide(*_h_RPiMinus - *_h_RPiPlus, *_h_RPiMinus + *_h_RPiPlus, _s_PiM_PiP);
      divide(*_h_RKSBar0 - *_h_RKS0, *_h_RKSBar0 + *_h_RKS0, _s_KSBar0_KS0);
      divide(*_h_RKMinus - *_h_RKPlus, *_h_RKMinus + *_h_RKPlus, _s_KM_KP);
      divide(*_h_RProton - *_h_RPBar, *_h_RProton + *_h_RPBar, _s_Pr_PBar);
      divide(*_h_RLambda - *_h_RLBar, *_h_RLambda + *_h_RLBar, _s_Lam_LBar);

      // Then the rest
      scale(_h_XpPiPlusN,      1/sumOfWeights());
      scale(_h_XpKPlusN,       1/sumOfWeights());
      scale(_h_XpProtonN,      1/sumOfWeights());
      scale(_h_XpChargedN,     1/sumOfWeights());
      scale(_h_XpK0N,          1/sumOfWeights());
      scale(_h_XpLambdaN,      1/sumOfWeights());
      scale(_h_XpKStar0N,      1/sumOfWeights());
      scale(_h_XpPhiN,         1/sumOfWeights());
      scale(_h_XpPiPlusLight,  1/_SumOfudsWeights);
      scale(_h_XpPiPlusCharm,  1/_SumOfcWeights);
      scale(_h_XpPiPlusBottom, 1/_SumOfbWeights);
      scale(_h_XpKPlusLight,   1/_SumOfudsWeights);
      scale(_h_XpKPlusCharm,   1/_SumOfcWeights);
      scale(_h_XpKPlusBottom,  1/_SumOfbWeights);
      scale(_h_XpKStar0Light,  1/_SumOfudsWeights);
      scale(_h_XpKStar0Charm,  1/_SumOfcWeights);
      scale(_h_XpKStar0Bottom, 1/_SumOfbWeights);
      scale(_h_XpProtonLight,  1/_SumOfudsWeights);
      scale(_h_XpProtonCharm,  1/_SumOfcWeights);
      scale(_h_XpProtonBottom, 1/_SumOfbWeights);
      scale(_h_XpLambdaLight,  1/_SumOfudsWeights);
      scale(_h_XpLambdaCharm,  1/_SumOfcWeights);
      scale(_h_XpLambdaBottom, 1/_SumOfbWeights);
      scale(_h_XpK0Light,      1/_SumOfudsWeights);
      scale(_h_XpK0Charm,      1/_SumOfcWeights);
      scale(_h_XpK0Bottom,     1/_SumOfbWeights);
      scale(_h_XpPhiLight,     1/_SumOfudsWeights);
      scale(_h_XpPhiCharm ,    1/_SumOfcWeights);
      scale(_h_XpPhiBottom,    1/_SumOfbWeights);
      scale(_h_RPiPlus,        1/_SumOfudsWeights);
      scale(_h_RPiMinus,       1/_SumOfudsWeights);
      scale(_h_RKS0,           1/_SumOfudsWeights);
      scale(_h_RKSBar0,        1/_SumOfudsWeights);
      scale(_h_RKPlus,         1/_SumOfudsWeights);
      scale(_h_RKMinus,        1/_SumOfudsWeights);
      scale(_h_RProton,        1/_SumOfudsWeights);
      scale(_h_RPBar,          1/_SumOfudsWeights);
      scale(_h_RLambda,        1/_SumOfudsWeights);
      scale(_h_RLBar,          1/_SumOfudsWeights);

      // Multiplicities
      double avgNumPartsAll, avgNumPartsLight,avgNumPartsCharm, avgNumPartsBottom;
      // pi+/-
      // all
      avgNumPartsAll = _multPiPlus[0]/sumOfWeights();
      bookScatter2D(24, 1, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multPiPlus[1]/_SumOfudsWeights;
      bookScatter2D(24, 1, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multPiPlus[2]/_SumOfcWeights;
      bookScatter2D(24, 1, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multPiPlus[3]/_SumOfbWeights;
      bookScatter2D(24, 1, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 1, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 1, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // K+/-
      // all
      avgNumPartsAll = _multKPlus[0]/sumOfWeights();
      bookScatter2D(24, 2, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multKPlus[1]/_SumOfudsWeights;
      bookScatter2D(24, 2, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multKPlus[2]/_SumOfcWeights;
      bookScatter2D(24, 2, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multKPlus[3]/_SumOfbWeights;
      bookScatter2D(24, 2, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 2, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 2, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // K0
      // all
      avgNumPartsAll = _multK0[0]/sumOfWeights();
      bookScatter2D(24, 3, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multK0[1]/_SumOfudsWeights;
      bookScatter2D(24, 3, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multK0[2]/_SumOfcWeights;
      bookScatter2D(24, 3, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multK0[3]/_SumOfbWeights;
      bookScatter2D(24, 3, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 3, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 3, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // K*0
      // all
      avgNumPartsAll = _multKStar0[0]/sumOfWeights();
      bookScatter2D(24, 4, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multKStar0[1]/_SumOfudsWeights;
      bookScatter2D(24, 4, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multKStar0[2]/_SumOfcWeights;
      bookScatter2D(24, 4, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multKStar0[3]/_SumOfbWeights;
      bookScatter2D(24, 4, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 4, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 4, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // phi
      // all
      avgNumPartsAll = _multPhi[0]/sumOfWeights();
      bookScatter2D(24, 5, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multPhi[1]/_SumOfudsWeights;
      bookScatter2D(24, 5, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multPhi[2]/_SumOfcWeights;
      bookScatter2D(24, 5, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multPhi[3]/_SumOfbWeights;
      bookScatter2D(24, 5, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 5, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 5, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // p
      // all
      avgNumPartsAll = _multProton[0]/sumOfWeights();
      bookScatter2D(24, 6, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multProton[1]/_SumOfudsWeights;
      bookScatter2D(24, 6, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multProton[2]/_SumOfcWeights;
      bookScatter2D(24, 6, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multProton[3]/_SumOfbWeights;
      bookScatter2D(24, 6, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 6, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 6, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
      // Lambda
      // all
      avgNumPartsAll = _multLambda[0]/sumOfWeights();
      bookScatter2D(24, 7, 1, true)->point(0).setY(avgNumPartsAll);
      // light
      avgNumPartsLight = _multLambda[1]/_SumOfudsWeights;
      bookScatter2D(24, 7, 2, true)->point(0).setY(avgNumPartsLight);
      // charm
      avgNumPartsCharm = _multLambda[2]/_SumOfcWeights;
      bookScatter2D(24, 7, 3, true)->point(0).setY(avgNumPartsCharm);
      // bottom
      avgNumPartsBottom = _multLambda[3]/_SumOfbWeights;
      bookScatter2D(24, 7, 4, true)->point(0).setY(avgNumPartsBottom);
      // charm-light
      bookScatter2D(25, 7, 1, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
      // bottom-light
      bookScatter2D(25, 7, 2, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);
    }

    //@}


  private:

    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles. Used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.
    double _SumOfudsWeights, _SumOfcWeights, _SumOfbWeights;
    vector<double> _multPiPlus, _multKPlus, _multK0,
      _multKStar0, _multPhi, _multProton, _multLambda;

    Histo1DPtr _h_XpPiPlusSig, _h_XpPiPlusN;
    Histo1DPtr _h_XpKPlusSig, _h_XpKPlusN;
    Histo1DPtr _h_XpProtonSig, _h_XpProtonN;
    Histo1DPtr _h_XpChargedN;
    Histo1DPtr _h_XpK0N, _h_XpLambdaN;
    Histo1DPtr _h_XpKStar0N, _h_XpPhiN;
    Histo1DPtr _h_XpPiPlusLight, _h_XpPiPlusCharm, _h_XpPiPlusBottom;
    Histo1DPtr _h_XpKPlusLight, _h_XpKPlusCharm, _h_XpKPlusBottom;
    Histo1DPtr _h_XpKStar0Light, _h_XpKStar0Charm, _h_XpKStar0Bottom;
    Histo1DPtr _h_XpProtonLight, _h_XpProtonCharm, _h_XpProtonBottom;
    Histo1DPtr _h_XpLambdaLight, _h_XpLambdaCharm, _h_XpLambdaBottom;
    Histo1DPtr _h_XpK0Light, _h_XpK0Charm, _h_XpK0Bottom;
    Histo1DPtr _h_XpPhiLight, _h_XpPhiCharm, _h_XpPhiBottom;

    Histo1DPtr _temp_XpChargedN1, _temp_XpChargedN2, _temp_XpChargedN3;
    Histo1DPtr _temp_XpKPlusCharm , _temp_XpKPlusLight;
    Histo1DPtr _temp_XpKStar0Charm, _temp_XpKStar0Light;
    Histo1DPtr _temp_XpProtonCharm, _temp_XpProtonLight;

    Histo1DPtr _h_RPiPlus, _h_RPiMinus;
    Histo1DPtr _h_RKS0, _h_RKSBar0;
    Histo1DPtr _h_RKPlus, _h_RKMinus;
    Histo1DPtr _h_RProton, _h_RPBar;
    Histo1DPtr _h_RLambda, _h_RLBar;

    Scatter2DPtr _s_Xp_PiPl_Ch, _s_Xp_KPl_Ch,  _s_Xp_Pr_Ch;
    Scatter2DPtr _s_Xp_PiPlCh_PiPlLi, _s_Xp_PiPlBo_PiPlLi;
    Scatter2DPtr _s_Xp_KPlCh_KPlLi, _s_Xp_KPlBo_KPlLi;
    Scatter2DPtr _s_Xp_KS0Ch_KS0Li, _s_Xp_KS0Bo_KS0Li;
    Scatter2DPtr _s_Xp_PrCh_PrLi, _s_Xp_PrBo_PrLi;
    Scatter2DPtr _s_Xp_LaCh_LaLi, _s_Xp_LaBo_LaLi;
    Scatter2DPtr _s_Xp_K0Ch_K0Li, _s_Xp_K0Bo_K0Li;
    Scatter2DPtr _s_Xp_PhiCh_PhiLi, _s_Xp_PhiBo_PhiLi;

    Scatter2DPtr _s_PiM_PiP, _s_KSBar0_KS0, _s_KM_KP, _s_Pr_PBar, _s_Lam_LBar;

    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(SLD_1999_S3743934);

}

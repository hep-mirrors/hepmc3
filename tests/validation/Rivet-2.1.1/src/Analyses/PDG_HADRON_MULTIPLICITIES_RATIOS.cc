// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief Implementation of PDG hadron multiplicities as ratios to \f$ \pi^\pm \f$ multiplicity
  /// @author Holger Schulz
  class PDG_HADRON_MULTIPLICITIES_RATIOS : public Analysis {
  public:

    /// Constructor
    PDG_HADRON_MULTIPLICITIES_RATIOS() : Analysis("PDG_HADRON_MULTIPLICITIES_RATIOS")
    {
      _weightedTotalNumPiPlus = 0;
    }


    /// @name Analysis methods
    //@{

    void analyze(const Event& e) {
      // First, veto on leptonic events by requiring at least 4 charged FS particles
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      const size_t numParticles = fs.particles().size();

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      if (numParticles < 2) {
        MSG_DEBUG("Failed leptonic event cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed leptonic event cut");

      // Get event weight for histo filling
      const double weight = e.weight();

      MSG_DEBUG("sqrt(S) = " << sqrtS()/GeV << " GeV");

      // Final state of unstable particles to get particle spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      if (sqrtS()/GeV >= 9.5 && sqrtS()/GeV <= 10.5) {
        foreach (const Particle& p, ufs.particles()) {
          const PdgId id = abs(p.pdgId());
          switch (id) {
             case 211:
                _weightedTotalNumPiPlus += weight;
                break;
             case 111:
                _histMeanMultiPi0->fill(_histMeanMultiPi0->bin(0).midpoint(), weight);
                break;
             case 321:
                _histMeanMultiKPlus->fill(_histMeanMultiKPlus->bin(0).midpoint(), weight);
                break;
             case 130:
             case 310:
                _histMeanMultiK0->fill(_histMeanMultiK0->bin(0).midpoint(), weight);
                break;
             case 221:
                _histMeanMultiEta->fill(_histMeanMultiEta->bin(0).midpoint(), weight);
                break;
             case 331:
                _histMeanMultiEtaPrime->fill(_histMeanMultiEtaPrime->bin(0).midpoint(), weight);
                break;
             case 411:
                _histMeanMultiDPlus->fill(_histMeanMultiDPlus->bin(0).midpoint(), weight);
                break;
             case 421:
                _histMeanMultiD0->fill(_histMeanMultiD0->bin(0).midpoint(), weight);
                break;
             case 431:
                _histMeanMultiDPlus_s->fill(_histMeanMultiDPlus_s->bin(0).midpoint(), weight);
                break;
             case 9010221:
                _histMeanMultiF0_980->fill(_histMeanMultiF0_980->bin(0).midpoint(), weight);
                break;
             case 113:
                _histMeanMultiRho770_0->fill(_histMeanMultiRho770_0->bin(0).midpoint(), weight);
                break;
             case 223:
                _histMeanMultiOmega782->fill(_histMeanMultiOmega782->bin(0).midpoint(), weight);
                break;
             case 323:
                _histMeanMultiKStar892Plus->fill(_histMeanMultiKStar892Plus->bin(0).midpoint(), weight);
                break;
             case 313:
                _histMeanMultiKStar892_0->fill(_histMeanMultiKStar892_0->bin(0).midpoint(), weight);
                break;
             case 333:
                _histMeanMultiPhi1020->fill(_histMeanMultiPhi1020->bin(0).midpoint(), weight);
                break;
             case 413:
                _histMeanMultiDStar2010Plus->fill(_histMeanMultiDStar2010Plus->bin(0).midpoint(), weight);
                break;
             case 423:
                _histMeanMultiDStar2007_0->fill(_histMeanMultiDStar2007_0->bin(0).midpoint(), weight);
                break;
             case 433:
                _histMeanMultiDStar_s2112Plus->fill(_histMeanMultiDStar_s2112Plus->bin(0).midpoint(), weight);
                break;
             case 443:
                _histMeanMultiJPsi1S->fill(_histMeanMultiJPsi1S->bin(0).midpoint(), weight);
                break;
             case 225:
                _histMeanMultiF2_1270->fill(_histMeanMultiF2_1270->bin(0).midpoint(), weight);
                break;
             case 2212:
                _histMeanMultiP->fill(_histMeanMultiP->bin(0).midpoint(), weight);
                break;
             case 3122:
                _histMeanMultiLambda->fill(_histMeanMultiLambda->bin(0).midpoint(), weight);
                break;
             case 3212:
                _histMeanMultiSigma0->fill(_histMeanMultiSigma0->bin(0).midpoint(), weight);
                break;
             case 3312:
                _histMeanMultiXiMinus->fill(_histMeanMultiXiMinus->bin(0).midpoint(), weight);
                break;
             case 2224:
                _histMeanMultiDelta1232PlusPlus->fill(_histMeanMultiDelta1232PlusPlus->bin(0).midpoint(), weight);
                break;
             case 3114:
                _histMeanMultiSigma1385Minus->fill(_histMeanMultiSigma1385Minus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3224:
                _histMeanMultiSigma1385Plus->fill(_histMeanMultiSigma1385Plus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3324:
                _histMeanMultiXi1530_0->fill(_histMeanMultiXi1530_0->bin(0).midpoint(), weight);
                break;
             case 3334:
                _histMeanMultiOmegaMinus->fill(_histMeanMultiOmegaMinus->bin(0).midpoint(), weight);
                break;
             case 4122:
                _histMeanMultiLambda_c_Plus->fill(_histMeanMultiLambda_c_Plus->bin(0).midpoint(), weight);
                break;
             case 4222:
             case 4112:
                _histMeanMultiSigma_c_PlusPlus_0->fill(_histMeanMultiSigma_c_PlusPlus_0->bin(0).midpoint(), weight);
                break;
             case 3124:
                _histMeanMultiLambda1520->fill(_histMeanMultiLambda1520->bin(0).midpoint(), weight);
                break;
          }
        }
      }

      if (sqrtS()/GeV >= 29 && sqrtS()/GeV <= 35) {
        foreach (const Particle& p, ufs.particles()) {
          const PdgId id = abs(p.pdgId());
          switch (id) {
             case 211:
                _weightedTotalNumPiPlus += weight;
                break;
             case 111:
                _histMeanMultiPi0->fill(_histMeanMultiPi0->bin(0).midpoint(), weight);
                break;
             case 321:
                _histMeanMultiKPlus->fill(_histMeanMultiKPlus->bin(0).midpoint(), weight);
                break;
             case 130:
             case 310:
                _histMeanMultiK0->fill(_histMeanMultiK0->bin(0).midpoint(), weight);
                break;
             case 221:
                _histMeanMultiEta->fill(_histMeanMultiEta->bin(0).midpoint(), weight);
                break;
             case 331:
                _histMeanMultiEtaPrime->fill(_histMeanMultiEtaPrime->bin(0).midpoint(), weight);
                break;
             case 411:
                _histMeanMultiDPlus->fill(_histMeanMultiDPlus->bin(0).midpoint(), weight);
                break;
             case 421:
                _histMeanMultiD0->fill(_histMeanMultiD0->bin(0).midpoint(), weight);
                break;
             case 431:
                _histMeanMultiDPlus_s->fill(_histMeanMultiDPlus_s->bin(0).midpoint(), weight);
                break;
             case 9010221:
                _histMeanMultiF0_980->fill(_histMeanMultiF0_980->bin(0).midpoint(), weight);
                break;
             case 113:
                _histMeanMultiRho770_0->fill(_histMeanMultiRho770_0->bin(0).midpoint(), weight);
                break;
             case 323:
                _histMeanMultiKStar892Plus->fill(_histMeanMultiKStar892Plus->bin(0).midpoint(), weight);
                break;
             case 313:
                _histMeanMultiKStar892_0->fill(_histMeanMultiKStar892_0->bin(0).midpoint(), weight);
                break;
             case 333:
                _histMeanMultiPhi1020->fill(_histMeanMultiPhi1020->bin(0).midpoint(), weight);
                break;
             case 413:
                _histMeanMultiDStar2010Plus->fill(_histMeanMultiDStar2010Plus->bin(0).midpoint(), weight);
                break;
             case 423:
                _histMeanMultiDStar2007_0->fill(_histMeanMultiDStar2007_0->bin(0).midpoint(), weight);
                break;
             case 225:
                _histMeanMultiF2_1270->fill(_histMeanMultiF2_1270->bin(0).midpoint(), weight);
                break;
             case 325:
                _histMeanMultiK2Star1430Plus->fill(_histMeanMultiK2Star1430Plus->bin(0).midpoint(), weight);
                break;
             case 315:
                _histMeanMultiK2Star1430_0->fill(_histMeanMultiK2Star1430_0->bin(0).midpoint(), weight);
                break;
             case 2212:
                _histMeanMultiP->fill(_histMeanMultiP->bin(0).midpoint(), weight);
                break;
             case 3122:
                _histMeanMultiLambda->fill(_histMeanMultiLambda->bin(0).midpoint(), weight);
                break;
             case 3312:
                _histMeanMultiXiMinus->fill(_histMeanMultiXiMinus->bin(0).midpoint(), weight);
                break;
             case 3114:
                _histMeanMultiSigma1385Minus->fill(_histMeanMultiSigma1385Minus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3224:
                _histMeanMultiSigma1385Plus->fill(_histMeanMultiSigma1385Plus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3334:
                _histMeanMultiOmegaMinus->fill(_histMeanMultiOmegaMinus->bin(0).midpoint(), weight);
                break;
             case 4122:
                _histMeanMultiLambda_c_Plus->fill(_histMeanMultiLambda_c_Plus->bin(0).midpoint(), weight);
                break;
          }
        }
      }

      if (sqrtS()/GeV >= 89.5 && sqrtS()/GeV <= 91.8) {
        foreach (const Particle& p, ufs.particles()) {
          const PdgId id = abs(p.pdgId());
          switch (id) {
             case 211:
                _weightedTotalNumPiPlus += weight;
                break;
             case 111:
                _histMeanMultiPi0->fill(_histMeanMultiPi0->bin(0).midpoint(), weight);
                break;
             case 321:
                _histMeanMultiKPlus->fill(_histMeanMultiKPlus->bin(0).midpoint(), weight);
                break;
             case 130:
             case 310:
                _histMeanMultiK0->fill(_histMeanMultiK0->bin(0).midpoint(), weight);
                break;
             case 221:
                _histMeanMultiEta->fill(_histMeanMultiEta->bin(0).midpoint(), weight);
                break;
             case 331:
                _histMeanMultiEtaPrime->fill(_histMeanMultiEtaPrime->bin(0).midpoint(), weight);
                break;
             case 411:
                _histMeanMultiDPlus->fill(_histMeanMultiDPlus->bin(0).midpoint(), weight);
                break;
             case 421:
                _histMeanMultiD0->fill(_histMeanMultiD0->bin(0).midpoint(), weight);
                break;
             case 431:
                _histMeanMultiDPlus_s->fill(_histMeanMultiDPlus_s->bin(0).midpoint(), weight);
                break;
             case 511:
                _histMeanMultiBPlus_B0_d->fill(_histMeanMultiBPlus_B0_d->bin(0).midpoint(), weight);
                break;
             case 521:
                _histMeanMultiBPlus_B0_d->fill(_histMeanMultiBPlus_B0_d->bin(0).midpoint(), weight);
                _histMeanMultiBPlus_u->fill(_histMeanMultiBPlus_u->bin(0).midpoint(), weight);
                break;
             case 531:
                _histMeanMultiB0_s->fill(_histMeanMultiB0_s->bin(0).midpoint(), weight);
                break;
             case 9010221:
                _histMeanMultiF0_980->fill(_histMeanMultiF0_980->bin(0).midpoint(), weight);
                break;
             case 9000211:
                _histMeanMultiA0_980Plus->fill(_histMeanMultiA0_980Plus->bin(0).midpoint(), weight);
                break;
             case 113:
                _histMeanMultiRho770_0->fill(_histMeanMultiRho770_0->bin(0).midpoint(), weight);
                break;
             case 213:
                _histMeanMultiRho770Plus->fill(_histMeanMultiRho770Plus->bin(0).midpoint(), weight);
                break;
             case 223:
                _histMeanMultiOmega782->fill(_histMeanMultiOmega782->bin(0).midpoint(), weight);
                break;
             case 323:
                _histMeanMultiKStar892Plus->fill(_histMeanMultiKStar892Plus->bin(0).midpoint(), weight);
                break;
             case 313:
                _histMeanMultiKStar892_0->fill(_histMeanMultiKStar892_0->bin(0).midpoint(), weight);
                break;
             case 333:
                _histMeanMultiPhi1020->fill(_histMeanMultiPhi1020->bin(0).midpoint(), weight);
                break;
             case 413:
                _histMeanMultiDStar2010Plus->fill(_histMeanMultiDStar2010Plus->bin(0).midpoint(), weight);
                break;
             case 433:
                _histMeanMultiDStar_s2112Plus->fill(_histMeanMultiDStar_s2112Plus->bin(0).midpoint(), weight);
                break;
             case 513:
             case 523:
             case 533:
                _histMeanMultiBStar->fill(_histMeanMultiBStar->bin(0).midpoint(), weight);
                break;
             case 443:
                _histMeanMultiJPsi1S->fill(_histMeanMultiJPsi1S->bin(0).midpoint(), weight);
                break;
             case 100443:
                _histMeanMultiPsi2S->fill(_histMeanMultiPsi2S->bin(0).midpoint(), weight);
                break;
             case 553:
                _histMeanMultiUpsilon1S->fill(_histMeanMultiUpsilon1S->bin(0).midpoint(), weight);
                break;
             case 20223:
                _histMeanMultiF1_1285->fill(_histMeanMultiF1_1285->bin(0).midpoint(), weight);
                break;
             case 20333:
                _histMeanMultiF1_1420->fill(_histMeanMultiF1_1420->bin(0).midpoint(), weight);
                break;
             case 445:
                _histMeanMultiChi_c1_3510->fill(_histMeanMultiChi_c1_3510->bin(0).midpoint(), weight);
                break;
             case 225:
                _histMeanMultiF2_1270->fill(_histMeanMultiF2_1270->bin(0).midpoint(), weight);
                break;
             case 335:
                _histMeanMultiF2Prime1525->fill(_histMeanMultiF2Prime1525->bin(0).midpoint(), weight);
                break;
             case 315:
                _histMeanMultiK2Star1430_0->fill(_histMeanMultiK2Star1430_0->bin(0).midpoint(), weight);
                break;
             case 515:
             case 525:
             case 535:
                _histMeanMultiBStarStar->fill(_histMeanMultiBStarStar->bin(0).midpoint(), weight);
                break;
             case 10433:
             case 20433:
                _histMeanMultiDs1Plus->fill(_histMeanMultiDs1Plus->bin(0).midpoint(), weight);
                break;
             case 435:
                _histMeanMultiDs2Plus->fill(_histMeanMultiDs2Plus->bin(0).midpoint(), weight);
                break;
             case 2212:
                _histMeanMultiP->fill(_histMeanMultiP->bin(0).midpoint(), weight);
                break;
             case 3122:
                _histMeanMultiLambda->fill(_histMeanMultiLambda->bin(0).midpoint(), weight);
                break;
             case 3212:
                _histMeanMultiSigma0->fill(_histMeanMultiSigma0->bin(0).midpoint(), weight);
                break;
             case 3112:
                _histMeanMultiSigmaMinus->fill(_histMeanMultiSigmaMinus->bin(0).midpoint(), weight);
                _histMeanMultiSigmaPlusMinus->fill(_histMeanMultiSigmaPlusMinus->bin(0).midpoint(), weight);
                break;
             case 3222:
                _histMeanMultiSigmaPlus->fill(_histMeanMultiSigmaPlus->bin(0).midpoint(), weight);
                _histMeanMultiSigmaPlusMinus->fill(_histMeanMultiSigmaPlusMinus->bin(0).midpoint(), weight);
                break;
             case 3312:
                _histMeanMultiXiMinus->fill(_histMeanMultiXiMinus->bin(0).midpoint(), weight);
                break;
             case 2224:
                _histMeanMultiDelta1232PlusPlus->fill(_histMeanMultiDelta1232PlusPlus->bin(0).midpoint(), weight);
                break;
             case 3114:
                _histMeanMultiSigma1385Minus->fill(_histMeanMultiSigma1385Minus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3224:
                _histMeanMultiSigma1385Plus->fill(_histMeanMultiSigma1385Plus->bin(0).midpoint(), weight);
                _histMeanMultiSigma1385PlusMinus->fill(_histMeanMultiSigma1385PlusMinus->bin(0).midpoint(), weight);
                break;
             case 3324:
                _histMeanMultiXi1530_0->fill(_histMeanMultiXi1530_0->bin(0).midpoint(), weight);
                break;
             case 3334:
                _histMeanMultiOmegaMinus->fill(_histMeanMultiOmegaMinus->bin(0).midpoint(), weight);
                break;
             case 4122:
                _histMeanMultiLambda_c_Plus->fill(_histMeanMultiLambda_c_Plus->bin(0).midpoint(), weight);
                break;
             case 5122:
                _histMeanMultiLambda_b_0->fill(_histMeanMultiLambda_b_0->bin(0).midpoint(), weight);
                break;
             case 3124:
                _histMeanMultiLambda1520->fill(_histMeanMultiLambda1520->bin(0).midpoint(), weight);
                break;
          }
        }
      }

      if (sqrtS()/GeV >= 130 && sqrtS()/GeV <= 200) {
        foreach (const Particle& p, ufs.particles()) {
          const PdgId id = abs(p.pdgId());
          switch (id) {
             case 211:
                _weightedTotalNumPiPlus += weight;
                break;
             case 321:
                _histMeanMultiKPlus->fill(_histMeanMultiKPlus->bin(0).midpoint(), weight);
                break;
             case 130:
             case 310:
                _histMeanMultiK0->fill(_histMeanMultiK0->bin(0).midpoint(), weight);
                break;
             case 2212:
                _histMeanMultiP->fill(_histMeanMultiP->bin(0).midpoint(), weight);
                break;
             case 3122:
                _histMeanMultiLambda->fill(_histMeanMultiLambda->bin(0).midpoint(), weight);
                break;
          }
        }
      }

    }



    void init() {
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");

      if (sqrtS()/GeV >= 9.5 && sqrtS()/GeV <= 10.5) {
        _histMeanMultiPi0                = bookHisto1D( 2, 1, 1);
        _histMeanMultiKPlus              = bookHisto1D( 3, 1, 1);
        _histMeanMultiK0                 = bookHisto1D( 4, 1, 1);
        _histMeanMultiEta                = bookHisto1D( 5, 1, 1);
        _histMeanMultiEtaPrime           = bookHisto1D( 6, 1, 1);
        _histMeanMultiDPlus              = bookHisto1D( 7, 1, 1);
        _histMeanMultiD0                 = bookHisto1D( 8, 1, 1);
        _histMeanMultiDPlus_s            = bookHisto1D( 9, 1, 1);
        _histMeanMultiF0_980             = bookHisto1D(13, 1, 1);
        _histMeanMultiRho770_0           = bookHisto1D(15, 1, 1);
        _histMeanMultiOmega782           = bookHisto1D(17, 1, 1);
        _histMeanMultiKStar892Plus       = bookHisto1D(18, 1, 1);
        _histMeanMultiKStar892_0         = bookHisto1D(19, 1, 1);
        _histMeanMultiPhi1020            = bookHisto1D(20, 1, 1);
        _histMeanMultiDStar2010Plus      = bookHisto1D(21, 1, 1);
        _histMeanMultiDStar2007_0        = bookHisto1D(22, 1, 1);
        _histMeanMultiDStar_s2112Plus    = bookHisto1D(23, 1, 1);
        _histMeanMultiJPsi1S             = bookHisto1D(25, 1, 1);
        _histMeanMultiF2_1270            = bookHisto1D(31, 1, 1);
        _histMeanMultiP                  = bookHisto1D(38, 1, 1);
        _histMeanMultiLambda             = bookHisto1D(39, 1, 1);
        _histMeanMultiSigma0             = bookHisto1D(40, 1, 1);
        _histMeanMultiXiMinus            = bookHisto1D(44, 1, 1);
        _histMeanMultiDelta1232PlusPlus  = bookHisto1D(45, 1, 1);
        _histMeanMultiSigma1385Minus     = bookHisto1D(46, 1, 1);
        _histMeanMultiSigma1385Plus      = bookHisto1D(47, 1, 1);
        _histMeanMultiSigma1385PlusMinus = bookHisto1D(48, 1, 1);
        _histMeanMultiXi1530_0           = bookHisto1D(49, 1, 1);
        _histMeanMultiOmegaMinus         = bookHisto1D(50, 1, 1);
        _histMeanMultiLambda_c_Plus      = bookHisto1D(51, 1, 1);
        _histMeanMultiSigma_c_PlusPlus_0 = bookHisto1D(53, 1, 1);
        _histMeanMultiLambda1520         = bookHisto1D(54, 1, 1);
      }

      if (sqrtS()/GeV >= 29 && sqrtS()/GeV <= 35) {
        _histMeanMultiPi0                = bookHisto1D( 2, 1, 2);
        _histMeanMultiKPlus              = bookHisto1D( 3, 1, 2);
        _histMeanMultiK0                 = bookHisto1D( 4, 1, 2);
        _histMeanMultiEta                = bookHisto1D( 5, 1, 2);
        _histMeanMultiEtaPrime           = bookHisto1D( 6, 1, 2);
        _histMeanMultiDPlus              = bookHisto1D( 7, 1, 2);
        _histMeanMultiD0                 = bookHisto1D( 8, 1, 2);
        _histMeanMultiDPlus_s            = bookHisto1D( 9, 1, 2);
        _histMeanMultiF0_980             = bookHisto1D(13, 1, 2);
        _histMeanMultiRho770_0           = bookHisto1D(15, 1, 2);
        _histMeanMultiKStar892Plus       = bookHisto1D(18, 1, 2);
        _histMeanMultiKStar892_0         = bookHisto1D(19, 1, 2);
        _histMeanMultiPhi1020            = bookHisto1D(20, 1, 2);
        _histMeanMultiDStar2010Plus      = bookHisto1D(21, 1, 2);
        _histMeanMultiDStar2007_0        = bookHisto1D(22, 1, 2);
        _histMeanMultiF2_1270            = bookHisto1D(31, 1, 2);
        _histMeanMultiK2Star1430Plus     = bookHisto1D(33, 1, 1);
        _histMeanMultiK2Star1430_0       = bookHisto1D(34, 1, 1);
        _histMeanMultiP                  = bookHisto1D(38, 1, 2);
        _histMeanMultiLambda             = bookHisto1D(39, 1, 2);
        _histMeanMultiXiMinus            = bookHisto1D(44, 1, 2);
        _histMeanMultiSigma1385Minus     = bookHisto1D(46, 1, 2);
        _histMeanMultiSigma1385Plus      = bookHisto1D(47, 1, 2);
        _histMeanMultiSigma1385PlusMinus = bookHisto1D(48, 1, 2);
        _histMeanMultiOmegaMinus         = bookHisto1D(50, 1, 2);
        _histMeanMultiLambda_c_Plus      = bookHisto1D(51, 1, 2);
      }

      if (sqrtS()/GeV >= 89.5 && sqrtS()/GeV <= 91.8) {
        _histMeanMultiPi0                = bookHisto1D( 2, 1, 3);
        _histMeanMultiKPlus              = bookHisto1D( 3, 1, 3);
        _histMeanMultiK0                 = bookHisto1D( 4, 1, 3);
        _histMeanMultiEta                = bookHisto1D( 5, 1, 3);
        _histMeanMultiEtaPrime           = bookHisto1D( 6, 1, 3);
        _histMeanMultiDPlus              = bookHisto1D( 7, 1, 3);
        _histMeanMultiD0                 = bookHisto1D( 8, 1, 3);
        _histMeanMultiDPlus_s            = bookHisto1D( 9, 1, 3);
        _histMeanMultiBPlus_B0_d         = bookHisto1D(10, 1, 1);
        _histMeanMultiBPlus_u            = bookHisto1D(11, 1, 1);
        _histMeanMultiB0_s               = bookHisto1D(12, 1, 1);
        _histMeanMultiF0_980             = bookHisto1D(13, 1, 3);
        _histMeanMultiA0_980Plus         = bookHisto1D(14, 1, 1);
        _histMeanMultiRho770_0           = bookHisto1D(15, 1, 3);
        _histMeanMultiRho770Plus         = bookHisto1D(16, 1, 1);
        _histMeanMultiOmega782           = bookHisto1D(17, 1, 2);
        _histMeanMultiKStar892Plus       = bookHisto1D(18, 1, 3);
        _histMeanMultiKStar892_0         = bookHisto1D(19, 1, 3);
        _histMeanMultiPhi1020            = bookHisto1D(20, 1, 3);
        _histMeanMultiDStar2010Plus      = bookHisto1D(21, 1, 3);
        _histMeanMultiDStar_s2112Plus    = bookHisto1D(23, 1, 2);
        _histMeanMultiBStar              = bookHisto1D(24, 1, 1);
        _histMeanMultiJPsi1S             = bookHisto1D(25, 1, 2);
        _histMeanMultiPsi2S              = bookHisto1D(26, 1, 1);
        _histMeanMultiUpsilon1S          = bookHisto1D(27, 1, 1);
        _histMeanMultiF1_1285            = bookHisto1D(28, 1, 1);
        _histMeanMultiF1_1420            = bookHisto1D(29, 1, 1);
        _histMeanMultiChi_c1_3510        = bookHisto1D(30, 1, 1);
        _histMeanMultiF2_1270            = bookHisto1D(31, 1, 3);
        _histMeanMultiF2Prime1525        = bookHisto1D(32, 1, 1);
        _histMeanMultiK2Star1430_0       = bookHisto1D(34, 1, 2);
        _histMeanMultiBStarStar          = bookHisto1D(35, 1, 1);
        _histMeanMultiDs1Plus            = bookHisto1D(36, 1, 1);
        _histMeanMultiDs2Plus            = bookHisto1D(37, 1, 1);
        _histMeanMultiP                  = bookHisto1D(38, 1, 3);
        _histMeanMultiLambda             = bookHisto1D(39, 1, 3);
        _histMeanMultiSigma0             = bookHisto1D(40, 1, 2);
        _histMeanMultiSigmaMinus         = bookHisto1D(41, 1, 1);
        _histMeanMultiSigmaPlus          = bookHisto1D(42, 1, 1);
        _histMeanMultiSigmaPlusMinus     = bookHisto1D(43, 1, 1);
        _histMeanMultiXiMinus            = bookHisto1D(44, 1, 3);
        _histMeanMultiDelta1232PlusPlus  = bookHisto1D(45, 1, 2);
        _histMeanMultiSigma1385Minus     = bookHisto1D(46, 1, 3);
        _histMeanMultiSigma1385Plus      = bookHisto1D(47, 1, 3);
        _histMeanMultiSigma1385PlusMinus = bookHisto1D(48, 1, 3);
        _histMeanMultiXi1530_0           = bookHisto1D(49, 1, 2);
        _histMeanMultiOmegaMinus         = bookHisto1D(50, 1, 3);
        _histMeanMultiLambda_c_Plus      = bookHisto1D(51, 1, 3);
        _histMeanMultiLambda_b_0         = bookHisto1D(52, 1, 1);
        _histMeanMultiLambda1520         = bookHisto1D(54, 1, 2);
      }

      if (sqrtS()/GeV >= 130 && sqrtS()/GeV <= 200) {
        _histMeanMultiKPlus             = bookHisto1D( 3, 1, 4);
        _histMeanMultiK0                = bookHisto1D( 4, 1, 4);
        _histMeanMultiP                 = bookHisto1D(38, 1, 4);
        _histMeanMultiLambda            = bookHisto1D(39, 1, 4);
      }
    }



    // Finalize
    void finalize() {
      if (sqrtS()/GeV >= 9.5 && sqrtS()/GeV <= 10.5) {
        scale(_histMeanMultiPi0               , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKPlus             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK0                , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEta               , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEtaPrime          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiD0                , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus_s           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF0_980            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiRho770_0          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiOmega782          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892Plus      , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892_0        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiPhi1020           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar2010Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar2007_0       , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar_s2112Plus   , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiJPsi1S            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF2_1270           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiP                 , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma0            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiXiMinus           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDelta1232PlusPlus , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Minus    , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385PlusMinus, 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiXi1530_0          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiOmegaMinus        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda_c_Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma_c_PlusPlus_0, 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda1520        , 1.0/_weightedTotalNumPiPlus);
      }

      if (sqrtS()/GeV >= 29 && sqrtS()/GeV <= 35) {
        scale(_histMeanMultiPi0               , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKPlus             , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK0                , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEta               , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEtaPrime          , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus             , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiD0                , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus_s           , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF0_980            , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiRho770_0          , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892Plus      , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892_0        , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiPhi1020           , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar2010Plus     , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar2007_0       , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF2_1270           , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK2Star1430Plus    , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK2Star1430_0      , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiP                 , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda            , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiXiMinus           , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Minus    , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Plus     , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385PlusMinus, 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiOmegaMinus        , 5.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda_c_Plus     , 5.0/_weightedTotalNumPiPlus);
      }

      if (sqrtS()/GeV >= 89.5 && sqrtS()/GeV <= 91.8) {
        scale(_histMeanMultiPi0               , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKPlus             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK0                , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEta               , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiEtaPrime          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiD0                , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDPlus_s           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiBPlus_B0_d        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiBPlus_u           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiB0_s              , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF0_980            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiA0_980Plus        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiRho770_0          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiRho770Plus        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiOmega782          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892Plus      , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiKStar892_0        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiPhi1020           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar2010Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDStar_s2112Plus   , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiBStar             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiJPsi1S            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiPsi2S             , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiUpsilon1S         , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF1_1285           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF1_1420           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiChi_c1_3510       , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF2_1270           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiF2Prime1525       , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK2Star1430_0      , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiBStarStar         , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDs1Plus           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDs2Plus           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiP                 , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma0            , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigmaMinus        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigmaPlus         , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigmaPlusMinus    , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiXiMinus           , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiDelta1232PlusPlus , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Minus    , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiSigma1385PlusMinus, 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiXi1530_0          , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiOmegaMinus        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda_c_Plus     , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda_b_0        , 1.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda1520        , 1.0/_weightedTotalNumPiPlus);
      }

      if (sqrtS()/GeV >= 130 && sqrtS()/GeV <= 200) {
        scale(_histMeanMultiKPlus            , 70.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiK0               , 70.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiP                , 70.0/_weightedTotalNumPiPlus);
        scale(_histMeanMultiLambda           , 70.0/_weightedTotalNumPiPlus);
      }
    }

    //@}


  private:

    double _weightedTotalNumPiPlus;

    Histo1DPtr _histMeanMultiPi0;
    Histo1DPtr _histMeanMultiKPlus;
    Histo1DPtr _histMeanMultiK0;
    Histo1DPtr _histMeanMultiEta;
    Histo1DPtr _histMeanMultiEtaPrime;
    Histo1DPtr _histMeanMultiDPlus;
    Histo1DPtr _histMeanMultiD0;
    Histo1DPtr _histMeanMultiDPlus_s;
    Histo1DPtr _histMeanMultiBPlus_B0_d;
    Histo1DPtr _histMeanMultiBPlus_u;
    Histo1DPtr _histMeanMultiB0_s;
    Histo1DPtr _histMeanMultiF0_980;
    Histo1DPtr _histMeanMultiA0_980Plus;
    Histo1DPtr _histMeanMultiRho770_0;
    Histo1DPtr _histMeanMultiRho770Plus;
    Histo1DPtr _histMeanMultiOmega782;
    Histo1DPtr _histMeanMultiKStar892Plus;
    Histo1DPtr _histMeanMultiKStar892_0;
    Histo1DPtr _histMeanMultiPhi1020;
    Histo1DPtr _histMeanMultiDStar2010Plus;
    Histo1DPtr _histMeanMultiDStar2007_0;
    Histo1DPtr _histMeanMultiDStar_s2112Plus;
    Histo1DPtr _histMeanMultiBStar;
    Histo1DPtr _histMeanMultiJPsi1S;
    Histo1DPtr _histMeanMultiPsi2S;
    Histo1DPtr _histMeanMultiUpsilon1S;
    Histo1DPtr _histMeanMultiF1_1285;
    Histo1DPtr _histMeanMultiF1_1420;
    Histo1DPtr _histMeanMultiChi_c1_3510;
    Histo1DPtr _histMeanMultiF2_1270;
    Histo1DPtr _histMeanMultiF2Prime1525;
    Histo1DPtr _histMeanMultiK2Star1430Plus;
    Histo1DPtr _histMeanMultiK2Star1430_0;
    Histo1DPtr _histMeanMultiBStarStar;
    Histo1DPtr _histMeanMultiDs1Plus;
    Histo1DPtr _histMeanMultiDs2Plus;
    Histo1DPtr _histMeanMultiP;
    Histo1DPtr _histMeanMultiLambda;
    Histo1DPtr _histMeanMultiSigma0;
    Histo1DPtr _histMeanMultiSigmaMinus;
    Histo1DPtr _histMeanMultiSigmaPlus;
    Histo1DPtr _histMeanMultiSigmaPlusMinus;
    Histo1DPtr _histMeanMultiXiMinus;
    Histo1DPtr _histMeanMultiDelta1232PlusPlus;
    Histo1DPtr _histMeanMultiSigma1385Minus;
    Histo1DPtr _histMeanMultiSigma1385Plus;
    Histo1DPtr _histMeanMultiSigma1385PlusMinus;
    Histo1DPtr _histMeanMultiXi1530_0;
    Histo1DPtr _histMeanMultiOmegaMinus;
    Histo1DPtr _histMeanMultiLambda_c_Plus;
    Histo1DPtr _histMeanMultiLambda_b_0;
    Histo1DPtr _histMeanMultiSigma_c_PlusPlus_0;
    Histo1DPtr _histMeanMultiLambda1520;

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(PDG_HADRON_MULTIPLICITIES_RATIOS);

}

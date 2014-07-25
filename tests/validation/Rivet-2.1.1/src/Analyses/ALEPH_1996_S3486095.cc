// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ParisiTensor.hh"
#include "Rivet/Projections/Hemispheres.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief ALEPH QCD study with event shapes and identified particles
  /// @author Holger Schulz
  class ALEPH_1996_S3486095 : public Analysis {

  public:

    /// Constructor
    ALEPH_1996_S3486095()
      : Analysis("ALEPH_1996_S3486095")
    {
      _numChParticles               = 0;
      _weightedTotalPartNum         = 0;
      _weightedTotalNumPiPlus       = 0;
      _weightedTotalNumKPlus        = 0;
      _weightedTotalNumP            = 0;
      _weightedTotalNumPhoton       = 0;
      _weightedTotalNumPi0          = 0;
      _weightedTotalNumEta          = 0;
      _weightedTotalNumEtaPrime     = 0;
      _weightedTotalNumK0           = 0;
      _weightedTotalNumLambda0      = 0;
      _weightedTotalNumXiMinus      = 0;
      _weightedTotalNumSigma1385Plus= 0;
      _weightedTotalNumXi1530_0     = 0;
      _weightedTotalNumRho          = 0;
      _weightedTotalNumOmega782     = 0;
      _weightedTotalNumKStar892_0   = 0;
      _weightedTotalNumPhi          = 0;
      _weightedTotalNumKStar892Plus = 0;
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      addProjection(Beam(), "Beams");
      const ChargedFinalState cfs;
      addProjection(cfs, "FS");
      addProjection(UnstableFinalState(), "UFS");
      addProjection(FastJets(cfs, FastJets::DURHAM, 0.7), "DurhamJets");
      addProjection(Sphericity(cfs), "Sphericity");
      addProjection(ParisiTensor(cfs), "Parisi");
      const Thrust thrust(cfs);
      addProjection(thrust, "Thrust");
      addProjection(Hemispheres(thrust), "Hemispheres");

      // Book histograms
      _histSphericity   = bookHisto1D(1, 1, 1);
      _histAplanarity   = bookHisto1D(2, 1, 1);

      _hist1MinusT      = bookHisto1D(3, 1, 1);
      _histTMinor       = bookHisto1D(4, 1, 1);

      _histY3           = bookHisto1D(5, 1, 1);
      _histHeavyJetMass = bookHisto1D(6, 1, 1);
      _histCParam       = bookHisto1D(7, 1, 1);
      _histOblateness   = bookHisto1D(8, 1, 1);

      _histScaledMom    = bookHisto1D(9, 1, 1);
      _histRapidityT    = bookHisto1D(10, 1, 1);

      _histPtSIn        = bookHisto1D(11, 1, 1);
      _histPtSOut       = bookHisto1D(12, 1, 1);

      _histLogScaledMom = bookHisto1D(17, 1, 1);

      _histChMult       = bookHisto1D(18, 1, 1);
      _histMeanChMult   = bookHisto1D(19, 1, 1);

      _histMeanChMultRapt05= bookHisto1D(20, 1, 1);
      _histMeanChMultRapt10= bookHisto1D(21, 1, 1);
      _histMeanChMultRapt15= bookHisto1D(22, 1, 1);
      _histMeanChMultRapt20= bookHisto1D(23, 1, 1);


      // Particle spectra
      _histMultiPiPlus        = bookHisto1D(25, 1, 1);
      _histMultiKPlus         = bookHisto1D(26, 1, 1);
      _histMultiP             = bookHisto1D(27, 1, 1);
      _histMultiPhoton        = bookHisto1D(28, 1, 1);
      _histMultiPi0           = bookHisto1D(29, 1, 1);
      _histMultiEta           = bookHisto1D(30, 1, 1);
      _histMultiEtaPrime      = bookHisto1D(31, 1, 1);
      _histMultiK0            = bookHisto1D(32, 1, 1);
      _histMultiLambda0       = bookHisto1D(33, 1, 1);
      _histMultiXiMinus       = bookHisto1D(34, 1, 1);
      _histMultiSigma1385Plus = bookHisto1D(35, 1, 1);
      _histMultiXi1530_0      = bookHisto1D(36, 1, 1);
      _histMultiRho           = bookHisto1D(37, 1, 1);
      _histMultiOmega782      = bookHisto1D(38, 1, 1);
      _histMultiKStar892_0    = bookHisto1D(39, 1, 1);
      _histMultiPhi           = bookHisto1D(40, 1, 1);

      _histMultiKStar892Plus  = bookHisto1D(43, 1, 1);

      // Mean multiplicities
      _histMeanMultiPi0           = bookHisto1D(44, 1,  2);
      _histMeanMultiEta           = bookHisto1D(44, 1,  3);
      _histMeanMultiEtaPrime      = bookHisto1D(44, 1,  4);
      _histMeanMultiK0            = bookHisto1D(44, 1,  5);
      _histMeanMultiRho           = bookHisto1D(44, 1,  6);
      _histMeanMultiOmega782      = bookHisto1D(44, 1,  7);
      _histMeanMultiPhi           = bookHisto1D(44, 1,  8);
      _histMeanMultiKStar892Plus  = bookHisto1D(44, 1,  9);
      _histMeanMultiKStar892_0    = bookHisto1D(44, 1, 10);
      _histMeanMultiLambda0       = bookHisto1D(44, 1, 11);
      _histMeanMultiSigma0        = bookHisto1D(44, 1, 12);
      _histMeanMultiXiMinus       = bookHisto1D(44, 1, 13);
      _histMeanMultiSigma1385Plus = bookHisto1D(44, 1, 14);
      _histMeanMultiXi1530_0      = bookHisto1D(44, 1, 15);
      _histMeanMultiOmegaOmegaBar = bookHisto1D(44, 1, 16);
    }


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
      _weightedTotalPartNum += numParticles * weight;

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);

      // Thrusts
      MSG_DEBUG("Calculating thrust");
      const Thrust& thrust = applyProjection<Thrust>(e, "Thrust");
      _hist1MinusT->fill(1 - thrust.thrust(), weight);
      _histTMinor->fill(thrust.thrustMinor(), weight);
      _histOblateness->fill(thrust.oblateness(), weight);

      // Jets
      MSG_DEBUG("Calculating differential jet rate plots:");
      const FastJets& durjet = applyProjection<FastJets>(e, "DurhamJets");
      if (durjet.clusterSeq()) {
        double y3 = durjet.clusterSeq()->exclusive_ymerge_max(2);
        if (y3>0.0) _histY3->fill(-1. * std::log(y3), weight);
      }

      // Sphericities
      MSG_DEBUG("Calculating sphericity");
      const Sphericity& sphericity = applyProjection<Sphericity>(e, "Sphericity");
      _histSphericity->fill(sphericity.sphericity(), weight);
      _histAplanarity->fill(sphericity.aplanarity(), weight);

      // C param
      MSG_DEBUG("Calculating Parisi params");
      const ParisiTensor& parisi = applyProjection<ParisiTensor>(e, "Parisi");
      _histCParam->fill(parisi.C(), weight);

      // Hemispheres
      MSG_DEBUG("Calculating hemisphere variables");
      const Hemispheres& hemi = applyProjection<Hemispheres>(e, "Hemispheres");
      _histHeavyJetMass->fill(hemi.scaledM2high(), weight);

      // Iterate over all the charged final state particles.
      double Evis = 0.0;
      double rapt05 = 0.;
      double rapt10 = 0.;
      double rapt15 = 0.;
      double rapt20 = 0.;
      //int numChParticles = 0;
      MSG_DEBUG("About to iterate over charged FS particles");
      foreach (const Particle& p, fs.particles()) {
        // Get momentum and energy of each particle.
        const Vector3 mom3 = p.momentum().vector3();
        const double energy = p.momentum().E();
        Evis += energy;
        _numChParticles += weight;

        // Scaled momenta.
        const double mom = mom3.mod();
        const double scaledMom = mom/meanBeamMom;
        const double logInvScaledMom = -std::log(scaledMom);
        _histLogScaledMom->fill(logInvScaledMom, weight);
        _histScaledMom->fill(scaledMom, weight);

        // Get momenta components w.r.t. thrust and sphericity.
        const double momT = dot(thrust.thrustAxis(), mom3);
        const double pTinS = dot(mom3, sphericity.sphericityMajorAxis());
        const double pToutS = dot(mom3, sphericity.sphericityMinorAxis());
        _histPtSIn->fill(fabs(pTinS/GeV), weight);
        _histPtSOut->fill(fabs(pToutS/GeV), weight);

        // Calculate rapidities w.r.t. thrust.
        const double rapidityT = 0.5 * std::log((energy + momT) / (energy - momT));
        _histRapidityT->fill(fabs(rapidityT), weight);
        if (std::fabs(rapidityT) <= 0.5)  {
            rapt05 += 1.0;
        }
        if (std::fabs(rapidityT) <= 1.0)  {
            rapt10 += 1.0;
        }
        if (std::fabs(rapidityT) <= 1.5) {
            rapt15 += 1.0;
        }
        if (std::fabs(rapidityT) <= 2.0)  {
            rapt20 += 1.0;
        }

      }

      _histChMult->fill(numParticles, weight);

      _histMeanChMultRapt05->fill(_histMeanChMultRapt05->bin(0).midpoint(), rapt05 * weight);
      _histMeanChMultRapt10->fill(_histMeanChMultRapt10->bin(0).midpoint(), rapt10 * weight);
      _histMeanChMultRapt15->fill(_histMeanChMultRapt15->bin(0).midpoint(), rapt15 * weight);
      _histMeanChMultRapt20->fill(_histMeanChMultRapt20->bin(0).midpoint(), rapt20 * weight);
      _histMeanChMult->fill(_histMeanChMult->bin(0).midpoint(), numParticles*weight);


      //// Final state of unstable particles to get particle spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      for (Particles::const_iterator p = ufs.particles().begin(); p != ufs.particles().end(); ++p) {
        const Vector3 mom3 = p->momentum().vector3();
        int id = abs(p->pdgId());
        const double mom = mom3.mod();
        const double energy = p->momentum().E();
        const double scaledMom = mom/meanBeamMom;
        const double scaledEnergy = energy/meanBeamMom;  // meanBeamMom is approximately beam energy
        switch (id) {
           case 22:
              _histMultiPhoton->fill(-1.*std::log(scaledMom), weight);
              _weightedTotalNumPhoton += weight;
              break;
           case -321:
           case 321:
              _weightedTotalNumKPlus += weight;
              _histMultiKPlus->fill(scaledMom, weight);
              break;
           case 211:
           case -211:
              _histMultiPiPlus->fill(scaledMom, weight);
              _weightedTotalNumPiPlus += weight;
              break;
           case 2212:
           case -2212:
              _histMultiP->fill(scaledMom, weight);
              _weightedTotalNumP += weight;
              break;
           case 111:
              _histMultiPi0->fill(scaledMom, weight);
              _histMeanMultiPi0->fill(_histMeanMultiPi0->bin(0).midpoint(), weight);
              _weightedTotalNumPi0 += weight;
              break;
           case 221:
              if (scaledMom >= 0.1) {
                _histMultiEta->fill(scaledEnergy, weight);
                _histMeanMultiEta->fill(_histMeanMultiEta->bin(0).midpoint(), weight);
                _weightedTotalNumEta += weight;
              }
              break;
           case 331:
              if (scaledMom >= 0.1) {
                _histMultiEtaPrime->fill(scaledEnergy, weight);
                _histMeanMultiEtaPrime->fill(_histMeanMultiEtaPrime->bin(0).midpoint(), weight);
                _weightedTotalNumEtaPrime += weight;
              }
              break;
           case 130: //klong
           case 310: //kshort
              _histMultiK0->fill(scaledMom, weight);
              _histMeanMultiK0->fill(_histMeanMultiK0->bin(0).midpoint(), weight);
              _weightedTotalNumK0 += weight;
              break;
           case 113:
              _histMultiRho->fill(scaledMom, weight);
              _histMeanMultiRho->fill(_histMeanMultiRho->bin(0).midpoint(), weight);
              _weightedTotalNumRho += weight;
              break;
           case 223:
              _histMultiOmega782->fill(scaledMom, weight);
              _histMeanMultiOmega782->fill(_histMeanMultiOmega782->bin(0).midpoint(), weight);
              _weightedTotalNumOmega782 += weight;
              break;
           case 333:
              _histMultiPhi->fill(scaledMom, weight);
              _histMeanMultiPhi->fill(_histMeanMultiPhi->bin(0).midpoint(), weight);
              _weightedTotalNumPhi += weight;
              break;
           case 313:
           case -313:
              _histMultiKStar892_0->fill(scaledMom, weight);
              _histMeanMultiKStar892_0->fill(_histMeanMultiKStar892_0->bin(0).midpoint(), weight);
              _weightedTotalNumKStar892_0 += weight;
              break;
           case 323:
           case -323:
              _histMultiKStar892Plus->fill(scaledEnergy, weight);
              _histMeanMultiKStar892Plus->fill(_histMeanMultiKStar892Plus->bin(0).midpoint(), weight);
              _weightedTotalNumKStar892Plus += weight;
              break;
           case 3122:
           case -3122:
              _histMultiLambda0->fill(scaledMom, weight);
              _histMeanMultiLambda0->fill(_histMeanMultiLambda0->bin(0).midpoint(), weight);
              _weightedTotalNumLambda0 += weight;
              break;
           case 3212:
           case -3212:
              _histMeanMultiSigma0->fill(_histMeanMultiSigma0->bin(0).midpoint(), weight);
              break;
           case 3312:
           case -3312:
              _histMultiXiMinus->fill(scaledEnergy, weight);
              _histMeanMultiXiMinus->fill(_histMeanMultiXiMinus->bin(0).midpoint(), weight);
              _weightedTotalNumXiMinus += weight;
              break;
           case 3114:
           case -3114:
           case 3224:
           case -3224:
              _histMultiSigma1385Plus->fill(scaledEnergy, weight);
              _histMeanMultiSigma1385Plus->fill(_histMeanMultiSigma1385Plus->bin(0).midpoint(), weight);
              _weightedTotalNumSigma1385Plus += weight;
              break;
           case 3324:
           case -3324:
              _histMultiXi1530_0->fill(scaledEnergy, weight);
              _histMeanMultiXi1530_0->fill(_histMeanMultiXi1530_0->bin(0).midpoint(), weight);
              _weightedTotalNumXi1530_0 += weight;
              break;
           case 3334:
              _histMeanMultiOmegaOmegaBar->fill(_histMeanMultiOmegaOmegaBar->bin(0).midpoint(), weight);
              break;
        }
      }

    }



    /// Finalize
    void finalize() {
      // Normalize inclusive single particle distributions to the average number
      // of charged particles per event.
      const double avgNumParts = _weightedTotalPartNum / sumOfWeights();

      normalize(_histPtSIn, avgNumParts);
      normalize(_histPtSOut, avgNumParts);

      normalize(_histRapidityT, avgNumParts);
      normalize(_histY3);

      normalize(_histLogScaledMom, avgNumParts);
      normalize(_histScaledMom, avgNumParts);

      // particle spectra
      scale(_histMultiPiPlus        ,1./sumOfWeights());
      scale(_histMultiKPlus         ,1./sumOfWeights());
      scale(_histMultiP             ,1./sumOfWeights());
      scale(_histMultiPhoton        ,1./sumOfWeights());
      scale(_histMultiPi0           ,1./sumOfWeights());
      scale(_histMultiEta           ,1./sumOfWeights());
      scale(_histMultiEtaPrime      ,1./sumOfWeights());
      scale(_histMultiK0            ,1./sumOfWeights());
      scale(_histMultiLambda0       ,1./sumOfWeights());
      scale(_histMultiXiMinus       ,1./sumOfWeights());
      scale(_histMultiSigma1385Plus ,1./sumOfWeights());
      scale(_histMultiXi1530_0      ,1./sumOfWeights());
      scale(_histMultiRho           ,1./sumOfWeights());
      scale(_histMultiOmega782      ,1./sumOfWeights());
      scale(_histMultiKStar892_0    ,1./sumOfWeights());
      scale(_histMultiPhi           ,1./sumOfWeights());

      scale(_histMultiKStar892Plus  ,1./sumOfWeights());

      //normalize(_histMultiPiPlus        ,_weightedTotalNumPiPlus / sumOfWeights());
      //normalize(_histMultiKPlus         ,_weightedTotalNumKPlus/sumOfWeights());
      //normalize(_histMultiP             ,_weightedTotalNumP/sumOfWeights());
      //normalize(_histMultiPhoton            ,_weightedTotalNumPhoton/sumOfWeights());
      //normalize(_histMultiPi0           ,_weightedTotalNumPi0/sumOfWeights());
      //normalize(_histMultiEta           ,_weightedTotalNumEta/sumOfWeights());
      //normalize(_histMultiEtaPrime      ,_weightedTotalNumEtaPrime/sumOfWeights());
      //normalize(_histMultiK0            ,_weightedTotalNumK0/sumOfWeights());
      //normalize(_histMultiLambda0       ,_weightedTotalNumLambda0/sumOfWeights());
      //normalize(_histMultiXiMinus       ,_weightedTotalNumXiMinus/sumOfWeights());
      //normalize(_histMultiSigma1385Plus ,_weightedTotalNumSigma1385Plus/sumOfWeights());
      //normalize(_histMultiXi1530_0      ,_weightedTotalNumXi1530_0 /sumOfWeights());
      //normalize(_histMultiRho           ,_weightedTotalNumRho/sumOfWeights());
      //normalize(_histMultiOmegaMinus    ,_weightedTotalNumOmegaMinus/sumOfWeights());
      //normalize(_histMultiKStar892_0    ,_weightedTotalNumKStar892_0/sumOfWeights());
      //normalize(_histMultiPhi           ,_weightedTotalNumPhi/sumOfWeights());

      //normalize(_histMultiKStar892Plus  ,_weightedTotalNumKStar892Plus/sumOfWeights());

      // event shape
      normalize(_hist1MinusT);
      normalize(_histTMinor);
      normalize(_histOblateness);

      normalize(_histSphericity);
      normalize(_histAplanarity);
      normalize(_histHeavyJetMass);
      normalize(_histCParam);


      // mean multiplicities
      scale(_histChMult              , 2.0/sumOfWeights()); // taking into account the binwidth of 2
      scale(_histMeanChMult          , 1.0/sumOfWeights());
      scale(_histMeanChMultRapt05    , 1.0/sumOfWeights());
      scale(_histMeanChMultRapt10    , 1.0/sumOfWeights());
      scale(_histMeanChMultRapt15    , 1.0/sumOfWeights());
      scale(_histMeanChMultRapt20    , 1.0/sumOfWeights());


      scale(_histMeanMultiPi0          , 1.0/sumOfWeights());
      scale(_histMeanMultiEta          , 1.0/sumOfWeights());
      scale(_histMeanMultiEtaPrime     , 1.0/sumOfWeights());
      scale(_histMeanMultiK0           , 1.0/sumOfWeights());
      scale(_histMeanMultiRho          , 1.0/sumOfWeights());
      scale(_histMeanMultiOmega782     , 1.0/sumOfWeights());
      scale(_histMeanMultiPhi          , 1.0/sumOfWeights());
      scale(_histMeanMultiKStar892Plus , 1.0/sumOfWeights());
      scale(_histMeanMultiKStar892_0   , 1.0/sumOfWeights());
      scale(_histMeanMultiLambda0      , 1.0/sumOfWeights());
      scale(_histMeanMultiSigma0       , 1.0/sumOfWeights());
      scale(_histMeanMultiXiMinus      , 1.0/sumOfWeights());
      scale(_histMeanMultiSigma1385Plus, 1.0/sumOfWeights());
      scale(_histMeanMultiXi1530_0     , 1.0/sumOfWeights());
      scale(_histMeanMultiOmegaOmegaBar, 1.0/sumOfWeights());
    }

    //@}


  private:
    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles - used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.
    double _weightedTotalPartNum;
    double _weightedTotalNumPiPlus;
    double _weightedTotalNumKPlus;
    double _weightedTotalNumP;
    double _weightedTotalNumPhoton;
    double _weightedTotalNumPi0;
    double _weightedTotalNumEta;
    double _weightedTotalNumEtaPrime;
    double _weightedTotalNumK0;
    double _weightedTotalNumLambda0;
    double _weightedTotalNumXiMinus;
    double _weightedTotalNumSigma1385Plus;
    double _weightedTotalNumXi1530_0;
    double _weightedTotalNumRho;
    double _weightedTotalNumOmega782;
    double _weightedTotalNumKStar892_0;
    double _weightedTotalNumPhi;
    double _weightedTotalNumKStar892Plus;
    double _numChParticles;

    /// @name Histograms
    //@{
    Histo1DPtr _histSphericity;
    Histo1DPtr _histAplanarity;

    Histo1DPtr _hist1MinusT;
    Histo1DPtr _histTMinor;

    Histo1DPtr _histY3;
    Histo1DPtr _histHeavyJetMass;
    Histo1DPtr _histCParam;
    Histo1DPtr _histOblateness;

    Histo1DPtr _histScaledMom;
    Histo1DPtr _histRapidityT;

    Histo1DPtr _histPtSIn;
    Histo1DPtr _histPtSOut;

    Histo1DPtr _histJetRate2Durham;
    Histo1DPtr _histJetRate3Durham;
    Histo1DPtr _histJetRate4Durham;
    Histo1DPtr _histJetRate5Durham;

    Histo1DPtr _histLogScaledMom;

    Histo1DPtr _histChMult;

    Histo1DPtr _histMultiPiPlus;
    Histo1DPtr _histMultiKPlus;
    Histo1DPtr _histMultiP;
    Histo1DPtr _histMultiPhoton;
    Histo1DPtr _histMultiPi0;
    Histo1DPtr _histMultiEta;
    Histo1DPtr _histMultiEtaPrime;
    Histo1DPtr _histMultiK0;
    Histo1DPtr _histMultiLambda0;
    Histo1DPtr _histMultiXiMinus;
    Histo1DPtr _histMultiSigma1385Plus;
    Histo1DPtr _histMultiXi1530_0;
    Histo1DPtr _histMultiRho;
    Histo1DPtr _histMultiOmega782;
    Histo1DPtr _histMultiKStar892_0;
    Histo1DPtr _histMultiPhi;
    Histo1DPtr _histMultiKStar892Plus;

    // mean multiplicities
    Histo1DPtr _histMeanChMult;
    Histo1DPtr _histMeanChMultRapt05;
    Histo1DPtr _histMeanChMultRapt10;
    Histo1DPtr _histMeanChMultRapt15;
    Histo1DPtr _histMeanChMultRapt20;

    Histo1DPtr _histMeanMultiPi0;
    Histo1DPtr _histMeanMultiEta;
    Histo1DPtr _histMeanMultiEtaPrime;
    Histo1DPtr _histMeanMultiK0;
    Histo1DPtr _histMeanMultiRho;
    Histo1DPtr _histMeanMultiOmega782;
    Histo1DPtr _histMeanMultiPhi;
    Histo1DPtr _histMeanMultiKStar892Plus;
    Histo1DPtr _histMeanMultiKStar892_0;
    Histo1DPtr _histMeanMultiLambda0;
    Histo1DPtr _histMeanMultiSigma0;
    Histo1DPtr _histMeanMultiXiMinus;
    Histo1DPtr _histMeanMultiSigma1385Plus;
    Histo1DPtr _histMeanMultiXi1530_0;
    Histo1DPtr _histMeanMultiOmegaOmegaBar;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALEPH_1996_S3486095);

}

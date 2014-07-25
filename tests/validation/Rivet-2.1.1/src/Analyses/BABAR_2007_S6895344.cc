// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief BABAR Lambda_c from fragmentation
  /// @author Peter Richardson
  class BABAR_2007_S6895344 : public Analysis {
  public:

    BABAR_2007_S6895344()
      : Analysis("BABAR_2007_S6895344")
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

      // Particle masses from PDGlive (accessed online 16. Nov. 2009).
      foreach (const Particle& p, ufs.particles()) {
        // only looking at Lambda_c
        if(abs(p.pdgId())!=4122) continue;
        MSG_DEBUG("Lambda_c found");
        double mH2 = 5.22780; // 2.28646^2
        const double mom = cms_boost.transform(p.momentum()).vector3().mod();
        double xp = mom/sqrt(s/4.0 - mH2);

        if(onresonance) {
          _histOn  ->fill(xp,weight);
          _sigmaOn ->fill(10.58,weight);
        }
        else {
          _histOff ->fill(xp,weight);
          _sigmaOff->fill(10.54,weight);
        }
      }
    } // analyze


    void finalize() {

      scale(_sigmaOn , 1./sumOfWeights());
      scale(_sigmaOff, 1./sumOfWeights());
      scale(_histOn  , 1./sumOfWeights());
      scale(_histOff , 1./sumOfWeights());
    } // finalize


    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");

      _histOff  = bookHisto1D(1,1,1);
      _sigmaOff = bookHisto1D(2,1,1);
      _histOn   = bookHisto1D(3,1,1);
      _sigmaOn  = bookHisto1D(4,1,1);

    } // init

  private:

    //@{
    // Histograms for the continuum cross sections
    Histo1DPtr _sigmaOn ;
    Histo1DPtr _sigmaOff;
    Histo1DPtr _histOn  ;
    Histo1DPtr _histOff ;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BABAR_2007_S6895344);

}

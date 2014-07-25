// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief CLEO charmed mesons and baryons from fragmentation
  /// @author Peter Richardson
  class CLEO_2004_S5809304 : public Analysis {
  public:

    CLEO_2004_S5809304()
      : Analysis("CLEO_2004_S5809304")
    { }


    void analyze(const Event& e) {
      const double weight = e.weight();

      // Loop through unstable FS particles and look for charmed mesons/baryons
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      const Beam beamproj = applyProjection<Beam>(e, "Beams");
      const ParticlePair& beams = beamproj.beams();
      FourMomentum mom_tot = beams.first.momentum() + beams.second.momentum();
      LorentzTransform cms_boost;
      if(mom_tot.vector3().mod()>0.001)
	cms_boost = LorentzTransform(-mom_tot.boostVector());
      const double s = sqr(beamproj.sqrtS());

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
          _sigmaD0A->fill(10.6,weight);
          _sigmaD0B->fill(10.6,weight);
          _histXpD0A->fill(xp, weight);
          _histXpD0B->fill(xp, weight);
          _histXpTotal->fill(xp, weight);
          break;
        case 411:
          MSG_DEBUG("D+ found");
          mH2 = 3.49547; // 1.86962^2
          xp = mom/sqrt(s/4.0 - mH2);
          _sigmaDPlus->fill(10.6,weight);
          _histXpDplus->fill(xp, weight);
          _histXpTotal->fill(xp, weight);
          break;

        case 413:
          MSG_DEBUG("D*+ found");
          mH2 = 4.04119; // 2.01027^2
          xp = mom/sqrt(s/4.0 - mH2);
          _sigmaDStarPlusA->fill(10.6,weight);
          _sigmaDStarPlusB->fill(10.6,weight);
          _histXpDStarPlusA->fill(xp, weight);
          _histXpDStarPlusB->fill(xp, weight);
          _histXpTotal->fill(xp, weight);
          break;

        case 423:
          MSG_DEBUG("D*0 found");
          mH2 = 4.02793; // 2.00697**2
          xp = mom/sqrt(s/4.0 - mH2);
          _sigmaDStar0A->fill(10.6,weight);
          _sigmaDStar0B->fill(10.6,weight);
          _histXpDStar0A->fill(xp, weight);
          _histXpDStar0B->fill(xp, weight);
          _histXpTotal->fill(xp, weight);
          break;
        }
      }
    } // analyze


    void finalize() {

      scale(_sigmaDPlus     , crossSection()/picobarn/sumOfWeights());
      scale(_sigmaD0A       , crossSection()/picobarn/sumOfWeights());
      scale(_sigmaD0B       , crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStarPlusA, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStarPlusB, crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStar0A   , crossSection()/picobarn/sumOfWeights());
      scale(_sigmaDStar0B   , crossSection()/picobarn/sumOfWeights());

      scale(_histXpDplus     , crossSection()/picobarn/sumOfWeights());
      scale(_histXpD0A       , crossSection()/picobarn/sumOfWeights());
      scale(_histXpD0B       , crossSection()/picobarn/sumOfWeights());
      scale(_histXpDStarPlusA, crossSection()/picobarn/sumOfWeights());
      scale(_histXpDStarPlusB, crossSection()/picobarn/sumOfWeights());
      scale(_histXpDStar0A   , crossSection()/picobarn/sumOfWeights());
      scale(_histXpDStar0B   , crossSection()/picobarn/sumOfWeights());
      scale(_histXpTotal     , crossSection()/picobarn/sumOfWeights()/4.);
    } // finalize


    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");

      // continuum cross sections
      _sigmaDPlus      = bookHisto1D(1,1,1);
      _sigmaD0A        = bookHisto1D(1,1,2);
      _sigmaD0B        = bookHisto1D(1,1,3);
      _sigmaDStarPlusA = bookHisto1D(1,1,4);
      _sigmaDStarPlusB = bookHisto1D(1,1,5);
      _sigmaDStar0A    = bookHisto1D(1,1,6);
      _sigmaDStar0B    = bookHisto1D(1,1,7);

       // histograms for continuum data
      _histXpDplus      = bookHisto1D(2, 1, 1);
      _histXpD0A        = bookHisto1D(3, 1, 1);
      _histXpD0B        = bookHisto1D(4, 1, 1);
      _histXpDStarPlusA = bookHisto1D(5, 1, 1);
      _histXpDStarPlusB = bookHisto1D(6, 1, 1);
      _histXpDStar0A    = bookHisto1D(7, 1, 1);
      _histXpDStar0B    = bookHisto1D(8, 1, 1);
      _histXpTotal      = bookHisto1D(9, 1, 1);

    } // init

  private:

    //@{
    // Histograms for the continuum cross sections
    Histo1DPtr _sigmaDPlus     ;
    Histo1DPtr _sigmaD0A       ;
    Histo1DPtr _sigmaD0B       ;
    Histo1DPtr _sigmaDStarPlusA;
    Histo1DPtr _sigmaDStarPlusB;
    Histo1DPtr _sigmaDStar0A   ;
    Histo1DPtr _sigmaDStar0B   ;

    // histograms for continuum data
    Histo1DPtr _histXpDplus     ;
    Histo1DPtr _histXpD0A       ;
    Histo1DPtr _histXpD0B       ;
    Histo1DPtr _histXpDStarPlusA;
    Histo1DPtr _histXpDStarPlusB;
    Histo1DPtr _histXpDStar0A   ;
    Histo1DPtr _histXpDStar0B   ;
    Histo1DPtr _histXpTotal     ;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CLEO_2004_S5809304);

}

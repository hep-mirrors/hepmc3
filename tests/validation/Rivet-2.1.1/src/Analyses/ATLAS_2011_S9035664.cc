// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief  J/psi production at ATLAS
  class ATLAS_2011_S9035664: public Analysis {
  public:

    /// Constructor
    ATLAS_2011_S9035664()
      : Analysis("ATLAS_2011_S9035664")
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(UnstableFinalState(), "UFS");
      _nonPrRapHigh    = bookHisto1D( 14, 1, 1);
      _nonPrRapMedHigh = bookHisto1D( 13, 1, 1);
      _nonPrRapMedLow  = bookHisto1D( 12, 1, 1);
      _nonPrRapLow     = bookHisto1D( 11, 1, 1);
      _PrRapHigh       = bookHisto1D( 18, 1, 1);
      _PrRapMedHigh    = bookHisto1D( 17, 1, 1);
      _PrRapMedLow     = bookHisto1D( 16, 1, 1);
      _PrRapLow        = bookHisto1D( 15, 1, 1);
      _IncRapHigh      = bookHisto1D( 20, 1, 1);
      _IncRapMedHigh   = bookHisto1D( 21, 1, 1);
      _IncRapMedLow    = bookHisto1D( 22, 1, 1);
      _IncRapLow       = bookHisto1D( 23, 1, 1);
    }


    void analyze(const Event& e) {

      // Get event weight for histo filling
      const double weight = e.weight();


      // Final state of unstable particles to get particle spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      foreach (const Particle& p, ufs.particles()) {
        if (abs(p.pdgId()) != 443) continue;
        HepMC::GenVertex* gv = p.genParticle()->production_vertex();
        bool nonPrompt = false;
        if (gv) {
          foreach (const GenParticle* pi, Rivet::particles(gv, HepMC::ancestors)) {
            const PdgId pid2 = pi->pdg_id();
            if (PID::isHadron(pid2) && PID::hasBottom(pid2)) {
              nonPrompt = true;
              break;
            }
          }
        }
        double rapidity = p.rapidity();
        double xp = p.momentum().perp();

        if (rapidity<=2.4 and rapidity>2.) {
          if (nonPrompt) _nonPrRapHigh->fill(xp, weight);
          else if (!nonPrompt) _PrRapHigh->fill(xp, weight);
          _IncRapHigh->fill(xp, weight);
        }
        else if (rapidity<=2. and rapidity>1.5) {
          if (nonPrompt) _nonPrRapMedHigh->fill(xp, weight);
          else if (!nonPrompt) _PrRapMedHigh->fill(xp, weight);
          _IncRapMedHigh->fill(xp, weight);
        }
        else if (rapidity<=1.5 and rapidity>0.75) {
          if (nonPrompt) _nonPrRapMedLow->fill(xp, weight);
          else if (!nonPrompt) _PrRapMedLow->fill(xp, weight);
          _IncRapMedLow->fill(xp, weight);
        }

        else if (rapidity<=0.75) {
          if (nonPrompt) _nonPrRapLow->fill(xp, weight);
          else if (!nonPrompt) _PrRapLow->fill(xp, weight);
          _IncRapLow->fill(xp, weight);
        }
      }
    }


    /// Finalize
    void finalize() {
      double factor = crossSection()/nanobarn*0.0593;

      scale(_PrRapHigh      , factor/sumOfWeights());
      scale(_PrRapMedHigh   , factor/sumOfWeights());
      scale(_PrRapMedLow    , factor/sumOfWeights());
      scale(_PrRapLow       , factor/sumOfWeights());

      scale(_nonPrRapHigh   , factor/sumOfWeights());
      scale(_nonPrRapMedHigh, factor/sumOfWeights());
      scale(_nonPrRapMedLow , factor/sumOfWeights());
      scale(_nonPrRapLow    , factor/sumOfWeights());

      scale(_IncRapHigh     , 1000.*factor/sumOfWeights());
      scale(_IncRapMedHigh  , 1000.*factor/sumOfWeights());
      scale(_IncRapMedLow   , 1000.*factor/sumOfWeights());
      scale(_IncRapLow      , 1000.*factor/sumOfWeights());

    }

    //@}


  private:

    Histo1DPtr _nonPrRapHigh;
    Histo1DPtr _nonPrRapMedHigh;
    Histo1DPtr _nonPrRapMedLow;
    Histo1DPtr _nonPrRapLow;

    Histo1DPtr _PrRapHigh;
    Histo1DPtr _PrRapMedHigh;
    Histo1DPtr _PrRapMedLow;
    Histo1DPtr _PrRapLow;

    Histo1DPtr _IncRapHigh;
    Histo1DPtr _IncRapMedHigh;
    Histo1DPtr _IncRapMedLow;
    Histo1DPtr _IncRapLow;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9035664);

}

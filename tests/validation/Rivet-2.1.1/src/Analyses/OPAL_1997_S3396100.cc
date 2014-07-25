// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief OPAL strange baryon paper
  /// @author Peter Richardson
  class OPAL_1997_S3396100 : public Analysis {
  public:

    /// Constructor
    OPAL_1997_S3396100()
      : Analysis("OPAL_1997_S3396100"),
        _weightedTotalNumLambda(0.)     ,_weightedTotalNumXiMinus(0.),
        _weightedTotalNumSigma1385Plus(0.),_weightedTotalNumSigma1385Minus(0.),
        _weightedTotalNumXi1530(0.)   ,_weightedTotalNumLambda1520(0.)
    {}


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");
      _histXpLambda         = bookHisto1D( 1, 1, 1);
      _histXiLambda         = bookHisto1D( 2, 1, 1);
      _histXpXiMinus        = bookHisto1D( 3, 1, 1);
      _histXiXiMinus        = bookHisto1D( 4, 1, 1);
      _histXpSigma1385Plus  = bookHisto1D( 5, 1, 1);
      _histXiSigma1385Plus  = bookHisto1D( 6, 1, 1);
      _histXpSigma1385Minus = bookHisto1D( 7, 1, 1);
      _histXiSigma1385Minus = bookHisto1D( 8, 1, 1);
      _histXpXi1530         = bookHisto1D( 9, 1, 1);
      _histXiXi1530         = bookHisto1D(10, 1, 1);
      _histXpLambda1520     = bookHisto1D(11, 1, 1);
      _histXiLambda1520     = bookHisto1D(12, 1, 1);
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

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);

      // Final state of unstable particles to get particle spectra
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");

      foreach (const Particle& p, ufs.particles()) {
        const int id = abs(p.pdgId());
        double xp = p.momentum().vector3().mod()/meanBeamMom;
        double xi = -log(xp);
        switch (id) {
        case 3312:
          _histXpXiMinus->fill(xp, weight);
          _histXiXiMinus->fill(xi, weight);
          _weightedTotalNumXiMinus += weight;
          break;
        case 3224:
          _histXpSigma1385Plus->fill(xp, weight);
          _histXiSigma1385Plus->fill(xi, weight);
          _weightedTotalNumSigma1385Plus += weight;
          break;
        case 3114:
          _histXpSigma1385Minus->fill(xp, weight);
          _histXiSigma1385Minus->fill(xi, weight);
          _weightedTotalNumSigma1385Minus += weight;
          break;
        case 3122:
          _histXpLambda->fill(xp, weight);
          _histXiLambda->fill(xi, weight);
          _weightedTotalNumLambda += weight;
          break;
        case 3324:
          _histXpXi1530->fill(xp, weight);
          _histXiXi1530->fill(xi, weight);
          _weightedTotalNumXi1530 += weight;
          break;
        case 3124:
          _histXpLambda1520->fill(xp, weight);
          _histXiLambda1520->fill(xi, weight);
          _weightedTotalNumLambda1520 += weight;
          break;
        }
      }
    }


    /// Finalize
    void finalize() {
      normalize(_histXpLambda        , _weightedTotalNumLambda       /sumOfWeights());
      normalize(_histXiLambda        , _weightedTotalNumLambda       /sumOfWeights());
      normalize(_histXpXiMinus       , _weightedTotalNumXiMinus      /sumOfWeights());
      normalize(_histXiXiMinus       , _weightedTotalNumXiMinus      /sumOfWeights());
      normalize(_histXpSigma1385Plus , _weightedTotalNumSigma1385Plus/sumOfWeights());
      normalize(_histXiSigma1385Plus , _weightedTotalNumSigma1385Plus/sumOfWeights());
      normalize(_histXpSigma1385Minus, _weightedTotalNumSigma1385Plus/sumOfWeights());
      normalize(_histXiSigma1385Minus, _weightedTotalNumSigma1385Plus/sumOfWeights());
      normalize(_histXpXi1530        , _weightedTotalNumXi1530       /sumOfWeights());
      normalize(_histXiXi1530        , _weightedTotalNumXi1530       /sumOfWeights());
      normalize(_histXpLambda1520    , _weightedTotalNumLambda1520   /sumOfWeights());
      normalize(_histXiLambda1520    , _weightedTotalNumLambda1520   /sumOfWeights());
    }

    //@}


  private:

    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles - used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.
    double _weightedTotalNumLambda;
    double _weightedTotalNumXiMinus;
    double _weightedTotalNumSigma1385Plus;
    double _weightedTotalNumSigma1385Minus;
    double _weightedTotalNumXi1530;
    double _weightedTotalNumLambda1520;

    Histo1DPtr _histXpLambda        ;
    Histo1DPtr _histXiLambda        ;
    Histo1DPtr _histXpXiMinus       ;
    Histo1DPtr _histXiXiMinus       ;
    Histo1DPtr _histXpSigma1385Plus ;
    Histo1DPtr _histXiSigma1385Plus ;
    Histo1DPtr _histXpSigma1385Minus;
    Histo1DPtr _histXiSigma1385Minus;
    Histo1DPtr _histXpXi1530        ;
    Histo1DPtr _histXiXi1530        ;
    Histo1DPtr _histXpLambda1520    ;
    Histo1DPtr _histXiLambda1520    ;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1997_S3396100);

}

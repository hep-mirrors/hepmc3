// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  /// @brief DELPHI strange baryon paper
  /// @author Hendrik Hoeth
  class DELPHI_1995_S3137023 : public Analysis {
  public:

    /// Constructor
    DELPHI_1995_S3137023()
      : Analysis("DELPHI_1995_S3137023")
    {
      _weightedTotalNumXiMinus = 0;
      _weightedTotalNumSigma1385Plus = 0;
    }


    /// @name Analysis methods
    //@{

    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(UnstableFinalState(), "UFS");

      _histXpXiMinus       = bookHisto1D(2, 1, 1);
      _histXpSigma1385Plus = bookHisto1D(3, 1, 1);
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
        switch (id) {
        case 3312:
          _histXpXiMinus->fill(p.momentum().vector3().mod()/meanBeamMom, weight);
          _weightedTotalNumXiMinus += weight;
          break;
        case 3114: case 3224:
          _histXpSigma1385Plus->fill(p.momentum().vector3().mod()/meanBeamMom, weight);
          _weightedTotalNumSigma1385Plus += weight;
          break;
        }
      }

    }


    /// Finalize
    void finalize() {
      normalize(_histXpXiMinus       , _weightedTotalNumXiMinus/sumOfWeights());
      normalize(_histXpSigma1385Plus , _weightedTotalNumSigma1385Plus/sumOfWeights());
    }

    //@}


  private:

    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles - used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.
    double _weightedTotalNumXiMinus;
    double _weightedTotalNumSigma1385Plus;

    Histo1DPtr _histXpXiMinus;
    Histo1DPtr _histXpSigma1385Plus;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(DELPHI_1995_S3137023);

}

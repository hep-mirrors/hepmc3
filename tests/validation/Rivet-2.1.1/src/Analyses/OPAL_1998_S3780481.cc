// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/InitialQuarks.hh"

namespace Rivet {


  /// @brief OPAL flavour-dependent fragmentation paper
  /// @author Hendrik Hoeth
  class OPAL_1998_S3780481 : public Analysis {
  public:

    /// Constructor
    OPAL_1998_S3780481() : Analysis("OPAL_1998_S3780481") {
      // Counters
      _weightedTotalPartNum = 0;
      _SumOfudsWeights = 0;
      _SumOfcWeights = 0;
      _SumOfbWeights = 0;
    }


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
      _weightedTotalPartNum += numParticles * weight;

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);

      int flavour = 0;
      const InitialQuarks& iqf = applyProjection<InitialQuarks>(e, "IQF");

      // If we only have two quarks (qqbar), just take the flavour.
      // If we have more than two quarks, look for the highest energetic q-qbar pair.
      /// @todo Yuck... does this *really* have to be quark-based?!?
      if (iqf.particles().size() == 2) {
        flavour = abs( iqf.particles().front().pdgId() );
      } else {
        map<int, double> quarkmap;
        foreach (const Particle& p, iqf.particles()) {
          if (quarkmap[p.pdgId()] < p.momentum().E()) {
            quarkmap[p.pdgId()] = p.momentum().E();
          }
        }
        double maxenergy = 0.;
        for (int i = 1; i <= 5; ++i) {
          if (quarkmap[i]+quarkmap[-i] > maxenergy) {
            flavour = i;
          }
        }
      }

      switch (flavour) {
      case 1:
      case 2:
      case 3:
        _SumOfudsWeights += weight;
        break;
      case 4:
        _SumOfcWeights += weight;
        break;
      case 5:
        _SumOfbWeights += weight;
        break;
      }

      foreach (const Particle& p, fs.particles()) {
        const double xp = p.momentum().vector3().mod()/meanBeamMom;
        const double logxp = -std::log(xp);
        _histXpall->fill(xp, weight);
        _histLogXpall->fill(logxp, weight);
        _histMultiChargedall->fill(_histMultiChargedall->bin(0).midpoint(), weight);
        switch (flavour) {
          /// @todo Use PDG code enums
        case PID::DQUARK:
        case PID::UQUARK:
        case PID::SQUARK:
          _histXpuds->fill(xp, weight);
          _histLogXpuds->fill(logxp, weight);
          _histMultiChargeduds->fill(_histMultiChargeduds->bin(0).midpoint(), weight);
          break;
        case PID::CQUARK:
          _histXpc->fill(xp, weight);
          _histLogXpc->fill(logxp, weight);
          _histMultiChargedc->fill(_histMultiChargedc->bin(0).midpoint(), weight);
          break;
        case PID::BQUARK:
          _histXpb->fill(xp, weight);
          _histLogXpb->fill(logxp, weight);
          _histMultiChargedb->fill(_histMultiChargedb->bin(0).midpoint(), weight);
          break;
        }
      }

    }


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(InitialQuarks(), "IQF");

      // Book histos
      _histXpuds           = bookHisto1D(1, 1, 1);
      _histXpc             = bookHisto1D(2, 1, 1);
      _histXpb             = bookHisto1D(3, 1, 1);
      _histXpall           = bookHisto1D(4, 1, 1);
      _histLogXpuds        = bookHisto1D(5, 1, 1);
      _histLogXpc          = bookHisto1D(6, 1, 1);
      _histLogXpb          = bookHisto1D(7, 1, 1);
      _histLogXpall        = bookHisto1D(8, 1, 1);
      _histMultiChargeduds = bookHisto1D(9, 1, 1);
      _histMultiChargedc   = bookHisto1D(9, 1, 2);
      _histMultiChargedb   = bookHisto1D(9, 1, 3);
      _histMultiChargedall = bookHisto1D(9, 1, 4);
    }


    /// Finalize
    void finalize() {
      const double avgNumParts = _weightedTotalPartNum / sumOfWeights();
      normalize(_histXpuds    , avgNumParts);
      normalize(_histXpc      , avgNumParts);
      normalize(_histXpb      , avgNumParts);
      normalize(_histXpall    , avgNumParts);
      normalize(_histLogXpuds , avgNumParts);
      normalize(_histLogXpc   , avgNumParts);
      normalize(_histLogXpb   , avgNumParts);
      normalize(_histLogXpall , avgNumParts);

      scale(_histMultiChargeduds, 1.0/_SumOfudsWeights);
      scale(_histMultiChargedc  , 1.0/_SumOfcWeights);
      scale(_histMultiChargedb  , 1.0/_SumOfbWeights);
      scale(_histMultiChargedall, 1.0/sumOfWeights());
    }

    //@}


  private:

    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles - used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.
    double _weightedTotalPartNum;

    double _SumOfudsWeights;
    double _SumOfcWeights;
    double _SumOfbWeights;

    Histo1DPtr _histXpuds;
    Histo1DPtr _histXpc;
    Histo1DPtr _histXpb;
    Histo1DPtr _histXpall;
    Histo1DPtr _histLogXpuds;
    Histo1DPtr _histLogXpc;
    Histo1DPtr _histLogXpb;
    Histo1DPtr _histLogXpall;
    Histo1DPtr _histMultiChargeduds;
    Histo1DPtr _histMultiChargedc;
    Histo1DPtr _histMultiChargedb;
    Histo1DPtr _histMultiChargedall;

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1998_S3780481);

}

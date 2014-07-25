// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ParisiTensor.hh"
#include "Rivet/Projections/Hemispheres.hh"
#include "Rivet/Projections/InitialQuarks.hh"
#include <cmath>

namespace Rivet {


  /// @brief OPAL multiplicities at various energies
  /// @author Peter Richardson
  class DELPHI_2000_S4328825 : public Analysis {
  public:

    /// Constructor
    DELPHI_2000_S4328825()
      : Analysis("DELPHI_2000_S4328825"),
        _weightedTotalChargedPartNumLight(0.),
        _weightedTotalChargedPartNumCharm(0.),
        _weightedTotalChargedPartNumBottom(0.),
        _weightLight(0.),_weightCharm(0.),_weightBottom(0.)
    {}

    /// @name Analysis methods
    //@{


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "CFS");
      addProjection(InitialQuarks(), "IQF");

    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      const FinalState& cfs = applyProjection<FinalState>(event, "CFS");
      if (cfs.size() < 2) vetoEvent;


      int flavour = 0;
      const InitialQuarks& iqf = applyProjection<InitialQuarks>(event, "IQF");

      // If we only have two quarks (qqbar), just take the flavour.
      // If we have more than two quarks, look for the highest energetic q-qbar pair.
      if (iqf.particles().size() == 2) {
        flavour = abs( iqf.particles().front().pdgId() );
      }
      else {
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
      const size_t numParticles = cfs.particles().size();
      switch (flavour) {
      case 1: case 2: case 3:
        _weightLight  += weight;
        _weightedTotalChargedPartNumLight  += numParticles * weight;
        break;
      case 4:
        _weightCharm  += weight;
        _weightedTotalChargedPartNumCharm  += numParticles * weight;
        break;
      case 5:
        _weightBottom += weight;
        _weightedTotalChargedPartNumBottom += numParticles * weight;
        break;
      }

    }


    void finalize() {
      Histo1D temphisto(refData(1, 1, 1));

      const double avgNumPartsBottom = _weightedTotalChargedPartNumBottom / _weightBottom;
      const double avgNumPartsCharm  = _weightedTotalChargedPartNumCharm  / _weightCharm;
      const double avgNumPartsLight  = _weightedTotalChargedPartNumLight  / _weightLight;
      Scatter2DPtr h_bottom = bookScatter2D(1, 1, 1);
      Scatter2DPtr h_charm  = bookScatter2D(1, 1, 2);
      Scatter2DPtr h_light  = bookScatter2D(1, 1, 3);
      Scatter2DPtr h_diff   = bookScatter2D(1, 1, 4);  // bottom minus light
      for (size_t b = 0; b < temphisto.numBins(); b++) {
        const double x  = temphisto.bin(b).midpoint();
        const double ex = temphisto.bin(b).width()/2.;
        if (inRange(sqrtS()/GeV, x-ex, x+ex)) {
          // @TODO: Fix y-error:
          h_bottom->addPoint(x, avgNumPartsBottom, ex, 0.);
          h_charm->addPoint(x, avgNumPartsCharm, ex, 0.);
          h_light->addPoint(x, avgNumPartsLight, ex, 0.);
          h_diff->addPoint(x, avgNumPartsBottom-avgNumPartsLight, ex, 0.);
        }
      }
    }

    //@}


  private:

    /// @name Multiplicities
    //@{
    double _weightedTotalChargedPartNumLight;
    double _weightedTotalChargedPartNumCharm;
    double _weightedTotalChargedPartNumBottom;
    //@}

    /// @name Weights
    //@{
    double _weightLight;
    double _weightCharm;
    double _weightBottom;
    //@}
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(DELPHI_2000_S4328825);

}

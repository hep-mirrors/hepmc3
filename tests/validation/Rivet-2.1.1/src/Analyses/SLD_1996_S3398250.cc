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


  /// @brief SLD multiplicities at mZ
  /// @author Peter Richardson
  class SLD_1996_S3398250 : public Analysis {
  public:

    /// Constructor
    SLD_1996_S3398250()
      : Analysis("SLD_1996_S3398250"),
        _weightLight(0.),_weightCharm(0.),_weightBottom(0.)
    {}

    /// @name Analysis methods
    //@{


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "CFS");
      addProjection(InitialQuarks(), "IQF");

      _h_bottom = bookHisto1D(1, 1, 1);
      _h_charm  = bookHisto1D(2, 1, 1);
      _h_light  = bookHisto1D(3, 1, 1);
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
        _h_light->fillBin(0, numParticles*weight);
        break;
      case 4:
        _weightCharm  += weight;
        _h_charm->fillBin(0, numParticles*weight);
        break;
      case 5:
        _weightBottom += weight;
        _h_bottom->fillBin(0, numParticles*weight);
        break;
      }

    }


    void multiplicity_subtract(const Histo1DPtr first, const Histo1DPtr second, int a, int b, int c) {
      const double x  = first->bin(0).midpoint();
      const double ex = first->bin(0).width()/2.;
      const double y  = first->bin(0).area() - second->bin(0).area();
      const double ey = sqrt(sqr(first->bin(0).areaErr()) + sqr(second->bin(0).areaErr()));
      Scatter2DPtr scatter = bookScatter2D(a, b, c);
      scatter->addPoint(x, y, ex, ey);
    }


    void finalize() {
      if (_weightBottom != 0) scale(_h_bottom, 1./_weightBottom);
      if (_weightCharm  != 0) scale(_h_charm,  1./_weightCharm );
      if (_weightLight  != 0) scale(_h_light,  1./_weightLight );

      multiplicity_subtract(_h_charm,  _h_light, 4, 1, 1);
      multiplicity_subtract(_h_bottom, _h_light, 5, 1, 1);
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

    Histo1DPtr _h_bottom;
    Histo1DPtr _h_charm;
    Histo1DPtr _h_light;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(SLD_1996_S3398250);

}

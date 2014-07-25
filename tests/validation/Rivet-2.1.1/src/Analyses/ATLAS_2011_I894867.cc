// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  class ATLAS_2011_I894867 : public Analysis {
  public:

    ATLAS_2011_I894867()
      : Analysis("ATLAS_2011_I894867")
    {    }

  public:

    void init() {
      addProjection(FinalState(), "FS");
      _h_sigma = bookHisto1D(1, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      if (fs.size() < 2) vetoEvent; // need at least two particles to calculate gaps

      double gapcenter = 0.;
      double LRG = 0.;
      double etapre = 0.;
      bool first = true;

      foreach(const Particle& p, fs.particlesByEta()) { // sorted from minus to plus
        if (first) { // First particle
          first = false;
          etapre = p.eta();
        } else {
          double gap = fabs(p.eta()-etapre);
          if (gap > LRG) {
            LRG = gap; // largest gap
            gapcenter = (p.eta()+etapre)/2.; // find the center of the gap to separate the X and Y systems.
          }
          etapre = p.eta();
        }
      }


      FourMomentum mxFourVector, myFourVector;
      foreach(const Particle& p, fs.particlesByEta()) {
        if (p.eta() > gapcenter) {
          mxFourVector += p.momentum();
        } else {
          myFourVector += p.momentum();
        }
      }
      const double M2 = max(mxFourVector.mass2(), myFourVector.mass2());
      const double xi = M2/sqr(sqrtS()); // sqrt(s)=7000 GeV, note that units cancel
      if (xi < 5e-6) vetoEvent;

      _h_sigma->fill(sqrtS()/GeV, weight);
    }


    void finalize() {
      scale(_h_sigma, crossSection()/millibarn/sumOfWeights());
    }

  private:

    Histo1DPtr _h_sigma;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_I894867);

}

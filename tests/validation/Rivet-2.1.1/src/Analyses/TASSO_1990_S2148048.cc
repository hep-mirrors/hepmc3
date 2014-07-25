// -*- C++ -*-
#include "Rivet/Analysis.hh"
//#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
/// @todo Include more projections as required, e.g. ChargedFinalState, FastJets, ZFinder...

namespace Rivet {


  class TASSO_1990_S2148048 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    TASSO_1990_S2148048()
      : Analysis("TASSO_1990_S2148048")
    {
      /// @todo Set whether your finalize method needs the generator cross section
      //_sumWPassed = 0;
    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      const ChargedFinalState cfs(-MAXDOUBLE, MAXDOUBLE, 0.1/GeV);
      addProjection(cfs, "CFS");

      //// Beams -- needed for x_p calculation
      //addProjection(Beam(), "Beams");

      // Thrust
      addProjection(Thrust(cfs), "Thrust");

      // For Sphericity and the like
      addProjection(Sphericity(cfs), "Sphericity");

      // Histos
      int offset = 0;
      switch (int(sqrtS()/GeV)) {
        case 14:
          offset = 0;
          break;
        case 22:
          offset = 1;
          break;
        case 35:
          offset = 2;
          break;
        case 44:
          offset = 3;
          break;
      }
      //_h_xp         = bookHisto1D( 2, 1, 1+offset);
      _h_sphericity = bookHisto1D( 6, 1, 1+offset);
      _h_aplanarity = bookHisto1D( 7, 1, 1+offset);
      _h_thrust     = bookHisto1D( 8, 1, 1+offset);
    }



    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();
      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");

      //// Get beams and average beam momentum
      //const ParticlePair& beams = applyProjection<Beam>(event, "Beams").beams();
      //const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   //beams.second.momentum().vector3().mod() ) / 2.0;

      // TASSO hadronic event selection TODO: move this into a trigger definition
      // See page 2 in publication
      // Condition 1)  --- require at least 5 (4) 'good' tracks
      int nch = cfs.particles().size();
      if ( (int(sqrtS()/GeV) > 27 && nch < 5) || (int(sqrtS()/GeV) <= 27 && nch < 4 ) ) {
        MSG_DEBUG("Failed # good tracks cut: " << nch);
        vetoEvent;
      }
      // Condition 2) ---
      // Condition 5) --- scalar momentum (not pT!!!) sum >= 0.265*s
      double momsum = 0.0;
      foreach (const Particle& p, cfs.particles()) {
        const double mom = p.momentum().vector3().mod();
        momsum += mom;
      }
      if (momsum <=0.265 * sqrtS()/GeV) {
        MSG_DEBUG("Failed pTsum cut: " << momsum << " < " << 0.265 * sqrtS()/GeV);
        vetoEvent;
      }

      // Raise counter for events that pass trigger conditions
      //_sumWPassed += event.weight();

      const Thrust& thrust = applyProjection<Thrust>(event, "Thrust");
      //const Vector3 & thrustAxis = thrust.thrustAxis ();
      //double theta = thrustAxis.theta();
      //if ( fabs(cos(theta)) >= 0.8 ) {
        //MSG_DEBUG("Failed thrust angle cut: " << fabs(cos(theta)));
        //vetoEvent;
      //}

      const Sphericity& sphericity = applyProjection<Sphericity>(event, "Sphericity");

      //// Fill histograms in order of appearance in paper
      //foreach (const Particle& p, cfs.particles()) {
        //// Get momentum and energy of each particle.
        //const Vector3 mom3 = p.momentum().vector3();
        //// Scaled momenta.
        //const double mom = mom3.mod();
        //const double scaledMom = mom/meanBeamMom;
        //_h_xp->fill(scaledMom, weight);
      //}
      //
      _h_sphericity->fill(sphericity.sphericity(), weight);
      _h_aplanarity->fill(sphericity.aplanarity(), weight);
      _h_thrust->fill(thrust.thrust(), weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      //scale(_h_xp, _sumWPassed/(crossSection()*sumOfWeights()));
      normalize(_h_sphericity);
      normalize(_h_aplanarity);
      normalize(_h_thrust    );
    }

    //@}


  private:

    // Data members like post-cuts event weight counters go here

    //double _sumWPassed;

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_xp        ;
    Histo1DPtr _h_sphericity;
    Histo1DPtr _h_aplanarity;
    Histo1DPtr _h_thrust    ;
    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(TASSO_1990_S2148048);

}

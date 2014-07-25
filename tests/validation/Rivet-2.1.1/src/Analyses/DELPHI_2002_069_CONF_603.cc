// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"


/// @todo Use inline PID functions instead
#define IS_PARTON_PDGID(id) ( abs(id) <= 100 && abs(id) != 22 && (abs(id) < 11 || abs(id) > 18) )
#define IS_BHADRON_PDGID(id) ( ((abs(id)/100)%10 == 5) || (abs(id) >= 5000 && abs(id) <= 5999) )

namespace Rivet {


  /// @brief DELPHI b-fragmentation measurement
  /// @author Hendrik Hoeth
  class DELPHI_2002_069_CONF_603 : public Analysis {
  public:

    /// Constructor
    DELPHI_2002_069_CONF_603()
      : Analysis("DELPHI_2002_069_CONF_603")
    {
    }


    /// @name Analysis methods
    //@{

    /// Book projections and histograms
    void init() {
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");

      _histXbprim     = bookHisto1D(1, 1, 1);
      _histXbweak     = bookHisto1D(2, 1, 1);
      _histMeanXbprim = bookProfile1D(4, 1, 1);
      _histMeanXbweak = bookProfile1D(5, 1, 1);
    }


    void analyze(const Event& e) {
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

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);


      foreach (const GenParticle* p, particles(e.genEvent())) {
        const GenVertex* pv = p->production_vertex();
        const GenVertex* dv = p->end_vertex();
        if (IS_BHADRON_PDGID(p->pdg_id())) {
          const double xp = p->momentum().e()/meanBeamMom;

          // If the B-hadron has a parton as parent, call it primary B-hadron:
          if (pv) {
            bool is_primary = false;
            for (GenVertex::particles_in_const_iterator pp = pv->particles_in_const_begin(); pp != pv->particles_in_const_end() ; ++pp) {
              if (IS_PARTON_PDGID((*pp)->pdg_id())) is_primary = true;
            }
            if (is_primary) {
              _histXbprim->fill(xp, weight);
              _histMeanXbprim->fill(_histMeanXbprim->bin(0).midpoint(), xp, weight);
            }
          }

          // If the B-hadron has no B-hadron as a child, it decayed weakly:
          if (dv) {
            bool is_weak = true;
            for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin() ;
                 pp != dv->particles_out_const_end() ; ++pp) {
              if (IS_BHADRON_PDGID((*pp)->pdg_id())) {
                is_weak = false;
              }
            }
            if (is_weak) {
              _histXbweak->fill(xp, weight);
              _histMeanXbweak->fill(_histMeanXbweak->bin(0).midpoint(), xp, weight);
            }
          }

        }
      }
    }


    // Finalize
    void finalize() {
      normalize(_histXbprim);
      normalize(_histXbweak);
    }


  private:

    /// Store the weighted sums of numbers of charged / charged+neutral
    /// particles - used to calculate average number of particles for the
    /// inclusive single particle distributions' normalisations.

    Histo1DPtr _histXbprim;
    Histo1DPtr _histXbweak;

    Profile1DPtr _histMeanXbprim;
    Profile1DPtr _histMeanXbweak;

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(DELPHI_2002_069_CONF_603);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Particle.hh"

namespace Rivet {

  class ATLAS_2012_I1118269 : public Analysis {
  public:

    ATLAS_2012_I1118269() : Analysis("ATLAS_2012_I1118269")
    {  }

    void init() {
      _h_sigma_vs_pt  = bookHisto1D(1, 1, 1);
      _h_sigma_vs_eta = bookHisto1D(2, 1, 1);
    }

    /// Perform the per-event analysis
    void analyze(const Event& event) {
      double weight = event.weight();

      Particles bhadrons;
      foreach (const GenParticlePtr p, particles(event.genEvent())) {

        if (!( PID::isHadron( p->pdg_id() ) && PID::hasBottom( p->pdg_id() )) ) continue;

        const GenVertexPtr dv = p->end_vertex();

        /// @todo In future, convert to use built-in 'last B hadron' function
        bool hasBdaughter = false;
        if ( PID::isHadron( p->pdg_id() ) && PID::hasBottom( p->pdg_id() )) { // b-hadron selection
          if (dv) {
            for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin() ; pp != dv->particles_out_const_end() ; ++pp) {
              if ( PID::isHadron( (*pp)->pdg_id() ) && PID::hasBottom( (*pp)->pdg_id()) ) {
                hasBdaughter = true;
              }
            }
          }
        }
        if (hasBdaughter) continue;

        bhadrons += Particle(*p);
      }

      foreach (const Particle& particle, bhadrons) {
        double eta = particle.eta();
        double pt = particle.pT();

        if (!(inRange(eta, -2.5, 2.5))) continue;
        if (pt < 9.*GeV) continue;

        _h_sigma_vs_pt->fill(pt, weight);
        _h_sigma_vs_eta->fill(fabs(eta), weight);

      }

    }


    void finalize() {
      scale(_h_sigma_vs_pt,  crossSection()/nanobarn/sumOfWeights());
      scale(_h_sigma_vs_eta, crossSection()/microbarn/sumOfWeights());
    }


  private:

    Histo1DPtr _h_sigma_vs_pt;
    Histo1DPtr _h_sigma_vs_eta;

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1118269);

}

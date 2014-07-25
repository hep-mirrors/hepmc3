// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"


bool hasDecayedTo(const HepMC3::GenParticlePtr p, int id1, int id2) {
  bool decision = false;
  HepMC3::GenVertexPtr decV  = p->end_vertex();
  std::vector<int> decids;
  if (decV->particles_out().size() == 2) {
    for (HepMC3::GenVertex::particles_out_const_iterator pp = decV->particles_out_const_begin() ;
         pp != decV->particles_out_const_end() ; ++pp) {
      decids.push_back(abs((*pp)->pdg_id()));
    }
    if ( (decids[0] == abs(id1) && decids[1] == abs(id2)) || (decids[1] == abs(id1) && decids[0] == abs(id2)) ) decision = true;

  };
  return decision;
}

bool hasDecayedTo(const Rivet::Particle& p, int id1, int id2) {
  return hasDecayedTo(p.genParticle(), id1, id2);
}


namespace Rivet {


  class ALEPH_1999_S4193598 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ALEPH_1999_S4193598()
      : Analysis("ALEPH_1999_S4193598")
    {
      _sumWpassed = 0.0;
    }

    //@}


  public:

    /// Book histograms and initialise projections before the run
    void init() {
      addProjection(Beam(), "Beams");
      addProjection(UnstableFinalState(), "UFS");
      addProjection(ChargedFinalState(), "CFS");

      _h_Xe_Ds = bookHisto1D(1, 1, 1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // Trigger condition
      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");
      if (cfs.size() < 5) vetoEvent;

      _sumWpassed += weight;

      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");

      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(event, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0/GeV;

      foreach (const Particle& p, ufs.particles()) {
        const PdgId pid = abs(p.pdgId());

        switch (pid) {
        case 413:

          // Accept all D*+- decays. Normalisation to D0 + pi+- in finalize()

          // Scaled energy.
          const double energy = p.momentum().E()/GeV;
          const double scaledEnergy = energy/meanBeamMom;
          _h_Xe_Ds->fill(scaledEnergy, weight);

          break;
        }
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {

      // Scale to the product of branching fractions D0*->D0 pi  x  D0->Kpi(charged)
      // Numbers are taken from PDG 2010
      scale(_h_Xe_Ds, 0.677*0.0389/_sumWpassed);

    }

  private:
    double _sumWpassed;

  private:

    Histo1DPtr _h_Xe_Ds;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALEPH_1999_S4193598);

}

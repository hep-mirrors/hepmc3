// -*- C++ -*-
#include <iostream>
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/UnstableFinalState.hh"
#include "Rivet/ParticleName.hh"

namespace Rivet {


  /// @brief BABAR tau lepton to three charged hadrons
  /// @author Peter Richardson
  class BABAR_2007_S7266081 : public Analysis {
  public:

    BABAR_2007_S7266081()
      : Analysis("BABAR_2007_S7266081"),
        _weight_total(0),
        _weight_pipipi(0), _weight_Kpipi(0), _weight_KpiK(0), _weight_KKK(0)
    {   }


    void init() {
      addProjection(UnstableFinalState(), "UFS");
      _hist_pipipi_pipipi = bookHisto1D( 1, 1, 1);
      _hist_pipipi_pipi   = bookHisto1D( 2, 1, 1);
      _hist_Kpipi_Kpipi   = bookHisto1D( 3, 1, 1);
      _hist_Kpipi_Kpi     = bookHisto1D( 4, 1, 1);
      _hist_Kpipi_pipi    = bookHisto1D( 5, 1, 1);
      _hist_KpiK_KpiK     = bookHisto1D( 6, 1, 1);
      _hist_KpiK_KK       = bookHisto1D( 7, 1, 1);
      _hist_KpiK_piK      = bookHisto1D( 8, 1, 1);
      _hist_KKK_KKK       = bookHisto1D( 9, 1, 1);
      _hist_KKK_KK        = bookHisto1D(10, 1, 1);
    }


    void analyze(const Event& e) {
      // Find the taus
      Particles taus;
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(e, "UFS");
      foreach (const Particle& p, ufs.particles()) {
        if (abs(p.pdgId()) != PID::TAU) continue;
        _weight_total += 1.;
        Particles pip, pim, Kp, Km;
        unsigned int nstable = 0;
        // get the boost to the rest frame
        LorentzTransform cms_boost;
        if (p.momentum().vector3().mod() > 1*MeV)
          cms_boost = LorentzTransform(-p.momentum().boostVector());
        // find the decay products we want
        findDecayProducts(p.genParticle(), nstable, pip, pim, Kp, Km);
        if (p.pdgId() < 0) {
          swap(pip, pim);
          swap(Kp, Km );
        }
        if (nstable != 4) continue;
        // pipipi
        if (pim.size() == 2 && pip.size() == 1) {
          _weight_pipipi += 1.;
          _hist_pipipi_pipipi->
            fill((pip[0].momentum()+pim[0].momentum()+pim[1].momentum()).mass(),1.);
          _hist_pipipi_pipi->
            fill((pip[0].momentum()+pim[0].momentum()).mass(),1.);
          _hist_pipipi_pipi->
            fill((pip[0].momentum()+pim[1].momentum()).mass(),1.);
        }
        else if (pim.size() == 1 && pip.size() == 1 && Km.size() == 1) {
          _weight_Kpipi += 1.;
          _hist_Kpipi_Kpipi->
            fill((pim[0].momentum()+pip[0].momentum()+Km[0].momentum()).mass(),1.);
          _hist_Kpipi_Kpi->
            fill((pip[0].momentum()+Km[0].momentum()).mass(),1.);
          _hist_Kpipi_pipi->
            fill((pim[0].momentum()+pip[0].momentum()).mass(),1.);
        }
        else if (Kp.size() == 1 && Km.size() == 1 && pim.size() == 1) {
          _weight_KpiK += 1.;
          _hist_KpiK_KpiK->
            fill((Kp[0].momentum()+Km[0].momentum()+pim[0].momentum()).mass(),1.);
          _hist_KpiK_KK->
            fill((Kp[0].momentum()+Km[0].momentum()).mass(),1.);
          _hist_KpiK_piK->
            fill((Kp[0].momentum()+pim[0].momentum()).mass(),1.);
        }
        else if (Kp.size() == 1 && Km.size() == 2) {
          _weight_KKK += 1.;
          _hist_KKK_KKK->
            fill((Kp[0].momentum()+Km[0].momentum()+Km[1].momentum()).mass(),1.);
          _hist_KKK_KK->
            fill((Kp[0].momentum()+Km[0].momentum()).mass(),1.);
          _hist_KKK_KK->
            fill((Kp[0].momentum()+Km[1].momentum()).mass(),1.);
        }
      }
    }


    void finalize() {
      if (_weight_pipipi > 0.) {
        scale(_hist_pipipi_pipipi, 1.0/_weight_pipipi);
        scale(_hist_pipipi_pipi  , 0.5/_weight_pipipi);
      }
      if (_weight_Kpipi > 0.) {
        scale(_hist_Kpipi_Kpipi  , 1.0/_weight_Kpipi);
        scale(_hist_Kpipi_Kpi    , 1.0/_weight_Kpipi);
        scale(_hist_Kpipi_pipi   , 1.0/_weight_Kpipi);
      }
      if (_weight_KpiK > 0.) {
        scale(_hist_KpiK_KpiK    , 1.0/_weight_KpiK);
        scale(_hist_KpiK_KK      , 1.0/_weight_KpiK);
        scale(_hist_KpiK_piK     , 1.0/_weight_KpiK);
      }
      if (_weight_KKK > 0.) {
        scale(_hist_KKK_KKK      , 1.0/_weight_KKK);
        scale(_hist_KKK_KK       , 0.5/_weight_KKK);
      }
      /// @note Using autobooking for these scatters since their x values are not really obtainable from the MC data
      bookScatter2D(11, 1, 1, true)->point(0).setY(100*_weight_pipipi/_weight_total, 100*sqrt(_weight_pipipi)/_weight_total);
      bookScatter2D(12, 1, 1, true)->point(0).setY(100*_weight_Kpipi/_weight_total, 100*sqrt(_weight_Kpipi)/_weight_total);
      bookScatter2D(13, 1, 1, true)->point(0).setY(100*_weight_KpiK/_weight_total, 100*sqrt(_weight_KpiK)/_weight_total);
      bookScatter2D(14, 1, 1, true)->point(0).setY(100*_weight_KKK/_weight_total, 100*sqrt(_weight_KKK)/_weight_total);
    }


  private:

    //@{

    // Histograms
    Histo1DPtr _hist_pipipi_pipipi, _hist_pipipi_pipi;
    Histo1DPtr _hist_Kpipi_Kpipi, _hist_Kpipi_Kpi, _hist_Kpipi_pipi;
    Histo1DPtr _hist_KpiK_KpiK, _hist_KpiK_KK, _hist_KpiK_piK;
    Histo1DPtr _hist_KKK_KKK, _hist_KKK_KK;

    // Weights counters
    double _weight_total, _weight_pipipi, _weight_Kpipi, _weight_KpiK, _weight_KKK;

    //@}

    void findDecayProducts(const GenParticlePtr p,
                           unsigned int & nstable,
                           Particles& pip, Particles& pim,
                           Particles& Kp, Particles& Km) {
      const GenVertexPtr dv = p->end_vertex();
      /// @todo Use better looping
      for (GenVertex::particles_out_const_iterator pp = dv->particles_out_const_begin(); pp != dv->particles_out_const_end(); ++pp) {
        int id = (*pp)->pdg_id();
        if (id == PID::PI0 )
          ++nstable;
        else if (id == PID::K0S)
          ++nstable;
        else if (id == PID::PIPLUS) {
          pip.push_back(Particle(**pp));
          ++nstable;
        }
        else if (id == PID::PIMINUS) {
          pim.push_back(Particle(**pp));
          ++nstable;
        }
        else if (id == PID::KPLUS) {
          Kp.push_back(Particle(**pp));
          ++nstable;
        }
        else if (id == PID::KMINUS) {
          Km.push_back(Particle(**pp));
          ++nstable;
        }
        else if ((*pp)->end_vertex()) {
          findDecayProducts(*pp, nstable, pip, pim, Kp, Km);
        }
        else
          ++nstable;
      }
    }


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(BABAR_2007_S7266081);

}

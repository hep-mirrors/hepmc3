// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/ParticleName.hh"

namespace Rivet {


  /// Underlying event activity in the Drell-Yan process at 7 TeV
  class CMS_2012_I1107658 : public Analysis {
  public:

    /// Constructor
    CMS_2012_I1107658()
      : Analysis("CMS_2012_I1107658")
    {   }


    /// Initialization
    void init() {

      /// @note Using a bare muon Z (but with a clustering radius!?)
      ZFinder zfinder(FinalState(), -2.4, 2.4, 20*GeV, PID::MUON, 4*GeV, 140*GeV, 0.2, ZFinder::NOCLUSTER);
      addProjection(zfinder, "ZFinder");

      ChargedFinalState cfs(-2, 2, 500*MeV);
      VetoedFinalState nonmuons(cfs);
      nonmuons.addVetoPairId(PID::MUON);
      addProjection(nonmuons, "nonmuons");

      _h_Nchg_towards_pTmumu                 = bookProfile1D(1, 1, 1);
      _h_Nchg_transverse_pTmumu              = bookProfile1D(2, 1, 1);
      _h_Nchg_away_pTmumu                    = bookProfile1D(3, 1, 1);
      _h_pTsum_towards_pTmumu                = bookProfile1D(4, 1, 1);
      _h_pTsum_transverse_pTmumu             = bookProfile1D(5, 1, 1);
      _h_pTsum_away_pTmumu                   = bookProfile1D(6, 1, 1);
      _h_avgpT_towards_pTmumu                = bookProfile1D(7, 1, 1);
      _h_avgpT_transverse_pTmumu             = bookProfile1D(8, 1, 1);
      _h_avgpT_away_pTmumu                   = bookProfile1D(9, 1, 1);
      _h_Nchg_towards_plus_transverse_Mmumu  = bookProfile1D(10, 1, 1);
      _h_pTsum_towards_plus_transverse_Mmumu = bookProfile1D(11, 1, 1);
      _h_avgpT_towards_plus_transverse_Mmumu = bookProfile1D(12, 1, 1);
      _h_Nchg_towards_zmass_81_101           = bookHisto1D(13, 1, 1);
      _h_Nchg_transverse_zmass_81_101        = bookHisto1D(14, 1, 1);
      _h_Nchg_away_zmass_81_101              = bookHisto1D(15, 1, 1);
      _h_pT_towards_zmass_81_101             = bookHisto1D(16, 1, 1);
      _h_pT_transverse_zmass_81_101          = bookHisto1D(17, 1, 1);
      _h_pT_away_zmass_81_101                = bookHisto1D(18, 1, 1);
      _h_Nchg_transverse_zpt_5               = bookHisto1D(19, 1, 1);
      _h_pT_transverse_zpt_5                 = bookHisto1D(20, 1, 1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();
      const ZFinder& zfinder = applyProjection<ZFinder>(event, "ZFinder");

      if (zfinder.bosons().size() != 1) vetoEvent;

      double Zpt = zfinder.bosons()[0].pT()/GeV;
      double Zphi = zfinder.bosons()[0].momentum().phi();
      double Zmass = zfinder.bosons()[0].momentum().mass()/GeV;

      Particles particles = applyProjection<VetoedFinalState>(event, "nonmuons").particles();

      int nTowards = 0;
      int nTransverse = 0;
      int nAway = 0;
      double ptSumTowards = 0;
      double ptSumTransverse = 0;
      double ptSumAway = 0;

      foreach (const Particle& p, particles) {
        double dphi = fabs(deltaPhi(Zphi, p.momentum().phi()));
        double pT = p.pT();

        if ( dphi < M_PI/3 ) {
          nTowards++;
          ptSumTowards += pT;
          if (Zmass > 81. && Zmass < 101.) _h_pT_towards_zmass_81_101->fill(pT, weight);
        } else if ( dphi < 2.*M_PI/3 ) {
          nTransverse++;
          ptSumTransverse += pT;
          if (Zmass > 81. && Zmass < 101.) _h_pT_transverse_zmass_81_101->fill(pT, weight);
          if (Zpt < 5.) _h_pT_transverse_zpt_5->fill(pT, weight);
        } else {
          nAway++;
          ptSumAway += pT;
          if (Zmass > 81. && Zmass < 101.) _h_pT_away_zmass_81_101->fill(pT, weight);
        }

      } // Loop over particles


      const double area = 8./3.*M_PI;
      if (Zmass > 81. && Zmass < 101.) {
        _h_Nchg_towards_pTmumu->         fill(Zpt, 1./area * nTowards, weight);
        _h_Nchg_transverse_pTmumu->      fill(Zpt, 1./area * nTransverse, weight);
        _h_Nchg_away_pTmumu->            fill(Zpt, 1./area * nAway, weight);
        _h_pTsum_towards_pTmumu->        fill(Zpt, 1./area * ptSumTowards, weight);
        _h_pTsum_transverse_pTmumu->     fill(Zpt, 1./area * ptSumTransverse, weight);
        _h_pTsum_away_pTmumu->           fill(Zpt, 1./area * ptSumAway, weight);
        if (nTowards > 0)    _h_avgpT_towards_pTmumu->    fill(Zpt, ptSumTowards/nTowards, weight);
        if (nTransverse > 0) _h_avgpT_transverse_pTmumu-> fill(Zpt, ptSumTransverse/nTransverse, weight);
        if (nAway > 0)       _h_avgpT_away_pTmumu->       fill(Zpt, ptSumAway/nAway, weight);
        _h_Nchg_towards_zmass_81_101->   fill(nTowards, weight);
        _h_Nchg_transverse_zmass_81_101->fill(nTransverse, weight);
        _h_Nchg_away_zmass_81_101->      fill(nAway, weight);
      }

      if (Zpt < 5.) {
        _h_Nchg_towards_plus_transverse_Mmumu->fill(Zmass, (nTowards + nTransverse)/(2.*area), weight);
        _h_pTsum_towards_plus_transverse_Mmumu->fill(Zmass, (ptSumTowards + ptSumTransverse)/(2.*area), weight);
        if ((nTowards + nTransverse) > 0) _h_avgpT_towards_plus_transverse_Mmumu->fill(Zmass, (ptSumTowards + ptSumTransverse)/(nTowards + nTransverse), weight);
        _h_Nchg_transverse_zpt_5->fill(nTransverse, weight);
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      if (integral(_h_Nchg_towards_zmass_81_101)    > 0) scale(_h_pT_towards_zmass_81_101,    1/integral(_h_Nchg_towards_zmass_81_101));
      if (integral(_h_Nchg_transverse_zmass_81_101) > 0) scale(_h_pT_transverse_zmass_81_101, 1/integral(_h_Nchg_transverse_zmass_81_101));
      if (integral(_h_Nchg_away_zmass_81_101)       > 0) scale(_h_pT_away_zmass_81_101,       1/integral(_h_Nchg_away_zmass_81_101));
      if (integral(_h_Nchg_transverse_zpt_5)        > 0) scale(_h_pT_transverse_zpt_5,        1/integral(_h_Nchg_transverse_zpt_5));
      normalize(_h_Nchg_towards_zmass_81_101);
      normalize(_h_Nchg_transverse_zmass_81_101);
      normalize(_h_Nchg_away_zmass_81_101);
      normalize(_h_Nchg_transverse_zpt_5);
    }


  private:


    /// @name Histogram objects
    //@{
    Profile1DPtr _h_Nchg_towards_pTmumu;
    Profile1DPtr _h_Nchg_transverse_pTmumu;
    Profile1DPtr _h_Nchg_away_pTmumu;
    Profile1DPtr _h_pTsum_towards_pTmumu;
    Profile1DPtr _h_pTsum_transverse_pTmumu;
    Profile1DPtr _h_pTsum_away_pTmumu;
    Profile1DPtr _h_avgpT_towards_pTmumu;
    Profile1DPtr _h_avgpT_transverse_pTmumu;
    Profile1DPtr _h_avgpT_away_pTmumu;
    Profile1DPtr _h_Nchg_towards_plus_transverse_Mmumu;
    Profile1DPtr _h_pTsum_towards_plus_transverse_Mmumu;
    Profile1DPtr _h_avgpT_towards_plus_transverse_Mmumu;
    Histo1DPtr _h_Nchg_towards_zmass_81_101;
    Histo1DPtr _h_Nchg_transverse_zmass_81_101;
    Histo1DPtr _h_Nchg_away_zmass_81_101;
    Histo1DPtr _h_pT_towards_zmass_81_101;
    Histo1DPtr _h_pT_transverse_zmass_81_101;
    Histo1DPtr _h_pT_away_zmass_81_101;
    Histo1DPtr _h_Nchg_transverse_zpt_5;
    Histo1DPtr _h_pT_transverse_zpt_5;
    //@}

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2012_I1107658);

}

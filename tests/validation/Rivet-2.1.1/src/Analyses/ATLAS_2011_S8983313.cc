// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/RivetMT2.hh"

namespace Rivet {


  class ATLAS_2011_S8983313 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2011_S8983313()
      : Analysis("ATLAS_2011_S8983313")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // projection to find the electrons
      std::vector<std::pair<double, double> > eta_e;
      eta_e.push_back(make_pair(-2.47,2.47));
      IdentifiedFinalState elecs(eta_e, 10.0*GeV);
      elecs.acceptIdPair(PID::ELECTRON);
      addProjection(elecs, "elecs");



      // veto region electrons
      std::vector<std::pair<double, double> > eta_v_e;
      eta_v_e.push_back(make_pair(-1.52,-1.37));
      eta_v_e.push_back(make_pair( 1.37, 1.52));
      IdentifiedFinalState veto_elecs(eta_v_e, 10.0*GeV);
      veto_elecs.acceptIdPair(PID::ELECTRON);
      addProjection(veto_elecs, "veto_elecs");



      // projection to find the muons
      std::vector<std::pair<double, double> > eta_m;
      eta_m.push_back(make_pair(-2.4,2.4));
      IdentifiedFinalState muons(eta_m, 10.0*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");


      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);


      /// Jet finder
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4),
                    "AntiKtJets04");

      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0),"cfs");

      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");


      /// Book histograms
      _count_A = bookHisto1D("count_A", 1, 0., 1.);
      _count_B = bookHisto1D("count_B", 1, 0., 1.);
      _count_C = bookHisto1D("count_C", 1, 0., 1.);
      _count_D = bookHisto1D("count_D", 1, 0., 1.);

      _hist_meff_A  = bookHisto1D("m_eff_A", 30, 0., 3000.);
      _hist_mT2_B   = bookHisto1D("m_T2", 25, 0., 1000.);
      _hist_meff_CD = bookHisto1D("m_eff_C_D", 30, 0., 3000.);
      _hist_eTmiss  = bookHisto1D("Et_miss", 20, 0., 1000.);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Particles veto_e
        = applyProjection<IdentifiedFinalState>(event, "veto_elecs").particles();
      if ( ! veto_e.empty() ) {
        MSG_DEBUG("electrons in veto region");
        vetoEvent;
      }


      Jets cand_jets;
      foreach (const Jet& jet,
               applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 4.9 ) {
          cand_jets.push_back(jet);
        }
      }

      Particles cand_e  = applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt();


      Particles cand_mu;
      Particles chg_tracks = applyProjection<ChargedFinalState>(event, "cfs").particles();
      foreach ( const Particle & mu,
                applyProjection<IdentifiedFinalState>(event, "muons").particlesByPt() ) {
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) <= 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV )
          cand_mu.push_back(mu);
      }

      Jets cand_jets_2;
      foreach ( const Jet& jet, cand_jets ) {
        if ( fabs( jet.eta() ) >= 2.5 )
          cand_jets_2.push_back( jet );
        else {
          bool away_from_e = true;
          foreach ( const Particle & e, cand_e ) {
            if ( deltaR(e.momentum(),jet.momentum()) <= 0.2 ) {
              away_from_e = false;
              break;
            }
          }
          if ( away_from_e )
            cand_jets_2.push_back( jet );
        }
      }

      Particles recon_e, recon_mu;

      foreach ( const Particle & e, cand_e ) {
        bool away = true;
        foreach ( const Jet& jet, cand_jets_2 ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        if ( away )
          recon_e.push_back( e );
      }

      foreach ( const Particle & mu, cand_mu ) {
        bool away = true;
        foreach ( const Jet& jet, cand_jets_2 ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        if ( away )
          recon_mu.push_back( mu );
      }


      // pTmiss
      Particles vfs_particles = applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();


      // final jet filter
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets_2 ) {
        if ( fabs( jet.eta() ) <= 2.5 )
          recon_jets.push_back( jet );
      }


      // now only use recon_jets, recon_mu, recon_e

      if ( ! ( recon_mu.empty() && recon_e.empty() ) ) {
        MSG_DEBUG("Charged leptons left after selection");
        vetoEvent;
      }

      if ( eTmiss <= 100 * GeV ) {
        MSG_DEBUG("Not enough eTmiss: " << eTmiss << " < 100");
        vetoEvent;
      }


      if ( recon_jets.empty() || recon_jets[0].pT() <= 120.0 * GeV ) {
        MSG_DEBUG("No hard leading jet in " << recon_jets.size() << " jets");
        vetoEvent;
      }

      // ==================== observables ====================

      // Njets, min_dPhi

      int Njets = 0;
      double min_dPhi = 999.999;
      double pTmiss_phi = pTmiss.phi();
      foreach ( const Jet& jet, recon_jets ) {
        if ( jet.pT() > 40 * GeV ) {
          if ( Njets < 3 )
            min_dPhi = min( min_dPhi,
                            deltaPhi( pTmiss_phi, jet.momentum().phi() ) );
          ++Njets;
        }
      }

      if ( Njets < 2 ) {
        MSG_DEBUG("Only " << Njets << " >40 GeV jets left");
        vetoEvent;
      }

      if ( min_dPhi <= 0.4 ) {
        MSG_DEBUG("dPhi too small");
        vetoEvent;
      }

      // m_eff

      double m_eff_2j = eTmiss
        + recon_jets[0].pT()
        + recon_jets[1].pT();

      double m_eff_3j = recon_jets.size() < 3 ? -999.0 : m_eff_2j + recon_jets[2].pT();

      // etmiss / m_eff

      double et_meff_2j = eTmiss / m_eff_2j;
      double et_meff_3j = eTmiss / m_eff_3j;

      FourMomentum a = recon_jets[0].momentum();
      FourMomentum b = recon_jets[1].momentum();

      double m_T2 = mT2::mT2( a,
                              b,
                              pTmiss,
                              0.0 ); // zero mass invisibles


    // ==================== FILL ====================

      MSG_DEBUG( "Trying to fill "
                 << Njets << ' '
                 << m_eff_2j << ' '
                 << et_meff_2j << ' '
                 << m_eff_3j << ' '
                 << et_meff_3j << ' '
                 << m_T2 );

      _hist_eTmiss->fill(eTmiss, weight);

      // AAAAAAAAAA
      if ( et_meff_2j > 0.3 ) {
        _hist_meff_A->fill(m_eff_2j, weight);
        if ( m_eff_2j > 500 * GeV ) {
          MSG_DEBUG("Hits A");
          _count_A->fill(0.5, weight);
        }
      }

      // BBBBBBBBBB
      _hist_mT2_B->fill(m_T2, weight);
      if ( m_T2 > 300 * GeV ) {
        MSG_DEBUG("Hits B");
        _count_B->fill(0.5, weight);
      }

      // need 3 jets for C and D
      if ( Njets >= 3 && et_meff_3j > 0.25 ) {

        _hist_meff_CD->fill(m_eff_3j, weight);

        // CCCCCCCCCC
        if ( m_eff_3j > 500 * GeV ) {
          MSG_DEBUG("Hits C");
          _count_C->fill(0.5, weight);
        }

        // DDDDDDDDDD
        if ( m_eff_3j > 1000 * GeV ) {
          MSG_DEBUG("Hits D");
          _count_D->fill(0.5, weight);
        }
      }

    }

    //@}

    void finalize() {

      double norm = crossSection()/picobarn*35.0/sumOfWeights();
      scale(_hist_meff_A ,100.*norm);
      scale(_hist_mT2_B  ,100.*norm);
      scale(_hist_meff_CD, 40.*norm);
      scale(_hist_eTmiss , 50.*norm);
    }


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_A;
    Histo1DPtr _count_B;
    Histo1DPtr _count_C;
    Histo1DPtr _count_D;
    Histo1DPtr _hist_meff_A;
    Histo1DPtr _hist_mT2_B;
    Histo1DPtr _hist_meff_CD;
    Histo1DPtr _hist_eTmiss;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S8983313);

}

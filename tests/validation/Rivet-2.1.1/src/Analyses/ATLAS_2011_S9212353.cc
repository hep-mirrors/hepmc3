// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  class ATLAS_2011_S9212353 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2011_S9212353()
      : Analysis("ATLAS_2011_S9212353")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialize projections before the run
    void init() {

      // projection to find the electrons
      std::vector<std::pair<double, double> > eta_e;
      eta_e.push_back(make_pair(-2.47,2.47));
      IdentifiedFinalState elecs(eta_e, 20.0*GeV);
      elecs.acceptIdPair(PID::ELECTRON);
      addProjection(elecs, "elecs");


      // veto region electrons (from 2010 arXiv:1102.2357v2)
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


      // Jet finder
      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4),
                   "AntiKtJets04");


      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0,0.5*GeV),"cfs");


      // for pTmiss
      addProjection(VisibleFinalState(-4.5,4.5),"vfs");


      /// Book histograms
      _3jl_count_mu_channel = bookHisto1D("3jl_count_muon_channel", 1, 0., 1.);
      _3jl_count_e_channel = bookHisto1D("3jl_count_electron_channel", 1, 0., 1.);
      _3jt_count_mu_channel = bookHisto1D("3jt_count_muon_channel", 1, 0., 1.);
      _3jt_count_e_channel = bookHisto1D("3jt_count_electron_channel", 1, 0., 1.);
      _3j_hist_eTmiss_e = bookHisto1D("3j_Et_miss_e", 65, 0., 650.);
      _3j_hist_eTmiss_mu = bookHisto1D("3j_Et_miss_mu", 65, 0., 650.);
      _3j_hist_mT_e = bookHisto1D("3j_mT_e", 58, 0., 580.);
      _3j_hist_mT_mu = bookHisto1D("3j_mT_mu", 58, 0., 580.);
      _3j_hist_m_eff_e = bookHisto1D("3j_m_eff_e", 46, 0., 2300.);
      _3j_hist_m_eff_mu = bookHisto1D("3j_m_eff_mu", 46, 0., 2300.);
      _3jl_hist_m_eff_e_final = bookHisto1D("3jl_m_eff_e_final", 15, 0., 1500.);
      _3jl_hist_m_eff_mu_final = bookHisto1D("3jl_m_eff_mu_final", 15, 0., 1500.);
      _3jt_hist_m_eff_e_final = bookHisto1D("3jt_m_eff_e_final", 15, 0., 1500.);
      _3jt_hist_m_eff_mu_final = bookHisto1D("3jt_m_eff_mu_final", 15, 0., 1500.);


      _4jl_count_mu_channel = bookHisto1D("4jl_count_muon_channel", 1, 0., 1.);
      _4jl_count_e_channel = bookHisto1D("4jl_count_electron_channel", 1, 0., 1.);
      _4jt_count_mu_channel = bookHisto1D("4jt_count_muon_channel", 1, 0., 1.);
      _4jt_count_e_channel = bookHisto1D("4jt_count_electron_channel", 1, 0., 1.);
      _4j_hist_eTmiss_e = bookHisto1D("4j_Et_miss_e", 65, 0., 650.);
      _4j_hist_eTmiss_mu = bookHisto1D("4j_Et_miss_mu", 65, 0., 650.);
      _4j_hist_mT_e = bookHisto1D("4j_mT_e", 58, 0., 580.);
      _4j_hist_mT_mu = bookHisto1D("4j_mT_mu", 58, 0., 580.);
      _4j_hist_m_eff_e = bookHisto1D("4j_m_eff_e", 46, 0., 2300.);
      _4j_hist_m_eff_mu = bookHisto1D("4j_m_eff_mu", 46, 0., 2300.);
      _4jl_hist_m_eff_e_final = bookHisto1D("4jl_m_eff_e_final", 15, 0., 1500.);
      _4jl_hist_m_eff_mu_final = bookHisto1D("4jl_m_eff_mu_final", 15, 0., 1500.);
      _4jt_hist_m_eff_e_final = bookHisto1D("4jt_m_eff_e_final", 15, 0., 1500.);
      _4jt_hist_m_eff_mu_final = bookHisto1D("4jt_m_eff_mu_final", 15, 0., 1500.);


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
      foreach ( const Jet& jet,
          applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 2.8 ) {
          cand_jets.push_back(jet);
        }
      }

      Particles candtemp_e =
        applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt();
      Particles candtemp_mu =
        applyProjection<IdentifiedFinalState>(event,"muons").particlesByPt();
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();
      Particles cand_mu;
      Particles cand_e;


      // pTcone around muon track
      foreach ( const Particle & mu, candtemp_mu ) {
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV )
          cand_mu.push_back(mu);
      }

      // pTcone around electron
      foreach ( const Particle e, candtemp_e ) {
        double pTinCone = -e.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1 * e.pT() )
          cand_e.push_back(e);
      }

      // discard jets that overlap with electrons
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets ) {
          bool away_from_e = true;
          foreach ( const Particle & e, cand_e ) {
            if ( deltaR(e.momentum(),jet.momentum()) < 0.2 ) {
              away_from_e = false;
              break;
            }
          }
          if ( away_from_e )
            recon_jets.push_back( jet );
      }

      // only consider leptons far from jet
      Particles recon_e, recon_mu;
      foreach ( const Particle & e, cand_e ) {
        bool e_near_jet = false;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 &&
               deltaR(e.momentum(),jet.momentum()) > 0.2 )
            e_near_jet = true;
        }
        if ( ! e_near_jet )
          recon_e.push_back( e );
      }

      foreach ( const Particle & mu, cand_mu ) {
        bool mu_near_jet = false;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 )
            mu_near_jet = true;
        }
        if ( ! mu_near_jet )
          recon_mu.push_back( mu );
      }

      // pTmiss
      Particles vfs_particles
        = applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();


      // ==================== observables ====================


      // Njets
      int Njets = 0;
      double pTmiss_phi = pTmiss.phi();
      foreach ( const Jet& jet, recon_jets ) {
        if ( fabs(jet.eta()) < 2.8 )
          Njets+=1;
      }
      if ( Njets < 3 ) {
        MSG_DEBUG("Only " << Njets << " jets w/ eta<2.8 left");
        vetoEvent;
      }

      Particles lepton;
      if ( recon_mu.empty() && recon_e.empty() ) {
        MSG_DEBUG("No leptons");
        vetoEvent;
      }
      else {
        foreach ( const Particle & mu, recon_mu )
            lepton.push_back(mu);
        foreach ( const Particle & e, recon_e )
            lepton.push_back(e);
      }


      std::sort(lepton.begin(), lepton.end(), cmpMomByPt);

      double e_id = 11;
      double mu_id = 13;

      // one hard leading lepton cut
      if ( fabs(lepton[0].pdgId()) == e_id &&
           lepton[0].pT() <= 25*GeV ) {
        vetoEvent;
      }
      else if ( fabs(lepton[0].pdgId()) == mu_id &&
                lepton[0].pT() <= 20*GeV ) {
        vetoEvent;
      }

      // exactly one hard leading lepton cut
      if(lepton.size()>1) {
        if ( fabs(lepton[1].pdgId()) == e_id &&
             lepton[1].pT() > 20*GeV ) {
          vetoEvent;
        }
        else if ( fabs(lepton[1].pdgId()) == mu_id &&
                  lepton[1].pT() > 10*GeV ) {
          vetoEvent;
        }
      }

      // 3JL
      if ( recon_jets[0].pT() > 60.0*GeV &&
           recon_jets[1].pT() > 25.0*GeV &&
           recon_jets[2].pT() > 25.0*GeV &&
           deltaPhi( pTmiss_phi, recon_jets[0].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[1].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[2].momentum().phi() ) > 0.2 ) {

        FourMomentum pT_l = lepton[0].momentum();
        double dPhi = deltaPhi( pT_l.phi(), pTmiss_phi);
        double mT = sqrt( 2 * pT_l.pT() * eTmiss * (1 - cos(dPhi)) );
        double m_eff = eTmiss + pT_l.pT()
          + recon_jets[0].pT()
          + recon_jets[1].pT()
          + recon_jets[2].pT();

        if ( fabs( lepton[0].pdgId() ) == e_id ) {
          _3j_hist_mT_e->fill(mT, weight);
          _3j_hist_eTmiss_e->fill(eTmiss, weight);
          _3j_hist_m_eff_e->fill(m_eff, weight);
          if ( mT > 100*GeV && eTmiss > 125*GeV ) {
            _3jl_hist_m_eff_e_final->fill(m_eff, weight);
            if ( m_eff > 500*GeV && eTmiss > 0.25*m_eff ) {
              _3jl_count_e_channel->fill(0.5,weight);
            }
          }
        }

        else if ( fabs( lepton[0].pdgId() ) == mu_id ) {
          _3j_hist_mT_mu->fill(mT, weight);
          _3j_hist_eTmiss_mu->fill(eTmiss, weight);
          _3j_hist_m_eff_mu->fill(m_eff, weight);
          if ( mT > 100*GeV && eTmiss > 125*GeV ) {
            _3jl_hist_m_eff_mu_final->fill(m_eff, weight);
            if ( m_eff > 500*GeV && eTmiss > 0.25*m_eff ) {
              _3jl_count_mu_channel->fill(0.5,weight);
            }
          }
        }

      }

      // 3JT
      if ( recon_jets[0].pT() > 80.0*GeV &&
           recon_jets[1].pT() > 25.0*GeV &&
           recon_jets[2].pT() > 25.0*GeV &&
           deltaPhi( pTmiss_phi, recon_jets[0].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[1].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[2].momentum().phi() ) > 0.2 ) {

        FourMomentum pT_l = lepton[0].momentum();
        double dPhi = deltaPhi( pT_l.phi(), pTmiss_phi);
        double mT = sqrt( 2 * pT_l.pT() * eTmiss * (1 - cos(dPhi)) );
        double m_eff = eTmiss + pT_l.pT()
          + recon_jets[0].pT()
          + recon_jets[1].pT()
          + recon_jets[2].pT();


        if ( fabs( lepton[0].pdgId() ) == e_id ) {
          if ( mT > 100*GeV && eTmiss > 240*GeV ) {
            _3jt_hist_m_eff_e_final->fill(m_eff, weight);
            if ( m_eff > 600*GeV && eTmiss > 0.15*m_eff ) {
              _3jt_count_e_channel->fill(0.5,weight);
            }
          }
        }

        else if ( fabs( lepton[0].pdgId() ) == mu_id ) {
          if ( mT > 100*GeV && eTmiss > 240*GeV ) {
            _3jt_hist_m_eff_mu_final->fill(m_eff, weight);
            if ( m_eff > 600*GeV && eTmiss > 0.15*m_eff ) {
              _3jt_count_mu_channel->fill(0.5,weight);
            }
          }
        }

      }

      if ( Njets < 4 ) {
        MSG_DEBUG("Only " << Njets << " jets w/ eta<2.8 left");
        vetoEvent;
      }



      // 4JL
      if ( recon_jets[0].pT() > 60.0*GeV &&
           recon_jets[1].pT() > 25.0*GeV &&
           recon_jets[2].pT() > 25.0*GeV &&
           recon_jets[3].pT() > 25.0*GeV &&
           deltaPhi( pTmiss_phi, recon_jets[0].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[1].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[2].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[3].momentum().phi() ) > 0.2 ) {

        FourMomentum pT_l = lepton[0].momentum();
        double dPhi = deltaPhi( pT_l.phi(), pTmiss_phi);
        double mT = sqrt( 2 * pT_l.pT() * eTmiss * (1 - cos(dPhi)) );
        double m_eff = eTmiss + pT_l.pT()
          + recon_jets[0].pT()
          + recon_jets[1].pT()
          + recon_jets[2].pT()
          + recon_jets[3].pT();


        if ( fabs( lepton[0].pdgId() ) == e_id ) {
          _4j_hist_mT_e->fill(mT, weight);
          _4j_hist_eTmiss_e->fill(eTmiss, weight);
          _4j_hist_m_eff_e->fill(m_eff, weight);
          if ( mT > 100*GeV && eTmiss > 140*GeV ) {
            _4jl_hist_m_eff_e_final->fill(m_eff, weight);
            if ( m_eff > 300*GeV && eTmiss > 0.3*m_eff ) {
              _4jl_count_e_channel->fill(0.5,weight);
            }
          }
        }

        // Muon channel signal region
        else if ( fabs( lepton[0].pdgId() ) == mu_id ) {
          _4j_hist_mT_mu->fill(mT, weight);
          _4j_hist_eTmiss_mu->fill(eTmiss, weight);
          _4j_hist_m_eff_mu->fill(m_eff, weight);
          if ( mT > 100*GeV && eTmiss > 140*GeV ) {
            _4jl_hist_m_eff_mu_final->fill(m_eff, weight);
            if ( m_eff > 300*GeV && eTmiss > 0.3*m_eff ) {
              _4jl_count_mu_channel->fill(0.5,weight);
            }
          }
        }

      }

      // 4JT
      if ( recon_jets[0].pT() > 60.0*GeV &&
           recon_jets[1].pT() > 40.0*GeV &&
           recon_jets[2].pT() > 40.0*GeV &&
           recon_jets[3].pT() > 40.0*GeV &&
           deltaPhi( pTmiss_phi, recon_jets[0].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[1].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[2].momentum().phi() ) > 0.2 &&
           deltaPhi( pTmiss_phi, recon_jets[3].momentum().phi() ) > 0.2 ) {

        FourMomentum pT_l = lepton[0].momentum();

        double m_eff = eTmiss + pT_l.pT()
          + recon_jets[0].pT()
          + recon_jets[1].pT()
          + recon_jets[2].pT()
          + recon_jets[3].pT();


        if ( fabs( lepton[0].pdgId() ) == e_id ) {
          if ( eTmiss > 200*GeV ) {
            _4jt_hist_m_eff_e_final->fill(m_eff, weight);
            if ( m_eff > 500*GeV && eTmiss > 0.15*m_eff ) {
              _4jt_count_e_channel->fill(0.5,weight);
            }
          }
        }

        // Muon channel signal region
        else if ( fabs( lepton[0].pdgId() ) == mu_id ) {
          if ( eTmiss > 200*GeV ) {
            _4jt_hist_m_eff_mu_final->fill(m_eff, weight);
            if ( m_eff > 500*GeV && eTmiss > 0.15*m_eff ) {
              _4jt_count_mu_channel->fill(0.5,weight);
            }
          }
        }

      }
     }

    //@}


    void finalize() {

      scale( _3j_hist_eTmiss_e, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3j_hist_eTmiss_mu, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3j_hist_m_eff_e, 50. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3j_hist_m_eff_mu, 50. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3j_hist_mT_e, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3j_hist_mT_mu, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3jl_hist_m_eff_e_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3jl_hist_m_eff_mu_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3jt_hist_m_eff_e_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _3jt_hist_m_eff_mu_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );

      scale( _4j_hist_eTmiss_e, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4j_hist_eTmiss_mu, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4j_hist_m_eff_e, 50. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4j_hist_m_eff_mu, 50. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4j_hist_mT_e, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4j_hist_mT_mu, 10. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4jl_hist_m_eff_e_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4jl_hist_m_eff_mu_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4jt_hist_m_eff_e_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );
      scale( _4jt_hist_m_eff_mu_final, 100. * 1.04e3 * crossSection()/sumOfWeights() );


    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _3jl_count_e_channel;
    Histo1DPtr _3jl_count_mu_channel;
    Histo1DPtr _3jt_count_e_channel;
    Histo1DPtr _3jt_count_mu_channel;
    Histo1DPtr _3j_hist_eTmiss_e;
    Histo1DPtr _3j_hist_eTmiss_mu;
    Histo1DPtr _3j_hist_m_eff_e;
    Histo1DPtr _3j_hist_m_eff_mu;
    Histo1DPtr _3j_hist_mT_e;
    Histo1DPtr _3j_hist_mT_mu;
    Histo1DPtr _3jl_hist_m_eff_e_final;
    Histo1DPtr _3jl_hist_m_eff_mu_final;
    Histo1DPtr _3jt_hist_m_eff_e_final;
    Histo1DPtr _3jt_hist_m_eff_mu_final;



    Histo1DPtr _4jl_count_e_channel;
    Histo1DPtr _4jl_count_mu_channel;
    Histo1DPtr _4jt_count_e_channel;
    Histo1DPtr _4jt_count_mu_channel;
    Histo1DPtr _4j_hist_eTmiss_e;
    Histo1DPtr _4j_hist_eTmiss_mu;
    Histo1DPtr _4j_hist_m_eff_e;
    Histo1DPtr _4j_hist_m_eff_mu;
    Histo1DPtr _4j_hist_mT_e;
    Histo1DPtr _4j_hist_mT_mu;
    Histo1DPtr _4jl_hist_m_eff_e_final;
    Histo1DPtr _4jl_hist_m_eff_mu_final;
    Histo1DPtr _4jt_hist_m_eff_e_final;
    Histo1DPtr _4jt_hist_m_eff_mu_final;


    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9212353);

}

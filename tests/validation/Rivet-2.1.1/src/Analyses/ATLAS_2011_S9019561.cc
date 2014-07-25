// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  class ATLAS_2011_S9019561 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2011_S9019561()
      : Analysis("ATLAS_2011_S9019561")
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
      IdentifiedFinalState elecs(eta_e, 20.0*GeV);
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
      IdentifiedFinalState muons(eta_m, 20.0*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");


      // jet finder
      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4),
                   "AntiKtJets04");


      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0,0.5*GeV),"cfs");


      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");


      /// book histograms
      _count_OS_e_mu = bookHisto1D("count_OS_e+-mu-+", 1, 0., 1.);
      _count_OS_e_e = bookHisto1D("count_OS_e+e-", 1, 0., 1.);
      _count_OS_mu_mu = bookHisto1D("count_OS_mu+mu-", 1, 0., 1.);
      _count_SS_e_mu = bookHisto1D("count_SS_e+-mu+-", 1, 0., 1.);
      _count_SS_e_e = bookHisto1D("count_SS_e+-e+-", 1, 0., 1.);
      _count_SS_mu_mu = bookHisto1D("count_SS_mu+-mu+-", 1, 0., 1.);

      _hist_eTmiss_OS  = bookHisto1D("Et_miss_OS", 20, 0., 400.);
      _hist_eTmiss_SS  = bookHisto1D("Et_miss_SS", 20, 0., 400.);

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
        if ( fabs( jet.eta() ) < 2.5 ) {
          cand_jets.push_back(jet);
        }
      }

      Particles cand_e =
        applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt();

      // charged particle for isolation
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();

      // apply muon isolation
      Particles cand_mu;
      // pTcone around muon track
      foreach ( const Particle & mu,
                applyProjection<IdentifiedFinalState>(event,"muons").particlesByPt() ) {
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV )
          cand_mu.push_back(mu);
      }

      // Discard jets that overlap with electrons
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets ) {
        bool away_from_e = true;
        foreach ( const Particle & e, cand_e ) {
          if ( deltaR(e.momentum(),jet.momentum()) <= 0.2 ) {
            away_from_e = false;
            break;
          }
        }
        if ( away_from_e )
         recon_jets.push_back( jet );
      }

      // Leptons far from jet
      Particles recon_e;
      foreach ( const Particle & e, cand_e ) {
        bool e_near_jet = false;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 ) {
            e_near_jet = true;
            break;
          }
        }
        // Electron isolation criterion
        if ( ! e_near_jet ) {
          double EtinCone = -e.momentum().Et();
          foreach ( const Particle & track, chg_tracks) {
            if ( deltaR(e.momentum(),track.momentum()) <= 0.2 )
              EtinCone += track.momentum().Et();
          }
          if ( EtinCone/e.pT() <= 0.15 )
            recon_e.push_back( e );
        }
      }

      Particles recon_mu;
      foreach ( const Particle & mu, cand_mu ) {
         bool mu_near_jet = false;
         foreach ( const Jet& jet, recon_jets ) {
           if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
             mu_near_jet = true;
             break;
           }
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

      // Exactly two leptons for each event
      if ( recon_mu.size() + recon_e.size() != 2)
        vetoEvent;

      // Lepton pair mass
      FourMomentum p_leptons;
      foreach ( Particle e, recon_e ) {
        p_leptons +=  e.momentum();
      }
      foreach ( Particle mu, recon_mu) {
        p_leptons += mu.momentum();
      }

      if ( p_leptons.mass() <= 5.0 * GeV)
        vetoEvent;

      // ==================== FILL ====================


      // electron, electron
      if (recon_e.size() == 2 ) {

        // SS ee
        if ( recon_e[0].pdgId() * recon_e[1].pdgId() > 0 ) {
          _hist_eTmiss_SS->fill(eTmiss, weight);
          if ( eTmiss > 100 ) {
            MSG_DEBUG("Hits SS e+/-e+/-");
            _count_SS_e_e->fill(0.5, weight);
          }
        }

        // OS ee
        else if ( recon_e[0].pdgId() * recon_e[1].pdgId() < 0) {
          _hist_eTmiss_OS->fill(eTmiss, weight);
          if ( eTmiss > 150 ) {
            MSG_DEBUG("Hits OS e+e-");
            _count_OS_e_e->fill(0.5, weight);
          }
        }
      }


      // muon, electron
      else if ( recon_e.size() == 1 ) {

        // SS mu_e
        if ( recon_e[0].pdgId() * recon_mu[0].pdgId() > 0 ) {
          _hist_eTmiss_SS->fill(eTmiss, weight);
          if ( eTmiss > 100 ) {
            MSG_DEBUG("Hits SS e+/-mu+/-");
            _count_SS_e_mu->fill(0.5, weight);
          }
        }

        // OS mu_e
        else if ( recon_e[0].pdgId() * recon_mu[0].pdgId() < 0) {
          _hist_eTmiss_OS->fill(eTmiss, weight);
          if ( eTmiss > 150 ) {
            MSG_DEBUG("Hits OS e+mu-");
            _count_OS_e_mu->fill(0.5, weight);
          }
        }
      }


      // muon, muon
      else if ( recon_mu.size() == 2 ) {

        // SS mu_mu
        if ( recon_mu[0].pdgId() * recon_mu[1].pdgId() > 0 ) {
          _hist_eTmiss_SS->fill(eTmiss, weight);
          if ( eTmiss > 100 ) {
            MSG_DEBUG("Hits SS mu+/-mu+/-");
            _count_SS_mu_mu->fill(0.5, weight);
          }
        }

        // OS mu_mu
        else if ( recon_mu[0].pdgId() * recon_mu[1].pdgId() < 0) {
          _hist_eTmiss_OS->fill(eTmiss, weight);
          if ( eTmiss > 150 ) {
            MSG_DEBUG("Hits OS mu+mu-");
            _count_OS_mu_mu->fill(0.5, weight);
          }
        }
      }


    }

    //@}


    void finalize() {
      double norm = crossSection()/picobarn*35./sumOfWeights();
      // event counts
      scale(_count_OS_e_mu ,norm);
      scale(_count_OS_e_e  ,norm);
      scale(_count_OS_mu_mu,norm);
      scale(_count_SS_e_mu ,norm);
      scale(_count_SS_e_e  ,norm);
      scale(_count_SS_mu_mu,norm);
      scale(_hist_eTmiss_OS,10.*norm);
      scale(_hist_eTmiss_SS,10.*norm);
    }


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_OS_e_mu;
    Histo1DPtr _count_OS_e_e;
    Histo1DPtr _count_OS_mu_mu;
    Histo1DPtr _count_SS_e_mu;
    Histo1DPtr _count_SS_e_e;
    Histo1DPtr _count_SS_mu_mu;
    Histo1DPtr _hist_eTmiss_OS;
    Histo1DPtr _hist_eTmiss_SS;

    //@}


  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9019561);

}

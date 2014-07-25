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


  /// 1-lepton and 2-lepton search for first or second generation leptoquarks
  /// @todo Clean up the debug stuff
  class ATLAS_2011_S9041966 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2011_S9041966()
      : Analysis("ATLAS_2011_S9041966"),
        // DEBUG
        count(0), vetoe(0), Njetscut(0), dilept(0),
        candmumujj(0), candeejj(0), onelept(0),
        eTmisscut(0), candmvjj(0), candevjj(0),
        mumujj(0), eejj(0),
        mTonelept(0), MLQonelept(0), MtLQonelept(0), Stvonelept(0),
        mTev(0), MLQev(0), MtLQev(0), Stvev(0),
        muvjj(0), evjj(0), emuvjj(0),
        cande(0), candmu(0),
        tmpe(0), tmpmu(0),
        mumuZCR(0), eeZCR(0),
        munuW2CR(0), munuttCR(0),
        enuW2CR(0), enuttCR(0)
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


      // veto region electrons
      std::vector<std::pair<double, double> > eta_v_e;
      eta_v_e.push_back(make_pair(-1.52,-1.35));
      eta_v_e.push_back(make_pair( 1.35, 1.52));
      IdentifiedFinalState veto_elecs(eta_v_e, 10.0*GeV);
      veto_elecs.acceptIdPair(PID::ELECTRON);
      addProjection(veto_elecs, "veto_elecs");

///DEBUG
      // projection to find all leptons
      IdentifiedFinalState all_mu_e;
      all_mu_e.acceptIdPair(PID::MUON);
      all_mu_e.acceptIdPair(PID::ELECTRON);
      addProjection(all_mu_e, "all_mu_e"); //debug



      // projection to find the muons
      std::vector<std::pair<double, double> > eta_m;
      eta_m.push_back(make_pair(-2.4,2.4));
      IdentifiedFinalState muons(eta_m, 20.0*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");


      // Jet finder
      VetoedFinalState vfs;
      vfs.addVetoPairDetail(PID::MUON,20*GeV,7000*GeV);
      vfs.addVetoPairDetail(PID::ELECTRON,20*GeV,7000*GeV);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4),
                   "AntiKtJets04");


      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0,0.5*GeV),"cfs");


      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");


      /// Book histograms
      _count_mumujj = bookHisto1D("count_2muons_dijet", 1, 0., 1.);
      _count_eejj   = bookHisto1D("count_2elecs_dijet", 1, 0., 1.);
      _count_muvjj  = bookHisto1D("count_muon_neutrino_dijet", 1, 0., 1.);
      _count_evjj   = bookHisto1D("count_elec_neutrino_dijet", 1, 0., 1.);

      _hist_St_mumu = bookHisto1D("hist_mumujj_St", 10, 450., 1650.);
      _hist_St_ee   = bookHisto1D("hist_eejj_St", 10, 450., 1650.);
      _hist_MLQ_muv = bookHisto1D("hist_munujj_MLQ", 9, 150., 600.);
      _hist_MLQ_ev  = bookHisto1D("hist_enujj_MLQ", 9, 150., 600.);

      _hist_St_mumu_ZCR   = bookHisto1D("CR_Zjets_St_mumu", 40, 0., 800.);
      _hist_St_ee_ZCR     = bookHisto1D("CR_Zjets_Stee", 40, 0., 800.);
      _hist_MLQ_munu_W2CR = bookHisto1D("CR_W2jets_MLQ_munu", 20, 0., 400.);
      _hist_MLQ_enu_W2CR  = bookHisto1D("CR_W2jets_MLQ_enu", 20, 0., 400.);
      _hist_MLQ_munu_ttCR = bookHisto1D("CR_tt_MLQ_munu", 35, 0., 700.);
      _hist_MLQ_enu_ttCR  = bookHisto1D("CR_tt_MLQ_enu", 35, 0., 700.);

    }



    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

///DEBUG
      count +=1; //cerr<< "Event " << count << '\n';
 // debug


      Particles veto_e
        = applyProjection<IdentifiedFinalState>(event, "veto_elecs").particles();
      if ( ! veto_e.empty() ) {
        MSG_DEBUG("electrons in veto region");
        vetoEvent;
      }
++vetoe;


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
      Particles cand_mu;
      Particles cand_e;
      Particles vfs_particles
        = applyProjection<VisibleFinalState>(event, "vfs").particles();


      // pTcone around muon track
      foreach ( const Particle & mu, candtemp_mu ) {
++tmpmu;
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, vfs_particles ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone/mu.pT() < 0.25 )
++candmu;
          cand_mu.push_back(mu);
      }

      // pTcone around electron
      foreach ( const Particle e, candtemp_e ) {
++tmpe;
        double pTinCone = -e.pT();
        foreach ( const Particle & track, vfs_particles ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone/e.pT() < 0.2 )
++cande;
          cand_e.push_back(e);
      }

      if ( cand_e.empty() && cand_mu.empty() ) {
        //cerr<<" ->Event vetoed. No candidate lept"<<'\n';
        vetoEvent;
      }


//DEBUG
// else{
// foreach (const Particle & mu,  cand_mu) {
//   cerr << "cand mu: " << "Id " << mu.pdgId() << "      eta " << mu.eta() << "      pT " << mu.pT() << '\n';
// }
// foreach (const Particle & lepton,  cand_e) {
//   cerr << "cand e: " << "Id " << lepton.pdgId() << "      eta " << lepton.eta() << "      pT " << lepton.pT() << '\n';
// }} // debug



     // pTmiss
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();


      // discard jets that overlap with leptons
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets ) {
          bool away_from_lept = true;
          foreach ( const Particle e, cand_e ) {
            if ( deltaR(e.momentum(),jet.momentum()) <= 0.5 ) {
              away_from_lept = false;
              break;
            }
          }
          foreach ( const Particle & mu, cand_mu ) {
            if ( deltaR(mu.momentum(),jet.momentum()) <= 0.5 ) {
              away_from_lept = false;
              break;
            }
          }
          if ( away_from_lept )
            recon_jets.push_back( jet );
      }



//DEBUG
// cerr << " Num of recon jets: " << recon_jets.size() << '\n';
// cerr << " Num of cand e: " << cand_e.size() << '\n';
// cerr << " Num of cand mu: " << cand_mu.size() << '\n';
//debug



      // ================ OBSERVABLES ================


      // At least 2 hard jets
      if ( recon_jets.size() < 2 ) {
        //cerr << " ->Event vetoed. Not enough hard jets." << '\n';
        vetoEvent;
      }
++Njetscut;


      // Initialize variables for observables
      double M_ll=0., M_LQ=0., St_ll=0., Mt_LQ=0., St_v=0., mT=0.;
      FourMomentum p_l, p_l1, p_l2, p_j[2];
      p_j[0] = recon_jets[0].momentum();
      p_j[1] = recon_jets[1].momentum();

      Particles dilept_pair;
      bool single_lept = false;

      if ( cand_mu.size() == 2 && cand_e.empty() ) {
++candmumujj;
        foreach ( const Particle& mu, cand_mu )
          dilept_pair.push_back(mu);
      }
      else if ( cand_e.size() == 2 && cand_mu.empty() ) {
++candeejj;
          foreach ( const Particle& e, cand_e )
            dilept_pair.push_back(e);
      }
      else if ( cand_mu.size() == 1 && cand_e.empty() ) {
++candmvjj;
        p_l = cand_mu[0].momentum();
        single_lept = true;
      }
      else if ( cand_e.size() == 1 && cand_mu.empty() ) {
++candevjj;
        p_l = cand_e[0].momentum();
        single_lept = true;
      }

      // Dilepton channel observables
      if ( ! dilept_pair.empty() ) {

        double E_l1, E_l2, E_j1, E_j2;
        double tmpM_LQ1[2], tmpM_LQ2[2], M_LQDiff1, M_LQDiff2;

        p_l1 = dilept_pair[0].momentum();
        p_l2 = dilept_pair[1].momentum();
        E_l1 = p_l1.E();
        E_l2 = p_l2.E();

        E_j1 = p_j[0].E();
        E_j2 = p_j[1].E();

        // Calculate possible leptoquark mass M_LQ and reconstruct average M_LQ

        tmpM_LQ1[0] = E_l1 + E_j1;
        tmpM_LQ1[1] = E_l2 + E_j2;
        M_LQDiff1 = abs( tmpM_LQ1[0] - tmpM_LQ1[1] );

        tmpM_LQ2[0] = E_l1 + E_j2;
        tmpM_LQ2[1] = E_l2 + E_j1;
        M_LQDiff2 = abs( tmpM_LQ2[0] - tmpM_LQ2[1] );

        if ( M_LQDiff1 > M_LQDiff2 )
          M_LQ = ( tmpM_LQ2[0] + tmpM_LQ2[1] ) / 2;
        else
          M_LQ = ( tmpM_LQ1[0] + tmpM_LQ1[1] ) / 2;

        // Calculate event transverse energy St
        St_ll = p_l1.pT() + p_l2.pT() + p_j[0].pT() + p_j[1].pT();

        // Dilept pair invariant mass M_ll
        M_ll = E_l1 + E_l2;

      }

      // 1-lepton channel observables
      else if ( single_lept ) {

        double tmpM_LQ[2], tmpMt_LQ[2], dPhi_j[2], M_LQDiff1, M_LQDiff2;

        // List of possible M_LQ, Mt_LQ pairings

        for ( int i = 0; i < 2; ++i ) {
          tmpM_LQ[i] = p_l.E() + p_j[i].E();
          dPhi_j[1-i] = deltaPhi( p_j[1-i].phi(), pTmiss.phi() );
          tmpMt_LQ[i] = sqrt( 2 * p_j[1-i].pT() * eTmiss * (1 - cos( dPhi_j[1-i] )) );
        }

        // Choose pairing that gives smallest absolute difference

        M_LQDiff1 = abs( tmpM_LQ[0] - tmpMt_LQ[0] );
        M_LQDiff2 = abs( tmpM_LQ[1] - tmpMt_LQ[1] );

        if ( M_LQDiff1 > M_LQDiff2 ) {
          M_LQ = tmpM_LQ[1];
          Mt_LQ = tmpMt_LQ[1];
        }
        else {
          M_LQ = tmpM_LQ[0];
          Mt_LQ = tmpMt_LQ[0];
        }

        // Event transverse energy
        St_v = p_l.pT() + eTmiss + p_j[0].pT() + p_j[1].pT();

        // Transverse mass mT
        double dPhi_l = deltaPhi( p_l.phi(), pTmiss.phi());
        mT = sqrt( 2 * p_l.pT() * eTmiss * (1 - cos(dPhi_l)) );

      }


    // ============== CONTROL REGIONS ===============

      // mumujj, Z control region
      if ( cand_mu.size() == 2 ) {
        if ( M_ll >= 81*GeV && M_ll <= 101*GeV ) {
++mumuZCR;
          _hist_St_mumu_ZCR->fill(St_ll, weight);
        }
      }
      // eejj, Z control region
      else if ( cand_e.size() == 2 ) {
        if ( M_ll >= 81*GeV && M_ll <= 101*GeV ) {
++eeZCR;
          _hist_St_ee_ZCR->fill(St_ll, weight);

        }
      }

      if ( cand_mu.size() == 1 ) {
        // munujj, W+2jets control region
        if ( recon_jets.size() == 2 &&
             mT >= 40*GeV && mT <= 150*GeV ) {
++munuW2CR;
          _hist_MLQ_munu_W2CR->fill(M_LQ, weight);
        }
        // munujj, tt control region
        if ( recon_jets.size() >= 4 &&
             recon_jets[0].pT() > 50*GeV && recon_jets[1].pT() > 40*GeV && recon_jets[2].pT() > 30*GeV ) {
++munuttCR;
          _hist_MLQ_munu_ttCR->fill(M_LQ, weight);
        }
      }
      if ( cand_e.size() == 1 ) {
        // enujj, W+2jets control region
        if ( recon_jets.size() == 2 &&
             mT >= 40*GeV && mT <= 150*GeV ) {
++enuW2CR;
          _hist_MLQ_enu_W2CR->fill(M_LQ, weight);
        }
        // enujj, tt control region
        if ( recon_jets.size() >= 4 &&
             recon_jets[0].pT() > 50*GeV && recon_jets[1].pT() > 40*GeV && recon_jets[2].pT() > 30*GeV ) {
++enuttCR;
          _hist_MLQ_enu_ttCR->fill(M_LQ, weight);
        }
      }




    // ========= PRESELECTION =======================



      // Single lepton channel cuts
      if ( single_lept ) {

        if ( eTmiss <= 25*GeV ) {
          //cerr << " ->Event vetoed. eTmiss=" << eTmiss << '\n';
          vetoEvent;
        }
++eTmisscut;

        if ( mT <= 40*GeV )
          vetoEvent;

//++mTcut;

        // enujj channel
        if ( cand_e.size() == 1 && cand_mu.empty() ) {

          // Triangle cut
          double dPhi_jet1 = deltaPhi( recon_jets[0].phi(), pTmiss.phi() );
          double dPhi_jet2 = deltaPhi( recon_jets[1].phi(), pTmiss.phi() );

          if ( dPhi_jet1 <= 1.5 * (1 - eTmiss/45) ||
               dPhi_jet2 <= 1.5 * (1 - eTmiss/45) ) {
++emuvjj;
            vetoEvent;
          }
       }
     }

    // ==================== FILL ====================


      // mumujj channel
      if ( cand_mu.size() == 2 ) {
        if ( M_ll <= 120*GeV ||
                M_LQ <= 150*GeV ||
                p_l1.pT() <= 30*GeV || p_l2.pT() <= 30*GeV ||
                p_j[0].pT() <= 30*GeV || p_j[1].pT() <= 30*GeV ||
                St_ll <= 450*GeV ) {
          //cerr<<" ->Dilept event vetoed. Table 4 cuts." << '\n';
          vetoEvent;
        }
        else {


++mumujj;
// cerr<< " ->MUMUJJ event selected." << '\n';
            _hist_St_mumu->fill(St_ll, weight);
            _count_mumujj->fill(0.5, weight);

        }
      }
      // eejj channel
      else if ( cand_e.size() == 2 ) {
        if ( M_ll <= 120*GeV ||
                M_LQ <= 150*GeV ||
                p_l1.pT() <= 30*GeV || p_l2.pT() <= 30*GeV ||
                p_j[0].pT() <= 30*GeV || p_j[1].pT() <= 30*GeV ||
                St_ll <= 450*GeV ) {
          //cerr<<" ->Dilept event vetoed. Table 4 cuts." << '\n';
          vetoEvent;
        }
        else {

++eejj;
//cerr<< " ->EEJJ event selected." << '\n';
            _hist_St_ee->fill(St_ll, weight);
            _count_eejj->fill(0.5, weight);

        }
      }


      // muvjj channel
      else if ( cand_mu.size() == 1 ) {



        if (M_LQ<=150*GeV) {
//cerr<<" ->muvjj event vetoed. Not enough M_LQ: " << M_LQ<< '\n';
          vetoEvent;
        }
++MLQonelept;
        if (Mt_LQ<=150*GeV) {
//cerr<<" ->muvjj event vetoed. Not enough Mt_LQ: " << Mt_LQ<< '\n';
          vetoEvent;
        }
++MtLQonelept;
        if (St_v<=400*GeV) {
//cerr<<" ->muvjj event vetoed. Not enough St_v: " << St_v<< '\n';
          vetoEvent;
        }
++Stvonelept;
        if (mT<=160*GeV) {
//cerr<<" ->muvjj event vetoed. Not enough mT: " << mT<<'\n';
          vetoEvent;
        }
++mTonelept;
        //else {
++muvjj;
//cerr<< " ->MUVJJ event selected." << '\n';
            _hist_MLQ_muv->fill(M_LQ, weight);
            _count_muvjj->fill(0.5, weight);

        //}
      }

      // evjj channel
      else if ( cand_e.size() == 1 ) {

if (M_LQ<=180*GeV) {
//cerr<<" ->evjj event vetoed. Not enough M_LQ: " << M_LQ<< '\n';
          vetoEvent;
        }
++MLQev;
        if (Mt_LQ<=180*GeV) {
//cerr<<" ->evjj event vetoed. Not enough Mt_LQ: " << Mt_LQ<< '\n';
          vetoEvent;
        }
++MtLQev;
        if (St_v<=410*GeV) {
//cerr<<" ->evjj event vetoed. Not enough St_v: " << St_v<< '\n';
          vetoEvent;
        }
++Stvev;
if (mT<=200*GeV) {
//cerr<<" ->evjj event vetoed. Not enough mT: " << mT<<'\n';
          vetoEvent;
        }
++mTev;
        //else {
++evjj;
//cerr<< " ->EVJJ event selected." << '\n';
_hist_MLQ_ev->fill(M_LQ, weight);
            _count_evjj->fill(0.5, weight);



//      if ( mT <= 200*GeV ||
//              M_LQ <= 180*GeV ||
//              Mt_LQ <= 180*GeV ||
//              St_v <= 410*GeV ) {
// cerr<<" ->evjj event vetoed. Doesn't pass table 4 cuts." << '\n';
//        vetoEvent;
//      }
//      else {
// ++evjj;
// cerr<< " ->EVJJ event selected." << '\n';
// _hist_MLQ_ev->fill(M_LQ, weight);
//          _count_evjj->fill(0.5, weight);

//      }


      }


    }

    //@}


    void finalize() {
// cerr << '\n' << "Of " << count << " events, saw "
// << vetoe << " (after veto region cut), "
// << Njetscut << " (after 2jet req). "
// << '\n'
// << "For " << dilept << " dilept events: "
// << candmumujj << " cand mumujj events, "
// << candeejj << " cand eejj events."
// << '\n'
// << "For " << onelept << " onelept events: "
// << candmvjj << " preselected mvjj events, "
// << candevjj << " preselected evjj events; "
// << eTmisscut << " (eTmiss req); "
// << emuvjj << " leftover; "
// << MLQonelept << " (muvjj M_LQ cut), "
// << MtLQonelept << " (muvjj Mt_LQ cut), "
// << Stvonelept << " (muvjj St_v cut), "
// << mTonelept << " (muvjj mT cut); "
// << MLQev << " (evjj M_LQ cut), "
// << MtLQev << " (evjj Mt_LQ cut), "
// << Stvev << " (evjj St_v cut), "
// << mTev << " (evjj mT cut). "
// << '\n'<<'\n'
// ;

// cerr << "CR - " << "mumu Z: " << mumuZCR << "  ee Z: " << eeZCR << "  munu W+2jets: " << munuW2CR << "  munu tt: " << munuttCR << "  enu W+2jets: " << enuW2CR << "  enu tt: " << enuttCR << '\n';

// cerr << "mumujj: " << mumujj << "      eejj: " << eejj << "      muvjj: " << muvjj << "      evjj: " << evjj << '\n';


        scale( _hist_St_ee, 120. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_St_mumu, 120. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_muv, 50. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_ev, 50. * 35. * crossSection()/sumOfWeights() );



        scale( _hist_St_mumu_ZCR, 20. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_St_ee_ZCR, 20. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_munu_W2CR, 20. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_enu_W2CR, 20. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_munu_ttCR, 20. * 35. * crossSection()/sumOfWeights() );
        scale( _hist_MLQ_enu_ttCR, 20. * 35. * crossSection()/sumOfWeights() );

/*
scale( _hist_eTmiss_mu, binwidth*luminosity* crossSection()/sumOfWeights() );
*/

    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_mumujj;
    Histo1DPtr _count_eejj;
    Histo1DPtr _count_muvjj;
    Histo1DPtr _count_evjj;

    Histo1DPtr _hist_St_mumu;
    Histo1DPtr _hist_St_ee;
    Histo1DPtr _hist_MLQ_muv;
    Histo1DPtr _hist_MLQ_ev;

    Histo1DPtr _hist_St_mumu_ZCR;
    Histo1DPtr _hist_St_ee_ZCR;
    Histo1DPtr _hist_MLQ_munu_W2CR;
    Histo1DPtr _hist_MLQ_enu_W2CR;
    Histo1DPtr _hist_MLQ_munu_ttCR;
    Histo1DPtr _hist_MLQ_enu_ttCR;




    //@}


    // DEBUG VARIABLES
    int count;
    int vetoe;
    int Njetscut;
    int dilept;
    int candmumujj;
    int candeejj;
    int onelept;
    int eTmisscut;
    int candmvjj;
    int candevjj;
    int mumujj;
    int eejj;
    int mTonelept;
    int MLQonelept;
    int MtLQonelept;
    int Stvonelept;
    int mTev;
    int MLQev;
    int MtLQev;
    int Stvev;
    int muvjj;
    int evjj;
    int emuvjj;
    int cande;
    int candmu;
    int tmpe;
    int tmpmu;
    int mumuZCR;
    int eeZCR;
    int munuW2CR;
    int munuttCR;
    int enuW2CR;
    int enuttCR;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9041966);

}

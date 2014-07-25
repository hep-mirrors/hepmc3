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


  class ATLAS_2012_I1180197 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2012_I1180197()
      : Analysis("ATLAS_2012_I1180197")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialize projections before the run
    void init() {

      // Projection to find the electrons
      vector<pair<double, double> > eta_e;
      eta_e.push_back(make_pair(-2.47,2.47));
      IdentifiedFinalState elecs(eta_e, 7.0*GeV);
      elecs.acceptIdPair(PID::ELECTRON);
      addProjection(elecs, "elecs");

      // Projection to find the muons
      vector<pair<double, double> > eta_m;
      eta_m.push_back(make_pair(-2.4,2.4));
      IdentifiedFinalState muons(eta_m, 6.0*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");

      // Jet finder
      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4), "AntiKtJets04");

      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0, 3.0, 0.5*GeV), "cfs");

      // for pTmiss
      addProjection(VisibleFinalState(-4.9, 4.9), "vfs");

      // Book histograms
      _count_1l_3jet_all_channel  = bookHisto1D("count_1l_3jet_all_channel", 1, 0., 1.);
      _count_1l_3jet_e_channel    = bookHisto1D("count_1l_3jet_e_channel"  , 1, 0., 1.);
      _count_1l_3jet_mu_channel   = bookHisto1D("count_1l_3jet_mu_channel" , 1, 0., 1.);
      _count_1l_4jet_all_channel  = bookHisto1D("count_1l_4jet_all_channel", 1, 0., 1.);
      _count_1l_4jet_e_channel    = bookHisto1D("count_1l_4jet_e_channel"  , 1, 0., 1.);
      _count_1l_4jet_mu_channel   = bookHisto1D("count_1l_4jet_mu_channel" , 1, 0., 1.);
      _count_1l_soft_all_channel  = bookHisto1D("count_1l_soft_all_channel", 1, 0., 1.);
      _count_1l_soft_e_channel    = bookHisto1D("count_1l_soft_e_channel"  , 1, 0., 1.);
      _count_1l_soft_mu_channel   = bookHisto1D("count_1l_soft_mu_channel" , 1, 0., 1.);

      _count_2l_2jet_all_channel  = bookHisto1D("count_2l_2jet_all_channel" , 1, 0., 1.);
      _count_2l_2jet_ee_channel   = bookHisto1D("count_2l_2jet_ee_channel"  , 1, 0., 1.);
      _count_2l_2jet_emu_channel  = bookHisto1D("count_2l_2jet_emu_channel" , 1, 0., 1.);
      _count_2l_2jet_mumu_channel = bookHisto1D("count_2l_2jet_mumu_channel", 1, 0., 1.);
      _count_2l_4jet_all_channel  = bookHisto1D("count_2l_4jet_all_channel" , 1, 0., 1.);
      _count_2l_4jet_ee_channel   = bookHisto1D("count_2l_4jet_ee_channel"  , 1, 0., 1.);
      _count_2l_4jet_emu_channel  = bookHisto1D("count_2l_4jet_emu_channel" , 1, 0., 1.);
      _count_2l_4jet_mumu_channel = bookHisto1D("count_2l_4jet_mumu_channel", 1, 0., 1.);
      _hist_1l_m_eff_3jet        = bookHisto1D("hist_1l_m_eff_3jet"       ,  6, 400., 1600.);
      _hist_1l_m_eff_4jet        = bookHisto1D("hist_1l_m_eff_4jet"       ,  4, 800., 1600.);
      _hist_1l_eTmiss_m_eff_soft = bookHisto1D("hist_1l_eTmiss_m_eff_soft",  6, 0.1 , 0.7  );
      _hist_2l_m_eff_2jet        = bookHisto1D("hist_2l_m_eff_2jet"       ,  5, 700., 1700.);
      _hist_2l_m_eff_4jet        = bookHisto1D("hist_2l_m_eff_4jet"       ,  5, 600., 1600.);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // get the candiate jets
      Jets cand_jets;
      foreach ( const Jet& jet,
                applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 4.5 ) {
          cand_jets.push_back(jet);
        }
      }
      // charged tracks for isolation
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();
      // find the electrons
      Particles cand_soft_e,cand_hard_e;
      foreach( const Particle & e,
               applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt()) {
        double pT  = e.momentum().pT();
        double eta = e.eta();
        // remove any leptons within 0.4 of any candidate jets
        bool e_near_jet = false;
        foreach ( const Jet& jet, cand_jets ) {
          double dR = deltaR(e.momentum(),jet.momentum());
          if ( inRange(dR, 0.2, 0.4) ) {
            e_near_jet = true;
            break;
          }
        }
        if ( e_near_jet ) continue;
        // soft selection
        if(pT>7.&&!(fabs(eta)>1.37&&fabs(eta) < 1.52)) {
          cand_soft_e.push_back(e);
        }
        // hard selection
        if(pT>10.) cand_hard_e.push_back(e);
      }
      Particles cand_soft_mu,cand_hard_mu;
      foreach( const Particle & mu,
               applyProjection<IdentifiedFinalState>(event, "muons").particlesByPt()) {
        double pT  = mu.momentum().pT();
        double eta = mu.eta();
        // remove any leptons within 0.4 of any candidate jets
        bool mu_near_jet = false;
        foreach ( const Jet& jet, cand_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            mu_near_jet = true;
            break;
          }
        }
        if ( mu_near_jet ) continue;
        // soft selection
        if (pT > 6*GeV && !inRange(fabs(eta), 1.37, 1.52)) {
          cand_soft_mu.push_back(mu);
        }
        // hard selection
        if (pT > 10*GeV) cand_hard_mu.push_back(mu);
      }
      // pTcone around muon track (hard)
      Particles recon_hard_mu;
      foreach ( const Particle & mu, cand_hard_mu ) {
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV ) recon_hard_mu.push_back(mu);
      }
      // pTcone around muon track (soft)
      Particles recon_soft_mu;
      foreach ( const Particle & mu, cand_soft_mu ) {
        double pTinCone = -mu.pT();
        if(-pTinCone>20.) continue;
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV ) recon_soft_mu.push_back(mu);
      }
      // pTcone around electron track (hard)
      Particles recon_hard_e;
      foreach ( const Particle & e, cand_hard_e ) {
        double pTinCone = -e.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1 * e.pT() ) recon_hard_e.push_back(e);
      }
      // pTcone around electron track (soft)
      Particles recon_soft_e;
      foreach ( const Particle & e, cand_soft_e ) {
        double pTinCone = -e.pT();
        if(-pTinCone>25.) continue;
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1 * e.pT() ) recon_soft_e.push_back(e);
      }

      // pTmiss
      FourMomentum pTmiss;
      foreach ( const Particle & p,
                applyProjection<VisibleFinalState>(event, "vfs").particles() ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();

      // hard lepton selection
      if( ! recon_hard_e.empty() || !recon_hard_mu.empty() ) {
        // discard jets that overlap with electrons
        Jets recon_jets;
        foreach ( const Jet& jet, cand_jets ) {
          if(fabs(jet.eta())>2.5||
             jet.momentum().pT() < 25*GeV) continue;
          bool away_from_e = true;
          foreach ( const Particle & e, cand_hard_e ) {
            if ( deltaR(e.momentum(),jet.momentum()) < 0.2 ) {
              away_from_e = false;
              break;
            }
          }
          if ( away_from_e ) recon_jets.push_back( jet );
        }
        // both selections require at least 2 jets
        // meff calculation
        double HT=0.;
        foreach( const Jet & jet, recon_jets) {
          HT += jet.momentum().pT();
        }
        double m_eff_inc  = HT+eTmiss;
        unsigned int njet = recon_jets.size();
        // 1 lepton only
        if( recon_hard_e.size() + recon_hard_mu.size() == 1 && njet >=3 )  {
          // get the lepton
          Particle lepton = recon_hard_e.empty() ?
            recon_hard_mu[0] : recon_hard_e[0];
          // lepton variables
          double pT = lepton.momentum().pT();
          double mT  = 2.*(pT*eTmiss -
                           lepton.momentum().x()*pTmiss.x() -
                           lepton.momentum().y()*pTmiss.y());
          mT = sqrt(mT);
          HT += pT;
          m_eff_inc += pT;
          // apply the cuts on the leptons and min no. of jets
          if( ( ( abs(lepton.pdgId()) == PID::ELECTRON && pT > 25. ) ||
                ( abs(lepton.pdgId()) == PID::MUON     && pT > 20. ) ) &&
              mT > 100. && eTmiss > 250. ) {
            double m_eff = pT+eTmiss;
            for (size_t ix = 0; ix < 3; ++ix)
              m_eff += recon_jets[ix].momentum().pT();
            // 3 jet channel
            if ( (njet == 3 || recon_jets[3].momentum().pT() < 80*GeV ) &&
                recon_jets[0].momentum().pT() > 100*GeV ) {
              if (eTmiss/m_eff > 0.3) {
                if (m_eff_inc > 1200*GeV) {
                  _count_1l_3jet_all_channel->fill(0.5,weight);
                  if (abs(lepton.pdgId()) == PID::ELECTRON )
                    _count_1l_3jet_e_channel->fill(0.5, weight);
                  else
                    _count_1l_3jet_mu_channel->fill(0.5, weight);
                }
                _hist_1l_m_eff_3jet->fill(min(1599., m_eff_inc), weight);
              }
            }
            // 4 jet channel
            else if (njet >=4 && recon_jets[3].momentum().pT() > 80*GeV) {
              m_eff += recon_jets[3].momentum().pT();
              if (eTmiss/m_eff>0.2) {
                if (m_eff_inc > 800*GeV) {
                  _count_1l_4jet_all_channel->fill(0.5, weight);
                  if(abs(lepton.pdgId()) == PID::ELECTRON )
                    _count_1l_4jet_e_channel->fill(0.5, weight);
                  else
                    _count_1l_4jet_mu_channel->fill(0.5, weight);
                }
                _hist_1l_m_eff_4jet->fill(min(1599., m_eff_inc), weight);
              }
            }
          }
        }
        // multi lepton
        else if( recon_hard_e.size() + recon_hard_mu.size() >= 2 && njet >=2 ) {
          // get all the leptons and sort them by pT
          Particles leptons(recon_hard_e.begin(),recon_hard_e.end());
          leptons.insert(leptons.begin(),recon_hard_mu.begin(),recon_hard_mu.end());
          std::sort(leptons.begin(), leptons.end(), cmpMomByPt);
          double m_eff(0.0);
          for (size_t ix = 0; ix < leptons.size(); ++ix)
            m_eff += leptons[ix].momentum().pT();
          m_eff_inc += m_eff;
          m_eff += eTmiss;
          for (size_t ix = 0; ix < (size_t) min(4, int(recon_jets.size())); ++ix)
            m_eff += recon_jets[ix].momentum().pT();
          // require opposite sign leptons
          if (leptons[0].pdgId()*leptons[1].pdgId()<0) {
            // 2 jet
            if (recon_jets[1].momentum().pT()>200 &&
               ( njet<4 || (njet>=4 && recon_jets[3].momentum().pT() < 50*GeV)) && eTmiss > 300*GeV) {
              _count_2l_2jet_all_channel->fill(0.5, weight);
              if (abs(leptons[0].pdgId()) == PID::ELECTRON && abs(leptons[1].pdgId()) == PID::ELECTRON )
                _count_2l_2jet_ee_channel->fill(0.5, weight);
              else if (abs(leptons[0].pdgId()) == PID::MUON && abs(leptons[1].pdgId()) == PID::MUON )
                _count_2l_2jet_mumu_channel->fill(0.5, weight);
              else
                _count_2l_2jet_emu_channel->fill(0.5, weight);
              _hist_2l_m_eff_2jet->fill(min(1699., m_eff_inc), weight);
            }
            // 4 jet
            else if (njet >= 4 && recon_jets[3].momentum().pT() > 50*GeV &&
                     eTmiss > 100*GeV && eTmiss/m_eff > 0.2) {
              if ( m_eff_inc > 650*GeV ) {
                _count_2l_4jet_all_channel->fill(0.5, weight);
                if (abs(leptons[0].pdgId()) == PID::ELECTRON && abs(leptons[1].pdgId()) == PID::ELECTRON )
                  _count_2l_4jet_ee_channel->fill(0.5, weight);
                else if (abs(leptons[0].pdgId()) == PID::MUON && abs(leptons[1].pdgId()) == PID::MUON )
                  _count_2l_4jet_mumu_channel->fill(0.5, weight);
                else
                  _count_2l_4jet_emu_channel->fill(0.5, weight);
              }
              _hist_2l_m_eff_4jet->fill(min(1599., m_eff_inc), weight);
            }
          }
        }
      }
      // soft lepton selection
      if ( recon_soft_e.size() + recon_soft_mu.size() == 1 ) {
        // discard jets that overlap with electrons
        Jets recon_jets;
        foreach ( const Jet& jet, cand_jets ) {
          if (fabs(jet.eta()) > 2.5 || jet.momentum().pT() < 25*GeV) continue;
          bool away_from_e = true;
          foreach ( const Particle & e, cand_soft_e ) {
            if ( deltaR(e.momentum(), jet.momentum()) < 0.2 ) {
              away_from_e = false;
              break;
            }
          }
          if ( away_from_e ) recon_jets.push_back( jet );
        }
        // meff calculation
        double HT=0.;
        foreach (const Jet & jet, recon_jets) {
          HT += jet.momentum().pT();
        }
        double m_eff_inc  = HT+eTmiss;
        // get the lepton
        Particle lepton = recon_soft_e.empty() ?
          recon_soft_mu[0] : recon_soft_e[0];
        // lepton variables
        double pT = lepton.momentum().pT();
        double mT  = 2.*(pT*eTmiss -
                         lepton.momentum().x()*pTmiss.x() -
                         lepton.momentum().y()*pTmiss.y());
        mT = sqrt(mT);
        m_eff_inc += pT;
        double m_eff = pT+eTmiss;
        // apply final cuts
        if (recon_jets.size() >= 2 && recon_jets[0].momentum().pT()>130*GeV && mT > 100*GeV && eTmiss > 250*GeV) {
          for (size_t ix = 0; ix < 2; ++ix)
            m_eff += recon_jets[0].momentum().pT();
          if (eTmiss/m_eff > 0.3) {
            _count_1l_soft_all_channel->fill(0.5, weight);
            if (abs(lepton.pdgId()) == PID::ELECTRON )
              _count_1l_soft_e_channel->fill(0.5, weight);
            else
              _count_1l_soft_mu_channel->fill(0.5, weight);
          }
          _hist_1l_eTmiss_m_eff_soft->fill( eTmiss/m_eff_inc, weight);
        }
      }
    }


    void finalize() {

      double norm = 4.7* crossSection()/sumOfWeights()/femtobarn;
      scale(_count_1l_3jet_all_channel  ,norm);
      scale(_count_1l_3jet_e_channel    ,norm);
      scale(_count_1l_3jet_mu_channel   ,norm);
      scale(_count_1l_4jet_all_channel  ,norm);
      scale(_count_1l_4jet_e_channel    ,norm);
      scale(_count_1l_4jet_mu_channel   ,norm);
      scale(_count_1l_soft_all_channel  ,norm);
      scale(_count_1l_soft_e_channel    ,norm);
      scale(_count_1l_soft_mu_channel   ,norm);
      scale(_count_2l_2jet_all_channel  ,norm);
      scale(_count_2l_2jet_ee_channel   ,norm);
      scale(_count_2l_2jet_emu_channel  ,norm);
      scale(_count_2l_2jet_mumu_channel ,norm);
      scale(_count_2l_4jet_all_channel  ,norm);
      scale(_count_2l_4jet_ee_channel   ,norm);
      scale(_count_2l_4jet_emu_channel  ,norm);
      scale(_count_2l_4jet_mumu_channel ,norm);

      scale(_hist_1l_m_eff_3jet         ,200.*norm);
      scale(_hist_1l_m_eff_4jet         ,200.*norm);
      scale(_hist_1l_eTmiss_m_eff_soft  ,0.1*norm);
      scale(_hist_2l_m_eff_2jet         ,200.*norm);
      scale(_hist_2l_m_eff_4jet         ,200.*norm);

    }

  private:

    /// @name Histos
    //@{
    Histo1DPtr _count_1l_3jet_all_channel;
    Histo1DPtr _count_1l_3jet_e_channel;
    Histo1DPtr _count_1l_3jet_mu_channel;
    Histo1DPtr _count_1l_4jet_all_channel;
    Histo1DPtr _count_1l_4jet_e_channel;
    Histo1DPtr _count_1l_4jet_mu_channel;
    Histo1DPtr _count_1l_soft_all_channel;
    Histo1DPtr _count_1l_soft_e_channel;
    Histo1DPtr _count_1l_soft_mu_channel;
    Histo1DPtr _count_2l_2jet_all_channel;
    Histo1DPtr _count_2l_2jet_ee_channel;
    Histo1DPtr _count_2l_2jet_emu_channel;
    Histo1DPtr _count_2l_2jet_mumu_channel;
    Histo1DPtr _count_2l_4jet_all_channel;
    Histo1DPtr _count_2l_4jet_ee_channel;
    Histo1DPtr _count_2l_4jet_emu_channel;
    Histo1DPtr _count_2l_4jet_mumu_channel;
    Histo1DPtr _hist_1l_m_eff_3jet;
    Histo1DPtr _hist_1l_m_eff_4jet;
    Histo1DPtr _hist_1l_eTmiss_m_eff_soft;
    Histo1DPtr _hist_2l_m_eff_2jet;
    Histo1DPtr _hist_2l_m_eff_4jet;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1180197);

}

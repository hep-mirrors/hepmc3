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


  class ATLAS_2012_CONF_2012_104 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2012_CONF_2012_104()
      : Analysis("ATLAS_2012_CONF_2012_104")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialize projections before the run
    void init() {

      // projection to find the electrons
      vector<pair<double, double> > eta_e;
      eta_e.push_back(make_pair(-2.47,2.47));
      IdentifiedFinalState elecs(eta_e, 10.0*GeV);
      elecs.acceptIdPair(PID::ELECTRON);
      addProjection(elecs, "elecs");

      // projection to find the muons
      vector<pair<double, double> > eta_m;
      eta_m.push_back(make_pair(-2.4,2.4));
      IdentifiedFinalState muons(eta_m, 10.0*GeV);
      muons.acceptIdPair(PID::MUON);
      addProjection(muons, "muons");

      // Jet finder
      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4), "AntiKtJets04");

      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0,0.5*GeV),"cfs");

      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");

      // Book histograms
      _count_e  = bookHisto1D("count_e" , 1, 0., 1.);
      _count_mu = bookHisto1D("count_mu", 1, 0., 1.);

      _hist_eTmiss_e  = bookHisto1D("hist_eTmiss_e"  , 25, 0., 1000.);
      _hist_eTmiss_mu = bookHisto1D("hist_eTmiss_mu" , 25, 0., 1000.);

    }

    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // get the candiate jets
      Jets cand_jets;
      foreach ( const Jet& jet,
                applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 2.8 ) {
          cand_jets.push_back(jet);
        }
      }

      // get the candidate "medium" leptons without isolation
      Particles cand_e;
      foreach( const Particle & e,
               applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt()) {
        // remove any leptons within 0.4 of any candidate jets
        bool e_near_jet = false;
        foreach ( const Jet& jet, cand_jets ) {
          double dR = deltaR(e.momentum(),jet.momentum());
          if ( dR < 0.4 && dR > 0.2 ) {
            e_near_jet = true;
            break;
          }
        }
        if ( ! e_near_jet ) cand_e.push_back(e);
      }
      Particles cand_mu;
      foreach( const Particle & mu,
               applyProjection<IdentifiedFinalState>(event, "muons").particlesByPt()) {
        // remove any leptons within 0.4 of any candidate jets
        bool mu_near_jet = false;
        foreach ( const Jet& jet, cand_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            mu_near_jet = true;
            break;
          }
        }
        if ( ! mu_near_jet ) cand_mu.push_back(mu);
      }
      // apply the isolation
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();
      // pTcone around muon track (hard)
      Particles recon_mu;
      foreach ( const Particle & mu, cand_mu ) {
        double pTinCone = -mu.pT();
        if(-pTinCone<25.) continue;
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV ) recon_mu.push_back(mu);
      }
      // pTcone around electron track (hard)
      Particles recon_e;
      foreach ( const Particle & e, cand_e ) {
        double pTinCone = -e.pT();
        if(-pTinCone<25.) continue;
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1 * e.pT() ) recon_e.push_back(e);
      }

      // discard jets that overlap with electrons
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets ) {
        if(fabs(jet.eta())>2.5||
           jet.momentum().perp()<25.) continue;
        bool away_from_e = true;
        foreach ( const Particle & e, cand_e ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.2 ) {
            away_from_e = false;
            break;
          }
        }
        if ( away_from_e ) recon_jets.push_back( jet );
      }

      // pTmiss
      FourMomentum pTmiss;
      foreach ( const Particle & p,
                applyProjection<VisibleFinalState>(event, "vfs").particles() ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();

      // at least 4 jets with pT>80.
      if(recon_jets.size()<4 || recon_jets[3].momentum().perp()<80.) vetoEvent;

      // only 1 signal lepton
      if( recon_e.size() + recon_mu.size() != 1 )
        vetoEvent;
      if( cand_e .size() + cand_mu .size() != 1 )
        vetoEvent;

      // start of meff calculation
      double HT=0.;
      foreach( const Jet & jet, recon_jets) {
        double pT = jet.momentum().perp();
        if(pT>40.) HT += pT;
      }

      // get the lepton
      Particle lepton = recon_e.empty() ? recon_mu[0] : recon_e[0];

      // lepton variables
      double pT = lepton.momentum().perp();

      double mT  = 2.*(pT*eTmiss -
                       lepton.momentum().x()*pTmiss.x() -
                       lepton.momentum().y()*pTmiss.y());
      mT = sqrt(mT);
      HT += pT;
      double m_eff_inc  = HT + eTmiss + pT;
      double m_eff_4 = eTmiss + pT;
      for(unsigned int ix=0;ix<4;++ix)
        m_eff_4 +=  recon_jets[ix].momentum().perp();

      // four jet selecton
      if(mT>100.&& eTmiss/m_eff_4>0.2 &&
         m_eff_inc > 800.) {
        if( eTmiss > 250. ) {
          if(abs(lepton.pdgId())==PID::ELECTRON)
            _count_e->fill(0.5,weight);
          else if(abs(lepton.pdgId())==PID::MUON)
            _count_mu->fill(0.5,weight);
        }
        if(abs(lepton.pdgId())==PID::ELECTRON)
          _hist_eTmiss_e ->fill(eTmiss,weight);
        else if(abs(lepton.pdgId())==PID::MUON)
          _hist_eTmiss_mu->fill(eTmiss,weight);
      }
    }
    //@}


    void finalize() {

      double norm = 5.8* crossSection()/sumOfWeights()/femtobarn;
      scale(_count_e ,norm);
      scale(_count_mu,norm);
      scale(_hist_eTmiss_e  ,40.*norm);
      scale(_hist_eTmiss_mu ,40.*norm);

    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_e ;
    Histo1DPtr _count_mu;

    Histo1DPtr _hist_eTmiss_e ;
    Histo1DPtr _hist_eTmiss_mu;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_CONF_2012_104);

}

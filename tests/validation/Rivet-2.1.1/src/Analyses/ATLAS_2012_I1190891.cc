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

  /// @author Peter Richardson
  class ATLAS_2012_I1190891 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I1190891()
      : Analysis("ATLAS_2012_I1190891")
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
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

      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");

      VetoedFinalState vfs;
      vfs.addVetoPairId(PID::MUON);

      /// Jet finder
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.4),
                    "AntiKtJets04");

      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0),"cfs");

      // Book histograms
      _hist_etmiss = bookHisto1D("hist_etmiss",10,0.,500.);
      _hist_meff   = bookHisto1D("hist_m_eff",7,0.,1050.);
      _count_SR1 = bookHisto1D("count_SR1", 1, 0., 1.);
      _count_SR2 = bookHisto1D("count_SR2", 1, 0., 1.);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();
      // get the jet candidates
      Jets cand_jets;
      foreach (const Jet& jet,
               applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 2.5 ) {
          cand_jets.push_back(jet);
        }
      }

      // candidate muons
      Particles cand_mu;
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();
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

      // candidate electrons
      Particles cand_e;
      foreach ( const Particle & e,
                applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt() ) {
        double pTinCone = -e.momentum().perp();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) <= 0.2 )
            pTinCone += track.pT();
        }
        if (pTinCone/e.momentum().perp()<0.1) {
          cand_e.push_back(e);
        }
      }

      // resolve jet/lepton ambiguity
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

      // only keep electrons more than R=0.4 from jets
      Particles cand2_e;
      for(unsigned int ie=0;ie<cand_e.size();++ie) {
        const Particle & e = cand_e[ie];
        // at least 0.4 from any jets
        bool away = true;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        // and 0.1 from any muons
        if ( away ) {
          foreach ( const Particle & mu, cand_mu ) {
            if ( deltaR(mu.momentum(),e.momentum()) < 0.1 ) {
              away = false;
              break;
            }
          }
        }
        // and 0.1 from electrons ( keep higher energy)
        for(unsigned int ie2=0;ie2<cand_e.size();++ie2) {
          if(ie==ie2) continue;
          if ( deltaR(e.momentum(),cand_e[ie2].momentum()) < 0.1 &&
               e.momentum().E() < cand_e[ie2].momentum().E() ) {
            away = false;
            break;
          }
        }
        // if isolated keep it
        if ( away )
          cand2_e.push_back( e );
      }

      // remove e+e- pairs with mass < 20.
      Particles recon_e;
      for(unsigned int ie=0;ie<cand2_e.size();++ie) {
        bool pass = true;
        for(unsigned int ie2=0;ie2<cand2_e.size();++ie2) {
          if(cand2_e[ie].pdgId()*cand2_e[ie2].pdgId()>0) continue;
          double mtest = (cand2_e[ie].momentum()+cand2_e[ie2].momentum()).mass();
          if(mtest<=20.) {
            pass = false;
            break;
          }
        }
        if(pass) recon_e.push_back(cand2_e[ie]);
      }

      // only keep muons more than R=0.4 from jets
      Particles cand2_mu;
      for(unsigned int imu=0;imu<cand_mu.size();++imu) {
        const Particle & mu = cand_mu[imu];
        bool away = true;
        // at least 0.4 from any jets
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        // and 0.1 from any electrona
        if ( away ) {
          foreach ( const Particle & e, cand_e ) {
            if ( deltaR(mu.momentum(),e.momentum()) < 0.1 ) {
              away = false;
              break;
            }
          }
        }
        if ( away )
          cand2_mu.push_back( mu );
      }

      // remove mu+mu- pairs with mass < 20.
      Particles recon_mu;
      for(unsigned int imu=0;imu<cand2_mu.size();++imu) {
        bool pass = true;
        for(unsigned int imu2=0;imu2<cand2_mu.size();++imu2) {
          if(cand2_mu[imu].pdgId()*cand2_mu[imu2].pdgId()>0) continue;
          double mtest = (cand2_mu[imu].momentum()+cand2_mu[imu2].momentum()).mass();
          if(mtest<=20.) {
            pass = false;
            break;
          }
        }
        if(pass) recon_mu.push_back(cand2_mu[imu]);
      }

      // pTmiss
      Particles vfs_particles =
        applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();

      // now only use recon_jets, recon_mu, recon_e

      // reject events with less than 4 electrons and muons
      if ( recon_mu.size() + recon_e.size() < 4 ) {
        MSG_DEBUG("To few charged leptons left after selection");
        vetoEvent;
      }

      // check if passes single lepton trigger
      bool passSingle =
        ( !recon_e .empty() && recon_e[0] .momentum().perp()>25. )||
        ( !recon_mu.empty() && recon_mu[0].momentum().perp()>20.);

      // or two lepton trigger
      bool passDouble =
        ( recon_mu.size()>=2 && recon_mu[1].momentum().perp()>12.) ||
        ( recon_e .size()>=2 && recon_e [1].momentum().perp()>17.) ||
        ( !recon_e.empty() && !recon_mu.empty() &&
          recon_e[0].momentum().perp()>15. &&  recon_mu[0].momentum().perp()>10.);

      // must pass a trigger
      if( !passSingle && !passDouble ) {
        MSG_DEBUG("Hardest lepton fails trigger");
        vetoEvent;
      }

      // calculate meff
      double meff = eTmiss;
      foreach ( const Particle & e , recon_e  )
        meff += e.momentum().perp();
      foreach ( const Particle & mu, recon_mu )
        meff += mu.momentum().perp();
      foreach ( const Jet & jet, recon_jets ) {
        double pT = jet.momentum().perp();
        if(pT>40.) meff += pT;
      }

      // mass of SFOS pairs closest to the Z mass
      for(unsigned int ix=0;ix<recon_e.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_e.size();++iy) {
          if(recon_e[ix].pdgId()*recon_e[iy].pdgId()>0) continue;
          double mtest = (recon_e[ix].momentum()+recon_e[iy].momentum()).mass();
          if(mtest>81.2 && mtest<101.2) vetoEvent;
        }
      }
      for(unsigned int ix=0;ix<recon_mu.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_mu.size();++iy) {
          if(recon_mu[ix].pdgId()*recon_mu[iy].pdgId()>0) continue;
          double mtest = (recon_mu[ix].momentum()+recon_mu[iy].momentum()).mass();
          if(mtest>81.2 && mtest<101.2) vetoEvent;
        }
      }

      // make the control plots
      _hist_etmiss ->fill(eTmiss,weight);
      _hist_meff   ->fill(meff  ,weight);
      // finally the counts
      if(eTmiss>50.) _count_SR1->fill(0.5,weight);
      if(meff >300.) _count_SR2->fill(0.5,weight);
    }

    //@}

    void finalize() {
      double norm = crossSection()/femtobarn*4.7/sumOfWeights();
      scale(_hist_etmiss,norm* 50.);
      scale(_hist_meff  ,norm*150.);
      scale(_count_SR1,norm);
      scale(_count_SR2,norm);
    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _hist_etmiss;
    Histo1DPtr _hist_meff;
    Histo1DPtr _count_SR1;
    Histo1DPtr _count_SR2;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1190891);

}

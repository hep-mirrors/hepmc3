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
  class ATLAS_2012_CONF_2012_001 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_CONF_2012_001()
      : Analysis("ATLAS_2012_CONF_2012_001")
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

      // projection to find the muons
      std::vector<std::pair<double, double> > eta_m;
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
      _hist_leptonpT.push_back(bookHisto1D(1,1,1));
      _hist_leptonpT.push_back(bookHisto1D(2,1,1));
      _hist_leptonpT.push_back(bookHisto1D(3,1,1));
      _hist_leptonpT.push_back(bookHisto1D(4,1,1));
      _hist_njet   = bookHisto1D(5,1,1);
      _hist_etmiss = bookHisto1D(6,1,1);
      _hist_mSFOS  = bookHisto1D(7,1,1);
      _hist_meff   = bookHisto1D(8,1,1);

      _hist_leptonpT_MC.push_back(bookHisto1D("hist_lepton_pT_1", 26, 0., 260));
      _hist_leptonpT_MC.push_back(bookHisto1D("hist_lepton_pT_2", 15, 0., 150));
      _hist_leptonpT_MC.push_back(bookHisto1D("hist_lepton_pT_3", 20, 0., 100));
      _hist_leptonpT_MC.push_back(bookHisto1D("hist_lepton_pT_4", 20, 0., 100));
      _hist_njet_MC   = bookHisto1D("hist_njet", 7, -0.5, 6.5);
      _hist_etmiss_MC = bookHisto1D("hist_etmiss",11,0.,220.);
      _hist_mSFOS_MC  = bookHisto1D("hist_m_SFOS",13,0.,260.);
      _hist_meff_MC   = bookHisto1D("hist_m_eff",19,0.,950.);

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
        if ( fabs( jet.eta() ) < 2.8 ) {
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
        double eta = e.eta();
        // remove electrons with pT<15 in old veto region
        if( fabs(eta)>1.37 && fabs(eta) < 1.52 && e.momentum().perp()< 15.*GeV)
          continue;
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
        // and 0.1 from electrons
        for(unsigned int ie2=0;ie2<cand_e.size();++ie2) {
          if(ie==ie2) continue;
          if ( deltaR(e.momentum(),cand_e[ie2].momentum()) < 0.1 ) {
            away = false;
            break;
          }
        }
        // if isolated keep it
        if ( away ) cand2_e.push_back( e );
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
        // and 0.1 from muons
        for(unsigned int imu2=0;imu2<cand_mu.size();++imu2) {
          if(imu==imu2) continue;
          if ( deltaR(mu.momentum(),cand_mu[imu2].momentum()) < 0.1 ) {
            away = false;
            break;
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

      // ATLAS calo problem
      if(rand()/static_cast<double>(RAND_MAX)<=0.42) {
        foreach ( const Particle & e, recon_e ) {
          double eta = e.eta();
          double phi = e.momentum().azimuthalAngle(MINUSPI_PLUSPI);
          if(eta>-0.1&&eta<1.5&&phi>-0.9&&phi<-0.5)
            vetoEvent;
        }
        foreach ( const Jet & jet, recon_jets ) {
          double eta = jet.rapidity();
          double phi = jet.momentum().azimuthalAngle(MINUSPI_PLUSPI);
          if(jet.momentum().perp()>40 && eta>-0.1&&eta<1.5&&phi>-0.9&&phi<-0.5)
            vetoEvent;
        }
      }

      // check at least one e/mu passing trigger
      if( !( !recon_e .empty() && recon_e[0] .momentum().perp()>25.)  &&
          !( !recon_mu.empty() && recon_mu[0].momentum().perp()>20.) ) {
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

      double mSFOS=1e30, mdiff=1e30;
      // mass of SFOS pairs closest to the Z mass
      for(unsigned int ix=0;ix<recon_e.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_e.size();++iy) {
          if(recon_e[ix].pdgId()*recon_e[iy].pdgId()>0) continue;
          double mtest = (recon_e[ix].momentum()+recon_e[iy].momentum()).mass();
          if(fabs(mtest-90.)<mdiff) {
            mSFOS = mtest;
            mdiff = fabs(mtest-90.);
          }
        }
      }
      for(unsigned int ix=0;ix<recon_mu.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_mu.size();++iy) {
          if(recon_mu[ix].pdgId()*recon_mu[iy].pdgId()>0) continue;
          double mtest = (recon_mu[ix].momentum()+recon_mu[iy].momentum()).mass();
          if(fabs(mtest-91.118)<mdiff) {
            mSFOS = mtest;
            mdiff = fabs(mtest-91.118);
          }
        }
      }

      // make the control plots
      // lepton pT
      unsigned int ie=0,imu=0;
      for(unsigned int ix=0;ix<4;++ix) {
        double pTe  = ie <recon_e .size() ?
          recon_e [ie ].momentum().perp() : -1*GeV;
        double pTmu = imu<recon_mu.size() ?
          recon_mu[imu].momentum().perp() : -1*GeV;
        if(pTe>pTmu) {
          _hist_leptonpT   [ix]->fill(pTe ,weight);
          _hist_leptonpT_MC[ix]->fill(pTe ,weight);
          ++ie;
        }
        else {
          _hist_leptonpT   [ix]->fill(pTmu,weight);
          _hist_leptonpT_MC[ix]->fill(pTmu,weight);
          ++imu;
        }
      }
      // njet
      _hist_njet   ->fill(recon_jets.size(),weight);
      _hist_njet_MC->fill(recon_jets.size(),weight);
      // etmiss
      _hist_etmiss   ->fill(eTmiss,weight);
      _hist_etmiss_MC->fill(eTmiss,weight);
      if(mSFOS<1e30) {
        _hist_mSFOS   ->fill(mSFOS,weight);
        _hist_mSFOS_MC->fill(mSFOS,weight);
      }
      _hist_meff   ->fill(meff,weight);
      _hist_meff_MC->fill(meff,weight);

      // finally the counts
      if(eTmiss>50.) {
        _count_SR1->fill(0.5,weight);
        if(mdiff>10.) _count_SR2->fill(0.5,weight);
      }
    }

    //@}

    void finalize() {
      double norm = crossSection()/femtobarn*2.06/sumOfWeights();
      // these are number of events at 2.06fb^-1 per 10 GeV
      scale(_hist_leptonpT   [0],norm*10.);
      scale(_hist_leptonpT   [1],norm*10.);
      scale(_hist_leptonpT_MC[0],norm*10.);
      scale(_hist_leptonpT_MC[1],norm*10.);
      // these are number of events at 2.06fb^-1 per 5 GeV
      scale(_hist_leptonpT   [2],norm*5.);
      scale(_hist_leptonpT   [3],norm*5.);
      scale(_hist_leptonpT_MC[2],norm*5.);
      scale(_hist_leptonpT_MC[3],norm*5.);
      // these are number of events at 2.06fb^-1 per 20 GeV
      scale(_hist_etmiss      ,norm*20.);
      scale(_hist_mSFOS       ,norm*20.);
      scale(_hist_etmiss_MC   ,norm*20.);
      scale(_hist_mSFOS_MC    ,norm*20.);
      // these are number of events at 2.06fb^-1 per 50 GeV
      scale(_hist_meff        ,norm*50.);
      scale(_hist_meff_MC     ,norm*50.);
      // these are number of events at 2.06fb^-1
      scale(_hist_njet        ,norm);
      scale(_hist_njet_MC     ,norm);
      scale(_count_SR1,norm);
      scale(_count_SR2,norm);
    }

  private:

    /// @name Histograms
    //@{
    vector<Histo1DPtr> _hist_leptonpT,_hist_leptonpT_MC;
    Histo1DPtr _hist_njet;
    Histo1DPtr _hist_njet_MC;
    Histo1DPtr _hist_etmiss;
    Histo1DPtr _hist_etmiss_MC;
    Histo1DPtr _hist_mSFOS;
    Histo1DPtr _hist_mSFOS_MC;
    Histo1DPtr _hist_meff;
    Histo1DPtr _hist_meff_MC;
    Histo1DPtr _count_SR1;
    Histo1DPtr _count_SR2;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_CONF_2012_001);

}

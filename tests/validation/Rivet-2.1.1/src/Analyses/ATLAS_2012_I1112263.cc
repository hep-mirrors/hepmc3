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
  class ATLAS_2012_I1112263 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I1112263()
      : Analysis("ATLAS_2012_I1112263")
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
      _hist_leptonpT_SR1.push_back(bookHisto1D("hist_lepton_pT_1_SR1",11,0.,220.));
      _hist_leptonpT_SR1.push_back(bookHisto1D("hist_lepton_pT_2_SR1", 7,0.,140.));
      _hist_leptonpT_SR1.push_back(bookHisto1D("hist_lepton_pT_3_SR1", 8,0.,160.));
      _hist_leptonpT_SR2.push_back(bookHisto1D("hist_lepton_pT_1_SR2",11,0.,220.));
      _hist_leptonpT_SR2.push_back(bookHisto1D("hist_lepton_pT_2_SR2", 7,0.,140.));
      _hist_leptonpT_SR2.push_back(bookHisto1D("hist_lepton_pT_3_SR2", 8,0.,160.));
      _hist_etmiss_SR1_A = bookHisto1D("hist_etmiss_SR1_A",15,10.,310.);
      _hist_etmiss_SR1_B = bookHisto1D("hist_etmiss_SR1_B", 9,10.,190.);
      _hist_etmiss_SR2_A = bookHisto1D("hist_etmiss_SR2_A",15,10.,310.);
      _hist_etmiss_SR2_B = bookHisto1D("hist_etmiss_SR2_B", 9,10.,190.);
      _hist_mSFOS= bookHisto1D("hist_mSFOF",9,0.,180.);

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
        // (NOT EXPLICIT IN THIS PAPER BUT IN SIMILAR 4 LEPTON PAPER and THIS DESCRPITION
        //  IS MUCH WORSE SO ASSUME THIS IS DONE)
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
      // (NOT EXPLICIT IN THIS PAPER BUT IN SIMILAR 4 LEPTON PAPER and THIS DESCRPITION
      //  IS MUCH WORSE SO ASSUME THIS IS DONE)
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
      Particles recon_e;
      foreach ( const Particle & e, cand_e ) {
        bool away = true;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        // and 0.1 from any muons
        if ( ! away ) {
          foreach ( const Particle & mu, cand_e ) {
            if ( deltaR(mu.momentum(),e.momentum()) < 0.1 ) {
              away = false;
              break;
            }
          }
        }
        if ( away )
          recon_e.push_back( e );
      }
      // only keep muons more than R=0.4 from jets
      Particles recon_mu;
      foreach ( const Particle & mu, cand_mu ) {
        bool away = true;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            away = false;
            break;
          }
        }
        // and 0.1 from any electrona
        if ( ! away ) {
          foreach ( const Particle & e, cand_e ) {
            if ( deltaR(mu.momentum(),e.momentum()) < 0.1 ) {
              away = false;
              break;
            }
          }
        }
        if ( away )
          recon_mu.push_back( mu );
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

      // reject events with wrong number of leptons
      if ( recon_mu.size() + recon_e.size() != 3 ) {
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

      // eTmiss cut
      if(eTmiss<50.) vetoEvent;

      // check at least 1 SFOS pair
      double mSFOS=1e30, mdiff=1e30;
      unsigned int nSFOS=0;
      for(unsigned int ix=0;ix<recon_e.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_e.size();++iy) {
          if(recon_e[ix].pdgId()*recon_e[iy].pdgId()>0) continue;
          ++nSFOS;
          double mtest = (recon_e[ix].momentum()+recon_e[iy].momentum()).mass();
          // veto is mass<20
          if(mtest<20.) vetoEvent;
          if(fabs(mtest-90.)<mdiff) {
            mSFOS = mtest;
            mdiff = fabs(mtest-90.);
          }
        }
      }
      for(unsigned int ix=0;ix<recon_mu.size();++ix) {
        for(unsigned int iy=ix+1;iy<recon_mu.size();++iy) {
          if(recon_mu[ix].pdgId()*recon_mu[iy].pdgId()>0) continue;
          ++nSFOS;
          double mtest = (recon_mu[ix].momentum()+recon_mu[iy].momentum()).mass();
          // veto is mass<20
          if(mtest<20.) vetoEvent;
          if(fabs(mtest-90.)<mdiff) {
            mSFOS = mtest;
            mdiff = fabs(mtest-90.);
          }
        }
      }
      // require at least 1 SFOS pair
      if(nSFOS==0) vetoEvent;
      // b-jet veto in SR!
      if(mdiff>10.) {
        foreach (const Jet & jet, recon_jets ) {
          if(jet.containsBottom() &&
             rand()/static_cast<double>(RAND_MAX)<=0.60)
            vetoEvent;
        }
      }
      // region SR1, Z depleted
      if(mdiff>10.) {
        _count_SR1->fill(0.5,weight);
        _hist_etmiss_SR1_A->fill(eTmiss,weight);
        _hist_etmiss_SR1_B->fill(eTmiss,weight);
        _hist_mSFOS->fill(mSFOS,weight);
      }
      // region SR2, Z enriched
      else {
        _count_SR2->fill(0.5,weight);
        _hist_etmiss_SR2_A->fill(eTmiss,weight);
        _hist_etmiss_SR2_B->fill(eTmiss,weight);
      }
      // make the control plots
      // lepton pT
      unsigned int ie=0,imu=0;
      for(unsigned int ix=0;ix<3;++ix) {
        Histo1DPtr hist = mdiff>10. ?
          _hist_leptonpT_SR1[ix] :  _hist_leptonpT_SR2[ix];
        double pTe  = ie <recon_e .size() ?
          recon_e [ie ].momentum().perp() : -1*GeV;
        double pTmu = imu<recon_mu.size() ?
          recon_mu[imu].momentum().perp() : -1*GeV;
        if(pTe>pTmu) {
          hist->fill(pTe ,weight);
          ++ie;
        }
        else {
          hist->fill(pTmu,weight);
          ++imu;
        }
      }
    }

    //@}

    void finalize() {
      double norm = crossSection()/femtobarn*2.06/sumOfWeights();
      // these are number of events at 2.06fb^-1 per 20 GeV
      for(unsigned int ix=0;ix<3;++ix) {
        scale(_hist_leptonpT_SR1[ix],norm*20.);
        scale(_hist_leptonpT_SR2[ix],norm*20.);
      }
      scale(_hist_etmiss_SR1_A,norm*20.);
      scale(_hist_etmiss_SR1_B,norm*20.);
      scale(_hist_etmiss_SR2_A,norm*20.);
      scale(_hist_etmiss_SR2_B,norm*20.);
      scale(_hist_mSFOS    ,norm*20.);
      // these are number of events at 2.06fb^-1
      scale(_count_SR1,norm);
      scale(_count_SR2,norm);
    }

  private:

  /// @name Histograms
  //@{
  vector<Histo1DPtr> _hist_leptonpT_SR1;
  vector<Histo1DPtr> _hist_leptonpT_SR2;
  Histo1DPtr _hist_etmiss_SR1_A;
  Histo1DPtr _hist_etmiss_SR1_B;
  Histo1DPtr _hist_etmiss_SR2_A;
  Histo1DPtr _hist_etmiss_SR2_B;
  Histo1DPtr _hist_mSFOS;
  Histo1DPtr _count_SR1;
  Histo1DPtr _count_SR2;
  //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1112263);

}

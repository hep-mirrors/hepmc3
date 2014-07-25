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


  class ATLAS_2012_I943401 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2012_I943401()
      : Analysis("ATLAS_2012_I943401")
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

      // projection to find the muons
      std::vector<std::pair<double, double> > eta_m;
      eta_m.push_back(make_pair(-2.4,2.4));
      IdentifiedFinalState muons(eta_m, 10.0*GeV);
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
      addProjection(VisibleFinalState(-4.5,4.5),"vfs");

      // book histograms

      // counts in signal regions
      _count_OS_SR1 = bookHisto1D("count_OS_SR1", 1, 0., 1.);
      _count_OS_SR2 = bookHisto1D("count_OS_SR2", 1, 0., 1.);
      _count_OS_SR3 = bookHisto1D("count_OS_SR3", 1, 0., 1.);
      _count_SS_SR1 = bookHisto1D("count_SS_SR1", 1, 0., 1.);
      _count_SS_SR2 = bookHisto1D("count_SS_SR2", 1, 0., 1.);
      _count_FS_SR1 = bookHisto1D("count_FS_SR1", 1, 0., 1.);
      _count_FS_SR2 = bookHisto1D("count_FS_SR2", 1, 0., 1.);
      _count_FS_SR3 = bookHisto1D("count_FS_SR3", 1, 0., 1.);

      // histograms from paper

      _hist_mll_SS_D         = bookHisto1D( 1,1,1);
      _hist_mll_SS_B         = bookHisto1D( 1,1,2);
      _hist_eTmiss_SS_D      = bookHisto1D( 2,1,1);
      _hist_eTmiss_SS_B      = bookHisto1D( 2,1,2);
      _hist_mll_SS_2Jet_D    = bookHisto1D( 3,1,1);
      _hist_mll_SS_2Jet_B    = bookHisto1D( 3,1,2);
      _hist_njet_SS_D        = bookHisto1D( 5,1,1);
      _hist_njet_SS_B        = bookHisto1D( 5,1,2);
      _hist_pT_j1_SS_D       = bookHisto1D( 6,1,1);
      _hist_pT_j1_SS_B       = bookHisto1D( 6,1,2);
      _hist_pT_j2_SS_D       = bookHisto1D( 7,1,1);
      _hist_pT_j2_SS_B       = bookHisto1D( 7,1,2);
      _hist_pT_l1_SS_D       = bookHisto1D( 8,1,1);
      _hist_pT_l1_SS_B       = bookHisto1D( 8,1,2);
      _hist_pT_l2_SS_D       = bookHisto1D( 9,1,1);
      _hist_pT_l2_SS_B       = bookHisto1D( 9,1,2);
      _hist_mll_OS_D         = bookHisto1D(10,1,1);
      _hist_mll_OS_B         = bookHisto1D(10,1,2);
      _hist_eTmiss_OS_D      = bookHisto1D(11,1,1);
      _hist_eTmiss_OS_B      = bookHisto1D(11,1,2);
      _hist_eTmiss_3Jet_OS_D = bookHisto1D(12,1,1);
      _hist_eTmiss_3Jet_OS_B = bookHisto1D(12,1,2);
      _hist_eTmiss_4Jet_OS_D = bookHisto1D(13,1,1);
      _hist_eTmiss_4Jet_OS_B = bookHisto1D(13,1,2);
      _hist_njet_OS_D        = bookHisto1D(14,1,1);
      _hist_njet_OS_B        = bookHisto1D(14,1,2);
      _hist_pT_j1_OS_D       = bookHisto1D(15,1,1);
      _hist_pT_j1_OS_B       = bookHisto1D(15,1,2);
      _hist_pT_j2_OS_D       = bookHisto1D(16,1,1);
      _hist_pT_j2_OS_B       = bookHisto1D(16,1,2);
      _hist_pT_l1_OS_D       = bookHisto1D(17,1,1);
      _hist_pT_l1_OS_B       = bookHisto1D(17,1,2);
      _hist_pT_l2_OS_D       = bookHisto1D(18,1,1);
      _hist_pT_l2_OS_B       = bookHisto1D(18,1,2);
      //????
      //   <dataPointSet name="d04-x01-y01" dimension="2" path="/REF/ATLAS_2011_I943401" title="EVENTS/10 GEV" >
      //   <dataPointSet name="d04-x01-y02" dimension="2" path="/REF/ATLAS_2011_I943401" title="EVENTS/10 GEV" >
    }

    /// Perform the event analysis
    void analyze(const Event& event) {
      // event weight
      const double weight = event.weight();

      // get the jet candidates
      Jets cand_jets;
      foreach (const Jet& jet,
        applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 2.8 ) {
          cand_jets.push_back(jet);
        }
      }

      // electron candidates
      Particles cand_e =
        applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt();

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
          if ( away_from_e ) recon_jets.push_back( jet );
      }
      // get the charged tracks for isolation
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();

      // Reconstructed electrons
      Particles recon_e;
      foreach ( const Particle & e, cand_e ) {
        // check not near a jet
        bool e_near_jet = false;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 ) {
            e_near_jet = true;
            break;
          }
        }
        if ( e_near_jet ) continue;
        // check the isolation
        double pTinCone = -e.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1*e.momentum().perp() )
          recon_e.push_back(e);
      }

      // Reconstructed Muons
      Particles recon_mu;
      Particles cand_mu =
        applyProjection<IdentifiedFinalState>(event,"muons").particlesByPt();
      foreach ( const Particle & mu, cand_mu ) {
        // check not near a jet
        bool mu_near_jet = false;
        foreach ( const Jet& jet, recon_jets ) {
          if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 ) {
            mu_near_jet = true;
            break;
          }
        }
        if ( mu_near_jet ) continue;
        // isolation
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV )
          recon_mu.push_back(mu);
      }

      // pTmiss
      Particles vfs_particles
        = applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();

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

      // Exactly two leptons for each event
      if ( recon_mu.size() + recon_e.size() != 2)
        vetoEvent;
      // two electrons highest pT > 25
      Particles recon_leptons;
      if(recon_e.size()==2&&recon_e[0].momentum().perp()>25.) {
        recon_leptons = recon_e;
      }
      // two muons highest pT > 20
      else if(recon_mu.size()==2&&recon_mu[0].momentum().perp()>20.) {
        recon_leptons = recon_mu;
      }
      else if(recon_e.size()==1 && recon_mu.size()==1 &&
              (recon_e[0].momentum().perp()>25. ||recon_mu[0].momentum().perp()>20. )) {
        if(recon_mu[0].momentum().perp()<recon_e[0].momentum().perp()) {
          recon_leptons.push_back(recon_e [0]);
          recon_leptons.push_back(recon_mu[0]);
        }
        else {
          recon_leptons.push_back(recon_mu[0]);
          recon_leptons.push_back(recon_e [0]);
        }
      }
      // fails trigger
      else
        vetoEvent;

      double mll = (recon_leptons[0].momentum()+recon_leptons[1].momentum()).mass();
      // lepton pair mass > 12.
      if(mll < 12.) vetoEvent;

      // same sign or opposite sign event
      int sign = recon_leptons[0].pdgId()*recon_leptons[1].pdgId();

      // same sign leptons
      if(sign>0) {
        _hist_mll_SS_D   ->fill(mll   ,weight);
        _hist_mll_SS_B   ->fill(mll   ,weight);
        _hist_eTmiss_SS_D->fill(eTmiss,weight);
        _hist_eTmiss_SS_B->fill(eTmiss,weight);
        if(recon_jets.size()>=2) {
          _hist_mll_SS_2Jet_D   ->fill(mll   ,weight);
          _hist_mll_SS_2Jet_B   ->fill(mll   ,weight);
        }
        _hist_njet_SS_D ->fill(recon_jets.size(),weight);
        _hist_njet_SS_B ->fill(recon_jets.size(),weight);
        if(!recon_jets.empty()) {
          _hist_pT_j1_SS_D->fill(recon_jets[0].momentum().perp(),weight);
          _hist_pT_j1_SS_B->fill(recon_jets[0].momentum().perp(),weight);
        }
        if(recon_jets.size()>2) {
          _hist_pT_j2_SS_D->fill(recon_jets[1].momentum().perp(),weight);
          _hist_pT_j2_SS_B->fill(recon_jets[1].momentum().perp(),weight);
        }
        _hist_pT_l1_SS_D->fill(recon_leptons[0].momentum().perp(),weight);
        _hist_pT_l1_SS_B->fill(recon_leptons[0].momentum().perp(),weight);
        _hist_pT_l2_SS_D->fill(recon_leptons[1].momentum().perp(),weight);
        _hist_pT_l2_SS_B->fill(recon_leptons[1].momentum().perp(),weight);
        // SS-SR1
        if(eTmiss>100.) {
          _count_SS_SR1->fill(0.5,weight);
        }
        // SS-SR2
        if(eTmiss>80. && recon_jets.size()>=2 &&
           recon_jets[1].momentum().perp()>50.) {
          _count_SS_SR2->fill(0.5,weight);
        }
      }
      // opposite sign
      else {
        _hist_mll_OS_D->fill(mll   ,weight);
        _hist_mll_OS_B->fill(mll   ,weight);
        _hist_eTmiss_OS_D->fill(eTmiss,weight);
        _hist_eTmiss_OS_B->fill(eTmiss,weight);
        if(recon_jets.size()>=3){
          _hist_eTmiss_3Jet_OS_D->fill(eTmiss,weight);
          _hist_eTmiss_3Jet_OS_B->fill(eTmiss,weight);
        }
        if(recon_jets.size()>=4){
          _hist_eTmiss_4Jet_OS_D->fill(eTmiss,weight);
          _hist_eTmiss_4Jet_OS_B->fill(eTmiss,weight);
        }
        _hist_njet_OS_D->fill(recon_jets.size(),weight);
        _hist_njet_OS_B->fill(recon_jets.size(),weight);
        if(!recon_jets.empty()) {
          _hist_pT_j1_OS_D->fill(recon_jets[0].momentum().perp(),weight);
          _hist_pT_j1_OS_B->fill(recon_jets[0].momentum().perp(),weight);
        }
        if(recon_jets.size()>2) {
          _hist_pT_j2_OS_D->fill(recon_jets[1].momentum().perp(),weight);
          _hist_pT_j2_OS_B->fill(recon_jets[1].momentum().perp(),weight);
        }
        _hist_pT_l1_OS_D->fill(recon_leptons[0].momentum().perp(),weight);
        _hist_pT_l1_OS_B->fill(recon_leptons[0].momentum().perp(),weight);
        _hist_pT_l2_OS_D->fill(recon_leptons[1].momentum().perp(),weight);
        _hist_pT_l2_OS_B->fill(recon_leptons[1].momentum().perp(),weight);
        // different signal regions
        // OS-SR1
        if(eTmiss>250.) {
          _count_OS_SR1->fill(0.5,weight);
        }
        // OS-SR2
        if(eTmiss>220. && recon_jets.size()>=3 &&
           recon_jets[0].momentum().perp()>80. &&
           recon_jets[2].momentum().perp()>40.) {
          _count_OS_SR2->fill(0.5,weight);
        }
        // OS-SR3
        if(eTmiss>100. && recon_jets.size()>=4 &&
           recon_jets[0].momentum().perp()>100. &&
           recon_jets[3].momentum().perp()>70.) {
          _count_OS_SR3->fill(0.5,weight);
        }
        // same flavour analysis
        static const double beta   = 0.75;
        static const double tau_e  = 0.96;
        static const double tau_mu = 0.816;
        double fs_weight = weight;
        if(abs(recon_leptons[0].pdgId())==PID::ELECTRON && abs(recon_leptons[1].pdgId())==PID::ELECTRON) {
          fs_weight /= beta*(1.-sqr(1.-tau_e));
        }
        else if(abs(recon_leptons[0].pdgId())==PID::MUON && abs(recon_leptons[1].pdgId())==PID::MUON) {
          fs_weight *= beta/(1.-sqr(1.-tau_mu));
        }
        else {
          fs_weight /= -(1.-(1.-tau_e)*(1.-tau_mu));
        }
        // FS-SR1
        if(eTmiss>80.&& (mll<80.||mll>100.)) {
          _count_FS_SR1->fill(0.5,fs_weight);
        }
        // FS-SR2
        if(eTmiss>80.&&recon_jets.size()>=2) {
          _count_FS_SR2->fill(0.5,fs_weight);
        }
        // FS-SR3
        if(eTmiss>250.) {
          _count_FS_SR3->fill(0.5,fs_weight);
        }
      }
    }

    //@}


    void finalize() {

      double norm = crossSection()/femtobarn*1.04/sumOfWeights();
      // event counts
      scale(_count_OS_SR1,norm);
      scale(_count_OS_SR2,norm);
      scale(_count_OS_SR3,norm);
      scale(_count_SS_SR1,norm);
      scale(_count_SS_SR2,norm);
      scale(_count_FS_SR1,norm);
      scale(_count_FS_SR2,norm);
      scale(_count_FS_SR3,norm);
      // histograms
      scale(_hist_mll_SS_D     ,norm*20.);
      scale(_hist_mll_SS_B     ,norm*20.);
      scale(_hist_eTmiss_SS_D  ,norm*20.);
      scale(_hist_eTmiss_SS_B  ,norm*20.);
      scale(_hist_mll_SS_2Jet_D,norm*50.);
      scale(_hist_mll_SS_2Jet_B,norm*50.);
      scale(_hist_njet_SS_D    ,norm    );
      scale(_hist_njet_SS_B    ,norm    );
      scale(_hist_pT_j1_SS_D   ,norm*20.);
      scale(_hist_pT_j1_SS_B   ,norm*20.);
      scale(_hist_pT_j2_SS_D   ,norm*20.);
      scale(_hist_pT_j2_SS_B   ,norm*20.);
      scale(_hist_pT_l1_SS_D   ,norm*5. );
      scale(_hist_pT_l1_SS_B   ,norm*5. );
      scale(_hist_pT_l2_SS_D   ,norm*5. );
      scale(_hist_pT_l2_SS_B   ,norm*5. );

      scale(_hist_mll_OS_D        ,norm*10.);
      scale(_hist_mll_OS_B        ,norm*10.);
      scale(_hist_eTmiss_OS_D     ,norm*10.);
      scale(_hist_eTmiss_OS_B     ,norm*10.);
      scale(_hist_eTmiss_3Jet_OS_D,norm*10.);
      scale(_hist_eTmiss_3Jet_OS_B,norm*10.);
      scale(_hist_eTmiss_4Jet_OS_D,norm*10.);
      scale(_hist_eTmiss_4Jet_OS_B,norm*10.);
      scale(_hist_njet_OS_D       ,norm    );
      scale(_hist_njet_OS_B       ,norm    );
      scale(_hist_pT_j1_OS_D      ,norm*20.);
      scale(_hist_pT_j1_OS_B      ,norm*20.);
      scale(_hist_pT_j2_OS_D      ,norm*20.);
      scale(_hist_pT_j2_OS_B      ,norm*20.);
      scale(_hist_pT_l1_OS_D      ,norm*20.);
      scale(_hist_pT_l1_OS_B      ,norm*20.);
      scale(_hist_pT_l2_OS_D      ,norm*20.);
      scale(_hist_pT_l2_OS_B      ,norm*20.);
    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_OS_SR1;
    Histo1DPtr _count_OS_SR2;
    Histo1DPtr _count_OS_SR3;
    Histo1DPtr _count_SS_SR1;
    Histo1DPtr _count_SS_SR2;
    Histo1DPtr _count_FS_SR1;
    Histo1DPtr _count_FS_SR2;
    Histo1DPtr _count_FS_SR3;

    Histo1DPtr _hist_mll_SS_D;
    Histo1DPtr _hist_mll_SS_B;
    Histo1DPtr _hist_eTmiss_SS_D;
    Histo1DPtr _hist_eTmiss_SS_B;
    Histo1DPtr _hist_mll_SS_2Jet_D;
    Histo1DPtr _hist_mll_SS_2Jet_B;
    Histo1DPtr _hist_njet_SS_D;
    Histo1DPtr _hist_njet_SS_B;
    Histo1DPtr _hist_pT_j1_SS_D;
    Histo1DPtr _hist_pT_j1_SS_B;
    Histo1DPtr _hist_pT_j2_SS_D;
    Histo1DPtr _hist_pT_j2_SS_B;
    Histo1DPtr _hist_pT_l1_SS_D;
    Histo1DPtr _hist_pT_l1_SS_B;
    Histo1DPtr _hist_pT_l2_SS_D;
    Histo1DPtr _hist_pT_l2_SS_B;

    Histo1DPtr _hist_mll_OS_D;
    Histo1DPtr _hist_mll_OS_B;
    Histo1DPtr _hist_eTmiss_OS_D;
    Histo1DPtr _hist_eTmiss_OS_B;
    Histo1DPtr _hist_eTmiss_3Jet_OS_D;
    Histo1DPtr _hist_eTmiss_3Jet_OS_B;
    Histo1DPtr _hist_eTmiss_4Jet_OS_D;
    Histo1DPtr _hist_eTmiss_4Jet_OS_B;
    Histo1DPtr _hist_njet_OS_D ;
    Histo1DPtr _hist_njet_OS_B ;
    Histo1DPtr _hist_pT_j1_OS_D;
    Histo1DPtr _hist_pT_j1_OS_B;
    Histo1DPtr _hist_pT_j2_OS_D;
    Histo1DPtr _hist_pT_j2_OS_B;
    Histo1DPtr _hist_pT_l1_OS_D;
    Histo1DPtr _hist_pT_l1_OS_B;
    Histo1DPtr _hist_pT_l2_OS_D;
    Histo1DPtr _hist_pT_l2_OS_B;
    //@}
  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I943401);

}

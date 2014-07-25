// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Tools/RivetMT2.hh"

namespace Rivet {


  class ATLAS_2012_I1186556 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2012_I1186556()
      : Analysis("ATLAS_2012_I1186556")
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
      IdentifiedFinalState elecs(eta_e, 20.0*GeV);
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
      addProjection(ChargedFinalState(-3.0,3.0,1.*GeV),"cfs");

      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");

      // Book histograms
      _count_SR_SF     = bookHisto1D("count_SR_SF"    , 1, 0., 1.);
      _count_SR_OF     = bookHisto1D("count_SR_OF"    , 1, 0., 1.);

      _hist_mT2_SF_exp = bookHisto1D("hist_mT2_SF_exp", 40 , 0., 200. );
      _hist_mT2_OF_exp = bookHisto1D("hist_mT2_OF_exp", 40 , 0., 200. );
      _hist_mT2_SF_MC  = bookHisto1D("hist_mT2_SF_MC" , 500, 0., 1000.);
      _hist_mT2_OF_MC  = bookHisto1D("hist_mT2_OF_MC" , 500, 0., 1000.);

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
	if ( e_near_jet ) continue;
	cand_e.push_back(e);
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
        if ( mu_near_jet ) continue;
	cand_mu.push_back(mu);
      }
      // pTcone around muon track
      Particles recon_mu;
      foreach ( const Particle & mu, cand_mu ) {
        double pTinCone = -mu.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(mu.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 1.8*GeV ) recon_mu.push_back(mu);
      }
      // pTcone around electron track
      Particles recon_e;
      foreach ( const Particle & e, cand_e ) {
        double pTinCone = -e.pT();
        foreach ( const Particle & track, chg_tracks ) {
          if ( deltaR(e.momentum(),track.momentum()) < 0.2 )
            pTinCone += track.pT();
        }
        if ( pTinCone < 0.1 * e.pT() ) recon_e.push_back(e);
      }

      // pTmiss
      FourMomentum pTmiss;
      foreach ( const Particle & p,
                applyProjection<VisibleFinalState>(event, "vfs").particles() ) {
        pTmiss -= p.momentum();
      }

      // discard jets that overlap with electrons
      Jets recon_jets;
      foreach ( const Jet& jet, cand_jets ) {
        if(fabs(jet.eta())>2.5||
           jet.momentum().perp()<20.) continue;
	bool away_from_e = true;
	foreach ( const Particle & e, cand_e ) {
	  if ( deltaR(e.momentum(),jet.momentum()) < 0.2 ) {
	    away_from_e = false;
	    break;
	  }
	}
	if ( away_from_e ) recon_jets.push_back( jet );
      }

      // put leptons into 1 vector and order by pT
      Particles leptons(recon_e.begin(),recon_e.end());
      leptons.insert(leptons.begin(),recon_mu.begin(),recon_mu.end());
      sort(leptons.begin(),leptons.end(),cmpMomByPt);

      // exactly two leptons
      if(leptons.size() !=2) vetoEvent;

      // hardest lepton pT greater the 25 (20) e(mu)
      if( (abs(leptons[0].pdgId())==PID::ELECTRON && leptons[0].momentum().perp()<25.) ||
	  (abs(leptons[0].pdgId())==PID::ELECTRON && leptons[0].momentum().perp()<20.))
	vetoEvent;

      // require opposite sign
      if(leptons[0].pdgId()*leptons[1].pdgId()>0) vetoEvent;

      // and invariant mass > 20
      double mll = (leptons[0].momentum()+leptons[1].momentum()).mass();
      if(mll<20.) vetoEvent;

      // two jets 1st pT > 50 and second pT> 25
      if(recon_jets.size()<2 || recon_jets[0].momentum().perp()<50. ||
	 recon_jets[1].momentum().perp()<25.) vetoEvent;

      // calculate mT2
      double m_T2 = mT2::mT2( leptons[0].momentum(),leptons[1].momentum(),
			      pTmiss,0.0 ); // zero mass invisibles

      // same flavour region
      if(leptons[0].pdgId()==-leptons[1].pdgId()) {
	// remove Z region
	if(mll>71.&&mll<111.) vetoEvent;
	// require at least 1 b jet
	unsigned int n_b=0;
	for(unsigned int ix=0;ix<recon_jets.size();++ix) {
	   if(recon_jets[ix].containsBottom() && rand()/static_cast<double>(RAND_MAX)<=0.60)
	     ++n_b;
	}
	if(n_b==0) vetoEvent;
	_hist_mT2_SF_exp->fill(m_T2,weight);
	_hist_mT2_SF_MC ->fill(m_T2,weight);
	if(m_T2>120.) _count_SR_SF->fill(0.5,weight);
      }
      // opposite flavour region
      else {
	_hist_mT2_OF_exp->fill(m_T2,weight);
	_hist_mT2_OF_MC ->fill(m_T2,weight);
	if(m_T2>120.) _count_SR_OF->fill(0.5,weight);
      }
    }
    //@}


    void finalize() {

      double norm = 4.7* crossSection()/sumOfWeights()/femtobarn;
      scale(_count_SR_SF    ,   norm);
      scale(_count_SR_OF    ,   norm);
      scale(_hist_mT2_SF_exp,5.*norm);
      scale(_hist_mT2_OF_exp,5.*norm);
      scale(_hist_mT2_SF_MC ,   norm/4.7);
      scale(_hist_mT2_OF_MC ,   norm/4.7);

    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_SR_SF;
    Histo1DPtr _count_SR_OF;

    Histo1DPtr _hist_mT2_SF_exp;
    Histo1DPtr _hist_mT2_OF_exp;
    Histo1DPtr _hist_mT2_SF_MC;
    Histo1DPtr _hist_mT2_OF_MC;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1186556);

}

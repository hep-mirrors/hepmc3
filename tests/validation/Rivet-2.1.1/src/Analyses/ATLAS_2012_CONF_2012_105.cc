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


  class ATLAS_2012_CONF_2012_105 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor

    ATLAS_2012_CONF_2012_105()
      : Analysis("ATLAS_2012_CONF_2012_105")
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
      addProjection(VisibleFinalState(-4.5,4.5),"vfs");

      // book histograms

      // counts in signal regions
      _count_ee   = bookHisto1D("count_ee"  , 1, 0., 1.);
      _count_emu  = bookHisto1D("count_emu" , 1, 0., 1.);
      _count_mumu = bookHisto1D("count_mumu", 1, 0., 1.);
      _count_ll   = bookHisto1D("count_ll"  , 1, 0., 1.);

      // histograms from paper
      _hist_eTmiss_ee   = bookHisto1D("eTmiss_ee"  , 8, 0., 400.);
      _hist_eTmiss_emu  = bookHisto1D("eTmiss_emu" , 8, 0., 400.);
      _hist_eTmiss_mumu = bookHisto1D("eTmiss_mumu", 8, 0., 400.);
      _hist_eTmiss_ll   = bookHisto1D("eTmiss_ll"  , 8, 0., 400.);
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
      Particles recon_leptons;
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
          recon_leptons.push_back(e);
      }

      // Reconstructed Muons
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
          recon_leptons.push_back(mu);
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
      if ( recon_leptons.size() != 2) vetoEvent;
      // ensure 1st hardest
      if(recon_leptons[0].momentum().perp()<recon_leptons[1].momentum().perp())
        std::swap(recon_leptons[0],recon_leptons[1]);
      // only keep same sign
      if(recon_leptons[0].pdgId()*recon_leptons[1].pdgId()<0)
        vetoEvent;
      // at least 4 jets pt>50
      if(recon_jets.size()<4||recon_jets[3].momentum().perp()<50.)
        vetoEvent;

      if(recon_leptons[0].pdgId()!=recon_leptons[1].pdgId())
        _hist_eTmiss_emu ->fill(eTmiss,weight);
      else if(abs(recon_leptons[0].pdgId())==PID::ELECTRON)
        _hist_eTmiss_ee ->fill(eTmiss,weight);
      else if(abs(recon_leptons[0].pdgId())==PID::MUON)
        _hist_eTmiss_mumu->fill(eTmiss,weight);
      _hist_eTmiss_ll->fill(eTmiss,weight);

      if(eTmiss>150.) {
        if(recon_leptons[0].pdgId()!=recon_leptons[1].pdgId())
          _count_emu ->fill(0.5,weight);
        else if(abs(recon_leptons[0].pdgId())==PID::ELECTRON)
          _count_ee  ->fill(0.5,weight);
        else if(abs(recon_leptons[0].pdgId())==PID::MUON)
          _count_mumu->fill(0.5,weight);
        _count_ll->fill(0.5,weight);
      }

    }

    //@}


    void finalize() {

      double norm = crossSection()/femtobarn*5.8/sumOfWeights();
      // event counts
      scale(_count_ee  ,norm);
      scale(_count_emu ,norm);
      scale(_count_mumu,norm);
      scale(_count_ll  ,norm);
      // histograms
      scale(_hist_eTmiss_ee  ,norm*50.);
      scale(_hist_eTmiss_emu ,norm*50.);
      scale(_hist_eTmiss_mumu,norm*50.);
      scale(_hist_eTmiss_ll  ,norm*50.);

    }

  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_ee  ;
    Histo1DPtr _count_emu ;
    Histo1DPtr _count_mumu;
    Histo1DPtr _count_ll  ;

    Histo1DPtr _hist_eTmiss_ee;
    Histo1DPtr _hist_eTmiss_emu;
    Histo1DPtr _hist_eTmiss_mumu;
    Histo1DPtr _hist_eTmiss_ll;
    //@}
  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_CONF_2012_105);

}

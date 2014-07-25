// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
// #include "Rivet/Tools/RivetMT2.hh"

namespace Rivet {


  class ATLAS_2011_CONF_2011_098 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2011_CONF_2011_098()
      : Analysis("ATLAS_2011_CONF_2011_098"),
	//debug variables
	threeJA(0), threeJB(0), threeJC(0), threeJD(0), bj(0), jets(0), zerolept(0), eTmisscut(0)
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

      /// Jet finder
      addProjection(FastJets(FinalState(), FastJets::ANTIKT, 0.4),
                    "AntiKtJets04");


      // all tracks (to do deltaR with leptons)
      addProjection(ChargedFinalState(-3.0,3.0),"cfs");

      // for pTmiss
      addProjection(VisibleFinalState(-4.9,4.9),"vfs");


      /// Book histograms
      _count_threeJA     = bookHisto1D("count_threeJA", 1, 0., 1.);
      _count_threeJB     = bookHisto1D("count_threeJB", 1, 0., 1.);
      _count_threeJC     = bookHisto1D("count_threeJC", 1, 0., 1.);
      _count_threeJD     = bookHisto1D("count_threeJD", 1, 0., 1.);
      _hist_meff_1bjet   = bookHisto1D("meff_1bjet", 32, 0., 1600.);
      _hist_eTmiss_1bjet = bookHisto1D("eTmiss_1bjet", 6, 0., 600.);
      _hist_pTj_1bjet    = bookHisto1D("pTjet_1bjet", 20, 0., 800.);
      _hist_meff_2bjet   = bookHisto1D("meff_2bjet", 32, 0., 1600.);
      _hist_eTmiss_2bjet = bookHisto1D("eTmiss_2bjet", 6, 0., 600.);
      _hist_pTj_2bjet    = bookHisto1D("pTjet_2bjet", 20, 0., 800.);


    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

      // Temp: calorimeter module failure with 10% acceptance loss;
      // region unknown ==> randomly choose 10% of events to be vetoed

      if ( rand()/static_cast<double>(RAND_MAX) < 0.1 )
        vetoEvent;

      Jets tmp_cand_jets;
      foreach (const Jet& jet,
               applyProjection<FastJets>(event, "AntiKtJets04").jetsByPt(20.0*GeV) ) {
        if ( fabs( jet.eta() ) < 2.8 ) {
          tmp_cand_jets.push_back(jet);
        }
      }

      Particles cand_e =
        applyProjection<IdentifiedFinalState>(event, "elecs").particlesByPt();
      Particles cand_mu =
        applyProjection<IdentifiedFinalState>(event, "muons").particlesByPt();
      Particles chg_tracks =
        applyProjection<ChargedFinalState>(event, "cfs").particles();

//cerr << "cand_e.size(): " << cand_e.size() << "   cand_mu.size(): " << cand_mu.size() << '\n';


      Jets cand_jets;
      foreach ( const Jet& jet, tmp_cand_jets ) {
        if ( fabs( jet.eta() ) >= 2.8 )
          cand_jets.push_back( jet );
        else {
          bool away_from_e = true;
          foreach ( const Particle & e, cand_e ) {
            if ( deltaR(e.momentum(),jet.momentum()) <= 0.2 ) {
              away_from_e = false;
              break;
            }
          }
          if ( away_from_e )
            cand_jets.push_back( jet );
        }
      }

      Particles cand_lept;

      bool isolated_e;
      foreach ( const Particle & e, cand_e ) {
        isolated_e = true;
        foreach ( const Jet& jet, cand_jets ) {
          if ( deltaR(e.momentum(),jet.momentum()) < 0.4 )
            isolated_e = false;
        }
        if ( isolated_e == true )
          cand_lept.push_back( e );
      }


      bool isolated_mu;
      foreach ( const Particle & mu, cand_mu ) {
	isolated_mu = true;
	foreach ( const Jet& jet, cand_jets ) {
	  if ( deltaR(mu.momentum(),jet.momentum()) < 0.4 )
	    isolated_mu = false;
        }
        if ( isolated_mu == true)
          cand_lept.push_back( mu );
      }


      // pTmiss
      Particles vfs_particles
        = applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum pTmiss;
      foreach ( const Particle & p, vfs_particles ) {
        pTmiss -= p.momentum();
      }
      double eTmiss = pTmiss.pT();


      // bjets
      Jets bjets,recon_jets;
      foreach (const Jet& j, cand_jets) {
	if(fabs( j.eta() ) <= 2.8) {
	  recon_jets.push_back(j);
	  if ( fabs( j.eta() ) <= 2.5 && j.momentum().perp()>50. &&
	       j.containsBottom() && rand()/static_cast<double>(RAND_MAX) < 0.5 )
	    bjets.push_back(j);
	}
      }

      if (bjets.empty())  {
        MSG_DEBUG("No b-jet axes in acceptance");
        vetoEvent;
      }

      ++bj;



      // Jets event selection
      if ( recon_jets.size() < 3 )
        vetoEvent;
      if ( recon_jets[0].pT() <= 130*GeV )
        vetoEvent;
      if ( recon_jets[1].pT() <= 50*GeV ||
	   recon_jets[2].pT() <= 50*GeV )
        vetoEvent;
      ++jets;

      // eTmiss cut
      if ( eTmiss <= 130*GeV )
        vetoEvent;

      ++eTmisscut;

      // 0-lepton requirement
      if ( !cand_lept.empty() )
        vetoEvent;
      ++zerolept;

      // m_eff cut
      double m_eff = eTmiss
        + recon_jets[0].pT()
        + recon_jets[1].pT()
        + recon_jets[2].pT();

      if ( eTmiss / m_eff <= 0.25 )
        vetoEvent;


      // min_dPhi
      double min_dPhi = 999.999;
      for ( int i = 0; i < 3; ++i ) {
        double dPhi = deltaPhi( pTmiss.phi(), recon_jets[i].momentum().phi() );
        min_dPhi = min( min_dPhi, dPhi );
      }

      if ( min_dPhi <= 0.4 )
        vetoEvent;



    // ==================== FILL ====================


      // 1 bjet
      if ( bjets.size() >= 1 ) {

        _hist_meff_1bjet->fill(m_eff, weight);
        _hist_eTmiss_1bjet->fill(eTmiss, weight);
        _hist_pTj_1bjet->fill(recon_jets[0].pT(), weight);

        // 3JA region
        if ( m_eff > 200*GeV ) {
	  ++threeJA;
        _count_threeJA->fill(0.5, weight);
        }

        // 3JB region
        if ( m_eff > 700*GeV ) {
	  ++threeJB;
        _count_threeJB->fill(0.5, weight);
        }
      }

      // 2 bjets
      if ( bjets.size() >= 2 ) {

        _hist_meff_2bjet->fill(m_eff, weight);
        _hist_eTmiss_2bjet->fill(eTmiss, weight);
        _hist_pTj_2bjet->fill(recon_jets[0].pT(), weight);

        // 3JC region
        if ( m_eff > 500*GeV ) {
	  ++threeJC;
          _count_threeJC->fill(0.5, weight);
        }

        // 3JD region
        if ( m_eff > 700*GeV ) {
	  ++threeJD;
          _count_threeJD->fill(0.5, weight);
        }
      }




    }

    //@}


    void finalize() {
      scale( _hist_meff_1bjet, 50. * 830. * crossSection()/sumOfWeights() );
      scale( _hist_eTmiss_1bjet, 100. * 830. * crossSection()/sumOfWeights() );
      scale( _hist_pTj_1bjet, 40. * 830. * crossSection()/sumOfWeights() );
      scale( _hist_meff_2bjet, 50. * 830. * crossSection()/sumOfWeights() );
      scale( _hist_eTmiss_2bjet, 100. * 830. * crossSection()/sumOfWeights() );
      scale( _hist_pTj_2bjet, 40. * 830. * crossSection()/sumOfWeights() );

// cerr<< '\n'<<'\n'
// << "Saw "
// << bj << " events aft bjets cut, "
// << jets << " events aft jet cuts, "
// << eTmisscut << " events aft eTmiss cut, "
// << zerolept << " events after 0-lept cut. "
// << '\n'
// << threeJA << " 3JA events, "
// << threeJB << " 3JB events, "
// << threeJC << " 3JC events, "
// << threeJD << " 3JD events. "
// << '\n'
// ;

    }


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _count_threeJA;
    Histo1DPtr _count_threeJB;
    Histo1DPtr _count_threeJC;
    Histo1DPtr _count_threeJD;
    Histo1DPtr _hist_meff_1bjet;
    Histo1DPtr _hist_eTmiss_1bjet;
    Histo1DPtr _hist_pTj_1bjet;
    Histo1DPtr _hist_meff_2bjet;
    Histo1DPtr _hist_eTmiss_2bjet;
    Histo1DPtr _hist_pTj_2bjet;

    //@}


// debug variables
int threeJA;
int threeJB;
int threeJC;
int threeJD;
int bj;
int jets;
int zerolept;
int eTmisscut;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_CONF_2011_098);

}

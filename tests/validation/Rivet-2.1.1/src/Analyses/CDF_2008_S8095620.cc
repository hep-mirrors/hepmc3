// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"

namespace Rivet {


  /// @brief CDF Run II Z + b-jet cross-section measurement
  class CDF_2008_S8095620 : public Analysis {
  public:

    /// Constructor.
    /// jet cuts: |eta| <= 1.5
    CDF_2008_S8095620()
      : Analysis("CDF_2008_S8095620"),
        _Rjet(0.7), _JetPtCut(20.), _JetEtaCut(1.5), _Lep1PtCut(18.), _Lep2PtCut(10.), _LepEtaCut(3.2),
        _sumWeightSelected(0.0)
    {
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      const FinalState fs(-3.2, 3.2);
      addProjection(fs, "FS");
      // Create a final state with any e+e- or mu+mu- pair with
      // invariant mass 76 -> 106 GeV and ET > 18 (Z decay products)
      vector<pair<PdgId,PdgId> > vids;
      vids.push_back(make_pair(PID::ELECTRON, PID::POSITRON));
      vids.push_back(make_pair(PID::MUON, PID::ANTIMUON));
      FinalState fs2(-3.2, 3.2);
      InvMassFinalState invfs(fs2, vids, 76*GeV, 106*GeV);
      addProjection(invfs, "INVFS");
      // Make a final state without the Z decay products for jet clustering
      VetoedFinalState vfs(fs);
      vfs.addVetoOnThisFinalState(invfs);
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::CDFMIDPOINT, 0.7), "Jets");

      // Book histograms
      _dStot    = bookHisto1D(1, 1, 1);
      _dSdET    = bookHisto1D(2, 1, 1);
      _dSdETA   = bookHisto1D(3, 1, 1);
      _dSdZpT   = bookHisto1D(4, 1, 1);
      _dSdNJet  = bookHisto1D(5, 1, 1);
      _dSdNbJet = bookHisto1D(6, 1, 1);
     }


    // Do the analysis
    void analyze(const Event& event) {
      // Check we have an l+l- pair that passes the kinematic cuts
      // Get the Z decay products (mu+mu- or e+e- pair)
      const InvMassFinalState& invMassFinalState = applyProjection<InvMassFinalState>(event, "INVFS");
      const Particles&  ZDecayProducts =  invMassFinalState.particles();

      // make sure we have 2 Z decay products (mumu or ee)
      if (ZDecayProducts.size() < 2) vetoEvent;
      //new cuts
      double Lep1Pt = ZDecayProducts[0].momentum().perp();
      double Lep2Pt = ZDecayProducts[1].momentum().perp();
      double Lep1Eta = fabs(ZDecayProducts[0].rapidity());
      double Lep2Eta = fabs(ZDecayProducts[1].rapidity());

      if (Lep1Eta > _LepEtaCut || Lep2Eta > _LepEtaCut) vetoEvent;

      if (abs(ZDecayProducts[0].pdgId())==13 &&
          ((Lep1Eta > 1.5 || Lep2Eta > 1.5) || (Lep1Eta > 1.0 && Lep2Eta > 1.0))) {
        vetoEvent;
      }

      if (Lep1Pt > Lep2Pt) {
        if (Lep1Pt < _Lep1PtCut || Lep2Pt < _Lep2PtCut) vetoEvent;
      }
      else {
        if (Lep1Pt < _Lep2PtCut || Lep2Pt < _Lep1PtCut) vetoEvent;
      }

      _sumWeightSelected += event.weight();
      /// @todo: write out a warning if there are more than two decay products
      FourMomentum Zmom = ZDecayProducts[0].momentum() +  ZDecayProducts[1].momentum();

      // Put all b-quarks in a vector
      /// @todo Use a b-hadron search rather than b-quarks for tagging
      Particles bquarks;
      foreach (const GenParticlePtr p, particles(event.genEvent())) {
        if (fabs(p->pdg_id()) == PID::BQUARK) {
          bquarks += Particle(*p);
        }
      }

      // Get jets
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      MSG_DEBUG("Jet multiplicity before any pT cut = " << jetpro.size());

      const PseudoJets& jets = jetpro.pseudoJetsByPt();
      MSG_DEBUG("jetlist size = " << jets.size());

      int numBJet = 0;
      int numJet  = 0;
      // for each b-jet plot the ET and the eta of the jet, normalise to the total cross section at the end
      // for each event plot N jet and pT(Z), normalise to the total cross section at the end
      for (PseudoJets::const_iterator jt = jets.begin(); jt != jets.end(); ++jt) {
        // select jets that pass the kinematic cuts
        if (jt->perp() > _JetPtCut && fabs(jt->rapidity()) <= _JetEtaCut) {
          numJet++;
          // does the jet contain a b-quark?
          bool bjet = false;
          foreach (const Particle& bquark,  bquarks) {
            if (deltaR(jt->rapidity(), jt->phi(), bquark.rapidity(),bquark.momentum().azimuthalAngle()) <= _Rjet) {
              bjet = true;
              break;
            }
          } // end loop around b-jets
          if (bjet) {
            numBJet++;
            _dSdET->fill(jt->perp(),event.weight());
            _dSdETA->fill(fabs(jt->rapidity()),event.weight());
          }
        }
      } // end loop around jets

      // wasn't asking for b-jets before!!!!
      if(numJet > 0 && numBJet > 0) _dSdNJet->fill(numJet,event.weight());
      if(numBJet > 0) {
        _dStot->fill(1960.0,event.weight());
        _dSdNbJet->fill(numBJet,event.weight());
        _dSdZpT->fill(Zmom.pT(),event.weight());
      }
    }



    // Finalize
    void finalize() {
      // normalise histograms
      // scale by 1 / the sum-of-weights of events that pass the Z cuts
      // since the cross sections are normalized to the inclusive
      // Z cross sections.
      double Scale = 1.0;
      if (_sumWeightSelected != 0.0) Scale = 1.0/_sumWeightSelected;
      scale(_dStot,Scale);
      scale(_dSdET,Scale);
      scale(_dSdETA,Scale);
      scale(_dSdNJet,Scale);
      scale(_dSdNbJet,Scale);
      scale(_dSdZpT,Scale);
    }

    //@}


  private:

    double _Rjet;
    double _JetPtCut;
    double _JetEtaCut;
    double _Lep1PtCut;
    double _Lep2PtCut;
    double _LepEtaCut;
    double _sumWeightSelected;

    //@{
    /// Histograms
    Histo1DPtr _dStot;
    Histo1DPtr _dSdET;
    Histo1DPtr _dSdETA;
    Histo1DPtr _dSdNJet;
    Histo1DPtr _dSdNbJet;
    Histo1DPtr _dSdZpT;

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2008_S8095620);

}

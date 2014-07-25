// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class ALICE_2011_S8945144 : public Analysis {
  public:

    ALICE_2011_S8945144()
      : Analysis("ALICE_2011_S8945144")
    {}


  public:

    void init() {
      const ChargedFinalState cfs(-15, 15);
      addProjection(cfs, "CFS");

      _histPtPions        = bookHisto1D("d01-x01-y01");
      _histPtAntiPions    = bookHisto1D("d01-x01-y02");
      _histPtKaons        = bookHisto1D("d02-x01-y01");
      _histPtAntiKaons    = bookHisto1D("d02-x01-y02");
      _histPtProtons      = bookHisto1D("d03-x01-y01");
      _histPtAntiProtons  = bookHisto1D("d03-x01-y02");
      _histAveragePt      = bookProfile1D("d04-x01-y01");
    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(event, "CFS");
      foreach (const Particle& p, cfs.particles()) {
        if(fabs(p.rapidity())<0.5) {
          switch (p.pdgId()) {
            case 211:
              _histPtPions->fill(p.pT()/GeV, weight);
              _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              break;
            case -211:
              _histPtAntiPions->fill(p.pT()/GeV, weight);
              _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              break;
            case 2212:
              if ( !(p.hasAncestor(3322) ||                             // Xi0
                     p.hasAncestor(3122) || p.hasAncestor(-3122) ||     // Lambda
                     p.hasAncestor(3222) || p.hasAncestor(-3222) ||     // Sigma+/-
                     p.hasAncestor(3312) || p.hasAncestor(-3312) ) ) {  // Xi-/+
                _histPtProtons->fill(p.pT()/GeV, weight);
                _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              }
              break;
            case -2212:
              if ( !(p.hasAncestor(3322) ||                             // Xi0
                     p.hasAncestor(3122) || p.hasAncestor(-3122) ||     // Lambda
                     p.hasAncestor(3222) || p.hasAncestor(-3222) ||     // Sigma+/-
                     p.hasAncestor(3312) || p.hasAncestor(-3312) ) ) {  // Xi-/+
                _histPtAntiProtons->fill(p.pT()/GeV, weight);
                _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              }
              break;
            case 321:
              _histPtKaons->fill(p.pT()/GeV, weight);
              _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              break;
            case -321:
              _histPtAntiKaons->fill(p.pT()/GeV, weight);
              _histAveragePt->fill(p.mass()/GeV, p.pT()/GeV, weight);
              break;
          }
        }
      }
    }


    void finalize() {
      scale(_histPtPions,       1./sumOfWeights());
      scale(_histPtProtons,     1./sumOfWeights());
      scale(_histPtKaons,       1./sumOfWeights());
      scale(_histPtAntiPions,   1./sumOfWeights());
      scale(_histPtAntiProtons, 1./sumOfWeights());
      scale(_histPtAntiKaons,   1./sumOfWeights());
    }


  private:

    Histo1DPtr _histPtPions;
    Histo1DPtr _histPtProtons;
    Histo1DPtr _histPtKaons;
    Histo1DPtr _histPtAntiPions;
    Histo1DPtr _histPtAntiProtons;
    Histo1DPtr _histPtAntiKaons;
    Profile1DPtr _histAveragePt;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALICE_2011_S8945144);

}

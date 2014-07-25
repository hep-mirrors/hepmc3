// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/UnstableFinalState.hh"

namespace Rivet {


  class ATLAS_2012_I1082009 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2012_I1082009()
      : Analysis("ATLAS_2012_I1082009"),
        _weight25_30(0.),_weight30_40(0.),_weight40_50(0.),
        _weight50_60(0.),_weight60_70(0.),_weight25_70(0.)
    {    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Input for the jets: No neutrinos, no muons
      VetoedFinalState veto;
      veto.addVetoPairId(PID::MUON);
      veto.vetoNeutrinos();
      FastJets jets(veto, FastJets::ANTIKT, 0.6);
      addProjection(jets, "jets");
      // unstable final-state for D*
      addProjection(UnstableFinalState(), "UFS");

      _h_pt25_30 = bookHisto1D( 8,1,1);
      _h_pt30_40 = bookHisto1D( 9,1,1);
      _h_pt40_50 = bookHisto1D(10,1,1);
      _h_pt50_60 = bookHisto1D(11,1,1);
      _h_pt60_70 = bookHisto1D(12,1,1);
      _h_pt25_70 = bookHisto1D(13,1,1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // get the jets
      Jets jets;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "jets").jetsByPt(25.0*GeV)) {
        if ( fabs(jet.eta()) < 2.5 ) jets.push_back(jet);
      }
      // get the D* mesons
      const UnstableFinalState& ufs = applyProjection<UnstableFinalState>(event, "UFS");
      Particles Dstar;
      foreach (const Particle& p, ufs.particles()) {
        const int id = abs(p.pdgId());
        if(id==413) Dstar.push_back(p);
      }

      // loop over the jobs
      foreach (const Jet& jet, jets ) {
        double perp = jet.momentum().perp();
        bool found = false;
        double z(0.);
        if(perp<25.||perp>70.) continue;
        foreach(const Particle & p, Dstar) {
          if(p.momentum().perp()<7.5) continue;
          if(deltaR(p, jet.momentum())<0.6) {
            Vector3 axis = jet.momentum().vector3().unit();
            z = axis.dot(p.momentum().vector3())/jet.momentum().E();
            if(z<0.3) continue;
            found = true;
            break;
          }
        }
        _weight25_70 += weight;
        if(found) _h_pt25_70->fill(z,weight);
        if(perp>=25.&&perp<30.) {
          _weight25_30 += weight;
          if(found) _h_pt25_30->fill(z,weight);
        }
        else if(perp>=30.&&perp<40.) {
          _weight30_40 += weight;
          if(found) _h_pt30_40->fill(z,weight);
        }
        else if(perp>=40.&&perp<50.) {
          _weight40_50 += weight;
          if(found) _h_pt40_50->fill(z,weight);
        }
        else if(perp>=50.&&perp<60.) {
          _weight50_60 += weight;
          if(found) _h_pt50_60->fill(z,weight);
        }
        else if(perp>=60.&&perp<70.) {
          _weight60_70 += weight;
          if(found) _h_pt60_70->fill(z,weight);
        }
      }
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_pt25_30,1./_weight25_30);
      scale(_h_pt30_40,1./_weight30_40);
      scale(_h_pt40_50,1./_weight40_50);
      scale(_h_pt50_60,1./_weight50_60);
      scale(_h_pt60_70,1./_weight60_70);
      scale(_h_pt25_70,1./_weight25_70);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    double _weight25_30,_weight30_40,_weight40_50;
    double _weight50_60,_weight60_70,_weight25_70;

    Histo1DPtr _h_pt25_30;
    Histo1DPtr _h_pt30_40;
    Histo1DPtr _h_pt40_50;
    Histo1DPtr _h_pt50_60;
    Histo1DPtr _h_pt60_70;
    Histo1DPtr _h_pt25_70;
    //@}

  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1082009);

}

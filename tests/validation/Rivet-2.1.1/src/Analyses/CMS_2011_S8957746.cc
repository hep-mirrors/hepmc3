// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/Thrust.hh"

namespace Rivet {

  /// Rivet analysis class for CMS_2011_S8957746 dataset
  class CMS_2011_S8957746 : public Analysis {
  public:

    /// Constructor
    CMS_2011_S8957746()
      : Analysis("CMS_2011_S8957746") {  }


    /// Initialization, called once before running
    void init() {
      // Projections
      const FastJets jets(FinalState(-5.0, 5.0, 0.0*GeV), FastJets::ANTIKT, 0.5);
      addProjection(jets, "Jets");

      // Book histograms
      _hist_T_90  = bookHisto1D(1, 1, 1);
      _hist_m_90  = bookHisto1D(2, 1, 1);
      _hist_T_125 = bookHisto1D(3, 1, 1);
      _hist_m_125 = bookHisto1D(4, 1, 1);
      _hist_T_200 = bookHisto1D(5, 1, 1);
      _hist_m_200 = bookHisto1D(6, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();
      const Jets& jets = applyProjection<FastJets>(event, "Jets").jetsByPt(30.0*GeV);
      if (jets.size() < 2 ||
          fabs(jets[0].eta()) >= 1.3 ||
          fabs(jets[1].eta()) >= 1.3 ||
          jets[0].pT() < 90*GeV) {
        vetoEvent;
      }
      std::vector<Vector3> momenta;
      foreach (const Jet& j, jets) {
        if (fabs(j.eta()) < 1.3) {
          Vector3 mom = j.momentum().vector3();
          mom.setZ(0.0);
          momenta.push_back(mom);
        }
      }
      if (momenta.size() == 2) {
        // We need to use a ghost so that Thrust.calc() doesn't return 1.
        momenta.push_back(Vector3(1e-10*MeV, 0., 0.));
      }
      Thrust thrust;
      thrust.calc(momenta);

      // The lowest bin also includes the underflow:
      const double T = max(log(1-thrust.thrust()), -12.0);
      const double M = max(log(thrust.thrustMajor()), -6.0);
      if (jets[0].pT()/GeV > 200) {
        _hist_T_200->fill(T, weight);
        _hist_m_200->fill(M, weight);
      } else if (jets[0].pT()/GeV > 125) {
        _hist_T_125->fill(T, weight);
        _hist_m_125->fill(M, weight);
      } else if (jets[0].pT()/GeV > 90) {
        _hist_T_90->fill(T, weight);
        _hist_m_90->fill(M, weight);
      }
    }


    void finalize() {
      normalize(_hist_T_90);
      normalize(_hist_m_90);
      normalize(_hist_T_125);
      normalize(_hist_m_125);
      normalize(_hist_T_200);
      normalize(_hist_m_200);
    }


  private:

    Histo1DPtr _hist_T_90;
    Histo1DPtr _hist_m_90;
    Histo1DPtr _hist_T_125;
    Histo1DPtr _hist_m_125;
    Histo1DPtr _hist_T_200;
    Histo1DPtr _hist_m_200;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S8957746);

}

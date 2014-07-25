// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/DISFinalState.hh"
#include "Rivet/Projections/CentralEtHCM.hh"

namespace Rivet {


  /// H1 energy flow in DIS
  ///
  /// @todo Make histograms match those in HepData and use autobooking
  /// @author Leif Lonnblad
  /// @author Andy Buckley
  class H1_1995_S3167097 : public Analysis {
  public:

    /// Constructor
    H1_1995_S3167097()
      : Analysis("H1_1995_S3167097")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      // Projections
      const DISKinematics& diskin = addProjection(DISKinematics(), "Kinematics");
      const DISFinalState& fshcm = addProjection(DISFinalState(diskin, DISFinalState::HCM), "FS");
      addProjection(CentralEtHCM(fshcm), "Y1HCM");

      // Histograms
      /// @todo Convert to use autobooking and correspond to HepData data tables
      _sumw.resize(9);
      _hEtFlow.resize(9);
      for (size_t i = 0; i < 9; ++i)
        _hEtFlow[i] = bookHisto1D(lexical_cast<string>(i), 24, -6, 6);
      _tmphAvEt = Histo1D(9, 1.0, 10.0);
      _tmphAvX  = Histo1D(9, 1.0, 10.0);
      _tmphAvQ2 = Histo1D(9, 1.0, 10.0);
      _tmphN    = Histo1D(9, 1.0, 10.0);
    }


    /// Calculate the bin number from the DISKinematics projection
    size_t _getbin(const DISKinematics& dk) {
      if (inRange(dk.Q2()/GeV2, 5.0, 10.0)) {
        if (inRange(dk.x(), 1e-4, 2e-4)) return 0;
        if (inRange(dk.x(), 2e-4, 5e-4) && dk.Q2() > 6.0*GeV2) return 1;
      } else if (inRange(dk.Q2()/GeV2, 10.0, 20.0)) {
        if (inRange(dk.x(), 2e-4, 5e-4)) return 2;
        if (inRange(dk.x(), 5e-4, 8e-4)) return 3;
        if (inRange(dk.x(), 8e-4, 1.5e-3)) return 4;
        if (inRange(dk.x(), 1.5e-3, 4e-3)) return 5;
      } else if (inRange(dk.Q2()/GeV2, 20.0, 50.0)) {
        if (inRange(dk.x(), 5e-4, 1.4e-3)) return 6;
        if (inRange(dk.x(), 1.4e-3, 3e-3)) return 7;
        if (inRange(dk.x(), 3e-3, 1e-2)) return 8;
      }
      return -1;
    }


    void analyze(const Event& event) {
      const FinalState& fs = applyProjection<FinalState>(event, "FS");
      const DISKinematics& dk = applyProjection<DISKinematics>(event, "Kinematics");
      const CentralEtHCM y1 = applyProjection<CentralEtHCM>(event, "Y1HCM");

      const int ibin = _getbin(dk);
      if (ibin < 0) vetoEvent;
      const double weight = event.weight();

      for (size_t i = 0, N = fs.particles().size(); i < N; ++i) {
        const double rap = fs.particles()[i].rapidity();
        const double et = fs.particles()[i].momentum().Et();
        _hEtFlow[ibin]->fill(rap, weight * et/GeV);
      }

      _sumw[ibin] += weight;
      _tmphAvEt.fill(ibin + 1.5, weight * y1.sumEt()/GeV);
      _tmphAvX.fill(ibin + 1.5, weight * dk.x());
      _tmphAvQ2.fill(ibin + 1.5, weight * dk.Q2()/GeV2);
      _tmphN.fill(ibin + 1.5, weight);
    }


    void finalize() {
      for (size_t ibin = 0; ibin < 9; ++ibin)
        scale(_hEtFlow[ibin], 0.5/_sumw[ibin]);
      /// @todo Improve this!
      addAnalysisObject(Scatter2DPtr( new Scatter2D(_tmphAvEt/_tmphN, histoPath("21")) ));
      addAnalysisObject(Scatter2DPtr( new Scatter2D(_tmphAvX/_tmphN,  histoPath("22")) ));
      addAnalysisObject(Scatter2DPtr( new Scatter2D(_tmphAvQ2/_tmphN, histoPath("23")) ));
    }

    //@}


  private:

    /// Histograms for the \f$ E_T \f$ flow
    vector<Histo1DPtr> _hEtFlow;

    /// Temporary histograms for averages in different kinematical bins.
    Histo1D _tmphAvEt, _tmphAvX, _tmphAvQ2, _tmphN;

    /// Weights counters for each kinematic bin
    vector<double> _sumw;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(H1_1995_S3167097);

}

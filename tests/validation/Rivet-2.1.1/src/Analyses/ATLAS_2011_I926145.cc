// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/ZFinder.hh"

namespace Rivet {

  /// @brief Measurement of electron and muon differential cross section from heavy flavour production
  ///
  /// lepton cross sections differential in pT
  ///
  /// @author Paul Bell, Holger Schulz


  class ATLAS_2011_I926145 : public Analysis {
  public:

    /// Constructor
    ATLAS_2011_I926145()
      : Analysis("ATLAS_2011_I926145")
    {
    }


  public:

    /// Book histograms and initialise projections before the run
    void init() {

      ///projection for electrons
      vector<pair<double, double> > eta_e;
      eta_e.push_back(std::make_pair(-2.00, -1.52));
      eta_e.push_back(std::make_pair(-1.37, 1.37));
      eta_e.push_back(std::make_pair(1.52, 2.00));
      IdentifiedFinalState elecs(eta_e, 7.0*GeV);
      elecs.acceptId(PID::ELECTRON);
      elecs.acceptId(PID::POSITRON);
      addProjection(elecs, "elecs");

      //projection for muons -- same phase space as above??? Not sure if the crack region has
      //to be removed for the muons as well
      std::vector<std::pair<double, double> > eta_m;
      //eta_m.push_back(make_pair(-2.00,-1.52));
      //eta_m.push_back(make_pair(-1.37,1.37));
      //eta_m.push_back(make_pair(1.52,2.00));
      //IdentifiedFinalState muons(eta_m, 7.0*GeV);
      IdentifiedFinalState muons(-2.0, 2.0, 7.0*GeV);
      muons.acceptId(PID::MUON);
      muons.acceptId(PID::ANTIMUON);
      addProjection(muons, "muons");

      // Projection for muons full range
      IdentifiedFinalState muons_full(-2.5, 2.5, 4.0*GeV);
      muons_full.acceptId(PID::MUON);
      muons_full.acceptId(PID::ANTIMUON);
      addProjection(muons_full, "muons_full");
      const FinalState fs20(-2.0, 2.0, 0.0*GeV);
      const FinalState fs25(-2.5, 2.5, 0.0*GeV);

      /// @todo Bare Zs ...
      ZFinder zfinder_e(fs20, -2.0, 2.0, 0.0*GeV, PID::ELECTRON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::NOCLUSTER);
      addProjection(zfinder_e, "ZFinder_e");
      ZFinder zfinder_mu(fs20, -2.0, 2.0, 0.0*GeV, PID::MUON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::NOCLUSTER);
      addProjection(zfinder_mu, "ZFinder_mu");
      ZFinder zfinder_mufull(fs25, -2.5, 2.5, 0.0*GeV, PID::MUON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::NOCLUSTER);
      addProjection(zfinder_mufull, "ZFinder_mufull");

      /// @todo ... but dressed Ws?
      WFinder wfinder_e(fs20, -2.0, 2.0, 0.0*GeV, PID::ELECTRON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wfinder_e, "WFinder_e");
      WFinder wfinder_mu(fs20, -2.0, 2.0, 0.0*GeV, PID::MUON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wfinder_mu, "WFinder_mu");
      WFinder wfinder_mufull(fs25, -2.5, 2.5, 0.0*GeV, PID::MUON, 60.0*GeV, 100.0*GeV, 25.0*GeV, 0.2);
      addProjection(wfinder_mufull, "WFinder_mufull");

      // Book histograms
      _histPt_elecs      = bookHisto1D(1 ,1 ,1);
      _histPt_muons      = bookHisto1D(2 ,1 ,1);
      _histPt_muons_full = bookHisto1D(3 ,1 ,1);
    }

    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      const FinalState& elecs      = applyProjection<FinalState>(event, "elecs");
      const FinalState& muons      = applyProjection<FinalState>(event, "muons");
      const FinalState& muons_full = applyProjection<FinalState>(event, "muons_full");

      // Veto event if no lepton is present
      if (elecs.size() == 0 && muons.size() == 0 && muons_full.size() == 0) {
        vetoEvent;
      }

      // Check for W and or Z bosons in event
      //
      // Z veto
      const ZFinder& zfinder_e      = applyProjection<ZFinder>(event, "ZFinder_e");
      const ZFinder& zfinder_mu     = applyProjection<ZFinder>(event, "ZFinder_mu");
      const ZFinder& zfinder_mufull = applyProjection<ZFinder>(event, "ZFinder_mufull");

      if (zfinder_e.bosons().size() > 0 || zfinder_mu.bosons().size() > 0 || zfinder_mufull.bosons().size() > 0) {
          MSG_DEBUG("Num elec Z-bosons found: " << zfinder_e.bosons().size());
          MSG_DEBUG("Num muon Z-bosons found: " << zfinder_mu.bosons().size());
          MSG_DEBUG("Num muon Z-bosons found (|eta|<2.5): " << zfinder_mufull.bosons().size());
          vetoEvent;
      }

      // W veto
      const WFinder& wfinder_e      = applyProjection<WFinder>(event, "WFinder_e");
      const WFinder& wfinder_mu     = applyProjection<WFinder>(event, "WFinder_mu");
      const WFinder& wfinder_mufull = applyProjection<WFinder>(event, "WFinder_mufull");

      if (wfinder_e.bosons().size() > 0 || wfinder_mu.bosons().size() > 0 || wfinder_mufull.bosons().size() > 0) {
          MSG_DEBUG("Num elec W-bosons found: " << wfinder_e.bosons().size());
          MSG_DEBUG("Num muon W-bosons found: " << wfinder_mu.bosons().size());
          MSG_DEBUG("Num muon W-bosons found (|eta|<2.5): " << wfinder_mufull.bosons().size());
          vetoEvent;
      }


      // Electron histogram
      if (elecs.size() > 0) {
        foreach (const Particle& ele, elecs.particles()) {
          if (ele.pT()*GeV < 26.0) {
            _histPt_elecs->fill(ele.pT()*GeV, weight);
          }
        }
      }

      // Muon histogram
      if (muons.size() > 0) {
        foreach (const Particle& muo, muons.particles()) {
          if (muo.pT()*GeV < 26.0) {
            _histPt_muons->fill(muo.pT()*GeV, weight);
          }
        }
      }

      // Muon full histogram
      if (muons_full.size() > 0) {
        foreach (const Particle& muo, muons_full.particles()) {
          if (muo.pT()*GeV < 100.0) {
            _histPt_muons_full->fill(muo.pT()*GeV, weight);
          }
        }
      }
    }

    /// Normalise histograms etc., after the run
    void finalize() {

      // Data cross-section is given in nb! x-sections in rivet are in pb!
      scale(_histPt_elecs,      crossSection()/nanobarn/sumOfWeights());
      scale(_histPt_muons,      crossSection()/nanobarn/sumOfWeights());
      scale(_histPt_muons_full, crossSection()/nanobarn/sumOfWeights());
    }



  private:

    /// @name Histograms
    Histo1DPtr _histPt_elecs;
    Histo1DPtr _histPt_muons;
    Histo1DPtr _histPt_muons_full;
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_I926145);

}

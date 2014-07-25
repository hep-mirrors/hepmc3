#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/Thrust.hh"

namespace Rivet {


  /// CMS Z+jets delta(phi) and jet thrust measurement at 7 TeV
  class CMS_2013_I1209721 : public Analysis {
  public:

    CMS_2013_I1209721()
      : Analysis("CMS_2013_I1209721")
    {    }


    /// Book projections and histograms
    void init() {
      // Full final state
      const FinalState fs(-5.0,5.0);
      addProjection(fs, "FS");
      // Z finders for electrons and muons
      const ZFinder zfe(fs, -2.4, 2.4, 20*GeV, PID::ELECTRON, 71*GeV, 111*GeV);
      const ZFinder zfm(fs, -2.4, 2.4, 20*GeV, PID::ELECTRON, 71*GeV, 111*GeV);
      addProjection(zfe, "ZFE");
      addProjection(zfm, "ZFM");
      // Jets
      const FastJets jets(fs, FastJets::ANTIKT, 0.5);
      addProjection(jets, "JETS");

      // Book histograms from data
      for (size_t i = 0; i < 2; ++i) {
        _histDeltaPhiZJ1_1[i]  = bookHisto1D(1, 1, 1);
        _histDeltaPhiZJ1_2[i]  = bookHisto1D(2, 1, 1);
        _histDeltaPhiZJ1_3[i]  = bookHisto1D(4, 1, 1);
        _histDeltaPhiZJ2_3[i]  = bookHisto1D(5, 1, 1);
        _histDeltaPhiZJ3_3[i]  = bookHisto1D(3, 1, 1);
        _histDeltaPhiJ1J2_3[i] = bookHisto1D(6, 1, 1);
        _histDeltaPhiJ1J3_3[i] = bookHisto1D(7, 1, 1);
        _histDeltaPhiJ2J3_3[i] = bookHisto1D(8, 1, 1);
        _histTransvThrust[i]   = bookHisto1D(9, 1, 1);
      }
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      // Apply the Z finders
      const ZFinder& zfe = applyProjection<ZFinder>(event, "ZFE");
      const ZFinder& zfm = applyProjection<ZFinder>(event, "ZFM");

      // Choose the Z candidate (there must be one)
      if (zfe.empty() && zfm.empty()) vetoEvent;
      const ParticleVector& z = !zfm.empty() ? zfm.bosons() : zfe.bosons();
      const ParticleVector& leptons = !zfm.empty() ? zfm.constituents() : zfe.constituents();

      // Determine whether we are in the boosted regime
      const bool is_boosted = (z[0].momentum().pT() > 150*GeV);

      // Build the jets
      const FastJets& jetfs = applyProjection<FastJets>(event, "JETS");
      const Jets& jets = jetfs.jetsByPt(50.*GeV, MAXDOUBLE, -2.5, 2.5);

      // Clean the jets against the lepton candidates, as in the paper, with a deltaR cut of 0.4 against the clustered leptons
      vector<const Jet*> cleanedJets;
      for (size_t i = 0; i < jets.size(); ++i) {
        bool isolated = true;
        for (size_t j = 0; j < 2; ++j) {
          if (deltaR(leptons[j], jets[i]) < 0.4) {
            isolated = false;
            break;
          }
        }
        if (isolated) cleanedJets.push_back(&jets[i]);
      }

      // Require at least 1 jet
      const unsigned int Njets = cleanedJets.size();
      if (Njets < 1) vetoEvent;

      // Now compute the thrust
      // Collect Z and jets transverse momenta to calculate transverse thrust
      vector<Vector3> momenta;
      momenta.clear();
      Vector3 mom = z[0].momentum().p();
      mom.setZ(0);
      momenta.push_back(mom);

      for (size_t i = 0; i < cleanedJets.size(); ++i) {
        Vector3 mj = cleanedJets[i]->momentum().vector3();
        mj.setZ(0);
        momenta.push_back(mj);
      }

      // Define a macro to appropriately fill both unboosted and boosted histo versions
      #define FILLx2(HNAME, VAL) do { double x = VAL; for (size_t i = 0; i < 2; ++i) { \
        if (i == 0 || is_boosted) HNAME[i]->fill(x, weight); } } while(0)

      Thrust thrust; thrust.calc(momenta);
      const double T = thrust.thrust();
      FILLx2(_histTransvThrust, log(max(1-T, 1e-6)));

      const double dphiZJ1 = deltaPhi(z[0], *cleanedJets[0]);
      FILLx2(_histDeltaPhiZJ1_1, dphiZJ1);
      if (Njets > 1) {
        FILLx2(_histDeltaPhiZJ1_2, dphiZJ1);
        if (Njets > 2) {
          FILLx2(_histDeltaPhiZJ1_3, dphiZJ1);
          FILLx2(_histDeltaPhiZJ2_3, deltaPhi(z[0], *cleanedJets[1]));
          FILLx2(_histDeltaPhiZJ3_3, deltaPhi(z[0], *cleanedJets[2]));
          FILLx2(_histDeltaPhiJ1J2_3, deltaPhi(*cleanedJets[0], *cleanedJets[1]));
          FILLx2(_histDeltaPhiJ1J3_3, deltaPhi(*cleanedJets[0], *cleanedJets[2]));
          FILLx2(_histDeltaPhiJ2J3_3, deltaPhi(*cleanedJets[1], *cleanedJets[2]));
        }
      }
    }


    /// Normalizations
    /// @note Most of these data normalizations neglect the overflow bins
    void finalize() {
      for (size_t i = 0; i < 2; ++i) {
        normalize(_histDeltaPhiZJ1_1[i], 1, false);
        normalize(_histDeltaPhiZJ1_2[i], 1, false);
        normalize(_histDeltaPhiZJ1_3[i], 1, false);
        normalize(_histDeltaPhiZJ2_3[i], 1, false);
        normalize(_histDeltaPhiZJ3_3[i], 1, false);
        normalize(_histDeltaPhiJ1J2_3[i], 1, false);
        normalize(_histDeltaPhiJ1J3_3[i], 1, false);
        normalize(_histDeltaPhiJ2J3_3[i], 1, false);
        normalize(_histTransvThrust[i]);
      }
    }


  private:

    // Arrays of unboosted/boosted histos
    Histo1DPtr _histDeltaPhiZJ1_1[2];
    Histo1DPtr _histDeltaPhiZJ1_2[2];
    Histo1DPtr _histDeltaPhiZJ1_3[2];
    Histo1DPtr _histDeltaPhiZJ2_3[2];
    Histo1DPtr _histDeltaPhiZJ3_3[2];
    Histo1DPtr _histDeltaPhiJ1J2_3[2];
    Histo1DPtr _histDeltaPhiJ1J3_3[2];
    Histo1DPtr _histDeltaPhiJ2J3_3[2];
    Histo1DPtr _histTransvThrust[2];

  };


  DECLARE_RIVET_PLUGIN(CMS_2013_I1209721);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
/// @todo These should be unnecessary from 2.2.0 onward when Rivet::Jet is rewritten
#include "fastjet/internal/base.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"

namespace Rivet {


  /// @brief Measurement of isolated diphoton + X differential cross-sections
  ///
  /// Inclusive isolated gamma gamma cross-sections, differential in M(gg), pT(gg),
  /// dphi(gg), cos(theta*)_CS
  ///
  /// @author Giovanni Marchiori
  ///
  class ATLAS_2012_I1199269 : public Analysis {
  public:

    /// Constructor
    ATLAS_2012_I1199269()
      : Analysis("ATLAS_2012_I1199269")
    {
      _eta_bins_areaoffset += 0.0, 1.5, 3.0;
    }


    /// Book histograms and initialise projections before the run
    void init() {

      FinalState fs;
      addProjection(fs, "FS");

      /// @todo Clean-up when jets are better
      FastJets fj(fs, FastJets::KT, 0.5);
      _area_def = new fastjet::AreaDefinition(fastjet::VoronoiAreaSpec());
      fj.useJetArea(_area_def);
      addProjection(fj, "KtJetsD05");

      IdentifiedFinalState photonfs(-2.37, 2.37, 22.0*GeV);
      photonfs.acceptId(PID::PHOTON);
      addProjection(photonfs, "Photon");

      _h_M            = bookHisto1D(1, 1, 1);
      _h_pT           = bookHisto1D(2, 1, 1);
      _h_dPhi         = bookHisto1D(3, 1, 1);
      _h_cosThetaStar = bookHisto1D(4, 1, 1);
    }


    // int getBinIndex(double x, const vector<double>& binedges) const {
    //   /// @todo Add linear and log bin guessing, use binary split if sufficiently large, etc.
    //   for (size_t i = 0; i < _binedges.size()-1; ++i)
    //     if (inRange(x, binedges[i], binedges[i+1])) return (int) i;
    //   return -1;
    // }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      /// Require at least 2 photons in final state
      const Particles photons = applyProjection<IdentifiedFinalState>(event, "Photon").particlesByPt();
      if (photons.size() < 2) vetoEvent;

      /// Compute the median energy density
      _ptDensity.clear();
      _sigma.clear();
      _Njets.clear();
      /// @todo Nicer way to do this... in C++11?
      vector<vector<double> > ptDensities;
      vector<double> emptyVec;
      ptDensities.assign(_eta_bins_areaoffset.size()-1, emptyVec);

      // Get jets, and corresponding jet areas
      const fastjet::ClusterSequenceArea* clust_seq_area = applyProjection<FastJets>(event, "KtJetsD05").clusterSeqArea();
      foreach (const fastjet::PseudoJet& jet, applyProjection<FastJets>(event, "KtJetsD05").pseudoJets(0.0*GeV)) {
        const double aeta = fabs(jet.eta());
        const double pt = jet.perp();
        const double area = clust_seq_area->area(jet);
        if (area < 1e-3) continue;
        const int ieta = binIndex(aeta, _eta_bins_areaoffset);
        if (ieta != -1) ptDensities[ieta].push_back(pt/area);
      }

      // Compute median jet properties over the jets in the event
      for (size_t b = 0; b < _eta_bins_areaoffset.size()-1; ++b) {
        double median = 0.0;
        double sigma = 0.0;
        int Njets = 0;
        if (ptDensities[b].size() > 0) {
          std::sort(ptDensities[b].begin(), ptDensities[b].end());
          int nDens = ptDensities[b].size();
          median = (nDens % 2 == 0) ? (ptDensities[b][nDens/2]+ptDensities[b][(nDens-2)/2])/2 : ptDensities[b][(nDens-1)/2];
          sigma = ptDensities[b][(int)(.15865*nDens)];
          Njets = nDens;
        }
        _ptDensity.push_back(median);
        _sigma.push_back(sigma);
        _Njets.push_back(Njets);
      }


      // Loop over photons and fill vector of isolated ones
      Particles isolated_photons;
      foreach (const Particle& photon, photons) {
        /// Remove photons in ECAL crack region
        if (inRange(photon.abseta(), 1.37, 1.52)) continue;
        const double eta_P = photon.eta();
        const double phi_P = photon.phi();

        // Compute isolation via particles within an R=0.4 cone of the photon
        Particles fs = applyProjection<FinalState>(event, "FS").particles();
        FourMomentum mom_in_EtCone;
        foreach (const Particle& p, fs) {
          // Reject if not in cone
          if (deltaR(photon.momentum(), p.momentum()) > 0.4) continue;
          // Reject if in the 5x7 cell central core
          if (fabs(eta_P - p.eta()) < 0.025 * 7 * 0.5 &&
              fabs(phi_P - p.phi()) < PI/128. * 5 * 0.5) continue;
          // Sum momentum
          mom_in_EtCone += p.momentum();
        }
        // Now figure out the correction (area*density)
        const double EtCone_area = PI*sqr(0.4) - (7*.025)*(5*PI/128.); // cone area - central core rectangle
        const double correction = _ptDensity[binIndex(fabs(eta_P), _eta_bins_areaoffset)] * EtCone_area;

        // Discard the photon if there is more than 4 GeV of cone activity
        // NOTE: Shouldn't need to subtract photon itself (it's in the central core)
        // NOTE: using expected cut at hadron/particle level, not at reco level
        if (mom_in_EtCone.Et() - correction > 4*GeV) continue;
        // Add isolated photon to list
        isolated_photons.push_back(photon);
      }

      // Require at least two isolated photons and select leading pT pair
      if (isolated_photons.size() < 2) vetoEvent;
      sortByPt(isolated_photons);
      FourMomentum y1 = isolated_photons[0].momentum();
      FourMomentum y2 = isolated_photons[1].momentum();

      // Leading photon should have pT > 25 GeV
      if (y1.pT() < 25*GeV) vetoEvent;

      // Require the two photons to be separated by dR > 0.4
      if (deltaR(y1, y2) < 0.4) vetoEvent;

      FourMomentum yy = y1 + y2;
      const double Myy = yy.mass();
      _h_M->fill(Myy/GeV, weight);
      const double pTyy = yy.pT();
      _h_pT->fill(pTyy/GeV, weight);
      const double dPhiyy = mapAngle0ToPi(y1.phi() - y2.phi());
      _h_dPhi->fill(dPhiyy, weight);
      const double costhetayy = 2 * y1.pT() * y2.pT() * sinh(y1.eta() - y2.eta()) / Myy / add_quad(Myy, pTyy);
      _h_cosThetaStar->fill(costhetayy, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_M, crossSection()/sumOfWeights());
      scale(_h_pT, crossSection()/sumOfWeights());
      scale(_h_dPhi, crossSection()/sumOfWeights());
      scale(_h_cosThetaStar, crossSection()/sumOfWeights());
    }


  private:

    Histo1DPtr _h_M, _h_pT, _h_dPhi, _h_cosThetaStar;

    fastjet::AreaDefinition* _area_def;

    vector<double> _eta_bins;
    vector<double> _eta_bins_areaoffset;

    vector<double> _ptDensity;
    vector<double> _sigma;
    vector<double> _Njets;
  };


  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1199269);

}

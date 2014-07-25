// -*- C++ -*-
#include <iostream>
#include <sstream>
#include <string>

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Jet.hh"
#include "Rivet/Projections/FastJets.hh"

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
  /// dphi(gg)
  ///
  /// @author Giovanni Marchiori
  class ATLAS_2011_S9120807 : public Analysis {
  public:

    /// Constructor
    ATLAS_2011_S9120807()
      : Analysis("ATLAS_2011_S9120807")
    {
      _eta_bins_areaoffset.push_back(0.0);
      _eta_bins_areaoffset.push_back(1.5);
      _eta_bins_areaoffset.push_back(3.0);
    }


  public:

    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs;
      addProjection(fs, "FS");

      FastJets fj(fs, FastJets::KT, 0.5);
      _area_def = new fastjet::AreaDefinition(fastjet::VoronoiAreaSpec());
      fj.useJetArea(_area_def);
      addProjection(fj, "KtJetsD05");

      IdentifiedFinalState photonfs(-2.37, 2.37, 16.0*GeV);
      photonfs.acceptId(PID::PHOTON);
      addProjection(photonfs, "Photon");

      _h_M    = bookHisto1D(1, 1, 1);
      _h_pT   = bookHisto1D(2, 1, 1);
      _h_dPhi = bookHisto1D(3, 1, 1);
    }


    int getEtaBin(double eta_w) const {
      double eta = fabs(eta_w);

      int v_iter=0;
      for (v_iter=0; v_iter < (int)_eta_bins_areaoffset.size()-1; v_iter++) {
        if (inRange(eta, _eta_bins_areaoffset[v_iter], _eta_bins_areaoffset[v_iter+1]))
          break;
      }
      return v_iter;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      const double weight = event.weight();

      ///
      /// require at least 2 photons in final state
      ///
      Particles photons = applyProjection<IdentifiedFinalState>(event, "Photon").particlesByPt();
      if (photons.size() < 2) {
        vetoEvent;
      }

      ///
      /// compute the median energy density
      ///
      std::vector<double> _ptDensity;
      std::vector< std::vector<double> > ptDensities;
      std::vector<double> emptyVec;
      ptDensities.assign(_eta_bins_areaoffset.size()-1, emptyVec);

      const fastjet::ClusterSequenceArea* clust_seq_area = applyProjection<FastJets>(event, "KtJetsD05").clusterSeqArea();
      foreach (const fastjet::PseudoJet& jet, applyProjection<FastJets>(event, "KtJetsD05").pseudoJets(0.0*GeV)) {
        double eta = fabs(jet.eta());
        double pt = fabs(jet.perp());

        /// get the cluster sequence
        double area = clust_seq_area->area(jet);

        if(area > 10e-4 && fabs(eta) < _eta_bins_areaoffset[_eta_bins_areaoffset.size()-1]) {
          ptDensities.at(getEtaBin(fabs(eta))).push_back(pt/area);
        }
      }

      for(int b=0; b<(int)_eta_bins_areaoffset.size()-1; b++) {
        double median = 0.0;
        if(ptDensities[b].size() > 0) {
            std::sort(ptDensities[b].begin(), ptDensities[b].end());
            int nDens = ptDensities[b].size();
            if( nDens%2 == 0 )
              median = (ptDensities[b][nDens/2] + ptDensities[b][(nDens-2)/2]) / 2;
            else
              median = ptDensities[b][(nDens-1)/2];
          }
        _ptDensity.push_back(median);
      }


      ///
      /// Loop over photons and fill vector of isolated ones
      ///
      Particles isolated_photons;
      foreach (const Particle& photon, photons) {

        ///
        /// remove photons in crack
        ///
        double eta_P = photon.eta();
        if (fabs(eta_P)>=1.37 && fabs(eta_P)<1.52) continue;

        double phi_P = photon.momentum().phi();

        ///
        /// compute isolation
        ///

        /// std EtCone
        Particles fs = applyProjection<FinalState>(event, "FS").particles();
        FourMomentum mom_in_EtCone;
        foreach (const Particle& p, fs) {
          /// check if it's in the cone of .4
          if (deltaR(eta_P, phi_P, p.eta(), p.momentum().phi()) >= 0.4) continue;

          /// check if it's in the 5x7 central core
          if (fabs(eta_P-p.eta()) < .025*7.0*0.5 &&
              fabs(phi_P-p.momentum().phi()) < (M_PI/128.)*5.0*0.5) continue;

          mom_in_EtCone += p.momentum();
        }

        /// now figure out the correction (area*density)
        double EtCone_area = M_PI*.4*.4 - (7.0*.025)*(5.0*M_PI/128.);
        double correction = _ptDensity[getEtaBin(eta_P)]*EtCone_area;

        /// shouldn't need to subtract photon
        /// note: using expected cut at hadron/particle level, not cut at reco level
        if (mom_in_EtCone.Et()-correction > 4.0*GeV) {
          continue;
        }

        /// add photon to list of isolated ones
        isolated_photons.push_back(photon);
      }

      ///
      /// require at least two isolated photons
      ///
      if (isolated_photons.size() < 2) {
        vetoEvent;
      }

      ///
      /// select leading pT pair
      ///
      std::sort(isolated_photons.begin(), isolated_photons.end(), cmpMomByPt);
      FourMomentum y1=isolated_photons[0].momentum();
      FourMomentum y2=isolated_photons[1].momentum();

      ///
      /// require the two photons to be separated (dR>0.4)
      ///
      if (deltaR(y1, y2)<0.4) {
        vetoEvent;
      }

      FourMomentum yy = y1+y2;
      double Myy = yy.mass()/GeV;
      double pTyy = yy.pT()/GeV;
      double dPhiyy = deltaPhi(y1.phi(), y2.phi());

      _h_M->fill(Myy, weight);
      _h_pT->fill(pTyy, weight);
      _h_dPhi->fill(dPhiyy, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      scale(_h_M, crossSection()/sumOfWeights());
      scale(_h_pT, crossSection()/sumOfWeights());
      scale(_h_dPhi, crossSection()/sumOfWeights());
    }

  private:

    Histo1DPtr _h_M;
    Histo1DPtr _h_pT;
    Histo1DPtr _h_dPhi;

    fastjet::AreaDefinition* _area_def;

    std::vector<double> _eta_bins_areaoffset;
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9120807);

}

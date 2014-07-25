// -*- C++ -*-
#include <iostream>
#include <sstream>
#include <string>

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Jet.hh"
#include "Rivet/Projections/FastJets.hh"

#include "fastjet/internal/base.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"

namespace Rivet {


  class ATLAS_2010_S8914702 : public Analysis {
  public:

    /// Constructor
    ATLAS_2010_S8914702()
      : Analysis("ATLAS_2010_S8914702")
    {
      _eta_bins.push_back( 0.00);
      _eta_bins.push_back( 0.60);
      _eta_bins.push_back( 1.37);
      _eta_bins.push_back( 1.52);
      _eta_bins.push_back( 1.81);

      _eta_bins_areaoffset.push_back(0.0);
      _eta_bins_areaoffset.push_back(1.5);
      _eta_bins_areaoffset.push_back(3.0);
    }


    /// Book histograms and initialise projections before the run
    void init() {
      FinalState fs;
      addProjection(fs, "FS");

      FastJets fj(fs, FastJets::KT, 0.5);
      _area_def = new fastjet::AreaDefinition(fastjet::VoronoiAreaSpec());
      fj.useJetArea(_area_def);
      addProjection(fj, "KtJetsD05");

      LeadingParticlesFinalState photonfs(FinalState(-1.81, 1.81, 15.0*GeV));
      photonfs.addParticleId(PID::PHOTON);
      addProjection(photonfs, "LeadingPhoton");

      int hist_bin = 0;
      for (int i = 0; i < (int)_eta_bins.size()-1; ++i) {
        if (fabs(_eta_bins[i] - 1.37) < .0001) continue;
        _h_Et_photon[i] = bookHisto1D(1, 1, hist_bin+1);
        hist_bin += 1;
      }
    }


    int getEtaBin(double eta_w, bool area_eta) const {
      double eta = fabs(eta_w);
      int v_iter = 0;
      if (!area_eta) {
        for (v_iter=0; v_iter < (int)_eta_bins.size()-1; ++v_iter) {
          if (eta >= _eta_bins.at(v_iter) && eta < _eta_bins.at(v_iter+1)) break;
        }
      } else {
        for (v_iter=0; v_iter < (int)_eta_bins_areaoffset.size()-1; ++v_iter) {
          if (eta >= _eta_bins_areaoffset.at(v_iter) && eta < _eta_bins_areaoffset.at(v_iter+1)) break;
        }
      }
      return v_iter;
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      Particles photons = applyProjection<LeadingParticlesFinalState>(event, "LeadingPhoton").particles();
      if (photons.size() != 1) {
        vetoEvent;
      }

      FourMomentum leadingPhoton = photons[0].momentum();
      double eta_P = leadingPhoton.eta();
      double phi_P = leadingPhoton.phi();

      if(fabs(eta_P)>=1.37 && fabs(eta_P)<1.52){
        vetoEvent;
      }

      int eta_bin = getEtaBin(eta_P,false);

      Particles fs = applyProjection<FinalState>(event, "FS").particles();
      FourMomentum mom_in_EtCone;
      foreach (const Particle& p, fs) {
        // check if it's in the cone of .4
        if (deltaR(eta_P, phi_P, p.eta(), p.momentum().phi()) >= 0.4) continue;

        // check if it's in the 5x7 central core
        if (fabs(eta_P-p.eta()) < .025*7.0*0.5 &&
            fabs(phi_P-p.momentum().phi()) < (PI/128.)*5.0*0.5) continue;
        mom_in_EtCone += p.momentum();
      }
      MSG_DEBUG("Done with initial EtCone.");

      // Now compute the median energy density
      _ptDensity.clear();
      _sigma.clear();
      _Njets.clear();

      std::vector< std::vector<double> > ptDensities;
      std::vector<double> emptyVec;
      ptDensities.assign(_eta_bins_areaoffset.size()-1,emptyVec);

      const fastjet::ClusterSequenceArea* clust_seq_area = applyProjection<FastJets>(event, "KtJetsD05").clusterSeqArea();
      foreach (const fastjet::PseudoJet& jet, applyProjection<FastJets>(event, "KtJetsD05").pseudoJets(0.0*GeV)) {
        //const double y = fabs(jet.rapidity());
        const double eta = fabs(jet.eta());
        const double pt = fabs(jet.perp());

        // Get the cluster sequence
        double area = clust_seq_area->area(jet);

        if (area > 10e-4 && fabs(eta)<_eta_bins_areaoffset[_eta_bins_areaoffset.size()-1]) {
          ptDensities.at(getEtaBin(fabs(eta),true)).push_back(pt/area);
        }
      }

      for (int b = 0; b < (int)_eta_bins_areaoffset.size()-1; ++b) {
        double median = 0.0;
        double sigma = 0.0;
        int Njets = 0;
        if (ptDensities[b].size() > 0) {
          std::sort(ptDensities[b].begin(), ptDensities[b].end());
          int nDens = ptDensities[b].size();
          if (nDens % 2 == 0) {
            median = (ptDensities[b][nDens/2]+ptDensities[b][(nDens-2)/2])/2;
          } else {
            median = ptDensities[b][(nDens-1)/2];
          }
          sigma = ptDensities[b][(int)(.15865*nDens)];
          Njets = nDens;
        }
        _ptDensity.push_back(median);
        _sigma.push_back(sigma);
        _Njets.push_back(Njets);
      }

      // Now figure out the correction
      float EtCone_area = PI*.4*.4 - (7.0*.025)*(5.0*PI/128.);
      float correction = _ptDensity[getEtaBin(eta_P,true)]*EtCone_area;
      MSG_DEBUG("Jet area correction done.");

      // Shouldn't need to subtract photon
      // NB. Using expected cut at hadron/particle level, not cut at reco level
      if (mom_in_EtCone.Et() - correction/*-leadingPhoton.Et()*/ > 4.0*GeV) {
        vetoEvent;
      }
      MSG_DEBUG("Passed isolation cut.");

      _h_Et_photon[eta_bin]->fill(leadingPhoton.Et(), weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      for (int i = 0; i < (int)_eta_bins.size()-1; ++i) {
        if (fabs(_eta_bins[i] - 1.37) < .0001) continue;
        scale(_h_Et_photon[i], crossSection()/sumOfWeights());
      }
    }


  private:

    Histo1DPtr _h_Et_photon[6];

    fastjet::AreaDefinition* _area_def;

    std::vector<float> _eta_bins;
    std::vector<float> _eta_bins_areaoffset;

    std::vector<float> _ptDensity;
    std::vector<float> _sigma;
    std::vector<float> _Njets;
  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2010_S8914702);

}

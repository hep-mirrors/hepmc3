// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {

  class ATLAS_2013_I1217867 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    ATLAS_2013_I1217867()
      : Analysis("ATLAS_2013_I1217867")
    {
      m_njet=4;
      _h_dI.resize(2, std::vector<Histo1DPtr>(m_njet));
      _h_dI_ratio.resize(2, std::vector<Histo1DPtr>(m_njet-1));
    }

    //@}


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      // Initialise projections

      FinalState fs(-5.0, 5.0, 0.0*GeV);

      IdentifiedFinalState bareElectrons(fs);
      bareElectrons.acceptIdPair(PID::ELECTRON);
      vector<pair<double, double> > etaRangesElectrons;
      etaRangesElectrons.push_back(make_pair(-2.47, -1.52));
      etaRangesElectrons.push_back(make_pair(-1.37, 1.37));
      etaRangesElectrons.push_back(make_pair(1.52, 2.47));
      DressedLeptons electronClusters(fs, bareElectrons, 0.1, true, etaRangesElectrons, 20.0*GeV);
      addProjection(electronClusters, "electronClusters");

      IdentifiedFinalState bareMuons(fs);
      bareMuons.acceptIdPair(PID::MUON);
      vector<pair<double, double> > etaRangesMuons;
      etaRangesMuons.push_back(make_pair(-2.4, 2.4));
      DressedLeptons muonClusters(fs, bareMuons, 0.1, true, etaRangesMuons, 20.0*GeV);
      addProjection(muonClusters, "muonClusters");

      IdentifiedFinalState neutrinos(-MAXDOUBLE, MAXDOUBLE, 25.0*GeV);
      neutrinos.acceptNeutrinos();
      addProjection(neutrinos, "neutrinos");

      VetoedFinalState jetFS(fs);
      jetFS.addVetoOnThisFinalState(electronClusters);
      jetFS.addVetoOnThisFinalState(muonClusters);
      jetFS.addVetoOnThisFinalState(neutrinos);
      FastJets jetpro(jetFS, FastJets::KT, 0.6);
      jetpro.useInvisibles(true);
      addProjection(jetpro, "jets");

      // Book histograms
      for (size_t flav=0; flav < 2; ++flav) {
        for (size_t i=0; i < m_njet; ++i) _h_dI[flav][i] = bookHisto1D(i+1, 1, flav+1);
        for (size_t i=0; i < m_njet-1; ++i) _h_dI_ratio[flav][i] = bookHisto1D(4+i+1, 1, flav+1);
      }
    }


    /// Perform the per-event analysis
    void analyze(const Event& e) {
      const double weight = e.weight();

      const DressedLeptons& electronClusters = applyProjection<DressedLeptons>(e, "electronClusters");
      const DressedLeptons& muonClusters = applyProjection<DressedLeptons>(e, "muonClusters");
      int ne = electronClusters.clusteredLeptons().size();
      int nmu = muonClusters.clusteredLeptons().size();

      FourMomentum lepton;
      size_t flav = 2;
      if (ne==1) {
        lepton=electronClusters.clusteredLeptons()[0].momentum();
        flav = 0;
        if (nmu > 0) vetoEvent;
      }
      else if (nmu == 1) {
        lepton=muonClusters.clusteredLeptons()[0].momentum();
        flav = 1;
        if (ne > 0) vetoEvent;
      }
      else {
        vetoEvent;
      }

      const Particles& neutrinos = applyProjection<FinalState>(e, "neutrinos").particlesByPt();
      if (neutrinos.size() < 1) vetoEvent;
      FourMomentum neutrino = neutrinos[0].momentum();

      double mtW=sqrt(2.0*lepton.pT()*neutrino.pT()*(1-cos(lepton.phi()-neutrino.phi())));
      if (mtW<40.0*GeV) vetoEvent;

      const fastjet::ClusterSequence* seq = applyProjection<FastJets>(e, "jets").clusterSeq();
      if (seq != NULL) {
        for (size_t i = 0; i < min(m_njet,(size_t)seq->n_particles()); ++i) {
          double d_ij = sqrt(seq->exclusive_dmerge_max(i));
          _h_dI[flav][i]->fill(d_ij, weight);

          if (i<m_njet-1) {
            if (d_ij>20.0*GeV) {
              double d_ijplus1 = sqrt(seq->exclusive_dmerge_max(i+1));
              _h_dI_ratio[flav][i]->fill(d_ijplus1/d_ij, weight);
            }
          }
        }
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      for (size_t flav = 0; flav < 2; ++flav) {
        for (size_t i = 0; i < m_njet; ++i) {
          normalize(_h_dI[flav][i], 1.0, false);
          if (i < m_njet-1) normalize(_h_dI_ratio[flav][i], 1.0, false);
        }
      }
    }

    //@}


  private:

    /// @name Histograms
    //@{
    std::vector<std::vector<Histo1DPtr> > _h_dI;
    std::vector<std::vector<Histo1DPtr> > _h_dI_ratio;

    //@}

    size_t m_njet;
  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2013_I1217867);


}

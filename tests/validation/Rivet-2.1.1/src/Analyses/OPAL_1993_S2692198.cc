// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "fastjet/JadePlugin.hh"

namespace Rivet {


  /// @brief OPAL photon production
  /// @author Peter Richardson
  class OPAL_1993_S2692198 : public Analysis {
  public:

    /// Constructor
    OPAL_1993_S2692198()
      : Analysis("OPAL_1993_S2692198")
    {    }


    /// @name Analysis methods
    //@{

    void analyze(const Event& e) {
      // Get event weight for histo filling
      const double weight = e.weight();

      // Extract the photons
      Particles photons;
      Particles nonPhotons;
      FourMomentum ptotal;
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      foreach (const Particle& p, fs.particles()) {
        ptotal+= p.momentum();
        if (p.pdgId() == PID::PHOTON) {
          photons.push_back(p);
        } else {
          nonPhotons.push_back(p);
        }
      }
      // No photon return but still count for normalisation
      if (photons.empty()) return;
      // Definition of the Durham algorithm
      fastjet::JetDefinition durham_def(fastjet::ee_kt_algorithm, fastjet::E_scheme, fastjet::Best);
      // Definition of the JADE algorithm
      fastjet::JadePlugin jade;
      fastjet::JetDefinition jade_def = fastjet::JetDefinition(&jade);
      // Now for the weird jet algorithm
      double evis = ptotal.mass();
      vector<fastjet::PseudoJet> input_particles;
      // Pseudo-jets from the non photons
      foreach (const Particle& p,  nonPhotons) {
        const FourMomentum p4 = p.momentum();
        input_particles.push_back(fastjet::PseudoJet(p4.px(), p4.py(), p4.pz(), p4.E()));
      }
      // Pseudo-jets from all bar the first photon
      for (size_t ix = 1; ix < photons.size(); ++ix) {
        const FourMomentum p4 = photons[ix].momentum();
        input_particles.push_back(fastjet::PseudoJet(p4.px(), p4.py(), p4.pz(), p4.E()));
      }
      // Now loop over the photons
      for (size_t ix = 0; ix < photons.size(); ++ix) {
        FourMomentum pgamma = photons[ix].momentum();
        // Run the jet clustering DURHAM
        fastjet::ClusterSequence clust_seq(input_particles, durham_def);
        // Cluster the jets
        for (size_t j = 0; j < _nPhotonDurham->numBins(); ++j) {
          bool accept(true);
          double ycut = _nPhotonDurham->bin(j).midpoint(); //< @todo Should this be xMin?
          double dcut = sqr(evis)*ycut;
          vector<fastjet::PseudoJet> exclusive_jets = sorted_by_E(clust_seq.exclusive_jets(dcut));
          for (size_t iy = 0; iy < exclusive_jets.size(); ++iy) {
            FourMomentum pjet(momentum(exclusive_jets[iy]));
            double cost = pjet.vector3().unit().dot(pgamma.vector3().unit());
            double ygamma = 2 * min(sqr(pjet.E()/evis), sqr(pgamma.E()/evis)) * (1 - cost);
            if (ygamma < ycut) {
              accept = false;
              break;
            }
          }
          if (!accept) continue;
          _nPhotonDurham->fill(ycut, weight*_nPhotonDurham->bin(j).width());
          size_t njet = min(size_t(4), exclusive_jets.size()) - 1;
          if (j < _nPhotonJetDurham[njet]->numBins()) {
            _nPhotonJetDurham[njet]->fillBin(j, weight*_nPhotonJetDurham[njet]->bin(j).width());
          }
        }
        // Run the jet clustering JADE
        fastjet::ClusterSequence clust_seq2(input_particles, jade_def);
        for (size_t j = 0; j < _nPhotonJade->numBins(); ++j) {
          bool accept(true);
          double ycut = _nPhotonJade->bin(j).midpoint(); //< @todo Should this be xMin?
          double dcut = sqr(evis)*ycut;
          vector<fastjet::PseudoJet> exclusive_jets = sorted_by_E(clust_seq2.exclusive_jets(dcut));
          for (size_t iy = 0; iy < exclusive_jets.size(); ++iy) {
            FourMomentum pjet(momentum(exclusive_jets[iy]));
            double cost = pjet.vector3().unit().dot(pgamma.vector3().unit());
            double ygamma = 2.*pjet.E()*pgamma.E()/sqr(evis)*(1.-cost);
            if (ygamma < ycut) {
              accept = false;
              break;
            }
          }
          if (!accept) continue;
          /// @todo Really want to use a "bar graph" here (i.e. ignore bin width)
          _nPhotonJade->fill(ycut, weight*_nPhotonJade->bin(j).width());
          size_t njet = min(size_t(4), exclusive_jets.size()) - 1;
          if (j < _nPhotonJetJade[njet]->numBins()) {
            _nPhotonJetJade[njet]->fillBin(j, weight*_nPhotonJetJade[njet]->bin(j).width());
          }
        }
        // Add this photon back in for the next iteration of the loop
        if (ix+1 != photons.size()) {
          input_particles[nonPhotons.size()+ix] = fastjet::PseudoJet(pgamma.px(), pgamma.py(), pgamma.pz(), pgamma.E());
        }
      }
    }


    void init() {
      // Projections
      addProjection(FinalState(), "FS");

      // Book datasets
      _nPhotonJade   = bookHisto1D(1, 1, 1);
      _nPhotonDurham = bookHisto1D(2, 1, 1);
      for (size_t ix = 0; ix < 4; ++ix) {
        _nPhotonJetJade  [ix] = bookHisto1D(3 , 1, 1+ix);
        _nPhotonJetDurham[ix] = bookHisto1D(4 , 1, 1+ix);
      }
    }


    /// Finalize
    void finalize() {
      const double fact = 1000/sumOfWeights();
      scale(_nPhotonJade, fact);
      scale(_nPhotonDurham, fact);
      for (size_t ix = 0; ix < 4; ++ix) {
        scale(_nPhotonJetJade  [ix],fact);
        scale(_nPhotonJetDurham[ix],fact);
      }
    }

    //@}

  private:

    Histo1DPtr _nPhotonJade;
    Histo1DPtr _nPhotonDurham;
    Histo1DPtr _nPhotonJetJade  [4];
    Histo1DPtr _nPhotonJetDurham[4];

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(OPAL_1993_S2692198);

}

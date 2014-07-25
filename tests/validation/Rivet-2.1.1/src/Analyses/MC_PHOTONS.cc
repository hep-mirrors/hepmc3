// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  class MC_PHOTONS : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    MC_PHOTONS()
      : Analysis("MC_PHOTONS")
    {    }

    //@}


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      IdentifiedFinalState leptons(-5.0, 5.0, 10*GeV);
      IdentifiedFinalState photons(-5.0, 5.0);
      leptons.acceptChLeptons();
      photons.acceptId(PID::PHOTON);
      addProjection(leptons, "lFS");
      addProjection(photons, "gammaFS");

      _h_Ptgamma = bookHisto1D("Ptgamma", logspace(50, 0.01, 30));
      _h_Egamma = bookHisto1D("Egamma", logspace(50, 0.01, 200));
      _h_sumPtgamma = bookHisto1D("sumPtgamma", 50, 0, 100);
      _h_sumEgamma = bookHisto1D("sumEgamma", 50, 0, sqrtS()/GeV/5.0);
      _h_DelR = bookHisto1D("DeltaR", 50, 0, 2);
      _h_DelR_weighted = bookHisto1D("DeltaR_ptweighted", 50, 0, 2);
      _h_DelR_R = bookHisto1D("DeltaR_R", 50, 0, 2);
      _h_DelR_R_weighted = bookHisto1D("DeltaR_R_ptweighted", 50, 0, 2);
      _p_DelR_vs_pTl = bookProfile1D("DeltaR_vs_pTlep", 50, 10, 120);
      _p_DelR_weighted_vs_pTl = bookProfile1D("DeltaR_ptweighted_vs_pTlep", 50, 10, 120);
      _p_DelR_R_vs_pTl = bookProfile1D("DeltaR_R_vs_pTlep", 50, 10, 120);
      _p_DelR_R_weighted_vs_pTl = bookProfile1D("DeltaR_R_ptweighted_vs_pTlep", 50, 10, 120);
      _p_sumPtgamma_vs_pTl = bookProfile1D("sumPtGamma_vs_pTlep", 50, 10, 120);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      /// Get photons and leptons
      const Particles& photons = applyProjection<FinalState>(event, "gammaFS").particles();
      MSG_DEBUG("Photon multiplicity = " << photons.size());
      const Particles& leptons = applyProjection<FinalState>(event, "lFS").particles();
      MSG_DEBUG("Photon multiplicity = " << leptons.size());

      // Initialise a map of sumPtgamma for each lepton
      map<size_t, double> sumpT_per_lep;
      for (size_t il = 0; il < leptons.size(); ++il) sumpT_per_lep[il] = 0;

      // Calculate photon energies and transverse momenta
      double sumPtgamma(0), sumEgamma(0);
      foreach (const Particle& p, photons) {
        // Individual and summed pTs and energies
        double pTgamma = p.pT()/GeV;
        double Egamma = p.momentum().E()/GeV;
        _h_Ptgamma->fill(pTgamma, weight);
        _h_Egamma->fill(Egamma, weight);
        sumPtgamma += pTgamma;
        sumEgamma += Egamma;

        // Calculate delta R with respect to the nearest lepton
        int ilep = -1;
        double delR = 10000;
        for (size_t il = 0; il < leptons.size(); ++il) {
          const double tmpdelR = deltaR(leptons[il].momentum(), p.momentum());
          if (tmpdelR < delR) {
            ilep = il;
            delR = tmpdelR;
          }
        }
        if (ilep != -1) {
          _h_DelR->fill(delR, weight);
          _h_DelR_weighted->fill(delR, weight*pTgamma/GeV);
          _h_DelR_R->fill(delR, weight/(delR+1e-5));
          _h_DelR_R_weighted->fill(delR, weight*pTgamma/GeV/(delR+1e-5));
          _p_DelR_vs_pTl->fill(leptons[ilep].pT()/GeV, delR, weight);
          _p_DelR_weighted_vs_pTl->fill(leptons[ilep].pT()/GeV, delR, weight*pTgamma/GeV);
          _p_DelR_R_vs_pTl->fill(leptons[ilep].pT()/GeV, delR, weight/(delR+1e-5));
          _p_DelR_R_weighted_vs_pTl->fill(leptons[ilep].pT()/GeV, delR, weight*pTgamma/GeV/(delR+1e-5));
          sumpT_per_lep[ilep] += pTgamma;
        }
      }

      // Histogram whole-event photon HT/energy
      _h_sumPtgamma->fill(sumPtgamma/GeV, weight);
      _h_sumEgamma->fill(sumEgamma/GeV, weight);

      // Histogram per-lepton sum(pT)
      for (size_t il = 0; il < leptons.size(); ++il) {
        _p_sumPtgamma_vs_pTl->fill(leptons[il].pT()/GeV, sumpT_per_lep[il]/GeV, weight);
      }

    }


    /// Normalise histograms etc., after the run
    void finalize() {
      normalize(_h_Ptgamma);
      normalize(_h_Egamma);
      normalize(_h_sumPtgamma);
      normalize(_h_sumEgamma);
      normalize(_h_DelR);
      normalize(_h_DelR_weighted);
      normalize(_h_DelR_R);
      normalize(_h_DelR_R_weighted);
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _h_Ptgamma, _h_Egamma;
    Histo1DPtr _h_sumPtgamma, _h_sumEgamma;
    Histo1DPtr _h_DelR, _h_DelR_weighted;
    Histo1DPtr _h_DelR_R, _h_DelR_R_weighted;
    Profile1DPtr _p_DelR_vs_pTl, _p_DelR_weighted_vs_pTl;
    Profile1DPtr _p_DelR_R_vs_pTl, _p_DelR_R_weighted_vs_pTl;
    Profile1DPtr _p_sumPtgamma_vs_pTl;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_PHOTONS);


}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/InvMassFinalState.hh"

namespace Rivet {


  /// CMS W + 2 jet double parton scattering analysis
  class CMS_2013_I1272853 : public Analysis {
  public:

    /// Constructor
    CMS_2013_I1272853()
      : Analysis("CMS_2013_I1272853") { }


    /// Book histograms and initialise projections before the run
    void init() {

       const FinalState fs;
       addProjection(fs, "FS");

       /// @todo Use C++11 initialisation syntax
       vector<PdgIdPair> vidsW;
       vidsW += make_pair(PID::MUON, PID::NU_MUBAR), make_pair(PID::ANTIMUON, PID::NU_MU);
       InvMassFinalState invfsW(fs, vidsW, 20*GeV, 1e6*GeV);
       addProjection(invfsW, "INVFSW");

       VetoedFinalState vfs(fs);
       vfs.addVetoOnThisFinalState(invfsW);
       addProjection(vfs, "VFS");
       addProjection(FastJets(vfs, FastJets::ANTIKT, 0.5), "Jets");

       _h_deltaS_eq2jet_Norm = bookHisto1D(1,1,1);
       _h_rel_deltaPt_eq2jet_Norm = bookHisto1D(2,1,1);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      // Find Ws
      const InvMassFinalState& invMassFinalStateW = applyProjection<InvMassFinalState>(event, "INVFSW");
      if (invMassFinalStateW.empty()) vetoEvent;
      const Particles& WDecayProducts = invMassFinalStateW.particles();
      if (WDecayProducts.size() < 2) vetoEvent;

      // Cuts on W decay properties
      const int iNU_MU = (WDecayProducts[1].abspid() == PID::NU_MU) ? 1 : 0;
      const int iAN_MU = 1 - iNU_MU;
      const double pt1  = WDecayProducts[iAN_MU].pT();
      const double pt2  = WDecayProducts[iNU_MU].Et();
      const double eta1 = WDecayProducts[iAN_MU].abseta();
      const double phi1 = WDecayProducts[iAN_MU].phi();
      const double phi2 = WDecayProducts[iNU_MU].phi();
      const double mt   = sqrt(2 * pt1 * pt2 * (1 - cos(phi1-phi2)));
      if (mt < 50*GeV || pt1 < 35*GeV || eta1 > 2.1 || pt2 < 30*GeV) vetoEvent;

      // Get jets and make sure there are at least two of them in |y| < 2
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      /// @todo Collapse this into jetpro.jetsByPt(ptGtr(20*GeV) & rapIn(2.0))
      vector<FourMomentum> jets;
      foreach (const Jet& jet, jetpro.jetsByPt(20*GeV))
        if (jet.absrap() < 2.0) jets.push_back(jet.momentum());
      if (jets.size() != 2) vetoEvent;

      const double mupx     = pt1 * cos(phi1);
      const double mupy     = pt1 * sin(phi1);
      const double met_x    = pt2 * cos(phi2);
      const double met_y    = pt2 * sin(phi2);
      const double dpt      = add_quad(jets[0].px() + jets[1].px(), jets[0].py() + jets[1].py());
      const double rel_dpt  = dpt / (jets[0].pT() + jets[1].pT());
      const double pT2      = sqr(mupx + met_x) + sqr(mupy + met_y);
      const double Px       = (mupx + met_x)*(jets[0].px() + jets[1].px());
      const double Py       = (mupy + met_y)*(jets[0].py() + jets[1].py());
      const double p1p2_mag = dpt * sqrt(pT2);
      const double dS       = acos((Px+Py) / p1p2_mag);

      const double weight = event.weight();
      _h_rel_deltaPt_eq2jet_Norm->fill(rel_dpt, weight);
      _h_deltaS_eq2jet_Norm->fill(dS, weight);
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      const double rel_dpt_bw = 1.0002 / 30.0;
      const double dphi_bw = 3.14160 / 30.0;
      normalize(_h_rel_deltaPt_eq2jet_Norm, rel_dpt_bw);
      normalize(_h_deltaS_eq2jet_Norm, dphi_bw);
    }


  private:

    Histo1DPtr _h_rel_deltaPt_eq2jet_Norm;
    Histo1DPtr _h_deltaS_eq2jet_Norm;

  };



  DECLARE_RIVET_PLUGIN(CMS_2013_I1272853);

}

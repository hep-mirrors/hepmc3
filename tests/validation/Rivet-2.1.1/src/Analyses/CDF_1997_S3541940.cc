// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


  /// @brief CDF properties of 6-jet events with large 6-jet mass
  class CDF_1997_S3541940 : public Analysis {
  public:

    CDF_1997_S3541940()
      : Analysis("CDF_1997_S3541940")
    {
    }


  public:

    void init() {

      const FinalState fs(-4.2, 4.2);
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      _h_m6J = bookHisto1D(1, 1, 1);
      _h_X3ppp = bookHisto1D(2, 1, 1);
      _h_X4ppp = bookHisto1D(3, 1, 1);
      _h_costheta3ppp = bookHisto1D(4, 1, 1);
      _h_psi3ppp = bookHisto1D(5, 1, 1);
      _h_f3ppp = bookHisto1D(6, 1, 1);
      _h_f4ppp = bookHisto1D(6, 1, 2);
      _h_f5ppp = bookHisto1D(6, 1, 3);
      _h_XApp = bookHisto1D(7, 1, 1);
      _h_XCp = bookHisto1D(8, 1, 1);
      _h_XE = bookHisto1D(9, 1, 1);
      _h_psiAppBpp = bookHisto1D(10, 1, 1);
      _h_psiCpDp = bookHisto1D(11, 1, 1);
      _h_psiEF = bookHisto1D(12, 1, 1);
      _h_fApp = bookHisto1D(13, 1, 1);
      _h_fBpp = bookHisto1D(14, 1, 1);
      _h_fCp = bookHisto1D(15, 1, 1);
      _h_fDp = bookHisto1D(16, 1, 1);
      _h_fE = bookHisto1D(17, 1, 1);
      _h_fF = bookHisto1D(18, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets;
      double sumEt = 0.0;
      FourMomentum jetsystem(0.0, 0.0, 0.0, 0.0);
      foreach (const Jet& jet, applyProjection<FastJets>(event, "Jets").jetsByEt()) {
        double Et = jet.momentum().Et();
        double eta = fabs(jet.eta());
        if (Et > 20.0*GeV && eta < 3.0) {
          bool separated=true;
          foreach (const Jet& ref, jets) {
            if (deltaR(jet.momentum(), ref.momentum()) < 0.9) {
              separated=false;
              break;
            }
          }
          if (!separated) continue;
          jets.push_back(jet);
          sumEt += Et;
          jetsystem += jet.momentum();
        }
        if (jets.size() >= 6) break;
      }

      if (jets.size() < 6) {
        vetoEvent;
      }

      if (sumEt < 320.0*GeV) {
        vetoEvent;
      }

      double m6J = _safeMass(jetsystem);
      if (m6J < 520.0*GeV) {
        vetoEvent;
      }

      LorentzTransform cms_boost(-jetsystem.boostVector());
      vector<FourMomentum> jets6;
      foreach (Jet jet, jets) {
        jets6.push_back(cms_boost.transform(jet.momentum()));
      }
      std::sort(jets6.begin(), jets6.end(), FourMomentum::byEDescending());

      FourMomentum pE, pF;
      vector<FourMomentum> jets5(_reduce(jets6, pE, pF));
      std::sort(jets5.begin(), jets5.end(), FourMomentum::byEDescending());

      FourMomentum pCp, pDp;
      vector<FourMomentum> jets4(_reduce(jets5, pCp, pDp));
      std::sort(jets4.begin(), jets4.end(), FourMomentum::byEDescending());

      FourMomentum pApp, pBpp;
      vector<FourMomentum> jets3(_reduce(jets4, pApp, pBpp));
      std::sort(jets3.begin(), jets3.end(), FourMomentum::byEDescending());
      FourMomentum p3ppp(jets3[0]);
      FourMomentum p4ppp(jets3[1]);
      FourMomentum p5ppp(jets3[2]);

      double X3ppp = 2.0*p3ppp.E()/m6J;
      if (X3ppp > 0.9) {
        vetoEvent;
      }

      FourMomentum pAV = cms_boost.transform(_avg_beam_in_lab(m6J, jetsystem.rapidity()));
      double costheta3ppp = pAV.vector3().unit().dot(p3ppp.vector3().unit());
      if (fabs(costheta3ppp) > 0.9) {
        vetoEvent;
      }

      // 3-jet-system variables
      _h_m6J->fill(m6J, weight);
      _h_X3ppp->fill(X3ppp, weight);
      _h_X4ppp->fill(2.0*p4ppp.E()/m6J, weight);
      _h_costheta3ppp->fill(costheta3ppp, weight);
      double psi3ppp = _psi(p3ppp, pAV, p4ppp, p5ppp);
      _h_psi3ppp->fill(psi3ppp, weight);
      _h_f3ppp->fill(_safeMass(p3ppp)/m6J, weight);
      _h_f4ppp->fill(_safeMass(p4ppp)/m6J, weight);
      _h_f5ppp->fill(_safeMass(p5ppp)/m6J, weight);

      // 4 -> 3 jet variables
      _h_fApp->fill(_safeMass(pApp)/m6J, weight);
      _h_fBpp->fill(_safeMass(pApp)/m6J, weight);
      _h_XApp->fill(pApp.E()/(pApp.E()+pBpp.E()), weight);
      double psiAppBpp = _psi(pApp, pBpp, pApp+pBpp, pAV);
      _h_psiAppBpp->fill(psiAppBpp, weight);

      // 5 -> 4 jet variables
      _h_fCp->fill(_safeMass(pCp)/m6J, weight);
      _h_fDp->fill(_safeMass(pDp)/m6J, weight);
      _h_XCp->fill(pCp.E()/(pCp.E()+pDp.E()), weight);
      double psiCpDp = _psi(pCp, pDp, pCp+pDp, pAV);
      _h_psiCpDp->fill(psiCpDp, weight);

      // 6 -> 5 jet variables
      _h_fE->fill(_safeMass(pE)/m6J, weight);
      _h_fF->fill(_safeMass(pF)/m6J, weight);
      _h_XE->fill(pE.E()/(pE.E()+pF.E()), weight);
      double psiEF = _psi(pE, pF, pE+pF, pAV);
      _h_psiEF->fill(psiEF, weight);
    }


    void finalize() {
      normalize(_h_m6J);
      normalize(_h_X3ppp);
      normalize(_h_X4ppp);
      normalize(_h_costheta3ppp);
      normalize(_h_psi3ppp);
      normalize(_h_f3ppp);
      normalize(_h_f4ppp);
      normalize(_h_f5ppp);
      normalize(_h_XApp);
      normalize(_h_XCp);
      normalize(_h_XE);
      normalize(_h_psiAppBpp);
      normalize(_h_psiCpDp);
      normalize(_h_psiEF);
      normalize(_h_fApp);
      normalize(_h_fBpp);
      normalize(_h_fCp);
      normalize(_h_fDp);
      normalize(_h_fE);
      normalize(_h_fF);
    }


  private:

    vector<FourMomentum> _reduce(const vector<FourMomentum>& jets,
                                 FourMomentum& combined1,
                                 FourMomentum& combined2) {
      double minMass2 = 1e9;
      size_t idx1(jets.size()), idx2(jets.size());
      for (size_t i = 0; i < jets.size(); ++i) {
        for (size_t j = i+1; j < jets.size(); ++j) {
          double mass2 = FourMomentum(jets[i] + jets[j]).mass2();
          if (mass2 < minMass2) {
            idx1 = i;
            idx2 = j;
          }
        }
      }
      vector<FourMomentum> newjets;
      for (size_t i = 0; i < jets.size(); ++i) {
        if (i != idx1 && i != idx2) newjets.push_back(jets[i]);
      }
      newjets.push_back(jets[idx1] + jets[idx2]);
      combined1 = jets[idx1];
      combined2 = jets[idx2];
      return newjets;
    }


    FourMomentum _avg_beam_in_lab(const double& m, const double& y) {
      const double mt = m/2.0;
      FourMomentum beam1(mt, 0, 0,  mt);
      FourMomentum beam2(mt, 0, 0, -mt);
      if (fabs(y) > 1e-3) {
        FourMomentum boostvec(cosh(y), 0.0, 0.0, sinh(y));
        LorentzTransform cms_boost(-boostvec.boostVector());
        cms_boost = cms_boost.inverse();
        beam1 = cms_boost.transform(beam1);
        beam2 = cms_boost.transform(beam2);
      }
      if (beam1.E() > beam2.E()) {
        return beam1 - beam2;
      }
      else {
        return beam2 - beam1;
      }
    }


    double _psi(const FourMomentum& p1, const FourMomentum& p2,
                const FourMomentum& p3, const FourMomentum& p4) {
      Vector3 p1xp2 = p1.vector3().cross(p2.vector3());
      Vector3 p3xp4 = p3.vector3().cross(p4.vector3());
      return mapAngle0ToPi(acos(p1xp2.unit().dot(p3xp4.unit())));
    }


    double _safeMass(const FourMomentum& p) {
      double mass2 = p.mass2();
      if (mass2 > 0.0) return sqrt(mass2);
      if (mass2 < -1e-5) MSG_WARNING("m2 = " << m2 << ". Assuming m2=0.");
      return 0.0;
    }


  private:

    Histo1DPtr _h_m6J;
    Histo1DPtr _h_X3ppp, _h_X4ppp;
    Histo1DPtr _h_costheta3ppp;
    Histo1DPtr _h_psi3ppp;
    Histo1DPtr _h_f3ppp, _h_f4ppp, _h_f5ppp;
    Histo1DPtr _h_XApp, _h_XCp, _h_XE;
    Histo1DPtr _h_psiAppBpp, _h_psiCpDp, _h_psiEF;
    Histo1DPtr _h_fApp, _h_fBpp, _h_fCp, _h_fDp, _h_fE, _h_fF;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1997_S3541940);

}

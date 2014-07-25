// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Math/LorentzTrans.hh"
#include "Rivet/Math/Vector3.hh"
#include "Rivet/Math/Units.hh"

namespace Rivet {


  /// @brief D0 topological distributions of 3- and 4-jet events.
  class D0_1996_S3214044 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    D0_1996_S3214044() : Analysis("D0_1996_S3214044")
    {    }


    /// @name Analysis methods
    //@{

    /// Book histograms
    void init() {
      const FinalState fs;
      addProjection(fs, "FS");
      /// @todo Use correct jet algorithm
      addProjection(FastJets(fs, FastJets::D0ILCONE, 0.7), "ConeJets");

      _h_3j_x3 = bookHisto1D(1, 1, 1);
      _h_3j_x5 = bookHisto1D(2, 1, 1);
      _h_3j_costheta3 = bookHisto1D(3, 1, 1);
      _h_3j_psi = bookHisto1D(4, 1, 1);
      _h_3j_mu34 = bookHisto1D(5, 1, 1);
      _h_3j_mu35 = bookHisto1D(6, 1, 1);
      _h_3j_mu45 = bookHisto1D(7, 1, 1);

      _h_4j_x3 = bookHisto1D(8, 1, 1);
      _h_4j_x4 = bookHisto1D(9, 1, 1);
      _h_4j_x5 = bookHisto1D(10, 1, 1);
      _h_4j_x6 = bookHisto1D(11, 1, 1);
      _h_4j_costheta3 = bookHisto1D(12, 1, 1);
      _h_4j_costheta4 = bookHisto1D(13, 1, 1);
      _h_4j_costheta5 = bookHisto1D(14, 1, 1);
      _h_4j_costheta6 = bookHisto1D(15, 1, 1);
      _h_4j_cosomega34 = bookHisto1D(16, 1, 1);
      _h_4j_cosomega35 = bookHisto1D(17, 1, 1);
      _h_4j_cosomega36 = bookHisto1D(18, 1, 1);
      _h_4j_cosomega45 = bookHisto1D(19, 1, 1);
      _h_4j_cosomega46 = bookHisto1D(20, 1, 1);
      _h_4j_cosomega56 = bookHisto1D(21, 1, 1);
      _h_4j_mu34 = bookHisto1D(22, 1, 1);
      _h_4j_mu35 = bookHisto1D(23, 1, 1);
      _h_4j_mu36 = bookHisto1D(24, 1, 1);
      _h_4j_mu45 = bookHisto1D(25, 1, 1);
      _h_4j_mu46 = bookHisto1D(26, 1, 1);
      _h_4j_mu56 = bookHisto1D(27, 1, 1);
      _h_4j_theta_BZ = bookHisto1D(28, 1, 1);
      _h_4j_costheta_NR = bookHisto1D(29, 1, 1);

    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets_in;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "ConeJets").jetsByEt(20.0*GeV)) {
        if (fabs(jet.eta()) < 3.0) {
          jets_in.push_back(jet);
        }
      }

      Jets jets_isolated;
      for (size_t i = 0; i < jets_in.size(); ++i) {
        bool isolated=true;
        for (size_t j = 0; j < jets_in.size(); ++j) {
          if (i != j && deltaR(jets_in[i].momentum(), jets_in[j].momentum()) < 1.4) {
            isolated = false;
            break;
          }
        }
        if (isolated) {
          jets_isolated.push_back(jets_in[i]);
        }
      }

      if (jets_isolated.size() == 0 || jets_isolated[0].momentum().Et() < 60.0*GeV) {
        vetoEvent;
      }

      if (jets_isolated.size() > 2) _threeJetAnalysis(jets_isolated, weight);
      if (jets_isolated.size() > 3) _fourJetAnalysis(jets_isolated, weight);
    }


    void finalize() {
      normalize(_h_3j_x3, 1.0);
      normalize(_h_3j_x5, 1.0);
      normalize(_h_3j_costheta3, 1.0);
      normalize(_h_3j_psi, 1.0);
      normalize(_h_3j_mu34, 1.0);
      normalize(_h_3j_mu35, 1.0);
      normalize(_h_3j_mu45, 1.0);
      normalize(_h_4j_x3, 1.0);
      normalize(_h_4j_x4, 1.0);
      normalize(_h_4j_x5, 1.0);
      normalize(_h_4j_x6, 1.0);
      normalize(_h_4j_costheta3, 1.0);
      normalize(_h_4j_costheta4, 1.0);
      normalize(_h_4j_costheta5, 1.0);
      normalize(_h_4j_costheta6, 1.0);
      normalize(_h_4j_cosomega34, 1.0);
      normalize(_h_4j_cosomega35, 1.0);
      normalize(_h_4j_cosomega36, 1.0);
      normalize(_h_4j_cosomega45, 1.0);
      normalize(_h_4j_cosomega46, 1.0);
      normalize(_h_4j_cosomega56, 1.0);
      normalize(_h_4j_mu34, 1.0);
      normalize(_h_4j_mu35, 1.0);
      normalize(_h_4j_mu36, 1.0);
      normalize(_h_4j_mu45, 1.0);
      normalize(_h_4j_mu46, 1.0);
      normalize(_h_4j_mu56, 1.0);
      normalize(_h_4j_theta_BZ, 1.0);
      normalize(_h_4j_costheta_NR, 1.0);
    }

    //@}


  private:

    /// @name Helper functions
    //@{

    void _threeJetAnalysis(const Jets& jets, const double& weight) {
      // >=3 jet events
      FourMomentum jjj(jets[0].momentum()+jets[1].momentum()+jets[2].momentum());
      const double sqrts = _safeMass(jjj);
      if (sqrts<200*GeV) {
        return;
      }

      LorentzTransform cms_boost(-jjj.boostVector());
      vector<FourMomentum> jets_boosted;
      foreach (Jet jet, jets) {
        jets_boosted.push_back(cms_boost.transform(jet.momentum()));
      }
      std::sort(jets_boosted.begin(), jets_boosted.end(), FourMomentum::byEDescending());
      FourMomentum p3(jets_boosted[0]);
      FourMomentum p4(jets_boosted[1]);
      FourMomentum p5(jets_boosted[2]);

      Vector3 beam1(0.0, 0.0, 1.0);
      Vector3 p1xp3 = beam1.cross(p3.vector3());
      Vector3 p4xp5 = p4.vector3().cross(p5.vector3());
      const double cospsi = p1xp3.dot(p4xp5)/p1xp3.mod()/p4xp5.mod();

      _h_3j_x3->fill(2.0*p3.E()/sqrts, weight);
      _h_3j_x5->fill(2.0*p5.E()/sqrts, weight);
      _h_3j_costheta3->fill(fabs(cos(p3.theta())), weight);
      _h_3j_psi->fill(acos(cospsi)/degree, weight);
      _h_3j_mu34->fill(_safeMass(FourMomentum(p3+p4))/sqrts, weight);
      _h_3j_mu35->fill(_safeMass(FourMomentum(p3+p5))/sqrts, weight);
      _h_3j_mu45->fill(_safeMass(FourMomentum(p4+p5))/sqrts, weight);
    }


    void _fourJetAnalysis(const Jets& jets, const double& weight) {
      // >=4 jet events
      FourMomentum jjjj(jets[0].momentum() + jets[1].momentum() + jets[2].momentum()+ jets[3].momentum());
      const double sqrts = _safeMass(jjjj);
      if (sqrts < 200*GeV) return;

      LorentzTransform cms_boost(-jjjj.boostVector());
      vector<FourMomentum> jets_boosted;
      foreach (Jet jet, jets) {
        jets_boosted.push_back(cms_boost.transform(jet.momentum()));
      }
      sort(jets_boosted.begin(), jets_boosted.end(), FourMomentum::byEDescending());
      FourMomentum p3(jets_boosted[0]);
      FourMomentum p4(jets_boosted[1]);
      FourMomentum p5(jets_boosted[2]);
      FourMomentum p6(jets_boosted[3]);

      Vector3 p3xp4 = p3.vector3().cross(p4.vector3());
      Vector3 p5xp6 = p5.vector3().cross(p6.vector3());
      const double costheta_BZ = p3xp4.dot(p5xp6)/p3xp4.mod()/p5xp6.mod();
      const double costheta_NR = (p3.vector3()-p4.vector3()).dot(p5.vector3()-p6.vector3())/
        (p3.vector3()-p4.vector3()).mod()/(p5.vector3()-p6.vector3()).mod();

      _h_4j_x3->fill(2.0*p3.E()/sqrts, weight);
      _h_4j_x4->fill(2.0*p4.E()/sqrts, weight);
      _h_4j_x5->fill(2.0*p5.E()/sqrts, weight);
      _h_4j_x6->fill(2.0*p6.E()/sqrts, weight);
      _h_4j_costheta3->fill(fabs(cos(p3.theta())), weight);
      _h_4j_costheta4->fill(fabs(cos(p4.theta())), weight);
      _h_4j_costheta5->fill(fabs(cos(p5.theta())), weight);
      _h_4j_costheta6->fill(fabs(cos(p6.theta())), weight);
      _h_4j_cosomega34->fill(cos(p3.angle(p4)), weight);
      _h_4j_cosomega35->fill(cos(p3.angle(p5)), weight);
      _h_4j_cosomega36->fill(cos(p3.angle(p6)), weight);
      _h_4j_cosomega45->fill(cos(p4.angle(p5)), weight);
      _h_4j_cosomega46->fill(cos(p4.angle(p6)), weight);
      _h_4j_cosomega56->fill(cos(p5.angle(p6)), weight);
      _h_4j_mu34->fill(_safeMass(FourMomentum(p3+p4))/sqrts, weight);
      _h_4j_mu35->fill(_safeMass(FourMomentum(p3+p5))/sqrts, weight);
      _h_4j_mu36->fill(_safeMass(FourMomentum(p3+p6))/sqrts, weight);
      _h_4j_mu45->fill(_safeMass(FourMomentum(p4+p5))/sqrts, weight);
      _h_4j_mu46->fill(_safeMass(FourMomentum(p4+p6))/sqrts, weight);
      _h_4j_mu56->fill(_safeMass(FourMomentum(p5+p6))/sqrts, weight);
      _h_4j_theta_BZ->fill(acos(costheta_BZ)/degree, weight);
      _h_4j_costheta_NR->fill(costheta_NR, weight);

    }

    double _safeMass(const FourMomentum& p) {
      double mass2=p.mass2();
      if (mass2>0.0) return sqrt(mass2);
      else if (mass2<-1.0e-5) {
        MSG_WARNING("m2 = " << m2 << ". Assuming m2=0.");
        return 0.0;
      }
      else return 0.0;
    }

  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_3j_x3;
    Histo1DPtr _h_3j_x5;
    Histo1DPtr _h_3j_costheta3;
    Histo1DPtr _h_3j_psi;
    Histo1DPtr _h_3j_mu34;
    Histo1DPtr _h_3j_mu35;
    Histo1DPtr _h_3j_mu45;

    Histo1DPtr _h_4j_x3;
    Histo1DPtr _h_4j_x4;
    Histo1DPtr _h_4j_x5;
    Histo1DPtr _h_4j_x6;
    Histo1DPtr _h_4j_costheta3;
    Histo1DPtr _h_4j_costheta4;
    Histo1DPtr _h_4j_costheta5;
    Histo1DPtr _h_4j_costheta6;
    Histo1DPtr _h_4j_cosomega34;
    Histo1DPtr _h_4j_cosomega35;
    Histo1DPtr _h_4j_cosomega36;
    Histo1DPtr _h_4j_cosomega45;
    Histo1DPtr _h_4j_cosomega46;
    Histo1DPtr _h_4j_cosomega56;
    Histo1DPtr _h_4j_mu34;
    Histo1DPtr _h_4j_mu35;
    Histo1DPtr _h_4j_mu36;
    Histo1DPtr _h_4j_mu45;
    Histo1DPtr _h_4j_mu46;
    Histo1DPtr _h_4j_mu56;
    Histo1DPtr _h_4j_theta_BZ;
    Histo1DPtr _h_4j_costheta_NR;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_1996_S3214044);

}

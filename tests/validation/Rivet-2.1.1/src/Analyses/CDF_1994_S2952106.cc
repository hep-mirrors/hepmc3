// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/VisibleFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"

namespace Rivet {


  /// @brief CDF Run I color coherence analysis
  /// @author Andy Buckley
  /// @author Lars Sonnenschein
  class CDF_1994_S2952106 : public Analysis {
  public:

    /// Constructor
    CDF_1994_S2952106() : Analysis("CDF_1994_S2952106")
    {
    }


    /// @name Analysis methods
    //@{

    void init() {
      const FinalState fs(-4.2, 4.2);
      addProjection(fs, "FS");
      addProjection(FastJets(fs, FastJets::CDFJETCLU, 0.7), "Jets");

      // Zero passed-cuts event weight counters
      _sumw = 0;

      // Output histograms
      _histJet1Et  = bookHisto1D(1,1,1);
      _histJet2Et  = bookHisto1D(2,1,1);
      _histJet3eta = bookScatter2D(3,1,1);
      _histR23     = bookScatter2D(4,1,1);
      _histAlpha   = bookScatter2D(5,1,1);

      // Temporary histos: these are the ones we actually fill for the plots which require correction
      _tmphistJet3eta.reset(new Histo1D(refData(3,1,1)));
      _tmphistR23.reset(    new Histo1D(refData(4,1,1)));
      _tmphistAlpha.reset(  new Histo1D(refData(5,1,1)));
    }



    // Do the analysis
    void analyze(const Event & event) {
      const Jets jets = applyProjection<FastJets>(event, "Jets").jetsByEt();
      MSG_DEBUG("Jet multiplicity before any cuts = " << jets.size());

      // ETs only from jets:
      double et_sinphi_sum = 0;
      double et_cosphi_sum = 0;
      double et_sum = 0;
      for (size_t i = 0; i< jets.size(); ++i) {
        et_sinphi_sum += jets[i].momentum().Et() * sin(jets[i].phi());
        et_cosphi_sum += jets[i].momentum().Et() * cos(jets[i].phi());
        et_sum += jets[i].momentum().Et();
      }

      // ET requirement
      if (sqrt(sqr(et_sinphi_sum) + sqr(et_cosphi_sum))/et_sum > 6.0) vetoEvent;

      // Check jet requirements
      if (jets.size() < 3) vetoEvent;
      if (jets[0].pT() < 110*GeV) vetoEvent;
      if (jets[2].pT() < 10*GeV) vetoEvent;

      // More jet 1,2,3 checks
      FourMomentum pj1(jets[0].momentum()), pj2(jets[1].momentum()), pj3(jets[2].momentum());
      if (fabs(pj1.eta()) > 0.7 || fabs(pj2.eta()) > 0.7) vetoEvent;
      MSG_DEBUG("Jet 1 & 2 eta, pT requirements fulfilled");

      // Require that jets are back-to-back within 20 degrees in phi
      if ((PI - deltaPhi(pj1.phi(), pj2.phi())) > (20/180.0)*PI) vetoEvent;
      MSG_DEBUG("Jet 1 & 2 phi requirement fulfilled");

      const double weight = event.weight();
      _sumw += weight;

      // Fill histos
      _histJet1Et->fill(pj1.pT(), weight);
      _histJet2Et->fill(pj2.pT(), weight);
      _tmphistJet3eta->fill(pj3.eta(), weight);
      _tmphistR23->fill(deltaR(pj2, pj3), weight);

      // Calc and plot alpha
      const double dPhi = deltaPhi(pj3.phi(), pj2.phi());
      const double dH = sign(pj2.eta()) * (pj3.eta() - pj2.eta());
      const double alpha = atan(dH/dPhi);
      _tmphistAlpha->fill(alpha*180./PI, weight);
    }


    /// Apply bin-wise detector correction factors
    void finalize() {

      // Normal scalings
      normalize(_histJet1Et, 12.3);
      normalize(_histJet2Et, 12.3);

      // eta3 correction
      const double eta3_CDF_sim[] =
        { 0.0013, 0.0037, 0.0047, 0.0071, 0.0093, 0.0117, 0.0151, 0.0149, 0.0197, 0.0257,
          0.0344, 0.0409, 0.0481, 0.0454, 0.0394, 0.0409, 0.0387, 0.0387, 0.0322, 0.0313,
          0.0290, 0.0309, 0.0412, 0.0417, 0.0412, 0.0397, 0.0417, 0.0414, 0.0376, 0.0316,
          0.0270, 0.0186, 0.0186, 0.0132, 0.0127, 0.0106, 0.0071, 0.0040, 0.0020, 0.0013 };
      const double eta3_CDF_sim_err[] =
        { 0.0009, 0.0009, 0.0007, 0.0007, 0.0007, 0.0010, 0.0012, 0.0012, 0.0013, 0.0016,
          0.0017, 0.0020, 0.0020, 0.0022, 0.0020, 0.0020, 0.0018, 0.0018, 0.0016, 0.0017,
          0.0017, 0.0019, 0.0020, 0.0021, 0.0020, 0.0020, 0.0019, 0.0020, 0.0018, 0.0017,
          0.0017, 0.0014, 0.0014, 0.0009, 0.0010, 0.0009, 0.0009, 0.0008, 0.0008, 0.0009 };
      const double eta3_Ideal_sim[] =
        { 0.0017, 0.0030, 0.0033, 0.0062, 0.0062, 0.0112, 0.0177, 0.0164, 0.0196, 0.0274,
          0.0351, 0.0413, 0.0520, 0.0497, 0.0448, 0.0446, 0.0375, 0.0329, 0.0291, 0.0272,
          0.0233, 0.0288, 0.0384, 0.0396, 0.0468, 0.0419, 0.0459, 0.0399, 0.0355, 0.0329,
          0.0274, 0.0230, 0.0201, 0.0120, 0.0100, 0.0080, 0.0051, 0.0051, 0.0010, 0.0010 };
      for (size_t i = 0;  i < 40; ++i) {
        const double yval = _tmphistJet3eta->bin(i).area() * (eta3_CDF_sim[i]/eta3_Ideal_sim[i]);
        const double yerr = _tmphistJet3eta->bin(i).areaErr() * (eta3_CDF_sim_err[i]/eta3_Ideal_sim[i]);
        _histJet3eta->addPoint(_tmphistJet3eta->bin(i).midpoint(), yval/_sumw,
                               _tmphistJet3eta->bin(i).width()/2.0, yerr/_sumw);
      }

      // R23 correction
      const double R23_CDF_sim[] =
        { 0.0005, 0.0161, 0.0570, 0.0762, 0.0723, 0.0705, 0.0598, 0.0563, 0.0557, 0.0579,
          0.0538, 0.0522, 0.0486, 0.0449, 0.0418, 0.0361, 0.0326, 0.0304, 0.0252, 0.0212,
          0.0173, 0.0176, 0.0145, 0.0127, 0.0103, 0.0065, 0.0049, 0.0045, 0.0035, 0.0029,
          0.0024, 0.0014, 0.0011, 0.0010, 0.0009 };
      const double R23_CDF_sim_err[] =
        { 0.0013, 0.0009, 0.0022, 0.0029, 0.0026, 0.0024, 0.0022, 0.0025, 0.0023, 0.0024,
          0.0021, 0.0021, 0.0021, 0.0021, 0.0021, 0.0019, 0.0019, 0.0016, 0.0017, 0.0014,
          0.0010, 0.0014, 0.0012, 0.0013, 0.0010, 0.0011, 0.0010, 0.0010, 0.0010, 0.0011,
          0.0011, 0.0009, 0.0008, 0.0008, 0.0009 };
      const double R23_Ideal_sim[] =
        { 0.0005, 0.0176, 0.0585, 0.0862, 0.0843, 0.0756, 0.0673, 0.0635, 0.0586, 0.0619,
          0.0565, 0.0515, 0.0466, 0.0472, 0.0349, 0.0349, 0.0266, 0.0254, 0.0204, 0.0179,
          0.0142, 0.0134, 0.0101, 0.0090, 0.0080, 0.0034, 0.0030, 0.0033, 0.0027, 0.0021,
          0.0012, 0.0006, 0.0004, 0.0005, 0.0003 };
      for (size_t i = 0;  i < 35; ++i) {
        const double yval = _tmphistR23->bin(i).area() * (R23_CDF_sim[i]/R23_Ideal_sim[i]);
        const double yerr = _tmphistR23->bin(i).areaErr() * (R23_CDF_sim_err[i]/R23_Ideal_sim[i]);
        _histR23->addPoint(_tmphistR23->bin(i).midpoint(), yval/_sumw,
                           _tmphistR23->bin(i).width()/2.0, yerr/_sumw);
      }

      // alpha correction
      const double alpha_CDF_sim[] =
        { 0.0517, 0.0461, 0.0490, 0.0452, 0.0451, 0.0435, 0.0317, 0.0287, 0.0294, 0.0261,
          0.0231, 0.0220, 0.0233, 0.0192, 0.0213, 0.0166, 0.0176, 0.0146, 0.0136, 0.0156,
          0.0142, 0.0152, 0.0151, 0.0147, 0.0164, 0.0186, 0.0180, 0.0210, 0.0198, 0.0189,
          0.0197, 0.0211, 0.0270, 0.0236, 0.0243, 0.0269, 0.0257, 0.0276, 0.0246, 0.0286 };
      const double alpha_CDF_sim_err[] =
        { 0.0024, 0.0025, 0.0024, 0.0024, 0.0024, 0.0022, 0.0019, 0.0018, 0.0019, 0.0016,
          0.0017, 0.0017, 0.0019, 0.0013, 0.0017, 0.0014, 0.0016, 0.0013, 0.0012, 0.0009,
          0.0014, 0.0014, 0.0014, 0.0014, 0.0014, 0.0015, 0.0014, 0.0016, 0.0016, 0.0015,
          0.0016, 0.0016, 0.0019, 0.0017, 0.0019, 0.0018, 0.0018, 0.0018, 0.0018, 0.0019 };
      const double alpha_Ideal_sim[] =
        { 0.0552, 0.0558, 0.0583, 0.0550, 0.0495, 0.0433, 0.0393, 0.0346, 0.0331, 0.0296,
          0.0258, 0.0196, 0.0171, 0.0179, 0.0174, 0.0141, 0.0114, 0.0096, 0.0076, 0.0087,
          0.0099, 0.0079, 0.0102, 0.0114, 0.0124, 0.0130, 0.0165, 0.0160, 0.0177, 0.0190,
          0.0232, 0.0243, 0.0238, 0.0248, 0.0235, 0.0298, 0.0292, 0.0291, 0.0268, 0.0316 };
      for (size_t i = 0;  i < 40; ++i) {
        const double yval = _tmphistAlpha->bin(i).area() * (alpha_CDF_sim[i]/alpha_Ideal_sim[i]);
        const double yerr = _tmphistAlpha->bin(i).areaErr() * (alpha_CDF_sim_err[i]/alpha_Ideal_sim[i]);
        _histAlpha->addPoint(_tmphistAlpha->bin(i).midpoint(), yval/_sumw,
                             _tmphistAlpha->bin(i).width()/2.0, yerr/_sumw);
      }
    }

    //@}


  private:

    /// @name Event weight counters
    //@{

    double _sumw;

    //@}


    /// @name Histograms
    //@{

    /// Straightforward output histos
    Histo1DPtr _histJet1Et, _histJet2Et;

    /// Output histos which need to have correction factors applied
    Scatter2DPtr _histR23, _histJet3eta, _histAlpha;

    /// Temporary histos, to be converted to DPSes
    Histo1DPtr _tmphistR23, _tmphistJet3eta, _tmphistAlpha;

    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_1994_S2952106);

}

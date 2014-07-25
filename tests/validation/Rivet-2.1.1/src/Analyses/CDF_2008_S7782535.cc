// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/JetShape.hh"

namespace Rivet {


  /// @brief CDF Run II b-jet shape paper
  class CDF_2008_S7782535 : public Analysis {
  public:

    /// Constructor
    CDF_2008_S7782535() : Analysis("CDF_2008_S7782535")
    {
    }


    /// @name Analysis methods
    //@{

    void init() {
      // Set up projections
      const FinalState fs(-3.6, 3.6);
      addProjection(fs, "FS");
      FastJets jetproj(fs, FastJets::CDFMIDPOINT, 0.7);
      jetproj.useInvisibles();
      addProjection(jetproj, "Jets");

      // Book histograms and corresponding jet shape projections
      _ptedges += 52, 80, 104, 142, 300;
      for (size_t i = 0; i < 4; ++i) {
        stringstream ss; ss << "JetShape" << i;
        const string pname = ss.str();
        _jsnames_pT[i] = pname;
        const JetShape jsp(jetproj, 0.0, 0.7, 7, _ptedges[i], _ptedges[i+1], 0.0, 0.7, RAPIDITY);
        addProjection(jsp, pname);
        _h_Psi_pT[i] = bookProfile1D(i+1, 2, 1);
      }
      _h_OneMinusPsi_vs_pT = bookScatter2D(5, 1, 1);
    }


    // Do the analysis
    void analyze(const Event& event) {
      const Jets& jets = applyProjection<FastJets>(event, "Jets").jets(_ptedges.front()*GeV, _ptedges.back()*GeV,
                                                                       0.0, 0.7, RAPIDITY);
      MSG_DEBUG("Jet multiplicity before any pT cut = " << jets.size());
      if (jets.size() == 0) {
        MSG_DEBUG("No jets found in required pT range");
        vetoEvent;
      }

      // Filter to just get a vector of b-jets
      Jets bjets;
      foreach (const Jet& j, jets) {
        if (j.containsBottom()) bjets += j;
      }
      if (bjets.empty())  {
        MSG_DEBUG("No b-jet axes in acceptance");
        vetoEvent;
      }

      // Bin b-jets in pT
      Jets bjets_ptbinned[4];
      foreach (const Jet& bj, bjets) {
        const FourMomentum pbj = bj.momentum();
        const int ipt = binIndex(pbj.pT(), _ptedges);
        if (ipt == -1) continue; //< Out of pT range (somehow!)
        bjets_ptbinned[ipt] += bj;
      }

      // Loop over jet pT bins and fill shape profiles
      const double weight = event.weight();
      for (size_t ipt = 0; ipt < 4; ++ipt) {
        if (bjets_ptbinned[ipt].empty()) continue;
        // Don't use the cached result: copy construct and calculate for provided b-jets only
        JetShape jsipt = applyProjection<JetShape>(event, _jsnames_pT[ipt]);
        jsipt.calc(bjets_ptbinned[ipt]);
        for (size_t ijet = 0; ijet < jsipt.numJets(); ++ijet) {
          for (size_t rbin = 0; rbin < jsipt.numBins(); ++rbin) {
            const double r_Psi = jsipt.rBinMax(rbin);
            _h_Psi_pT[ipt]->fill(r_Psi/0.7, jsipt.intJetShape(ijet, rbin), weight);
          }
        }
      }

    }


    /// Finalize
    void finalize() {

      // Construct final 1-Psi(0.3/0.7) profile from Psi profiles
      for (size_t i = 0; i < _ptedges.size()-1; ++i) {
        // Get entry for rad_Psi = 0.2 bin
        Profile1DPtr ph_i = _h_Psi_pT[i];
        const double ex = 0.5*(_ptedges[i+1] - _ptedges[i]);
        const double x  = _ptedges[i] + ex;
        const double y  = 1.0 - ph_i->bin(1).mean();
        const double ey = ph_i->bin(1).stdErr();
        _h_OneMinusPsi_vs_pT->addPoint(x, y, ex, ey);
      }

    }

    //@}


  private:

    /// @name Analysis data
    //@{

    /// Jet \f$ p_\perp\f$ bins.
    vector<double> _ptedges; // This can't be a raw array if we want to initialise it non-painfully

    /// JetShape projection name for each \f$p_\perp\f$ bin.
    string _jsnames_pT[4];

    //@}


    /// @name Histograms
    //@{
    Profile1DPtr _h_Psi_pT[4];
    Scatter2DPtr _h_OneMinusPsi_vs_pT;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CDF_2008_S7782535);

}

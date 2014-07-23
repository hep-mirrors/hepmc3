// -*- C++ -*-
#ifndef RIVET_MC_JetAnalysis_HH
#define RIVET_MC_JetAnalysis_HH

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Base class providing common functionality for MC jet validation analyses
  class MC_JetAnalysis : public Analysis {
  public:

    /// Default constructor.
    MC_JetAnalysis(const string& name,
                   size_t njet,
                   const string& jetpro_name,
                   double jetptcut=20*GeV);


    /// @name Analysis methods
    //@{
    virtual void init();
    virtual void analyze(const Event& event);
    virtual void finalize();
    //@}


  protected:

    /// The number of jets for which histograms are to be initialised
    size_t m_njet;

    /// The name of the jet projection to be used for this analysis
    /// (this projection has to be registered by the derived analysis!)
    const std::string m_jetpro_name;

    /// Jet pT cutoff
    double m_jetptcut;

    /// @todo Add jet masses and d(rap)

    /// @name Histograms
    //@{
    std::vector<Histo1DPtr> _h_pT_jet;
    std::vector<Histo1DPtr> _h_eta_jet;
    std::vector<Histo1DPtr> _h_eta_jet_plus, _h_eta_jet_minus;
    std::vector<Histo1DPtr> _h_rap_jet;
    std::vector<Histo1DPtr> _h_rap_jet_plus, _h_rap_jet_minus;
    std::vector<Histo1DPtr> _h_mass_jet;
    std::map<std::pair<size_t, size_t>, Histo1DPtr> _h_deta_jets;
    std::map<std::pair<size_t, size_t>, Histo1DPtr> _h_dphi_jets;
    std::map<std::pair<size_t, size_t>, Histo1DPtr> _h_dR_jets;
    Histo1DPtr _h_jet_multi_exclusive;
    Histo1DPtr _h_jet_multi_inclusive;
    Scatter2DPtr _h_jet_multi_ratio;
    Histo1DPtr _h_jet_HT;
    //@}

  };

}

#endif

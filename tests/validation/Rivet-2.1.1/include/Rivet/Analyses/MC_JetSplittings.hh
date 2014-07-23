// -*- C++ -*-
#ifndef RIVET_MC_JetSplittings_HH
#define RIVET_MC_JetSplittings_HH

#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief Base class providing common functionality for MC jet validation analyses
  class MC_JetSplittings : public Analysis {
  public:

    /// Default constructor.
    MC_JetSplittings(const string& name,
                   size_t njet,
                   const string& jetpro_name);


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

    /// @name Histograms
    //@{
    std::vector<Histo1DPtr> _h_log10_d;
    std::vector<Scatter2DPtr> _h_log10_R;
    //@}

  };

}

#endif

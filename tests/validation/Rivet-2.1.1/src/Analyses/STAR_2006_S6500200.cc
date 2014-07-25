// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"

namespace Rivet {


  /// @brief STAR identified hadron spectra in pp at 200 GeV
  class STAR_2006_S6500200 : public Analysis {
  public:

    /// Constructor
    STAR_2006_S6500200()
      : Analysis("STAR_2006_S6500200"),
        _sumWeightSelected(0.0)
    {  }


    /// Book projections and histograms
    void init() {
      ChargedFinalState bbc1(-5.0,-3.3, 0.0*GeV); // beam-beam-counter trigger
      ChargedFinalState bbc2( 3.3, 5.0, 0.0*GeV); // beam-beam-counter trigger
      addProjection(bbc1, "BBC1");
      addProjection(bbc2, "BBC2");

      IdentifiedFinalState pionfs(-2.5, 2.5, 0.3*GeV);
      IdentifiedFinalState protonfs(-2.5, 2.5, 0.4*GeV);
      pionfs.acceptIdPair(PID::PIPLUS);
      protonfs.acceptIdPair(PID::PROTON);
      addProjection(pionfs, "PionFS");
      addProjection(protonfs, "ProtonFS");

      _h_pT_piplus     = bookHisto1D(1, 1, 1); // full range pion binning
      _h_pT_piminus    = bookHisto1D(1, 2, 1); // full range pion binning
      _tmp_pT_piplus   = bookHisto1D("TMP/pT_piplus", refData(2, 3, 1)); // pi histo compatible with more restricted proton binning
      _tmp_pT_piminus  = bookHisto1D("TMP/pT_piminus", refData(2, 4, 1)); // pi histo compatible with more restricted proton binning
      _h_pT_proton     = bookHisto1D(1, 3, 1);
      _h_pT_antiproton = bookHisto1D(1, 4, 1);

      _s_piminus_piplus = bookScatter2D(2, 1, 1);
      _s_antipr_pr      = bookScatter2D(2, 2, 1);
      _s_pr_piplus      = bookScatter2D(2, 3, 1);
      _s_antipr_piminus = bookScatter2D(2, 4, 1);
    }


    /// Do the analysis
    void analyze(const Event& event) {
      const ChargedFinalState& bbc1 = applyProjection<ChargedFinalState>(event, "BBC1");
      const ChargedFinalState& bbc2 = applyProjection<ChargedFinalState>(event, "BBC2");
      if (bbc1.size() < 1 || bbc2.size() < 1) {
        MSG_DEBUG("Failed beam-beam-counter trigger");
        vetoEvent;
      }

      const double weight = event.weight();

      const IdentifiedFinalState& pionfs = applyProjection<IdentifiedFinalState>(event, "PionFS");
      foreach (const Particle& p, pionfs.particles()) {
        if (fabs(p.rapidity()) < 0.5) {
          /// @todo Use a binned counter to avoid this bin width cancellation hack
          const double pT = p.pT() / GeV;
          ((p.pdgId() > 0) ? _h_pT_piplus : _h_pT_piminus)->fill(pT, weight/pT);
          ((p.pdgId() > 0) ? _tmp_pT_piplus : _tmp_pT_piminus)->fill(pT, weight/pT);
        }
      }

      const IdentifiedFinalState& protonfs = applyProjection<IdentifiedFinalState>(event, "ProtonFS");
      foreach (const Particle& p, protonfs.particles()) {
        if (fabs(p.rapidity()) < 0.5) {
          /// @todo Use a binned counter to avoid this bin width cancellation hack
          const double pT = p.pT() / GeV;
          ((p.pdgId() > 0) ? _h_pT_proton : _h_pT_antiproton)->fill(pT, weight/pT);
        }
      }
      _sumWeightSelected += event.weight();
    }


    /// Finalize
    void finalize() {
      divide(_h_pT_piminus, _h_pT_piplus, _s_piminus_piplus);
      divide(_h_pT_antiproton, _h_pT_proton, _s_antipr_pr);
      divide(_h_pT_proton, _tmp_pT_piplus, _s_pr_piplus);
      divide(_h_pT_antiproton, _tmp_pT_piminus, _s_antipr_piminus);
      scale(_h_pT_piplus,     1/(2*M_PI*_sumWeightSelected));
      scale(_h_pT_piminus,    1/(2*M_PI*_sumWeightSelected));
      scale(_h_pT_proton,     1/(2*M_PI*_sumWeightSelected));
      scale(_h_pT_antiproton, 1/(2*M_PI*_sumWeightSelected));
    }


  private:

    double _sumWeightSelected;
    Histo1DPtr _h_pT_piplus, _h_pT_piminus, _h_pT_proton, _h_pT_antiproton;
    Histo1DPtr _tmp_pT_piplus, _tmp_pT_piminus;
    Scatter2DPtr _s_piminus_piplus, _s_antipr_pr, _s_pr_piplus, _s_antipr_piminus;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(STAR_2006_S6500200);

}

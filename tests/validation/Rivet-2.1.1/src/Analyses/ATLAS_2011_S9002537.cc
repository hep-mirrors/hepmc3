// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  class ATLAS_2011_S9002537 : public Analysis {
  public:

    ATLAS_2011_S9002537()
      : Analysis("ATLAS_2011_S9002537")
    {  }


    void init() {
      IdentifiedFinalState Muons(-2.4, 2.4, 20*GeV);
      Muons.acceptIdPair(PID::MUON);
      addProjection(Muons, "muons");

      ChargedFinalState CFS(-2.8, 2.8, 0*GeV);
      addProjection(CFS, "tracks");

      MissingMomentum missmom(FinalState(-5,5, 0*GeV));
      addProjection(missmom, "MissingMomentum");

      /// @todo Will need to register TMP histograms for future histogramming
      _tmp_h_plus  = Histo1D(refData(1,1,1));
      _tmp_h_minus = Histo1D(refData(1,1,1));
      _h_asym = bookScatter2D(1, 1, 1);
    }


    void analyze(const Event& event) {

      const IdentifiedFinalState& muons = applyProjection<IdentifiedFinalState>(event, "muons");
      if (muons.size() < 1) vetoEvent;
      const ChargedFinalState& tracks = applyProjection<ChargedFinalState>(event, "tracks");

      Particles selected_muons;
      foreach (Particle muon, muons.particles()) {
        FourMomentum testmom = muon.momentum();
        double ptmu(testmom.pT()), ptsum(-ptmu), ratio(0.);
        foreach (Particle track, tracks.particles()) {
          const FourMomentum& trackmom = track.momentum();
          if (deltaR(testmom, trackmom) < 0.4) {
            ptsum += trackmom.pT();
            ratio  = ptsum/ptmu;
            if (ratio > 0.2) break;
          }
        }
        if (ratio < 0.2) selected_muons.push_back(muon);
      }
      if (selected_muons.size() < 1) vetoEvent;

      const FourMomentum muonmom = selected_muons[0].momentum();
      const MissingMomentum& missmom = applyProjection<MissingMomentum>(event, "MissingMomentum");
      FourMomentum missvec = -missmom.visibleMomentum();
      if (fabs(missvec.Et()) < 25*GeV) vetoEvent;

      double MTW = sqrt( 2 * missvec.pT() * muonmom.pT() * (1 - cos( deltaPhi(missvec.phi(), muonmom.phi()) )) );
      if (MTW < 40*GeV) vetoEvent;

      Histo1D& htmp = (selected_muons[0].pdgId() > 0) ? _tmp_h_minus : _tmp_h_plus;
      htmp.fill(muonmom.eta(), event.weight());
    }


    /// Normalise histograms etc., after the run
    void finalize() {
      assert(_tmp_h_plus.numBins() == _tmp_h_minus.numBins());
      for (size_t i = 0; i < _tmp_h_plus.numBins(); ++i) {
        const double num   = _tmp_h_plus.bin(i).sumW() - _tmp_h_minus.bin(i).sumW();
        const double denom = _tmp_h_plus.bin(i).sumW() + _tmp_h_minus.bin(i).sumW();
        const double relerr = _tmp_h_plus.bin(i).relErr()  + _tmp_h_minus.bin(i).relErr();
        const double asym = (num != 0 && denom != 0) ? num / denom : 0;
        const double asym_err = (num != 0 && denom != 0) ? asym*relerr : 0;
        _h_asym->addPoint(_tmp_h_plus.bin(i).midpoint(), asym, _tmp_h_plus.bin(i).width()/2.0, asym_err);
      }
    }


  private:

    Scatter2DPtr _h_asym;
    /// @todo Will need to register TMP histograms for future histogramming
    Histo1D  _tmp_h_plus, _tmp_h_minus;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2011_S9002537);

}

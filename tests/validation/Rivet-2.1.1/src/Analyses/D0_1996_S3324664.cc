// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  /// @brief D0 azimuthal correlation of jets widely separated in rapidity
  class D0_1996_S3324664 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor
    D0_1996_S3324664() : Analysis("D0_1996_S3324664")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      const FinalState fs;
      addProjection(fs, "FS");
      /// @todo Use correct jet algorithm
      addProjection(FastJets(fs, FastJets::D0ILCONE, 0.7), "ConeJets");

      _h_deta = bookHisto1D(1, 1, 1);
      _h_dphi.addHistogram(0.0, 2.0, bookHisto1D(2, 1, 1));
      _h_dphi.addHistogram(2.0, 4.0, bookHisto1D(2, 1, 2));
      _h_dphi.addHistogram(4.0, 6.0, bookHisto1D(2, 1, 3));
      _h_cosdphi_deta = bookProfile1D(3, 1, 1);
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      Jets jets;
      foreach (const Jet& jet, applyProjection<FastJets>(event, "ConeJets").jets(20.0*GeV)) {
        if (fabs(jet.eta()) < 3.0) {
          jets.push_back(jet);
        }
      }

      if (jets.size() < 2) {
        vetoEvent;
      }

      FourMomentum minjet = jets[0].momentum();
      FourMomentum maxjet = jets[1].momentum();
      double mineta = minjet.eta();
      double maxeta = maxjet.eta();

      foreach(const Jet& jet, jets) {
        double eta = jet.eta();
        if (eta < mineta) {
          minjet = jet.momentum();
          mineta = eta;
        }
        else if (eta > maxeta) {
          maxjet = jet.momentum();
          maxeta = eta;
        }
      }

      if (minjet.Et()<50*GeV && maxjet.Et()<50.0*GeV) {
        vetoEvent;
      }

      double deta = maxjet.eta()-minjet.eta();
      double dphi = mapAngle0To2Pi(maxjet.phi()-minjet.phi());

      _h_deta->fill(deta, weight);
      _h_dphi.fill(deta, 1.0-dphi/M_PI, weight);
      _h_cosdphi_deta->fill(deta, cos(M_PI-dphi), weight);

    }


    void finalize() {
      // Normalised to #events
      normalize(_h_deta, 8830.0); // fixed norm OK

      // I have no idea what this is normalised to... in the paper it says unity!
      /// @todo Understand this!
      foreach (Histo1DPtr histo, _h_dphi.getHistograms()) {
        /// @todo Prefer to scale rather than normalize, if possible
        normalize(histo, 0.0798);
      }

    }

    //@}


  private:

    /// @name Histograms
    //@{

    Histo1DPtr _h_deta;
    BinnedHistogram<double> _h_dphi;
    Profile1DPtr _h_cosdphi_deta;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_1996_S3324664);

}

// -*- C++ -*-
#include "Rivet/Analysis.hh"
// #include "Rivet/Projections/ChargedFinalState.hh"

namespace Rivet {


  /// Generic analysis looking at various distributions of final state particles
  class MC_PDFS : public Analysis {
  public:

    /// Constructor
    MC_PDFS()
      : Analysis("MC_PDFS")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {
      // Projections
      // addProjection(ChargedFinalState(-5.0, 5.0, 500*MeV), "CFS");

      // Histograms
      _histPdfX = bookHisto1D("PdfX", logspace(50, 0.000001, 1.0));
      _histPdfXmin = bookHisto1D("PdfXmin", logspace(50, 0.000001, 1.0));
      _histPdfXmax = bookHisto1D("PdfXmax", logspace(50, 0.000001, 1.0));
      _histPdfQ = bookHisto1D("PdfQ", 50, 0.0, 30.0);
      // _histPdfXQ = bookHisto2D("PdfXQ", logspace(50, 0.000001, 1.0), linspace(50, 0.0, 30.0));
      // _histPdfTrackptVsX = bookProfile1D("PdfTrackptVsX", logspace(50, 0.000001, 1.0));
      // _histPdfTrackptVsQ = bookProfile1D("PdfTrackptVsQ", 50, 0.0, 30.0);
    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {
      const double weight = event.weight();

      // This analysis needs a valid HepMC PDF info object to do anything
      if (event.genEvent()->pdf_info() == 0) vetoEvent;
      HepMC3::PdfInfo pdfi = *(event.genEvent()->pdf_info());

      MSG_DEBUG("PDF Q = " << pdfi.scalePDF << " for (id, x) = "
                << "(" << pdfi.id1 << ", " << pdfi.x1 << ") "
                << "(" << pdfi.id2 << ", " << pdfi.x2 << ")");
      _histPdfX->fill(pdfi.x1, weight);
      _histPdfX->fill(pdfi.x2, weight);
      _histPdfXmin->fill(std::min(pdfi.x1, pdfi.x2), weight);
      _histPdfXmax->fill(std::max(pdfi.x1, pdfi.x2), weight);
      _histPdfQ->fill(pdfi.scalePDF, weight); // always in GeV?

      // const FinalState& cfs = applyProjection<FinalState>(event, "CFS");
      // foreach (const Particle& p, cfs.particles()) {
      //   if (fabs(eta) < 2.5 && p.pT() > 10*GeV) {
      //     _histPdfTrackptVsX->fill(pdfi.x1(), p.pT()/GeV, weight);
      //     _histPdfTrackptVsX->fill(pdfi.x2(), p.pT()/GeV, weight);
      //     _histPdfTrackptVsQ->fill(pdfi.scalePDF(), p.pT()/GeV, weight);
      //   }
      // }

    }



    /// Finalize
    void finalize() {
      scale(_histPdfX, 1/sumOfWeights());
      scale(_histPdfXmin, 1/sumOfWeights());
      scale(_histPdfXmax, 1/sumOfWeights());
      scale(_histPdfQ, 1/sumOfWeights());
    }

    //@}


  private:

    /// @name Histograms
    //@{
    Histo1DPtr _histPdfX, _histPdfXmin, _histPdfXmax, _histPdfQ;
    // Profile1DPtr   _histPdfTrackptVsX, _histPdfTrackptVsQ;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_PDFS);

}

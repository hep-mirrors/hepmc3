// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/BinnedHistogram.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"

namespace Rivet {


   /// CMS ratio of 3-jet to 2-jet cross-sections
   class CMS_2011_S9088458 : public Analysis {
   public:


     CMS_2011_S9088458()
       : Analysis("CMS_2011_S9088458") {  }


     void init() {
       FinalState fs;
       FastJets akt(fs, FastJets::ANTIKT, 0.5);
       addProjection(akt, "antikT");

       _h_tmp_dijet = Histo1D(refData(1, 1, 1));
       _h_tmp_trijet = Histo1D(refData(1, 1, 1));
       _h_r32 = bookScatter2D(1, 1, 1);
     }


     void analyze(const Event & event) {
       const double weight = event.weight();

       Jets highpT_jets;
       double HT = 0;
       foreach(const Jet & jet, applyProjection<JetAlg>(event, "antikT").jetsByPt(50.0*GeV)) {
         if (fabs(jet.eta()) < 2.5) {
           highpT_jets.push_back(jet);
           HT += jet.pT();
         }
       }
       if (highpT_jets.size() < 2) vetoEvent;
       if (highpT_jets.size() >= 2) _h_tmp_dijet.fill(HT/TeV, weight);
       if (highpT_jets.size() >= 3) _h_tmp_trijet.fill(HT/TeV, weight);
     }


     void finalize() {
       divide(_h_tmp_trijet, _h_tmp_dijet, _h_r32);
     }


   private:

     Histo1D _h_tmp_dijet, _h_tmp_trijet;
     Scatter2DPtr _h_r32;

  };


  // A hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S9088458);

}

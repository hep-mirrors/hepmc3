// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/Beam.hh"
using namespace std;

namespace Rivet {

  class CMS_2011_S8884919 : public Analysis {
  public:

    CMS_2011_S8884919()
      : Analysis("CMS_2011_S8884919")
    {    }


    void init() {
      ChargedFinalState cfs(-2.4, 2.4, 0.0*GeV);
      addProjection(cfs, "CFS");

      // eta bins
      _etabins.push_back(0.5);
      _etabins.push_back(1.0);
      _etabins.push_back(1.5);
      _etabins.push_back(2.0);
      _etabins.push_back(2.4) ;

      if (fuzzyEquals(sqrtS()/GeV, 900)) {
        for (size_t ietabin=0; ietabin < _etabins.size(); ietabin++) {
          _h_dNch_dn.push_back( bookHisto1D( 2 + ietabin, 1, 1) );
        }
        _h_dNch_dn_pt500_eta24 = bookHisto1D(20, 1, 1);
        _h_dmpt_dNch_eta24 = bookProfile1D(23, 1, 1);
      }

      if (fuzzyEquals(sqrtS()/GeV, 2360)) {
        for (size_t ietabin=0; ietabin < _etabins.size(); ietabin++) {
          _h_dNch_dn.push_back( bookHisto1D(7 + ietabin, 1, 1) );
        }
        _h_dNch_dn_pt500_eta24 = bookHisto1D(21, 1, 1);
        _h_dmpt_dNch_eta24 = bookProfile1D(24, 1, 1);
      }

      if (fuzzyEquals(sqrtS()/GeV, 7000)) {
        for (size_t ietabin=0; ietabin < _etabins.size(); ietabin++) {
          _h_dNch_dn.push_back( bookHisto1D(12 + ietabin, 1, 1) );
        }
        _h_dNch_dn_pt500_eta24 = bookHisto1D(22, 1, 1);
        _h_dmpt_dNch_eta24 = bookProfile1D(25, 1, 1);
      }
    }


    void analyze(const Event& event) {
      const double weight = event.weight();

      // Get the charged particles
      const ChargedFinalState& charged = applyProjection<ChargedFinalState>(event, "CFS");

      // Resetting the multiplicity for the event to 0;
      vector<int> _nch_in_Evt;
      vector<int> _nch_in_Evt_pt500;
      _nch_in_Evt.assign(_etabins.size(), 0);
      _nch_in_Evt_pt500.assign(_etabins.size(), 0);
      double sumpt = 0;

      // Loop over particles in event
      foreach (const Particle& p, charged.particles()) {
        // Selecting only charged hadrons
        if (! PID::isHadron(p.pdgId())) continue;

        double pT = p.pT();
        double eta = p.eta();
        sumpt += pT;
        for (int ietabin = _etabins.size()-1; ietabin >= 0; --ietabin) {
          if (fabs(eta) <= _etabins[ietabin]){
            ++(_nch_in_Evt[ietabin]);
            if (pT > 0.5/GeV) ++(_nch_in_Evt_pt500[ietabin]);
          }
          else
            break;
        }
      }

      // Filling multiplicity-dependent histogramms
      for (size_t ietabin = 0; ietabin < _etabins.size(); ietabin++) {
        _h_dNch_dn[ietabin]->fill(_nch_in_Evt[ietabin], weight);
      }

      // Do only if eta bins are the needed ones
      if (_etabins[4] == 2.4 && _etabins[0] == 0.5) {
        if (_nch_in_Evt[4] != 0) {
          _h_dmpt_dNch_eta24->fill(_nch_in_Evt[4], sumpt/GeV / _nch_in_Evt[4], weight);
        }
        _h_dNch_dn_pt500_eta24->fill(_nch_in_Evt_pt500[4], weight);
      } else {
        MSG_WARNING("You changed the number of eta bins, but forgot to propagate it everywhere !!");
      }
    }


    void finalize() {
      for (size_t ietabin = 0; ietabin < _etabins.size(); ietabin++){
        normalize(_h_dNch_dn[ietabin]);
      }
      normalize(_h_dNch_dn_pt500_eta24);
    }


  private:

    vector<Histo1DPtr> _h_dNch_dn;
    Histo1DPtr _h_dNch_dn_pt500_eta24;
    Profile1DPtr _h_dmpt_dNch_eta24;

    vector<double> _etabins;

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_2011_S8884919);

}

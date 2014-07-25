// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/Thrust.hh"
#include "Rivet/Projections/Sphericity.hh"
#include "Rivet/Projections/ParisiTensor.hh"
#include "Rivet/Projections/Hemispheres.hh"
#include "Rivet/Projections/Beam.hh"

namespace Rivet {


  /// @brief ALEPH jet rates and event shapes at LEP 1 and 2
  class ALEPH_2004_S5765862 : public Analysis {
  public:

    ALEPH_2004_S5765862()
      : Analysis("ALEPH_2004_S5765862") , _initialisedJets(false),
        _initialisedSpectra(false), _weightedTotalChargedPartNum(0)
    {
    }


  public:

    void init() {
      _initialisedJets    = true;
      _initialisedSpectra = true;
      // TODO: According to the paper they seem to discard neutral particles
      //       between 1 and 2 GeV. That correction is included in the systematic
      //       uncertainties and overly complicated to program, so we ignore it.
      const FinalState fs;
      addProjection(fs, "FS");
      FastJets durhamjets(fs, FastJets::DURHAM, 0.7);
      durhamjets.useInvisibles(true);
      addProjection(durhamjets, "DurhamJets");

      const Thrust thrust(fs);
      addProjection(thrust, "Thrust");
      addProjection(Sphericity(fs), "Sphericity");
      addProjection(ParisiTensor(fs), "Parisi");
      addProjection(Hemispheres(thrust), "Hemispheres");

      const ChargedFinalState cfs;
      addProjection(Beam(), "Beams");
      addProjection(cfs, "CFS");

      // Histos
      // offset for the event shapes and jets
      int offset = 0;
      switch (int(sqrtS()/GeV + 0.5)) {
        case 91: offset = 0; break;
        case 133: offset = 1; break;
        case 161: offset = 2; break;
        case 172: offset = 3; break;
        case 183: offset = 4; break;
        case 189: offset = 5; break;
        case 200: offset = 6; break;
        case 206: offset = 7; break;
        default:
          _initialisedJets = false;
      }
      // event shapes
      if(_initialisedJets) {
        _h_thrust = bookHisto1D(offset+54, 1, 1);
        _h_heavyjetmass = bookHisto1D(offset+62, 1, 1);
        _h_totaljetbroadening = bookHisto1D(offset+70, 1, 1);
        _h_widejetbroadening = bookHisto1D(offset+78, 1, 1);
        _h_cparameter = bookHisto1D(offset+86, 1, 1);
        _h_thrustmajor = bookHisto1D(offset+94, 1, 1);
        _h_thrustminor = bookHisto1D(offset+102, 1, 1);
        _h_jetmassdifference = bookHisto1D(offset+110, 1, 1);
        _h_aplanarity = bookHisto1D(offset+118, 1, 1);
        _h_planarity  = offset==0 ? Histo1DPtr() : bookHisto1D(offset+125, 1, 1);
        _h_oblateness = bookHisto1D(offset+133, 1, 1);
        _h_sphericity = bookHisto1D(offset+141, 1, 1);

        // Durham n->m jet resolutions
        _h_y_Durham[0] = bookHisto1D(offset+149, 1, 1);   // y12 d149 ... d156
        _h_y_Durham[1] = bookHisto1D(offset+157, 1, 1);   // y23 d157 ... d164
        if (offset<6) { // there is no y34, y45 and y56 for 200 gev
          _h_y_Durham[2] = bookHisto1D(offset+165, 1, 1); // y34 d165 ... d172, but not 171
          _h_y_Durham[3] = bookHisto1D(offset+173, 1, 1); // y45 d173 ... d179
          _h_y_Durham[4] = bookHisto1D(offset+180, 1, 1); // y56 d180 ... d186
        }
        else if (offset==6) {
          _h_y_Durham[2].reset();
          _h_y_Durham[3].reset();
          _h_y_Durham[4].reset();
        }
        else if (offset==7) {
          _h_y_Durham[2] = bookHisto1D(172, 1, 1);
          _h_y_Durham[3] = bookHisto1D(179, 1, 1);
          _h_y_Durham[4] = bookHisto1D(186, 1, 1);
        }

        // Durham n-jet fractions
        _h_R_Durham[0] = bookHisto1D(offset+187, 1, 1); // R1 d187 ... d194
        _h_R_Durham[1] = bookHisto1D(offset+195, 1, 1); // R2 d195 ... d202
        _h_R_Durham[2] = bookHisto1D(offset+203, 1, 1); // R3 d203 ... d210
        _h_R_Durham[3] = bookHisto1D(offset+211, 1, 1); // R4 d211 ... d218
        _h_R_Durham[4] = bookHisto1D(offset+219, 1, 1); // R5 d219 ... d226
        _h_R_Durham[5] = bookHisto1D(offset+227, 1, 1); // R>=6 d227 ... d234
      }
      // offset for the charged particle distributions
      offset = 0;
      switch (int(sqrtS()/GeV + 0.5)) {
        case 133: offset = 0; break;
        case 161: offset = 1; break;
        case 172: offset = 2; break;
        case 183: offset = 3; break;
        case 189: offset = 4; break;
        case 196: offset = 5; break;
        case 200: offset = 6; break;
        case 206: offset = 7; break;
        default:
          _initialisedSpectra=false;
      }
      if (_initialisedSpectra) {
        _h_xp = bookHisto1D( 2+offset, 1, 1);
        _h_xi = bookHisto1D(11+offset, 1, 1);
        _h_xe = bookHisto1D(19+offset, 1, 1);
        _h_pTin  = bookHisto1D(27+offset, 1, 1);
        _h_pTout = offset!=7 ? Histo1DPtr() : bookHisto1D(35, 1, 1);
        _h_rapidityT = bookHisto1D(36+offset, 1, 1);
        _h_rapidityS = bookHisto1D(44+offset, 1, 1);
      }

      if (!_initialisedSpectra && !_initialisedJets) {
        MSG_WARNING("CoM energy of events sqrt(s) = " << sqrtS()/GeV
                    << " doesn't match any available analysis energy .");
      }
    }


    void analyze(const Event& e) {
      const double weight = e.weight();

      const Thrust& thrust = applyProjection<Thrust>(e, "Thrust");
      const Sphericity& sphericity = applyProjection<Sphericity>(e, "Sphericity");

      if(_initialisedJets) {
        bool LEP1 = fuzzyEquals(sqrtS(),91.2*GeV,0.01);
        // event shapes
        double thr = LEP1 ? thrust.thrust() : 1.0 - thrust.thrust();
        _h_thrust->fill(thr,weight);
        _h_thrustmajor->fill(thrust.thrustMajor(),weight);
        if(LEP1)
          _h_thrustminor->fill(log(thrust.thrustMinor()),weight);
        else
          _h_thrustminor->fill(thrust.thrustMinor(),weight);
        _h_oblateness->fill(thrust.oblateness(),weight);

        const Hemispheres& hemi = applyProjection<Hemispheres>(e, "Hemispheres");
        _h_heavyjetmass->fill(hemi.scaledM2high(),weight);
        _h_jetmassdifference->fill(hemi.scaledM2diff(),weight);
        _h_totaljetbroadening->fill(hemi.Bsum(),weight);
        _h_widejetbroadening->fill(hemi.Bmax(),weight);

        const ParisiTensor& parisi = applyProjection<ParisiTensor>(e, "Parisi");
        _h_cparameter->fill(parisi.C(),weight);

        _h_aplanarity->fill(sphericity.aplanarity(),weight);
        if(_h_planarity)
          _h_planarity->fill(sphericity.planarity(),weight);
        _h_sphericity->fill(sphericity.sphericity(),weight);

        // Jet rates
        const FastJets& durjet = applyProjection<FastJets>(e, "DurhamJets");
        double log10e = log10(exp(1.));
        if (durjet.clusterSeq()) {
          double logynm1=0.;
          double logyn;
          for (size_t i=0; i<5; ++i) {
            double yn = durjet.clusterSeq()->exclusive_ymerge_max(i+1);
            if (yn<=0.0) continue;
            logyn = -log(yn);
            if (_h_y_Durham[i]) {
              _h_y_Durham[i]->fill(logyn, weight);
            }
            if(!LEP1) logyn *= log10e;
            for (size_t j = 0; j < _h_R_Durham[i]->numBins(); ++j) {
              double val   = _h_R_Durham[i]->bin(j).xMin();
              double width = _h_R_Durham[i]->bin(j).width();
              if(-val<=logynm1) break;
              if(-val<logyn) {
                _h_R_Durham[i]->fill(val+0.5*width, weight*width);
              }
            }
            logynm1 = logyn;
          }
          for (size_t j = 0; j < _h_R_Durham[5]->numBins(); ++j) {
            double val   = _h_R_Durham[5]->bin(j).xMin();
            double width = _h_R_Durham[5]->bin(j).width();
            if(-val<=logynm1) break;
            _h_R_Durham[5]->fill(val+0.5*width, weight*width);
          }
        }
        if( !_initialisedSpectra) {
          const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(e, "CFS");
          const size_t numParticles = cfs.particles().size();
          _weightedTotalChargedPartNum += numParticles * weight;
        }
      }

      // charged particle distributions
      if(_initialisedSpectra) {
        const ChargedFinalState& cfs = applyProjection<ChargedFinalState>(e, "CFS");
        const size_t numParticles = cfs.particles().size();
        _weightedTotalChargedPartNum += numParticles * weight;
        const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
        const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                     beams.second.momentum().vector3().mod() ) / 2.0;
        foreach (const Particle& p, cfs.particles()) {
          const double xp = p.momentum().vector3().mod()/meanBeamMom;
          _h_xp->fill(xp   , weight);
          const double logxp = -std::log(xp);
          _h_xi->fill(logxp, weight);
          const double xe = p.momentum().E()/meanBeamMom;
          _h_xe->fill(xe   , weight);
          const double pTinT  = dot(p.momentum().vector3(), thrust.thrustMajorAxis());
          const double pToutT = dot(p.momentum().vector3(), thrust.thrustMinorAxis());
          _h_pTin->fill(fabs(pTinT/GeV), weight);
          if(_h_pTout) _h_pTout->fill(fabs(pToutT/GeV), weight);
          const double momT = dot(thrust.thrustAxis()        ,p.momentum().vector3());
          const double rapidityT = 0.5 * std::log((p.momentum().E() + momT) /
                                                  (p.momentum().E() - momT));
          _h_rapidityT->fill(fabs(rapidityT), weight);
          const double momS = dot(sphericity.sphericityAxis(),p.momentum().vector3());
          const double rapidityS = 0.5 * std::log((p.momentum().E() + momS) /
                                                  (p.momentum().E() - momS));
          _h_rapidityS->fill(fabs(rapidityS), weight);
        }
      }
    }

    void finalize() {
      if(!_initialisedJets && !_initialisedSpectra) return;

      if (_initialisedJets) {
        normalize(_h_thrust);
        normalize(_h_heavyjetmass);
        normalize(_h_totaljetbroadening);
        normalize(_h_widejetbroadening);
        normalize(_h_cparameter);
        normalize(_h_thrustmajor);
        normalize(_h_thrustminor);
        normalize(_h_jetmassdifference);
        normalize(_h_aplanarity);
        if(_h_planarity) normalize(_h_planarity);
        normalize(_h_oblateness);
        normalize(_h_sphericity);

        for (size_t n=0; n<6; ++n) {
          scale(_h_R_Durham[n], 1./sumOfWeights());
        }

        for (size_t n = 0; n < 5; ++n) {
          if (_h_y_Durham[n]) {
            scale(_h_y_Durham[n], 1.0/sumOfWeights());
          }
        }
      }

      Histo1D temphisto(refData(1, 1, 1));
      const double avgNumParts = _weightedTotalChargedPartNum / sumOfWeights();
      Scatter2DPtr  mult = bookScatter2D(1, 1, 1);
      for (size_t b = 0; b < temphisto.numBins(); b++) {
        const double x  = temphisto.bin(b).midpoint();
        const double ex = temphisto.bin(b).width()/2.;
        if (inRange(sqrtS()/GeV, x-ex, x+ex)) {
          mult->addPoint(x, avgNumParts, ex, 0.);
        }
      }

      if (_initialisedSpectra) {
        normalize(_h_xp, avgNumParts);
        normalize(_h_xi, avgNumParts);
        normalize(_h_xe, avgNumParts);
        normalize(_h_pTin , avgNumParts);
        if (_h_pTout) normalize(_h_pTout, avgNumParts);
        normalize(_h_rapidityT, avgNumParts);
        normalize(_h_rapidityS, avgNumParts);
      }
    }

  private:

    bool _initialisedJets;
    bool _initialisedSpectra;

    Histo1DPtr _h_xp;
    Histo1DPtr _h_xi;
    Histo1DPtr _h_xe;
    Histo1DPtr _h_pTin;
    Histo1DPtr _h_pTout;
    Histo1DPtr _h_rapidityT;
    Histo1DPtr _h_rapidityS;
    Histo1DPtr _h_thrust;
    Histo1DPtr _h_heavyjetmass;
    Histo1DPtr _h_totaljetbroadening;
    Histo1DPtr _h_widejetbroadening;
    Histo1DPtr _h_cparameter;
    Histo1DPtr _h_thrustmajor;
    Histo1DPtr _h_thrustminor;
    Histo1DPtr _h_jetmassdifference;
    Histo1DPtr _h_aplanarity;
    Histo1DPtr _h_planarity;
    Histo1DPtr _h_oblateness;
    Histo1DPtr _h_sphericity;

    Histo1DPtr _h_R_Durham[6];
    Histo1DPtr _h_y_Durham[5];

    double _weightedTotalChargedPartNum;

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ALEPH_2004_S5765862);

}

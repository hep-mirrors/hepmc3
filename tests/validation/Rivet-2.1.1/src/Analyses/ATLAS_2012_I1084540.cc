// -*- C++ -*-
/**
 * @name ATLAS Diffractive Gaps Rivet Analysis
 * @author Tim Martin, tim.martin@cern.ch
 * @version 1.0
 * @date 16/01/2012
 * @see http://arxiv.org/abs/1201.2808
 * @note pp, sqrt(s) = 7 TeV
 * @note Rapidity gap finding algorithm designed to compliment
 * the ATLAS detector acceptance. Forward rapidity gap sizes
 * are calculated for each event, considering all stable
 * particles above pT cut values 200, 400, 600 and 800 MeV in
 * turn. A forward rapidity gap is defined to be the largest
 * continuous region stretching inward from either edge of the
 * detector at eta = |4.9| which contains zero particles above
 * pT Cut. Soft diffractive topologies are isolated at large
 * gap sizes.
 *
 */
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"

namespace Rivet {


  class ATLAS_2012_I1084540 : public Analysis {
  public:

    ATLAS_2012_I1084540() : Analysis("ATLAS_2012_I1084540"), _etaBins(49), _etaMax(4.9) {
    }


    /// @name Analysis methods
    //@{
    /// Book histograms and initialise projections before the run
    void init() {
      //All final states. Rapidity range = ATLAS calorimetry. Lowest pT cut = 200 MeV.
      const FinalState cnfs2(-_etaMax, _etaMax, 0.2 * GeV);
      const FinalState cnfs4(-_etaMax, _etaMax, 0.4 * GeV);
      const FinalState cnfs6(-_etaMax, _etaMax, 0.6 * GeV);
      const FinalState cnfs8(-_etaMax, _etaMax, 0.8 * GeV);
      addProjection(cnfs2, "CNFS2");
      addProjection(cnfs4, "CNFS4");
      addProjection(cnfs6, "CNFS6");
      addProjection(cnfs8, "CNFS8");

      _etaBinSize = (2. * _etaMax)/(double)_etaBins;

      //Book histogram
      _h_DeltaEtaF_200 = bookHisto1D(1, 1, 1);
      _h_DeltaEtaF_400 = bookHisto1D(2, 1, 1);
      _h_DeltaEtaF_600 = bookHisto1D(3, 1, 1);
      _h_DeltaEtaF_800 = bookHisto1D(4, 1, 1);
    }

  private:
    void fillMap(const FinalState& fs, bool* energyMap, double pTcut) {
      // Fill true/false array by iterating over particles and compare their
      // pT with pTcut
      foreach (const Particle& p, fs.particlesByEta()) {
        int checkBin = -1;
        double checkEta = -_etaMax;
        while (1) {
          checkEta += _etaBinSize;
          ++checkBin;
          if (p.eta() < checkEta) {
            energyMap[checkBin] = (p.pT() > pTcut * GeV);
            break;
          }
        }
      }
    }

  public:
    /// Perform the per-event analysis
    void analyze(const Event& event) {
      static unsigned int event_count = 0;
      ++event_count;
      const double weight = event.weight();
      const FinalState& fs2 = applyProjection<FinalState>(event, "CNFS2");
      const FinalState& fs4 = applyProjection<FinalState>(event, "CNFS4");
      const FinalState& fs6 = applyProjection<FinalState>(event, "CNFS6");
      const FinalState& fs8 = applyProjection<FinalState>(event, "CNFS8");

      // Set up Yes/No arrays for energy in each eta bin at each pT cut
      bool* energyMap_200 = new bool[_etaBins];
      bool* energyMap_400 = new bool[_etaBins];
      bool* energyMap_600 = new bool[_etaBins];
      bool* energyMap_800 = new bool[_etaBins];
      for (int i = 0; i < _etaBins; ++i) {
        energyMap_200[i] = false;
        energyMap_400[i] = false;
        energyMap_600[i] = false;
        energyMap_800[i] = false;
      }

      // Veto bins based on final state particles > Cut (Where Cut = 200 - 800 MeV pT)
      fillMap(fs2, energyMap_200, 0.2);
      fillMap(fs4, energyMap_400, 0.4);
      fillMap(fs6, energyMap_600, 0.6);
      fillMap(fs8, energyMap_800, 0.8);

      // Apply gap finding algorithm
      // Detector layout follows...
      // -Eta [Proton  ---  DetectorCSide  ---  DetectorBarrel  ---  DetectorASide  ---  Proton] +Eta
      bool gapDirectionAt200 = false; //False is gap on C size, True is gap on A side.
      double largestEdgeGap_200 = 0.;
      double largestEdgeGap_400 = 0.;
      double largestEdgeGap_600 = 0.;
      double largestEdgeGap_800 = 0.;

      for (int E = 200; E <= 800; E += 200) {
        double EdgeGapSizeA = -1, EdgeGapSizeC = -1;
        bool* energyMap = 0;
        switch (E) {
          case 200: energyMap = energyMap_200; break;
          case 400: energyMap = energyMap_400; break;
          case 600: energyMap = energyMap_600; break;
          case 800: energyMap = energyMap_800; break;
        }

        // Look left to right
        for (int a = 0; a < _etaBins; ++a) {
          if (energyMap[a] == true) {
            EdgeGapSizeA = (_etaBinSize * a);
            break;
          }
        }

        // And look right to left
        for (int c = _etaBins-1; c >= 0; --c) {
          if (energyMap[c] == true) {
            EdgeGapSizeC = (2 * _etaMax) - (_etaBinSize * (c+1));
            if (fuzzyEquals(EdgeGapSizeC, 4.47035e-08)) EdgeGapSizeC = 0.0;
            break;
          }
        }
        // Put your hands on your hips

        // Find the largest gap
        double largestEdgeGap = 0.;
        if (E == 200) {
          // If the 200 MeV pass, take the biggest of the two gaps. Make note of which side for higher pT cuts.
          largestEdgeGap = std::max(EdgeGapSizeA,EdgeGapSizeC);
          gapDirectionAt200 = (EdgeGapSizeA > EdgeGapSizeC);
        } else {
          // Use the direction from 200 MeV pass, most accurate measure of which side gap is on.
          if (gapDirectionAt200) {
            largestEdgeGap = EdgeGapSizeA;
          }
          else largestEdgeGap = EdgeGapSizeC;
        }

        // Check case of empty detector
        if (largestEdgeGap < 0.0) largestEdgeGap = 2.0 * _etaMax;

        // Fill bin centre
        switch (E) {
          case 200: _h_DeltaEtaF_200->fill(largestEdgeGap + _etaBinSize/2., weight); break;
          case 400: _h_DeltaEtaF_400->fill(largestEdgeGap + _etaBinSize/2., weight); break;
          case 600: _h_DeltaEtaF_600->fill(largestEdgeGap + _etaBinSize/2., weight); break;
          case 800: _h_DeltaEtaF_800->fill(largestEdgeGap + _etaBinSize/2., weight); break;
        }

        if (E == 200) largestEdgeGap_200 = largestEdgeGap;
        if (E == 400) largestEdgeGap_400 = largestEdgeGap;
        if (E == 600) largestEdgeGap_600 = largestEdgeGap;
        if (E == 800) largestEdgeGap_800 = largestEdgeGap;
      }

      // Algorithm result every 1000 events
      if (event_count % 1000 == 0) {
        for (int E = 200; E <= 800; E += 200) {
          bool* energyMap = 0;
          double largestEdgeGap = 0;
          switch (E) {
            case 200: energyMap = energyMap_200; largestEdgeGap = largestEdgeGap_200; break;
            case 400: energyMap = energyMap_400; largestEdgeGap = largestEdgeGap_400; break;
            case 600: energyMap = energyMap_600; largestEdgeGap = largestEdgeGap_600; break;
            case 800: energyMap = energyMap_800; largestEdgeGap = largestEdgeGap_800; break;
          }
          MSG_DEBUG("Largest Forward Gap at pT Cut " << E << " MeV=" << largestEdgeGap
            << " eta, NFinalState pT > 200 in ATLAS acceptance:" << fs2.particles().size());
          std::string hitPattern = "Detector HitPattern=-4.9[";
          for (int a = 0; a < _etaBins; ++a) {
            if (energyMap[a] == true) hitPattern += "X";
            else hitPattern += "_";
          }
          hitPattern += "]4.9";
          MSG_DEBUG(hitPattern);
          std::string gapArrow = "                         ";
            if (!gapDirectionAt200) {
            int drawSpaces = (int)(_etaBins - (largestEdgeGap/_etaBinSize) + 0.5);
            for (int i = 0; i < drawSpaces; ++i) gapArrow += " ";
          }
          int drawArrows = (int)((largestEdgeGap/_etaBinSize) + 0.5);
          for (int i = 0; i < drawArrows; ++i) gapArrow += "^";
          MSG_DEBUG(gapArrow);
        }
      }

      delete[] energyMap_200;
      delete[] energyMap_400;
      delete[] energyMap_600;
      delete[] energyMap_800;
    }

    /// Normalise histograms after the run, Scale to cross section
    void finalize() {
      MSG_DEBUG("Cross Section=" << crossSection() / millibarn << "mb, SumOfWeights=" << sumOfWeights());
      scale(_h_DeltaEtaF_200, (crossSection() / millibarn)/sumOfWeights());
      scale(_h_DeltaEtaF_400, (crossSection() / millibarn)/sumOfWeights());
      scale(_h_DeltaEtaF_600, (crossSection() / millibarn)/sumOfWeights());
      scale(_h_DeltaEtaF_800, (crossSection() / millibarn)/sumOfWeights());
    }
    //@}

  private:
    /// @name Histograms
    //@{
    Histo1DPtr _h_DeltaEtaF_200;
    Histo1DPtr _h_DeltaEtaF_400;
    Histo1DPtr _h_DeltaEtaF_600;
    Histo1DPtr _h_DeltaEtaF_800;
    //@}
    /// @name Private variables
    //@{
    int _etaBins;
    double _etaMax;
    double _etaBinSize;
    //@}
  };

  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ATLAS_2012_I1084540);

}

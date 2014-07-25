// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/LeadingParticlesFinalState.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {


  /// @brief D0 Run I differential W/Z boson cross-section analysis
  /// @author Lars Sonnenschein
  class D0_2001_S4674421 : public Analysis {
  public:

    /// @name Constructors etc.
    //@{

    /// Constructor.
    D0_2001_S4674421()
      : Analysis("D0_2001_S4674421")
    {    }


    /// @name Analysis methods
    //@{

    void init() {
      // Final state projection
      FinalState fs(-5.0, 5.0); // corrected for detector acceptance
      addProjection(fs, "FS");

      // Z -> e- e+
      LeadingParticlesFinalState eeFS(FinalState(-5.0, 5.0, 0.)); //20.);
      eeFS.addParticleIdPair(PID::ELECTRON);
      addProjection(eeFS, "eeFS");

      // W- -> e- nu_e~
      LeadingParticlesFinalState enuFS(FinalState(-5.0, 5.0, 0.)); //25.);
      enuFS.addParticleId(PID::ELECTRON).addParticleId(PID::NU_EBAR);
      addProjection(enuFS, "enuFS");

      // W+ -> e+ nu_e
      LeadingParticlesFinalState enubFS(FinalState(-5.0, 5.0, 0.)); //25.);
      enubFS.addParticleId(PID::POSITRON).addParticleId(PID::NU_E);
      addProjection(enubFS, "enubFS");

      // Remove neutrinos for isolation of final state particles
      VetoedFinalState vfs(fs);
      vfs.vetoNeutrinos();
      addProjection(vfs, "VFS");

      // Counters
      _eventsFilledW = 0.0;
      _eventsFilledZ = 0.0;

      // Histograms
      _h_dsigdpt_w = bookHisto1D(1, 1, 1);
      _h_dsigdpt_z = bookHisto1D(1, 1, 2);
      _h_dsigdpt_scaled_z = bookScatter2D(2, 1, 1);
    }



    void analyze(const Event& event) {
      const double weight = event.weight();

      const LeadingParticlesFinalState& eeFS = applyProjection<LeadingParticlesFinalState>(event, "eeFS");
      // Z boson analysis
      if (eeFS.particles().size() >= 2) {
        // If there is a Z candidate:
        // Fill Z pT distributions
        double deltaM2=1e30,mass2(0.);
        double pT=-1.;
        const Particles& Zdaughters = eeFS.particles();
        for(unsigned int ix=0;ix<Zdaughters.size();++ix) {
          for(unsigned int iy=ix+1;iy<Zdaughters.size();++iy) {
            if(Zdaughters[ix].pdgId()!=-Zdaughters[iy].pdgId()) continue;
            const FourMomentum pmom = Zdaughters[ix].momentum() + Zdaughters[iy].momentum();
            double mz2 = pmom.mass2();
            double dm2 = abs(mz2-sqr(91.118*GeV));
            if(dm2<deltaM2) {
              pT = pmom.pT();
              deltaM2 = dm2;
              mass2 = mz2;
            }
          }
        }
        if (pT>0. && mass2 > 0. && inRange(sqrt(mass2)/GeV, 75.0, 105.0)) {
          _eventsFilledZ += weight;
          MSG_DEBUG("Z pmom.pT() = " << pT/GeV << " GeV");
          _h_dsigdpt_z->fill(pT/GeV, weight);
          // return if found a Z
          return;
        }
      }
      // There is no Z -> ee candidate... so this might be a W event
      const LeadingParticlesFinalState& enuFS = applyProjection<LeadingParticlesFinalState>(event, "enuFS");
      const LeadingParticlesFinalState& enubFS = applyProjection<LeadingParticlesFinalState>(event, "enubFS");

      double deltaM2=1e30;
      double pT=-1.;
      for(unsigned int iw=0;iw<2;++iw) {
        Particles Wdaughters;
        Wdaughters = iw==0 ? enuFS.particles() : enubFS.particles();
        for(unsigned int ix=0;ix<Wdaughters.size();++ix) {
          for(unsigned int iy=ix+1;iy<Wdaughters.size();++iy) {
            if(Wdaughters[ix].pdgId()==Wdaughters[iy].pdgId())  continue;
            const FourMomentum pmom = Wdaughters[0].momentum() + Wdaughters[1].momentum();
            double dm2 = abs(pmom.mass2()-sqr(80.4*GeV));
            if(dm2<deltaM2) {
              pT = pmom.pT();
              deltaM2 = dm2;
            }
          }
        }
      }
      if(pT>0.) {
        _eventsFilledW += weight;
        _h_dsigdpt_w->fill(pT/GeV, weight);
      }
    }



    void finalize() {
      // Get cross-section per event (i.e. per unit weight) from generator
      const double xSecPerEvent = crossSectionPerEvent()/picobarn;

      // Correct W pT distribution to W cross-section
      const double xSecW = xSecPerEvent * _eventsFilledW;

      // Correct Z pT distribution to Z cross-section
      const double xSecZ = xSecPerEvent * _eventsFilledZ;

      // Get W and Z pT integrals
      const double wpt_integral = integral(_h_dsigdpt_w);
      const double zpt_integral = integral(_h_dsigdpt_z);

      // Divide and scale ratio histos
      if (xSecW == 0 || wpt_integral == 0 || xSecZ == 0 || zpt_integral == 0) {
        MSG_WARNING("Not filling ratio plot because input histos are empty");
      } else {
        // Scale factor converts event counts to cross-sections, and inverts the
        // branching ratios since only one decay channel has been analysed for each boson.
        // Oh, and we put MW/MZ in, like they do in the paper.
        const double MW_MZ = 0.8820; // Ratio M_W/M_Z
        const double BRZEE_BRWENU = 0.033632 / 0.1073; // Ratio of branching fractions
        const double scalefactor = (xSecW / wpt_integral) / (xSecZ / zpt_integral) * MW_MZ * BRZEE_BRWENU;
        for (size_t ibin = 0; ibin < _h_dsigdpt_w->numBins(); ibin++) {
          const double xval = _h_dsigdpt_w->bin(ibin).midpoint();
          const double xerr = _h_dsigdpt_w->bin(ibin).width() / 2.;
          double yval(0), yerr(0);
          if (_h_dsigdpt_w->bin(ibin).sumW() != 0 && _h_dsigdpt_z->bin(ibin).sumW() != 0) {
            yval = scalefactor * _h_dsigdpt_w->bin(ibin).sumW() / _h_dsigdpt_z->bin(ibin).sumW();
            yerr = yval * sqrt( sqr(_h_dsigdpt_w->bin(ibin).relErr()) + sqr(_h_dsigdpt_z->bin(ibin).areaErr()) );
          }
          _h_dsigdpt_scaled_z->addPoint(xval, yval, xerr, yerr);
        }
      }

      // Normalize non-ratio histos
      normalize(_h_dsigdpt_w, xSecW);
      normalize(_h_dsigdpt_z, xSecZ);
    }


    //@}

  private:

    /// @name Event counters for cross section normalizations
    //@{
    double _eventsFilledW;
    double _eventsFilledZ;
    //@}

    //@{
    /// Histograms
    Histo1DPtr  _h_dsigdpt_w;
    Histo1DPtr  _h_dsigdpt_z;
    Scatter2DPtr _h_dsigdpt_scaled_z;
    //@}

  };



  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(D0_2001_S4674421);

}

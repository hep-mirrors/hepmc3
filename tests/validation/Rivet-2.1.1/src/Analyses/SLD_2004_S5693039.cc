// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/Beam.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Projections/InitialQuarks.hh"
#include "Rivet/Projections/Thrust.hh"

namespace Rivet {


  /// @brief SLD flavour-dependent fragmentation paper
  /// @author Peter Richardson
  class SLD_2004_S5693039 : public Analysis {
  public:

    /// Constructor
    SLD_2004_S5693039() : Analysis("SLD_2004_S5693039"),
        _weightedTotalChargedPartNumLight(0.),
        _weightedTotalChargedPartNumCharm(0.),
        _weightedTotalChargedPartNumBottom(0.),
        _weightLight(0.),_weightCharm(0.),_weightBottom(0.)
    {}

    /// @name Analysis methods
    //@{

    void analyze(const Event& e) {
      // First, veto on leptonic events by requiring at least 2 charged FS particles
      const FinalState& fs = applyProjection<FinalState>(e, "FS");
      const size_t numParticles = fs.particles().size();

      // Even if we only generate hadronic events, we still need a cut on numCharged >= 2.
      if (numParticles < 2) {
        MSG_DEBUG("Failed ncharged cut");
        vetoEvent;
      }
      MSG_DEBUG("Passed ncharged cut");
      // Get event weight for histo filling
      const double weight = e.weight();
      // Get beams and average beam momentum
      const ParticlePair& beams = applyProjection<Beam>(e, "Beams").beams();
      const double meanBeamMom = ( beams.first.momentum().vector3().mod() +
                                   beams.second.momentum().vector3().mod() ) / 2.0;
      MSG_DEBUG("Avg beam momentum = " << meanBeamMom);
      int flavour = 0;
      const InitialQuarks& iqf = applyProjection<InitialQuarks>(e, "IQF");

      // If we only have two quarks (qqbar), just take the flavour.
      // If we have more than two quarks, look for the highest energetic q-qbar pair.
      Particles quarks;
      if (iqf.particles().size() == 2) {
        flavour = abs( iqf.particles().front().pdgId() );
        quarks = iqf.particles();
      }
      else {
        map<int, Particle > quarkmap;
        foreach (const Particle& p, iqf.particles()) {
          if (quarkmap.find(p.pdgId())==quarkmap.end())
            quarkmap[p.pdgId()] = p;
          else if (quarkmap[p.pdgId()].momentum().E() < p.momentum().E())
            quarkmap[p.pdgId()] = p;
        }
        double maxenergy = 0.;
        for (int i = 1; i <= 5; ++i) {
          double energy(0.);
          if(quarkmap.find( i)!=quarkmap.end())
            energy += quarkmap[ i].momentum().E();
          if(quarkmap.find(-i)!=quarkmap.end())
            energy += quarkmap[-i].momentum().E();
          if (energy > maxenergy) flavour = i;
        }
        if(quarkmap.find( flavour)!=quarkmap.end())
          quarks.push_back(quarkmap[ flavour]);
        if(quarkmap.find(-flavour)!=quarkmap.end())
          quarks.push_back(quarkmap[-flavour]);
      }
      // total multiplicities
      switch (flavour) {
      case PID::DQUARK:
      case PID::UQUARK:
      case PID::SQUARK:
        _weightLight  += weight;
        _weightedTotalChargedPartNumLight  += numParticles * weight;
        break;
      case PID::CQUARK:
        _weightCharm  += weight;
        _weightedTotalChargedPartNumCharm  += numParticles * weight;
        break;
      case PID::BQUARK:
        _weightBottom += weight;
        _weightedTotalChargedPartNumBottom += numParticles * weight;
        break;
      }
      // thrust axis for projections
      Vector3 axis = applyProjection<Thrust>(e, "Thrust").thrustAxis();
      double dot(0.);
      if(!quarks.empty()) {
        dot = quarks[0].momentum().vector3().dot(axis);
        if(quarks[0].pdgId()<0) dot *= -1.;
      }
      // spectra and individual multiplicities
      foreach (const Particle& p, fs.particles()) {
        double pcm = p.momentum().vector3().mod();
        const double xp = pcm/meanBeamMom;

        // if in quark or antiquark hemisphere
        bool quark = p.momentum().vector3().dot(axis)*dot>0.;

        _h_PCharged ->fill(pcm     , weight);
        // all charged
        switch (flavour) {
        case PID::DQUARK:
        case PID::UQUARK:
        case PID::SQUARK:
          _h_XpChargedL->fill(xp, weight);
          break;
        case PID::CQUARK:
          _h_XpChargedC->fill(xp, weight);
          break;
        case PID::BQUARK:
          _h_XpChargedB->fill(xp, weight);
          break;
        }

        int id = abs(p.pdgId());
        // charged pions
        if (id == PID::PIPLUS) {
          _h_XpPiPlus->fill(xp, weight);
          _h_XpPiPlusTotal->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _h_XpPiPlusL->fill(xp, weight);
            _h_NPiPlusL->fill(sqrtS(), weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RPiPlus->fill(xp, weight);
            else
              _h_RPiMinus->fill(xp, weight);
            break;
          case PID::CQUARK:
            _h_XpPiPlusC->fill(xp, weight);
            _h_NPiPlusC->fill(sqrtS(), weight);
            break;
          case PID::BQUARK:
            _h_XpPiPlusB->fill(xp, weight);
            _h_NPiPlusB->fill(sqrtS(), weight);
            break;
          }
        }
        else if (id == PID::KPLUS) {
          _h_XpKPlus->fill(xp, weight);
          _h_XpKPlusTotal->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _h_XpKPlusL->fill(xp, weight);
            _h_NKPlusL->fill(sqrtS(), weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RKPlus->fill(xp, weight);
            else
              _h_RKMinus->fill(xp, weight);
            break;
          case PID::CQUARK:
            _h_XpKPlusC->fill(xp, weight);
            _h_NKPlusC->fill(sqrtS(), weight);
            break;
          case PID::BQUARK:
            _h_XpKPlusB->fill(xp, weight);
            _h_NKPlusB->fill(sqrtS(), weight);
            break;
          }
        }
        else if (id == PID::PROTON) {
          _h_XpProton->fill(xp, weight);
          _h_XpProtonTotal->fill(xp, weight);
          switch (flavour) {
          case PID::DQUARK:
          case PID::UQUARK:
          case PID::SQUARK:
            _h_XpProtonL->fill(xp, weight);
            _h_NProtonL->fill(sqrtS(), weight);
            if( ( quark && p.pdgId()>0 ) || ( !quark && p.pdgId()<0 ))
              _h_RProton->fill(xp, weight);
            else
              _h_RPBar  ->fill(xp, weight);
            break;
          case PID::CQUARK:
            _h_XpProtonC->fill(xp, weight);
            _h_NProtonC->fill(sqrtS(), weight);
            break;
          case PID::BQUARK:
            _h_XpProtonB->fill(xp, weight);
            _h_NProtonB->fill(sqrtS(), weight);
            break;
          }
        }
      }
    }


    void init() {
      // Projections
      addProjection(Beam(), "Beams");
      addProjection(ChargedFinalState(), "FS");
      addProjection(InitialQuarks(), "IQF");
      addProjection(Thrust(FinalState()), "Thrust");

      // Book histograms
      _h_PCharged   = bookHisto1D( 1, 1, 1);
      _h_XpPiPlus   = bookHisto1D( 2, 1, 2);
      _h_XpKPlus    = bookHisto1D( 3, 1, 2);
      _h_XpProton   = bookHisto1D( 4, 1, 2);
      _h_XpPiPlusTotal = bookHisto1D( 2, 2, 2);
      _h_XpKPlusTotal  = bookHisto1D( 3, 2, 2);
      _h_XpProtonTotal = bookHisto1D( 4, 2, 2);
      _h_XpPiPlusL  = bookHisto1D( 5, 1, 1);
      _h_XpPiPlusC  = bookHisto1D( 5, 1, 2);
      _h_XpPiPlusB  = bookHisto1D( 5, 1, 3);
      _h_XpKPlusL   = bookHisto1D( 6, 1, 1);
      _h_XpKPlusC   = bookHisto1D( 6, 1, 2);
      _h_XpKPlusB   = bookHisto1D( 6, 1, 3);
      _h_XpProtonL  = bookHisto1D( 7, 1, 1);
      _h_XpProtonC  = bookHisto1D( 7, 1, 2);
      _h_XpProtonB  = bookHisto1D( 7, 1, 3);
      _h_XpChargedL = bookHisto1D( 8, 1, 1);
      _h_XpChargedC = bookHisto1D( 8, 1, 2);
      _h_XpChargedB = bookHisto1D( 8, 1, 3);

      _h_NPiPlusL  = bookHisto1D( 5, 2, 1);
      _h_NPiPlusC  = bookHisto1D( 5, 2, 2);
      _h_NPiPlusB  = bookHisto1D( 5, 2, 3);
      _h_NKPlusL   = bookHisto1D( 6, 2, 1);
      _h_NKPlusC   = bookHisto1D( 6, 2, 2);
      _h_NKPlusB   = bookHisto1D( 6, 2, 3);
      _h_NProtonL  = bookHisto1D( 7, 2, 1);
      _h_NProtonC  = bookHisto1D( 7, 2, 2);
      _h_NProtonB  = bookHisto1D( 7, 2, 3);

      _h_RPiPlus  = bookHisto1D( 9, 1, 1);
      _h_RPiMinus = bookHisto1D( 9, 1, 2);
      _h_RKPlus   = bookHisto1D(10, 1, 1);
      _h_RKMinus  = bookHisto1D(10, 1, 2);
      _h_RProton  = bookHisto1D(11, 1, 1);
      _h_RPBar    = bookHisto1D(11, 1, 2);

      // Ratios: used as target of divide() later
      _s_PiM_PiP  = bookScatter2D(9, 1, 3);
	  _s_KM_KP	  = bookScatter2D(10, 1, 3);
	  _s_Pr_PBar  = bookScatter2D(11, 1, 3);

    }


    /// Finalize
    void finalize() {

      // Multiplicities
      /// @todo Include errors
      const double avgNumPartsLight = _weightedTotalChargedPartNumLight / _weightLight;
      const double avgNumPartsCharm = _weightedTotalChargedPartNumCharm / _weightCharm;
      const double avgNumPartsBottom = _weightedTotalChargedPartNumBottom / _weightBottom;
      bookScatter2D(8, 2, 1, true)->point(0).setY(avgNumPartsLight);
	  bookScatter2D(8, 2, 2, true)->point(0).setY(avgNumPartsCharm);
	  bookScatter2D(8, 2, 3, true)->point(0).setY(avgNumPartsBottom);
	  bookScatter2D(8, 3, 2, true)->point(0).setY(avgNumPartsCharm - avgNumPartsLight);
	  bookScatter2D(8, 3, 3, true)->point(0).setY(avgNumPartsBottom - avgNumPartsLight);

      // Do divisions
      divide(*_h_RPiMinus - *_h_RPiPlus, *_h_RPiMinus + *_h_RPiPlus, _s_PiM_PiP);
      divide(*_h_RKMinus - *_h_RKPlus, *_h_RKMinus + *_h_RKPlus, _s_KM_KP);
      divide(*_h_RProton - *_h_RPBar, *_h_RProton + *_h_RPBar, _s_Pr_PBar);

      // Scale histograms
      scale(_h_PCharged,      1./sumOfWeights());
      scale(_h_XpPiPlus,      1./sumOfWeights());
      scale(_h_XpKPlus,       1./sumOfWeights());
      scale(_h_XpProton,      1./sumOfWeights());
      scale(_h_XpPiPlusTotal, 1./sumOfWeights());
      scale(_h_XpKPlusTotal,  1./sumOfWeights());
      scale(_h_XpProtonTotal, 1./sumOfWeights());
      scale(_h_XpPiPlusL,     1./_weightLight);
      scale(_h_XpPiPlusC,     1./_weightCharm);
      scale(_h_XpPiPlusB,     1./_weightBottom);
      scale(_h_XpKPlusL,      1./_weightLight);
      scale(_h_XpKPlusC,      1./_weightCharm);
      scale(_h_XpKPlusB,      1./_weightBottom);
      scale(_h_XpProtonL,     1./_weightLight);
      scale(_h_XpProtonC,     1./_weightCharm);
      scale(_h_XpProtonB,     1./_weightBottom);

      scale(_h_XpChargedL, 1./_weightLight);
      scale(_h_XpChargedC, 1./_weightCharm);
      scale(_h_XpChargedB, 1./_weightBottom);

      scale(_h_NPiPlusL, 1./_weightLight);
      scale(_h_NPiPlusC, 1./_weightCharm);
      scale(_h_NPiPlusB, 1./_weightBottom);
      scale(_h_NKPlusL,  1./_weightLight);
      scale(_h_NKPlusC,  1./_weightCharm);
      scale(_h_NKPlusB,  1./_weightBottom);
      scale(_h_NProtonL, 1./_weightLight);
      scale(_h_NProtonC, 1./_weightCharm);
      scale(_h_NProtonB, 1./_weightBottom);

      // Paper suggests this should be 0.5/weight but it has to be 1.0 to get normalisations right...
      scale(_h_RPiPlus,  1./_weightLight);
      scale(_h_RPiMinus, 1./_weightLight);
      scale(_h_RKPlus,   1./_weightLight);
      scale(_h_RKMinus,  1./_weightLight);
      scale(_h_RProton,  1./_weightLight);
      scale(_h_RPBar,    1./_weightLight);
    }

    //@}


  private:

    /// @name Multiplicities
    //@{
    double _weightedTotalChargedPartNumLight;
    double _weightedTotalChargedPartNumCharm;
    double _weightedTotalChargedPartNumBottom;
    //@}

    /// @name Weights
    //@{
    double _weightLight, _weightCharm, _weightBottom;
    //@}

    // Histograms
    //@{
    Histo1DPtr _h_PCharged;
    Histo1DPtr _h_XpPiPlus, _h_XpKPlus, _h_XpProton;
    Histo1DPtr _h_XpPiPlusTotal, _h_XpKPlusTotal, _h_XpProtonTotal;
    Histo1DPtr _h_XpPiPlusL, _h_XpPiPlusC, _h_XpPiPlusB;
    Histo1DPtr _h_XpKPlusL, _h_XpKPlusC, _h_XpKPlusB;
    Histo1DPtr _h_XpProtonL, _h_XpProtonC, _h_XpProtonB;
    Histo1DPtr _h_XpChargedL, _h_XpChargedC, _h_XpChargedB;
    Histo1DPtr _h_NPiPlusL, _h_NPiPlusC, _h_NPiPlusB;
    Histo1DPtr _h_NKPlusL, _h_NKPlusC, _h_NKPlusB;
    Histo1DPtr _h_NProtonL, _h_NProtonC, _h_NProtonB;
    Histo1DPtr _h_RPiPlus, _h_RPiMinus, _h_RKPlus;
    Histo1DPtr _h_RKMinus, _h_RProton, _h_RPBar;
    Scatter2DPtr _s_PiM_PiP, _s_KM_KP, _s_Pr_PBar;
    //@}

  };


  // Hook for the plugin system
  DECLARE_RIVET_PLUGIN(SLD_2004_S5693039);

}

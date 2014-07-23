#ifndef RIVET_PARTICLE_UTILS_HH
#define RIVET_PARTICLE_UTILS_HH

#include "Rivet/Particle.hh"

namespace Rivet {

  namespace PID {


    /// @name PID operations on Rivet::Particle wrapper
    //@{

    /// if this is a nucleus (ion), get A
    /// Ion numbers are +/- 10LZZZAAAI.
    // int A(const Particle& p) { return A(p.pdgId()); }

    /// if this is a nucleus (ion), get Z
    /// Ion numbers are +/- 10LZZZAAAI.
    // int Z(const Particle& p) { return Z(p.pdgId()); }

    /// if this is a nucleus (ion), get nLambda
    /// Ion numbers are +/- 10LZZZAAAI.
    // int lambda( const Particle& p) { return lambda(p.pdgId()); }

    /// Absolute value of particle ID
    inline int abspid( const Particle& p) { return abspid(p.pdgId()); }

    /// Is this a valid meson ID?
    inline bool isMeson( const Particle& p ) { return isMeson(p.pdgId()); }
    /// Is this a valid baryon ID?
    inline bool isBaryon( const Particle& p ) { return isBaryon(p.pdgId()); }
    /// Is this a valid diquark ID?
    inline bool isDiQuark( const Particle& p ) { return isDiQuark(p.pdgId()); }
    /// Is this a valid hadron ID?
    inline bool isHadron( const Particle& p ) { return isHadron(p.pdgId()); }
    /// Is this a valid lepton ID?
    inline bool isLepton( const Particle& p ) { return isLepton(p.pdgId()); }
    /// Is this a valid ion ID?
    inline bool isNucleus( const Particle& p ) { return isNucleus(p.pdgId()); }
    /// Is this a valid pentaquark ID?
    inline bool isPentaquark( const Particle& p ) { return isPentaquark(p.pdgId()); }
    /// Is this a valid SUSY ID?
    inline bool isSUSY( const Particle& p ) { return isSUSY(p.pdgId()); }
    /// Is this a valid R-hadron ID?
    inline bool isRhadron( const Particle& p ) { return isRhadron(p.pdgId()); }

    /// Does this particle contain an up quark?
    inline bool hasUp( const Particle& p ) { return hasUp(p.pdgId()); }
    /// Does this particle contain a down quark?
    inline bool hasDown( const Particle& p ) { return hasDown(p.pdgId()); }
    /// Does this particle contain a strange quark?
    inline bool hasStrange( const Particle& p ) { return hasStrange(p.pdgId()); }
    /// Does this particle contain a charm quark?
    inline bool hasCharm( const Particle& p ) { return hasCharm(p.pdgId()); }
    /// Does this particle contain a bottom quark?
    inline bool hasBottom( const Particle& p ) { return hasBottom(p.pdgId()); }
    /// Does this particle contain a top quark?
    inline bool hasTop( const Particle& p ) { return hasTop(p.pdgId()); }

    /// jSpin returns 2J+1, where J is the total spin
    inline int jSpin( const Particle& p ) { return jSpin(p.pdgId()); }
    /// sSpin returns 2S+1, where S is the spin
    inline int sSpin( const Particle& p ) { return sSpin(p.pdgId()); }
    /// lSpin returns 2L+1, where L is the orbital angular momentum
    inline int lSpin( const Particle& p ) { return lSpin(p.pdgId()); }

    /// Return 3 times the charge (3 x quark charge is an int)
    inline int threeCharge( const Particle& p ) { return threeCharge(p.pdgId()); }
    /// Return the charge
    inline double charge( const Particle& p ) { return threeCharge(p)/3.0; }

    //@}


  }

}

#endif

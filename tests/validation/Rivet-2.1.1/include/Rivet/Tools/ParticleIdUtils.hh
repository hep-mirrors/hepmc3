// ----------------------------------------------------------------------
//
// ParticleIDMethods.hh
// Author:  Lynn Garren, Andy Buckley
//
//  various utilities to extract information from the particle ID
//
//  In the standard numbering scheme, the PID digits (base 10) are:
//            +/- n nr nl nq1 nq2 nq3 nj
//  It is expected that any 7 digit number used as a PID will adhere to
//  the Monte Carlo numbering scheme documented by the PDG.
//  Note that many "new" particles not explicitly defined already
//  can be expressed within this numbering scheme.
//
//  These are the same methods that can be found in HepPDT::ParticleID
// ----------------------------------------------------------------------
#ifndef RIVET_PARTICLE_ID_UTILS_HH
#define RIVET_PARTICLE_ID_UTILS_HH

namespace Rivet {

  namespace PID {


    /// @name PID operations on Rivet::Particle wrapper
    //@{

    // /// if this is a nucleus (ion), get A
    // /// Ion numbers are +/- 10LZZZAAAI.
    // int A(const int & pid );

    // /// if this is a nucleus (ion), get Z
    // /// Ion numbers are +/- 10LZZZAAAI.
    // int Z(const int & pid );

    // /// if this is a nucleus (ion), get nLambda
    // /// Ion numbers are +/- 10LZZZAAAI.
    // int lambda( const int & pid );

    /// Absolute value of particle ID
    int abspid( const int & pid );

    /// Is this a valid ID?
    bool isValid( const int & pid );
    /// Is this a valid meson ID?
    bool isMeson( const int & pid );
    /// Is this a valid baryon ID?
    bool isBaryon( const int & pid );
    /// Is this a valid diquark ID?
    bool isDiQuark( const int & pid );
    /// Is this a valid hadron ID?
    bool isHadron( const int & pid );
    /// Is this a valid lepton ID?
    bool isLepton( const int & pid );
    /// Is this a neutrino ID?
    bool isNeutrino( const int & pid );
    /// Is this a valid ion ID?
    bool isNucleus( const int & pid );
    /// Is this a valid pentaquark ID?
    bool isPentaquark( const int & pid );
    /// Is this a valid SUSY ID?
    bool isSUSY( const int & pid );
    /// Is this a valid R-hadron ID?
    bool isRhadron( const int & pid );

    /// Does this particle contain an up quark?
    bool hasUp( const int & pid );
    /// Does this particle contain a down quark?
    bool hasDown( const int & pid );
    /// Does this particle contain a strange quark?
    bool hasStrange( const int & pid );
    /// Does this particle contain a charm quark?
    bool hasCharm( const int & pid );
    /// Does this particle contain a bottom quark?
    bool hasBottom( const int & pid );
    /// Does this particle contain a top quark?
    bool hasTop( const int & pid );

    /// jSpin returns 2J+1, where J is the total spin
    int jSpin( const int & pid );
    /// sSpin returns 2S+1, where S is the spin
    int sSpin( const int & pid );
    /// lSpin returns 2L+1, where L is the orbital angular momentum
    int lSpin( const int & pid );

    /// Return 3 times the charge (3 x quark charge is an int)
    int threeCharge( const int & pid );
    /// Return the charge
    inline double charge( const int & pid ) { return threeCharge(pid)/3.0; }

    //@}


  }

}

#endif

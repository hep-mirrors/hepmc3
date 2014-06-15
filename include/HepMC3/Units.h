#ifndef HEPMC3_UNITS_H
#define HEPMC3_UNITS_H
/**
 *  @file Units.h
 *  @brief Definition of \b class Units
 *
 *  @class HepMC3::Units
 *  @brief Stores units-related enums and conversion functions
 *
 *  Manages units used by HepMC::GenEvent
 *
 */
#include <iostream>
#include <cstring>
#include "HepMC3/FourVector.h"

namespace HepMC3 {

class Units {
public:
    enum MomentumUnit { MEV, GEV }; //!< Momentum units
    enum LengthUnit   { MM,  CM  }; //!< Position units

public:
    static MomentumUnit default_momentum_unit() { return GEV; } //!< Default momentum unit
    static LengthUnit   default_length_unit()   { return CM;  } //!< Default length unit

    /** @brief Get momentum unit based on its name*/
    static MomentumUnit momentum_unit( const char *name ) {
        if( strncmp(name,"GEV",3) == 0 ) return GEV;
        if( strncmp(name,"MEV",3) == 0 ) return MEV;

        ERROR("Units::momentum_unit: unrecognised unit name: '" << name <<"', setting to GEV" )

        return GEV;
    }

    /** @brief Get length unit based on its name*/
    static LengthUnit length_unit( const char *name ) {
        if( strncmp(name,"CM",2) == 0 ) return CM;
        if( strncmp(name,"MM",2) == 0 ) return MM;

        ERROR("Units::length_unit: unrecognised unit name: '" << name <<"', setting to CM" )

        return CM;
    }

    /** @brief Get name of momentum unit */
    static const char* name( MomentumUnit u ) {
        switch(u) {
            case MEV: return "MEV";
            case GEV: return "GEV";
        }

        return "<UNDEFINED>";
    }

    /** @brief Get name of length unit */
    static const char* name( LengthUnit u ) {
        switch(u) {
            case MM: return "MM";
            case CM: return "CM";
        }

        return "<UNDEFINED>";
    }

    /** @brief Convert FourVector to different momentum unit */
    static void convert( FourVector &m, MomentumUnit from, MomentumUnit to ) {
        if( from == to ) return;

        if( from == GEV ) {
            // GEV -> MEV
            m *= 1000.;
        }
        else if( from == MEV ) {
            // MEV -> GEV
            m *= 0.001;
        }
    }

    /** @brief Convert FourVector to different length unit */
    static void convert( FourVector &m, LengthUnit from, LengthUnit to ) {
        if( from == to ) return;

        if( from == CM ) {
            // CM -> MM
            m *= 100.;
        }
        else if( from == MM ) {
            // MM -> CM
            m *= 0.01;
        }
    }

};

} // namespace HepMC3

#endif

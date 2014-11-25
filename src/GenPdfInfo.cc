// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenPdfInfo.cc
 *  @brief Implementation of \b struct GenPdfInfo
 *
 */
#include "HepMC/GenPdfInfo.h"
#include <cstring> // memcmp

namespace HepMC {

void GenPdfInfo::set( int parton_id1, int parton_id2, double x1, double x2,
                      double scale_in, double xf1, double xf2,
                      int pdf_id1, int pdf_id2) {
    parton_id[0] = parton_id1;
    parton_id[1] = parton_id2;
    x[0]         = x1;
    x[1]         = x2;
    scale        = scale_in;
    xf[0]        = xf1;
    xf[1]        = xf2;
    pdf_id[0]    = pdf_id1;
    pdf_id[1]    = pdf_id2;
}

bool GenPdfInfo::operator==( const GenPdfInfo& a ) const {
    return ( memcmp( this, &a, sizeof(struct GenPdfInfo) ) == 0 );
}

bool GenPdfInfo::operator!=( const GenPdfInfo& a ) const {
    return !( a == *this );
}

bool GenPdfInfo::is_valid() const
{
    if( parton_id[0] != 0 ) return true;
    if( parton_id[1] != 0 ) return true;
    if( x[0]         != 0 ) return true;
    if( x[1]         != 0 ) return true;
    if( scale        != 0 ) return true;
    if( xf[0]        != 0 ) return true;
    if( xf[1]        != 0 ) return true;
    if( pdf_id[0]    != 0 ) return true;
    if( pdf_id[1]    != 0 ) return true;

    return false;
}

void GenPdfInfo::print( std::ostream & ostr) const {
    ostr << " GenPdfInfo: " << parton_id[0]
         << " " << parton_id[1]
         << " " << x[0]
         << " " << x[1]
         << " " << scale
         << " " << xf[0]
         << " " << xf[1]
         << " " << pdf_id[0]
         << " " << pdf_id[1]
         << " " << std::endl;
}

} // namespace HepMC

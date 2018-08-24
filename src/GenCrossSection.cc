// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenCrossSection.cc
 *  @brief Implementation of \b class GenCrossSection
 *
 */
#include "HepMC/GenCrossSection.h"
#include "HepMC/GenEvent.h"
#include <cstring> // memcmp
#include <cstdlib> // atoi
#include <sstream>
#include <iomanip>

namespace HepMC {

void GenCrossSection::setup() {
    if ( event() ) {
        cross_sections = vector<double>(event()->weights().size(),
                                        cross_section);
        cross_section_errors = vector<double>(event()->weights().size(),
                                              cross_section_error);
    }
}

int GenCrossSection::windx(string wName) const {
    if ( !event() || !event()->run_info() ) return 0;
    return event()->run_info()->weight_index(wName);
}

bool GenCrossSection::from_string(const string &att) {
    const char *cursor = att.data();
    
    cross_section = atof(cursor);
    cross_sections.push_back(cross_section);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    cross_section_error = atof(cursor);
    cross_section_errors.push_back(cross_section_error);

    if( !(cursor = strchr(cursor+1,' ')) ) accepted_events = -1;
    else accepted_events = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) attempted_events = -1;
    else attempted_events = atof(cursor);

    setup();

    for (int i = 0, N = cross_sections.size(); i < N; ++i ) {
        if( !(cursor = strchr(cursor+1,' ')) ) break;
        cross_sections[i] = atof(cursor);
        if( !(cursor = strchr(cursor+1,' ')) ) break;
        cross_section_errors[i] = atof(cursor);
    }

    return true;
}

bool GenCrossSection::to_string(string &att) const {
    std::ostringstream os;

    os << std::setprecision(8) << std::scientific
       << cross_section << " "
       << cross_section_error << " "
       << accepted_events << " "
       << attempted_events;

    for (int i = 0, N = cross_sections.size(); i < N; ++i )
        os << " " << cross_sections[i]
           << " " << cross_section_errors[i];

    att = os.str();

    return true;
}

bool GenCrossSection::operator==( const GenCrossSection& a ) const {
  return ( memcmp( (void*)this, (void*) &a, sizeof(class GenCrossSection) ) == 0 );
}

bool GenCrossSection::operator!=( const GenCrossSection& a ) const {
    return !( a == *this );
}

bool GenCrossSection::is_valid() const {
    if( cross_section       != 0 ) return true;
    if( cross_section_error != 0 ) return true;
    return false;
}

} // namespace HepMC

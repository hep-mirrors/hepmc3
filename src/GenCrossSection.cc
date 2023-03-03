// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenCrossSection.cc
 *  @brief Implementation of \b class GenCrossSection
 *
 */
#include <cstdlib> // atoi
#include <cstring> // memcmp
#include <iomanip>
#include <sstream>

#include "HepMC3/GenCrossSection.h"
#include "HepMC3/GenEvent.h"


namespace HepMC3 {


int GenCrossSection::windx(const std::string& wName) const {
    if ( !event() || !event()->run_info() ) {return 0;}
    return event()->run_info()->weight_index(wName);
}

void GenCrossSection::set_cross_section(const double& xs, const double& xs_err, const long& n_acc, const long& n_att) {
    double cross_section       = xs;
    double cross_section_error = xs_err;
    accepted_events     = n_acc;
    attempted_events    = n_att;
    size_t N = std::max( event() ? event()->weights().size() : 0, size_t{1});
    cross_sections = std::vector<double>(N, cross_section);
    cross_section_errors = std::vector<double>(N, cross_section_error);
}


void GenCrossSection::set_cross_section(const std::vector<double>& xs, const std::vector<double>& xs_err, const long& n_acc, const long& n_att) {
    cross_sections       = xs;
    cross_section_errors = xs_err;
    accepted_events     = n_acc;
    attempted_events    = n_att;
}


bool GenCrossSection::from_string(const std::string &att) {
    const char *cursor = att.data();
    cross_sections.clear();
    cross_section_errors.clear();


    double cross_section = atof(cursor);
    cross_sections.emplace_back(cross_section);

    if ( !(cursor = strchr(cursor+1, ' ')) ) {return false;}
    double cross_section_error = atof(cursor);
    cross_section_errors.emplace_back(cross_section_error);

    if ( !(cursor = strchr(cursor+1, ' ')) ) {accepted_events = -1; attempted_events = -1;}
    else
    {
        accepted_events = atoi(cursor);
        if ( !(cursor = strchr(cursor+1, ' ')) ) { attempted_events = -1; }
        else { attempted_events = atoi(cursor); }
    }
    size_t N = event() ? std::max(event()->weights().size(), size_t{1}) : size_t{1};
    const size_t max_n_cross_sections = 1000;
    while (cross_sections.size() < max_n_cross_sections) {
        if ( !(cursor = strchr(cursor+1, ' ')) ) break;
        cross_sections.emplace_back(atof(cursor));
        if ( !(cursor = strchr(cursor+1, ' ')) ) break;
        cross_section_errors.emplace_back(atof(cursor));
    }
    if (cross_sections.size() >= max_n_cross_sections) {
        HEPMC3_WARNING("GenCrossSection::from_string: too many optional cross-sections  N=" << cross_sections.size() << " or ill-formed input:" << att)
    }
    // Use the default values to fill the vector to the size of N.
    size_t oldsize = cross_sections.size();
    if (oldsize != N) {
        HEPMC3_WARNING("GenCrossSection::from_string: the number of cross-sections cross_sections.size()=" << cross_sections.size() << " does not match the number of weights" << event()->weights().size())
    }
    for (size_t i = oldsize; i < N; i++) {cross_sections.emplace_back(cross_section); cross_section_errors.emplace_back(cross_section_error);}
    return true;
}

bool GenCrossSection::to_string(std::string &att) const {
    std::ostringstream os;

    os << std::setprecision(8) << std::scientific
       << (cross_sections.empty()?0.0:cross_sections.at(0)) << " "
       << (cross_section_errors.empty()?0.0:cross_section_errors.at(0)) << " "
       << accepted_events << " "
       << attempted_events;
    if ( event() && event()->weights().size() != cross_sections.size() ) {
        HEPMC3_WARNING("GenCrossSection::to_string: the number of cross-sections cross_sections.size()=" << cross_sections.size() << " does not match the number of weights" << event()->weights().size())
    }
    for (size_t i = 1; i < cross_sections.size(); ++i ) {
        os << " " << cross_sections.at(i)
           << " " << (cross_section_errors.size()>i?cross_section_errors.at(i):0.0);
    }
    att = os.str();

    return true;
}

bool GenCrossSection::operator==(const GenCrossSection& a) const {
    return ( memcmp( (void*)this, (void*) &a, sizeof(class GenCrossSection) ) == 0 );
}

bool GenCrossSection::operator!=(const GenCrossSection& a) const {
    return !( a == *this );
}

bool GenCrossSection::is_valid() const {
    if ( cross_sections.empty() ) { return false; }
    if ( cross_section_errors.empty() ) { return false; }
    if ( cross_section_errors.size() != cross_sections.size() ) { return false; }
    if ( cross_sections.at(0)       != 0 ) { return true; }
    if ( cross_section_errors.at(0) != 0 ) { return true; }
    return false;
}

} // namespace HepMC3

/**
 *  @file GenCrossSection.cc
 *  @brief Implementation of \b struct GenCrossSection
 *
 */
#include "HepMC3/GenCrossSection.h"

namespace HepMC3 {

bool GenCrossSection::operator==( const GenCrossSection& a ) const {
    return (    a.cross_section       == this->cross_section
             && a.cross_section_error == this->cross_section_error );
}

bool GenCrossSection::operator!=( const GenCrossSection& a ) const {
    return !( a == *this );
}

void GenCrossSection::print( std::ostream & ostr) const {
    ostr << "GenCrossSection: " << cross_section
         << " " << cross_section_error
         << " " << std::endl;
}

} // namespace HepMC3

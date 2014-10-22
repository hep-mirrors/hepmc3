/**
 *  @file GenCrossSection.cc
 *  @brief Implementation of \b struct GenCrossSection
 *
 */
#include "HepMC/GenCrossSection.h"
#include <cstring> // memcmp

namespace HepMC {

bool GenCrossSection::operator==( const GenCrossSection& a ) const {
    return ( memcmp( this, &a, sizeof(struct GenCrossSection) ) == 0 );
}

bool GenCrossSection::operator!=( const GenCrossSection& a ) const {
    return !( a == *this );
}

bool GenCrossSection::is_valid() const {
    if( cross_section       != 0 ) return true;
    if( cross_section_error != 0 ) return true;
    return false;
}

void GenCrossSection::print( std::ostream & ostr) const {
    ostr << " GenCrossSection: " << cross_section
         << " " << cross_section_error
         << " " << std::endl;
}

} // namespace HepMC

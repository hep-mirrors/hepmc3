/**
 *  @file GenCrossSection.cc
 *  @brief Implementation of \b class GenCrossSection
 *
 */
#include "HepMC/GenCrossSection.h"
#include <cstring> // memcmp

namespace HepMC {

bool GenCrossSection::parse_attribute_container(const AttributeContainer &att) {
    const char *cursor = att.data();

    cross_section = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    cross_section_error = atof(cursor);

    return true;
}

bool GenCrossSection::fill_attribute_container(AttributeContainer &att) const {
    char buf[64];

    sprintf(buf,"%.8e %.8e\n",
            cross_section,
            cross_section_error);

    att = buf;

    return true;
}

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

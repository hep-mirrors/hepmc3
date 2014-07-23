#ifndef  HEPMC3_CROSS_SECTION_H
#define  HEPMC3_CROSS_SECTION_H
/**
 *  @file GenCrossSection.h
 *  @brief Definition of \b struct GenCrossSection
 *
 *  @struct HepMC3::GenCrossSection
 *  @brief Stores additional information about cross-section
 *
 *  HepMC3::GenCrossSection is used to store the generated cross section.
 *  This class is meant to be used to pass, on an event by event basis,
 *  the current best guess of the total cross section.
 *  It is expected that the final cross section will be stored elsewhere.
 *
 *    - double cross_section;           // cross section in pb
 *    - double cross_section_error;     // error associated with this cross section
 *
 *  The units of cross_section and cross_section_error are expected to be pb.
 *
 *  GenCrossSection information will be written if GenEvent contains a pointer
 *  to a valid GenCrossSection object.
 *
 */
#include <iostream>

namespace HepMC3 {

struct GenCrossSection {

//
// Fields
//
public:
    double cross_section;       //!< Generated cross-section
    double cross_section_error; //!< Generated cross-section error

//
// Functions
//
public:
    /** @brief Set all fields */
    void set_cross_section( double xs, double xs_err ) {
        cross_section       = xs;
        cross_section_error = xs_err;
    }

    bool operator==( const GenCrossSection& ) const; //!< Operator ==
    bool operator!=( const GenCrossSection& ) const; //!< Operator !=

    void print(std::ostream &ostr) const; //!< Print the contents to output stream
};

} // namespace HepMC3

#endif

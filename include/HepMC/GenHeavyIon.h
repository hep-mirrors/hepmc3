#ifndef  HEPMC_HEAVYION_H
#define  HEPMC_HEAVYION_H
/**
 *  @file GenHeavyIon.h
 *  @brief Definition of attribute \b class GenHeavyIon
 *
 *  @class HepMC::GenHeavyIon
 *  @brief Stores additional information about Heavy Ion generator
 *
 *  This is an example of event attribute used to store Heavy Ion information
 *
 */
#include <iostream>
#include "HepMC/Attribute.h"

namespace HepMC {

class GenHeavyIon : public Attribute {

//
// Fields
//
public:
    int   Ncoll_hard;
    int   Npart_proj;
    int   Npart_targ;
    int   Ncoll;
    int   spectator_neutrons;
    int   spectator_protons;
    int   N_Nwounded_collisions;
    int   Nwounded_N_collisions;
    int   Nwounded_Nwounded_collisions;
    float impact_parameter;
    float event_plane_angle;
    float eccentricity;
    float sigma_inel_NN;

//
// Functions
//
public:
    /** @brief Implementation of Attribute::parse_attribute_container */
    bool parse_attribute_container(const AttributeContainer &att);

    /** @brief Implementation of Attribute::fill_attribute_container */
    bool fill_attribute_container(AttributeContainer &att) const;

    /** @brief Set all fields */
    void set( int nh, int np, int nt, int nc, int ns, int nsp,
              int nnw=0, int nwn=0, int nwnw=0,
              float im=0., float pl=0., float ec=0., float s=0. );

    bool operator==( const GenHeavyIon& ) const; ///< Operator ==
    bool operator!=( const GenHeavyIon& ) const; ///< Operator !=
    bool is_valid()                       const; ///< Verify that the instance contains non-zero information

    void print(std::ostream &ostr = std::cout) const; ///< Print the contents to output stream
};

} // namespace HepMC

#endif

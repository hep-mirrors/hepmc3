#ifndef  HEPMC3_HEAVYION_H
#define  HEPMC3_HEAVYION_H
/**
 *  @file HeavyIon.h
 *  @brief Definition of \b struct HeavyIon
 *
 *  @struct HepMC3::HeavyIon
 *  @brief Stores additional information about Heavy Ion generator
 *
 *  Creation and use of this information is optional
 *
 */
#include <iostream>

namespace HepMC3 {

struct HeavyIon {

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
    void      set( int nh, int np, int nt, int nc, int ns, int nsp,
                   int nnw=0, int nwn=0, int nwnw=0,
                   float im=0., float pl=0., float ec=0., float s=0. ); //!< Set all fields

    bool      operator==( const HeavyIon& ) const; //!< Operator ==
    bool      operator!=( const HeavyIon& ) const; //!< Operator !=
    bool      is_valid()                    const; //!< Verify that the instance contains non-zero information

    void      print(std::ostream &ostr)     const; //!< Print the contents to output stream
};

} // namespace HepMC3

#endif

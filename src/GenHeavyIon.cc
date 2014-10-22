/**
 *  @file GenHeavyIon.cc
 *  @brief Implementation of \b struct GenHeavyIon
 *
 */
#include "HepMC/GenHeavyIon.h"
#include <cstring> // memcmp

namespace HepMC {

void GenHeavyIon::set( int nh, int np, int nt, int nc, int ns, int nsp,
                    int nnw, int nwn, int nwnw,
                    float im, float pl, float ec, float s ) {
    Ncoll_hard                   = nh;
    Npart_proj                   = np;
    Npart_targ                   = nt;
    Ncoll                        = nc;
    spectator_neutrons           = ns;
    spectator_protons            = nsp;
    N_Nwounded_collisions        = nnw;
    Nwounded_N_collisions        = nwn;
    Nwounded_Nwounded_collisions = nwnw;
    impact_parameter             = im;
    event_plane_angle            = pl;
    eccentricity                 = ec;
    sigma_inel_NN                = s;
}

bool GenHeavyIon::operator==( const GenHeavyIon& a ) const {
    return ( memcmp( this, &a, sizeof(struct GenHeavyIon) ) == 0 );
}

bool GenHeavyIon::operator!=( const GenHeavyIon& a ) const {
    return !( a == *this );
}

bool GenHeavyIon::is_valid() const {
    if( Ncoll_hard                   != 0 ) return true;
    if( Npart_proj                   != 0 ) return true;
    if( Npart_targ                   != 0 ) return true;
    if( Ncoll                        != 0 ) return true;
    if( spectator_neutrons           != 0 ) return true;
    if( spectator_protons            != 0 ) return true;
    if( N_Nwounded_collisions        != 0 ) return true;
    if( Nwounded_N_collisions        != 0 ) return true;
    if( Nwounded_Nwounded_collisions != 0 ) return true;
    if( impact_parameter             != 0 ) return true;
    if( event_plane_angle            != 0 ) return true;
    if( eccentricity                 != 0 ) return true;
    if( sigma_inel_NN                != 0 ) return true;
    return false;
}

void GenHeavyIon::print( std::ostream &ostr ) const {
    ostr << " GenHeavyIon: " << Ncoll_hard
         << " " << Npart_proj
         << " " << Npart_targ
         << " " << Ncoll
         << " " << spectator_neutrons
         << " " << spectator_protons
         << " " << N_Nwounded_collisions
         << " " << Nwounded_N_collisions
         << " " << Nwounded_Nwounded_collisions
         << " " << impact_parameter
         << " " << event_plane_angle
         << " " << eccentricity
         << " " << sigma_inel_NN
         << std::endl;
}

} // namespace HepMC

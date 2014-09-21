/**
 *  @file GenHeavyIon.cc
 *  @brief Implementation of \b struct GenHeavyIon
 *
 */
#include "HepMC/GenHeavyIon.h"

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
    return (    a.Ncoll_hard                   == this->Ncoll_hard
             && a.Npart_proj                   == this->Npart_proj
             && a.Npart_targ                   == this->Npart_targ
             && a.Ncoll                        == this->Ncoll
             && a.N_Nwounded_collisions        == this->N_Nwounded_collisions
             && a.Nwounded_N_collisions        == this->Nwounded_N_collisions
             && a.Nwounded_Nwounded_collisions == this->Nwounded_Nwounded_collisions
             && a.spectator_neutrons           == this->spectator_neutrons
             && a.spectator_protons            == this->spectator_protons
             && a.impact_parameter             == this->impact_parameter
             && a.event_plane_angle            == this->event_plane_angle
             && a.eccentricity                 == this->eccentricity
             && a.sigma_inel_NN                == this->sigma_inel_NN );
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

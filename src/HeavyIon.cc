/**
 *  @file HeavyIon.cc
 *  @brief Implementation of \b class HeavyIon
 *
 */
#include "HepMC3/HeavyIon.h"

namespace HepMC3 {

HeavyIon::HeavyIon():
m_Ncoll_hard(0),
m_Npart_proj(0),
m_Npart_targ(0),
m_Ncoll(0),
m_spectator_neutrons(0),
m_spectator_protons(0),
m_N_Nwounded_collisions(0),
m_Nwounded_N_collisions(0),
m_Nwounded_Nwounded_collisions(0),
m_impact_parameter(0),
m_event_plane_angle(0),
m_eccentricity(0),
m_sigma_inel_NN(0) {
}

HeavyIon::HeavyIon( int nh, int np, int nt, int nc, int ns, int nsp,
                    int nnw, int nwn, int nwnw,
                    float im, float pl, float ec, float s ):
m_Ncoll_hard(nh),
m_Npart_proj(np),
m_Npart_targ(nt),
m_Ncoll(nc),
m_spectator_neutrons(ns),
m_spectator_protons(nsp),
m_N_Nwounded_collisions(nnw),
m_Nwounded_N_collisions(nwn),
m_Nwounded_Nwounded_collisions(nwnw),
m_impact_parameter(im),
m_event_plane_angle(pl),
m_eccentricity(ec),
m_sigma_inel_NN(s) {
}

HeavyIon::HeavyIon( HeavyIon const & orig ):
m_Ncoll_hard(orig.m_Ncoll_hard),
m_Npart_proj(orig.m_Npart_proj),
m_Npart_targ(orig.m_Npart_targ),
m_Ncoll(orig.m_Ncoll),
m_spectator_neutrons(orig.m_spectator_neutrons),
m_spectator_protons(orig.m_spectator_protons),
m_N_Nwounded_collisions(orig.m_N_Nwounded_collisions),
m_Nwounded_N_collisions(orig.m_Nwounded_N_collisions),
m_Nwounded_Nwounded_collisions(orig.m_Nwounded_Nwounded_collisions),
m_impact_parameter(orig.m_impact_parameter),
m_event_plane_angle(orig.m_event_plane_angle),
m_eccentricity(orig.m_eccentricity),
m_sigma_inel_NN(orig.m_sigma_inel_NN) {
}

HeavyIon& HeavyIon::operator=( const HeavyIon & rhs ) {
    m_Ncoll_hard                   = rhs.m_Ncoll_hard;
    m_Npart_proj                   = rhs.m_Npart_proj;
    m_Npart_targ                   = rhs.m_Npart_targ;
    m_Ncoll                        = rhs.m_Ncoll;
    m_N_Nwounded_collisions        = rhs.m_N_Nwounded_collisions;
    m_Nwounded_N_collisions        = rhs.m_Nwounded_N_collisions;
    m_Nwounded_Nwounded_collisions = rhs.m_Nwounded_Nwounded_collisions;
    m_spectator_neutrons           = rhs.m_spectator_neutrons;
    m_spectator_protons            = rhs.m_spectator_protons;
    m_impact_parameter             = rhs.m_impact_parameter;
    m_event_plane_angle            = rhs.m_event_plane_angle;
    m_eccentricity                 = rhs.m_eccentricity;
    m_sigma_inel_NN                = rhs.m_sigma_inel_NN;

    return *this;
}

bool HeavyIon::operator==( const HeavyIon& a ) const {
    return (    a.Ncoll_hard()                   == this->Ncoll_hard()
             && a.Npart_proj()                   == this->Npart_proj()
             && a.Npart_targ()                   == this->Npart_targ()
             && a.Ncoll()                        == this->Ncoll()
             && a.N_Nwounded_collisions()        == this->N_Nwounded_collisions()
             && a.Nwounded_N_collisions()        == this->Nwounded_N_collisions()
             && a.Nwounded_Nwounded_collisions() == this->Nwounded_Nwounded_collisions()
             && a.spectator_neutrons()           == this->spectator_neutrons()
             && a.spectator_protons()            == this->spectator_protons()
             && a.impact_parameter()             == this->impact_parameter()
             && a.event_plane_angle()            == this->event_plane_angle()
             && a.eccentricity()                 == this->eccentricity()
             && a.sigma_inel_NN()                == this->sigma_inel_NN() );
}

bool HeavyIon::operator!=( const HeavyIon& a ) const {
    return !( a == *this );
}

bool HeavyIon::is_valid() const {
    if( m_Ncoll_hard                   != 0 ) return true;
    if( m_Npart_proj                   != 0 ) return true;
    if( m_Npart_targ                   != 0 ) return true;
    if( m_Ncoll                        != 0 ) return true;
    if( m_spectator_neutrons           != 0 ) return true;
    if( m_spectator_protons            != 0 ) return true;
    if( m_N_Nwounded_collisions        != 0 ) return true;
    if( m_Nwounded_N_collisions        != 0 ) return true;
    if( m_Nwounded_Nwounded_collisions != 0 ) return true;
    if( m_impact_parameter             != 0 ) return true;
    if( m_event_plane_angle            != 0 ) return true;
    if( m_eccentricity                 != 0 ) return true;
    if( m_sigma_inel_NN                != 0 ) return true;
    return false;
}

void HeavyIon::print( std::ostream &ostr ) const {
    ostr << " HeavyIon: " << Ncoll_hard()
         << " " << Npart_proj()
         << " " << Npart_targ()
         << " " << Ncoll()
         << " " << spectator_neutrons()
         << " " << spectator_protons()
         << " " << N_Nwounded_collisions()
         << " " << Nwounded_N_collisions()
         << " " << Nwounded_Nwounded_collisions()
         << " " << impact_parameter()
         << " " << event_plane_angle()
         << " " << eccentricity()
         << " " << sigma_inel_NN()
         << std::endl;
}

} // namespace HepMC3

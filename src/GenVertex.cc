/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class GenVertex
 *
 */
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Setup.h"

#include <boost/foreach.hpp>

namespace HepMC {

GenVertex::GenVertex( const FourVector& position ):
m_event(NULL),
m_id(0),
m_version_created(0),
m_version_deleted(0) {
    m_data.position = position;
}

GenVertex::GenVertex( const GenVertexData &data):
m_event(NULL),
m_id(0),
m_data(data),
m_version_created(0),
m_version_deleted(0) {
}

void GenVertex::print( std::ostream& ostr ) const {
    ostr << "GenVertex:  ";
    ostr << id() << " (versions: ";

    int vd = version_deleted();
    if(vd) ostr<<(int)version_created()<<"-"<<vd-1;
    else   ostr<<(int)version_created()<<"-"<<(int)parent_event()->last_version();

    ostr <<") barcode: ";
    ostr << " in: "  << particles_in().size()
         << " out: " << particles_out().size();

    const FourVector &pos = position();
    if( !pos.is_zero() ) {
        ostr << " @ " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();
    }
    else ostr << " (X,cT): 0";

    ostr << std::endl;
}

void GenVertex::print_version( unsigned char version, std::ostream& ostr ) const {
    if( !is_in_version(version) ) return;

    ostr << "Vtx: ";
    ostr.width(6);
    ostr << id() ;

    const FourVector &pos = position();
    if( !pos.is_zero() ) {
        ostr << " (X,cT): " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();
    }
    else ostr << " (X,cT): 0";

    ostr << std::endl;

    bool printed_header = false;

    // Print out all the incoming particles
    BOOST_FOREACH( const GenParticlePtr &p, particles_in() ) {

        if( !p->is_in_version(version) ) continue;

        if( !printed_header ) {
            ostr << " I: ";
            printed_header = true;
        }
        else ostr << "    ";

        p->print_version(version,ostr);
    }

    printed_header = false;

    // Print out all the outgoing particles
    BOOST_FOREACH( const GenParticlePtr &p, particles_out() ) {

        if( !p->is_in_version(version) ) continue;

        if( !printed_header ) {
            ostr << " O: ";
            printed_header = true;
        }
        else ostr << "    ";

        p->print_version(version,ostr);
    }
}

void GenVertex::add_particle_in( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    BOOST_FOREACH( const GenParticlePtr &pp, particles_in() ) {
        if( pp == p ) return;
    }

    m_particles_in.push_back(p);

    p->set_end_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}

void GenVertex::add_particle_out( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    BOOST_FOREACH( const GenParticlePtr &pp, particles_out() ) {
        if( pp == p ) return;
    }

    m_particles_out.push_back(p);

    p->set_production_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}


GenVertexPtr GenVertex::new_version() {
    if( !m_event ) return GenVertexPtr();

    if( m_next_version ) {
        WARNING( "GenVertex::new_version: newer version of this vertex exists" )
        return GenVertexPtr();
    }

    if( version_created() == m_event->last_version() ) {
        WARNING( "GenVertex::new_version: this vertex already belongs to last version" )
        return GenVertexPtr();
    }

    if( version_deleted() != 0 ) {
        WARNING( "GenVertex::new_version: vertex marked as deleted" )
        return GenVertexPtr();
    }

    GenVertexPtr new_v = new GenVertex(data());
    m_event->add_vertex(new_v);

    m_next_version    = new_v;
    m_version_deleted = m_event->last_version();

    return new_v;
}

void GenVertex::mark_deleted() {
    if( !m_event ) return;
    if( m_version_deleted ) return;

    m_version_deleted = m_event->last_version();
}

const FourVector& GenVertex::position() const {

    if( !m_data.position.is_zero() ) return m_data.position;

    // No position information - search ancestors
    BOOST_FOREACH( const GenParticlePtr &p, particles_in() ) {
        const GenVertexPtr &v = p->production_vertex();
        if(v) return v->position();
    }

    return FourVector::ZERO_VECTOR();
}

void GenVertex::set_position(const FourVector& new_pos) {
    m_data.position = new_pos;
}

} // namespace HepMC

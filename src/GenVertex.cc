/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class GenVertex
 *
 */
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Setup.h"

#include <boost/foreach.hpp>

namespace HepMC3 {

GenVertex::GenVertex( const FourVector& position ):
m_event(NULL),
m_id(0) {
    m_data.position = position;
}

void GenVertex::print( std::ostream& ostr, bool event_listing_format ) const {

    // Standalone format. Used when calling:
    // vertex.print()
    if( !event_listing_format ) {
        ostr << "GenVertex:  " << id() << " (barcode: " << barcode() << ")"
             << " in: "  << particles_in().size()
             << " out: " << particles_out().size();

        const FourVector &pos = position();
        if( !pos.is_zero() ) {
            ostr << " @ " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();
        }
        else ostr << " (X,cT): 0";

        ostr << std::endl;
    }
    // Event listing format. Used when calling:
    // event.print()
    else {

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

            if( !printed_header ) {
                ostr << " I: ";
                printed_header = true;
            }
            else ostr << "    ";

            p->print(ostr,1);
        }

        printed_header = false;

        // Print out all the outgoing particles
        BOOST_FOREACH( const GenParticlePtr &p, particles_out() ) {

            if( !printed_header ) {
                ostr << " O: ";
                printed_header = true;
            }
            else ostr << "    ";

            p->print(ostr,1);
        }
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

} // namespace HepMC3

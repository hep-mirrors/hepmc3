/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class GenVertex
 *
 */
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Setup.h"

#include "HepMC/foreach.h"

namespace HepMC {

GenVertex::GenVertex( const FourVector& position ):
m_event(NULL),
m_id(0) {
    m_data.position = position;
}

GenVertex::GenVertex( const GenVertexData &data):
m_event(NULL),
m_id(0),
m_data(data) {
}

void GenVertex::print( std::ostream& ostr ) const {
    ostr << "GenVertex:  " << id();
    ostr << " barcode: ";
    ostr.width(5);
    ostr << barcode() <<" PDGID: ";
    ostr.width(3);
    ostr << " in: "  << particles_in().size();
    ostr.width(3);
    ostr << " out: " << particles_out().size();

    const FourVector &pos = position();
    if( !pos.is_zero() ) {

        // Find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize         prec = ostr.precision();

        if(m_event) ostr.precision(m_event->print_precision());

        ostr.setf(std::ios::scientific, std::ios::floatfield);
        ostr.setf(std::ios_base::showpos);

        ostr << " @ " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();

        // Restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
    else ostr << " (X,cT): 0";

    ostr << std::endl;
}

void GenVertex::print_event_listing(std::ostream& ostr ) const {
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
    FOREACH( const GenParticlePtr &p, particles_in() ) {
        if( !printed_header ) {
            ostr << " I: ";
            printed_header = true;
        }
        else ostr << "    ";

        p->print_event_listing(ostr);
    }

    printed_header = false;

    // Print out all the outgoing particles
    FOREACH( const GenParticlePtr &p, particles_out() ) {
        if( !printed_header ) {
            ostr << " O: ";
            printed_header = true;
        }
        else ostr << "    ";

        p->print_event_listing(ostr);
    }
}

void GenVertex::add_particle_in( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    FOREACH( const GenParticlePtr &pp, particles_in() ) {
        if( pp == p ) return;
    }

    m_particles_in.push_back(p);

    p->set_end_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}

void GenVertex::add_particle_out( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    FOREACH( const GenParticlePtr &pp, particles_out() ) {
        if( pp == p ) return;
    }

    m_particles_out.push_back(p);

    p->set_production_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}

const FourVector& GenVertex::position() const {

    if( !m_data.position.is_zero() ) return m_data.position;

    // No position information - search ancestors
    FOREACH( const GenParticlePtr &p, particles_in() ) {
        const GenVertexPtr &v = p->production_vertex();
        if(v) return v->position();
    }

    return FourVector::ZERO_VECTOR();
}

void GenVertex::set_position(const FourVector& new_pos) {
    m_data.position = new_pos;
}

} // namespace HepMC

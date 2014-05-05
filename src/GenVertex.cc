/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class HepMC3::GenVertex
 *
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Setup.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex( const FourVector& position ):
m_data( boost::make_shared<GenVertexData>() ) {
    m_data->event    = NULL;
    m_data->position = position;
}

GenVertex::GenVertex( const shared_ptr<GenVertexData> &data ):m_data(data) {
}

void GenVertex::print( std::ostream& ostr, bool event_listing_format ) const {

    // Standalone format. Used when calling:
    // vertex.print()
    if( !event_listing_format ) {
        ostr << "GenVertex:  " << barcode()
             << " in: "  << particles_in().size()
             << " out: " << particles_out().size();

        const FourVector &pos = position();
        if( !pos.is_zero() ) {
            ostr << " @ " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();
        }
        else ostr << " (X,cT): 0";

        ostr << endl;
    }
    // Event listing format. Used when calling:
    // event.print()
    else {

        ostr << "Vtx: ";
        ostr.width(6);
        ostr << barcode();

        const FourVector &pos = position();
        if( !pos.is_zero() ) {
            ostr << " (X,cT): " << pos.x()<<" "<<pos.y()<<" "<<pos.z()<<" "<<pos.t();
        }
        else ostr << " (X,cT): 0";

        ostr << endl;

        bool printed_header = false;

        // Print out all the incoming particles
        BOOST_FOREACH( const GenParticle &p, particles_in() ) {

            if( !printed_header ) {
                ostr << " I: ";
                printed_header = true;
            }
            else ostr << "    ";

            p.print(ostr,1);
        }

        printed_header = false;

        // Print out all the outgoing particles
        BOOST_FOREACH( const GenParticle &p, particles_out() ) {

            if( !printed_header ) {
                ostr << " O: ";
                printed_header = true;
            }
            else ostr << "    ";

            p.print(ostr,1);
        }
    }
}

int GenVertex::serialization_barcode() const {
    if( particles_in().size() > 1 || !m_data->position.is_zero() ) return barcode();
    if( particles_in().size() == 0 ) return 0;

    return particles_in()[0].barcode();
}

void GenVertex::add_particle_in(GenParticle &p) {
    if(m_data->event) m_data->event->add_particle(p);

    p.m_data->end_vertex = m_data;

    m_data->particles_in.push_back(p);
}

void GenVertex::add_particle_out(GenParticle &p) {
    if(m_data->event) m_data->event->add_particle(p);

    p.m_data->production_vertex = m_data;

    m_data->particles_out.push_back(p);
}

const FourVector& GenVertex::position() const {

    if( !m_data->position.is_zero() ) return m_data->position;

    // No position information - search ancestors
    BOOST_FOREACH( const GenParticle &p, particles_in() ) {
        const GenVertex &v = p.production_vertex();
        if(v) return v.position();
    }

    return FourVector::ZERO_VECTOR();
}

void GenVertex::set_position(const FourVector& new_pos) {
    m_data->position = new_pos;
}

} // namespace HepMC3

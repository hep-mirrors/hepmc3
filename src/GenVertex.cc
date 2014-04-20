/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class HepMC3::GenVertex
 *
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

#include <boost/foreach.hpp>
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex(GenEvent &event, int data_index, GenVertexData &data):
m_event(event),
m_version_created(m_event.last_version()),
m_version_deleted(std::numeric_limits<unsigned char>::max()),
m_data_index(data_index),
m_data(data),
m_last_version(this),
m_is_required(false) {
}

void GenVertex::print( std::ostream& ostr, bool event_listing_format, unsigned char version ) const {

    // Standalone format. Used when calling:
    // vertex.print()
    if( !event_listing_format ) {
        ostr << "GenVertex:  " << barcode() << " (ver. ";
        if( !is_deleted() ) ostr<<" "<<(int)m_version_created<<" ) ";
        else                ostr<<(int)m_version_created<<"-"<<(int)m_version_deleted<<") ";
        ostr <<") (X,cT):0 "
             << "P in: "  << m_particles_in.size()
             <<" P out: " << m_particles_out.size() << endl;
    }
    // Event listing format. Used when calling:
    // event.print()
    else {

        if( version == std::numeric_limits<unsigned char>::max() ) version = m_event.last_version();

        if( version_created() > version || version_deleted() <= version) return;

        ostr << "Vtx: ";
        ostr.width(6);
        ostr << barcode()
             << "  (X,cT): 0" << endl;

        bool printed_header = false;

        // Print out all the incoming particles
        BOOST_FOREACH( GenParticle *p, particles_in() ) {

            if( p->version_created() > version || p->version_deleted() <= version) continue;

            if( !printed_header ) {
                ostr << " I: ";
                printed_header = true;
            }
            else ostr << "    ";

            p->print(ostr,1);
        }

        printed_header = false;

        // Print out all the outgoing particles
        BOOST_FOREACH( GenParticle *p, particles_out() ) {

            if( p->version_created() > version || p->version_deleted() <= version) continue;

            if( !printed_header ) {
                ostr << " O: ";
                printed_header = true;
            }
            else ostr << "    ";

            p->print(ostr,1);
        }
    }
}

int GenVertex::serialization_barcode() const {
    if( m_is_required )              return barcode();
    if( m_particles_in.size() == 0 ) return 0;

    return m_particles_in[0]->barcode();
}

void GenVertex::add_particle_in(GenParticle &p) {
    if( p.version_created() > m_version_created ) {
        ERROR( "GenVertex::add_particle_in: cannot add incoming particle from later version." )
        return;
    }

    p.set_end_vertex(this);
    m_particles_in.push_back( &p );

    // This vertex must be serialized if it has more than one incoming particle
    if(m_particles_in.size()>1) m_is_required = true;
}

void GenVertex::add_particle_out(GenParticle &p) {

    p.set_production_vertex(m_last_version);
    m_particles_out.push_back( &p );
}

void GenVertex::mark_deleted() {
    m_version_deleted = m_event.last_version();
}

} // namespace HepMC3

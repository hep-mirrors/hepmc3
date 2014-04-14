/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class HepMC3::GenVertex
 *
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex(GenEvent &event, int data_index, GenVertexData &data):
m_event(event),
m_version_created(m_event.last_version()),
m_version_deleted(255),
m_data_index(data_index),
m_data(data),
m_last_version(this) {
}

void GenVertexData::print() const {
    std::cout<<std::endl;
}

void GenVertex::print( std::ostream& ostr, bool event_listing_format ) const {

    // Standalone format. Used when calling:
    // vertex.print()
    if( !event_listing_format ) {
        ostr << "GenVertex:  " << barcode() << " (ver. ";
        if( !is_deleted() ) ostr<<" "<<m_version_created<<" ) ";
        else                ostr<<m_version_created<<"-"<<m_version_deleted<<") ";
        ostr <<") (X,cT):0 "
             << "P in: "  << m_particles_in.size()
             <<" P out: " << m_particles_out.size() << endl;
    }
    // Event listing format. Used when calling:
    // event.print()
    else {
        ostr << "Vtx: ";
        ostr.width(6);
        ostr << barcode()
             << "  (X,cT): 0" << endl;
    }
}

void GenVertex::add_particle_in(GenParticle &p) {

    // Direct change, no version checking
    p.m_end_vertex = this;
    m_particles_in.push_back( &p );
}

void GenVertex::add_particle_out(GenParticle &p) {

    // Direct change, no version checking
    p.m_production_vertex = this;
    m_particles_out.push_back( &p );
}

void GenVertex::mark_deleted() {
    m_version_deleted = m_event.last_version();
}

} // namespace HepMC3

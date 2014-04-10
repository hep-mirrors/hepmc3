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

GenVertex::GenVertex(GenEvent *event):
m_event(event),
m_barcode(0),
m_version_created(0),
m_version_deleted(255),
m_next_version(0) {
}

void GenVertex::print( std::ostream& ostr, bool event_listing_format  ) const {

    // Standalone format. Used when calling:
    // vertex->print()
    if( !event_listing_format ) {
        ostr << "GenVertex:  " << barcode() << " (ver.: ";
        if(m_version_deleted==255) ostr<<" "<<m_version_created<<" ";
        else                       ostr<<m_version_created<<"-"<<m_version_deleted;
        ostr <<") (X,cT):0 "
             << "Pin: "  << m_particles_in.size()
             <<" Pout: " << m_particles_out.size() << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        ostr << "Vtx: ";
        ostr.width(6);
        ostr << barcode()
             << "  (X,cT): 0" << endl;
    }
}

void GenVertex::add_particle_in(GenParticle &p) {

    p.m_end_vertex = m_barcode;
    m_particles_in.push_back(p.m_barcode);
}

void GenVertex::add_particle_out(GenParticle &p) {

    p.m_production_vertex = m_barcode;
    m_particles_out.push_back(p.m_barcode);
}

} // namespace HepMC3

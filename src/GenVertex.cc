/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class HepMC3::GenVertex
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex():
m_barcode(0),
m_version_created(0),
m_version_deleted(255) {
}

GenVertex::~GenVertex() {}

void GenVertex::print( std::ostream& ostr, bool event_listing_format  ) const {

    // Standalone format. Used when calling:
    // vertex->print()
    if( !event_listing_format ) {
        ostr << "GenVertex: " << barcode()
             << " (X,cT):0 "
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

void GenVertex::add_particle_in(GenParticle *p) {
    if(!p) return;

    if( m_barcode && p->barcode()==0 ) {
        WARNING( "GenVertex::add_particle_in:  particle must be added to the event first!" )
        return;
    }
    if( m_version_created < p->version_created() ) {
        WARNING( "GenVertex::add_particle_in:  cannot add incoming particle to vertex from older version!" )
        return;
    }

    p->set_end_vertex(this);
    m_particles_in.push_back(p);
}

void GenVertex::add_particle_out(GenParticle *p) {
    if(!p) return;

    if( m_barcode && p->barcode()==0 ) {
        WARNING( "GenVertex::add_particle_out: particle must be added to the event first!" )
        return;
    }
    if( m_version_deleted <= p->version_created() ) {
        WARNING( "GenVertex::add_particle_out: cannot add particle to deleted vertex!" )
        return;
    }

    p->set_production_vertex(this);
    m_particles_out.push_back(p);
}

} // namespace HepMC3

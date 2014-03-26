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
m_parent_event(0),
m_barcode(0) {
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
        // Find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize prec = ostr.precision();
        ostr << "Vtx: ";
        ostr.width(6);
        ostr << barcode()
             << "  (X,cT): 0" << endl;

        // Print out all the incoming particles
        for(unsigned int i=0;i<m_particles_in.size();++i) {
            ostr << "  I ";
            m_particles_in[i]->print(ostr,1);
        }

        // Restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
}

void GenVertex::add_particle_in(GenParticle *p) {
    // If vertex is part of the event, add particle to this event
    if( m_parent_event && p->parent_event() != m_parent_event ) {
        m_parent_event->add_particle(p);
    }
    p->set_end_vertex_barcode(m_barcode);
    m_particles_in.push_back(p);
}

void GenVertex::add_particle_out(GenParticle *p) {
    // If vertex is part of the event, add particle to this event
    if( m_parent_event && p->parent_event() != m_parent_event ) {
        m_parent_event->add_particle(p);
    }
    p->set_production_vertex_barcode(m_barcode);
    m_particles_out.push_back(p);
}

void GenVertex::remove_particle(GenParticle *p) {
    for( unsigned int i=0; i<m_particles_in.size(); ++i ) {
        if( m_particles_in[i] == p ) {
            m_particles_in.erase(m_particles_in.begin()+i);
            return;
        }
    }
    for( unsigned int i=0; i<m_particles_out.size(); ++i ) {
        if( m_particles_out[i] == p ) {
            m_particles_out.erase(m_particles_out.begin()+i);
            return;
        }
    }
}

void GenVertex::set_parent_event(GenEvent *evt) {
    // If vertex already belongs to other event - remove it from that event
    if( m_parent_event && m_parent_event != evt ) {
        m_parent_event->remove_vertex(this);
    }

    // Do not add if already part of this event
    if( m_parent_event == evt) return;

    if( evt ) {
        m_barcode      = -evt->get_lowest_vertex_barcode(); //!< @todo Not the best way to do it - fix it when versioning is ready
        m_parent_event = evt;
    }
    else {
        m_barcode      = 0;
        m_parent_event = NULL;
    }
}

} // namespace HepMC3

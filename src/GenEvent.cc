/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent():
m_event_number(0),
m_highest_particle(0),
m_lowest_vertex(0) {
}

GenEvent::~GenEvent() {
    for( unsigned int i=0; i<m_particles.size(); ++i) {
        delete m_particles[i];
    }
    for( unsigned int i=0; i<m_vertices.size(); ++i) {
        delete m_vertices[i];
    }
}

void GenEvent::add_particle(GenParticle *p) {
    ++m_highest_particle;
    p->set_parent_event(this);
    m_particles.push_back(p);
}

void GenEvent::add_vertex(GenVertex *v) {
    ++m_lowest_vertex;
    v->set_parent_event(this);
    m_vertices.push_back(v);
}

void GenEvent::remove_particle(GenParticle *p) {
    int index = find_particle_index(p);
    if( index >= 0 ) m_particles.erase(m_particles.begin()+index);
    else {
        ERROR( "GenEvent:remove_particle: particle not found in the container!" )
        DEBUG( 1, "Pointer: "<<p<<" barcode: "<<p->barcode()<<" parent event pointer: "<<p->parent_event() )
        return;
    }
    p->set_parent_event(NULL);
}

void GenEvent::remove_vertex(GenVertex *v) {
    int index = find_vertex_index(v);
    if( index >= 0 ) m_vertices.erase(m_vertices.begin()+index);
    else {
        ERROR( "GenEvent:remove_vertex vertex not found in the container!" )
        DEBUG( 1, "Pointer: "<<v<<" barcode: "<<v->barcode()<<" parent event pointer: "<<v->parent_event() )
        return;
    }
    v->set_parent_event(NULL);
}

int GenEvent::find_particle_index(GenParticle *p) const {
    if(!p) return -1;

    for(unsigned int i=0;i<m_particles.size(); ++i) {
        if( p == m_particles[i] ) return i;
    }

    return -1;
}

int GenEvent::find_vertex_index(GenVertex *v) const {
    if(!v) return -1;

    for(unsigned int i=0;i<m_vertices.size(); ++i) {
        if( v == m_vertices[i] ) return i;
    }

    return -1;
}

void GenEvent::print( ostream& ostr) const {

    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Entries this event: " << m_vertices.size() << " vertices, "
       << m_particles.size() << " particles.\n";

    // Print a legend to describe the particle info
    ostr << "                                    GenParticle Legend" << endl;
    ostr << "     Barcode   PDG ID   "
       << "( Px,       Py,       Pz,     E )"
       << "   Stat-Subst  Prod V|P" << endl;
    ostr << "________________________________________________________________________________" << endl;

    // Print all particles and vertices in the event

    /** @remark Assumption is made that barcodes of vertices and particles
                are in topological order! */
    int highest_vertex_already_printed = 0;

    for( unsigned int i=0; i<m_particles.size(); ++i) {
        int production_vertex = m_particles[i]->production_vertex_barcode();
        if( production_vertex < highest_vertex_already_printed ) {
            highest_vertex_already_printed = production_vertex;
            for( unsigned int j=0; j<m_vertices.size(); ++j ) {
                if( m_vertices[j]->barcode() == production_vertex ) {
                    m_vertices[j]->print(ostr,true);
                    break;
                }
            }
        }
        ostr<<"    ";
        m_particles[i]->print(ostr,true);
    }

    ostr << "________________________________________________________________________________" << endl;
}

} // namespace HepMC3

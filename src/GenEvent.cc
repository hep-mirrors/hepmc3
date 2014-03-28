/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent():
m_event_number(0),
m_highest_particle(0),
m_lowest_vertex(0),
m_current_version_id(0) {

    // Create default version
    m_versions.push_back(GenEventVersion("Version 0"));
}

GenEvent::~GenEvent() {

}

void GenEvent::add_particle(GenParticle *p) {

    if(!p) return;

    if( p->parent_event() ) {

        // Do not add if already present
        if( p->parent_event() == this ) return;
        else {
            ERROR( "GenEvent::add_particle: already belongs to other event" )
            return;
        }
    }

    bool set_barcode = p->set_barcode(m_highest_particle+1);

    if(!set_barcode) {
        ERROR( "GenEvent::add_particle: barcode already set!" )
        return;
    }

    ++m_highest_particle;

    p->set_parent_event(this);
    m_versions[m_current_version_id].record_change(p);
}

void GenEvent::add_vertex(GenVertex *v) {

    if(!v) return;

    if( v->parent_event() ) {

        // Do not add if already present
        if( v->parent_event() == this ) return;
        else {
            ERROR( "GenEvent::add_particle: already belongs to other event" )
            return;
        }
    }

    bool set_barcode = v->set_barcode( -(m_lowest_vertex+1) );

    if(!set_barcode) {
        ERROR( "GenEvent::add_vertex: barcode already set!" )
        return;
    }

    // Store status of the incoming/outgoing particles
    // to check if they need to be added
    vector<GenParticle*> particles_to_be_added;

    // Incoming particles
    for( vector<GenParticle*>::iterator i = v->particles_in().begin(); i != v->particles_in().end(); ++i ) {
        GenEvent *parent_event = (*i)->parent_event();
        if( parent_event && parent_event != this) {
            ERROR( "GenEvent::add_vertex: one of the incoming particles belongs to other event" )
            return;
        }

        // Particle does not belong to an event - add it and continue
        if( !parent_event ) {
            particles_to_be_added.push_back(*i);
            continue;
        }
    }

    // Outgoing particles
    for( vector<GenParticle*>::iterator i = v->particles_out().begin(); i != v->particles_out().end(); ++i ) {
        GenEvent *parent_event = (*i)->parent_event();
        if( parent_event && parent_event != this) {
            ERROR( "GenEvent::add_vertex: one of the outgoing particles belongs to other event" )
            return;
        }

        // Particle does not belong to an event - add it and continue
        if( !parent_event ) {
            particles_to_be_added.push_back(*i);
            continue;
        }
    }

    ++m_lowest_vertex;

    // Add all particles and only then add vertex
    for( vector<GenParticle*>::iterator i = particles_to_be_added.begin(); i != particles_to_be_added.end(); ++i ) {
        add_particle(*i);
    }

    // Restore incoming/outgoing indexes
    for( vector<GenParticle*>::iterator i = v->particles_in().begin(); i != v->particles_in().end(); ++i ) {
        (*i)->set_end_vertex_barcode(v->barcode());
    }
    for( vector<GenParticle*>::iterator i = v->particles_out().begin(); i != v->particles_out().end(); ++i ) {
        (*i)->set_production_vertex_barcode(v->barcode());
    }

    v->set_parent_event(this);
    m_versions[m_current_version_id].record_change(v);
}

void GenEvent::delete_particle(GenParticle *p) {

    m_versions[m_current_version_id].record_deleted(p);
}

void GenEvent::delete_vertex(GenVertex *v) {

    m_versions[m_current_version_id].record_deleted(v);
}

void GenEvent::print( ostream& ostr) const {

    const vector<GenParticle*> &particles = m_versions[m_current_version_id].particles();
    const vector<GenVertex*>   &vertices  = m_versions[m_current_version_id].vertices();

    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Versions in this event: " << m_versions.size() << endl;
    ostr << " Entries this event: " << vertices.size() << " vertices, "
       << particles.size() << " particles." << endl;

    // Print a legend to describe the particle info
    ostr << "                                    GenParticle Legend" << endl;
    ostr << "     Barcode   PDG ID   "
       << "( Px,       Py,       Pz,     E )"
       << "   Stat-Subst  Prod V|P" << endl;
    ostr << "________________________________________________________________________________" << endl;

    // Print all particles and vertices in the event
    int highest_vertex_already_printed = 0;

    for( vector<GenParticle*>::const_iterator i = particles.begin(); i != particles.end(); ++i ) {
        int production_vertex = (*i)->production_vertex_barcode();

        if( production_vertex < highest_vertex_already_printed ) {
            highest_vertex_already_printed = production_vertex;

            for( vector<GenVertex*>::const_iterator j = vertices.begin(); j != vertices.end(); ++j ) {
                if( (*j)->barcode() == production_vertex ) {
                    (*j)->print(ostr,true);
                    break;
                }
            }
        }
        ostr<<"    ";
        (*i)->print(ostr,true);
    }

    ostr << "________________________________________________________________________________" << endl;
}

} // namespace HepMC3

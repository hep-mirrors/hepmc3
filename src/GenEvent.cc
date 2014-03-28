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
m_last_particle(0),
m_last_vertex(0),
m_current_version(0),
m_last_version(0) {

    // Create default version
    m_versions.push_back(GenEventVersion(0, "Version 0"));
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

    bool set_barcode = p->set_barcode(m_last_particle+1);

    if(!set_barcode) {
        ERROR( "GenEvent::add_particle: barcode already set!" )
        return;
    }

    ++m_last_particle;

    p->set_parent_event(this);
    m_versions[m_current_version].record_change(p);
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

    bool set_barcode = v->set_barcode( m_last_vertex-1 );

    if(!set_barcode) {
        ERROR( "GenEvent::add_vertex: barcode already set!" )
        return;
    }

    // Store status of the incoming/outgoing particles
    // to check if they need to be added
    vector<GenParticle*> particles_to_be_added;

    // Incoming particles
    for( vector<GenParticle*>::const_iterator i = v->particles_in().begin(); i != v->particles_in().end(); ++i ) {
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
    for( vector<GenParticle*>::const_iterator i = v->particles_out().begin(); i != v->particles_out().end(); ++i ) {
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

    --m_last_vertex;

    // Add all particles and only then add vertex
    for( vector<GenParticle*>::const_iterator i = particles_to_be_added.begin(); i != particles_to_be_added.end(); ++i ) {
        add_particle(*i);
    }

    // Restore incoming/outgoing indexes
    for( vector<GenParticle*>::const_iterator i = v->particles_in().begin(); i != v->particles_in().end(); ++i ) {
        (*i)->set_end_vertex_barcode(v->barcode());
    }
    for( vector<GenParticle*>::const_iterator i = v->particles_out().begin(); i != v->particles_out().end(); ++i ) {
        (*i)->set_production_vertex_barcode(v->barcode());
    }

    v->set_parent_event(this);
    m_versions[m_current_version].record_change(v);
}

void GenEvent::delete_particle(GenParticle *p) {

    m_versions[m_current_version].record_deleted(p);
}

void GenEvent::delete_vertex(GenVertex *v) {

    m_versions[m_current_version].record_deleted(v);
}

void GenEvent::print( ostream& ostr) const {

    const vector<GenParticle*> &particles = m_versions[m_current_version].particles();
    const vector<GenVertex*>   &vertices  = m_versions[m_current_version].vertices();

    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Version: \"" << m_versions[m_current_version].name()
         << "\" (version id: " << m_current_version << ", last version id: " << m_last_version << ")" <<endl;
    ostr << " Entries in this event: " << vertices.size() << " vertices, "
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

void GenEvent::create_new_version(const char *name) {
    m_last_version++;
    m_versions.push_back( GenEventVersion(m_last_version,name) );

    m_current_version = m_last_version;
}

void GenEvent::set_current_version(int ver) {
    if( ver<0 || ver>m_last_version ) {
        WARNING( "GenEvent::set_current_version: Version no outside of version range" )
        return;
    }

    m_current_version = ver;
}

} // namespace HepMC3

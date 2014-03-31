/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <cmath>  // std::abs
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
    m_versions.push_back( new GenEventVersion(0, "Version 0"));
}

GenEvent::~GenEvent() {
    for( vector<GenEventVersion*>::const_iterator i = m_versions.begin(); i != m_versions.end(); ++i ) {
        delete (*i);
    }
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
    m_versions[m_current_version]->record_change(p);
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
        (*i)->set_descendant(v->barcode());
    }
    for( vector<GenParticle*>::const_iterator i = v->particles_out().begin(); i != v->particles_out().end(); ++i ) {
        (*i)->set_ancestor(v->barcode());
    }

    v->set_parent_event(this);
    m_versions[m_current_version]->record_change(v);
}

void GenEvent::delete_particle(GenParticle *p) {

    p->set_version_deleted(m_last_version);
}

void GenEvent::delete_vertex(GenVertex *v) {

    v->set_version_deleted(m_last_version);
}

void GenEvent::print( ostream& ostr) const {

    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Version: \"" << m_versions[m_current_version]->name()
         << "\" (version id: " << m_current_version << ", last version id: " << m_last_version << ")" <<endl;
    ostr << " Entries in this event: " << -m_last_vertex << " vertices, "
       << m_last_particle << " particles." << endl;

    // Print a legend to describe the particle info
    ostr << "                                    GenParticle Legend" << endl;
    ostr << "     Barcode   PDG ID   "
       << "( Px,       Py,       Pz,     E )"
       << "   Stat-Subst  Prod V|P" << endl;
    ostr << "________________________________________________________________________________" << endl;

    int last_vertex_printed = 0;

    for(int i=0; i<=m_current_version; ++i) {
        vector<GenParticle*>::const_iterator p = m_versions[i]->particles().begin();
        vector<GenVertex*>::const_iterator   v = m_versions[i]->vertices().begin();

        for( ; p != m_versions[i]->particles().end(); ++p ) {
            if( (*p)->ancestor()<0 && (*p)->ancestor()<last_vertex_printed && v != m_versions[i]->vertices().end() ) {
                (*v)->print(ostr,true);
                last_vertex_printed = (*v)->barcode();
                ++v;
            }

            ostr<<"    ";
            (*p)->print(ostr,true);
        }
    }

    ostr << "________________________________________________________________________________" << endl;
}

void GenEvent::create_new_version(const char *name) {
    m_last_version++;
    m_versions.push_back( new GenEventVersion(m_last_version,name) );

    m_current_version = m_last_version;
}

void GenEvent::set_current_version(int ver) {
    if( ver<0 || ver>m_last_version ) {
        WARNING( "GenEvent::set_current_version: Version no outside of version range" )
        return;
    }

    m_current_version = ver;
}

GenVertex* GenEvent::find_vertex(int barcode) {
    for(int i=m_current_version; i>=0; --i) {
        for( vector<GenVertex*>::const_iterator j = m_versions[i]->vertices().begin(); j!=m_versions[i]->vertices().end(); ++j) {
            if( (*j)->barcode() == barcode ) return *j;
        }
    }

    return NULL;
}

GenParticle* GenEvent::find_particle(int barcode) {
    for(int i=m_current_version; i>=0; --i) {
        for( vector<GenParticle*>::const_iterator j = m_versions[i]->particles().begin(); j!=m_versions[i]->particles().end(); ++j) {
            if( (*j)->barcode() == barcode ) return *j;
        }
    }

    return NULL;
}

} // namespace HepMC3

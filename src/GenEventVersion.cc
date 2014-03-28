/**
 *  @file GenEventVersion.cc
 *  @brief Implementation of \b class HepMC3::GenEventVersion
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include "HepMC3/GenEventVersion.h"
#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

GenEventVersion::GenEventVersion(const char *name):
m_name(name) {
}

GenEventVersion::~GenEventVersion() {
    for( vector<GenParticle*>::const_iterator i = m_particles.begin(); i != m_particles.end(); ++i ) {
        delete (*i);
    }
    for( vector<GenVertex*>::const_iterator i = m_vertices.begin(); i != m_vertices.end(); ++i ) {
        delete (*i);
    }
}

void GenEventVersion::record_change(GenParticle *p) {

    DEBUG( 10, "GenEventVersion: adding particle: "<<p->barcode()<<" ("<<p->pdg_id()<<") " )

    // First entry
    if( m_particles.size() == 0 ) {
        m_particles.push_back( p );
        DEBUG( 10, "GenEventVersion: particle added as first: "<<p->barcode()<<" ("<<p->pdg_id()<<") " )
        return;
    }

    // Entry not within the list range
    if( m_particles.back()->barcode() < p->barcode() ) {
        m_particles.push_back( p );
        DEBUG( 10, "GenEventVersion: particle added as last: "<<p->barcode()<<" ("<<p->pdg_id()<<") " )
        return;
    }

    // Particle within the list range
    for( vector<GenParticle*>::iterator i = m_particles.begin(); i != m_particles.end(); ++i ) {

        // Already exists on the list - do nothing
        if ((*i)->barcode() == p->barcode() ) return;

        // Particle does not exist on the list - insert (max O(n))
        if ((*i)->barcode() >  p->barcode() ) {
            m_particles.insert( i, p );
            DEBUG( 10, "GenEventVersion: particle added in-between: "<<p->barcode()<<" ("<<p->pdg_id()<<") " )
            return;
        }
    }
}

void GenEventVersion::record_change(GenVertex *v) {

    DEBUG( 10, "GenEventVersion: adding vertex: "<<v->barcode() )

    // First entry
    if( m_vertices.size() == 0 ) {
        m_vertices.push_back( v );
        DEBUG( 10, "GenEventVersion: vertex added as first: "<<v->barcode() )
        return;
    }

    // Entry not within the list range
    if( m_vertices.back()->barcode() > v->barcode() ) {
        m_vertices.push_back( v );
        DEBUG( 10, "GenEventVersion: vertex added as last: "<<v->barcode() )
        return;
    }

    // Particle within the list range
    for( vector<GenVertex*>::iterator i = m_vertices.begin(); i != m_vertices.end(); ++i  ) {

        // Already exists on the list - do nothing
        if ((*i)->barcode() == v->barcode() ) return;

        // Particle does not exist on the list - insert (max O(n))
        if ((*i)->barcode() >  v->barcode() ) {
            m_vertices.insert( i, v );
            DEBUG( 10, "GenEventVersion: vertex added in-between: "<<v->barcode() )
            return;
        }
    }
}

void GenEventVersion::record_deleted(int barcode, vector<int> &container ) {

    // First entry
    if( container.size() == 0 ) {
        container.push_back( barcode );
        return;
    }

    // Entry not within the list range
    if( container.back() > barcode ) {
        container.push_back( barcode );
        return;
    }

    // Particle within the list range
    for( vector<int>::iterator i = container.begin(); i != container.end(); ++i ) {

        // Already exists on the list - do nothing
        if ((*i) == barcode ) return;

        // Particle does not exist on the list - insert (max O(n))
        if ((*i) >  barcode ) {
            container.insert( i, barcode);
            return;
        }
    }
}

} // namespace HepMC3

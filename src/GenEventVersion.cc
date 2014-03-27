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
    for( unsigned int i=0; i<m_particles.size(); ++i) {
        delete m_particles[i];
    }
    for( unsigned int i=0; i<m_vertices.size(); ++i) {
        delete m_vertices[i];
    }
}

bool GenEventVersion::has_record(GenParticle *p) const {
    if( m_deleted_particle_barcodes.size() ) {
        if( p->barcode() >= m_deleted_particle_barcodes.front() &&
            p->barcode() <= m_deleted_particle_barcodes.back() ) return true;
    }

    if( m_particles.size() ) {
        if( p->barcode() >= m_particles.front()->barcode() &&
            p->barcode() <= m_particles.back()->barcode() ) return true;
    }

    return false;
}

bool GenEventVersion::has_record(GenVertex *v) const {
    if( m_deleted_vertex_barcodes.size() ) {
        if( v->barcode() >= m_deleted_vertex_barcodes.front() &&
            v->barcode() <= m_deleted_vertex_barcodes.back() ) return true;
    }

    if( m_vertices.size() ) {
        if( v->barcode() >= m_vertices.front()->barcode() &&
            v->barcode() <= m_vertices.back()->barcode() ) return true;
    }

    return false;
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
    for( unsigned int i=0; i<m_particles.size(); ++i ) {

        // Already exists on the list - do nothing
        if (m_particles[i]->barcode() == p->barcode() ) return;

        // Particle does not exist on the list - insert (max O(n))
        if (m_particles[i]->barcode() >  p->barcode() ) {
            m_particles.insert( m_particles.begin() + i, p );
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
    for( unsigned int i=0; i<m_vertices.size(); ++i ) {

        // Already exists on the list - do nothing
        if (m_vertices[i]->barcode() == v->barcode() ) return;

        // Particle does not exist on the list - insert (max O(n))
        if (m_vertices[i]->barcode() >  v->barcode() ) {
            m_vertices.insert( m_vertices.begin() + i, v );
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
    for( unsigned int i=0; i<container.size(); ++i ) {

        // Already exists on the list - do nothing
        if (container[i] == barcode ) return;

        // Particle does not exist on the list - insert (max O(n))
        if (container[i] >  barcode ) {
            container.insert( container.begin() + i, barcode);
            return;
        }
    }
}

} // namespace HepMC3

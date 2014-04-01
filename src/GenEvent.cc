/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <boost/foreach.hpp>
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

    ++m_last_particle;

    p->set_barcode(m_last_particle);
    p->set_version_created(m_last_version);
    m_versions[m_last_version]->record_change(p);
}

void GenEvent::add_vertex(GenVertex *v) {
    if(!v) return;

    // Add all incoming particles
    BOOST_FOREACH( GenParticle *p, v->particles_in() ) {
        if( !p->barcode() ) add_particle(p);
    }

    // Add all outgoing particles
    BOOST_FOREACH( GenParticle *p, v->particles_out() ) {
        if( !p->barcode() ) add_particle(p);
    }

    --m_last_vertex;

    v->set_barcode(m_last_vertex);
    v->set_version_created(m_last_version);
    m_versions[m_last_version]->record_change(v);
}

void GenEvent::delete_particle(GenParticle *p) {

    p->set_version_deleted(m_last_version);

    // Removing incoming particle from the vertex makes this vertex
    // and all subsequent vertices and particles invalid
    if(p->end_vertex()) delete_vertex(p->end_vertex());
}

void GenEvent::delete_vertex(GenVertex *v) {

    v->set_version_deleted(m_last_version);

    // Removing incoming particle from the vertex makes this vertex
    // and all subsequent vertices and particles invalid
    BOOST_FOREACH( GenParticle *p, v->particles_out() ) {
        p->set_version_deleted(m_last_version);
        if(p->end_vertex()) delete_vertex(p->end_vertex());
    }
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
       << "   Stat-Subst  ProdVtx" << endl;
    ostr << "________________________________________________________________________________" << endl;

    BOOST_FOREACH( GenEventVersion *ver, boost::make_iterator_range(m_versions.begin(), m_versions.begin()+m_current_version+1) ) {
        BOOST_FOREACH( GenVertex *v, ver->vertices() ) {
            if( v->version_deleted() <= m_current_version ) continue;
            if( v->version_created() >  m_current_version ) continue;
            v->print(ostr,1);

            bool printed_header = false;

            // Print out all the incoming particles
            BOOST_FOREACH( GenParticle *p, v->particles_in() ) {
                if( p->version_deleted() <= m_current_version ) continue;
                if( p->version_created() >  m_current_version ) continue;
                if( !printed_header ) {
                    ostr << " I: ";
                    printed_header = true;
                }
                else ostr << "    ";
                p->print(ostr,1);
            }

            printed_header = false;

            // Print out all the outgoing particles
            BOOST_FOREACH( GenParticle *p, v->particles_out() ) {
                if( p->version_deleted() <= m_current_version ) continue;
                if( p->version_created() >  m_current_version ) continue;

                if( !printed_header ) {
                    ostr << " O: ";
                    printed_header = true;
                }
                else ostr << "    ";
                p->print(ostr,1);
            }
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

} // namespace HepMC3

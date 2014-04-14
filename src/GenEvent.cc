/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"

#include "HepMC3/Search/FindParticles.h"

#include <vector>
#include <stdexcept> // std::out_of_range
#include <cmath>     // std::abs

#include <boost/range/iterator_range.hpp>
#include <boost/foreach.hpp>
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent():
m_event_number(0),
m_print_precision(2) {

    // Create default version
    m_versions.push_back( "Version 0" );
}

void GenEvent::print_version( unsigned short int version, std::ostream& ostr ) const {
    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Version: \"" << m_versions[version-1]
         << "\" (version id: " << version << ", last version id: " << m_versions.size() << ")" <<endl;
    ostr << " Entries in this event: " << m_vertices.size() << " vertices, "
         << m_particles.size() << " particles." << endl;

    // Print a legend to describe the particle info
    ostr << "                                    GenParticle Legend" << endl;
    ostr << "     Barcode   PDG ID   "
         << "( Px,       Py,       Pz,     E )"
         << "   Stat-Subst  ProdVtx" << endl;
    ostr << "________________________________________________________________________________" << endl;

    // Find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize         prec = ostr.precision();

    // Set precision
    ostr.precision(m_print_precision);

    FindParticles search(*this, FIND_ALL, VERSION_CREATED <= version && VERSION_DELETED > version);

    int last_vertex_barcode = 0;

    BOOST_FOREACH( const GenParticle *p, search.results() ) {
        if(p->end_vertex()) {
            if( last_vertex_barcode <= p->end_vertex()->barcode() ) continue;

            const GenVertex *v = p->end_vertex();

            last_vertex_barcode = v->barcode();

            v->print(ostr,1);

            bool printed_header = false;

            // Print out all the incoming particles
            BOOST_FOREACH( GenParticle *p, v->particles_in() ) {

                if( p->m_version_created > version || p->m_version_deleted <= version) continue;

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

                if( p->m_version_created > version || p->m_version_deleted <= version) continue;

                if( !printed_header ) {
                    ostr << " O: ";
                    printed_header = true;
                }
                else ostr << "    ";

                p->print(ostr,1);
            }
        }
    }

    // Restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
    ostr << "________________________________________________________________________________" << endl;
}

void GenEvent::dump() const {
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<"--------- EVENT DUMP --------"<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<std::endl;

    std::cout<<"GenParticle:"<<std::endl;
    for(unsigned int i=0; i<m_particles.size(); ++i) {
        m_particles[i].print();
    }
    std::cout<<"GenParticleData:"<<std::endl;
    for(unsigned int i=0; i<m_particle_data.size(); ++i) {
        std::cout<<i<<": ";
        m_particle_data[i].print();
    }
    std::cout<<"GenVertex:"<<std::endl;
    for(unsigned int i=0; i<m_vertices.size(); ++i) {
        std::cout<<i<<": ";
        m_vertices[i].print();
    }
    std::cout<<"GenVertexData:"<<std::endl;
    for(unsigned int i=0; i<m_vertex_data.size(); ++i) {
        std::cout<<i<<": ";
        m_vertex_data[i].print();
    }
}

GenParticle& GenEvent::particle(int barcode) const {
    if ( barcode <= 0 || barcode > (int)m_particles.size() ) throw std::out_of_range( "GenEvent::get_particle" );

    return m_particles[ barcode - 1 ];
}

GenVertex& GenEvent::vertex(int barcode) const {
    barcode = -barcode;
    if ( barcode <= 0 || barcode >  (int)m_vertices.size() ) throw std::out_of_range( "GenEvent::get_vertex" );

    return m_vertices[ barcode - 1 ];
}

GenParticle& GenEvent::new_particle( const GenParticleData *data ) {

    GenParticle      *p = m_particles.new_uninitialized_object();
    GenParticleData *pd = m_particle_data.new_object();

    new (p) GenParticle( *this, m_particle_data.size() - 1, *pd );

    if(data) *pd = *data; // copy

    return *p;
}

GenVertex& GenEvent::new_vertex( const GenVertexData *data ) {

    GenVertex      *v = m_vertices.new_uninitialized_object();
    GenVertexData *vd = m_vertex_data.new_object();

    new (v) GenVertex( *this, m_vertex_data.size() - 1, *vd );

    if(data) *vd = *data; // copy

    return *v;
}

void GenEvent::delete_particle(GenParticle &p) {
    if( p.is_deleted() ) return;

    // Deleting particle invalidates rest of the decay tree
    if( p.m_end_vertex ) delete_vertex( *p.m_end_vertex );

    p.mark_deleted();
}

void GenEvent::delete_vertex(GenVertex &v) {
    if( v.is_deleted() ) return;

    // Deleting vertex invalidates rest of the decay tree
    BOOST_FOREACH( GenParticle *p, v.m_particles_out ) {
        delete_particle(*p);
    }

    v.mark_deleted();
}

void GenEvent::new_version( const std::string name ) {
    m_versions.push_back( name );
}

void GenEvent::record_change(GenParticle& p) {

    // Check if this particle already exists in the newest version
    if( p.m_last_version->m_version_created == last_version() ) return;

    if( p.is_deleted() ) {
        WARNING( "GenEvent::record_change: Cannot change deleted particle" )
        return;
    }

    // Create new particle as a copy of previous one
    GenParticle      *new_p = m_particles.new_uninitialized_object();
    GenParticleData *new_pd = m_particle_data.new_object(p.m_data);

    new (new_p) GenParticle( *this, m_particle_data.size() - 1, *new_pd );

    // Copy GenParticle information
    new_p->m_production_vertex = p.m_production_vertex;
    new_p->m_end_vertex        = p.m_end_vertex;

    // Mark this particle as deleted and update last version pointer
    if( !p.is_deleted() ) p.m_version_deleted = last_version();
    p.m_last_version = new_p;

    // Add new particle to production/end vertices
    if( new_p->m_production_vertex ) new_p->m_production_vertex->add_particle_out(*new_p);
    if( new_p->m_end_vertex )        new_p->m_end_vertex->add_particle_in(*new_p);
}

void GenEvent::record_change(GenVertex& v) {
}

} // namespace HepMC3

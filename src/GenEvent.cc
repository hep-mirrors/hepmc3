/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"

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

void GenEvent::print_version( unsigned int version, std::ostream& ostr ) const {
    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Version: \"" << m_versions[version]
         << "\" (version id: " << version << ", last version id: " << m_versions.size()-1 << ")" <<endl;
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

    for( unsigned int i=0; i<m_vertices.size(); ++i ) {
        GenVertex &v = m_vertices[i];
        if( v.m_version_deleted <= version || v.m_version_created > version ) continue;
        v.print(ostr,1);

        bool printed_header = false;


        // Print out all the incoming particles
        BOOST_FOREACH( int barcode, v.particles_in() ) {
            const GenParticle &p = m_particles[barcode-1];

            if( p.m_version_deleted <= version || p.m_version_created > version ) continue;
            if( !printed_header ) {
                ostr << " I: ";
                printed_header = true;
            }
            else ostr << "    ";
            p.print(ostr,1);
        }

        printed_header = false;

        // Print out all the outgoing particles
        BOOST_FOREACH( int barcode, v.particles_out() ) {
            const GenParticle &p = m_particles[barcode-1];

            if( p.m_version_deleted <= version || p.m_version_created > version ) continue;
            if( !printed_header ) {
                ostr << " O: ";
                printed_header = true;
            }
            else ostr << "    ";
            p.print(ostr,1);
        }
    }

    // Restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
    ostr << "________________________________________________________________________________" << endl;
}

void GenEvent::dump() const {
    for(unsigned int i=0; i<m_particles.size(); ++i) {
        m_particles[i].print();
    }
    for(unsigned int i=0; i<m_vertices.size(); ++i) {
        m_vertices[i].print();
    }
}

GenParticle& GenEvent::get_particle(int barcode) const {
    if ( barcode <= 0 || barcode > (int)m_particles.size() ) throw std::out_of_range( "GenEvent::get_particle" );

    return m_particles[ barcode - 1 ];
}

GenVertex&   GenEvent::get_vertex(int barcode) const {
    barcode = -barcode;
    if ( barcode <= 0 || barcode >  (int)m_vertices.size() ) throw std::out_of_range( "GenEvent::get_vertex" );

    return m_vertices[ barcode - 1 ];
}

GenParticle& GenEvent::new_particle() {

    unsigned int barcode = m_particles.size();

    m_particles.increase_size();

    GenParticle &p = m_particles[barcode];

    p.m_event           = this;
    p.m_barcode         = barcode + 1;
    p.m_version_created = m_versions.size() - 1;

    return p;
}

GenVertex& GenEvent::new_vertex() {

    unsigned int barcode = m_vertices.size();

    m_vertices.increase_size();
    m_vertices[barcode].m_event   = this;
    m_vertices[barcode].m_barcode = -(barcode + 1);

    return m_vertices[barcode];
}

void GenEvent::delete_particle(GenParticle &p) {
    if( p.m_version_deleted == 255 ) p.m_version_deleted = m_versions.size() - 1;

    // Deleting particle invalidates rest of the decay tree
    if( p.m_end_vertex ) delete_vertex( m_vertices[ (-p.m_end_vertex) - 1 ] );
}

void GenEvent::delete_vertex(GenVertex &v) {
    v.m_version_deleted = m_versions.size() - 1;

    // Deleting vertex invalidates rest of the decay tree
    BOOST_FOREACH( int p_barcode, v.particles_out() ) {
        delete_particle( m_particles[p_barcode-1] );
    }
}

void GenEvent::new_version( const std::string name ) {
    m_versions.push_back( name );
}

int GenEvent::record_change(GenParticle& p) {
    int version = m_versions.size() - 1;

    if( p.m_version_deleted <= version ) {
        WARNING( "GenEvent::record_change: Cannot change deleted particle" )
        return 0;
    }

    // Return barcode of this particle if only one version present
    if( version==0 ) return p.m_barcode;

    // Check if this particle has already been added to the new version
    if( p.m_next_version && m_particles[ p.m_next_version-1 ].m_version_created == version+1 ) {
        return p.m_next_version;
    }

    // Adding particle to new version adds its downstream
    if(p.m_end_vertex) record_change( m_vertices[ (-p.m_end_vertex)-1 ] );

    GenParticle &next       = new_particle();
    next = p;
    next.m_barcode          = m_particles.size();
    next.m_version_created  = version;
    p.m_version_deleted     = version;
    p.m_next_version        = next.m_barcode;

    // New particle needs to be added to production vertex
    if(p.m_production_vertex) {
        m_vertices[ (-p.m_production_vertex)-1 ].add_particle_out(next);
    }

    return next.m_barcode;
}

int GenEvent::record_change(GenVertex& p) {
    return 0;
}

} // namespace HepMC3

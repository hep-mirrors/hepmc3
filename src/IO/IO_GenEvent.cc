/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class HepMC3::IO_GenEvent
 *
 */
#include "HepMC3/IO/IO_GenEvent.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Setup.h"

#include <limits>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#include <boost/range/iterator_range.hpp>
#include <boost/foreach.hpp>
using std::endl;
using std::vector;
using std::pair;
using std::set;

namespace HepMC3 {

IO_GenEvent::IO_GenEvent(const std::string &filename, std::ios::openmode mode):
IO_Base(filename,mode),
m_precision(16),
m_buffer(NULL),
m_cursor(NULL),
m_buffer_size( 256*1024 ) {
}

IO_GenEvent::~IO_GenEvent() {
    forced_flush();
    if( m_buffer ) delete[] m_buffer;
}

void IO_GenEvent::write_event(const GenEvent &evt) {
    if ( m_file.rdstate() ) return;
    if ( m_mode != std::ios::out ) {
        ERROR( "IO_GenEvent: attempting to write to input file" )
        return;
    }

    allocate_buffer();
    if( !m_buffer ) return;

    // Make sure nothing was left from previous event
    flush();

    m_cursor += sprintf(m_cursor, "E %i %i %i\n",evt.event_number(),evt.vertices_count(),evt.particles_count());
    flush();

    int vertices_processed    = 0;
    int lowest_vertex_barcode = 0;

    set<pair<int,int> > deleted_barcodes;

    for(unsigned int i=0; i<evt.data().versions.size(); ++i) {

        const GenEventVersionInfo &v = evt.data().versions[i];

        m_cursor += sprintf(m_cursor, "T ");
        flush();

        write_string( evt.data().versions[i].name );

        m_cursor += sprintf(m_cursor, "\n");
        flush();

        // Get the upper range of particles and vertices
        int last_particle_index = evt.particles_count() - 1;
        int last_vertex_index   = evt.vertices_count()  - 1;

        if( i+1 < evt.data().versions.size() ) {
            last_particle_index = evt.data().versions[i+1].first_particle_index - 1;
            last_vertex_index   = evt.data().versions[i+1].first_vertex_index - 1;
        }

        // Print all particles/vertices deleted in this version
        typedef std::pair<int,int> ___int_int_pair___;

        BOOST_FOREACH( const ___int_int_pair___ &del, deleted_barcodes ) {

            if(del.first>(int)i+1) {
                deleted_barcodes.erase( deleted_barcodes.begin(), deleted_barcodes.find(del) );
                break;
            }

            if( del.second > 0 ) m_cursor += sprintf(m_cursor, "P %i X\n", del.second);
            else                 m_cursor += sprintf(m_cursor, "V %i X\n", del.second);

            flush();
        }

        // Print particles
        for( int j=v.first_particle_index; j<=last_particle_index; ++j ) {

            const GenParticle &p = evt.particles()[j];

            // Ignore particles that were added and removed in the same version
            if( p.version_deleted() == i+1 ) continue;

            // Check to see if we need to write a vertex first
            int production_vertex = p.data().production_vertex;

            if( production_vertex < 0 ) {
                GenVertex &v = evt.vertex( production_vertex );

                production_vertex = v.serialization_barcode();

                if (production_vertex < lowest_vertex_barcode) {
                    write_vertex(v);
                    if( v.version_deleted()<std::numeric_limits<unsigned char>::max() && !v.has_new_version() ) {
                        deleted_barcodes.insert( pair<int,int>( v.version_deleted(),v.barcode() ) );
                    }
                }

                ++vertices_processed;
                lowest_vertex_barcode = v.barcode();
            }

            // Check if this is a new version of some other particle
            int old_version = 0;

            BOOST_FOREACH( const ___int_int_pair___ &ver, evt.data().version_links ) {

                if( ver.first == p.barcode() ) {
                    old_version = ver.second;
                    break;
                }
            }

            if( old_version ) write_particle( p, old_version, true  );
            else              write_particle( p, production_vertex,    false );

            if( p.version_deleted()<std::numeric_limits<unsigned char>::max() && !p.has_new_version() ) {
                deleted_barcodes.insert( pair<int,int>( p.version_deleted(),p.barcode() ) );
            }
        }

        // Write any remaining vertices
        while( vertices_processed<last_vertex_index ) {
            GenVertex &v = evt.vertices()[vertices_processed];

            int barcode = v.serialization_barcode();

            if( barcode < 0 ) {
                write_vertex( v );
                if( v.version_deleted()<std::numeric_limits<unsigned char>::max() && !v.has_new_version() ) {
                    deleted_barcodes.insert( pair<int,int>( v.version_deleted(),v.barcode() ) );
                }
            }

            ++vertices_processed;
            lowest_vertex_barcode = v.barcode();
        }
    }

    // Flush rest of the buffer to file
    flush();
}

bool IO_GenEvent::fill_next_event(GenEvent &evt) {
    if ( m_file.rdstate() ) return 0;
    if ( m_mode != std::ios::in ) {
        ERROR( "IO_GenEvent: attempting to read from output file" )
        return 0;
    }

    WARNING( "IO_GenEvent: Reading not implemented (yet)" )

    return 1;
}

void IO_GenEvent::allocate_buffer() {
    if( m_buffer ) return;
    while( !m_buffer && m_buffer_size >= 256 ) {
        m_buffer = new char[ m_buffer_size ]();
        if(!m_buffer) {
            m_buffer_size /= 2;
            WARNING( "IO_GenEvent::allocate_buffer: buffer size too large. Dividing by 2. New size: "<<m_buffer_size )
        }
    }

    if( !m_buffer ) {
        ERROR( "IO_GenEvent::allocate_buffer: could not allocate buffer!" )
        return;
    }

    m_cursor = m_buffer;
}

void IO_GenEvent::write_vertex(const GenVertex &v) {

    m_cursor += sprintf( m_cursor, "V %i [",v.barcode() );
    flush();

    bool printed_first = false;

    BOOST_FOREACH( const GenParticle *p, v.particles_in() ) {
        // NOTE: particles from higher version can only be added if they are
        //       a new version of already existing incoming particle.
        //       Therefore, we skip them
        if( p->version_created() > v.version_created() ) continue;

        if( !printed_first ) {
            m_cursor  += sprintf(m_cursor,"%i", p->barcode());
            printed_first = true;
        }
        else m_cursor += sprintf(m_cursor,",%i",p->barcode());

        flush();
    }

    const FourVector &pos = v.position();
    if( !pos.is_zero() ) {
        m_cursor += sprintf(m_cursor,"] @ %.*e",m_precision,pos.x());
        flush();
        m_cursor += sprintf(m_cursor," %.*e",   m_precision,pos.y());
        flush();
        m_cursor += sprintf(m_cursor," %.*e",   m_precision,pos.z());
        flush();
        m_cursor += sprintf(m_cursor," %.*e\n", m_precision,pos.t());
        flush();
    }
    else {
        m_cursor += sprintf(m_cursor,"]\n");
        flush();
    }
}

void IO_GenEvent::write_particle(const GenParticle &p, int second_field, bool is_new_version) {

    m_cursor += sprintf(m_cursor,"P %i",p.barcode());
    flush();

    if ( is_new_version ) m_cursor += sprintf(m_cursor," X%i",second_field);
    else                  m_cursor += sprintf(m_cursor," %i",second_field);
    flush();

    m_cursor += sprintf(m_cursor," %i",   p.pdg_id() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p.momentum().px() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p.momentum().py());
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p.momentum().pz() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p.momentum().e() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p.generated_mass() );
    flush();
    m_cursor += sprintf(m_cursor," %i\n", p.status() );
    flush();
}

inline void IO_GenEvent::write_string( const std::string &str ) {

    // First let's check if string will fit into the buffer
    unsigned long length = m_cursor-m_buffer;

    if( m_buffer_size - length > str.length() ) {
        strncpy(m_cursor,str.data(),str.length());
        m_cursor += str.length();
        flush();
    }
    // If not, flush the buffer and write the string directly
    else {
        forced_flush();
        m_file.write( str.data(), str.length() );
    }
}

inline void IO_GenEvent::flush() {
    // The maximum size of single add to the buffer (other than by
    // using IO_GenEvent::write) is 32 bytes. This is a safe value as
    // we will not allow precision larger than 24 anyway
    unsigned long length = m_cursor-m_buffer;
    if( m_buffer_size - length < 32 ) {
        m_file.write( m_buffer, length );
        m_cursor = m_buffer;
    }
}

inline void IO_GenEvent::forced_flush() {
    m_file.write( m_buffer, m_cursor-m_buffer );
    m_cursor = m_buffer;
}

} // namespace HepMC3

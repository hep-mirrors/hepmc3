/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class IO_GenEvent
 *
 */
#include "HepMC3/IO/IO_GenEvent.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Setup.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <boost/foreach.hpp>
using std::vector;

namespace HepMC3 {

IO_GenEvent::IO_GenEvent(const std::string &filename, std::ios::openmode mode):
IO_FileBase(filename,mode),
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

    int vertices_processed = 0;
    int lowest_vertex_id   = 0;

    m_cursor += sprintf(m_cursor, "T Version 0");
    flush();

    m_cursor += sprintf(m_cursor, "\n");
    flush();

    // Print particles
    BOOST_FOREACH( const GenParticlePtr &p, evt.particles() ) {

        // Check to see if we need to write a vertex first
        const GenVertexPtr &v = p->production_vertex();
        int production_vertex = 0;

        if(v) {

            // Check if we need this vertex at all
            if( v->particles_in().size() > 1 || !v->data().position.is_zero() ) production_vertex = v->id();
            else if( v->particles_in().size() == 1 )                            production_vertex = v->particles_in()[0]->id();

            if (production_vertex < lowest_vertex_id) {
                write_vertex(v);
            }

            ++vertices_processed;
            lowest_vertex_id = v->id();
        }

        write_particle( p, production_vertex, false );
    }

    // Flush rest of the buffer to file
    forced_flush();
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

void IO_GenEvent::write_vertex(const GenVertexPtr &v) {

    m_cursor += sprintf( m_cursor, "V %i [",v->id() );
    flush();

    bool printed_first = false;

    BOOST_FOREACH( const GenParticlePtr &p, v->particles_in() ) {

        if( !printed_first ) {
            m_cursor  += sprintf(m_cursor,"%i", p->id());
            printed_first = true;
        }
        else m_cursor += sprintf(m_cursor,",%i",p->id());

        flush();
    }

    const FourVector &pos = v->position();
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

void IO_GenEvent::write_particle(const GenParticlePtr &p, int second_field, bool is_new_version) {

    m_cursor += sprintf(m_cursor,"P %i",p->id());
    flush();

    if ( is_new_version ) m_cursor += sprintf(m_cursor," X%i",second_field);
    else                  m_cursor += sprintf(m_cursor," %i",second_field);
    flush();

    m_cursor += sprintf(m_cursor," %i",   p->pdg_id() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p->momentum().px() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p->momentum().py());
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p->momentum().pz() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p->momentum().e() );
    flush();
    m_cursor += sprintf(m_cursor," %.*e", m_precision,p->generated_mass() );
    flush();
    m_cursor += sprintf(m_cursor," %i\n", p->status() );
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

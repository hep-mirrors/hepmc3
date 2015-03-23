// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
///
/// @file WriterAscii.cc
/// @brief Implementation of \b class WriterAscii
///
#include "HepMC/WriterAscii.h"

#include "HepMC/Version.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Units.h"
#include "HepMC/foreach.h"

namespace HepMC {

WriterAscii::WriterAscii(const std::string &filename):
m_file(filename),
m_precision(16),
m_buffer(NULL),
m_cursor(NULL),
m_buffer_size( 256*1024 ) {
    if( !m_file.is_open() ) {
        ERROR( "WriterAscii: could not open output file: "<<filename )
    }
    else {
        m_file << "HepMC::Version " << HepMC::version() << std::endl;
        m_file << "HepMC::IO_GenEvent-START_EVENT_LISTING" << std::endl;
    }
}

WriterAscii::~WriterAscii() {
    close();

    if( m_buffer ) delete[] m_buffer;
}

void WriterAscii::write_event(const GenEvent &evt) {
    if( !m_file.is_open() ) return;

    allocate_buffer();
    if( !m_buffer ) return;

    // Make sure nothing was left from previous event
    flush();

    // Write event info
    m_cursor += sprintf(m_cursor, "E %i %i %i\n",evt.event_number(),evt.vertices_count(),evt.particles_count());
    flush();

    // Write units
    m_cursor += sprintf(m_cursor, "U %s %s\n",Units::name(evt.momentum_unit()).c_str(),Units::name(evt.length_unit()).c_str());
    flush();

    // Write global attributes
    typedef map< std::string, shared_ptr<Attribute> >::value_type value_typeG;
    FOREACH( value_typeG vglob, m_global_attributes )
      m_cursor += sprintf(m_cursor, "A G %s \n",vglob.first.c_str());

    // Write attributes
    typedef map< string, map<int, shared_ptr<Attribute> > >::value_type value_type1;
    typedef map<int, shared_ptr<Attribute> >::value_type                value_type2;

    FOREACH( const value_type1& vt1, evt.attributes() ) {
        FOREACH( const value_type2& vt2, vt1.second ) {

	    if ( skip_global(vt1.first, vt2.second) ) continue;

            string st;

            bool status = vt2.second->to_string(st);

            if( !status ) {
                WARNING( "WriterAscii::write_event: problem serializing attribute: "<<vt1.first )
            }
            else {
	        if ( vt2.second->is_global() ) {
		    m_cursor +=
		        sprintf(m_cursor, "A G %s ",vt1.first.c_str());
	        } else {
                    m_cursor +=
		        sprintf(m_cursor, "A %i %s ",vt2.first,vt1.first.c_str());
	        }
                flush();
                write_string(escape(st));
                m_cursor += sprintf(m_cursor, "\n");
                flush();
            }
        }
    }

    int vertices_processed = 0;
    int lowest_vertex_id   = 0;

    // Print particles
    FOREACH( const GenParticlePtr &p, evt.particles() ) {

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

        write_particle( p, production_vertex );
    }

    // Flush rest of the buffer to file
    forced_flush();
}

void WriterAscii::allocate_buffer() {
    if( m_buffer ) return;
    while( !m_buffer && m_buffer_size >= 256 ) {
        m_buffer = new char[ m_buffer_size ]();
        if(!m_buffer) {
            m_buffer_size /= 2;
            WARNING( "WriterAscii::allocate_buffer: buffer size too large. Dividing by 2. New size: "<<m_buffer_size )
        }
    }

    if( !m_buffer ) {
        ERROR( "WriterAscii::allocate_buffer: could not allocate buffer!" )
        return;
    }

    m_cursor = m_buffer;
}

std::string WriterAscii::escape(const std::string s) {
    std::string ret;
    ret.reserve( s.length()*2 );
    for ( std::string::const_iterator it = s.begin(); it != s.end(); ++it ) {
        switch ( *it ) {
            case '\\': ret += "\\\\"; break;
            case '\n': ret += "\\|"; break;
            default: ret += *it;
        }
    }
    return ret;
}

bool WriterAscii::skip_global(std::string name, shared_ptr<Attribute> att) {
    if ( !att->is_global() ) return false;
    std::map< std::string, shared_ptr<Attribute> >::iterator globit = m_global_attributes.find(name);
    if ( globit != m_global_attributes.end() ) return true;
    m_global_attributes[name] = att;
    return false;
}

void WriterAscii::write_vertex(const GenVertexPtr &v) {

    m_cursor += sprintf( m_cursor, "V %i [",v->id() );
    flush();

    bool printed_first = false;

    FOREACH( const GenParticlePtr &p, v->particles_in() ) {

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

inline void WriterAscii::flush() {
    // The maximum size of single add to the buffer (other than by
    // using WriterAscii::write) is 32 bytes. This is a safe value as
    // we will not allow precision larger than 24 anyway
    unsigned long length = m_cursor-m_buffer;
    if( m_buffer_size - length < 32 ) {
        m_file.write( m_buffer, length );
        m_cursor = m_buffer;
    }
}

inline void WriterAscii::forced_flush() {
    m_file.write( m_buffer, m_cursor-m_buffer );
    m_cursor = m_buffer;
}

void WriterAscii::write_particle(const GenParticlePtr &p, int second_field) {

    m_cursor += sprintf(m_cursor,"P %i",p->id());
    flush();

    m_cursor += sprintf(m_cursor," %i",   second_field);
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

inline void WriterAscii::write_string( const std::string &str ) {

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

void WriterAscii::close() {
    if( !m_file.is_open() ) return;

    forced_flush();
    m_file << "HepMC::IO_GenEvent-END_EVENT_LISTING" << std::endl << std::endl;
    m_file.close();
}

} // namespace HepMC

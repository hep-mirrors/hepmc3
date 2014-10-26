/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class IO_GenEvent
 *
 */
#include "HepMC/IO/IO_GenEvent.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Setup.h"
#include "HepMC/Units.h"
#include "HepMC/GenHeavyIon.h"
#include "HepMC/GenPdfInfo.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

#include <HepMC/foreach.h>

using std::vector;

namespace HepMC {

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

    // Write event info
    m_cursor += sprintf(m_cursor, "E %i %i %i\n",evt.event_number(),evt.vertices_count(),evt.particles_count());
    flush();

    // Write units
    m_cursor += sprintf(m_cursor, "U %s %s\n",Units::name(evt.momentum_unit()).c_str(),Units::name(evt.length_unit()).c_str());
    flush();

    // Write heavy ion information (if present)
    const GenHeavyIon *hi = evt.heavy_ion();
    if(hi) {
        m_cursor += sprintf(m_cursor,"H %i %i %i",hi->Ncoll_hard,hi->Npart_proj,hi->Npart_targ);
        flush();
        m_cursor += sprintf(m_cursor," %i %i %i",hi->Ncoll,hi->spectator_neutrons,hi->spectator_protons);
        flush();
        m_cursor += sprintf(m_cursor," %i %i %i",hi->N_Nwounded_collisions,hi->Nwounded_N_collisions,hi->Nwounded_Nwounded_collisions);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,hi->impact_parameter);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,hi->event_plane_angle);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,hi->eccentricity);
        flush();
        m_cursor += sprintf(m_cursor," %.*e\n",m_precision,hi->sigma_inel_NN);
        flush();
    }

    // Write pdf information (if present)
    const GenPdfInfo *pi = evt.pdf_info();
    if(pi) {
        m_cursor += sprintf(m_cursor,"F %i %i",pi->parton_id[0],pi->parton_id[1]);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,pi->x[0]);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,pi->x[1]);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,pi->scale);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,pi->xf[0]);
        flush();
        m_cursor += sprintf(m_cursor," %.*e",m_precision,pi->xf[1]);
        flush();
        m_cursor += sprintf(m_cursor," %i %i\n",pi->pdf_id[0],pi->pdf_id[1]);
        flush();
    }

    // Write cross-section information (if present)
    const GenCrossSection *cs = evt.cross_section();
    if(cs) {
        m_cursor += sprintf(m_cursor,"C %.*e",m_precision,cs->cross_section);
        flush();
        m_cursor += sprintf(m_cursor," %.*e\n",m_precision,cs->cross_section_error);
        flush();
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

void IO_GenEvent::write_particle(const GenParticlePtr &p, int second_field) {

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

bool IO_GenEvent::fill_next_event(GenEvent &evt) {
    if ( m_file.rdstate() ) return false;
    if ( m_mode != std::ios::in ) {
        ERROR( "IO_GenEvent: attempting to read from output file" )
        return false;
    }

    char               buf[512];
    bool               parsed_event_header    = false;
    bool               is_parsing_successful  = true;
    std::pair<int,int> vertices_and_particles(0,0);

    //
    // Parse event, vertex and particle information
    //
    while(!m_file.rdstate()) {
        m_file.getline(buf,512);

        if( strlen(buf) == 0 ) continue;

        // Check for IO_GenEvent header/footer
        if( strncmp(buf,"HepMC",5) == 0 ) {
            if(parsed_event_header) {
                is_parsing_successful = true;
                break;
            }
            continue;
        }

        switch(buf[0]) {
            case 'E':
                vertices_and_particles = parse_event_information(evt,buf);
                if(vertices_and_particles.second<0) {
                    is_parsing_successful = false;
                }
                else {
                    is_parsing_successful = true;
                    parsed_event_header   = true;
                }
                break;
            case 'V':
                is_parsing_successful = parse_vertex_information(evt,buf);
                break;
            case 'P':
                is_parsing_successful = parse_particle_information(evt,buf);
                break;
            case 'U':
                is_parsing_successful = parse_units(evt,buf);
                break;
            case 'F':
                is_parsing_successful = parse_pdf_info(evt,buf);
                break;
            case 'H':
                is_parsing_successful = parse_heavy_ion(evt,buf);
                break;
            case 'C':
                is_parsing_successful = parse_cross_section(evt,buf);
                break;
            default:
                WARNING( "IO_GenEvent: skipping unrecognised prefix: " << buf[0] )
                is_parsing_successful = true;
                break;
        }

        if( !is_parsing_successful ) break;

        // Check for next event
        buf[0] = m_file.peek();
        if( parsed_event_header && buf[0]=='E' ) break;
    }

    // Check if all particles and vertices were parsed
    if( (int)evt.vertices_count()  != vertices_and_particles.first ||
        (int)evt.particles_count() != vertices_and_particles.second ) {
        ERROR( "IO_GenEvent: not all particles or vertices were parsed" )
        is_parsing_successful = false;
    }

    // Check if there were errors during parsing
    if( !is_parsing_successful ) {
        ERROR( "IO_GenEvent: event parsing failed. Returning empty event" )
        DEBUG( 1, "Parsing failed at line:" << std::endl << buf )

        evt.clear();
        m_file.clear(std::ios::badbit);

        return false;
    }

    return true;
}

std::pair<int,int> IO_GenEvent::parse_event_information(GenEvent &evt, const char *buf) {
    static const std::pair<int,int>  err(-1,-1);
    std::pair<int,int>               ret(-1,-1);
    const char                      *cursor   = buf;
    int                              event_no = 0;

    // event number
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    event_no = atoi(cursor);
    evt.set_event_number(event_no);

    // num_vertices
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    ret.first = atoi(cursor);

    // num_particles
    if( !(cursor = strchr(cursor+1,' ')) ) return err;
    ret.second = atoi(cursor);

    DEBUG( 10, "IO_GenEvent: E: "<<event_no<<" ("<<ret.first<<"V, "<<ret.second<<"P)" )

    return ret;
}

bool IO_GenEvent::parse_units(GenEvent &evt, const char *buf) {
    const char *cursor  = buf;

    // momentum
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    ++cursor;
    Units::MomentumUnit momentum_unit = Units::momentum_unit(cursor);

    // length
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    ++cursor;
    Units::LengthUnit length_unit = Units::length_unit(cursor);

    evt.set_units(momentum_unit,length_unit);

    DEBUG( 10, "IO_GenEvent: U: " << Units::name(evt.momentum_unit()) << " " << Units::name(evt.length_unit()) )

    return true;
}

bool IO_GenEvent::parse_pdf_info(GenEvent &evt, const char *buf) {
    GenPdfInfoPtr pi = make_shared<GenPdfInfo>();
    const char *cursor = buf;

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->parton_id[0] = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->parton_id[1] = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->x[0] = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->x[1] = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->scale = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->xf[0] = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->xf[1] = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->pdf_id[0] = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    pi->pdf_id[1] = atoi(cursor);

    evt.set_pdf_info(pi);

    return true;
}

bool IO_GenEvent::parse_heavy_ion(GenEvent &evt, const char *buf) {
    GenHeavyIonPtr hi = make_shared<GenHeavyIon>();
    const char *cursor = buf;

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Ncoll_hard = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Npart_proj = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Npart_targ = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Ncoll = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->spectator_neutrons = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->spectator_protons = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->N_Nwounded_collisions = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Nwounded_N_collisions = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->Nwounded_Nwounded_collisions = atoi(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->impact_parameter = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->event_plane_angle = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->eccentricity = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    hi->sigma_inel_NN = atof(cursor);

    evt.set_heavy_ion(hi);

    return true;
}

bool IO_GenEvent::parse_cross_section(GenEvent &evt, const char *buf) {
    GenCrossSectionPtr cs = make_shared<GenCrossSection>();
    const char *cursor = buf;

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    cs->cross_section = atof(cursor);

    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    cs->cross_section_error = atof(cursor);

    evt.set_cross_section(cs);

    return true;
}

bool IO_GenEvent::parse_vertex_information(GenEvent &evt, const char *buf) {
    GenVertexPtr  data = make_shared<GenVertex>();
    FourVector    position;
    const char   *cursor          = buf;
    const char   *cursor2         = NULL;
    int           barcode         = 0;
    int           particle_in     = 0;
    int           highest_barcode = evt.particles_count();

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    barcode = atoi(cursor);

    // skip to the list of particles
    if( !(cursor = strchr(cursor+1,'[')) ) return false;

    while(true) {
        ++cursor;             // skip the '[' or ',' character
        cursor2     = cursor; // save cursor position
        particle_in = atoi(cursor);

        // add incoming particle to the vertex
        if( particle_in > 0 && particle_in <= highest_barcode) {
            data->add_particle_in( evt.particles()[particle_in-1] );
        }
        else {
            return false;
        }

        // check for next particle or end of particle list
        if( !(cursor = strchr(cursor+1,',')) ) {
            if( !(cursor = strchr(cursor2+1,']')) ) return false;
            break;
        }
    }

    // check if there is position information
    if( (cursor = strchr(cursor+1,'@')) ) {

        // x
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setX(atof(cursor));

        // y
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setY(atof(cursor));

        // z
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setZ(atof(cursor));

        // t
        if( !(cursor = strchr(cursor+1,' ')) ) return false;
        position.setT(atof(cursor));
        data->set_position( position );

    }

    DEBUG( 10, "IO_GenEvent: V: "<<barcode<<" with "<<data->particles_in().size()<<" particles)" )

    evt.add_vertex(data);

    return true;
}

bool IO_GenEvent::parse_particle_information(GenEvent &evt, const char *buf) {
    GenParticlePtr  data = make_shared<GenParticle>();
    FourVector      momentum;
    const char     *cursor  = buf;
    int             mother_barcode = 0;

    // verify barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return false;

    if( atoi(cursor) != (int)evt.particles_count() + 1 ) {
        ERROR( "IO_GenEvent: particle barcode mismatch" )
        return false;
    }

    // mother barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    mother_barcode = atoi(cursor);

    // add particle to corresponding vertex
    if( mother_barcode > 0 && mother_barcode <= (int)evt.particles_count() ) {

        GenParticlePtr mother = evt.particles()[ mother_barcode-1 ];
        GenVertexPtr   vertex = mother->end_vertex();

        // create new vertex if needed
        if( !vertex ) {
            vertex = make_shared<GenVertex>();
            vertex->add_particle_in(mother);
        }

        vertex->add_particle_out(data);
        evt.add_vertex(vertex);
    }
    else if( mother_barcode < 0 && -mother_barcode <= (int)evt.vertices_count() ) {
        evt.vertices()[ (-mother_barcode)-1 ]->add_particle_out(data);
    }

    // pdg id
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_pdg_id( atoi(cursor) );

    // px
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPx(atof(cursor));

    // py
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPy(atof(cursor));

    // pz
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setPz(atof(cursor));

    // pe
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    momentum.setE(atof(cursor));
    data->set_momentum(momentum);

    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_generated_mass( atof(cursor) );

    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return false;
    data->set_status( atoi(cursor) );

    evt.add_particle(data);

    DEBUG( 10, "IO_GenEvent: P: "<<data->barcode()<<" ( mother: "<<mother_barcode<<", pdg_id: "<<data->pdg_id()<<")" )

    return true;
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

} // namespace HepMC

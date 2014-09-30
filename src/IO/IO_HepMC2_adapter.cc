/**
 *  @file IO_HepMC2_adapter.cc
 *  @brief Implementation of \b class IO_HepMC2_adapter
 *
 */
#include "HepMC/IO/IO_HepMC2_adapter.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/Setup.h"

#include <cstring>
#include <cstdlib>

namespace HepMC {

IO_HepMC2_adapter::IO_HepMC2_adapter(const std::string& filename, std::ios::openmode mode):
IO_FileBase(filename,mode) {
    if(mode != std::ios::in) {
        ERROR( "IO_HepMC2_adapter can only accept ios::in mode" )
        m_file.clear(std::ios::badbit);
    }
}

void IO_HepMC2_adapter::write_event(const GenEvent &evt) {
    WARNING( "IO_HepMC2_adapter: write_event not implemented for HepMC2 adapter" )
}

bool IO_HepMC2_adapter::fill_next_event(GenEvent &evt) {
    char          buf[512];
    bool          parsed_event_header            = false;
    bool          is_parsing_successful          = true;
    int           parsing_result                 = 0;
    unsigned int  vertices_count                 = 0;
    unsigned int  current_vertex_particles_count = 0;
    unsigned int  current_vertex_particles_parsed= 0;

    // Empty cache
    m_vertex_cache.clear();
    m_vertex_barcodes.clear();

    m_particle_cache.clear();
    m_end_vertex_barcodes.clear();

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
                parsing_result = parse_event_information(evt,buf);
                if(parsing_result<0) {
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing event information" )
                }
                else {
                    vertices_count = parsing_result;
                    m_vertex_cache.reserve(vertices_count);
                    m_particle_cache.reserve(vertices_count*3);
                    m_vertex_barcodes.reserve(vertices_count);
                    m_end_vertex_barcodes.reserve(vertices_count*3);
                    is_parsing_successful = true;
                }
                parsed_event_header = true;
                break;
            case 'V':
                // If starting new vertex: verify if previous was fully parsed

                /** @bug HepMC2 files produced with Pythia8 are known to have wrong
                         information about number of particles in vertex. Hence '<' sign */
                if(current_vertex_particles_parsed < current_vertex_particles_count) {
                    is_parsing_successful = false;
                    break;
                }
                current_vertex_particles_parsed = 0;

                parsing_result = parse_vertex_information(buf);

                if(parsing_result<0) {
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing vertex information" )
                }
                else {
                    current_vertex_particles_count = parsing_result;
                    is_parsing_successful = true;
                }
                break;
            case 'P':

                parsing_result   = parse_particle_information(buf);

                if(parsing_result<0) {
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing particle information" )
                }
                else {
                    ++current_vertex_particles_parsed;
                    is_parsing_successful = true;
                }
                break;
            case 'U':
                is_parsing_successful = parse_units(evt,buf);
                break;
            case 'F':
                DEBUG( 10, "IO_HepMC2_adapter: F: skipping Flow" )
                is_parsing_successful = true;
                break;
            case 'H':
                DEBUG( 10, "IO_HepMC2_adapter: H: skipping Heavy Ions (for now)" )
                is_parsing_successful = true;
                break;
            default:
                WARNING( "IO_HepMC2_adapter: skipping unrecognised prefix: " << buf[0] )
                is_parsing_successful = true;
                break;
        }

        if( !is_parsing_successful ) break;

        // Check for next event
        buf[0] = m_file.peek();
        if( parsed_event_header && buf[0]=='E' ) break;
    }

    // Check if all particles in last vertex were parsed

    /** @bug HepMC2 files produced with Pythia8 are known to have wrong
             information about number of particles in vertex. Hence '<' sign */
    if( is_parsing_successful && current_vertex_particles_parsed < current_vertex_particles_count ) {
        ERROR( "IO_HepMC2_adapter: not all particles parsed" )
        is_parsing_successful = false;
    }
    // Check if all vertices were parsed
    else if( is_parsing_successful && m_vertex_cache.size() != vertices_count ) {
        ERROR( "IO_HepMC2_adapter: not all vertices parsed" )
        is_parsing_successful = false;
    }

    if( !is_parsing_successful ) {
        ERROR( "IO_HepMC2_adapter: event parsing failed. Returning empty event" )
        DEBUG( 1, "Parsing failed at line:" << std::endl << buf )
        //event->clear();
        m_file.clear(std::ios::badbit);
        return 0;
    }

    // Restore production vertex pointers
    for(unsigned int i=0; i<m_particle_cache.size(); ++i) {
        if( !m_end_vertex_barcodes[i] ) continue;

        for(unsigned int j=0; j<m_vertex_cache.size(); ++j) {
            if( m_vertex_barcodes[j] == m_end_vertex_barcodes[i] ) {
                m_vertex_cache[j]->add_particle_in(m_particle_cache[i]);
                break;
            }
        }
    }

    // Reserve memory for the event
    evt.reserve( m_particle_cache.size(), m_vertex_cache.size() );

    // Add whole event tree in topological order
    evt.add_tree( m_particle_cache );

    return 1;
}

int IO_HepMC2_adapter::parse_event_information(GenEvent &evt, const char *buf) {
    const char          *cursor             = buf;
    int                  event_no           = 0;
    int                  vertices_count     = 0;
    int                  random_states_size = 0;
    int                  weights_size       = 0;
    std::vector<long>    random_states(0);
    std::vector<double>  weights(0);

    // event number
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    event_no = atoi(cursor);
    evt.set_event_number(event_no);

    // SKIPPED: mpi
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: event scale
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: alpha_qcd
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: alpha_qed
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: signal_process_id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: signal_process_vertex
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // num_vertices
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    vertices_count = atoi(cursor);

    // SKIPPED: beam 1
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: beam 2
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: random states
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    random_states_size = atoi(cursor);
    random_states.resize(random_states_size);

    for ( int i = 0; i < random_states_size; ++i ) {
        if( !(cursor = strchr(cursor+1,' ')) ) return -1;
        random_states[i] = atoi(cursor);
    }

    // SKIPPED: weights
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    weights_size = atoi(cursor);
    weights.resize(weights_size);

    for ( int i = 0; i < weights_size; ++i ) {
        if( !(cursor = strchr(cursor+1,' ')) ) return -1;
        weights[i] = atof(cursor);
    }

    DEBUG( 10, "IO_HepMC2_adapter: E: "<<event_no<<" ("<<vertices_count<<"V, "<<weights_size<<"W, "<<random_states_size<<"RS)" )

    return vertices_count;
}

bool IO_HepMC2_adapter::parse_units(GenEvent &evt, const char *buf) {
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

    DEBUG( 10, "IO_HepMC2_adapter: U: " << Units::name(evt.momentum_unit()) << " " << Units::name(evt.length_unit()) )

    return true;
}

int IO_HepMC2_adapter::parse_vertex_information(const char *buf) {
    GenVertexPtr  data = make_shared<GenVertex>();
    FourVector    position;
    const char   *cursor            = buf;
    int           barcode           = 0;
    int           num_particles_out = 0;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    barcode = atoi(cursor);

    // SKIPPED: id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // x
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    position.setX(atof(cursor));

    // y
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    position.setY(atof(cursor));

    // z
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    position.setZ(atof(cursor));

    // t
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    position.setT(atof(cursor));
    data->set_position( position );

    // SKIPPED: num_orphans_in
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // num_particles_out
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    num_particles_out = atoi(cursor);

    // SKIPPING: weights_size, weights

    // Add original vertex barcode to the cache
    m_vertex_cache.push_back( data );
    m_vertex_barcodes.push_back( barcode );

    DEBUG( 10, "IO_HepMC2_adapter: V: "<<-(int)m_vertex_cache.size()<<" (old barcode"<<barcode<<") "<<num_particles_out<<" particles)" )

    return num_particles_out;
}

int IO_HepMC2_adapter::parse_particle_information(const char *buf) {
    GenParticlePtr  data = make_shared<GenParticle>();
    FourVector      momentum;
    const char     *cursor  = buf;
    int             end_vtx = 0;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->set_status_subcode( atoi(cursor) );

    // id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->set_pdg_id( atoi(cursor) );

    // px
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    momentum.setPx(atof(cursor));

    // py
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    momentum.setPy(atof(cursor));

    // pz
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    momentum.setPz(atof(cursor));

    // pe
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    momentum.setE(atof(cursor));
    data->set_momentum(momentum);

    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->set_generated_mass( atof(cursor) );

    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->set_status( atoi(cursor) );

    // SKIPPED: theta
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: phi
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // end_vtx_code
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    end_vtx = atoi(cursor);

    // SKIPPING: flow_size, flow patterns

    // Set prod_vtx link
    if( end_vtx == m_vertex_barcodes.back() ) {
        m_vertex_cache.back()->add_particle_in(data);
        end_vtx = 0;
    }
    else {
        m_vertex_cache.back()->add_particle_out(data);
    }

    m_particle_cache.push_back( data );
    m_end_vertex_barcodes.push_back( end_vtx );

    DEBUG( 10, "IO_HepMC2_adapter: P: "<<m_particle_cache.size()<<" ( old barcode: "<<data->status_subcode()<<", pdg_id: "<<data->pdg_id()<<") end vertex: "<<end_vtx )

    return 0;
}

} // namespace HepMC

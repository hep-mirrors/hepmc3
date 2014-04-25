/**
 *  @file IO_HepMC2_adapter.cc
 *  @brief Implementation of \b class HepMC3::IO_HepMC2_adapter
 *
 */
#include "HepMC3/IO/IO_HepMC2_adapter.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Setup.h"

#include <cstring>
#include <cstdlib>
#include <deque>

#include <boost/foreach.hpp>

namespace HepMC3 {

IO_HepMC2_adapter::IO_HepMC2_adapter(const std::string& filename, std::ios::openmode mode):
IO_Base(filename,mode) {
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

    // Empty cache after last event
    empty_cache();

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
                DEBUG( 10, "IO_HepMC2_adapter: U: skipping unit info (for now)" )
                is_parsing_successful = true;
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
        DEBUG( 1, "Parsed failed at line:" << std::endl << buf )
        //event->clear();
        m_file.clear(std::ios::badbit);
        return 0;
    }

    // Restore production vertex pointers
    for(unsigned int i=0; i<m_particle_cache.size(); ++i) {
        if(!m_particle_cache[i]->end_vertex) continue;

        int index = -1;
        for(unsigned int j=0; j<m_vertex_cache.size(); ++j) {
            if(m_vertex_cache[j].first == m_particle_cache[i]->end_vertex) {
                index = j;
                break;
            }
        }
        if( index >= 0 ) {
            m_links_cache[index].first.push_back( i+1 );
            m_particle_cache[i]->end_vertex = index + 1;
        }
    }

    //
    // Add particles to the event in topological order
    //
    std::deque<int>           sorting;
    std::vector<GenParticle*> new_particles( m_particle_cache.size() );

    // Find all starting particles (particles that have no production vertex)
    for( unsigned int i=0; i<m_particle_cache.size(); ++i) {
        const GenParticleData *p = m_particle_cache[i];

        if( !p->production_vertex || m_links_cache[p->production_vertex-1].first.size() == 0 ) {
            sorting.push_back(i);
        }
    }

    DEBUG_CODE_BLOCK(
        unsigned int sorting_loop_count = 0;
        unsigned int max_deque_size     = 0;
    )

    while( !sorting.empty() ) {
        DEBUG_CODE_BLOCK(
            if( sorting.size() > max_deque_size ) max_deque_size = sorting.size();
            ++sorting_loop_count;
        )

        int              i    = sorting.front();
        GenParticleData *data = m_particle_cache[i];

        bool added = false;

        // Add all mothers to the front of the list
        if( data->production_vertex ) {
            BOOST_FOREACH( int j, m_links_cache[data->production_vertex-1].first ) {
                if( new_particles[j-1] ) continue;
                sorting.push_front(j-1);
                added = true;
            }
        }

        // If we have added at least one mother, our particle is not the first particle on the list
        if( added ) continue;

        // Remove this particle from the list to be sorted
        sorting.pop_front();

        // Skip further processing if this particle was already added
        if( new_particles[i] ) continue;

        // Add new particle to the event
        // NOTE: particle data will be copied to the event container
        new_particles[i] = &evt.new_particle(data);

        GenVertex *prod_vtx = NULL;

        // Check if production vertex for this particle already exist
        if( data->production_vertex ) {
            if( m_links_cache[data->production_vertex-1].first.size() ) {
                prod_vtx = new_particles[ m_links_cache[data->production_vertex-1].first[0]-1 ]->end_vertex();

                // If not - create one
                if(!prod_vtx) {
                    // Add new vertex to the event
                    // NOTE: vertex data will be copied to the event container
                    prod_vtx = &evt.new_vertex( m_vertex_cache[data->production_vertex-1].second );

                    // Add all incoming particles
                    BOOST_FOREACH( int j, m_links_cache[data->production_vertex-1].first ) {
                        // NOTE: if sorting algorithm works correctly, all
                        //       mothers have been added before this particle
                        prod_vtx->add_particle_in( *new_particles[j-1] );
                    }
                }
            }
        }

        if( prod_vtx ) prod_vtx->add_particle_out( *new_particles[i] );

        // Add all daughters to the end of the list
        if( data->end_vertex ) {
            BOOST_FOREACH( int j, m_links_cache[data->end_vertex-1].second ) {
                if( new_particles[j-1] ) continue;
                sorting.push_back(j-1);
            }
        }
    }

    DEBUG_CODE_BLOCK(
        DEBUG( 6, "IO_HepMC2_adapter - particles sorted: "
                   <<evt.particles_count()<<", max deque size: "
                   <<max_deque_size<<", iterations: "<<sorting_loop_count )
    )

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

int IO_HepMC2_adapter::parse_vertex_information(const char *buf) {
    GenVertexData *data = new GenVertexData();
    const char    *cursor            = buf;
    int            barcode           = 0;
    int            num_particles_out = 0;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    barcode = atoi(cursor);

    // SKIPPED: id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // x
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->position.setX(atof(cursor));

    // y
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->position.setY(atof(cursor));

    // z
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->position.setZ(atof(cursor));

    // t
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->position.setT(atof(cursor));

    // SKIPPED: num_orphans_in
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // num_particles_out
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    num_particles_out = atoi(cursor);

    // SKIPPING: weights_size, weights

    DEBUG( 10, "IO_HepMC2_adapter: V: "<<barcode<<", "<<num_particles_out<<" particles)" )

    // Add original vertex barcode to the cache
    m_vertex_cache.push_back( std::pair<int,GenVertexData*>(barcode,data) );
    m_links_cache.push_back( std::pair< vector<int>,vector<int> >() );

    return num_particles_out;
}

int IO_HepMC2_adapter::parse_particle_information(const char *buf) {
    GenParticleData *data = new GenParticleData();
    const char *cursor             = buf;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->status_subcode = atoi(cursor);

    // id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->pdg_id = atoi(cursor);

    // px
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->momentum.setPx(atof(cursor));

    // py
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->momentum.setPy(atof(cursor));

    // pz
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->momentum.setPz(atof(cursor));

    // pe
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->momentum.setE(atof(cursor));

    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->mass = atof(cursor);
    if(data->mass) data->is_mass_set = true;

    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->status = atoi(cursor);

    // SKIPPED: theta
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: phi
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // end_vtx_code
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    data->end_vertex = atoi(cursor);

    // SKIPPING: flow_size, flow patterns

    m_particle_cache.push_back(data);

    // Set links
    if( data->end_vertex == m_vertex_cache.back().first ) {
        m_links_cache.back().first.push_back( m_particle_cache.size() );
        data->production_vertex = 0;
    }
    else {
        m_links_cache.back().second.push_back( m_particle_cache.size() );
        data->production_vertex = m_vertex_cache.size();
    }

    DEBUG( 10, "IO_HepMC2_adapter: P: "<<m_particle_cache.size()<<" ( old barcode: "<<data->status_subcode<<", pdg_id: "<<data->pdg_id<<") in vertex: "<<data->end_vertex )

    return 0;
}

void IO_HepMC2_adapter::empty_cache() {
    for( unsigned int i=0;i< m_vertex_cache.size(); ++i)   delete m_vertex_cache[i].second;
    for( unsigned int i=0;i< m_particle_cache.size(); ++i) delete m_particle_cache[i];

    m_links_cache.clear();
    m_vertex_cache.clear();
    m_particle_cache.clear();
}

} // namespace HepMC3

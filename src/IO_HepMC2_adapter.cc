/**
 *  @file IO_HepMC2_adapter.cc
 *  @brief Implementation of \b class HepMC3::IO_HepMC2_adapter
 *
 *  @date Created       <b> 23th March 2014 </b>
 *  @date Last modified <b> 28th March 2014 </b>
 */
#include <cstring>
#include <cstdlib>
#include <algorithm>    // std::sort
#include "HepMC3/IO_HepMC2_adapter.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

IO_HepMC2_adapter::IO_HepMC2_adapter(const std::string& filename, std::ios::openmode mode):
IO_Base(filename,mode) {
    if(mode != std::ios::in) {
        ERROR( "IO_HepMC2_adapter can only accept ios::in mode" )
        m_file.clear(std::ios::badbit);
    }
}

void IO_HepMC2_adapter::write_event(const GenEvent *evt) {
    WARNING( "IO_HepMC2_adapter: write_event not implemented for HepMC2 adapter" )
}

bool IO_HepMC2_adapter::fill_next_event(GenEvent *evt) {
    char          buf[512];
    bool          parsed_event_header            = false;
    bool          is_parsing_successful          = false;
    int           parsing_result                 = 0;
    unsigned int  vertices_count                 = 0;
    unsigned int  current_vertex_particles_count = 0;
    GenVertex    *current_vertex                 = NULL;
    GenParticle  *current_particle               = NULL;

    m_vertex_barcode_cache.clear();
    m_particle_cache.clear();
    m_vertex_cache.clear();

    //
    // Parse event, vertex and particle information
    //
    while(!m_file.rdstate()) {
        m_file.getline(buf,512);

        if( strlen(buf) == 0 ) continue;

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
                if(current_vertex && current_vertex->particles_out().size() < current_vertex_particles_count) {
                    is_parsing_successful = false;
                    break;
                }

                current_vertex = new GenVertex();

                parsing_result = parse_vertex_information(current_vertex,buf);
                if(parsing_result<0) {
                    delete current_vertex;
                    current_vertex = NULL;
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing vertex information" )
                }
                else {
                    m_vertex_cache.push_back(current_vertex);
                    current_vertex_particles_count = parsing_result;
                    is_parsing_successful = true;
                }
                break;
            case 'P':
                current_particle = new GenParticle();
                parsing_result = parse_particle_information(current_particle,buf);
                if(parsing_result<0) {
                    delete current_particle;
                    current_particle = NULL;
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing particle information" )
                }
                else {
                    m_particle_cache.push_back(current_particle);
                    if(current_vertex) current_vertex->add_particle_out(current_particle);
                    is_parsing_successful = true;
                }
                break;
            case 'U':
                DEBUG( 10, "IO_HepMC2_adapter: U: skipping unit info (for now)" )
                is_parsing_successful = true;
                break;
            case 'H':
                DEBUG( 10, "IO_HepMC2_adapter: H: skipping header info (for now)" )
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
    if( is_parsing_successful && current_vertex &&
        current_vertex->particles_out().size() < current_vertex_particles_count ) {
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

    //
    // Rebuild incoming particle information | max O(V*P)
    //
    for( vector<GenParticle*>::const_iterator i = m_particle_cache.begin(); i != m_particle_cache.end(); ++i ) {
        int barcode = (*i)->end_vertex_barcode();
        if( barcode == 0 ) continue;

        for( unsigned int j=0; j<m_vertex_barcode_cache.size(); ++j ) {
            if( m_vertex_barcode_cache[j] == barcode ) {
                m_vertex_cache[j]->add_particle_in(*i);
                barcode = 0;
                break;
            }
        }
        if( barcode != 0 ) {
            ERROR( "IO_HepMC2_adapter: problem rebuilding tree for particle with barcode: "<<(*i)->barcode() )
            //event->clear();
            m_file.clear(std::ios::badbit);
            return 0;
        }
    }

    //
    // Sort
    //
    GenVertex::topological_compare c;
    std::sort( m_vertex_cache.begin(), m_vertex_cache.end(), c );

    //
    // Removing unnecessary vertices and adding the rest to the event | max O(V)
    //
    for( vector<GenVertex*>::const_iterator i = m_vertex_cache.begin(); i != m_vertex_cache.end(); ++i ) {
        // No incoming particles
        if( (*i)->particles_in().size()==0) {
            for( vector<GenParticle*>::const_iterator j = (*i)->particles_out().begin(); j != (*i)->particles_out().end(); ++j ) {
                (*j)->set_production_vertex_barcode(0);

                // Add outgoing particles to the event
                evt->add_particle(*j);
            }

            delete (*i);
        }
        // Has one incoming particle
        else if( (*i)->particles_in().size()==1) {
            GenParticle *p = (*i)->particles_in()[0];
            p->set_end_vertex_barcode(0);
            int barcode = p->barcode();

            // Add ancestors that are not in the event first
            if( barcode==0 ) {
                evt->add_particle(p);
                barcode = p->barcode();
            }

            for( vector<GenParticle*>::const_iterator j = (*i)->particles_out().begin(); j != (*i)->particles_out().end(); ++j ) {
                (*j)->set_production_vertex_barcode(barcode);

                // In some cases beam particle is written with production vertex
                // same as end vertex
                if( (*j)->production_vertex_barcode()==(*j)->barcode() ) {
                    (*j)->set_production_vertex_barcode(0);
                }

                // Add outgoing particles to the event
                evt->add_particle(*j);
            }

            delete *i;
        }
        // Has more than one incoming particle - add to the event
        else evt->add_vertex(*i);
    }

    return 1;
}

int IO_HepMC2_adapter::parse_event_information(GenEvent *evt, const char *buf) {
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
    evt->set_event_number(event_no);

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

int IO_HepMC2_adapter::parse_vertex_information(GenVertex *v, const char *buf) {
    const char *cursor            = buf;
    int         barcode           = 0;
    int         num_particles_out = 0;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    barcode = atoi(cursor);
    m_vertex_barcode_cache.push_back(barcode);

    // SKIPPED: id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: x
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: y
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: z
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: t
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: num_orphans_in
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // num_particles_out
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    num_particles_out = atoi(cursor);

    // SKIPPING: weights_size, weights

    DEBUG( 10, "IO_HepMC2_adapter: V: "<<v->barcode()<<" (old barcode: "<<barcode<<","<<num_particles_out<<"P)" )

    return num_particles_out;
}

int IO_HepMC2_adapter::parse_particle_information(GenParticle *p, const char *buf) {
    const char *cursor             = buf;
    int         barcode            = 0;
    int         pdg_id             = 0;
    int         end_vertex_barcode = 0;
    FourVector  momentum;

    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    barcode = atoi(cursor);
    p->set_barcode(barcode);
    p->set_status_subcode(barcode);

    // id
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    pdg_id = atoi(cursor);
    p->set_pdg_id(pdg_id);

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
    p->set_momentum(momentum);

    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    p->set_generated_mass(atof(cursor));

    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    p->set_status(atoi(cursor));

    // SKIPPED: theta
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // SKIPPED: phi
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // end_vtx_code
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    end_vertex_barcode = atoi(cursor);
    p->set_end_vertex_barcode(end_vertex_barcode);

    // SKIPPING: flow_size, flow patterns

    DEBUG( 10, "IO_HepMC2_adapter: P: "<<p->barcode()<<" (old barcode: "<<barcode<<", pdg_id: "<<pdg_id<<") in vertex: "<<end_vertex_barcode )

    return 0;
}

} // namespace HepMC3

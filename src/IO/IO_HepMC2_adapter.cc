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
    GenVertex    *current_vertex                 = NULL;
    GenParticle  *current_particle               = NULL;

    m_vertex_barcode_cache.clear();
    m_end_vertex_barcode_cache.clear();

    // Temporary event for storing particles and vertices
    GenEvent temp_event;

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
                if(current_vertex && (current_vertex->particles_out().size() + current_vertex->particles_in().size()) < current_vertex_particles_count) {
                    is_parsing_successful = false;
                    break;
                }

                current_vertex = &temp_event.new_vertex();

                parsing_result = parse_vertex_information(current_vertex,buf);
                if(parsing_result<0) {
                    current_vertex = NULL;
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing vertex information" )
                }
                else {
                    current_vertex_particles_count = parsing_result;
                    is_parsing_successful = true;
                }
                break;
            case 'P':
                current_particle = &temp_event.new_particle();
                parsing_result = parse_particle_information(current_particle,buf);
                if(parsing_result<0) {
                    current_particle = NULL;
                    is_parsing_successful = false;
                    ERROR( "IO_HepMC2_adapter: error parsing particle information" )
                }
                else {
                    if(current_vertex) {
                        // If particle belongs to this vertex
                        if( m_end_vertex_barcode_cache.back().second == m_vertex_barcode_cache.back().second ) {
                            current_vertex->add_particle_in(*current_particle);
                            m_end_vertex_barcode_cache.back().second = 0;
                        }
                        else current_vertex->add_particle_out(*current_particle);
                    }
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
    if( is_parsing_successful && current_vertex &&
        current_vertex->particles_out().size() < current_vertex_particles_count ) {
        ERROR( "IO_HepMC2_adapter: not all particles parsed" )
        is_parsing_successful = false;
    }
    // Check if all vertices were parsed
    else if( is_parsing_successful && m_vertex_barcode_cache.size() != vertices_count ) {
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
    typedef std::pair<GenParticle*,int> ___particle_int_pair___;
    typedef std::pair<GenVertex*,int>   ___vertex_int_pair___;

    BOOST_FOREACH( ___particle_int_pair___ &p, m_end_vertex_barcode_cache ) {

        if(p.second==0) continue;

        BOOST_FOREACH( ___vertex_int_pair___ &v, m_vertex_barcode_cache ) {

        //DEBUG( 1, "Pair: "<<p.second<<" "<<v.second<<" "<<p.first->barcode()<<" "<<v.first->barcode() )
            if( p.second == v.second ) {
                v.first->add_particle_in(*p.first);
                break;
            }
        }
    }

    //
    // Add particles to the event in topological order
    //

    /** @todo Optimize sorting.
     *
     *  Currently the intermediate step is copying from temp_event to evt,
     *  which costs twice as much time and memory than needed.
     */
    std::deque<const GenVertex*> sorting;
    std::vector<int>             new_barcodes(temp_event.particles_count());

    DEBUG_CODE_BLOCK(
        unsigned int sorting_loop_count = 0;
        unsigned int max_deque_size     = 0;
    )

    // Find all starting particles (particles that have no production vertex)
    BOOST_FOREACH( ___particle_int_pair___ &p, m_end_vertex_barcode_cache ) {
        const GenVertex *v = p.first->production_vertex();
        if( !v || v->particles_in().size() == 0 ) {
            if( p.first->end_vertex() ) sorting.push_back(p.first->end_vertex());
        }
    }

    // Topological sort
    while( !sorting.empty() ) {

        DEBUG_CODE_BLOCK(
            if( sorting.size() > max_deque_size ) max_deque_size = sorting.size();
            ++sorting_loop_count;
        )

        const GenVertex *v = sorting.front();

        bool added = false;

        // Add all production vertices to the front of the list
        BOOST_FOREACH( GenParticle *p, v->particles_in() ) {
            if( p->production_vertex() && !p->production_vertex()->is_deleted() ) {
                sorting.push_front( p->production_vertex() );
                added = true;
            }
        }

        // If at least one new vertex was added then our vertex
        // is not the first on the list
        if( added ) continue;

        // Remove this vertex from the list to be sorted
        sorting.pop_front();

        // If this vertex has not yet been added by any particle,
        // add this vertex and all incoming/outgoing particles to the event
        if( !v->is_deleted() ) {

            GenVertex &new_v = evt.new_vertex(&v->data());

            BOOST_FOREACH( const GenParticle *p, v->particles_in() ) {
                if(p->is_deleted()) {
                    // Use new barcode list to add incoming particle
                    new_v.add_particle_in( evt.particle( new_barcodes[ p->barcode()-1 ] ) );
                }
                else {
                    // Create new particle and add it as incoming particle
                    GenParticle &new_p = evt.new_particle(&p->data());

                    // Store new barcode
                    new_barcodes[ p->barcode()-1 ] = new_p.barcode();

                    new_v.add_particle_in(new_p);

                    temp_event.particle( p->barcode() ).mark_deleted();
                }
            }

            BOOST_FOREACH( const GenParticle *p, v->particles_out() ) {
                if(p->is_deleted()) {
                    // Use new barcode list to add outgoing particle
                    new_v.add_particle_out( evt.particle( new_barcodes[ p->barcode()-1 ] ) );
                }
                else {
                    // Create new particle and add it as outgoing particle
                    GenParticle &new_p = evt.new_particle(&p->data());

                    // Store new barcode
                    new_barcodes[ p->barcode()-1 ] = new_p.barcode();

                    new_v.add_particle_out(new_p);

                    temp_event.particle( p->barcode() ).mark_deleted();
                }
            }

            temp_event.vertex( v->barcode() ).mark_deleted();
        }

        // Add all outgoing vertices to the end list
        BOOST_FOREACH( GenParticle *p, v->particles_out() ) {
            if( p->end_vertex() && !p->end_vertex()->is_deleted() ) {
                sorting.push_back( p->end_vertex() );
            }
        }
    }

    DEBUG_CODE_BLOCK(
        DEBUG( 6, "IO_HepMC2_adapter - vertices sorted: "
                   <<evt.vertices_count()<<", max deque size: "
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

int IO_HepMC2_adapter::parse_vertex_information(GenVertex *v, const char *buf) {
    const char *cursor            = buf;
    int         barcode           = 0;
    int         num_particles_out = 0;
    FourVector  position;

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
    v->set_position(position);

    // SKIPPED: num_orphans_in
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;

    // num_particles_out
    if( !(cursor = strchr(cursor+1,' ')) ) return -1;
    num_particles_out = atoi(cursor);

    // SKIPPING: weights_size, weights

    DEBUG( 10, "IO_HepMC2_adapter: V: "<<v->barcode()<<" (old barcode: "<<barcode<<","<<num_particles_out<<"P)" )

    // Add original vertex barcode to the cache
    m_vertex_barcode_cache.push_back( std::pair<GenVertex*,int>(v,barcode) );

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

    // SKIPPING: flow_size, flow patterns

    DEBUG( 10, "IO_HepMC2_adapter: P: "<<p->barcode()<<" (old barcode: "<<barcode<<", pdg_id: "<<pdg_id<<") in vertex: "<<end_vertex_barcode )

    m_end_vertex_barcode_cache.push_back( std::pair<GenParticle*,int>(p,end_vertex_barcode) );

    return 0;
}

} // namespace HepMC3

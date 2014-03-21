#include <cstring>
#include <cstdlib>
#include "HepMC3/IO_HepMC2_adapter.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
using std::cout;
using std::endl;

namespace HepMC3 {

void IO_HepMC2_adapter::write_event(const GenEvent *evt) {
    cout<<"adapter err1"<<endl;
}

bool IO_HepMC2_adapter::fill_next_event(GenEvent *evt) {
    char          buf[256];
    bool          is_parsing_successful          = false;
    bool          parsed_event_header            = false;
    unsigned int  vertices_count                 = 0;
    unsigned int  current_vertex_particles_count = 0;
    GenVertex    *current_vertex                 = NULL;
    GenParticle  *current_particle               = NULL;
    
    //
    // Parse event, vertex and particle information
    //
    while(!m_file.rdstate()) {        
        m_file.getline(buf,256);
        
        if( strlen(buf) == 0 ) continue;

        switch(buf[0]) {
            case 'E':
                vertices_count         = parse_event_information(evt,buf);
                is_parsing_successful  = (bool)vertices_count;
                parsed_event_header    = true;
                break;
            case 'V':
                // If starting new vertex: verify if previous was fully parsed
                if(current_vertex && current_vertex->particles_out().size()!=current_vertex_particles_count) break;
                
                current_vertex = new GenVertex();
                evt->add_vertex(current_vertex);
                current_vertex_particles_count = parse_vertex_information(current_vertex,buf);
                is_parsing_successful   = (bool)current_vertex_particles_count;
                break;
            case 'P':
                current_particle = new GenParticle();
                evt->add_particle(current_particle); // :TODO: this line shouldn't be needed!!
                current_vertex->add_particle_out(current_particle);
                is_parsing_successful = parse_particle_information(current_particle,buf);
                break;
            case 'U':
                //cout<<"U: units ignored (for now)"<<endl;
                is_parsing_successful = true;
                break;
            case 'H':
                //cout<<"H: header/footer lines ignored (for now)"<<endl;
                is_parsing_successful = true;
                break;
            default:
                cout<<"WARNING! Unrecognised prefix: "<<buf[0]<<endl;
                is_parsing_successful = false;
                break;
        }
        
        if( !is_parsing_successful ) break;

        // Check for next event
        buf[0] = m_file.peek();
        if( parsed_event_header && buf[0]=='E' ) break;
    }
    
    // Check if all vertices and all particles in last vertex were parsed
    if( current_vertex &&
        current_vertex->particles_out().size() != current_vertex_particles_count ) {
        cout<<"Vertex: not all particles parsed"<<endl;
        is_parsing_successful = false;
    }
    else if( evt->vertices().size() != vertices_count ) {
        cout<<"Event: not all vertices parsed"<<endl;
        is_parsing_successful = false;
    }
    
    if( !is_parsing_successful ) {
        cout<<"Clearing event..."<<endl;
        //event->clear();
        m_file.clear(std::ios::badbit);
        return 0;
    }

    //
    // Rebuild tree structure | max O(V*P)
    //
    for( unsigned int i=0; i<evt->particles().size(); ++i ) {
        int buf = evt->particles()[i]->end_vertex_barcode();
        if( buf == 0 ) continue;
        
        for( unsigned int j=0; j<evt->vertices().size(); ++j ) {
            if( evt->vertices()[j]->barcode() == buf ) {
                evt->vertices()[j]->add_particle_in(evt->particles()[i]);
                buf = 0;
                break;
            }
        }
        if( buf != 0 ) {
            cout<<"Error: problem rebuilding tree"<<i<<endl;
            cout<<"Clearing event..."<<endl;
            //event->clear();
            m_file.clear(std::ios::badbit);
            return 0;
        }
    }
    
    return 1;
}

unsigned int IO_HepMC2_adapter::parse_event_information(GenEvent *evt, const char *buf) {
    const char          *cursor             = buf;
    int                  event_no           = 0;
    int                  vertices_count     = 0;
    int                  random_states_size = 0;
    int                  weights_size       = 0;
    std::vector<long>    random_states(0);
    std::vector<double>  weights(0);
    
    // event number
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    event_no = atoi(cursor);
    evt->set_event_number(event_no);

    // SKIPPED: mpi
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: event scale
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: alpha_qcd
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: alpha_qed
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: signal_process_id
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: signal_process_vertex
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // num_vertices
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    vertices_count = atoi(cursor);

    // SKIPPED: beam 1
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: beam 2
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;

    // SKIPPED: random states
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    random_states_size = atoi(cursor);
    random_states.resize(random_states_size);

    for ( int i = 0; i < random_states_size; ++i ) {
        if( !(cursor = strchr(cursor+1,' ')) ) return 0;
        random_states[i] = atoi(cursor);
    }

    // SKIPPED: weights
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    weights_size = atoi(cursor);
    weights.resize(weights_size);

    for ( int i = 0; i < weights_size; ++i ) {
        if( !(cursor = strchr(cursor+1,' ')) ) return 0;
        weights[i] = atof(cursor);
    }

    //cout<<"E: "<<event_no<<" ("<<vertices_count<<"V, "<<weights_size<<"W, "<<random_states_size<<"RS)"<<endl;
    
    return vertices_count;
}

unsigned int IO_HepMC2_adapter::parse_vertex_information(GenVertex *v, const char *buf) {
    const char *cursor            = buf;
    int         barcode           = 0;
    int         num_particles_out = 0;
    
    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    barcode = atoi(cursor);
    v->set_barcode(barcode);
    
    // SKIPPED: id
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: x
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: y
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: z
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: t
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: num_orphans_in
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // num_particles_out
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    num_particles_out = atoi(cursor);
        
    // SKIPPING: weights_size, weights
    //cout<<"V: "<<barcode<<" ("<<num_particles_out<<"P)"<<endl;
    return num_particles_out;
}

bool IO_HepMC2_adapter::parse_particle_information(GenParticle *p, const char *buf) {
    const char *cursor             = buf;
    int         barcode            = 0;
    int         pdg_id             = 0;
    int         end_vertex_barcode = 0;
    FourVector  momentum;
    
    // barcode
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    barcode = atoi(cursor);
    p->set_barcode(barcode);
    
    // id
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    pdg_id = atoi(cursor);
    p->set_pdg_id(pdg_id);
    
    // px
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    momentum.setPx(atof(cursor));
    
    // py
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    momentum.setPy(atof(cursor));
    
    // pz
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    momentum.setPz(atof(cursor));
    
    // pe
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    momentum.setE(atof(cursor));
    
    // m
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    p->set_generated_mass(atof(cursor));
    
    // status
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    p->set_status(atoi(cursor));
    
    // SKIPPED: theta
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // SKIPPED: phi
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    
    // end_vtx_code
    if( !(cursor = strchr(cursor+1,' ')) ) return 0;
    end_vertex_barcode = atoi(cursor);
    p->set_end_vertex_barcode(end_vertex_barcode);
    
    // SKIPPING: flow_size, flow patterns

    //cout<<"P: "<<barcode<<" (pdg_id: "<<pdg_id<<") in vertex: "<<end_vertex_barcode<<endl;
    p->set_momentum(momentum);
    
    return 1;
}

} // namespace HepMC3

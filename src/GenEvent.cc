#include "HepMC3/GenEvent.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent():
m_event_number(0),
m_lowest_barcode(0),
m_lowest_vertex(0) {
}

GenEvent::~GenEvent() {

}

void GenEvent::add_particle(GenParticle *p) {
    p->set_barcode(++m_lowest_barcode);
    m_particles.push_back(p);
}

void GenEvent::add_vertex(GenVertex *v) {
    v->set_barcode(++m_lowest_vertex);
    m_vertices.push_back(v);
}

void GenEvent::print( ostream& ostr, int format ) const {

    if( format == 0 ) {
        ostr << "GenEvent: " << m_event_number
             << " V: " << m_vertices.size()
             << " P: " << m_particles.size() << endl;
    }
    else if( format == 1 ) {
        ostr << "________________________________________________________________________________" << endl;
        ostr << "GenEvent: #" << m_event_number << endl;
        ostr << " Entries this event: " << m_vertices.size() << " vertices, "
           << m_particles.size() << " particles.\n";

        // Print a legend to describe the particle info
        ostr << "                                    GenParticle Legend" << endl;
        ostr << "        Barcode   PDG ID      "
           << "( Px,       Py,       Pz,     E )"
           << " Stat  Prod V|P" << endl;
        ostr << "________________________________________________________________________________" << endl;

        // Print all particles and vertices in the event
        // NOTE: assumption is made that barcodes of vertices and particles
        //       are in topological order!
        int highest_vertex_already_printed = 0;
        for( unsigned int i=0; i<m_particles.size(); ++i) {
            int production_vertex = m_particles[i]->production_vertex_barcode();
            if( production_vertex < highest_vertex_already_printed ) {
                highest_vertex_already_printed = production_vertex;
                for( unsigned int j=0; j<m_vertices.size(); ++j ) {
                    if( m_vertices[j]->barcode() == production_vertex ) {
                        m_vertices[j]->print(ostr,1);
                        break;
                    }
                }
            }
            ostr<<"    ";
            m_particles[i]->print(ostr,1);
        }

        ostr << "________________________________________________________________________________" << endl;
    }
    else if( format == 2 ) {
        ostr << "E " << m_event_number << " " << m_vertices.size() << " " << m_particles.size() << endl;

        // Print all particles and vertices in the event
        // NOTE: assumption is made that barcodes of vertices and particles
        //       are in topological order!
        int highest_vertex_already_printed = 0;
        for( unsigned int i=0; i<m_particles.size(); ++i) {
            int production_vertex = m_particles[i]->production_vertex_barcode();
            if( production_vertex < highest_vertex_already_printed ) {
                highest_vertex_already_printed = production_vertex;
                for( unsigned int j=0; j<m_vertices.size(); ++j ) {
                    if( m_vertices[j]->barcode() == production_vertex ) {
                        m_vertices[j]->print(ostr,2);
                        break;
                    }
                }
            }
            m_particles[i]->print(ostr,2);
        }
    }
    else if( format == 3 ) {
        ostr << "GenEvent: ROOT streamer format not ready" << endl;
    }
}

} // namespace HepMC3

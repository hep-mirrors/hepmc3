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
    for( unsigned int i=0; i<m_particles.size(); ++i) {
        delete m_particles[i];
    }
    for( unsigned int i=0; i<m_vertices.size(); ++i) {
        delete m_vertices[i];
    }
}

void GenEvent::add_particle(GenParticle *p) {
    p->set_barcode(++m_lowest_barcode);
    m_particles.push_back(p);
}

void GenEvent::add_vertex(GenVertex *v) {
    v->set_barcode(++m_lowest_vertex);
    m_vertices.push_back(v);
}

void GenEvent::print( ostream& ostr) const {

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
                    m_vertices[j]->print(ostr,true);
                    break;
                }
            }
        }
        ostr<<"    ";
        m_particles[i]->print(ostr,true);
    }

    ostr << "________________________________________________________________________________" << endl;
}

} // namespace HepMC3

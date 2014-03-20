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

void GenEvent::print( ostream& ostr ) const {

  ostr << "________________________________________________________________________________" << endl;
  ostr << "GenEvent: #" << event_number() << endl;
  ostr << " Entries this event: " << m_vertices.size() << " vertices, "
       << m_particles.size() << " particles.\n";

  // Print a legend to describe the particle info
  ostr << "                                    GenParticle Legend" << endl;
   ostr << "        Barcode   PDG ID      "
       << "( Px,       Py,       Pz,     E )"
       << " Stat  DecayVtx" << endl;
  ostr << "________________________________________________________________________________" << endl;

  // Print all Vertices
  for(unsigned int i=0;i<m_vertices.size();++i) {
    m_vertices[i]->print();
  }

  ostr << "________________________________________________________________________________" << endl;
}

} // namespace HepMC3

#include "HepMC3/GenVertex.h"
using std::cout;
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex():
  m_barcode(0) {
}

GenVertex::~GenVertex() {}

void GenVertex::print( std::ostream& ostr ) const {
  // find the current stream state
  std::ios_base::fmtflags orig = ostr.flags();
   std::streamsize prec = ostr.precision();
  ostr << "GenVertex:";
  ostr.width(9);
  ostr << barcode();
  ostr << " (X,cT):0";
  ostr << endl;

  // print out all the incoming, then outgoing particles
  for(unsigned int i=0;i<m_particles_in.size();++i) {
    if(i==0) {
      ostr << " I ";
      ostr.width(2);
      ostr << m_particles_in.size()<<": ";
    }
    else ostr << "       ";
    m_particles_in[i]->print(1,ostr);
  }

  for(unsigned int i=0;i<m_particles_out.size();++i) {
    if(i==0) {
      ostr << " O ";
      ostr.width(2);
      ostr << m_particles_out.size()<<": ";
    }
    else ostr << "       ";
    m_particles_out[i]->print(1,ostr);
  }
  // restore the stream state
  ostr.flags(orig);
  ostr.precision(prec);
}

void GenVertex::add_particle_in(GenParticle *p) {
  p->set_production_vertex_barcode(m_barcode);
  m_particles_in.push_back(p);
}

void GenVertex::add_particle_out(GenParticle *p) {
  p->set_end_vertex_barcode(m_barcode);
  m_particles_out.push_back(p);
}

} // namespace HepMC3

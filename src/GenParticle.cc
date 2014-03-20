#include <iostream>
#include "HepMC3/GenParticle.h"
using std::cout;
using std::endl;
using std::ostream;

namespace HepMC3 {

GenParticle::GenParticle():
  m_momentum(0.0,0.0,0.0,0.0),
  m_pdgid(0),
  m_status(0),
  m_barcode(0),
  m_production_vertex(0),
  m_end_vertex(0),
  m_generated_mass(0.0) {

}

GenParticle::GenParticle(FourVector momentum, int pdgid, int status):
  m_momentum(momentum),
  m_pdgid(pdgid),
  m_status(status),
  m_barcode(0),
  m_production_vertex(0),
  m_end_vertex(0),
  m_generated_mass(0.0) {
}

GenParticle::~GenParticle() {}

void GenParticle::print(int format, ostream& ostr ) const {
  if(format==0) {
    ostr << "GenParticle: "
         << barcode() << " ID:" << pdg_id()
         << " (P,E)=" << momentum().px() << "," << momentum().py()
         << "," << momentum().pz() << "," << momentum().e()
         << " Stat:" << status();
    ostr << " PV:" << production_vertex_barcode();
    ostr << " EV:" << end_vertex_barcode() << endl;
  }
  else if(format==1) {
    // find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize prec = ostr.precision();
    ostr << " ";
    ostr.width(9);
    ostr << barcode();
    ostr.width(9);
    ostr << pdg_id() << " ";
    ostr.width(9);
    ostr.precision(2);
    ostr.setf(std::ios::scientific, std::ios::floatfield);
    ostr.setf(std::ios_base::showpos);
    ostr << momentum().px() << ",";
    ostr.width(9);
    ostr << momentum().py() << ",";
    ostr.width(9);
    ostr << momentum().pz() << ",";
    ostr.width(9);
    ostr << momentum().e() << " ";
    ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
    ostr.unsetf(std::ios_base::showpos);
    if ( end_vertex_barcode()) {
      ostr.width(3);
      ostr << status() << " ";
      ostr.width(9);
      ostr << end_vertex_barcode();
    } else if ( !end_vertex_barcode() ) {
      // There is no valid end_vertex
      // For consistency across different compilers, do not print anything
      ostr.width(3);
      ostr << status();
    }
    ostr << endl;
    // restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
  }
  else if(format==2) {
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize prec = ostr.precision();

    ostr<<barcode()<<" "<<pdg_id()<<" ";
    ostr.setf(std::ios::scientific, std::ios::floatfield);
    ostr.precision(16);
    ostr<<momentum().px()<<" "<<momentum().py()<<" "<<momentum().pz()<<" "<<momentum().e()<<" ";
    ostr<<generated_mass()<<" ";
    ostr<<production_vertex_barcode()<<" "<<end_vertex_barcode()<<" ";
    ostr<<status()<<endl;
    ostr.flags(orig);
    ostr.precision(prec);
  }
}

} // namespace HepMC3

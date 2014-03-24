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

void GenParticle::print(ostream& ostr, bool event_listing_format) const {

    // Standalone format. Used when calling:
    // particle->print()
    if( !event_listing_format ) {
        ostr << "GenParticle: "
             << barcode() << " ID:" << pdg_id()
             << " (P,E)=" << m_momentum.px() << "," << m_momentum.py()
             << "," << m_momentum.pz() << "," << m_momentum.e()
             << " Stat:" << status()
             << " PV:" << production_vertex_barcode()
             << " EV:" << end_vertex_barcode() << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
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
        ostr << m_momentum.px() << ",";
        ostr.width(9);
        ostr << m_momentum.py() << ",";
        ostr.width(9);
        ostr << m_momentum.pz() << ",";
        ostr.width(9);
        ostr << m_momentum.e() << " ";
        ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
        ostr.unsetf(std::ios_base::showpos);
        ostr.width(3);
        ostr << status() << " ";

        if( production_vertex_barcode() ) {
            ostr.width(9);
            ostr << production_vertex_barcode();
        }

        ostr << endl;

        ostr.flags(orig);
        ostr.precision(prec);
    }
}

} // namespace HepMC3

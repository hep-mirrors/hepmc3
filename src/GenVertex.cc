#include <iostream>
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex():
    m_barcode(0) {
}

GenVertex::~GenVertex() {}

void GenVertex::print( std::ostream& ostr, bool event_listing_format  ) const {

    // Standalone format. Used when calling:
    // vertex->print()
    if( !event_listing_format ) {
        ostr << "GenVertex: " << barcode()
             << " (X,cT):0 "
             << "Pin: "  << m_particles_in.size()
             <<" Pout: " << m_particles_out.size() << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        // find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize prec = ostr.precision();
        ostr << "Vtx: ";
        ostr.width(9);
        ostr << barcode()
             << "  (X,cT): 0" << endl;

        // print out all the incoming, then outgoing particles
        for(unsigned int i=0;i<m_particles_in.size();++i) {
            ostr << "  I ";
            m_particles_in[i]->print(ostr,1);
        }

        // restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
}

void GenVertex::add_particle_in(GenParticle *p) {
    m_particles_in.push_back(p);
}

void GenVertex::add_particle_out(GenParticle *p) {
    p->set_production_vertex_barcode(m_barcode);
    m_particles_out.push_back(p);
}

} // namespace HepMC3

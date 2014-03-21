#include <iostream>
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
using std::cout;
using std::endl;

namespace HepMC3 {

GenVertex::GenVertex():
    m_barcode(0) {
}

GenVertex::~GenVertex() {}

void GenVertex::print( std::ostream& ostr, int format ) const {

    if( format == 0 ) {
        ostr << "GenVertex: " << barcode()
             << " (X,cT):0 "
             << "Pin: "  << m_particles_in.size()
             <<" Pout: " << m_particles_out.size() << endl;
    }
    else if( format == 1 ) {
        // find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize prec = ostr.precision();
        ostr << "GenVertex:";
        ostr.width(9);
        ostr << barcode()
             << " (X,cT):0" << endl;

        // print out all the incoming, then outgoing particles
        for(unsigned int i=0;i<m_particles_in.size();++i) {
            if(i==0) {
                ostr << " I ";
                ostr.width(2);
                ostr << m_particles_in.size() <<": ";
            }
            else {
                ostr << "       ";
            }
            m_particles_in[i]->print(ostr,1);
        }

        for(unsigned int i=0;i<m_particles_out.size();++i) {
            if(i==0) {
                ostr << " O ";
                ostr.width(2);
                ostr << m_particles_out.size() <<": ";
            }
            else ostr << "       ";
            m_particles_out[i]->print(ostr,1);
        }
        // restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
    else if( format == 2 ) {
        ostr<<"GenVertex: File IO format not ready"<<endl;
    }
    else if( format == 3 ) {
        ostr<<"GenVertex: ROOT streamer format not ready"<<endl;
    }
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

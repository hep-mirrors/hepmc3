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
    else if( format == 2 ) {
        unsigned int in_size = m_particles_in.size();

        ostr<<"V ";
        ostr<<m_barcode<<" ";

        ostr<<"[";
        if(in_size) {
            for( unsigned int i=0; i<in_size-1; ++i ) {
                ostr<<m_particles_in[i]->barcode()<<",";
            }
            ostr<<m_particles_in.back()->barcode();
        }
        ostr<<"] ";

        ostr<<"@ 0 0 0 0";
        ostr<<endl;
    }
    else if( format == 3 ) {
        ostr<<"GenVertex: ROOT streamer format not ready"<<endl;
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

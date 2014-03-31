/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class HepMC3::IO_GenEvent
 *
 *  @date Created       <b> 23th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include <fstream>
#include "HepMC3/IO_GenEvent.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

void IO_GenEvent::write_event(const GenEvent *evt) {
    if ( !evt || m_file.rdstate() ) return;
	if ( m_mode != std::ios::out ) {
	    ERROR( "IO_GenEvent: attempting to write to input file" )
	    return;
	}

    m_file << "E " << evt->event_number()
           << " "  << evt->vertices().size()
           << " "  << evt->particles().size()
           << endl;

    for( vector<GenEventVersion*>::const_iterator v  = evt->versions().begin();
                                                  v != evt->versions().end();
                                                  ++v ) {

        // Version info
        m_file << "T " << (*v)->name() << endl;

        // Print all particles and vertices in this version
        int highest_vertex_already_printed = 0;

        for( vector<GenParticle*>::const_iterator i  = (*v)->particles().begin(); i != (*v)->particles().end(); ++i ) {

            int production_vertex = (*i)->ancestor();
            if( production_vertex < highest_vertex_already_printed ) {

                highest_vertex_already_printed = production_vertex;
                for( vector<GenVertex*>::const_iterator j = (*v)->vertices().begin(); j != (*v)->vertices().end(); ++j ) {
                    if( (*j)->barcode() == production_vertex ) {
                        write_vertex(*j);
                        break;
                    }
                }
            }
            write_particle(*i);
        }
    }
}

bool IO_GenEvent::fill_next_event(GenEvent *evt) {
    if ( !evt || m_file.rdstate() ) return 0;
	if ( m_mode != std::ios::in ) {
	    ERROR( "IO_GenEvent: attempting to read from output file" )
	    return 0;
	}

    WARNING( "IO_GenEvent: Reading not implemented (yet)" )

    return 1;
}

void IO_GenEvent::write_vertex(const GenVertex *v) {
    unsigned int in_size = v->particles_in().size();

    m_file << "V " << v->barcode()
           << " [";

    if(in_size) {
        for( unsigned int i=0; i<in_size-1; ++i ) {
            m_file << v->particles_in()[i]->barcode() << ",";
        }
        m_file << v->particles_in().back()->barcode();
    }
    m_file << "] ";

    m_file << "@ 0 0 0 0";
    m_file << endl;
}

void IO_GenEvent::write_particle(const GenParticle *p) {
    std::ios_base::fmtflags orig = m_file.flags();
    std::streamsize prec = m_file.precision();
    m_file.setf(std::ios::scientific, std::ios::floatfield);
    m_file.precision(m_precision);

    m_file << "P "<< p->barcode()
           << " " << p->ancestor()
           << " " << p->pdg_id()
           << " " << p->momentum().px()
           << " " << p->momentum().py()
           << " " << p->momentum().pz()
           << " " << p->momentum().e()
           << " " << p->generated_mass()
           << " " << p->status()
           << endl;

    m_file.flags(orig);
    m_file.precision(prec);
}

} // namespace HepMC3

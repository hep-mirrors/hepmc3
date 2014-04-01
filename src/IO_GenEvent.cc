/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class HepMC3::IO_GenEvent
 *
 *  @date Created       <b> 23th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <boost/foreach.hpp>
#include <boost/range/iterator_range.hpp>
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

    BOOST_FOREACH( GenEventVersion *ver , evt->versions() ) {

        // Version info
        m_file << "T " << ver->name() << endl;

        // Print vertices
        BOOST_FOREACH( GenVertex *v, evt->vertices() ) {
            write_vertex(v);
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

    // Write all incoming particles
    BOOST_FOREACH( GenParticle *p, v->particles_in() ) {
        write_particle(p);
    }

    m_file << "V " << v->barcode()
           << " [";

    if(v->particles_in().size()) {
        BOOST_FOREACH( GenParticle *p, boost::make_iterator_range(v->particles_in().begin(), v->particles_in().end()-1) ) {
            m_file << p->barcode() <<",";
        }
        m_file << v->particles_in().back()->barcode();
    }

    m_file << "] ";

    m_file << "@ 0 0 0 0";
    m_file << endl;

    // Write outgoing particles without their end vertex
    BOOST_FOREACH( GenParticle *p, v->particles_out() ) {
        if( !p->end_vertex() ) write_particle(p);
    }
}

void IO_GenEvent::write_particle(const GenParticle *p) {
    std::ios_base::fmtflags orig = m_file.flags();
    std::streamsize prec = m_file.precision();
    m_file.setf(std::ios::scientific, std::ios::floatfield);
    m_file.precision(m_precision);

    m_file << "P "<< p->barcode()
           << " ";
    if( p->production_vertex() ) m_file << p->production_vertex()->barcode();
    else                         m_file << "0";

    m_file << " " << p->pdg_id()
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

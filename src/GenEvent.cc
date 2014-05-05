/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Setup.h"

#include <vector>

#include <boost/foreach.hpp>
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent() {
    /** @bug Reallocation of vertices can change the pointer inside shared_ptr
     *  which should never happen. Needs to be checked why.
     */
    m_particles.reserve(100000);
    m_vertices.reserve(1000);
    set_print_precision(2);
    set_event_number(0);
}

void GenEvent::print( std::ostream& ostr ) const {
    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << event_number() << endl;
    ostr << "Entries in this event: " << vertices().size() << " vertices, "
         << particles().size() << " particles." << endl;

    // Print a legend to describe the particle info
    ostr << "                                    GenParticle Legend" << endl;
    ostr << "     Barcode   PDG ID   "
         << "( Px,       Py,       Pz,     E )"
         << "   Stat-Subst  ProdVtx" << endl;
    ostr << "________________________________________________________________________________" << endl;

    // Find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize         prec = ostr.precision();

    // Set precision
    ostr.precision( print_precision() );

    // Print all vertices
    BOOST_FOREACH( const GenVertex &v, vertices() ) {
        v.print(ostr,1);
    }

    // Restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
    ostr << "________________________________________________________________________________" << endl;
}

void GenEvent::dump() const {
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<"--------- EVENT DUMP --------"<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<std::endl;

    std::cout<<"GenParticle ("<<particles().size()<<")"<<std::endl;
    BOOST_FOREACH( const GenParticle &p, particles() ) {
        p.print();
    }

    std::cout<<"GenVertex ("<<vertices().size()<<")"<<std::endl;
    BOOST_FOREACH( const GenVertex &v, vertices() ) {
        v.print();
    }
    std::cout<<"-----------------------------"<<std::endl;
}

void GenEvent::add_particle(GenParticle &p) {
    if( p.in_event() ) return;

    p.m_data->event = this;
    p.m_data->index = particles().size();

    m_particles.push_back(p);
}

void GenEvent::add_vertex(GenVertex &v) {
    if( v.in_event() ) return;

    // Add all incoming and outgoing particles
    BOOST_FOREACH( GenParticle &p, v.m_data->particles_in ) {
        add_particle(p);
    }

    BOOST_FOREACH( GenParticle &p, v.m_data->particles_out ) {
        add_particle(p);
    }

    v.m_data->event = this;
    v.m_data->index = vertices().size();

    m_vertices.push_back(v);
}

void GenEvent::delete_particle(GenParticle &p) {
    return;
}

void GenEvent::delete_vertex(GenVertex &v) {
    return;
}

} // namespace HepMC3

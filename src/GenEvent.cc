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
#include <deque>

#include <boost/foreach.hpp>
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent() {
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

void GenEvent::add_tree( const vector<GenParticle> &particles ) {

    std::deque<GenVertex> sorting;

    // Find all starting vertices (end vertex of particles that have no production vertex)
    for( unsigned int i=0; i<particles.size(); ++i) {
        const GenParticle &p = particles[i];
        GenVertex v = p.production_vertex();
        if( !v || v.particles_in().size()==0 ) {
            GenVertex v2 = p.end_vertex();
            if(v2) sorting.push_back(v2);
        }
    }

    DEBUG_CODE_BLOCK(
        unsigned int sorting_loop_count = 0;
        unsigned int max_deque_size     = 0;
    )

    // Add vertices to the event in topological order
    while( !sorting.empty() ) {
        DEBUG_CODE_BLOCK(
            if( sorting.size() > max_deque_size ) max_deque_size = sorting.size();
            ++sorting_loop_count;
        )

        GenVertex &v = sorting.front();

        bool added = false;

        // Add all mothers to the front of the list
        BOOST_FOREACH( const GenParticle &p, v.particles_in() ) {
            GenVertex v2 = p.production_vertex();
            if( v2 && !v2.in_event() ) {
                sorting.push_front(v2);
                added = true;
            }
        }

        // If we have added at least one production vertex,
        // our vertex is not the first one on the list
        if( added ) continue;

        // If vertex not yet added
        if( !v.in_event() ) {

            add_vertex(v);

            // Add all end vertices to the end of the list
            BOOST_FOREACH( const GenParticle &p, v.particles_out() ) {
                GenVertex v2 = p.end_vertex();
                if( v2 && !v2.in_event() ) {
                    sorting.push_back(v2);
                }
            }
        }

        sorting.pop_front();
    }

    DEBUG_CODE_BLOCK(
        DEBUG( 6, "IO_HepMC2_adapter - particles sorted: "
                   <<particles_count()<<", max deque size: "
                   <<max_deque_size<<", iterations: "<<sorting_loop_count )
    )
}

void GenEvent::delete_particle(GenParticle &p) {
    return;
}

void GenEvent::delete_vertex(GenVertex &v) {
    return;
}

void GenEvent::reserve(unsigned int particles, unsigned int vertices) {
    m_particles.reserve(particles);
    m_vertices.reserve(vertices);
}

} // namespace HepMC3

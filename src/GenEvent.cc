/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class GenEvent
 *
 */
#include "HepMC3/GenEvent.h"

#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Setup.h"

#include "HepMC3/Data/GenEventData.h"

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
    BOOST_FOREACH( const GenVertexPtr &v, vertices() ) {
        v->print(ostr,1);
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

    std::cout<<"GenParticlePtr ("<<particles().size()<<")"<<std::endl;
    BOOST_FOREACH( const GenParticlePtr &p, particles() ) {
        p->print();
    }

    std::cout<<"GenVertexPtr ("<<vertices().size()<<")"<<std::endl;
    BOOST_FOREACH( const GenVertexPtr &v, vertices() ) {
        v->print();
    }
    std::cout<<"-----------------------------"<<std::endl;
}

void GenEvent::add_particle( const GenParticlePtr &p ) {
    if( p->in_event() ) return;

    p->m_event = this;
    p->m_index = particles().size();

    m_particles.push_back(p);
}

void GenEvent::add_vertex( const GenVertexPtr &v ) {
    if( v->in_event() ) return;

    v->m_event = this;
    v->m_index = vertices().size();

    m_vertices.push_back(v);

    // Add all incoming and outgoing particles and restore their production/end vertices
    BOOST_FOREACH( const GenParticlePtr &p, v->m_particles_in ) {
        if(!p->in_event()) {
            add_particle(p);
            p->m_end_vertex = v->m_this;
        }
    }

    BOOST_FOREACH( const GenParticlePtr &p, v->m_particles_out ) {
        if(!p->in_event()) {
            add_particle(p);
            p->m_production_vertex = v->m_this;
        }
    }

}

void GenEvent::add_tree( const vector<GenParticlePtr> &particles ) {

    std::deque<GenVertexPtr> sorting;

    // Find all starting vertices (end vertex of particles that have no production vertex)
    BOOST_FOREACH( const GenParticlePtr &p, particles ) {
        const GenVertexPtr &v = p->production_vertex();
        if( !v || v->particles_in().size()==0 ) {
            const GenVertexPtr &v2 = p->end_vertex();
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

        GenVertexPtr &v = sorting.front();

        bool added = false;

        // Add all mothers to the front of the list
        BOOST_FOREACH( const GenParticlePtr &p, v->particles_in() ) {
            GenVertexPtr v2 = p->production_vertex();
            if( v2 && !v2->in_event() ) {
                sorting.push_front(v2);
                added = true;
            }
        }

        // If we have added at least one production vertex,
        // our vertex is not the first one on the list
        if( added ) continue;

        // If vertex not yet added
        if( !v->in_event() ) {

            add_vertex(v);

            // Add all end vertices to the end of the list
            BOOST_FOREACH( const GenParticlePtr &p, v->particles_out() ) {
                GenVertexPtr v2 = p->end_vertex();
                if( v2 && !v2->in_event() ) {
                    sorting.push_back(v2);
                }
            }
        }

        sorting.pop_front();
    }

    DEBUG_CODE_BLOCK(
        DEBUG( 6, "GenEvent - particles sorted: "
                   <<particles_count()<<", max deque size: "
                   <<max_deque_size<<", iterations: "<<sorting_loop_count )
    )
}

void GenEvent::delete_particle( const GenParticlePtr &p ) {
    return;
}

void GenEvent::delete_vertex( const GenVertexPtr &v ) {
    return;
}

void GenEvent::reserve(unsigned int particles, unsigned int vertices) {
    m_particles.reserve(particles);
    m_vertices.reserve(vertices);
}

GenEventData* GenEvent::serializable_data() const {
    GenEventData* data = new GenEventData();

    BOOST_FOREACH( const GenParticlePtr &p, particles() ) {
        data->particles.push_back(*p);
    }

    BOOST_FOREACH( const GenVertexPtr &v, vertices() ) {
        data->vertices.push_back(*v);

        int barcode = v->barcode();
        BOOST_FOREACH( const GenParticlePtr &p, v->particles_in() ) {
            data->links.push_back( std::pair<int,int>(p->barcode(),barcode) );
        }

        BOOST_FOREACH( const GenParticlePtr &p, v->particles_out() ) {
            data->links.push_back( std::pair<int,int>(barcode,p->barcode()) );
        }
    }

    return data;
}

//
// Deprecated functions
//

inline void GenEvent::add_particle( GenParticle *p ) {
    add_particle( GenParticlePtr(p) );
}

inline void GenEvent::add_vertex( GenVertex *v ) {
    add_vertex( GenVertexPtr(v) );
}

} // namespace HepMC3

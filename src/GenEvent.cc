/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class GenEvent
 *
 */
#include "HepMC/GenEvent.h"

#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/Setup.h"

#include "HepMC/Data/GenEventData.h"

#include <vector>
#include <deque>

#include "HepMC/foreach.h"

using std::endl;

namespace HepMC {

GenEvent::GenEvent(Units::MomentumUnit momentum_unit, Units::LengthUnit length_unit) {
    set_print_precision(2);
    set_event_number(0);
    m_momentum_unit = momentum_unit;
    m_length_unit   = length_unit;
}

void GenEvent::print( std::ostream& ostr ) const {

    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << event_number() << endl;
    ostr << " Momenutm units: " << Units::name(m_momentum_unit)
         << " Position units: " << Units::name(m_length_unit) << endl;
    if( m_pdf_info )      m_pdf_info->print(ostr);
    if( m_cross_section ) m_cross_section->print(ostr);
    if( m_heavy_ion )     m_heavy_ion->print(ostr);
    ostr << " Entries in this event: " << vertices().size() << " vertices, "
         << particles().size() << " particles." << endl;
    ostr << " Beam particle indexes:";
    if(m_beam_particle_1) ostr << " " << m_beam_particle_1->id();
    if(m_beam_particle_2) ostr << " " << m_beam_particle_2->id();
    ostr << "\n";

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
    FOREACH( const GenVertexPtr &v, vertices() ) {
        v->print_event_listing(ostr);
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

    if( m_pdf_info )      m_pdf_info->print();
    if( m_cross_section ) m_cross_section->print();
    if( m_heavy_ion )     m_heavy_ion->print();

    std::cout<<"GenParticlePtr ("<<particles().size()<<")"<<std::endl;

    FOREACH( const GenParticlePtr &p, particles() )
    {
        p->print();
    }

    std::cout<<"GenVertexPtr ("<<vertices().size()<<")"<<std::endl;
    FOREACH( const GenVertexPtr &v, vertices() ) {
        v->print();
    }

    std::cout<<"-----------------------------"<<std::endl;
}

void GenEvent::add_particle( const GenParticlePtr &p ) {
    if( p->in_event() ) return;

    m_particles.push_back(p);

    p->m_event = this;
    p->m_id    = particles().size();
}

void GenEvent::add_vertex( const GenVertexPtr &v ) {
    if( v->in_event() ) return;

    m_vertices.push_back(v);

    v->m_event = this;
    v->m_id    = -(int)vertices().size();

    // Add all incoming and outgoing particles and restore their production/end vertices
    FOREACH( const GenParticlePtr &p, v->m_particles_in ) {
        if(!p->in_event()) add_particle(p);
        p->set_end_vertex(v->m_this.lock());
    }

    FOREACH( const GenParticlePtr &p, v->m_particles_out ) {
        if(!p->in_event()) add_particle(p);
        p->set_production_vertex(v->m_this.lock());
    }
}

void GenEvent::add_tree( const vector<GenParticlePtr> &particles ) {

    std::deque<GenVertexPtr> sorting;

    // Find all starting vertices (end vertex of particles that have no production vertex)
    FOREACH( const GenParticlePtr &p, particles ) {
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
        FOREACH( const GenParticlePtr &p, v->particles_in() ) {
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
            FOREACH( const GenParticlePtr &p, v->particles_out() ) {
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

void GenEvent::reserve(unsigned int particles, unsigned int vertices) {
    m_particles.reserve(particles);
    m_vertices.reserve(vertices);
}

void GenEvent::set_units( Units::MomentumUnit new_momentum_unit, Units::LengthUnit new_length_unit) {
    if( new_momentum_unit != m_momentum_unit ) {
        FOREACH( GenParticlePtr &p, m_particles ) {
            Units::convert( p->m_data.momentum, m_momentum_unit, new_momentum_unit );
        }

        m_momentum_unit = new_momentum_unit;
    }

    if( new_length_unit != m_length_unit ) {
        FOREACH( GenVertexPtr &v, m_vertices ) {
            FourVector &fv = v->m_data.position;
            if( !fv.is_zero() ) Units::convert( fv, m_length_unit, new_length_unit );
        }

        m_length_unit = new_length_unit;
    }
}

void GenEvent::clear() {
    m_event_number = 0;

    m_heavy_ion.reset();
    m_pdf_info.reset();
    m_cross_section.reset();

    m_particles.clear();
    m_vertices.clear();
}

//
// Deprecated functions
//

void GenEvent::add_particle( GenParticle *p ) {
    add_particle( GenParticlePtr(p) );
}

void GenEvent::add_vertex( GenVertex *v ) {
    add_vertex( GenVertexPtr(v) );
}

void GenEvent::set_heavy_ion(GenHeavyIon *hi) {
     m_heavy_ion.reset(hi);
}

void GenEvent::set_pdf_info(GenPdfInfo *pi) {
     m_pdf_info.reset(pi);
}

void GenEvent::set_cross_section(GenCrossSection *cs) {
     m_cross_section.reset(cs);
}

} // namespace HepMC

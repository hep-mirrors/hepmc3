/**
 *  @file IO_RootStreamer.cc
 *  @brief Implementation of \b class IO_RootStreamer
 *
 */
#include "HepMC3/IO/IO_RootStreamer.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Setup.h"

#include <vector>

#include <boost/foreach.hpp>
using std::vector;

namespace HepMC3 {

IO_RootStreamer::IO_RootStreamer() {
}

void IO_RootStreamer::write_event(const GenEvent &evt) {

    // Clear content of m_data container
    m_data.particles.clear();
    m_data.vertices.clear();
    m_data.links1.clear();
    m_data.links2.clear();

    m_data.particles.reserve( evt.particles().size() );
    m_data.vertices.reserve( evt.vertices().size() );
    m_data.links1.reserve( evt.particles().size()*2 );
    m_data.links2.reserve( evt.particles().size()*2 );

    BOOST_FOREACH( const GenParticlePtr &p, evt.particles() ) {
        m_data.particles.push_back( p->data() );
    }

    BOOST_FOREACH( const GenVertexPtr &v, evt.vertices() ) {
        m_data.vertices.push_back( v->data() );
        int v_id = v->id();

        BOOST_FOREACH( const GenParticlePtr &p, v->particles_in() ) {
            m_data.links1.push_back( p->id() );
            m_data.links2.push_back( v_id    );
        }

        BOOST_FOREACH( const GenParticlePtr &p, v->particles_out() ) {
            m_data.links1.push_back( v_id    );
            m_data.links2.push_back( p->id() );
        }
    }
}

bool IO_RootStreamer::fill_next_event(GenEvent &evt) {

    WARNING( "IO_RootStreamer: Reading not implemented (yet)" )

    return true;
}

} // namespace HepMC3

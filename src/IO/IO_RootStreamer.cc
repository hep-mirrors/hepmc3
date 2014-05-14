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

    BOOST_FOREACH( const GenParticlePtr &p, evt.particles() ) {
        m_data.particles.push_back( p->data() );
    }

    BOOST_FOREACH( const GenVertexPtr &v, evt.vertices() ) {
        m_data.vertices.push_back( v->data() );
    }
}

bool IO_RootStreamer::fill_next_event(GenEvent &evt) {

    WARNING( "IO_RootStreamer: Reading not implemented (yet)" )

    return true;
}

} // namespace HepMC3

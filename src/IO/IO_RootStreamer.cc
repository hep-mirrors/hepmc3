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
        write_particle(p);
    }

    BOOST_FOREACH( const GenVertexPtr &v, evt.vertices() ) {
        write_vertex(v);
    }
}

bool IO_RootStreamer::fill_next_event(GenEvent &evt) {

    WARNING( "IO_RootStreamer: Reading not implemented (yet)" )

    return true;
}

void IO_RootStreamer::write_vertex(const GenVertexPtr &v) {
    m_data.vertices.push_back( GenVertexData() );

    GenVertexData &data = m_data.vertices.back();

    data.position = v->position();
}

void IO_RootStreamer::write_particle(const GenParticlePtr &p) {
    m_data.particles.push_back( GenParticleData() );

    GenParticleData &data = m_data.particles.back();

    data.pdg_id                = p->pdg_id();
    data.momentum              = p->momentum();
    data.status                = p->status();
    data.status_subcode        = p->status_subcode();
    data.generated_mass        = p->generated_mass();
    data.is_generated_mass_set = p->is_generated_mass_set();

    data.production_vertex = 0;

    const GenVertexPtr &vP = p->production_vertex();
    if(vP) data.production_vertex = vP->barcode();

    data.end_vertex = 0;

    const GenVertexPtr &vE = p->end_vertex();
    if(vE) data.end_vertex = vE->barcode();
}


} // namespace HepMC3

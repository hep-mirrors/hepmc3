/**
 *  @file GenEventVersion.cc
 *  @brief Implementation of \b class HepMC3::GenEventVersion
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include "HepMC3/GenEventVersion.h"
#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

GenEventVersion::GenEventVersion(int number, int particle_barcode, int vertex_barcode, const char *name):
m_version(number),
m_first_particle_barcode(particle_barcode),
m_first_vertex_barcode(vertex_barcode),
m_name(name) {
}

GenParticle& GenEventVersion::create_particle() {

    m_particles.increase_size();

    m_particles[m_particles.size()-1].set_barcode( m_particles.size() + m_first_particle_barcode - 1 );

    return m_particles[m_particles.size()-1];
}

GenVertex&   GenEventVersion::create_vertex() {

    m_vertices.increase_size();

    m_vertices[m_vertices.size()-1].set_barcode( m_first_vertex_barcode - m_vertices.size() - 1 );

    return m_vertices[m_vertices.size()-1];
}

void GenEventVersion::delete_particle(GenParticle &p) {
    p.set_version_deleted(m_version);
}

void GenEventVersion::delete_vertex(GenVertex &v) {
    v.set_version_deleted(m_version);
}

const GenParticle& GenEventVersion::get_particle(int barcode) const {

    return m_particles[ barcode - m_first_particle_barcode ];
}

const GenVertex&   GenEventVersion::get_vertex(int barcode) const {

    return m_vertices [ -(barcode + m_first_vertex_barcode) ];
}

} // namespace HepMC3

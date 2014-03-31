/**
 *  @file GenParticle.cc
 *  @brief Implementation of \b class HepMC3::GenParticle
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <iostream>
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::endl;
using std::ostream;

namespace HepMC3 {

GenParticle::GenParticle():
m_parent_event(NULL),
m_momentum(0.0,0.0,0.0,0.0),
m_pdgid(0),
m_status(0),
m_status_subcode(0),
m_barcode(0),
m_ancestor(0),
m_descendant(0),
m_generated_mass(0.0),
m_is_generated_mass_set(false),
m_version_deleted(255) {

}

GenParticle::GenParticle(FourVector momentum, int pdgid, int status):
m_parent_event(NULL),
m_momentum(momentum),
m_pdgid(pdgid),
m_status(status),
m_status_subcode(0),
m_barcode(0),
m_ancestor(0),
m_descendant(0),
m_generated_mass(0.0),
m_is_generated_mass_set(false),
m_version_deleted(255) {

}

GenParticle::~GenParticle() {}

GenVertex* GenParticle::production_vertex() const {
    if( m_ancestor < 0 ) return m_parent_event->find_vertex(m_ancestor);
    else return NULL;
}

GenVertex* GenParticle::create_production_vertex() {
    if( !m_parent_event ) {
        ERROR( "GenParticle::create_production_vertex: particle must belong to an event" )
        return NULL;
    }

    if( m_ancestor < 0 ) return m_parent_event->find_vertex(m_ancestor);

    GenVertex *v = new GenVertex();

    if( m_ancestor > 0 ) v->add_particle_in(m_parent_event->find_particle(m_ancestor));

    v->add_particle_out(this);
    m_parent_event->add_vertex(v);

    return v;
}

GenVertex* GenParticle::end_vertex() const {
    if( m_descendant < 0 ) return m_parent_event->find_vertex(m_descendant);
    else return NULL;
}

GenVertex* GenParticle::create_end_vertex() {
    if( !m_parent_event ) {
        ERROR( "GenParticle::create_end_vertex: particle must belong to an event" )
        return NULL;
    }

    if( m_descendant < 0 ) return m_parent_event->find_vertex(m_descendant);

    GenVertex *v = new GenVertex();

    // Do not trust descendants - find them yourself
    for( vector<GenParticle*>::const_iterator i  = m_parent_event->particles().begin();
                                              i != m_parent_event->particles().end();
                                              ++i ) {
        if( (*i)->ancestor() == m_barcode ) v->add_particle_out(*i);
    }

    v->add_particle_in(this);
    m_parent_event->add_vertex(v);

    return v;
}

void GenParticle::print(ostream& ostr, bool event_listing_format) const {

    // Standalone format. Used when calling:
    // particle->print()
    if( !event_listing_format ) {
        ostr << "GenParticle: "
             << barcode() << " ID:" << pdg_id()
             << " (P,E)=" << m_momentum.px() << "," << m_momentum.py()
             << "," << m_momentum.pz() << "," << m_momentum.e()
             << " Stat:" << status()
             << " PV:" << ancestor()
             << " EV:" << descendant() << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        // Find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize prec = ostr.precision();

        ostr << " ";
        ostr.width(6);
        ostr << barcode();
        ostr.width(9);
        ostr << pdg_id() << " ";
        ostr.width(9);
        ostr.precision(2);
        ostr.setf(std::ios::scientific, std::ios::floatfield);
        ostr.setf(std::ios_base::showpos);
        ostr << m_momentum.px() << ",";
        ostr.width(9);
        ostr << m_momentum.py() << ",";
        ostr.width(9);
        ostr << m_momentum.pz() << ",";
        ostr.width(9);
        ostr << m_momentum.e() << " ";
        ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
        ostr.unsetf(std::ios_base::showpos);
        ostr.width(3);
        ostr << status();
        if( status_subcode() ) {
            ostr << "-";
            ostr.width(9);
            ostr << std::left << status_subcode() << std::right;
        }
        else ostr << "          ";

        if( ancestor() ) {
            ostr.width(6);
            ostr << ancestor();
        }

        ostr << endl;

        // Restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
}

bool GenParticle::set_barcode(int barcode) {
    if( m_parent_event ) return false;

    m_barcode = barcode;

    return true;
}

double GenParticle::generated_mass() const {
    if( m_is_generated_mass_set ) return m_generated_mass;
    else                          return momentum().m();
}

} // namespace HepMC3

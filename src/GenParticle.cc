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
m_production_vertex(0),
m_end_vertex(0),
m_generated_mass(0.0) {

}

GenParticle::GenParticle(FourVector momentum, int pdgid, int status):
m_parent_event(NULL),
m_momentum(momentum),
m_pdgid(pdgid),
m_status(status),
m_status_subcode(0),
m_barcode(0),
m_production_vertex(0),
m_end_vertex(0),
m_generated_mass(0.0) {

}

GenParticle::~GenParticle() {}

void GenParticle::print(ostream& ostr, bool event_listing_format) const {

    // Standalone format. Used when calling:
    // particle->print()
    if( !event_listing_format ) {
        ostr << "GenParticle: "
             << barcode() << " ID:" << pdg_id()
             << " (P,E)=" << m_momentum.px() << "," << m_momentum.py()
             << "," << m_momentum.pz() << "," << m_momentum.e()
             << " Stat:" << status()
             << " PV:" << production_vertex_barcode()
             << " EV:" << end_vertex_barcode() << endl;
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

        if( production_vertex_barcode() ) {
            ostr.width(6);
            ostr << production_vertex_barcode();
        }

        ostr << endl;

        // Restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);
    }
}

void GenParticle::set_parent_event(GenEvent *evt) {
    // If particle already belongs to other event - remove it from that event
    if( m_parent_event && m_parent_event != evt ) {
        m_parent_event->remove_particle(this);
    }

    // Do not add if already part of this event
    if( m_parent_event == evt) return;

    if( evt ) {
        m_barcode      = evt->get_highest_particle_barcode(); //!< @todo Not the best way to do it - fix it when versioning is ready
        m_parent_event = evt;
    }
    else {
        m_barcode      = 0;
        m_parent_event = NULL;
    }
}

} // namespace HepMC3

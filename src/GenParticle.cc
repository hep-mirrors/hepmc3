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
m_production_vertex(0),
m_end_vertex(0),
m_momentum(0.0,0.0,0.0,0.0),
m_pdgid(0),
m_status(0),
m_status_subcode(0),
m_barcode(0),
m_generated_mass(0.0),
m_is_generated_mass_set(false),
m_version_created(0),
m_version_deleted(255) {

}

GenParticle::GenParticle(FourVector momentum, int pdgid, int status):
m_production_vertex(0),
m_end_vertex(0),
m_momentum(momentum),
m_pdgid(pdgid),
m_status(status),
m_status_subcode(0),
m_barcode(0),
m_generated_mass(0.0),
m_is_generated_mass_set(false),
m_version_created(0),
m_version_deleted(255) {

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
             << " Stat: " << status()
             << " PV: " << production_vertex()
             << " EV: " << end_vertex()
             << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        ostr << " ";
        ostr.width(6);
        ostr << barcode();
        ostr.width(9);
        ostr << pdg_id() << " ";
        ostr.width(9);
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

        if( production_vertex() ) {
            ostr.width(6);
            ostr << production_vertex();
        }

        ostr << endl;
    }
}

double GenParticle::generated_mass() const {
    if( m_is_generated_mass_set ) return m_generated_mass;
    else                          return momentum().m();
}

} // namespace HepMC3

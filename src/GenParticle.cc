/**
 *  @file GenParticle.cc
 *  @brief Implementation of \b class HepMC3::GenParticle
 *
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Setup.h"

namespace HepMC3 {

GenParticle::GenParticle( const FourVector &momentum, int pdg_id, int status ):
m_event(NULL),
m_index(0),
m_pdg_id(pdg_id),
m_momentum(momentum),
m_status(status),
m_status_subcode(0),
m_mass(0.0),
m_is_mass_set(false) {
}

void GenParticle::print( std::ostream& ostr, bool event_listing_format ) const {

    int prod_vtx_barcode = (!m_production_vertex.expired()) ? m_production_vertex.lock()->barcode() : 0;

    // Standalone format. Used when calling:
    // particle->print()
    if( !event_listing_format ) {
        ostr << "GenParticle: ";
        ostr.width(3);
        ostr << barcode();
        ostr << " ID: ";
        ostr.width(5);
        ostr << m_pdg_id;

        // Find the current stream state
        std::ios_base::fmtflags orig = ostr.flags();
        std::streamsize         prec = ostr.precision();

        if(m_event) ostr.precision(m_event->print_precision());

        ostr.setf(std::ios::scientific, std::ios::floatfield);
        ostr.setf(std::ios_base::showpos);
        ostr << " (P,E)=" << m_momentum.px()
                   << "," << m_momentum.py()
                   << "," << m_momentum.pz()
                   << "," << m_momentum.e();

        // Restore the stream state
        ostr.flags(orig);
        ostr.precision(prec);

        int end_vtx_barcode = (!m_end_vertex.expired()) ? m_end_vertex.lock()->barcode() : 0;

        ostr << " Stat: " << m_status
             << " PV: " << prod_vtx_barcode
             << " EV: " << end_vtx_barcode
             << std::endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        ostr << " ";
        ostr.width(6);
        ostr << barcode();

        ostr.width(9);
        ostr << m_pdg_id << " ";
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
        ostr << m_status;
        if( m_status_subcode ) {
            ostr << "-";
            ostr.width(9);
            ostr << std::left << m_status_subcode << std::right;
        }
        else ostr << "          ";

        if( prod_vtx_barcode ) {
            ostr.width(6);
            ostr << prod_vtx_barcode;
        }

        ostr << std::endl;
    }
}

double GenParticle::generated_mass() const {
    if(m_is_mass_set) return m_mass;
    else              return m_momentum.m();
}

void GenParticle::set_pdg_id(int pdg_id) {
    m_pdg_id = pdg_id;
}

void GenParticle::set_status(int status) {
    m_status = status;
}

void GenParticle::set_status_subcode(int subcode) {
    m_status_subcode = subcode;
}

void GenParticle::set_momentum(const FourVector& momentum) {
    m_momentum = momentum;
}

void GenParticle::set_generated_mass(double m) {
    m_mass        = m;
    m_is_mass_set = true;
}

void GenParticle::unset_generated_mass() {
    m_mass        = 0.;
    m_is_mass_set = false;
}

GenVertexPtr GenParticle::production_vertex() const {
    return m_production_vertex.lock();
}

GenVertexPtr GenParticle::end_vertex() const {
    return m_end_vertex.lock();
}

} // namespace HepMC3

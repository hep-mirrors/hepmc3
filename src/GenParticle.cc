/**
 *  @file GenParticle.cc
 *  @brief Implementation of \b class HepMC3::GenParticle
 *
 */
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenEvent.h"

#include "HepMC3/Log.h"
using std::endl;

namespace HepMC3 {

GenParticle::GenParticle(GenEvent &event, int data_index, GenParticleData &data):
m_event(event),
m_version_created(m_event.last_version()),
m_version_deleted(255),
m_data_index(data_index),
m_data(data),
m_last_version(this) {
}

void GenParticleData::print() const {
    std::cout<<pdg_id
             <<" "<<production_vertex
             <<" "<<end_vertex
             <<" "<<momentum.px()
             <<" "<<momentum.py()
             <<" "<<momentum.pz()
             <<" "<<momentum.e()
             <<" "<<status
             <<" "<<status_subcode
             <<" "<<mass
             <<" "<<is_mass_set
             <<std::endl;
}

void GenParticle::print( std::ostream& ostr, bool event_listing_format ) const {

    // Standalone format. Used when calling:
    // particle->print()
    if( !event_listing_format ) {
        ostr << "GenParticle: ";
        ostr.width(3);
        ostr << barcode() << " (v. ";
        if( !is_deleted() ) ostr<<" "<<m_version_created<<" ) ";
        else                ostr<<m_version_created<<"-"<<m_version_deleted<<") ";

        ostr << "ID:" << m_data.pdg_id
             << " (P,E)=" << m_data.momentum.px() << "," << m_data.momentum.py()
             << "," << m_data.momentum.pz() << "," << m_data.momentum.e()
             << " Stat: " << m_data.status
             << " PV: " << m_data.production_vertex
             << " EV: " << m_data.end_vertex
             << endl;
    }
    // Event listing format. Used when calling:
    // event->print()
    else {
        ostr << " ";
        ostr.width(6);
        ostr << barcode();

        ostr.width(9);
        ostr << m_data.pdg_id << " ";
        ostr.width(9);
        ostr.setf(std::ios::scientific, std::ios::floatfield);
        ostr.setf(std::ios_base::showpos);
        ostr << m_data.momentum.px() << ",";
        ostr.width(9);
        ostr << m_data.momentum.py() << ",";
        ostr.width(9);
        ostr << m_data.momentum.pz() << ",";
        ostr.width(9);
        ostr << m_data.momentum.e() << " ";
        ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
        ostr.unsetf(std::ios_base::showpos);
        ostr.width(3);
        ostr << m_data.status;
        if( m_data.status_subcode ) {
            ostr << "-";
            ostr.width(9);
            ostr << std::left << m_data.status_subcode << std::right;
        }
        else ostr << "          ";

        if( m_data.production_vertex ) {
            ostr.width(6);
            ostr << m_data.production_vertex;
        }

        ostr << endl;
    }
}

void GenParticle::mark_deleted() {
    m_version_deleted = m_event.last_version();
}

double GenParticle::generated_mass() const {
    if(m_last_version->m_data.is_mass_set) return m_last_version->m_data.mass;
    else                            return m_last_version->m_data.momentum.m();
}

void GenParticle::set_pdg_id(int pdg_id) {
    m_event.record_change(*this);
    m_last_version->m_data.pdg_id = pdg_id;
}

void GenParticle::set_status(int status) {
    m_event.record_change(*this);
    m_last_version->m_data.status = status;
}

void GenParticle::set_status_subcode(int subcode) {
    m_event.record_change(*this);
    m_last_version->m_data.status_subcode = subcode;
}

void GenParticle::set_momentum(const FourVector& momentum) {
    m_event.record_change(*this);
    m_last_version->m_data.momentum = momentum;
}

void GenParticle::set_generated_mass(double m) {
    m_event.record_change(*this);
    m_last_version->m_data.mass = m;
    m_last_version->m_data.is_mass_set = true;
}

void GenParticle::unset_generated_mass() {
    m_event.record_change(*this);
    m_last_version->m_data.mass = 0.;
    m_last_version->m_data.is_mass_set = false;
}

GenVertex* GenParticle::production_vertex() const {
    if( m_data.production_vertex >= 0 ) return NULL;

    GenVertex &v = m_event.vertex(m_data.production_vertex);

    if( v.is_deleted() ) return NULL;

    return &v;
}

void GenParticle::set_production_vertex( const GenVertex *v ) {

    /** @note HepMC3 format requires that all versions of the particle
     *        have the same origin. Therefore changing production vertex
     *        in new version is prohibited.
     */
    if( m_event.last_version() != m_version_created ) {
        ERROR( "GenParticle: Cannot change production vertex. Create a copy of this particle instead")
        return;
    }

    if(v) m_data.production_vertex = v->barcode();
    else  m_data.production_vertex = 0;
}

GenVertex* GenParticle::end_vertex() const {
    if( m_data.end_vertex >= 0 ) return NULL;

    GenVertex &v = m_event.vertex(m_data.end_vertex);

    if( v.is_deleted() ) return NULL;

    return &v;
}

void GenParticle::set_end_vertex( const GenVertex *v ) {
    m_event.record_change(*this);
    if(v) m_data.end_vertex = v->barcode();
    else  m_data.end_vertex = 0;
}

} // namespace HepMC3

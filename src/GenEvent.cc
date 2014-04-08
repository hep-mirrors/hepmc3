/**
 *  @file GenEvent.cc
 *  @brief Implementation of \b class HepMC3::GenEvent
 *
 *  @date Created       <b> 19th March 2014 </b>
 *  @date Last modified <b> 25th March 2014 </b>
 */
#include <boost/foreach.hpp>
#include <cmath>  // std::abs
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"
using std::ostream;
using std::endl;

namespace HepMC3 {

GenEvent::GenEvent():
m_event_number(0),
m_last_particle(0),
m_last_vertex(0),
m_last_version(0),
m_print_precision(2) {

    // Create default version
    m_versions.push_back( GenEventVersion(0, 1, 1, "Version 0") );
}

void GenEvent::print_version( int version, std::ostream& ostr ) const {
    ostr << "________________________________________________________________________________" << endl;
    ostr << "GenEvent: #" << m_event_number << endl;
    ostr << " Version: \"" << m_versions[m_last_version].name()
         << "\" (version id: " << m_last_version << ", last version id: " << m_last_version << ")" <<endl;
    ostr << " Entries in this event: " << -m_last_vertex << " vertices, "
       << m_last_particle << " particles." << endl;

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
    ostr.precision(m_print_precision);

    for( unsigned int i=0; i<m_versions[m_last_version].vertices().size(); ++i ) {
        GenVertex &v = m_versions[m_last_version].vertices()[i];
        if( v.version_deleted() <= m_last_version ) continue;
        v.print(ostr,1);

        bool printed_header = false;

        // Print out all the incoming particles
        BOOST_FOREACH( int barcode, v.particles_in() ) {
            const GenParticle &p = m_versions[m_last_version].get_particle(barcode);

            if( p.version_deleted() <= m_last_version ) continue;
            if( !printed_header ) {
                ostr << " I: ";
                printed_header = true;
            }
            else ostr << "    ";
            p.print(ostr,1);
        }

        printed_header = false;

        // Print out all the outgoing particles
        BOOST_FOREACH( int barcode, v.particles_out() ) {
            const GenParticle &p = m_versions[m_last_version].get_particle(barcode);

            if( p.version_deleted() <= m_last_version ) continue;
            if( !printed_header ) {
                ostr << " O: ";
                printed_header = true;
            }
            else ostr << "    ";
            p.print(ostr,1);
        }
    }

    // Restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
    ostr << "________________________________________________________________________________" << endl;
}

void GenEvent::create_new_version(const char *name) {
    ++m_last_version;
    m_versions.push_back( GenEventVersion( m_last_version, m_last_particle, m_last_vertex, name) );
}

} // namespace HepMC3

/**
 *  @file IO_GenEvent.cc
 *  @brief Implementation of \b class HepMC3::IO_GenEvent
 *
 */
#include "HepMC3/IO_GenEvent.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Log.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#include <boost/range/iterator_range.hpp>
#include <boost/foreach.hpp>
using std::endl;
using std::vector;
using std::pair;
using std::set;

namespace HepMC3 {

void IO_GenEvent::write_event(const GenEvent &evt) {
    if ( m_file.rdstate() ) return;
    if ( m_mode != std::ios::out ) {
        ERROR( "IO_GenEvent: attempting to write to input file" )
        return;
    }

    m_file << "E " << evt.event_number()
           << " "  << evt.vertices_count()
           << " "  << evt.particles_count()
           << endl;

    int vertices_processed    = 0;
    int lowest_vertex_barcode = 0;

    set<pair<int,int> > deleted_barcodes;

    for(unsigned int i=0; i<evt.versions().size(); ++i) {

        const VersionInfo &v = evt.versions()[i];
        m_file << "T " << evt.versions()[i].name
               << endl;

        // Get the upper range of particles and vertices
        int last_particle_index = evt.particles_count() - 1;
        int last_vertex_index   = evt.vertices_count()  - 1;

        if( i+1 < evt.versions().size() ) {
            last_particle_index = evt.versions()[i+1].first_particle_index - 1;
            last_vertex_index   = evt.versions()[i+1].first_vertex_index - 1;
        }

        // Print all particles/vertices deleted in this version
        typedef std::pair<int,int> ___int_int_pair___;

        BOOST_FOREACH( const ___int_int_pair___ &del, deleted_barcodes ) {

            if(del.first>(int)i+1) {
                deleted_barcodes.erase( deleted_barcodes.begin(), deleted_barcodes.find(del) );
                break;
            }

            if( del.second > 0 ) m_file << "P "<<del.second<<" X"<<endl;
            else                 m_file << "V "<<del.second<<" X"<<endl;
        }

        // Print particles
        for( int j=v.first_particle_index; j<=last_particle_index; ++j ) {

            const GenParticle &p = evt.particles()[j];

            // Ignore particles that were added and removed in the same version
            if( p.version_deleted() == i+1 ) continue;

            // Check to see if we need to write a vertex first
            int production_vertex = p.data().production_vertex;

            if( production_vertex < 0 ) {
                GenVertex &v = evt.vertex( production_vertex );

                production_vertex = v.serialization_barcode();

                if (production_vertex < lowest_vertex_barcode) {
                    write_vertex(v);
                    if( v.version_deleted()<255 && !v.has_new_version() ) {
                        deleted_barcodes.insert( pair<int,int>( v.version_deleted(),v.barcode() ) );
                    }
                }

                ++vertices_processed;
                lowest_vertex_barcode = v.barcode();
            }

            // Check if this is a new version of some other particle
            int old_version = 0;

            BOOST_FOREACH( const ___int_int_pair___ &ver, evt.version_links() ) {

                if( ver.first == p.barcode() ) {
                    old_version = ver.second;
                    break;
                }
            }

            if( old_version ) write_particle( p, old_version, true  );
            else              write_particle( p, production_vertex,    false );

            if( p.version_deleted()<255 && !p.has_new_version() ) {
                deleted_barcodes.insert( pair<int,int>( p.version_deleted(),p.barcode() ) );
            }
        }

        // Write any remaining vertices
        while( vertices_processed<last_vertex_index ) {
            GenVertex &v = evt.vertices()[vertices_processed];

            int barcode = v.serialization_barcode();

            if( barcode < 0 ) {
                write_vertex( v );
                if( v.version_deleted()<255 && !v.has_new_version() ) {
                    deleted_barcodes.insert( pair<int,int>( v.version_deleted(),v.barcode() ) );
                }
            }

            ++vertices_processed;
            lowest_vertex_barcode = v.barcode();
        }
    }
}

bool IO_GenEvent::fill_next_event(GenEvent &evt) {
    if ( m_file.rdstate() ) return 0;
    if ( m_mode != std::ios::in ) {
        ERROR( "IO_GenEvent: attempting to read from output file" )
        return 0;
    }

    WARNING( "IO_GenEvent: Reading not implemented (yet)" )

    return 1;
}

void IO_GenEvent::write_vertex(const GenVertex &v) {

    m_file << "V " << v.barcode()
           << " [";

    BOOST_FOREACH( const GenParticle *p, v.particles_in() ) {
        m_file << p->barcode();
        if( p != v.particles_in().back() ) m_file << ",";
    }

    m_file << "] ";

    m_file << "@ 0 0 0 0";
    m_file << endl;
}

void IO_GenEvent::write_particle(const GenParticle &p, int second_field, bool is_new_version) {
    std::ios_base::fmtflags orig = m_file.flags();
    std::streamsize prec = m_file.precision();
    m_file.setf(std::ios::scientific, std::ios::floatfield);
    m_file.precision(m_precision);

    m_file << "P "<< p.barcode();

    if ( is_new_version ) m_file << " X" << second_field;
    else                  m_file << " "  << second_field;

    m_file << " " << p.pdg_id()
           << " " << p.momentum().px()
           << " " << p.momentum().py()
           << " " << p.momentum().pz()
           << " " << p.momentum().e()
           << " " << p.generated_mass()
           << " " << p.status()
           << endl;

    m_file.flags(orig);
    m_file.precision(prec);
}

} // namespace HepMC3

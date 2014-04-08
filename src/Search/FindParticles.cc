/**
 *  @file FindParticles.cc
 *  @brief Implementation of \b class HepMC3::FindParticles
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 1 April 2014 </b>
 */
#include <boost/foreach.hpp>
#include "HepMC3/Search/FindParticles.h"
#include "HepMC3/Search/Filter.h"
#include "HepMC3/Search/FilterList.h"
#include "HepMC3/GenEventVersion.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

FindParticles::FindParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list):
m_event(evt) {

    BOOST_FOREACH( const GenEventVersion &v, m_event.versions() ) {
        for( unsigned int i=0; i<v.particles().size(); ++i ) {
            GenParticle &p = v.particles()[i];
            if( passed_all_filters(p,filter_list) ) {
                if( filter_type == FIND_LAST && m_results.size()>0 ) m_results.clear();
                m_results.push_back(&p);

                if( filter_type == FIND_FIRST ) return;
            }
        }
    }
}

FindParticles::FindParticles(const GenEvent &evt, const GenParticle &p, FilterParticle filter_type, FilterList filter_list):
m_event(evt) {

    switch(filter_type) {
        case FIND_ALL_ANCESTORS:
            if( !p.production_vertex() ) break;

            recursive_check_ancestors(m_event.get_vertex(p.production_vertex()), filter_list);
            break;
        case FIND_ALL_DESCENDANTS:
            if( !p.end_vertex() ) break;

            recursive_check_descendants(m_event.get_vertex(p.end_vertex()), filter_list);
            break;
        case FIND_MOTHERS:
            if( !p.production_vertex() ) break;

            BOOST_FOREACH( int p_barcode, m_event.get_vertex(p.production_vertex()).particles_in() ) {
                const GenParticle &p = m_event.get_particle(p_barcode);
                if( passed_all_filters(p,filter_list) ) {
                    m_results.push_back(&p);
                }
            }
            break;
        case FIND_DAUGHTERS:
            if( !p.end_vertex() ) break;

            BOOST_FOREACH( int p_barcode, m_event.get_vertex(p.end_vertex()).particles_out() ) {
                const GenParticle &p = m_event.get_particle(p_barcode);
                if( passed_all_filters(p,filter_list) ) {
                    m_results.push_back(&p);
                }
            }
            break;
    };

}

void FindParticles::narrow_down( FilterList filter_list ) {

    int first_null = -1;

    // cost-efficient removing of particles that didn't pass filters
    for(unsigned int i=0; i<m_results.size(); ++i) {

        if( !passed_all_filters(*m_results[i],filter_list) ) {

            m_results[i] = NULL;

            if( first_null < 0 ) first_null = i;
        }
        else {
            if( first_null >= 0 ) {
                m_results[first_null] = m_results[i];
                m_results[i] = NULL;
                ++first_null;
            }
        }
    }

    if( first_null >= 0 ) m_results.resize( first_null );
}

bool FindParticles::passed_all_filters(const GenParticle &p, FilterList &filter_list) {
    if( filter_list.filters().size() == 0 ) return true;

    BOOST_FOREACH( const Filter &f, filter_list.filters() ) {
        if( f.passed_filter(m_event,p) == false ) return false;
    }

    DEBUG( 10, "Filter: passed" )
    return true;
}

void FindParticles::recursive_check_ancestors(const GenVertex &v, FilterList &filter_list) {

    BOOST_FOREACH( int p_barcode, v.particles_in() ) {
        const GenParticle &p = m_event.get_particle(p_barcode);

        if( passed_all_filters(p,filter_list) ) {
            m_results.push_back(&p);
        }

        if( !p.production_vertex() ) continue;
        recursive_check_ancestors(m_event.get_vertex(p.production_vertex()),filter_list);
    }
}

void FindParticles::recursive_check_descendants(const GenVertex &v, FilterList &filter_list) {

    BOOST_FOREACH( int p_barcode, v.particles_out() ) {
        const GenParticle &p = m_event.get_particle(p_barcode);

        if( passed_all_filters(p,filter_list) ) {
            m_results.push_back(&p);
        }
        if( !p.end_vertex() ) continue;
        recursive_check_descendants(m_event.get_vertex(p.end_vertex()),filter_list);
    }
}

} // namespace HepMC3

/**
 *  @file FindParticles.cc
 *  @brief Implementation of \b class HepMC3::FindParticles
 *
 */
#include "HepMC3/Search/FindParticles.h"
#include "HepMC3/Search/FilterList.h"
#include "HepMC3/Search/Filter.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

#include <boost/foreach.hpp>

namespace HepMC3 {

FindParticles::FindParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list) {

    for( unsigned int i=0; i<evt.particles_count(); ++i ) {

        GenParticle &p = evt.particle(i+1);
        if( passed_all_filters(p,filter_list) ) {
            if( filter_type == FIND_LAST ) m_results.clear();

            m_results.push_back( &p );

            if( filter_type == FIND_FIRST ) return;
        }
    }
}

FindParticles::FindParticles(const GenParticle &p, FilterParticle filter_type, FilterList filter_list) {

    switch(filter_type) {
        case FIND_ALL_ANCESTORS:
            if( !p.production_vertex() ) break;

            recursive_check_ancestors( *p.production_vertex(), filter_list );
            break;
        case FIND_ALL_DESCENDANTS:
            if( !p.end_vertex() ) break;

            recursive_check_descendants( *p.end_vertex(), filter_list );
            break;
        case FIND_MOTHERS:
            if( !p.production_vertex() ) break;

            BOOST_FOREACH( GenParticle *p_in, p.production_vertex()->particles_in() ) {

                if( passed_all_filters(*p_in,filter_list) ) {
                    m_results.push_back( p_in );
                }
            }
            break;
        case FIND_DAUGHTERS:
            if( !p.end_vertex() ) break;

            BOOST_FOREACH( GenParticle *p_out, p.end_vertex()->particles_out() ) {

                if( passed_all_filters(*p_out,filter_list) ) {
                    m_results.push_back( p_out );
                }
            }
            break;
        case FIND_PRODUCTION_SIBLINGS:
            if( !p.end_vertex() ) break;

            BOOST_FOREACH( GenParticle *p_in, p.end_vertex()->particles_in() ) {

                if( passed_all_filters(*p_in,filter_list) ) {
                    m_results.push_back( p_in );
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

            if( first_null < 0 ) first_null = i;
        }
        else {
            if( first_null >= 0 ) {
                m_results[first_null] = m_results[i];
                ++first_null;
            }
        }
    }

    if( first_null >= 0 ) m_results.resize( first_null );
}

bool FindParticles::passed_all_filters(const GenParticle &p, FilterList &filter_list) {
    if( filter_list.filters().size() == 0 ) return true;

    BOOST_FOREACH( const Filter &f, filter_list.filters() ) {
        if( f.passed_filter(p) == false ) return false;
    }

    DEBUG( 10, "Filter: passed" )
    return true;
}

void FindParticles::recursive_check_ancestors(const GenVertex &v, FilterList &filter_list) {

    BOOST_FOREACH( GenParticle *p_in, v.particles_in() ) {

        if( passed_all_filters(*p_in,filter_list) ) {
            m_results.push_back(p_in);
        }

        if( !p_in->production_vertex() ) continue;
        recursive_check_ancestors( *p_in->production_vertex(), filter_list );
    }
}

void FindParticles::recursive_check_descendants(const GenVertex &v, FilterList &filter_list) {

    BOOST_FOREACH(GenParticle *p_out, v.particles_out() ) {

        if( passed_all_filters(*p_out,filter_list) ) {
            m_results.push_back(p_out);
        }

        if( !p_out->end_vertex() ) continue;
        recursive_check_descendants( *p_out->end_vertex(), filter_list );
    }
}

} // namespace HepMC3

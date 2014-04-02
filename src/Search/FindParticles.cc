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

FindParticles::FindParticles(const GenEvent *evt, FilterEvent filter_type, FilterList filter_list) {

    BOOST_FOREACH( GenEventVersion *v, evt->versions() ) {
        BOOST_FOREACH( GenParticle *p, v->particles() ) {

            if( passed_all_filters(p,filter_list) ) {

                if( filter_type == FIND_LAST && m_results.size()>0 ) m_results.clear();
                m_results.push_back(p);

                if( filter_type == FIND_FIRST ) return;
            }
        }
    }
}

FindParticles::FindParticles(const GenParticle *p, FilterParticle filter_type, FilterList filter_list) {

    if(!p) return;

    if( filter_type == FIND_ANCESTORS ) {
        recursive_check_ancestors(p->production_vertex(), filter_list);
    }
    else if( filter_type == FIND_DESCENDANTS ) {
        recursive_check_descendants(p->end_vertex(), filter_list);
    }
}

bool FindParticles::passed_all_filters(GenParticle *p, FilterList &filter_list) {
    BOOST_FOREACH( const Filter &f, filter_list.filters() ) {
        if( f.passed_filter(p) == false ) return false;
    }
    return true;
}

void FindParticles::recursive_check_ancestors(GenVertex *v, FilterList &filter_list) {

    if(!v) return;

    BOOST_FOREACH( GenParticle *p, v->particles_in() ) {

        if( passed_all_filters(p,filter_list) ) m_results.push_back(p);
        recursive_check_ancestors(p->production_vertex(),filter_list);
    }
}

void FindParticles::recursive_check_descendants(GenVertex *v, FilterList &filter_list) {
    if(!v) return;

    BOOST_FOREACH( GenParticle *p, v->particles_out() ) {

        if( passed_all_filters(p,filter_list) ) m_results.push_back(p);
        recursive_check_descendants(p->end_vertex(),filter_list);
    }
}

} // namespace HepMC3

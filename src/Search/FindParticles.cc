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
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

FindParticles::FindParticles(const GenEvent *evt, FilterResult r, FilterList l):
m_event(evt),
m_result_count(r),
m_filter_list(l) {

    BOOST_FOREACH( GenEventVersion *v, evt->versions() ) {

        BOOST_FOREACH( GenParticle *p, v->particles() ) {

            bool result = true;

            BOOST_FOREACH( Filter &f, m_filter_list.filters() ) {

                result = f.passed_filter(p);
                if( !result ) break;

            }
            if( !result ) continue;

            if( r == FIND_LAST && m_results.size()>0 ) m_results.clear();
            m_results.push_back(p);

            if( r == FIND_FIRST ) return;
        }
    }
}

FindParticles::~FindParticles() {
}

} // namespace HepMC3

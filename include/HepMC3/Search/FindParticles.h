#ifndef  HEPMC3_SEARCH_FINDPARTICLES_H
#define  HEPMC3_SEARCH_FINDPARTICLES_H
/**
 *  @file FindParticles.h
 *  @brief Definition of \b class HepMC3::FindParticles
 *
 *  @class HepMC3::FindParticles
 *  @brief Search engine for HepMC3::GenEvent class
 *  
 *  @ingroup search_engine
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include <vector>
#include "HepMC3/Search/FilterList.h"
using std::vector;

namespace HepMC3 {

class GenParticle;
class GenEvent;
class GenVertex;

/** List of methods of searching through all particles in the event */
enum FilterEvent {
    FIND_ALL,
    FIND_FIRST,
    FIND_LAST
};

/** List of methods of searching starting from particle pointer */
enum FilterParticle {
    FIND_ANCESTORS,
    FIND_DESCENDANTS
};

class FindParticles {
//
// Constructors
//
public:
    /** HepMC3::GenEvent-based constructor */
    FindParticles(const GenEvent *evt,  FilterEvent    filter_type, FilterList filter_list = FilterList() );

    /** HepMC3::GenParticle-based constructor */
    FindParticles(const GenParticle *p, FilterParticle filter_type, FilterList filter_list = FilterList() );

    /** Narrow down the results applying additional filters */
    void narrow_down( FilterList filter_list );
//
// Functions
//
private:
    /** Check if particle passed all filters */
    bool passed_all_filters(GenParticle *p, FilterList &filter_list);

    /** Check if all ancestors passed the filter
     *  Recursively check all particles and production vertices of these particles
     */
    void recursive_check_ancestors(GenVertex *v, FilterList &filter_list);

    /** Check if all descendants passed the filter
     *  Recursively check all particles and end vertices of these particles
     */
    void recursive_check_descendants(GenVertex *v, FilterList &filter_list);
//
// Accessors
//
public:
    const vector<GenParticle*>& results() const { return m_results; } //!< Get results

//
// Fields
//
private:
    vector<GenParticle*>  m_results; //!< List of results
};

} // namespace HepMC3

#endif

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
 */
#include "HepMC3/Search/FilterList.h"

#include <vector>
using std::vector;

namespace HepMC3 {

class GenEvent;
class GenVertex;
class GenParticle;

/** List of methods of searching through all particles in the event */
enum FilterEvent {
    FIND_ALL,
    FIND_FIRST,
    FIND_LAST
};

/** List of methods of searching starting from particle pointer */
enum FilterParticle {
    FIND_ALL_ANCESTORS,
    FIND_ALL_DESCENDANTS,
    FIND_MOTHERS,
    FIND_DAUGHTERS
};

class FindParticles {
//
// Constructors
//
public:
    /** HepMC3::GenEvent-based constructor */
    FindParticles(const GenEvent &evt, FilterEvent filter_type, FilterList filter_list = FilterList() );

    /** HepMC3::GenParticle-based constructor */
    FindParticles(const GenEvent &evt, const GenParticle &p, FilterParticle filter_type, FilterList filter_list = FilterList() );

    /** Narrow down the results applying additional filters */
    void narrow_down( FilterList filter_list );
//
// Functions
//
private:
    /** Check if particle passed all filters */
    bool passed_all_filters(const GenParticle &p, FilterList &filter_list);

    /** Check if all ancestors passed the filter
     *  Recursively check all particles and production vertices of these particles
     */
    void recursive_check_ancestors(const GenVertex &v, FilterList &filter_list);

    /** Check if all descendants passed the filter
     *  Recursively check all particles and end vertices of these particles
     */
    void recursive_check_descendants(const GenVertex &v, FilterList &filter_list);
//
// Accessors
//
public:
    const vector<const GenParticle*>& results() const { return m_results; } //!< Get results

//
// Fields
//
private:
    const GenEvent             &m_event;   //!< Event
    vector<const GenParticle*>  m_results; //!< List of results
};

} // namespace HepMC3

#endif

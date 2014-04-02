#ifndef  HEPMC3_SEARCH_FINDPARTICLES_H
#define  HEPMC3_SEARCH_FINDPARTICLES_H
/**
 *  @file FindParticles.h
 *  @brief Definition of \b class HepMC3::FindParticles
 *
 *  @class HepMC3::FindParticles
 *  @brief Search engine for HepMC3::GenEvent class
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 1 April 2014 </b>
 */
#include <vector>
#include "HepMC3/Search/FilterList.h"
using std::vector;

namespace HepMC3 {

class GenParticle;

enum FilterResult {
    INVALID_RESULT = 0,
    FIND_ALL       = 1,
    FIND_FIRST     = 2,
    FIND_LAST      = 4
};

class GenEvent;

class FindParticles {
//
// Constructors
//
public:
    /** Default constructor */
    FindParticles(const GenEvent *evt, FilterResult r, FilterList l);

    /** Default destructor */
    ~FindParticles();

//
// Functions
//
public:
    const vector<GenParticle*>& results() const { return m_results; }
//
// Accessors
//
public:

//
// Fields
//
private:
    const GenEvent       *m_event;
    FilterResult          m_result_count;
    FilterList            m_filter_list;
    vector<GenParticle*>  m_results;
};

} // namespace HepMC3

#endif

#ifndef  HEPMC3_SEARCH_FILTERLIST_H
#define  HEPMC3_SEARCH_FILTERLIST_H
/**
 *  @file FilterList.h
 *  @brief Definition of \b class HepMC3::FilterList
 *
 *  @class HepMC3::FilterList
 *  @brief List of filters for the search engine
 *
 *  Constructs lists out of HepMC3::Filter objects
 *
 *  @ingroup search_engine
 *
 *  @date Created       <b> 2 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include <vector>
#include "HepMC3/Search/Filter.h"
using std::vector;

namespace HepMC3 {

class FilterList {
//
// Constructors
//
public:
    /** Default constructor
     *  Used when no filters were passed to search enginge
     */
    FilterList() {}

    /** Single filter constructor
     *  Used as casting operator when just one filter
     *  was passed to search engine
     */
    FilterList(const Filter &f1);

    /** Two filters constructor
     *  Used by global HepMC3::Filter AND operator to quickly join pair of filters
     */
    FilterList(const Filter &f1, const Filter &f2);

    /** AND operator
     *  Used to join multiple filters
     */
    FilterList& operator&&(const Filter &f);

//
// Accessors
//
public:
    const vector<Filter>& filters() { return m_filters; } //!< Get list of filters

//
// Fields
//
private:
    vector<Filter> m_filters; //!< List of filters
};

/** Filter AND operator
 *  Defined so that HepMC3::FilterList can be constructed
 *  when there is more than one filter passed to search engine
 */
FilterList operator&&(const Filter &f1, const Filter &f2);

} // namespace HepMC3

#endif

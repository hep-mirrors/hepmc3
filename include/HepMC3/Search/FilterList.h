#ifndef  HEPMC3_SEARCH_FILTERLIST_H
#define  HEPMC3_SEARCH_FILTERLIST_H
/**
 *  @file FilterList.h
 *  @brief Definition of \b class HepMC3::FilterList
 *
 *  @class HepMC3::FilterList
 *  @brief List of filters for the search engine
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 1 April 2014 </b>
 */
#include <vector>
#include "HepMC3/Search/Filter.h"
using std::vector;

namespace HepMC3 {

class FilterList {
public:
    FilterList() {}
    FilterList(const Filter f1);
    FilterList(const Filter f1, const Filter f2);

    FilterList operator&&(const Filter f);

    vector<Filter>& filters() { return m_filters; }
private:
    vector<Filter> m_filters;
};

FilterList operator&&(const Filter f1, const Filter f2);

} // namespace HepMC3

#endif

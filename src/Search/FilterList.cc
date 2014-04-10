/**
 *  @file FilterList.cc
 *  @brief Implementation of \b class HepMC3::FilterList
 *
 */
#include "HepMC3/Search/FilterList.h"
#include "HepMC3/Search/Filter.h"

namespace HepMC3 {

FilterList::FilterList(const Filter &f1) {
    m_filters.push_back(f1);
}

FilterList::FilterList(const Filter &f1, const Filter &f2) {
    m_filters.push_back(f1); m_filters.push_back(f2);
}

FilterList& FilterList::operator&&(const Filter &f) {
    m_filters.push_back(f);
    return *this;
}

FilterList operator&&(const Filter &f1, const Filter &f2) {
    return FilterList(f1,f2);
}

} // namespace HepMC3

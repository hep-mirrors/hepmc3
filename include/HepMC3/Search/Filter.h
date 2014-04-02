#ifndef  HEPMC3_SEARCH_FILTER_H
#define  HEPMC3_SEARCH_FILTER_H
/**
 *  @file Filter.h
 *  @brief Definition of \b class HepMC3::Filter
 *
 *  @class HepMC3::Filter
 *  @brief Filter for the search engine
 *
 *  Objects of this class cannot be directly created by user.
 *  Use predefined static const HepMC3::FilterBase objects to construct
 *  appropriate filters. See @ref search_engine for more details.
 *
 *  @ingroup search_engine
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include "HepMC3/Search/FilterBase.h"

namespace HepMC3 {

class GenParticle;

class Filter : public FilterBase {

friend class FilterBase; // To allow call to protected constructor

//
// Constructors
//
protected:
    /** Internal constructor for integer-type filters
     *  Invoked when an operator == != < > <= or >= with integer value
     *  is used on a filter.
     */
    Filter(FilterIntegerParam p, FilterOperator o, int value):FilterBase(p),m_operator(o),m_int_value(value) {}

//
// Functions
//
public:
    /** Check if HepMC3::genParticle passed this filter */
    bool passed_filter(const GenParticle *p) const;

private:
    /** HepMC3::Filter::passed_filter helper for integer-type filters */
    bool passed_int_filter(const GenParticle *p) const;

    /** HepMC3::Filter::passed_filter helper for pointer-type filters */
    bool passed_particle_pointer_filter(const GenParticle *p) const;

//
// Fields
//
private:
    FilterOperator   m_operator;                //!< Operator used by filter

    union {
        GenParticle *m_particle_pointer_value;  //!< HepMC3::GenParticle pointer used as a filter parameter (if pointer-type filter)
        int          m_int_value;               //!< Integer value used as a filter parameter (if integer-type filter)
    };
};

} // namespace HepMC3

#endif

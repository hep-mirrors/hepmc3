#ifndef  HEPMC_SEARCH_FILTER_H
#define  HEPMC_SEARCH_FILTER_H
/**
 *  @file Filter.h
 *  @brief Definition of \b class Filter
 *
 *  @class HepMC::Filter
 *  @brief Class used to define filters for search engine
 *
 *  Objects of this class cannot be directly created by user.
 *  Use predefined static const HepMC::FilterBase objects to construct
 *  appropriate filters. See @ref search_engine for more details.
 *
 *  @ingroup search_engine
 *
 */
#include "HepMC/Search/FilterBase.h"
#include "HepMC/Data/SmartPointer.h"

namespace HepMC {

class GenEvent;

class Filter : protected FilterBase {

friend class FilterBase; // To allow call to protected constructor

//
// Constructors
//
protected:
    /** @brief Internal constructor for integer-type filters
     *
     *  Invoked when an operator == != < > <= or >= with integer value
     *  is used on a filter.
     */
    Filter(FilterIntegerParam p, FilterOperator o, int value):FilterBase(p),m_operator(o),m_int_value(value) {}

    /** @brief Internal constructor for boolean-type filters
     *
     *  Used to initialize global static const filters and to create
     *  new filters using operator '!'
     */
    Filter(FilterBoolParam p, bool value = true):FilterBase(p),m_operator(EQUAL),m_bool_value(value) {}

//
// Functions
//
public:
    /** @brief Check if HepMC::GenParticle passed this filter */
    bool passed_filter(const GenParticlePtr &p) const;

    /** @brief Logical NOT operator for boolean filters */
    Filter operator!() const { return Filter(m_bool,!m_bool_value); }

private:
    /** @brief Filter::passed_filter helper for integer-type filters */
    bool passed_int_filter(const GenParticlePtr &p) const;

    /** @brief Filter::passed_filter helper for pointer-type filters */
    bool passed_bool_filter(const GenParticlePtr &p) const;

//
// Fields
//
private:
    FilterOperator   m_operator;                //!< Operator used by filter

    union {
        int  m_int_value;               //!< Integer value used as a filter parameter (if integer-type filter)
        bool m_bool_value;              //!< Boolean value used as a filter parameter (if boolean-type filter)
    };
};

static const Filter HAS_END_VERTEX           = FilterBase::init_has_end_vertex();           //!< Filter for checking if HepMC::GenParticle::end_vertex()        != NULL
static const Filter HAS_PRODUCTION_VERTEX    = FilterBase::init_has_production_vertex();    //!< Filter for checking if HepMC::GenParticle::production_vertex() != NULL
static const Filter HAS_SAME_PDG_ID_DAUGHTER = FilterBase::init_has_same_pdg_id_daughter(); //!< Filter for checking if end vertex has particle with same pdg_id

} // namespace HepMC

#endif

#ifndef  HEPMC3_SEARCH_FILTERBASE_H
#define  HEPMC3_SEARCH_FILTERBASE_H
/**
 *  @file FilterBase.h
 *  @brief Definition of \b class HepMC3::FilterBase
 *
 *  @class HepMC3::FilterBase
 *  @brief Base for the HepMC3::Filter class
 *
 *  Objects of this class cannot be directly created by user.
 *  See @ref search_engine for details on how to use this class.
 *  
 *  @ingroup search_engine
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
namespace HepMC3 {

class GenParticle;
class Filter;

class FilterBase {
//
// Enums
//
protected:

/** List of filter parameter types */
enum FilterParamType {
    INTEGER_PARAM,
    PARTICLE_POINTER_PARAM
};

/** List of filters with integer parameter */
enum FilterIntegerParam {
    STATUS,
    STATUS_SUBCODE,
    VERSION_CREATED,
    VERSION_DELETED
};

/** List of filters with particle pointer parameter */
enum FilterParticlePointerParam {
    HAS_END_VERTEX,
    HAS_PRODUCTION_VERTEX
};

/** List of operators on filter values */
enum FilterOperator {
    EQUAL,
    GREATER,
    GREATER_OR_EQUAL,
    LESS,
    LESS_OR_EQUAL,
    NOT_EQUAL
};

//
// Const initializers
//
public:
    static const FilterBase init_version_created() { return FilterBase(VERSION_CREATED); } //!< init static const VERSION_CREATED
    static const FilterBase init_version_deleted() { return FilterBase(VERSION_DELETED); } //!< init static const VERSION_CREATED
    static const FilterBase init_status()          { return FilterBase(STATUS);          } //!< init static const STATUS
    static const FilterBase init_status_subcode()  { return FilterBase(STATUS_SUBCODE);  } //!< init static const STATUS_SUBCODE

//
// Constructors
//
protected:

    /** Integer-type internal constructor
     *  Used by static const initializers to define filters.
     */
    FilterBase(FilterIntegerParam p):m_value_type(INTEGER_PARAM),m_int(p) {}

//
// Functions
//
public:
    Filter operator==(int value) const; //!< Create integer filter with operator ==
    Filter operator!=(int value) const; //!< Create integer filter with operator !=
    Filter operator> (int value) const; //!< Create integer filter with operator >
    Filter operator>=(int value) const; //!< Create integer filter with operator >=
    Filter operator< (int value) const; //!< Create integer filter with operator <
    Filter operator<=(int value) const; //!< Create integer filter with operator <=

//
// Fields
//
protected:
    FilterParamType                m_value_type;       //!< Parameter type

    union {
        FilterParticlePointerParam m_particle_pointer; //!< Pointer value (if pointer parameter type)
        FilterIntegerParam         m_int;              //!< Integer value (if integer parameter type)
    };
};

static const FilterBase STATUS          = FilterBase::init_status();           //!< Filter base for filtering HepMC3::GenParticle::status()
static const FilterBase STATUS_SUBCODE  = FilterBase::init_status_subcode();   //!< Filter base for filtering HepMC3::GenParticle::status_subcode()
static const FilterBase VERSION_CREATED = FilterBase::init_version_created();  //!< Filter base for filtering HepMC3::GenParticle::version_created()
static const FilterBase VERSION_DELETED = FilterBase::init_version_deleted();  //!< Filter base for filtering HepMC3::GenParticle::version_deleted()

} // namespace HepMC3

#endif

#ifndef  HEPMC3_SEARCH_FILTERBASE_H
#define  HEPMC3_SEARCH_FILTERBASE_H
/**
 *  @file FilterBase.h
 *  @brief Definition of \b class HepMC3::FilterBase
 *
 *  @class HepMC3::FilterBase
 *  @brief Base class for HepMC3::Filter class
 *
 *  Objects of this class cannot be directly created by user.
 *  See @ref search_engine for details on how this class is used.
 *
 *  @ingroup search_engine
 *
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
    BOOL_PARAM
};

/** List of filters with integer parameter */
enum FilterIntegerParam {
    STATUS,
    STATUS_SUBCODE,
    VERSION_CREATED,
    VERSION_DELETED,
    PDG_ID,
    ABS_PDG_ID
};

/** List of filters boolean parameter */
enum FilterBoolParam {
    HAS_END_VERTEX,
    HAS_PRODUCTION_VERTEX,
    HAS_SAME_PDG_ID_DAUGHTER
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
    static const FilterBase init_pdg_id()          { return FilterBase(PDG_ID);          } //!< init static const PDG_ID
    static const FilterBase init_abs_pdg_id()      { return FilterBase(ABS_PDG_ID);      } //!< init static const ABS_PDG_ID

    static const Filter     init_has_end_vertex();           //!< init static const HAS_END_VERTEX
    static const Filter     init_has_production_vertex();    //!< init static const HAS_PRODUCTION_VERTEX
    static const Filter     init_has_same_pdg_id_daughter(); //!< init static const HAS_SAME_PDG_ID_DAUGHTER

//
// Constructors
//
protected:

    /** Integer-type internal constructor
     *  Used by static const initializers to define filters.
     */
    FilterBase(FilterIntegerParam p):m_value_type(INTEGER_PARAM),m_int(p) {}

    /** Boolean-type internal constructor
     *  Used by static const initializers to define filters.
     */
    FilterBase(FilterBoolParam p):m_value_type(BOOL_PARAM),m_bool(p) {}
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
        FilterIntegerParam  m_int;  //!< Integer value (if integer parameter type)
        FilterBoolParam     m_bool; //!< Boolean value (if boolean parameter type)
    };
};

static const FilterBase STATUS          = FilterBase::init_status();           //!< Filter base for filtering HepMC3::GenParticle::status()
static const FilterBase STATUS_SUBCODE  = FilterBase::init_status_subcode();   //!< Filter base for filtering HepMC3::GenParticle::status_subcode()
static const FilterBase VERSION_CREATED = FilterBase::init_version_created();  //!< Filter base for filtering HepMC3::GenParticle::version_created()
static const FilterBase VERSION_DELETED = FilterBase::init_version_deleted();  //!< Filter base for filtering HepMC3::GenParticle::version_deleted()
static const FilterBase PDG_ID          = FilterBase::init_pdg_id();           //!< Filter base for filtering HepMC3::GenParticle::pdg_id()
static const FilterBase ABS_PDG_ID      = FilterBase::init_abs_pdg_id();       //!< Filter base for filtering abs( HepMC3::GenParticle::pdg_id() )

} // namespace HepMC3

#endif

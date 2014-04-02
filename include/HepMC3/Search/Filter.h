#ifndef  HEPMC3_SEARCH_FILTER_H
#define  HEPMC3_SEARCH_FILTER_H
/**
 *  @file Filter.h
 *  @brief Definition of \b class HepMC3::Filter
 *
 *  @class HepMC3::Filter
 *  @brief Filters for the search engine
 *
 *  New objects of this class cannot be created. Use the list of predefined
 *  filters to create specific filters out of them.
 *  See search tutorial for more details
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 1 April 2014 </b>
 */
namespace HepMC3 {

class GenParticle;

class Filter {

//
// Enums
//
/** enum FilterIntegerParam
 *  Used for filtering integer type parameters.
 *  This will be translated to proper check when filter is used
 */
enum FilterIntegerParam {
    STATUS,
    STATUS_SUBCODE,
    VERSION_CREATED,
    VERSION_DELETED
};

/** enum FilterParticlePointerParam
 *  Used for filtering based on particle pointer.
 *  This will be translated to proper check when filter is used
 */
enum FilterParticlePointerParam {
    IS_DESCENDANT
};

/** enum FilterParamType
 *  Used to determine the type of the filter.
 */
enum FilterParamType {
    INTEGER_PARAM,
    PARTICLE_POINTER_PARAM
};

/** enum FilterOperator
 *  This will be translated to proper operator when filter is used.
 *  Also used to store the operator the filter is going to use.
 */
enum FilterOperator {
    INVALID_OPERATOR,
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
    static const Filter init_version_created() { return Filter(VERSION_CREATED); } //!< init static const VERSION_CREATED
    static const Filter init_version_deleted() { return Filter(VERSION_DELETED); } //!< init static const VERSION_CREATED
    static const Filter init_status()          { return Filter(STATUS);          } //!< init static const STATUS
    static const Filter init_status_subcode()  { return Filter(STATUS_SUBCODE);  } //!< init static const STATUS_SUBCODE

//
// Constructors
//
private:

    /** Integer-type constructor
     *  Used by static const initializers to define filters.
     *  Such filters are then used to create proper filters throug operators,
     *  Eg. STATUS >= 2
     */
    Filter(FilterIntegerParam p):m_value_type(INTEGER_PARAM),m_operator(INVALID_OPERATOR),m_int(p),m_int_value(0) {}

    /** Integer-type internal constructor
     *  Invoked when an operator == != < > <= or >= is used on a filter.
     */
    Filter(FilterIntegerParam p, FilterOperator o, int value):m_value_type(INTEGER_PARAM),m_operator(o),m_int(p),m_int_value(value) {}

//
// Functions
//
public:
    Filter operator==(int value) const { return Filter(m_int,EQUAL,           value); } //!< Create integer filter with operator ==
    Filter operator!=(int value) const { return Filter(m_int,NOT_EQUAL,       value); } //!< Create integer filter with operator !=
    Filter operator> (int value) const { return Filter(m_int,GREATER,         value); } //!< Create integer filter with operator >
    Filter operator>=(int value) const { return Filter(m_int,GREATER_OR_EQUAL,value); } //!< Create integer filter with operator >=
    Filter operator< (int value) const { return Filter(m_int,LESS,            value); } //!< Create integer filter with operator <
    Filter operator<=(int value) const { return Filter(m_int,LESS_OR_EQUAL,   value); } //!< Create integer filter with operator <=

    bool passed_filter(const GenParticle *p) const;
    bool is_particle_pointer_type() const               { return m_value_type==PARTICLE_POINTER_PARAM; }


private:
    bool passed_int_filter(const GenParticle *p) const;
    bool passed_particle_pointer_filter(const GenParticle *p) const;

//
// Fields
//
private:
    FilterParamType  m_value_type;
    FilterOperator   m_operator;

    union {
        FilterParticlePointerParam m_particle_pointer;
        FilterIntegerParam         m_int;
    };

    union {
        GenParticle *m_particle_pointer_value;
        int          m_int_value;
    };
};

static const Filter STATUS          = Filter::init_status();
static const Filter STATUS_SUBCODE  = Filter::init_status_subcode();
static const Filter VERSION_CREATED = Filter::init_version_created();
static const Filter VERSION_DELETED = Filter::init_version_deleted();

} // namespace HepMC3

#endif

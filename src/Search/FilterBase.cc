/**
 *  @file FilterBase.cc
 *  @brief Definition of \b class HepMC3::FilterBase
 *
 *  @date Created       <b> 2 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include "HepMC3/Search/Filter.h"
#include "HepMC3/Search/FilterBase.h"

namespace HepMC3 {

// Define operators for integer-type filters
Filter FilterBase::operator==(int value) const { return Filter(m_int,EQUAL,           value); }
Filter FilterBase::operator!=(int value) const { return Filter(m_int,NOT_EQUAL,       value); }
Filter FilterBase::operator> (int value) const { return Filter(m_int,GREATER,         value); }
Filter FilterBase::operator>=(int value) const { return Filter(m_int,GREATER_OR_EQUAL,value); }
Filter FilterBase::operator< (int value) const { return Filter(m_int,LESS,            value); }
Filter FilterBase::operator<=(int value) const { return Filter(m_int,LESS_OR_EQUAL,   value); }

// Define initializators for boolean-type filters
const Filter FilterBase::init_has_end_vertex()           { return Filter(HAS_END_VERTEX);           }
const Filter FilterBase::init_has_production_vertex()    { return Filter(HAS_PRODUCTION_VERTEX);    }
const Filter FilterBase::init_has_same_pdg_id_daughter() { return Filter(HAS_SAME_PDG_ID_DAUGHTER); }

} // namespace HepMC3

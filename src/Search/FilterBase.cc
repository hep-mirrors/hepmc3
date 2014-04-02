/**
 *  @file FilterBase.cc
 *  @brief Definition of operators of \b class HepMC3::FilterBase
 *
 *  @date Created       <b> 2 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include "HepMC3/Search/Filter.h"
#include "HepMC3/Search/FilterBase.h"

namespace HepMC3 {

Filter FilterBase::operator==(int value) const { return Filter(m_int,EQUAL,           value); }
Filter FilterBase::operator!=(int value) const { return Filter(m_int,NOT_EQUAL,       value); }
Filter FilterBase::operator> (int value) const { return Filter(m_int,GREATER,         value); }
Filter FilterBase::operator>=(int value) const { return Filter(m_int,GREATER_OR_EQUAL,value); }
Filter FilterBase::operator< (int value) const { return Filter(m_int,LESS,            value); }
Filter FilterBase::operator<=(int value) const { return Filter(m_int,LESS_OR_EQUAL,   value); }

} // namespace HepMC3

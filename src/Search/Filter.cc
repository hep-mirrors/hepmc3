/**
 *  @file Filter.cc
 *  @brief Implementation of \b class HepMC3::Filter
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include "HepMC3/Search/Filter.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

bool Filter::passed_filter(const GenParticle *p) const {
        if(!p) return false;

        if( m_value_type == INTEGER_PARAM )          return passed_int_filter(p);
        if( m_value_type == PARTICLE_POINTER_PARAM ) return passed_particle_pointer_filter(p);

        // This should never happen
        ERROR( "Unsupported value type ("<<m_value_type<<")" )
        return false;
}

bool Filter::passed_int_filter(const GenParticle *p) const {

    int value = 0;

    switch(m_int) {
        case STATUS:          value = p->status();          break;
        case STATUS_SUBCODE:  value = p->status_subcode();  break;
        case VERSION_CREATED: value = p->version_created(); break;
        case VERSION_DELETED: value = p->version_deleted(); break;
        case PDG_ID:          value = p->pdg_id();          break;
        default:
            // This should never happen
            ERROR( "Unsupported filter ("<<m_int<<")" )
            return false;
    };

    DEBUG( 10, "Filter: checking barcode="<<p->barcode()<<" param="<<m_int<<" operator="<<m_operator<<" value="<<value<<" compare to="<<m_int_value )

    switch(m_operator) {
        case EQUAL:            return (value==m_int_value);
        case GREATER:          return (value> m_int_value);
        case LESS:             return (value< m_int_value);
        case GREATER_OR_EQUAL: return (value>=m_int_value);
        case LESS_OR_EQUAL:    return (value<=m_int_value);
        case NOT_EQUAL:        return (value!=m_int_value);
    };

    return false;
}

bool Filter::passed_particle_pointer_filter(const GenParticle *p) const {
    return false;
}

} // namespace HepMC3

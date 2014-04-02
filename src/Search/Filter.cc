/**
 *  @file Filter.cc
 *  @brief Implementation of \b class HepMC3::Filter
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 1 April 2014 </b>
 */
#include "HepMC3/Search/Filter.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

bool Filter::passed_filter(const GenParticle *p) const {
        if(!p) return false;

        if( m_value_type == INTEGER_PARAM )          return passed_int_filter(p);
        if( m_value_type == PARTICLE_POINTER_PARAM ) return passed_particle_pointer_filter(p);

        // This will never happen
        return false;
}

bool Filter::passed_int_filter(const GenParticle *p) const {

    int value = 0;

    switch(m_int) {
        case STATUS:          value = p->status();          break;
        case STATUS_SUBCODE:  value = p->status_subcode();  break;
        case VERSION_CREATED: value = p->version_created(); break;
        case VERSION_DELETED: value = p->version_deleted(); break;
        default:
            // This will never happen
            return false;
    };

    DEBUG( 1, "Filter: checking barcode="<<p->barcode()<<" p="<<m_int<<" o="<<m_operator<<" v="<<value<<" r="<<m_int_value )

    switch(m_operator) {
        case EQUAL:            return (value==m_int_value);
        case GREATER:          return (value> m_int_value);
        case LESS:             return (value< m_int_value);
        case GREATER_OR_EQUAL: return (value>=m_int_value);
        case LESS_OR_EQUAL:    return (value<=m_int_value);
        case NOT_EQUAL:        return (value!=m_int_value);
        default:
            // This will never happen
            return false;
    };

    return false;
}

bool Filter::passed_particle_pointer_filter(const GenParticle *p) const {
    return false;
}

} // namespace HepMC3

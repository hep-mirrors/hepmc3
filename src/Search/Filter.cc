/**
 *  @file Filter.cc
 *  @brief Implementation of \b class HepMC3::Filter
 *
 */
#include "HepMC3/Search/Filter.h"

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Log.h"

#include <boost/foreach.hpp>

namespace HepMC3 {

bool Filter::passed_filter(const GenParticle &p) const {

        if( m_value_type == INTEGER_PARAM ) return passed_int_filter (p);
        if( m_value_type == BOOL_PARAM    ) return passed_bool_filter(p);

        // This should never happen
        ERROR( "Unsupported value type ("<<m_value_type<<")" )
        return false;
}

bool Filter::passed_int_filter(const GenParticle &p ) const {

    int value = 0;

    switch(m_int) {
        case STATUS:          value = p.m_data.status;          break;
        case STATUS_SUBCODE:  value = p.m_data.status_subcode;  break;
        case VERSION_CREATED: value = p.m_version_created;      break;
        case VERSION_DELETED: value = p.m_version_deleted;      break;
        case PDG_ID:          value = p.m_data.pdg_id;          break;
        case ABS_PDG_ID:      value = abs( p.m_data.pdg_id );   break;
        default:
            // This should never happen
            ERROR( "Unsupported filter ("<<m_int<<")" )
            return false;
    };

    DEBUG( 10, "Filter: checking barcode="<<p.barcode()<<" param="<<m_int<<" operator="<<m_operator<<" value="<<value<<" compare to="<<m_int_value )

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

bool Filter::passed_bool_filter(const GenParticle &p ) const {

    bool       result = false;
    GenVertex *buf    = NULL;

    DEBUG( 10, "Filter: checking barcode="<<p.barcode()<<" param="<<m_bool<<" value="<<m_bool_value<<" (bool)" )

    switch( m_bool ) {
        case HAS_END_VERTEX:           result = (p.m_end_vertex_barcode != 0); break;
        case HAS_PRODUCTION_VERTEX:    result = (p.m_data.ancestor      != 0); break;
        case HAS_SAME_PDG_ID_DAUGHTER:
            buf = p.end_vertex();
            if( !buf ) {
                result = false;
                break;
            }

            if( buf->particles_out().size() == 0 ) {
                result = false;
                break;
            }

            BOOST_FOREACH( GenParticle *p_out, buf->particles_out() ) {

                if( p_out->m_data.pdg_id == p.m_data.pdg_id ) {
                    result = true;
                    break;
                }
            }

            break;
    };

    if( m_bool_value == false ) result = !result;

    return result;
}

} // namespace HepMC3

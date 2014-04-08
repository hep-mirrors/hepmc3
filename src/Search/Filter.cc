/**
 *  @file Filter.cc
 *  @brief Implementation of \b class HepMC3::Filter
 *
 *  @date Created       <b> 1 April 2014 </b>
 *  @date Last modified <b> 2 April 2014 </b>
 */
#include <boost/foreach.hpp>
#include "HepMC3/Search/Filter.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Log.h"

namespace HepMC3 {

bool Filter::passed_filter(const GenEvent &evt, const GenParticle &p) const {

        if( m_value_type == INTEGER_PARAM ) return passed_int_filter(evt, p);
        if( m_value_type == BOOL_PARAM    ) return passed_bool_filter(evt, p);

        // This should never happen
        ERROR( "Unsupported value type ("<<m_value_type<<")" )
        return false;
}

bool Filter::passed_int_filter(const GenEvent &evt, const GenParticle &p) const {

    int value = 0;

    switch(m_int) {
        case STATUS:          value = p.status();          break;
        case STATUS_SUBCODE:  value = p.status_subcode();  break;
        case VERSION_CREATED: value = p.version_created(); break;
        case VERSION_DELETED: value = p.version_deleted(); break;
        case PDG_ID:          value = p.pdg_id();          break;
        case ABS_PDG_ID:      value = abs( p.pdg_id() );   break;
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

bool Filter::passed_bool_filter(const GenEvent &evt, const GenParticle &p) const {

    bool result = false;

    DEBUG( 10, "Filter: checking barcode="<<p.barcode()<<" param="<<m_bool<<" value="<<m_bool_value<<" (bool)" )

    switch( m_bool ) {
        case HAS_END_VERTEX:           result = (p.end_vertex()        != 0); break;
        case HAS_PRODUCTION_VERTEX:    result = (p.production_vertex() != 0); break;
        case HAS_SAME_PDG_ID_DAUGHTER:
            if( !p.end_vertex() ) {
                result = false;
                break;
            }
            
            const GenVertex &v = evt.get_vertex(p.end_vertex());
            if( v.particles_out().size() == 0 ) {
                result = false;
                break;
            }

            BOOST_FOREACH( int p_barcode, v.particles_out() ) {
                if( evt.get_particle(p_barcode).pdg_id() == p.pdg_id() ) {
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

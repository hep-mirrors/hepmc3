// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file Filter.cc
 *  @brief Implementation of \b class Filter
 *
 */
#include "HepMC/Search/Filter.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/Setup.h"

#include "HepMC/foreach.h"

namespace HepMC {

bool Filter::passed_filter(const GenParticlePtr &p) const {

        if( m_value_type == INTEGER_PARAM ) return passed_int_filter (p);
        if( m_value_type == BOOL_PARAM    ) return passed_bool_filter(p);

        // This should never happen
        ERROR( "Unsupported value type ("<<m_value_type<<")" )
        return false;
}

bool Filter::passed_int_filter(const GenParticlePtr &p ) const {

    int value = 0;

    switch(m_int) {
        case STATUS:          value = p->status();          break;
        case STATUS_SUBCODE:  value = p->status_subcode();  break;
        case PDG_ID:          value = p->pdg_id();          break;
        case ABS_PDG_ID:      value = abs( p->pdg_id() );   break;
        default:
            // This should never happen
            ERROR( "Unsupported filter ("<<m_int<<")" )
            return false;
    };

    DEBUG( 10, "Filter: checking id="<<p->id()<<" param="<<m_int<<" operator="<<m_operator<<" value="<<value<<" compare to="<<m_int_value )

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

bool Filter::passed_bool_filter(const GenParticlePtr &p ) const {

    bool         result = false;
    GenVertexPtr buf;

    DEBUG( 10, "Filter: checking id="<<p->id()<<" param="<<m_bool<<" value="<<m_bool_value<<" (bool)" )

    switch( m_bool ) {
        case HAS_END_VERTEX:           result = p->end_vertex();        break;
        case HAS_PRODUCTION_VERTEX:    result = p->production_vertex(); break;
        case HAS_SAME_PDG_ID_DAUGHTER:
            buf = p->end_vertex();
            if( !buf ) {
                result = false;
                break;
            }

            if( buf->particles_out().size() == 0 ) {
                result = false;
                break;
            }

            FOREACH( const GenParticlePtr &p_out, buf->particles_out() ) {

                if( p_out->pdg_id() == p->pdg_id() ) {
                    result = true;
                    break;
                }
            }

            break;
    };

    if( m_bool_value == false ) result = !result;

    return result;
}

} // namespace HepMC

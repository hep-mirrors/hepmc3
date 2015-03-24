// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenVertex.cc
 *  @brief Implementation of \b class GenVertex
 *
 */
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Setup.h"
#include "HepMC/Attribute.h"

#include "HepMC/foreach.h"

namespace HepMC {

GenVertex::GenVertex( const FourVector& position ):
m_event(NULL),
m_id(0) {
    m_data.position = position;
}

GenVertex::GenVertex( const GenVertexData &data):
m_event(NULL),
m_id(0),
m_data(data) {
}

void GenVertex::add_particle_in( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    FOREACH( const GenParticlePtr &pp, particles_in() ) {
        if( pp == p ) return;
    }

    m_particles_in.push_back(p);

    p->set_end_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}

void GenVertex::add_particle_out( const GenParticlePtr &p ) {
    if(!p) return;

    // Avoid duplicates
    FOREACH( const GenParticlePtr &pp, particles_out() ) {
        if( pp == p ) return;
    }

    m_particles_out.push_back(p);

    p->set_production_vertex( m_this.lock() );

    if(m_event) m_event->add_particle(p);
}

const FourVector& GenVertex::position() const {

    if( !m_data.position.is_zero() ) return m_data.position;

    // No position information - search ancestors
    FOREACH( const GenParticlePtr &p, particles_in() ) {
        const GenVertexPtr &v = p->production_vertex();
        if(v) return v->position();
    }

    return FourVector::ZERO_VECTOR();
}

void GenVertex::set_position(const FourVector& new_pos) {
    m_data.position = new_pos;
}

bool GenVertex::add_attribute(std::string name, shared_ptr<Attribute> att) {
  if ( !parent_event() ) return false;
  parent_event()->add_attribute(name, att, id());
  return true;
}

void GenVertex::remove_attribute(std::string name) {
  if ( parent_event() ) parent_event()->remove_attribute(name, id());
}

} // namespace HepMC

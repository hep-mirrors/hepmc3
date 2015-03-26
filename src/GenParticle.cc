// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file GenParticle.cc
 *  @brief Implementation of \b class GenParticle
 *
 */
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Setup.h"
#include "HepMC/Attribute.h"

namespace HepMC {

GenParticle::GenParticle( const FourVector &momentum, int pdg_id, int status ):
m_event(NULL),
m_id(0) {
    m_data.pdg_id            = pdg_id;
    m_data.momentum          = momentum;
    m_data.status            = status;
    m_data.is_mass_set       = false;
    m_data.mass              = 0.0;
}

GenParticle::GenParticle( const GenParticleData &data ):
m_event(NULL),
m_id(0),
m_data(data) {
}

double GenParticle::generated_mass() const {
    if(m_data.is_mass_set) return m_data.mass;
    else                   return m_data.momentum.m();
}

void GenParticle::set_pdg_id(int pdg_id) {
    m_data.pdg_id = pdg_id;
}

void GenParticle::set_status(int status) {
    m_data.status = status;
}

void GenParticle::set_momentum(const FourVector& momentum) {
    m_data.momentum = momentum;
}

void GenParticle::set_generated_mass(double m) {
    m_data.mass        = m;
    m_data.is_mass_set = true;
}

void GenParticle::unset_generated_mass() {
    m_data.mass        = 0.;
    m_data.is_mass_set = false;
}

GenVertexPtr GenParticle::production_vertex() {
    return m_production_vertex.lock();
}

const GenVertexPtr GenParticle::production_vertex() const {
    return m_production_vertex.lock();
}

GenVertexPtr GenParticle::end_vertex() {
    return m_end_vertex.lock();
}

const GenVertexPtr GenParticle::end_vertex() const {
    return m_end_vertex.lock();
}

bool GenParticle::add_attribute(std::string name, shared_ptr<Attribute> att) {
  if ( !parent_event() ) return false;
  parent_event()->add_attribute(name, att, id());
  return true;
}

void GenParticle::remove_attribute(std::string name) {
  if ( parent_event() ) parent_event()->remove_attribute(name, id());
}

} // namespace HepMC

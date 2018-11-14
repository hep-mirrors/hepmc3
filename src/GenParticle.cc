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
#include "HepMC/Search/FindParticles.h"
#include "HepMC/Setup.h"
#include "HepMC/Attribute.h"

namespace HepMC {


GenParticle::GenParticle( const FourVector &mom, int pidin, int stat):
m_event(nullptr),
m_event_const(nullptr),
m_id(0) {
    m_data.pid               = pidin;
    m_data.momentum          = mom;
    m_data.status            = stat;
    m_data.is_mass_set       = false;
    m_data.mass              = 0.0;
}

GenParticle::GenParticle( const GenParticleData &dat ):
m_event(nullptr),
m_event_const(nullptr),
m_id(0),
m_data(dat) {
}

double GenParticle::generated_mass() const {
    if(m_data.is_mass_set) return m_data.mass;
    else                   return m_data.momentum.m();
}

void GenParticle::set_pid(int pidin) {
    m_data.pid = pidin;
}

void GenParticle::set_status(int stat) {
    m_data.status = stat;
}

void GenParticle::set_momentum(const FourVector& mom) {
    m_data.momentum = mom;
}

void GenParticle::set_generated_mass(double m) {
    m_data.mass        = m;
    m_data.is_mass_set = true;
}

void GenParticle::unset_generated_mass() {
    m_data.mass        = 0.;
    m_data.is_mass_set = false;
}

void GenParticle::checkValidity(weak_ptr<GenVertex> vtx, weak_ptr<const GenVertex> vtx_const)const{
  // If the vertex has been set from a const vertex then there is no
  // non-const version to return.  In principle we could simply
  // access that default empty ptr, but that might be confusing and hard to debug
  // I think throwing here will make any debugging much easier, since
  // We can tell people to set the vertex via the non const version
  // or request only the const version.
  if(vtx.expired() && ! vtx_const.expired()){
    throw std::runtime_error("You have requested access to a non-const GenVertex from a GenParticle whose vertex has been set from a const GenVertex.  Either set the GenVertex using a non-const, or request a ConstGenVertex instead.");
  }
  return;
}
  
GenVertexPtr GenParticle::production_vertex() {
    checkValidity(m_production_vertex, m_production_vertex_const);
    return m_production_vertex.lock();
}

ConstGenVertexPtr GenParticle::production_vertex() const {
    return m_production_vertex_const.lock();
}

GenVertexPtr GenParticle::end_vertex() {
    checkValidity(m_end_vertex, m_end_vertex_const);
    return m_end_vertex.lock();
}

ConstGenVertexPtr GenParticle::end_vertex() const {
    return m_end_vertex_const.lock();
}

vector<GenParticlePtr> GenParticle::parents(){
    checkValidity(m_production_vertex, m_production_vertex_const);
    return (m_production_vertex.expired())? vector<GenParticlePtr>() : m_production_vertex.lock()->particles_in();
}

vector<ConstGenParticlePtr> GenParticle::parents() const{
    return (m_production_vertex_const.expired()) ? vector<ConstGenParticlePtr>() : m_production_vertex_const.lock()->particles_in();
}
  
vector<GenParticlePtr> GenParticle::children(){
    checkValidity(m_end_vertex, m_end_vertex_const);
    return (m_end_vertex.expired())? vector<GenParticlePtr>() : m_end_vertex.lock()->particles_out();
}

vector<ConstGenParticlePtr> GenParticle::children() const{
    return (m_end_vertex_const.expired()) ? vector<ConstGenParticlePtr>() : m_end_vertex_const.lock()->particles_out();
}

vector<GenParticlePtr> GenParticle::ancestors() {
  checkValidity(m_production_vertex, m_production_vertex_const);
  return (m_production_vertex.expired()) ? vector<GenParticlePtr>() : findParticles(m_production_vertex.lock(), ANCESTORS);
}

vector<ConstGenParticlePtr> GenParticle::ancestors() const {
  return (m_production_vertex_const.expired()) ? vector<ConstGenParticlePtr>() : findParticles(m_production_vertex_const.lock(), ANCESTORS);
}

vector<GenParticlePtr> GenParticle::descendants() {
  checkValidity(m_end_vertex, m_end_vertex_const);
  return (m_end_vertex.expired()) ? vector<GenParticlePtr>() : findParticles(m_end_vertex.lock(), DESCENDANTS);
}

vector<ConstGenParticlePtr> GenParticle::descendants() const {
  return (m_end_vertex_const.expired()) ? vector<ConstGenParticlePtr>() : findParticles(m_end_vertex_const.lock(), DESCENDANTS);
}

void GenParticle::set_gen_event(GenEvent *evt){
  m_event = evt;
  m_event_const = evt;
  return;
}

void GenParticle::set_gen_event(const GenEvent *evt){
  m_event = 0;
  m_event_const = evt;
  return;
}

void GenParticle::set_id(int id){
  m_id = id;
  return;
}

void GenParticle::set_production_vertex(GenVertexPtr vtx){
  m_production_vertex = vtx;
  m_production_vertex_const = std::const_pointer_cast<const GenVertex>(vtx);
  return;
}
 
void GenParticle::set_production_vertex(ConstGenVertexPtr vtx){
  // In this case there cannot be access to the non-const version
  // Therefore we reset it (equiv to ptr=0 for raw ptrs)
  m_production_vertex.reset();
  m_production_vertex_const = vtx;
  return;
}
 
void GenParticle::set_end_vertex(GenVertexPtr vtx){
  m_end_vertex = vtx;
  m_end_vertex_const = std::const_pointer_cast<const GenVertex>(vtx);
  return;
}
 
void GenParticle::set_end_vertex(ConstGenVertexPtr vtx){
  // In this case there cannot be access to the non-const version
  // Therefore we reset it (equiv to ptr=0 for raw ptrs)
  m_end_vertex.reset();
  m_end_vertex_const = vtx;
  return;
}
 
bool GenParticle::add_attribute(std::string name, shared_ptr<Attribute> att) {
  if ( !parent_event() ) return false;
  parent_event()->add_attribute(name, att, id());
  return true;
}

vector<string> GenParticle::attribute_names() const {
  if ( parent_event() ) return parent_event()->attribute_names(id());

  return vector<string>();
}

void GenParticle::remove_attribute(std::string name) {
  if ( parent_event() ) parent_event()->remove_attribute(name, id());
}

string GenParticle::attribute_as_string(string name) const {
    return parent_event() ? parent_event()->attribute_as_string(name, id()) : string();
}

} // namespace HepMC

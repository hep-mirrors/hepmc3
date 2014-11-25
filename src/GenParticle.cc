// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
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

namespace HepMC {

GenParticle::GenParticle( const FourVector &momentum, int pdg_id, int status ):
m_event(NULL),
m_id(0) {
    m_data.pdg_id            = pdg_id;
    m_data.momentum          = momentum;
    m_data.status            = status;
    m_data.status_subcode    = 0;
    m_data.is_mass_set       = false;
    m_data.mass              = 0.0;
}

GenParticle::GenParticle( const GenParticleData &data ):
m_event(NULL),
m_id(0),
m_data(data) {
}

void GenParticle::print( std::ostream& ostr ) const {

    ostr << "GenParticle: ";
    ostr.width(3);
    ostr << id();
    ostr << " barcode: ";
    ostr.width(5);
    ostr << barcode() <<" PDGID: ";
    ostr.width(5);
    ostr << m_data.pdg_id;

    // Find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize         prec = ostr.precision();

    if(m_event) ostr.precision(m_event->print_precision());

    ostr.setf(std::ios::scientific, std::ios::floatfield);
    ostr.setf(std::ios_base::showpos);
    ostr << " (P,E)=" << m_data.momentum.px()
               << "," << m_data.momentum.py()
               << "," << m_data.momentum.pz()
               << "," << m_data.momentum.e();

    // Restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);

    int prod_vtx_id = (!m_production_vertex.expired()) ? m_production_vertex.lock()->id() : 0;
    int end_vtx_id  = (!m_end_vertex.expired())        ? m_end_vertex.lock()->id()        : 0;

    ostr << " Stat: " << m_data.status
         << " PV: " << prod_vtx_id
         << " EV: " << end_vtx_id
         << std::endl;
}

void GenParticle::print_event_listing( std::ostream& ostr ) const {
    ostr << " ";
    ostr.width(6);
    ostr << id();
    ostr.width(9);
    ostr << pdg_id() << " ";
    ostr.width(9);
    ostr.setf(std::ios::scientific, std::ios::floatfield);
    ostr.setf(std::ios_base::showpos);
    ostr << m_data.momentum.px() << ",";
    ostr.width(9);
    ostr << m_data.momentum.py() << ",";
    ostr.width(9);
    ostr << m_data.momentum.pz() << ",";
    ostr.width(9);
    ostr << m_data.momentum.e() << " ";
    ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
    ostr.unsetf(std::ios_base::showpos);
    ostr.width(3);
    ostr << m_data.status;
    if( m_data.status_subcode ) {
        ostr << "-";
        ostr.width(9);
        ostr << std::left << m_data.status_subcode << std::right;
    }
    else ostr << "          ";

    int prod_vtx_id = (!m_production_vertex.expired()) ? m_production_vertex.lock()->id() : 0;

    if( prod_vtx_id ) {
        ostr.width(6);
        ostr << prod_vtx_id;
    }

    ostr << std::endl;
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

void GenParticle::set_status_subcode(int subcode) {
    m_data.status_subcode = subcode;
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

GenVertexPtr GenParticle::production_vertex() const {
    return m_production_vertex.lock();
}

GenVertexPtr GenParticle::end_vertex() const {
    return m_end_vertex.lock();
}

} // namespace HepMC

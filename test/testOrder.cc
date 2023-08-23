// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/Attribute.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Relatives.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3TestUtils.h"
#include <thread>
#include <random>
#include <iterator>
using namespace HepMC3;
std::shared_ptr<GenEvent> generate() {
    std::shared_ptr<GenEvent> evt = std::make_shared<GenEvent>();
    std::shared_ptr<GenRunInfo> run = std::make_shared<GenRunInfo>();
    evt->set_run_info(run);
    GenParticlePtr b1 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr b2 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenParticlePtr b3 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  -32.238),   1,  3 );
    GenParticlePtr b4 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  -32.238),   1,  3 );
    GenParticlePtr b5 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  -32.238),   1,  3 );
    GenParticlePtr b6 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  -32.238),   1,  3 );
    GenVertexPtr v1 = std::make_shared<GenVertex>();
    v1->add_particle_in(b3);
    v1->add_particle_in(b4);
    v1->add_particle_out(b5);
    v1->add_particle_out(b6);
    evt->add_vertex(v1);
    evt->add_beam_particle(b1);
    evt->add_beam_particle(b2);
    GenVertexPtr v2 = std::make_shared<GenVertex>();
    GenVertexPtr v3 = std::make_shared<GenVertex>();
    v2->add_particle_out(b3);
    v2->add_particle_in(b1);
    v3->add_particle_out(b4);
    v3->add_particle_in(b2);
    evt->add_vertex(v2);
    evt->add_vertex(v3);
    return evt;
}


int main()
{
    auto evt = generate();
    WriterAscii W("testOrder.hepmc");
    W.write_event(*(evt.get()));
    W.close();
    ReaderAscii R("testOrder.hepmc");
    GenEvent e;
    R.read_event(e);
    R.close();
    return 0;
}

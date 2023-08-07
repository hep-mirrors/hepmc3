// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include  <ctime>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Relatives.h"
#include <cassert>
using namespace HepMC3;

int main() {
    std::vector<GenParticlePtr> b;
    GenParticlePtr b0 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   14000.0,  14000.0  ),2212,  3 );
    GenVertexPtr v1 = std::make_shared<GenVertex>();
    GenParticlePtr b1 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr b2 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenParticlePtr b3 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    v1->add_particle_in(b0);
    v1->add_particle_out(b1);
    v1->add_particle_out(b2);
    v1->add_particle_out(b3);
    GenParticlePtr p1 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr p2 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenVertexPtr v = std::make_shared<GenVertex>();
    v->add_particle_in (b3);
    v->add_particle_out(p1);
    v->add_particle_out(p2);
    b.push_back(b0);
    /** This code tests if the Relatives:: functions give the same result for the objects with ids!=0, i.e. those in
     * an event and the objects w/o ids, i.e. those outside of an event */
    GenEvent evt;
    auto desc1 = HepMC3::Relatives::DESCENDANTS(b.front());
    evt.add_tree(b);
    auto desc2 = HepMC3::Relatives::DESCENDANTS(b.front());
    if (desc1.size() != desc2.size()) return 1;
    if (desc2.size() == evt.particles().size() - 1) return 2;
    return 0;
}

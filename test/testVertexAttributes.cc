// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
GenEvent generate1() {
    GenEvent evt;
    std::shared_ptr<GenRunInfo> run = std::make_shared<GenRunInfo>();
    evt.set_run_info(run);
    GenParticlePtr b1 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
    GenParticlePtr b2 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenParticlePtr b3 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
    GenVertexPtr v1 = std::make_shared<GenVertex>();
    v1->add_particle_in (b1);
    v1->add_particle_in(b2);
    v1->add_particle_out(b3);
    evt.add_vertex(v1);
    for (size_t z= 0; z < 12; z++) {
        auto particles = evt.particles();
        for (auto p: particles) {
            if (p->end_vertex()) continue;
            GenParticlePtr p1 = std::make_shared<GenParticle>( FourVector( 0.0,    0.0,   7000.0,  7000.0  ),2212,  3 );
            GenParticlePtr p2 = std::make_shared<GenParticle>( FourVector( 0.750, -1.569,   32.191,  32.238),   1,  3 );
            GenVertexPtr v = std::make_shared<GenVertex>();
            v->add_particle_in (p);
            v->add_particle_out(p1);
            v->add_particle_out(p2);
            evt.add_vertex(v);
            v->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(v->id()));
            p1->add_attribute("phi",std::make_shared<HepMC3::DoubleAttribute>(0.1));
            p1->add_attribute("theta",std::make_shared<HepMC3::DoubleAttribute>(0.1));
            p1->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(p1->id()));
            p2->add_attribute("phi",std::make_shared<HepMC3::DoubleAttribute>(0.1));
            p2->add_attribute("theta",std::make_shared<HepMC3::DoubleAttribute>(0.1));
            p2->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(p2->id()));
        }
    }
    return evt;
}
int main() {
    GenEvent e = generate1();
    auto v1 = e.vertices().back();
    auto barcode1 = v1->attribute<IntAttribute>("barcode");
    auto val1 = barcode1?barcode1->value():-10001;
    e.remove_vertex(e.vertices()[e.vertices().size()/2]);
    auto v2 = e.vertices().back();
    auto barcode2 = v2->attribute<IntAttribute>("barcode");
    auto val2 = barcode2?barcode2->value():-10002;
    if (val1==val2) {
      return 0;
    }
    HepMC3::Print::line(v2,true);
    return 1;
}

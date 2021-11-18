// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include  <ctime>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
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
      v->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(-20));
      p1->add_attribute("phi",std::make_shared<HepMC3::DoubleAttribute>(0.1));
      p1->add_attribute("theta",std::make_shared<HepMC3::DoubleAttribute>(0.1));
      p1->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(10));
      p2->add_attribute("phi",std::make_shared<HepMC3::DoubleAttribute>(0.1));
      p2->add_attribute("theta",std::make_shared<HepMC3::DoubleAttribute>(0.1));
      p2->add_attribute("barcode",std::make_shared<HepMC3::IntAttribute>(10));
    }
   }
return evt;
}



GenEvent generate2() {
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
    std::vector<std::string> names; names.reserve(2048);
    std::vector<std::shared_ptr<Attribute> > atts; atts.reserve(2048);
    std::vector<int> ids; ids.reserve(2048);
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
      names.push_back("barcode");
      names.push_back("barcode");
      names.push_back("phi");
      names.push_back("theta");
      names.push_back("barcode");
      names.push_back("phi");
      names.push_back("theta");
      atts.push_back(std::make_shared<HepMC3::IntAttribute>(-20));
      atts.push_back(std::make_shared<HepMC3::DoubleAttribute>(0.1));
      atts.push_back(std::make_shared<HepMC3::DoubleAttribute>(0.1));
      atts.push_back(std::make_shared<HepMC3::IntAttribute>(10));
      atts.push_back(std::make_shared<HepMC3::DoubleAttribute>(0.1));
      atts.push_back(std::make_shared<HepMC3::DoubleAttribute>(0.1));
      atts.push_back(std::make_shared<HepMC3::IntAttribute>(10));
      ids.push_back(v->id());
      ids.push_back(p1->id());
      ids.push_back(p1->id());
      ids.push_back(p1->id());
      ids.push_back(p2->id());
      ids.push_back(p2->id());
      ids.push_back(p2->id());
    }
   }
   evt.add_attributes(names, atts, ids);
return evt;
}
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
int main()
{
 auto rawstart1= Clock::now();
for (int i = 0; i < 1000; i++) generate1();
    auto now1 = Clock::now();


  auto rawstart2 = Clock::now();

for (int i = 0; i < 1000; i++) generate2();
    auto now2 = Clock::now();
    
    
    
    
std::cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(now1-rawstart1).count() << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(now2-rawstart2).count()<< std::endl;

return 0;
}

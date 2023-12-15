// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include  <ctime>
#include  <list>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
using namespace HepMC3;

GenEvent generate(size_t N) {
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
    for (size_t z= 0; z < N; z++) {
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


void rem_part1(HepMC3::GenEvent& event)
{
  // This is two-staged, because if we remove elements while looping
  // over the list, then we can get problems.
  size_t n = 0;
  std::list<HepMC3::GenParticlePtr> toRemove;
  for (auto iter = event.particles().rbegin();
       iter != event.particles().rend(); ++iter) {
    if (n++ % 10 == 0) toRemove.emplace_back(*iter);
  }
  for (auto& p : toRemove) event.remove_particle(p);
}

void rem_part2(HepMC3::GenEvent& event)
{
  // This is two-staged, because if we remove elements while looping
  // over the list, then we can get problems.
  size_t n = 0;
  std::vector<HepMC3::GenParticlePtr> toRemove;
  toRemove.reserve(event.particles_size()/10 + 10 );
  for (auto& iter: event.particles()) {
    if (n++ % 10 == 0) toRemove.emplace_back(iter);
  }
  std::sort(toRemove.begin(),toRemove.end(), [](const auto& a, const auto& b) {return a->id()>b->id();});
  for (auto& p : toRemove) event.remove_particle(p);
}
void rem_part3(HepMC3::GenEvent& event)
{
  // This is two-staged, because if we remove elements while looping
  // over the list, then we can get problems.
  size_t n = 0;
  std::list<HepMC3::GenParticlePtr> toRemove;
  for (auto iter = event.particles().rbegin();
       iter != event.particles().rend(); ++iter) {
    if (n++ % 10 == 0) toRemove.emplace_back(*iter);
  }
  for (auto& p : toRemove) event.remove_particle(p);
}

#include <chrono>
using Clock =  std::chrono::high_resolution_clock;
int main()
{
    size_t N = 13;
    GenEvent X[5];
    X[1] = generate(N);
    X[2] = generate(N);
    X[3] = generate(N);
    printf("Particles: %i\n",X[1].particles_size());
    
    auto rawstart1= Clock::now();
    rem_part1(X[1]);
    auto now1 = Clock::now();

    auto rawstart2= Clock::now();
    rem_part2(X[2]);
    auto now2 = Clock::now();


    auto rawstart3= Clock::now();
    rem_part3(X[3]);
    auto now3 = Clock::now();



    std::cout<< std::chrono::duration_cast<std::chrono::milliseconds>(now1-rawstart1).count() << " "
             << std::chrono::duration_cast<std::chrono::milliseconds>(now2-rawstart2).count() << " "
             << std::chrono::duration_cast<std::chrono::milliseconds>(now3-rawstart3).count() << " "
             << std::endl;

    return 0;
}

// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include <fstream>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Print.h"
#include "HepMC3/PrintStreams.h"
using namespace HepMC3;
int main()
{
    GenVertexPtr vertex = std::make_shared<GenVertex>();
    ConstGenVertexPtr const_vertex = std::make_shared<const GenVertex>();
    GenParticlePtr particle = std::make_shared<GenParticle>(FourVector(1,2,3,4),1,2);
    ConstGenParticlePtr const_particle = std::make_shared<const GenParticle>(FourVector(1,2,3,4),1,2);

    std::cout << vertex << std::endl;
    std::cout << const_vertex << std::endl;

    std::cout << particle << std::endl;
    std::cout << const_particle << std::endl;

    return 0;
}

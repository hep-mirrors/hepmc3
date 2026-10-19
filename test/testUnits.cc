// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2026 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Verifies momentum and length unit conversions.
//
#include <iostream>

#include "HepMC3/Units.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
using namespace HepMC3;
double conversion_factor( Units::MomentumUnit from, Units::MomentumUnit  to )
{
    FourVector m( 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand());
    const FourVector msave(m);
    Units::convert(m,from,to );
    return m.e()/msave.e();//NAN?
}
double conversion_factor( Units::LengthUnit from, Units::LengthUnit  to )
{
    FourVector m( 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand());
    const FourVector msave(m);
    Units::convert(m,from,to );
    return m.e()/msave.e();//NAN?
}
bool neq(const double a,const double b)
{
    return std::abs(a-b) >= 0.001*(std::abs(a)+std::abs(b));
}
int main()
{

    int err = 0;
    double cf = 0;
    GenEvent evt;
    std::cout << "Default units: " << Units::name(evt.momentum_unit())
              << " " << Units::name(evt.length_unit()) << std::endl;

    // check momentum conversion factors
    cf = conversion_factor( Units::GEV, Units::GEV );
    if( neq(cf,1) )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for GEV to GEV - should be 1 \n";
    }
    cf =  conversion_factor( Units::MEV, Units::MEV );
    if( neq(cf,1)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MEV to MEV - should be 1 \n";
    }
    cf =  conversion_factor( Units::MEV, Units::GEV );
    if( neq(cf,0.001)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MEV to GEV - should be 0.001 \n";
    }
    cf =  conversion_factor( Units::GEV, Units::MEV );
    if( neq(cf,1000.0)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for GEV to MEV - should be 1000 \n";
    }

    // check length conversion factors
    cf =  conversion_factor( Units::MM, Units::MM );
    if( neq(cf,1)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MM to MM - should be 1 \n";
    }
    cf =  conversion_factor( Units::CM, Units::CM );
    if( neq(cf,1)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for CM to CM - should be 1 \n";
    }
    cf =  conversion_factor( Units::CM, Units::MM );
    if( neq(cf,10.0)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for CM to MM - should be 10 \n";
    }
    cf =  conversion_factor( Units::MM, Units::CM );
    if( neq(cf,0.1)  )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MM to CM - should be 0.1 \n";
    }

    // check that set_units also converts the event position (root vertex)
    GenEvent evt2(Units::GEV, Units::CM);
    GenParticlePtr beam = std::make_shared<GenParticle>(FourVector(0.0, 0.0, 7000.0, 7000.0), 2212, 4);
    GenVertexPtr v1 = std::make_shared<GenVertex>(FourVector(0.0, 0.0, 2.0, 0.0));
    v1->add_particle_in(beam);
    evt2.add_vertex(v1);
    evt2.shift_position_to(FourVector(0.0, 0.0, 1.0, 0.0));
    evt2.set_units(Units::GEV, Units::MM);
    if( neq(evt2.event_pos().z(), 10.0) )
    {
        ++err;
        std::cerr << "wrong event position " << evt2.event_pos().z()
                  << " after set_units from CM to MM - should be 10 \n";
    }
    if( neq(beam->production_vertex()->position().z(), 10.0) )
    {
        ++err;
        std::cerr << "wrong beam production vertex position " << beam->production_vertex()->position().z()
                  << " after set_units from CM to MM - should be 10 \n";
    }
    if( neq(v1->position().z(), 30.0) )
    {
        ++err;
        std::cerr << "wrong vertex position " << v1->position().z()
                  << " after set_units from CM to MM - should be 30 \n";
    }

    return err;
}

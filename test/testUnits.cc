//
// Test Units
//
#include <iostream>

#include "HepMC3/Units.h"
#include "HepMC3/GenEvent.h"
using namespace HepMC3;
double conversion_factor( Units::MomentumUnit from, Units::MomentumUnit  to )
{
    FourVector m( 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand());
    FourVector msave(m);
    Units::convert(m,from,to );
    return m.e()/msave.e();//NAN?
}
double conversion_factor( Units::LengthUnit from, Units::LengthUnit  to )
{
    FourVector m( 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand(), 0.5*RAND_MAX-std::rand());
    FourVector msave(m);
    Units::convert(m,from,to );
    return m.e()/msave.e();//NAN?
}
int main()
{

    int err = 0;
    double cf;
    GenEvent evt;
    std::cout << "Default units: " << Units::name(evt.momentum_unit())
              << " " << Units::name(evt.length_unit()) << std::endl;

    // check momentum conversion factors
    cf = conversion_factor( Units::GEV, Units::GEV );
    if( cf != 1 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for GEV to GEV - should be 1 \n";
    }
    cf =  conversion_factor( Units::MEV, Units::MEV );
    if( cf != 1 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MEV to MEV - should be 1 \n";
    }
    cf =  conversion_factor( Units::MEV, Units::GEV );
    if( cf != 0.001 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MEV to GEV - should be 0.001 \n";
    }
    cf =  conversion_factor( Units::GEV, Units::MEV );
    if( cf != 1000.0 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for GEV to MEV - should be 1000 \n";
    }

    // check length conversion factors
    cf =  conversion_factor( Units::MM, Units::MM );
    if( cf != 1 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MM to MM - should be 1 \n";
    }
    cf =  conversion_factor( Units::CM, Units::CM );
    if( cf != 1 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for CM to CM - should be 1 \n";
    }
    cf =  conversion_factor( Units::CM, Units::MM );
    if( cf != 10.0 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for CM to MM - should be 10 \n";
    }
    cf =  conversion_factor( Units::MM, Units::CM );
    if( cf != 0.1 )
    {
        ++err;
        std::cerr << "wrong conversion factor " << cf
                  << " for MM to CM - should be 0.1 \n";
    }

    return err;
}

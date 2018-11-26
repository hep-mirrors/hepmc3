//////////////////////////////////////////////////////////////////////////
// testWeights.cc
//
// garren@fnal.gov, January 2010
// test Weights
//////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "HepMC/GenRunInfo.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Print.h"
#include <stdexcept>
int main()
{
    HepMC::GenEvent evt;
    std::shared_ptr<HepMC::GenRunInfo> run = std::make_shared<HepMC::GenRunInfo>();;
    evt.set_run_info(run);
    // original functionality
    evt.weights().push_back(2.0);
    evt.weights().push_back(4.56);
    assert( evt.weights()[0] == 2.0 );
    assert( evt.weights()[1] == 4.56 );
    assert( evt.weights().size() == 2 );
    assert( !evt.weights().empty() );

    std::vector<double> vec;
    for( int i = 0; i < 15; ++i )
        {
            double x = (double)i + 0.14*(double)i;
            vec.push_back( x );
        }
    evt.weights() = vec;
    assert( evt.weights().size() == 15 );
    evt.weights().pop_back();
    assert( evt.weights().size() == 14 );

    // for asserts only
#ifndef NDEBUG
    double v1 = vec[0];
    size_t vs = vec.size();
#endif

    // new functionality
    std::vector<std::string> names;
    for( size_t i = 0; i < evt.weights().size(); ++i ) names.push_back(std::to_string(i));
    std::string nm = "tau";
    names.push_back(nm);
    run->set_weight_names(names);

    evt.weight(nm) = 3.1;
    assert( evt.weights().size() == (vs) );

    // lookup a nonexistent name
    try
        {
            double x = evt.weight("bad");
            std::cout << "lookup of nonexistent name returns " << x << std::endl;
        }
    catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "HepMC testWeights: the above error is intentional" << std::endl;
        }
    HepMC::Print::listing(evt);
    return 0;
}

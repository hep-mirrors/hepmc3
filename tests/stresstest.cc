/**
 * @brief A throwaway test of installation
 *
 * Based on HepMC2/examples/example_BuildEventFromScratch.cc
 */
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/Search/FindParticles.h"
#include "HepMC3/IO/IO_GenEvent.h"

#include <iostream>

#include <boost/foreach.hpp>
using namespace HepMC3;

int main() {
    //
    // In this example we will place the following event into HepMC "by hand"
    //
    //     name status pdg_id  parent Px       Py    Pz       Energy      Mass
    //  1  !p+!    3   2212    0,0    0.000    0.000 7000.000 7000.000    0.938
    //  2  !p+!    3   2212    0,0    0.000    0.000-7000.000 7000.000    0.938
    //=========================================================================
    //  3  !d!     3      1    1,1    0.750   -1.569   32.191   32.238    0.000
    //  4  !u~!    3     -2    2,2   -3.047  -19.000  -54.629   57.920    0.000
    //  5  !W-!    3    -24    1,2    1.517   -20.68  -20.605   85.925   80.799
    //  6  !gamma! 1     22    1,2   -3.813    0.113   -1.833    4.233    0.000
    //  7  !d!     1      1    5,5   -2.445   28.816    6.082   29.552    0.010
    //  8  !u~!    1     -2    5,5    3.962  -49.498  -26.687   56.373    0.006

    // now we build the graph, which will looks like
    //                       p7                         #
    // p1                   /                           #
    //   \v1__p3      p5---v4                           #
    //         \_v3_/       \                           #
    //         /    \        p8                         #
    //    v2__p4     \                                  #
    //   /            p6                                #
    // p2                                               #
    //                                                  #
    int count=0;
        IO_GenEvent file("test.hepmc",std::ios::out);

    while(++count<=10000) {
        if(count%100==0) std::cout<<"C: "<<count<<std::endl;
    GenEvent evt;

    GenParticle &p1= evt.new_particle( FourVector(0,0,7000,700),                2212, 3 );
    GenParticle &p2= evt.new_particle( FourVector(0,0,-7000,700),               2212, 3 );
    GenParticle &p3= evt.new_particle( FourVector(.750,-1.569,32.191,32.238),   1,    3 );
    GenParticle &p4= evt.new_particle( FourVector(-3.047,-19.,-54.629,57.920), -2,    3 );

    GenVertex &v1=evt.new_vertex();
    v1.add_particle_in ( p1 );
    v1.add_particle_out( p3 );

    GenVertex &v2=evt.new_vertex();
    v2.add_particle_in ( p2 );
    v2.add_particle_out( p4 );

    for(int i=0; i<1000; ++i) {
    GenVertex &v3=evt.new_vertex();
    v3.add_particle_in( p3 );
    v3.add_particle_in( p4 );
    

    GenParticle &p5= evt.new_particle( FourVector(-3.813,0.113,-1.833,4.233 ),   22, 1 );
    GenParticle &p6= evt.new_particle( FourVector(1.517,-20.68,-20.605,85.925), -24, 3 );

    v3.add_particle_out( p5 );
    v3.add_particle_out( p6 );
    
    GenVertex &v4=evt.new_vertex();
    v4.add_particle_in ( p6 );
        FindParticles search2(p5, FIND_ALL_ANCESTORS );
FindParticles search3(p4, FIND_ALL_DESCENDANTS, STATUS == 1 && !HAS_END_VERTEX );
    search3.narrow_down( PDG_ID >= -6 && PDG_ID <= 6 );
    

    GenParticle &p7= evt.new_particle( FourVector(-2.445,28.816,6.082,29.552),    1, 1 );
    GenParticle &p8= evt.new_particle( FourVector(3.962,-49.498,-26.687,56.373), -2, 1 );

    v4.add_particle_out( p7 );
    v4.add_particle_out( p8 );
    }

    
    file.set_precision(4);
    file.write_event(evt);
    
        FindParticles search(evt, FIND_ALL, STATUS == 1 && STATUS_SUBCODE == 0 );

    //evt.dump();  break;
    }

    return 0;
}
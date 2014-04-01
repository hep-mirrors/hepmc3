/**
 * @brief A throwaway test of installation
 *
 * Based on HepMC2/examples/example_BuildEventFromScratch.cc
 *
 * @author Tomasz Przedzinski
 * @date 20th March 2014
 */
#include <iostream>
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/GenParticle.h"
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

    GenEvent* evt = new GenEvent();
    //
    // create vertex 1 and vertex 2, together with their inparticles
    GenVertex* v1 = new GenVertex();
    v1->add_particle_in( new GenParticle( FourVector(0,0,7000,7000), 2212, 3 ) );
    GenVertex* v2 = new GenVertex();
    v2->add_particle_in( new GenParticle( FourVector(0,0,-7000,7000), 2212, 3 ) );
    //
    // create the outgoing particles of v1 and v2
    GenParticle* p3 = new GenParticle( FourVector(.750,-1.569,32.191,32.238), 1, 3 );
    v1->add_particle_out( p3 );
    GenParticle* p4 = new GenParticle( FourVector(-3.047,-19.,-54.629,57.920), -2, 3 );
    v2->add_particle_out( p4 );
    // add v1 & v2 to the event
    evt->add_vertex( v1 );
    evt->add_vertex( v2 );
    //
    // create v3
    GenVertex* v3 = new GenVertex();
    v3->add_particle_in( p3 );
    v3->add_particle_in( p4 );
    GenParticle* p5 = new GenParticle( FourVector(-3.813,0.113,-1.833,4.233 ), 22, 1 );
    GenParticle* p6 = new GenParticle( FourVector(1.517,-20.68,-20.605,85.925), -24,3);
    v3->add_particle_out( p5 );
    v3->add_particle_out( p6 );
    evt->add_vertex( v3 );
    //
    // create v4
    GenVertex* v4 = new GenVertex();
    v4->add_particle_in( p6 );
    GenParticle* p7 = new GenParticle( FourVector(-2.445,28.816,6.082,29.552), 1,1 );
    GenParticle* p8 = new GenParticle( FourVector(3.962,-49.498,-26.687,56.373), -2,1 );
    v4->add_particle_out( p7 );
    v4->add_particle_out( p8 );
    evt->add_vertex( v4 );

    // versioning test
    evt->create_new_version("Second tool");

    evt->delete_particle( p7 );

    GenParticle* p9 = new GenParticle( FourVector(1., 2., 3., 4.), 1,1 );
    evt->add_particle( p9 );
    v4->add_particle_out( p9 );

    evt->create_new_version("Third tool");

    evt->delete_particle(p4);

    // printout
    evt->set_current_version(0);
    evt->print();
    evt->set_current_version(1);
    evt->print();
    evt->set_current_version(evt->last_version());
    evt->print();

    std::cout<<"Testing few errors: "<<std::endl;

    GenParticle* p10 = new GenParticle( FourVector(5., 4., 3., 2.), 1,1 );
    evt->add_particle(p10);
    v4->add_particle_out(p10);
    v4->add_particle_in(p10);

    // now clean-up by deleting all objects from memory
    //
    // deleting the event deletes all contained vertices, and all particles
    // contained in those vertices
    delete evt;

    return 0;
}

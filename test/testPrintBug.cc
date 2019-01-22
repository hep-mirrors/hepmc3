//
// Thanks to Bob McElrath and Frank Siegert for this test
// andrii.verbytskyi@mpp.mpg.gov, Nov. 2018

#include <fstream>

#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/Print.h"

int main()
{
    HepMC::GenEvent p_event(HepMC::Units::GEV, HepMC::Units::MM);
    for(int i=0; i<10; i++)
        {
            HepMC::FourVector vector(1.0,1.0,1.0,1.0);
            HepMC::GenVertexPtr vertex=std::make_shared<HepMC::GenVertex>();
            vertex->set_position(vector);
            vertex->set_id(i);
            for(int j=0; j<3; j++)
                {
                    HepMC::GenParticlePtr particle = std::make_shared<HepMC::GenParticle>(vector,1,2);
                    vertex->add_particle_in(particle);
                }
            for(int j=0; j<3; j++)
                {
                    HepMC::GenParticlePtr particle = std::make_shared<HepMC::GenParticle>(vector,1,2);
                    vertex->add_particle_out(particle);
                }
            p_event.add_vertex(vertex);
        }
    HepMC::Print::listing(p_event);
    // cleanup
    p_event.clear();
    return 0;
}

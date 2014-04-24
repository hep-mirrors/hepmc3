#ifndef SIMPLE_EVENT_TOOL_H
#define SIMPLE_EVENT_TOOL_H

#ifdef HEPMC2
#include "HepMC/IO_GenEvent.h"
#else
#include "HepMC3/IO/IO_GenEvent.h"
#include "HepMC3/IO/IO_HepMC2_adapter.h"
using namespace HepMC3;

#endif // ifdef HEPMC2

#include "ValidationTool.h"

class SimpleEventTool : public ValidationTool {
public:
    SimpleEventTool() {}

    const std::string name()   { return "SimpleEventTool"; }
    bool tool_modifies_event() { return true; }

    void initialize() {}
    void finalize()   {}

    int process(GenEvent &hepmc) {
        // Create some four vectors for the electrons
        double ele_mass_sqr = 0.000511*0.000511;
        FourVector momentum_e1;
        FourVector momentum_e2;

        momentum_e1.setPz( -2);
        momentum_e2.setPz(3.5);

        momentum_e1.setE(sqrt(momentum_e1.pz()*momentum_e1.pz()+ele_mass_sqr));
        momentum_e2.setE(sqrt(momentum_e2.pz()*momentum_e2.pz()+ele_mass_sqr));

        FourVector momentum_tau1(+1.38605041e+00,+1.38605041e+00,+7.50000000e-01,+2.75000005e+00);
        FourVector momentum_tau2(-1.38605041e+00,-1.38605041e+00,+7.50000000e-01,+2.75000005e+00);

        // Make particles
        GenParticle &e1   = hepmc.new_particle( momentum_e1,   -11, 2 );
        GenParticle &e2   = hepmc.new_particle( momentum_e2,    11, 2 );
        GenParticle &tau1 = hepmc.new_particle( momentum_tau1, -15, 1 );
        GenParticle &tau2 = hepmc.new_particle( momentum_tau2,  15, 1 );

        // Set masses
        e1.  set_generated_mass(0.000511);
        e2.  set_generated_mass(0.000511);
        tau1.set_generated_mass(1.777);
        tau2.set_generated_mass(1.777);

        // Make vertex
        GenVertex &vertex = hepmc.new_vertex();
        vertex.add_particle_in(e1);
        vertex.add_particle_in(e2);
        vertex.add_particle_out(tau1);
        vertex.add_particle_out(tau2);

        return 0;
    }
};

#endif

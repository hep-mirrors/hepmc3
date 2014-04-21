using namespace Tauolapp;

/** Create a simple HepMC3::GenEvent
 ** e+ + e- -> tau+ tau -
 **/
HepMC3::GenEvent * tauola_make_simple_event(){

    HepMC3::GenEvent * event = new HepMC3::GenEvent();

    //Create some four vectors for the electrons
    double ele_mass_sqr = parmas_.amell*parmas_.amell;
    HepMC3::FourVector momentum_e1;
    HepMC3::FourVector momentum_e2;

    momentum_e1.setPz( -2);
    momentum_e2.setPz(3.5);

    momentum_e1.setE(sqrt(momentum_e1.pz()*momentum_e1.pz()+ele_mass_sqr));
    momentum_e2.setE(sqrt(momentum_e2.pz()*momentum_e2.pz()+ele_mass_sqr));

    HepMC3::FourVector momentum_tau1(+1.38605041e+00,+1.38605041e+00,+7.50000000e-01,+2.75000005e+00);
    HepMC3::FourVector momentum_tau2(-1.38605041e+00,-1.38605041e+00,+7.50000000e-01,+2.75000005e+00);

    //make particles
    HepMC3::GenParticle &e1   = event->new_particle( momentum_e1,   -11, 3 );
    HepMC3::GenParticle &e2   = event->new_particle( momentum_e2,    11, 3 );
    HepMC3::GenParticle &tau1 = event->new_particle( momentum_tau1, -15, 1 );
    HepMC3::GenParticle &tau2 = event->new_particle( momentum_tau2,  15, 1 );

    //set the masses
    e1.  set_generated_mass(parmas_.amell);
    e2.  set_generated_mass(parmas_.amell);
    tau1.set_generated_mass(parmas_.amtau);
    tau2.set_generated_mass(parmas_.amtau);

    //make the vertex
    HepMC3::GenVertex &vertex = event->new_vertex();
    vertex.add_particle_in(e1);
    vertex.add_particle_in(e2);
    vertex.add_particle_out(tau1);
    vertex.add_particle_out(tau2);

    return event;
}

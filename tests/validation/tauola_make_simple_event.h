using namespace Tauolapp;

/** Create a simple HepMC3::GenEvent
 ** e+ + e- -> tau+ tau -
 **/
HepMC3::GenEvent * tauola_make_simple_event(){

    HepMC3::GenEvent * event = new HepMC3::GenEvent();

    //Create some four vectors for the electrons
    double ele_mass_sqr = parmas_.amell*parmas_.amell;
    HepMC3::FourVector momentum_e1(0,0,0,0);
    HepMC3::FourVector momentum_e2(0,0,0,0);

    momentum_e1.setPz(-2); //change these
    momentum_e2.setPz(3.5); //as needed

    momentum_e1.setE(sqrt(momentum_e1.pz()*momentum_e1.pz()+ele_mass_sqr));
    momentum_e2.setE(sqrt(momentum_e2.pz()*momentum_e2.pz()+ele_mass_sqr));

    //make taus
    double tau_mass = parmas_.amtau;

    //make tau's four vector
    //HepMC3::FourVector momentum_tau1(0,0,0,0);
    //HepMC3::FourVector momentum_tau2(0,0,0,0);
    
    HepMC3::FourVector momentum_tau1(+1.38605041e+00,+1.38605041e+00,+7.50000000e-01,+2.75000005e+00);
    HepMC3::FourVector momentum_tau2(-1.38605041e+00,-1.38605041e+00,+7.50000000e-01,+2.75000005e+00);

    //make particles
    HepMC3::GenParticle &e1   = event->new_particle(); e1.  set_momentum(momentum_e1);   e1.  set_pdg_id(-11); e1.  set_status(3);
    HepMC3::GenParticle &e2   = event->new_particle(); e2.  set_momentum(momentum_e2);   e2.  set_pdg_id( 11); e2.  set_status(3);
    HepMC3::GenParticle &tau1 = event->new_particle(); tau1.set_momentum(momentum_tau1); tau1.set_pdg_id(-15); tau1.set_status(1);
    HepMC3::GenParticle &tau2 = event->new_particle(); tau2.set_momentum(momentum_tau2); tau2.set_pdg_id( 15); tau2.set_status(1);

    //set the masses
    e1.  set_generated_mass(parmas_.amell);
    e2.  set_generated_mass(parmas_.amell);
    tau1.set_generated_mass(tau_mass);
    tau2.set_generated_mass(tau_mass);

    //make the vertex
    HepMC3::GenVertex &vertex = event->new_vertex();
    vertex.add_particle_in(e1);
    vertex.add_particle_in(e2);
    vertex.add_particle_out(tau1);
    vertex.add_particle_out(tau2);

    return event;
}

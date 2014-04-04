#include "Tauola/Tauola.h"
#include "Tauola/TauolaHepMC3Event.h"
#include "Tauola/TauolaHepMC3Particle.h"

//HepMC3 headers
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

using namespace std;
using namespace Tauolapp;

/** Create a simple HepMC3::GenEvent
 ** e+ + e- -> tau+ tau -
 **/
HepMC3::GenEvent * make_simple_tau_event(){

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
    HepMC3::FourVector momentum_tau1(0,0,0,0);
    HepMC3::FourVector momentum_tau2(0,0,0,0);

    //make particles
    HepMC3::GenParticle * e1 = new HepMC3::GenParticle(momentum_e1,-11,3);
    HepMC3::GenParticle * e2 = new HepMC3::GenParticle(momentum_e2,11,3);
    HepMC3::GenParticle * tau1 = new HepMC3::GenParticle(momentum_tau1,-15,1);
    HepMC3::GenParticle * tau2 = new HepMC3::GenParticle(momentum_tau2,15,1);

    //set the masses
    e1->set_generated_mass(parmas_.amell);
    e2->set_generated_mass(parmas_.amell);
    tau1->set_generated_mass(tau_mass);
    tau2->set_generated_mass(tau_mass);

    //make the vertex
    HepMC3::GenVertex * vertex = new HepMC3::GenVertex();
    vertex->add_particle_in(e1);
    vertex->add_particle_in(e2);
    vertex->add_particle_out(tau1);
    vertex->add_particle_out(tau2);

    event->add_vertex(vertex);

    //calculate center of mass frame
    HepMC3::FourVector cms(0,0,(momentum_e1.pz()+momentum_e2.pz()),
              momentum_e1.e()+momentum_e2.e());

    HepMC3::GenParticle * cms_particle = new HepMC3::GenParticle(cms,0,0);

    //Make TauolaParticles for doing boosting:
    TauolaHepMC3Particle * cms_boost = new TauolaHepMC3Particle(cms_particle);

    TauolaHepMC3Particle first_tau(tau1);
    TauolaHepMC3Particle second_tau(tau2);

    double tau_energy = 0.5*sqrt(cms.e()*cms.e() - (cms.px()*cms.px()+cms.py()*cms.py()+cms.pz()*cms.pz()));

    first_tau.setE(tau_energy);
    first_tau.setPx((1.0/sqrt(2.0))*sqrt(tau_energy*tau_energy-tau_mass*tau_mass));
    first_tau.setPy((1.0/sqrt(2.0))*sqrt(tau_energy*tau_energy-tau_mass*tau_mass));

    second_tau.setE(tau_energy);
    second_tau.setPx(-1*(1.0/sqrt(2.0))*sqrt(tau_energy*tau_energy-tau_mass*tau_mass));
    second_tau.setPy(-1*(1.0/sqrt(2.0))*sqrt(tau_energy*tau_energy-tau_mass*tau_mass));

    first_tau.boostFromRestFrame(cms_boost);
    second_tau.boostFromRestFrame(cms_boost);

    //clean up
    delete cms_boost;
    delete cms_particle;

    return event;
}

/** example main for decaying a stop with modified tauola */
int main(void){

    int NumberOfEvents = 10;

    //These three lines are not really necessary since they are the default
    Tauola::setDecayingParticle(15);
    Tauola::setSameParticleDecayMode(0);
    Tauola::setOppositeParticleDecayMode(0);

    Tauola::initialize();

    // Begin event loop. Generate event.
    for (int iEvent = 0; iEvent < NumberOfEvents; ++iEvent) {

        // Convert event record to Tau format

        HepMC3::GenEvent * event = make_simple_tau_event();
        event->set_print_precision(8);
        cout << "BEFORE:"<<endl;
        event->print();

        TauolaHepMC3Event *t_event = new TauolaHepMC3Event(event);
        t_event->decayTaus();

        cout << "AFTER:"<<endl;
        event->print();

        //clean up
        delete event;
        delete t_event;
    }
    // This is an access to old FORTRAN info on generated tau sample.
    // That is why it refers to old version number (eg. 2.7) for TAUOLA.
    Tauola::summary();
}


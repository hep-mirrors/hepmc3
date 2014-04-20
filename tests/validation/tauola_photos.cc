// Tauola headers
#include "Tauola/Tauola.h"
#include "Tauola/TauolaHepMC3Event.h"
#include "Tauola/TauolaHepMC3Particle.h"

// Photos headers
#include "Photos/Photos.h"
#include "Photos/PhotosHepMC3Event.h"

// HepMC3 headers
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/IO/IO_GenEvent.h"

#include "tauola_make_simple_event.h"
using namespace std;
using namespace Tauolapp;
using namespace Photospp;

/** example main for decaying a stop with modified tauola */
int main(void){

    int NumberOfEvents = 1000;

    //These three lines are not really necessary since they are the default
    Tauola::setDecayingParticle(15);
    Tauola::setSameParticleDecayMode(0);
    Tauola::setOppositeParticleDecayMode(0);

    Tauola::initialize();

    Photos::initialize();
    Photos::setInfraredCutOff(0.001/200);
    Photos::createHistoryEntries(true,3);

    HepMC3::IO_GenEvent file("test.hepmc",std::ios::out);
    file.set_precision(4);

    // Begin event loop. Generate event.
    for (int iEvent = 0; iEvent < NumberOfEvents; ++iEvent) {

        // Convert event record to Tau format

        HepMC3::GenEvent * event = tauola_make_simple_event();
        event->set_print_precision(8);

        //cout << "BEFORE:"<<endl;
        //event->print();

        // Decay taus using Tauola++
        TauolaHepMC3Event t_event(event);
        t_event.decayTaus();

        // Process by Photos++
        PhotosHepMC3Event p_event(event);
        p_event.process();

        //cout << "AFTER:"<<endl;
        //event->print();

        file.write_event(*event);

        //clean up
        delete event;
    }
    // This is an access to old FORTRAN info on generated tau sample.
    // That is why it refers to old version number (eg. 2.7) for TAUOLA.
    Tauola::summary();
}


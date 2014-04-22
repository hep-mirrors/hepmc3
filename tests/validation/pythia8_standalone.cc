// pythia header files
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC3.h"

// HepMC3 header files
#include "HepMC3/GenEvent.h"
#include "HepMC3/IO/IO_GenEvent.h"

#include <iostream>
using namespace Pythia8;

int main(int argc, char **argv) {

    if( argc < 3 ) {
        std::cout<<"Usage: "<<argv[0]<<" <config_file> <events_limit>"<<std::endl;
        exit(-1);
    }

    int evtLimit = atoi(argv[2]);

    HepMC3::Pythia8ToHepMC3 ToHepMC3;
    Pythia pythia;
    Event& event = pythia.event;

    pythia.readFile(argv[1]);
    pythia.init();

    HepMC3::IO_GenEvent file("test.hepmc",std::ios::out);
    int evtCount = 0;

    while(true) {
        if( evtLimit && evtCount >= evtLimit ) break;
        ++evtCount;

        if( !pythia.next() ) continue;

        HepMC3::GenEvent hepmc3;
        ToHepMC3.fill_next_event( event, &hepmc3);

        //event.list();
        //hepmc3.print();

        file.write_event(hepmc3);
    }

    pythia.statistics();
}

/**
 *  @brief Basic example of use for pythia8 interface
 *
 */
#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"

#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC3.h"

#include <iostream>
using namespace HepMC;

int main() {
    Pythia8::Pythia pythia;
    Pythia8ToHepMC3 pythiaToHepMC;

    pythia.readFile("pythia8_ee_to_Z_to_tautau.conf");
    pythia.init();

    IO_GenEvent file("out.hepmc3",ios::out);

    for( int i = 0; i< 10000; ++i ) {
        if( !pythia.next() ) continue;

        HepMC::GenEvent hepmc( Units::GEV, Units::MM );

        pythiaToHepMC.fill_next_event(pythia.event, &hepmc);

        if( i==0 ) {
            std::cout << "First event: " << std::endl;
            hepmc.print();
        }

        file.write_event(hepmc);
    }

    file.close();
    pythia.statistics();
}

/**
 *  @file  pythia8_example.cc
 *  @brief Basic example of use for pythia8 interface
 *
 */
#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"

#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC3.h"

#include <iostream>
using namespace HepMC;

/** Main program */
int main(int argc, char **argv) {
    Pythia8::Pythia pythia;
    Pythia8ToHepMC3 pythiaToHepMC;

    char* filename = (char*)"pythia8_ee_to_Z_to_tautau.conf";
    if(argc>1) filename = argv[1];

    pythia.readFile(filename);
    pythia.init();

    char* output = (char*)"out.hepmc3"; 
    if(argc==3) output = argv[2];

    IO_GenEvent file(output,ios::out);

    int    nEvent    = pythia.mode("Main:numberOfEvents");

    for( int i = 0; i< nEvent; ++i ) {
        if( !pythia.next() ) continue;

        HepMC::GenEvent hepmc( Units::GEV, Units::MM );

        pythiaToHepMC.fill_next_event(pythia.event, &hepmc, -1, &pythia.info);

        if( i==0 ) {
            std::cout << "First event: " << std::endl;
            hepmc.print();
        }

        file.write_event(hepmc);
    }

    file.close();
    pythia.statistics();
}

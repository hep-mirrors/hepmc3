/**
 *  @file rootIO_example_read.cc
 *  @brief Basic example of use of root I/O: reading events from file
 *
 *  @author Witold Pokorski
 *  @date   16/10/14
 */
#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"
#include "IO_RootStreamer.h"

#include <iostream>

using namespace HepMC;
using std::cout;
using std::endl;

/** Main */
int main(int argc, char **argv) {

    if( argc<3 ) {
        cout << "Usage: " << argv[0] << " <input_root_file> <output_hepmc3_file>" << endl;
        exit(-1);
    }

    IO_RootStreamer root_input (argv[1],std::ios::in);
    IO_GenEvent     text_output(argv[2],std::ios::out);

    int events_parsed = 0;

    while( !root_input.rdstate() ) {

        GenEvent evt(Units::GEV,Units::MM);

        root_input.fill_next_event(evt);

        if( root_input.rdstate() ) break;

        if( events_parsed == 0 ) {
            cout << "First event: " << endl;
            evt.print();
        }

        text_output.write_event(evt);
        ++events_parsed;

        if( events_parsed%1000 == 0 ) {
            cout << "Event: " << events_parsed << endl;
        }
    }

    root_input.close();
    text_output.close();

    std::cout << "Events parsed and written: " << events_parsed << std::endl;

    return 0;
}

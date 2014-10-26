/**
 *  @file HepMC2_adapter_example.cc
 *  @brief Example of use of HepMC2 adapter
 *
 *  Converts selected HepMC2 file to HepMC3 file
 *
 */
#include "HepMC/GenEvent.h"

#include "HepMC/IO/IO_HepMC2_adapter.h"
#include "HepMC/IO/IO_GenEvent.h"

#include <iostream>
#include <cstdlib> // atoi
using namespace HepMC;
using std::cout;
using std::endl;

int main(int argc, char **argv) {

    if( argc < 3 ) {
        cout<<"Usage: " << argv[0] <<" <input_hepmc2_file> <output_hepmc3_file> [<optional_events_limit>]" << endl;
        exit(-1);
    }

    // Open input and output files
    IO_HepMC2_adapter adapter(argv[1]);
    IO_GenEvent       output_file(argv[2],std::ios::out);
    int events_parsed = 0;
    int events_limit  = 0;

    if( argc >= 4 ) events_limit = atoi(argv[3]);

    while( !adapter.rdstate() ) {
        GenEvent evt(Units::GEV,Units::MM);

        // Read event from input file
        adapter.fill_next_event(evt);

        // If reading failed - exit loop
        if( adapter.rdstate() ) break;

        // Save event to output file
        output_file.write_event(evt);

        if(events_parsed==0) {
            cout << " First event: " << endl;
            evt.print();
        }

        ++events_parsed;
        if( events_parsed%100 == 0 ) cout<<"Events parsed: "<<events_parsed<<endl;
        if( events_limit && events_parsed >= events_limit ) break;
    }

    adapter.close();
    output_file.close();

    return 0;
}

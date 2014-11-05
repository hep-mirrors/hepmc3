//
//  rootIOexample.cc
//  HepMC3
//
//  Created by Witold Pokorski on 16/10/14.
//
//

#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"
#include "IO_RootStreamer.h"

#include <iostream>
#include <sstream>

using namespace HepMC;
using std::cout;
using std::endl;

int main(int argc, char **argv) {

    if( argc<3 ) {
        cout << "Usage: " << argv[0] << " <HepMC3_input_file> <output_file>" << endl;
        exit(-1);
    }

    IO_RootStreamer rootio(argv[2],"RECREATE");

    //
    IO_GenEvent input_file (argv[1],std::ios::in);

    int events_parsed = 0;

    while(!input_file.rdstate()) {

        GenEvent evt(Units::GEV,Units::MM);

        input_file.fill_next_event(evt);

        if(input_file.rdstate()) break;

        if( events_parsed == 0 ) {
            cout << "First event: " << endl;
            evt.print();
        }

        rootio.write_event(evt);

    }

    input_file.close();

    return 0;
}

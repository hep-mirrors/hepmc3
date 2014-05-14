#include "ValidationControl.h"

#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    if( argc<2 ) {
            std::cout<<"Usage: "<<argv[0]<<" <config_file> [<events_limit>]"<<std::endl;
            exit(-1);
    }

    ValidationControl control;

    // Read setup from config file
    control.read_file(argv[1]);

    // Override event limit
    if( argc >= 3 ) control.set_event_limit(atoi(argv[2]));

    //
    // Process events
    //
    control.initialize();

    while( control.new_event() )
    {
        GenEvent HepMCEvt;
        control.process(HepMCEvt);
    }

    control.finalize();
}
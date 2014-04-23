#include "ValidationControl.h"
using std::cout;
using std::endl;

int EventsToCheck=20;

int main(int argc, char **argv)
{
    if( argc<2 ) {
            std::cout<<"Usage: "<<argv[0]<<" <config_file> [<events_limit>]"<<std::endl;
            exit(-1);
    }

    ValidationControl control;

    // Read setup from config file
    control.read_file(argv[1]);

    // Set additional options
    control.print_events(3);
    control.check_momentum_for_events(100);

    if( argc >= 3 ) control.set_event_limit(atoi(argv[2]));

    // Initialize
    control.initialize();

    // Event loop
    while( control.new_event() )
    {
        GenEvent HepMCEvt;
        control.process(HepMCEvt);
    }

    // Finalize
    control.finalize();
}
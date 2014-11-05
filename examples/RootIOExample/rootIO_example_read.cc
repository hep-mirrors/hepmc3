//
//  rootIOexample.cc
//  HepMC3
//
//  Created by Witold Pokorski on 16/10/14.
//
//

#include "HepMC/GenEvent.h"
#include "IO_RootStreamer.h"

using namespace HepMC;
using std::cout;
using std::endl;

int main(int argc, char **argv) {

    if( argc<2 ) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(-1);
    }

    IO_RootStreamer rootio(argv[1], "READ");

    GenEvent evt(Units::GEV,Units::MM);
    int events_parsed = 0;

    while (rootio.fill_next_event(evt))
    {        
      if( events_parsed == 0 ) {
	cout << "First event: " << endl;
	evt.print();
      }

      ++events_parsed;
      
      if( events_parsed%1000 == 0 ) {
	cout << "Event: " << events_parsed << endl;
      }
    }
    
    cout << "Events parsed: " << events_parsed << endl;
    return 0;
}

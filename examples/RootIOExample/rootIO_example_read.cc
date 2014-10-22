//
//  rootIOexample.cc
//  HepMC3
//
//  Created by Witold Pokorski on 16/10/14.
//
//

#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_RootStreamer.h"

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TKey.h"

#include <iostream>
#include <sstream>

using namespace HepMC;
using std::cout;
using std::endl;

int main(int argc, char **argv) {

    if( argc<2 ) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        exit(-1);
    }

    // initialize ROOT
    TSystem ts;
    gSystem->Load("librootIO_example_ClassesDict");

    TFile fo(argv[1]);

    IO_RootStreamer input;
    GenEventData* eventdata = NULL;

    fo.GetListOfKeys()->Print();

    TIter next(fo.GetListOfKeys());
    TKey *key;

    int events_parsed = 0;

    while ((key=(TKey*)next()))
    {
        eventdata = (GenEventData*)key->ReadObj();

        if( !eventdata ) break;

        GenEvent evt(Units::GEV,Units::MM);

        input.set_data( *eventdata );
        input.fill_next_event(evt);

        if( events_parsed == 0 ) {
            cout << "First event: " << endl;
            evt.print();
        }

        delete eventdata;

        ++events_parsed;

        if( events_parsed%1000 == 0 ) {
            cout << "Event: " << events_parsed << endl;
        }
    }

    cout << "Events parsed: " << events_parsed << endl;
    return 0;
}

#include "HepMC/GenEvent.h"
#include "HepMC/GenRunInfo.h"
#include "HepMC/WriterAscii.h"
#include "HepMC/Print.h"

#include <ctime>
#include <chrono>

#include "MyClass.h"

#include "TFile.h"
#include "TSystem.h"
#include "TKey.h"

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

    long long int totalTimeRead = 0;

    TFile fo(argv[1]);
    WriterAscii text_output(argv[2]);

    MyClass* myevent;
    int events_parsed = 0;


    fo.GetListOfKeys()->Print();

    TIter next(fo.GetListOfKeys());
    TKey *key;


    while ((key=(TKey*)next()))
    {
        const char *cl = key->GetClassName();

        if( strncmp(cl,"MyClass",7) != 0 ) continue;

        auto startRead = std::chrono::high_resolution_clock::now();

        fo.GetObject(key->GetName(), myevent);

        auto endRead = std::chrono::high_resolution_clock::now();

        std::chrono::duration<int,std::nano> diff_read_next_event = endRead - startRead;
        totalTimeRead += diff_read_next_event.count();


        cout << "Event: " << key->GetName() << endl;

        if( events_parsed == 0 ) {
            cout << "First event: " << endl;
	    Print::listing(*(myevent->GetEvent()));
        }


        text_output.write_event(*(myevent->GetEvent()));
        ++events_parsed;

        if( events_parsed%1000 == 0 ) {
            cout << "Event: " << events_parsed << endl;
        }

        delete myevent->GetEvent();
        delete myevent;
    }

    text_output.close();

    std::cout << "Events parsed and written: " << events_parsed << std::endl;

  float nano2mili = 1./1000000.;
  printf("%-40s %20.2f ms \n",  "total time of Fill():" , nano2mili*(float)totalTimeRead/(float)events_parsed );



    return 0;
}

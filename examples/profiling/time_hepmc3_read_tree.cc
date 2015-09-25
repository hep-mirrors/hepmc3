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


 



 



    return 0;
}

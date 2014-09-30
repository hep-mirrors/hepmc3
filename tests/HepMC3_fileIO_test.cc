/**
 *  @file HepMC3_fileIO_test.cc
 *  @brief Test of file I/O
 *
 *  Parses HepMC3 input file and saves it as new HepMC3 file.
 *  diff of these files should show no difference
 *
 */
#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"

#include <iostream>
using namespace HepMC;
using std::cout;
using std::endl;

int main(int argc, char **argv) {

    if( argc<3 ) {
        cout << "Usage: " << argv[0] << " <HepMC3_input_file> <output_file>" << endl;
        exit(-1);
    }
    
    IO_GenEvent input_file (argv[1],std::ios::in);
    IO_GenEvent output_file(argv[2],std::ios::out);
    
    int events_parsed = 0;

    while(!input_file.rdstate()) {
        if(events_parsed%100==0) cout<<"Events parsed: "<<events_parsed<<endl;

        GenEvent evt(Units::GEV,Units::MM);

        input_file.fill_next_event(evt);
        
        if(input_file.rdstate()) break;
        
        output_file.write_event(evt);
        if(events_parsed==0) {
            evt.print();
            evt.dump();
        }

        events_parsed++;
    }

    input_file.close();
    output_file.close();

    return 0;
}

//
//  rootIOexample.cc
//  HepMC3
//
//  Created by Witold Pokorski on 16/10/14.
//
//

#include "HepMC/GenEvent.h"
#include "HepMC/IO/IO_GenEvent.h"
#include "HepMC/IO/IO_RootStreamer.h"

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"

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
    
    // initialize ROOT
    TSystem ts;
    gSystem->Load("librootIO_example_ClassesDict");
    
    
    TFile* fFile = new TFile(argv[2],"RECREATE");
    
    //
    IO_GenEvent input_file (argv[1],std::ios::in);
    IO_RootStreamer output;
    
    int events_parsed = 0;
    
    while(!input_file.rdstate()) {
        
        GenEvent evt(Units::GEV,Units::MM);
        
        input_file.fill_next_event(evt);
        
        if(input_file.rdstate()) break;
        
        output.write_event(evt);
        
        std::ostringstream os;
        os << events_parsed;
        std::string stevt = "Event_" + os.str();
        const char* chevt = stevt.c_str();
        
        cout << "writing " << stevt << endl;
        
        fFile->WriteObject(&(output.data()), chevt);
        
        events_parsed++;
    }
    
    input_file.close();
    fFile->Close();
    
    return 0;
}

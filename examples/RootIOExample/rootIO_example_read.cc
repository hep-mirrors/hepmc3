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

  GenEventData* event;

  fo.GetListOfKeys()->Print();
 
  TIter next(fo.GetListOfKeys());
  TKey *key;

  while ((key=(TKey*)next()))
    {
      fo.GetObject(key->GetName(), event);
 
      cout << "Event: " << key->GetName() << endl;
    }    
  return 0;
}

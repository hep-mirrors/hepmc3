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

  IO_RootStreamer input;
  GenEventData* eventdata;

  fo.GetListOfKeys()->Print();
 
  TIter next(fo.GetListOfKeys());
  TKey *key;

  while ((key=(TKey*)next()))
    {
      fo.GetObject(key->GetName(), eventdata);
 
      cout << "Event: " << key->GetName() << endl;
      cout << "Number of particles: " << eventdata->particles.size() << endl;
      cout << "Number of vertices: " << eventdata->vertices.size() << endl;
      cout << "pz of particle 1 " << eventdata->particles[1].momentum.z() << endl;

      GenEvent evt(Units::GEV,Units::MM);
        
      input.fill_next_event(evt);
      evt.print();
    }    
  return 0;
}

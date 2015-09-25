#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC3.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenRunInfo.h"
#include "HepMC/ReaderAscii.h"
#include "HepMC/Print.h"
#include "HepMC/Data/GenEventData.h"
#include "HepMC/Data/GenRunInfoData.h"

// Following line to be used with HepMC 2.04 onwards.
#ifdef HEPMC_HAS_UNITS
#include "HepMC/Units.h"
#endif

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TDataType.h"

#include <sstream>
#include <iostream>

#include <cstdio>
#include <ctime>
#include <chrono>

#include "MyClass.h"

using namespace Pythia8; 
using namespace HepMC;
using namespace std;

int main(int argc, const char* argv[]) {

   int nevents = 10;
   if (argc > 1) nevents=atoi( argv[1] );

   unsigned long long totalTimeFill = 0;
   unsigned long long totalTime_fill_next_event = 0;
   unsigned long long totalTime_pythia8_next = 0;

   std::chrono::duration<int,std::nano> totalTime;

   auto start = std::chrono::high_resolution_clock::now();

   TFile * myfile = new TFile("test3.root","RECREATE");

  // Interface for conversion from Pythia8::Event to HepMC one. 
  HepMC::Pythia8ToHepMC3 ToHepMC;

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia8::Pythia pythia;
  pythia.readString("HardQCD:all = on");    
  pythia.readString("PhaseSpace:pTHatMin = 20.");    
  pythia.readString("Beams:eCM = 13000.");
  pythia.init();

   HepMC::GenEvent* hepmcevt;

   MyClass* myclass = new MyClass();   
   TTree *T = new TTree("T","test");
   T->Branch("event",  &myclass, 256000,0);   

  // Begin event loop. Generate event. Skip if error. List first one.
  for (int iEvent = 0; iEvent < nevents; ++iEvent) {

   auto start_pythia8_next = std::chrono::high_resolution_clock::now();
   if (!pythia.next()) continue;
   auto end_pythia8_next = std::chrono::high_resolution_clock::now();
   std::chrono::duration<int,std::nano> diff_pythia8_next = end_pythia8_next - start_pythia8_next;
   totalTime_pythia8_next += diff_pythia8_next.count();

    // Construct new empty HepMC event. 
    hepmcevt = new HepMC::GenEvent( HepMC::Units::GEV, HepMC::Units::MM);

    // Fill HepMC event, including PDF info.
    auto start_fill_next_event = std::chrono::high_resolution_clock::now();
    ToHepMC.fill_next_event( pythia.event,  hepmcevt, -1, &pythia.info );
    auto end_fill_next_event = std::chrono::high_resolution_clock::now();
   
    std::chrono::duration<int,std::nano> diff_fill_next_event = end_fill_next_event - start_fill_next_event;
    totalTime_fill_next_event += diff_fill_next_event.count();


    MyClass* myclass = new MyClass();
    myclass->SetEventNumber(iEvent);
    myclass->SetEvent(hepmcevt);



    auto startFill = std::chrono::high_resolution_clock::now();
    T->Fill();    
    auto endFill = std::chrono::high_resolution_clock::now();

    std::chrono::duration<int,std::nano> diffFill = endFill - startFill;
    totalTimeFill += diffFill.count();

    delete hepmcevt;

  }

  T->Write();
  myfile->Close();

  auto end = std::chrono::high_resolution_clock::now();

  long long int nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  float nano2mili = 1./1000000.;
  printf("%-40s %20.2f ms \n",  "total time of Fill():" , nano2mili*(float)totalTimeFill/(float)nevents );
  printf("%-40s %20.2f ms \n",  "total time of fill_next_event():" , nano2mili*(float)totalTime_fill_next_event/(float)nevents );
  printf("%-40s %20.2f ms \n",  "total time of pythia8_next():" , nano2mili*(float)totalTime_pythia8_next/(float)nevents );
  printf("%-40s %20.2f ms \n",  " total time:" , nano2mili*(float)nanos/(float)nevents );

  return 0;
}

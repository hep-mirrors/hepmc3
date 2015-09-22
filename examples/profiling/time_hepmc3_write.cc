
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC3.h"
#include "HepMC/WriterAscii.h"

#include "TROOT.h"
#include "TSystem.h"


#include "HepMC/GenEvent.h"
#include "HepMC/GenRunInfo.h"
#include "HepMC/ReaderAscii.h"
#include "HepMC/Print.h"

#include "TFile.h"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <chrono>

using namespace HepMC;
using std::cout;
using std::endl;

#include "MyClass.h"

using namespace Pythia8; 
using namespace HepMC;
using namespace std;


/** Main */
int main(int argc, char **argv) {

    WriterAscii text_output("test3.hepmc3");

   int nevents = 10;
   if (argc > 1) nevents=atoi( argv[1] );

   long long int totalTimeFill = 0;
   long long int totalTime_fill_next_event = 0;
   long long int totalTime_pythia8_next = 0;

   std::chrono::duration<int,std::nano> totalTime;
   auto start = std::chrono::high_resolution_clock::now();

   TFile * fFile = new TFile("test3.root","RECREATE");

    int events_parsed = 0;

    bool is_gen_run_info_written = false;

  // Interface for conversion from Pythia8::Event to HepMC one. 
  HepMC::Pythia8ToHepMC3 ToHepMC;

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia8::Pythia pythia;
  pythia.readString("HardQCD:all = on");    
  pythia.readString("PhaseSpace:pTHatMin = 20.");    
  pythia.readString("Beams:eCM = 13000.");
  pythia.init();

 for (int iEvent = 0; iEvent < nevents; ++iEvent) {

    auto start_pythia8_next = std::chrono::high_resolution_clock::now();
    if (!pythia.next()) continue;
    auto end_pythia8_next = std::chrono::high_resolution_clock::now(); 

    std::chrono::duration<int,std::nano> diff_pythia8_next = end_pythia8_next - start_pythia8_next;
    totalTime_pythia8_next += diff_pythia8_next.count();



    GenEvent evt(Units::GEV,Units::MM);


    auto start_fill_next_event = std::chrono::high_resolution_clock::now();

    ToHepMC.fill_next_event( pythia.event,  &evt , -1, &pythia.info );
    auto end_fill_next_event = std::chrono::high_resolution_clock::now(); 
    std::chrono::duration<int,std::nano> diff_fill_next_event = end_fill_next_event - start_fill_next_event;
    totalTime_fill_next_event += diff_fill_next_event.count();
    


    MyClass* myclass = new MyClass();

    myclass->SetEvent(&evt);

    std::string stevt = "Event_" + os.str();
    const char* chevt = stevt.c_str();

        
    auto startFill = std::chrono::high_resolution_clock::now();
    fFile->WriteObject(myclass, chevt);
    auto endFill = std::chrono::high_resolution_clock::now();


    text_output.write_event( evt );

   
    std::chrono::duration<int,std::nano> diffFill = endFill - startFill;
    totalTimeFill += diffFill.count();


    ++events_parsed;

    if( events_parsed%1000 == 0 ) {
        cout << "Event: " << events_parsed << endl;
    }
    delete myclass;







}
    fFile->Close();
    std::cout << "Events parsed and written: " << events_parsed << std::endl;

  auto end = std::chrono::high_resolution_clock::now();

  long long int nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

  float nano2mili = 1./1000000.;
  printf("%-40s %20.2f ms \n",  "total time of Fill():" , nano2mili*(float)totalTimeFill/(float)nevents );
  printf("%-40s %20.2f ms \n",  "total time of fill_next_event():" , nano2mili*(float)totalTime_fill_next_event/(float)nevents );
  printf("%-40s %20.2f ms \n",  "total time of pythia8_next():" , nano2mili*(float)totalTime_pythia8_next/(float)nevents );
  printf("%-40s %20.2f ms \n",  " total time:" , nano2mili*(float)nanos/(float)nevents );


  text_output.close();

    return 0;
}

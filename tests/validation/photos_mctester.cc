// Photospp header files
#include "Photos/Photos.h"
#include "Photos/PhotosHepMC3Event.h"
#include "Photos/Log.h"

// MC-TESTER header files
#include "Generate.h"
#include "Setup.H"
#include "HepMC3Event.h"

// HepMC header files
#include "HepMC3/IO/IO_HepMC2_adapter.h"
#include "HepMC3/IO/IO_GenEvent.h"
#include "HepMC3/Search/FindParticles.h"

#include <boost/foreach.hpp>

#include "photos_check_momentum_conservation.h"
using namespace HepMC3;
using namespace Photospp;

int EventsToCheck=20;

int main(int argc, char **argv)
{
        if( argc<2 ) {
                std::cout<<"Usage: "<<argv[0]<<" <input_file> [<events_limit>]"<<std::endl;
                exit(-1);
        }

        Photos::initialize();
        Photos::setInfraredCutOff(0.001/200);
        //Log::LogDebug();

        HepMC3::IO_HepMC2_adapter file(argv[1],std::ios::in);
        HepMC3::IO_GenEvent out("test.hepmc",std::ios::out);

        MC_Initialize();

        ::Setup::decay_particle = 23;

        int evtCount = 0;
        int evtLimit = 0;
        if( argc >= 3 ) evtLimit = atoi(argv[2]);

        // Begin event loop
        while(true)
        {
                evtCount++;
                // Create event
                HepMC3::GenEvent HepMCEvt;

                HepMCEvt.set_print_precision(8);
                file.fill_next_event(HepMCEvt);

                if(file.rdstate()) break;

                //cout << "BEFORE:"<<endl;
                //HepMCEvt->print();

                if(evtCount<EventsToCheck)
                {
                        cout<<"                                          "<<endl;
                        cout<<"Momentum conservation chceck BEFORE/AFTER Photos"<<endl;
                        photos_check_momentum_conservation(HepMCEvt);
                }

                // Process by photos
                PhotosHepMC3Event evt(&HepMCEvt);
                evt.process();

                // Run MC-TESTER on the event
                HepMC3Event temp_event(HepMCEvt,false);
                MC_Analyze(&temp_event);

                if(evtCount<EventsToCheck)
                {
                        photos_check_momentum_conservation(HepMCEvt);
                }

                //cout << "AFTER:"<<endl;
                //HepMCEvt.print();

                out.write_event(HepMCEvt);

                if( evtLimit && evtCount >= evtLimit ) break;
        }

        MC_Finalize();
}
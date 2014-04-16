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
using namespace HepMC3;
using namespace Photospp;

int EventsToCheck=20;

// elementary test of HepMC typically executed before
// detector simulation based on http://home.fnal.gov/~mrenna/HCPSS/HCPSShepmc.html
// similar test was performed in Fortran
// we perform it before and after Photos (for the first several events)
void checkMomentumConservationInEvent(GenEvent &evt)
{
        //cout<<"List of stable particles: "<<endl;

        double px=0.0,py=0.0,pz=0.0,e=0.0;

        FindParticles search( evt, FIND_ALL, STATUS == 1 );

        BOOST_FOREACH( GenParticle *p, search.results() ) {
            HepMC3::FourVector m = p->momentum();
            px+=m.px();
            py+=m.py();
            pz+=m.pz();
            e +=m.e();
            //(*p)->print();
        }

        cout.precision(6);
        cout.setf(std::ios_base::floatfield);
        cout<<endl<<"Vector Sum: "<<px<<" "<<py<<" "<<pz<<" "<<e<<endl;
}

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

        Setup::decay_particle = 23;

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
                        checkMomentumConservationInEvent(HepMCEvt);
                }

                // Process by photos
                PhotosHepMC3Event evt(&HepMCEvt);
                evt.process();

                // Run MC-TESTER on the event
                HepMC3Event temp_event(HepMCEvt,false);
                MC_Analyze(&temp_event);

                if(evtCount<EventsToCheck)
                {
                        checkMomentumConservationInEvent(HepMCEvt);
                }

                //cout << "AFTER:"<<endl;
                //HepMCEvt.print();

                out.write_event(HepMCEvt);

                if( evtLimit && evtCount >= evtLimit ) break;
        }

        MC_Finalize();
}
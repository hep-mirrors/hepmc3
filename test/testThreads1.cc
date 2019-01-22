// -*- C++ -*-
#include "HepMC3/Attribute.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3TestUtils.h"
#include <thread>
const int NinputCopies=4;
const int NmaxThreads=3;
void attribute_function1(const HepMC::GenEvent& e, const int& id)
{
    shared_ptr<HepMC::GenCrossSection>  xs     = e.attribute<HepMC::GenCrossSection>("GenCrossSection",0);
    printf("XS in event  %i  is %f, id=%i\n",e.event_number(),xs->xsec(),id);
}
int main()
{
    HepMC::ReaderAsciiHepMC2 inputA("inputCommon.hepmc");
    if(inputA.failed()) return 1;
    std::vector<HepMC::GenEvent> evts;
    while( !inputA.failed() )
    {
        HepMC::GenEvent evt=HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM);
        inputA.read_event(evt);
        if( inputA.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        evts.push_back(evt);
    }
    inputA.close();
    std::vector<HepMC::GenEvent> thr_evts[NinputCopies];
    for (int i=0; i<NinputCopies; i++)thr_evts[i]=evts;

    for (int i=0; i<NinputCopies; i++)
        for (int e=0; e<evts.size(); e++)
        {
            std::vector<std::thread> threads;
            int j1=-thr_evts[i].at(e).vertices().size();
            int j2=thr_evts[i].at(e).particles().size();
            int d=(j2-j1)/NmaxThreads;
            std::vector<int> ids;
            ids.push_back(0);
            for (int j=j1; j<j2; j+=d)
                ids.push_back(j);
            /* The arguments to the thread function are moved or copied by value.
            If a reference argument needs to be passed to the thread function, it
            has to be wrapped (e.g. with std::ref or std::cref).
            */
            for (int j=0; j<ids.size(); j++)
                threads.push_back(std::thread(attribute_function1,std::cref(thr_evts[i].at(e)),ids[j]));
            for (auto& th : threads) th.join();
            threads.clear();
        }
    for (int k=0; k<NinputCopies; k++)
    {
        HepMC::WriterAscii       outputA("outputThreads1_"+std::to_string(k)+".hepmc");
        if(outputA.failed()) return 2;
        for (size_t i=0; i<thr_evts[k].size(); i++) outputA.write_event(thr_evts[k].at(i));
        thr_evts[k].clear();
        outputA.close();
        if (k>0)
        {
            int result=COMPARE_ASCII_FILES("outputThreads1_"+std::to_string(k-1)+".hepmc","outputThreads1_"+std::to_string(k)+".hepmc");
            if (result!=0) return result;
        }
    }
    return 0;
}

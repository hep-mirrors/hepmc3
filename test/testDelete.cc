// -*- C++ -*-
#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/ReaderAscii.h"
#include "HepMC/WriterAscii.h"
#include "HepMC/ReaderAsciiHepMC2.h"
#include "HepMC/WriterAsciiHepMC2.h"
#include "HepMC3TestUtils.h"
int main()
{
    HepMC::ReaderAsciiHepMC2 inputA("inputDelete.hepmc");
    if(inputA.failed()) return 1;
    std::vector<HepMC::GenEvent> evts;
    while( !inputA.failed() )
        {
            HepMC::GenEvent evt=HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM);
            inputA.read_event(evt);
            if( inputA.failed() )  {printf("End of file reached. Exit.\n"); break;}
            evts.push_back(evt);
        }
    inputA.close();
//No alien particles should be detached from vertices or removed from events
    int i=0;
    int j=0;
    while(i==j)
    {
    i=rand()% evts.size();
    j=rand()% evts.size();    
    }
    evts[i].remove_particles(evts[j].particles());

    for (std::vector<HepMC::GenParticlePtr>::const_iterator p=evts.at(i).particles().begin();p!=evts.at(i).particles().end();++p)
    evts[j].remove_particle(*p);
        
    for (std::vector<HepMC::GenParticlePtr>::const_iterator p=evts.at(i).particles().begin();p!=evts.at(i).particles().end();++p)
    for (std::vector<HepMC::GenVertexPtr>::iterator v=evts.at(j).vertices().begin();v!=evts.at(j).vertices().end();++v)
    {
    (*v)->remove_particle_in(*p);
    (*v)->remove_particle_out(*p);
    }

    HepMC::WriterAscii       outputA("frominputDelete.hepmc");
    if(outputA.failed()) return 2;
    for (size_t i=0;i<evts.size();i++) outputA.write_event(evts[i]);
    evts.clear();
    outputA.close();


    HepMC::ReaderAscii inputB("frominputDelete.hepmc");
    if(inputB.failed()) return 3;
    HepMC::WriterAsciiHepMC2       outputB("fromfrominputDelete.hepmc");
    if(outputB.failed()) return 4;
    while( !inputB.failed() )
        {
            HepMC::GenEvent evt(HepMC::Units::GEV,HepMC::Units::MM);
            inputB.read_event(evt);
            if( inputB.failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputB.write_event(evt);
            evt.clear();
        }
    inputB.close();
    outputB.close();
    return COMPARE_ASCII_FILES("fromfrominputDelete.hepmc","inputDelete.hepmc");
}

// -*- C++ -*-
#include "HepMC/GenEvent.h"
#include "HepMC/ReaderAscii.h"
#include "HepMC/WriterAscii.h"
#include "HepMC/ReaderHEPEVT.h"
#include "HepMC/WriterHEPEVT.h"
#include "HepMC3TestUtils.h"
int main()
{
    HepMC::ReaderHEPEVT* inputA =new HepMC::ReaderHEPEVT("inputIO3.hepevt");
    if(inputA->failed()) return 1;
    HepMC::WriterAscii*       outputA=new HepMC::WriterAscii("frominputIO3.hepmc");
    if(outputA->failed()) return 2;
    while( !inputA->failed() )
        {
            HepMC::GenEvent evt(HepMC::Units::GEV,HepMC::Units::MM);
            inputA->read_event(evt);
            if( inputA->failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputA->write_event(evt);
            evt.clear();
        }
    inputA->close();
    outputA->close();

    HepMC::ReaderAscii* inputB =new HepMC::ReaderAscii("frominputIO3.hepmc");
    if(inputB->failed()) return 3;
    HepMC::WriterHEPEVT*       outputB=new HepMC::WriterHEPEVT("fromfrominputIO3.hepevt");
    if(outputB->failed()) return 4;
    while( !inputB->failed() )
        {
            HepMC::GenEvent evt(HepMC::Units::GEV,HepMC::Units::MM);
            inputB->read_event(evt);
            if( inputB->failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputB->write_event(evt);
            evt.clear();
        }
    inputB->close();
    outputB->close();
    return COMPARE_ASCII_FILES("fromfrominputIO3.hepevt","inputIO3.hepevt");
}

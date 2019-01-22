// -*- C++ -*-
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/WriterHEPEVT.h"
#include "HepMC3TestUtils.h"
int main()
{
    HepMC::ReaderHEPEVT inputA("inputIO3.hepevt");
    if(inputA.failed()) return 1;
    HepMC::WriterAscii       outputA("frominputIO3.hepmc");
    if(outputA.failed())   return 2;
    while( !inputA.failed() )
        {
            HepMC::GenEvent evt(HepMC::Units::GEV,HepMC::Units::MM);
            inputA.read_event(evt);
            if( inputA.failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputA.write_event(evt);
            evt.clear();
        }
    inputA.close();
    outputA.close();

    HepMC::ReaderAscii inputB("frominputIO3.hepmc");
    if(inputB.failed()) return 3;
    HepMC::WriterHEPEVT       outputB("fromfrominputIO3.hepevt");
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
    return COMPARE_ASCII_FILES("fromfrominputIO3.hepevt","inputIO3.hepevt");
}

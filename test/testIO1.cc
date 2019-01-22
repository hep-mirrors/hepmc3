// -*- C++ -*-
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3TestUtils.h"
int main()
{
    HepMC::ReaderAsciiHepMC2 inputA("inputIO1.hepmc");
    if(inputA.failed()) return 1;
    HepMC::WriterAscii       outputA("frominputIO1.hepmc");
    if(outputA.failed()) return 2;
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


    HepMC::ReaderAscii inputB("frominputIO1.hepmc");
    if(inputB.failed()) return 3;
    HepMC::WriterAsciiHepMC2       outputB("fromfrominputIO1.hepmc");
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
    return COMPARE_ASCII_FILES("fromfrominputIO1.hepmc","inputIO1.hepmc");
}

// -*- C++ -*-
#include "HepMC/GenEvent.h"
#include "HepMC/ReaderRootTree.h"
#include "HepMC/WriterRootTree.h"
#include "HepMC/ReaderAsciiHepMC2.h"
#include "HepMC/WriterAsciiHepMC2.h"
#include "HepMC3TestUtils.h"
int main()
{
    HepMC::ReaderAsciiHepMC2 inputA("inputIO2.hepmc");
    if(inputA.failed()) return 1;
    HepMC::WriterRootTree       outputA("frominputIO2.root");
    if(outputA.failed())  return 2;
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

    HepMC::ReaderRootTree inputB("frominputIO2.root");
    if(inputB.failed()) return 3;
    HepMC::WriterAsciiHepMC2       outputB("fromfrominputIO2.hepmc");
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
    return COMPARE_ASCII_FILES("fromfrominputIO2.hepmc","inputIO2.hepmc");
}

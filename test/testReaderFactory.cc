// -*- C++ -*-
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderFactory.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main()
{
    ReaderPtr inputA = ReaderFactory::make_reader("inputReaderFactory.hepmc");
    if(inputA->failed()) return 1;
    WriterAscii       outputA("frominputReaderFactory.hepmc");
    if(outputA.failed()) return 2;
    while( !inputA->failed() )
        {
            GenEvent evt(Units::GEV,Units::MM);
            inputA->read_event(evt);
            if( inputA->failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputA.write_event(evt);
            evt.clear();
        }
    inputA->close();
    outputA.close();


    ReaderPtr inputB = ReaderFactory::make_reader("frominputReaderFactory.hepmc");
    if(inputB->failed()) return 3;
    WriterAsciiHepMC2       outputB("fromfrominputReaderFactory.hepmc");
    if(outputB.failed()) return 4;
    while( !inputB->failed() )
        {
            GenEvent evt(Units::GEV,Units::MM);
            inputB->read_event(evt);
            if( inputB->failed() )  {printf("End of file reached. Exit.\n"); break;}
            outputB.write_event(evt);
            evt.clear();
        }
    inputB->close();
    outputB.close();
    return COMPARE_ASCII_FILES("fromfrominputReaderFactory.hepmc","inputReaderFactory.hepmc");
}

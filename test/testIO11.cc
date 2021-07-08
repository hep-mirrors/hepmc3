// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderOSCAR.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/Print.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main()
{
    ReaderOSCAR inputA("inputIO11.oscar1999");
    if(inputA.failed()) return 1;
    WriterAscii       outputA("frominputIO11.hepmc");
    if(outputA.failed()) return 2;
    while( !inputA.failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        inputA.read_event(evt);
        if( inputA.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        outputA.write_event(evt);
        evt.clear();
    }
    inputA.close();
    outputA.close();


    ReaderAscii inputB("frominputIO11.hepmc");
    if(inputB.failed()) return 3;
//    WriterAsciiHepMC2       outputB("fromfrominputIO1.hepmc");
//    if(outputB.failed()) return 4;
    while( !inputB.failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        inputB.read_event(evt);
        if( inputB.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
//        outputB.write_event(evt);
        evt.clear();
    }
    inputB.close();
//    outputB.close();
    return 0;
}

// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderOSCAR1999.h"
#include "HepMC3/WriterOSCAR1999.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/Print.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main()
{
    ReaderOSCAR1999 inputA("inputIO16.oscar1999A");
    if(inputA.failed()) return 1;
    WriterAscii       outputA("frominputIO16.hepmc");
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


    ReaderAscii inputB("frominputIO16.hepmc");
    if(inputB.failed()) return 3;
    std::shared_ptr<WriterOSCAR1999>       outputB = nullptr;
    while( !inputB.failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        inputB.read_event(evt);
        if( inputB.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        if (!outputB)     outputB = std::make_shared<WriterOSCAR1999>("fromfrominputIO16.oscar1999A", evt.run_info());
        if(outputB->failed()) return 4;
        outputB->write_event(evt);
        evt.clear();
    }
    inputB.close();
    outputB->close();
    return COMPARE_ASCII_FILES("fromfrominputIO16.oscar1999A","inputIO16.oscar1999A");
}

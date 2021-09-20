// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderOSCAR1997.h"
#include "HepMC3/WriterOSCAR1997.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/Print.h"
#include "HepMC3TestUtils.h"
using namespace HepMC3;
int main()
{
    ReaderOSCAR1997 inputA("inputIO13.oscar1997A");
    if(inputA.failed()) return 1;
    WriterAscii       outputA("frominputIO13.hepmc");
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


    ReaderAscii inputB("frominputIO13.hepmc");
    if(inputB.failed()) return 3;
    std::shared_ptr<WriterOSCAR1997>       outputB = nullptr;
    while( !inputB.failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        inputB.read_event(evt);
        if( inputB.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        if (!outputB)     outputB = std::make_shared<WriterOSCAR1997>("fromfrominputIO13.oscar1997A", evt.run_info());
        if(outputB->failed()) return 4;
        outputB->write_event(evt);
        evt.clear();
    }
    inputB.close();
    outputB->close();
    return 0;
}

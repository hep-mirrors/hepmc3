// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderLHEF.h"
#include "HepMC3/WriterAscii.h"
using namespace HepMC3;
int main()
{
    ReaderLHEF inputA("inputIO11.lhe");
    if(inputA.failed()) return 1;
    WriterAscii outputA("frominputIO11.hepmc");
    if(outputA.failed()) return 2;
    while( !inputA.failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        bool read_res =  inputA.read_event(evt);
        if (!read_res && !inputA.failed()) {
            printf("Error reading event from LHE file. Exit.\n");
            return 1;
        }
        if( inputA.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        if (read_res) outputA.write_event(evt);
        evt.clear();
    }
    inputA.close();
    outputA.close();
    return 0;
}

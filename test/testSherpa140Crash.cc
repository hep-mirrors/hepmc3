// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2025 The HepMC collaboration (see AUTHORS for details)
//
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
using namespace HepMC3;
int main()
{
    ReaderAsciiHepMC2 inputA("inputSherpa140Crash.hepmc");
    if(inputA.failed()) return 1;
    std::vector<std::shared_ptr<GenEvent> > evts;
    while( !inputA.failed() )
    {
        std::shared_ptr<GenEvent>  evt= std::make_shared<GenEvent>();
        inputA.read_event(*evt);
        if( inputA.failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        evts.push_back(evt);
    }
    inputA.close();

    return EXIT_SUCCESS;
}


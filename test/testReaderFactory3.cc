// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#if defined(__linux__) || defined(__darwin__)|| defined(__APPLE__) || defined(__FreeBSD__) || defined(__sun)
#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/ReaderFactory.h"
#include "HepMC3TestUtils.h"
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
using namespace HepMC3;


void  writer_function(int* result)
{
    *result = 0;
    ReaderAsciiHepMC2 inputA("inputReaderFactory1.hepmc");
    if(inputA.failed()) {
        *result = 1;
        printf("Error in writer_function.\n");
        return;
    }
    WriterAscii outputA("frominputReaderFactory3.hepmc");
    if(outputA.failed()) {
        *result = 2;
        printf("Error in writer_function.\n");
        return;
    }
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
}

void  reader_function(int * result)
{
    *result = 0;
    std::shared_ptr<Reader> inputB = deduce_reader("frominputReaderFactory3.hepmc");
    if(inputB->failed()) {
        *result = 1;
        printf("Error in reader_function.\n");
        return;
    }
    WriterAsciiHepMC2 outputB("fromfrominputReaderFactory3.hepmc");
    if(outputB.failed()) {
        *result = 2;
        printf("Error in reader_function.\n");
        return;
    }
    while( !inputB->failed() )
    {
        GenEvent evt(Units::GEV,Units::MM);
        inputB->read_event(evt);
        if( inputB->failed() )  {
            printf("End of file reached. Exit.\n");
            break;
        }
        outputB.write_event(evt);
        evt.clear();
    }
    inputB->close();
    outputB.close();
}


int main()
{
    unlink("frominputReaderFactory3.hepmc");
    int  returnValuemkfifo = mkfifo("frominputReaderFactory3.hepmc", 0666);
    if ( returnValuemkfifo == 0 ) {
        printf("FIFO created.\n");
    } else {
        printf("Something went wrong during the FIFO creation.\n");
        return 15;
    }

    std::vector<std::thread> threads;
    threads.reserve(2);
    std::vector<int> results{0,0};
    std::thread readt(reader_function, &(results[0]));
    std::thread writet(writer_function, &(results[1]));
    readt.join();
    writet.join();
    if ( results[0] != 0 || results[1] != 0) {
        printf("Something went wrong during reading/writing %i %i\n", results[0], results[1]);
        return 10;

    }

    int returnValueunlink = unlink("frominputReaderFactory3.hepmc");
    if (returnValueunlink == 0) {
        printf("FIFO deleted.\n");
    } else {
        printf("Something went wrong during the FIFO deletion.\n");
        return 5;
    }
    return COMPARE_ASCII_FILES("fromfrominputReaderFactory3.hepmc","inputReaderFactory1.hepmc");
}
#else
int main()
{
    return 0;
}
#endif

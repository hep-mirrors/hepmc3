// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Test deduce_reader on gzipped protobuf streams
//

#if defined(__linux__) || defined(__darwin__) || defined(__APPLE__) ||         \
    defined(__FreeBSD__) || defined(__sun)
#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/WriterGZ.h"
#include "HepMC3/Writerprotobuf.h"

#include "HepMC3/ReaderFactory.h"

#include "HepMC3TestUtils.h"
#include <sys/stat.h>
#include <unistd.h>
using namespace HepMC3;

void writer_function(int *result) {
    *result = 0;
    ReaderAsciiHepMC2 inputA("inputReaderFactory1.hepmc");
    if (inputA.failed()) {
        *result = 1;
        printf("Error in writer_function.\n");
        return;
    }
    WriterGZ<Writerprotobuf> outputA("frominputReaderFactory4.pb.gz");
    if (outputA.failed()) {
        *result = 2;
        printf("Error in writer_function.\n");
        return;
    }

    while (!inputA.failed()) {
        GenEvent evt(Units::GEV, Units::MM);
        inputA.read_event(evt);
        if (inputA.failed()) {
            printf("End of file reached. Exit.\n");
            break;
        }
        outputA.write_event(evt);
        evt.clear();
    }
    inputA.close();
    outputA.close();
}

void reader_function(int *result) {
    *result = 0;
    std::shared_ptr<Reader> inputB =
        deduce_reader("frominputReaderFactory4.pb.gz");
    if (inputB->failed()) {
        *result = 1;
        printf("Error in reader_function.\n");
        return;
    }
    WriterAsciiHepMC2 outputB("fromfrominputReaderFactory4.hepmc");
    if (outputB.failed()) {
        *result = 2;
        printf("Error in reader_function.\n");
        return;
    }
    while (!inputB->failed()) {
        GenEvent evt(Units::GEV, Units::MM);
        inputB->read_event(evt);
        if (inputB->failed()) {
            printf("End of file reached. Exit.\n");
            break;
        }
        outputB.write_event(evt);
        evt.clear();
    }
    inputB->close();
    outputB.close();
}

int main() {
    std::vector<int> results{0, 0};
    writer_function(&(results[1]));
    reader_function(&(results[0]));
    if (results[0] != 0 || results[1] != 0) {
        printf("Something went wrong during reading/writing %i %i\n", results[0],
               results[1]);
        return 10;
    }

    return COMPARE_ASCII_FILES("fromfrominputReaderFactory4.hepmc",
                               "inputReaderFactory1.hepmc");
}
#else
int main() { return 0; }
#endif

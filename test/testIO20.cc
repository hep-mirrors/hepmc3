// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2026 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Verifies deducing reader type for binary protobuf files.
//

// These are the only headers in ReaderPlugin, so including these firstmakes
// sure the hack below doesn't leak out of the ReaderPlugin header
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderFactory.h"
int main() {
    std::shared_ptr<HepMC3::Reader> rdr = HepMC3::deduce_reader("inputIO20.proto");
    if (!rdr) return 1;
    rdr->close();
    return 0;
}

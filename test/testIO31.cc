// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
// -- Purpose: Test that files without a trailing newline still read all events
//

#include "HepMC3/ReaderFactory.h"
#include "HepMC3/Setup.h"

int main() {
  HepMC3::Setup::set_print_errors(true);
  HepMC3::Setup::set_errors_level(999);
  HepMC3::Setup::set_print_warnings(true);
  HepMC3::Setup::set_warnings_level(999);
  HepMC3::Setup::set_debug_level(999);

  auto rdr = HepMC3::deduce_reader("inputIO31.hepmc");
  if (!rdr) {
    return 1;
  }

  HepMC3::GenEvent ev;

  int ctr = 0;

  while (!rdr->failed()) {
    rdr->read_event(ev);
    if (rdr->failed()) {
      break;
    }
    ctr++;
  }

  if (ctr != 5) {
    std::cout << "[ERROR]: Expected to read 5 events, but read: " << ctr
              << std::endl;
    return 1;
  }

  return 0;
}

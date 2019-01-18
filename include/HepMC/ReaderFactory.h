// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_CONTRIB_READER_FACTORY_H
#define HEPMC_CONTRIB_READER_FACTORY_H

#include "HepMC/ReaderAscii.h"
#include "HepMC/ReaderAsciiHepMC2.h"

#include <memory>
#include <string>

namespace HepMC{
  
  /** @brief Make a reader suitable for reading HepMC2 or HepMC 3 files, depending on which type of file is passed
   *
   *  HepMC uses different implementations of the Reader niterface for HepMC 2 and HepMC 3
   *  input files.  This means a user needs to know which kind of file they will be opening.
   *  This convenience function will attempt to discover which type the file is and return
   *  the appropriate reader.
   */
  std::shared_ptr<Reader> make_reader(const std::string &filename);
    
  
}

#endif

// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_VERSION_H
#define HEPMC_VERSION_H

#include <string>

/// HepMC version string
/// @todo Insert value from build system
#define HEPMC_VERSION "3.0.0"

/// HepMC version as an int (MICRO + 1000*MINOR + 1000000*MAJOR)
// Use like "#if defined HEPMC_VERSION_CODE && LHAPDF_VERSION_CODE < 3001042" for < 3.1.42
/// @todo Insert value from build system
#define HEPMC_VERSION_CODE 3000000


/// @todo Add feature detection macros (clearly version-related, and why proliferate headers?)


namespace HepMC {


  /// Get the HepMC library version string
  inline std::string version() {
    return HEPMC_VERSION;
  }


}
#endif

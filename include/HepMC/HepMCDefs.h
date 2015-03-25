// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_DEFS_H
#define HEPMC_DEFS_H

#include "HepMC/Version.h"


/// @todo This header should have a better "real" name, maybe located together
/// with things like the version and logging/error handling utilities (or
/// accessed with them via a convenience HepMC/Setup/Common.h or similar). Emit
/// a build warning to code which tries to access HepMC/HepMCDefs.h, which is a
/// terrible name but should be provided for a while for backward compatibility.


// The IO_GenEvent class is available
#ifndef HEPMC_HAS_IO_GENEVENT
#define HEPMC_HAS_IO_GENEVENT
#endif

// The IO_Ascii class is NOT available
/// @todo Reinstate
#ifndef HEPMC_IO_ASCII_REMOVED
#define HEPMC_IO_ASCII_REMOVED
#endif

// The ParticleData class is NOT available
#ifndef HEPMC_PARTICLE_DATA_REMOVED
#define HEPMC_PARTICLE_DATA_REMOVED
#endif

// The PdfInfo class is available
#ifndef HEPMC_HAS_PDF_INFO
#define HEPMC_HAS_PDF_INFO
#endif

// HepMC uses SimpleVector (FourVector) to store momentum and position
/// @todo Remove, since SimpleVector.h does not exist
#ifndef HEPMC_HAS_SIMPLE_VECTOR
#define HEPMC_HAS_SIMPLE_VECTOR
#endif

// Units are defined in HepMC
#ifndef HEPMC_HAS_UNITS
#define HEPMC_HAS_UNITS 2
#endif

// The GenCrossSection class is available
#ifndef HEPMC_HAS_CROSS_SECTION
#define HEPMC_HAS_CROSS_SECTION
#endif

// The iterator range classes are available
#ifndef HEPMC_HAS_ITERATOR_RANGES
#define HEPMC_HAS_ITERATOR_RANGES
#endif

// The HepMC::GenWeights class allows named weights (version 2)
#ifndef HEPMC_HAS_NAMED_WEIGHTS
#define HEPMC_HAS_NAMED_WEIGHTS 2
#endif

// The HepMC::GenVertex class has a status() member
#ifndef HEPMC_VERTEX_HAS_STATUS
#define HEPMC_VERTEX_HAS_STATUS
#endif

// The HeavyIon class is available
#ifndef HEPMC_HAS_HEAVY_ION
#define HEPMC_HAS_HEAVY_ION
#endif

// The HepMC::HeavyIon class contains centrality
#ifndef HEPMC_HEAVY_ION_HAS_CENTRALITY
#define HEPMC_HEAVY_ION_HAS_CENTRALITY
#endif

#endif

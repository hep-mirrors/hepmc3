// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2015 The HepMC collaboration (see AUTHORS for details)
#ifndef HEPMC_COMMON_H
#define HEPMC_COMMON_H

/// @todo Rename to e.g. HepMC/Build/Utils.h?

#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <iostream>


/// Neater/extensible C++11 availability test
#if __cplusplus >= 201103L
#define HEPMC_HAS_CXX11
#endif


/// Define a FOREACH directive
#ifdef  HEPMC_HAS_CXX11
#define FOREACH( iterator, container ) for ( iterator: container )
#else
#include <boost/foreach.hpp>
#define FOREACH( iterator, container ) BOOST_FOREACH( iterator, container )
#endif


/// Deprecation macro
#ifndef HEPMC_DEPRECATED
#if __GNUC__ && __cplusplus && HEPMC_NO_DEPRECATION_WARNINGS == 0
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION >= 40500
  #define HEPMC_DEPRECATED(x) __attribute__((deprecated(x)))
#else
  #define HEPMC_DEPRECATED(x) __attribute__((deprecated))
#endif
#else
  #define HEPMC_DEPRECATED(x)
#endif
#endif


namespace HepMC {
  // ...
} // namespace HepMC

#endif

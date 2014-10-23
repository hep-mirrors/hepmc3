// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_FOREACH_H
#define HEPMC_FOREACH_H


#ifdef BUILD_WITH_11

      #define FOREACH( iterator, container ) for( iterator: container )
 
#else

      #include <boost/foreach.hpp>

      #define FOREACH( iterator, container ) BOOST_FOREACH( iterator, container ) 

#endif


#endif

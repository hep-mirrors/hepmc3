// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_READER_H
#define HEPMC_READER_H
///
/// @file  Reader.h
/// @brief Definition of interface \b Reader
///
/// @class HepMC::Reader
/// @brief Base class for all I/O readers
///
/// @ingroup IO
///

namespace HepMC {

  // Forward declaration
  class GenEvent;

  class Reader {
  public:

    /// Virtual destructor
    virtual ~Reader() {}

    /// Fill next event from input into @a evt
    virtual bool read_event(GenEvent& evt) = 0;

  };


} // namespace HepMC

#endif

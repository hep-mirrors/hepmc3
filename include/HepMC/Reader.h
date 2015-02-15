// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_READER_H
#define HEPMC_READER_H

namespace HepMC {


  // Forward declaration
  class GenEvent;


  /// @brief Base class for all I/O interfaces
  /// @ingroup IO
  class Reader {
  public:

    /// Virtual destructor
    virtual ~Reader() {}

    /// Fill next event from input into @a evt
    virtual bool read_event(GenEvent& evt) = 0;

  };


} // namespace HepMC

#endif

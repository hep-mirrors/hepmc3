// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC_WRITER_H
#define HEPMC_WRITER_H

namespace HepMC {


  // Forward declaration
  class GenEvent;


  /// @brief Base class for all I/O interfaces
  /// @ingroup IO
  class Writer {
  public:

    /// Virtual destructor
    virtual ~Writer() {}

    /// Write event @a evt to output target
    virtual void write_event(const GenEvent &evt) = 0;

  };


} // namespace HepMC

#endif

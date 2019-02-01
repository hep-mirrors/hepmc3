// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READER_H
#define HEPMC3_READER_H
///
/// @file  Reader.h
/// @brief Definition of interface \b Reader
///
/// @class HepMC::Reader
/// @brief Base class for all I/O readers
///
/// @ingroup IO
///

#include "HepMC3/GenRunInfo.h"

namespace HepMC3 {

  // Forward declaration
  class GenEvent;

  class Reader {
  public:
    
    /// Virtual destructor
    virtual ~Reader() {}

    virtual void initialize(const string &filename) = 0;
    
    /// Fill next event from input into @a evt
    virtual bool read_event(GenEvent& evt) = 0;
    virtual bool failed()=0;
    virtual void close()=0;

    /// Return a list of strings to search for to match the file the reader can read
    /// The idea is a factory can check line-by-line for each of these entries
    virtual vector<string> fileSignatures() const {return vector<string>();}
    
    /// In the case of more complicated file signatures, users can implement this
    /// to fall back on to id a file that the reader is capable of reading.
    /// e.g. eventually a gzip reader might implement this
    /// By default it returns false, so is not used
    virtual bool matchesFile(const string &filename) const {return false;}
    
    /// In the case of more complicated file signatures, users can implement this
    /// to fall back on to id a file that the reader is capable of reading.
    /// e.g. eventually a gzip reader might implement this
    /// By default it returns false, so is not used
    virtual bool matchesFile(std::istream & stream) const {return false;}
    
    /// Get the global GenRunInfo object.
    shared_ptr<GenRunInfo> run_info() const {
      return m_run_info;
    }

  protected:

    /// Set the global GenRunInfo object.
    void set_run_info(shared_ptr<GenRunInfo> run) {
      m_run_info = run;
    }

  private:

    /// The global GenRunInfo object.
    shared_ptr<GenRunInfo> m_run_info;

  };


} // namespace HepMC3

#endif

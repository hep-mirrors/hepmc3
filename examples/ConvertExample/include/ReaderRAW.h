// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERRAW_H
#define HEPMC3_READERRAW_H
///
/// @file  ReaderRAW.h
/// @brief Definition of class \b ReaderRAW
///
/// @class HepMC3::ReaderRAW
/// @brief GenEvent I/O parsing for structured text files
///
/// @ingroup IO
///
#include <set>
#include <string>
#include <fstream>
#include <istream>
#include <iterator>
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"


namespace HepMC3 {


class ReaderRAW : public Reader {
public:

    /// @brief Constructor
    ReaderRAW(const std::string& filename);
    /// The ctor to read from stdin
    ReaderRAW(std::istream &);
    /// @brief Destructor
    ~ReaderRAW();

    /// @brief skip events
    bool skip(const int)  override;

    /// @brief Load event from file
    ///
    /// @param[out] evt Event to be filled
    bool read_event(GenEvent& evt)  override;

    /// @todo No-arg version returning GenEvent?

    /// @brief Return status of the stream
    bool failed()  override;

    /// @todo Implicit cast to bool = !failed()?

    /// @brief Close file stream
    void close()  override;


    //@}


private:

    std::ifstream m_file; //!< Input file
    std::istream* m_stream; ///< For ctor when reading from stdin
    bool m_isstream; ///< toggles usage of m_file or m_stream

};


} // namespace HepMC3

#endif

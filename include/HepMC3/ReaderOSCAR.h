// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READEROSCAR_H
#define HEPMC3_READEROSCAR_H
///
/// @file  ReaderOSCAR.h
/// @brief Definition of class \b ReaderOSCAR
///
/// @class HepMC3::ReaderOSCAR
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


class ReaderOSCAR : public Reader {
public:

    /// @brief Constructor
    ReaderOSCAR(const std::string& filename);
    /// The ctor to read from stdin
    ReaderOSCAR(std::istream &);
    /// @brief Destructor
    ~ReaderOSCAR();

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


    void parse_header();
    void end_event();

private:

    std::ifstream m_file; //!< Input file
    std::istream* m_stream; ///< For ctor when reading from stdin
    bool m_isstream; ///< toggles usage of m_file or m_stream
    std::vector<std::string> m_header; ///< header lines 

};


} // namespace HepMC3

#endif

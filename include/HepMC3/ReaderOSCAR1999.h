// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READEROSCAR1999_H
#define HEPMC3_READEROSCAR1999_H
///
/// @file  ReaderOSCAR1999.h
/// @brief Definition of class \b ReaderOSCAR1999
///
/// @class HepMC3::ReaderOSCAR1999
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


class ReaderOSCAR1999 : public Reader {
public:

    /// @brief Constructor
    ReaderOSCAR1999(const std::string& filename);
    /// The ctor to read from stdin
    ReaderOSCAR1999(std::istream &);
    /// @brief Destructor
    ~ReaderOSCAR1999();

    /// @brief skip events
    bool skip(const int)  override;

    /// @brief Load event from file
    ///
    /// @param[out] evt Event to be filled
    bool read_event(GenEvent& evt)  override;

    /// @brief Return status of the stream
    bool failed()  override;

    /// @brief Close file stream
    void close()  override;

private:
    int m_OSCARType; ///< the type of input: 1=particles only, 2=full event
    bool parse_header(); ///< code to parse header
    std::ifstream m_file; //!< Input file
    std::istream* m_stream; ///< For ctor when reading from stdin
    bool m_isstream; ///< toggles usage of m_file or m_stream
    std::vector<std::string> m_header; ///< header lines
    std::vector<GenVertexPtr> m_vertices; ///< pool of vertices
    std::map<int, std::vector<std::pair<GenParticlePtr,FourVector> > > m_prod; ///< association between the particle id, particle object and production vertex
};


} // namespace HepMC3

#endif

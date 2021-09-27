// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2021 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READEROSCAR2013_H
#define HEPMC3_READEROSCAR2013_H
///
/// @file  ReaderOSCAR.h
/// @brief Definition of class \b ReaderOSCAR2013
///
/// @class HepMC3::ReaderOSCAR2013
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


class ReaderOSCAR2013 : public Reader {
public:

    /// @brief Constructor
    ReaderOSCAR2013(const std::string& filename);
    /// The ctor to read from stdin
    ReaderOSCAR2013(std::istream &);
    /// @brief Destructor
    ~ReaderOSCAR2013();

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
    std::ifstream m_file; //!< Input file
    std::istream* m_stream; ///< For ctor when reading from stdin
    bool m_isstream; ///< toggles usage of m_file or m_stream
    std::vector<std::string> m_header; ///< header lines
    std::vector<GenVertexPtr> m_vertices; ///< pool of vertices
    std::map<int, std::vector<std::pair<GenParticlePtr,FourVector> > > m_prod; ///< association between the particle id, particle object and production vertex
};


} // namespace HepMC3

#endif

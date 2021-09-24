// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2020 The HepMC collaboration (see AUTHORS for details)
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

    /// @todo No-arg version returning GenEvent?

    /// @brief Return status of the stream
    bool failed()  override;

    /// @todo Implicit cast to bool = !failed()?

    /// @brief Close file stream
    void close()  override;

private:
    int m_OSCARType;// { unknown,   final_id_p_x, full_event_history };
    bool parse_header();

    std::ifstream m_file; //!< Input file
    std::istream* m_stream; ///< For ctor when reading from stdin
    bool m_isstream; ///< toggles usage of m_file or m_stream
    std::vector<std::string> m_header; ///< header lines
    std::vector<GenVertexPtr> m_vertices;
    std::map<int, std::vector<std::pair<GenParticlePtr,FourVector> > > m_prod;
};


} // namespace HepMC3

#endif

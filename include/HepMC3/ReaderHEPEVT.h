// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERHEPEVT_H
#define HEPMC3_READERHEPEVT_H
/**
 *  @file  ReaderHEPEVT.h
 *  @brief Definition of \b class ReaderHEPEVT
 *
 *  @class HepMC3::ReaderHEPEVT
 *  @brief GenEvent I/O parsing and serialization for HEPEVT files
 *
 *
 *  @ingroup IO
 *
 */
#include <set>
#include <string>
#include <fstream>
#include <istream>
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenRunInfo.h"
#include "HepMC3/Data/GenEventData.h"


namespace HepMC3
{

class ReaderHEPEVT : public Reader
{
//
// Constructors
//
public:
    /** @brief Default constructor */
    ReaderHEPEVT(const std::string &filename);

    /// The ctor to read from stdin
    ReaderHEPEVT(std::istream &);
//
// Functions
//
public:
    /** @brief Find and read event header line  from file
    *
    */
    virtual bool read_hepevt_event_header();
    /** @brief read particle from file
    *
    * @param[in] i Particle id
    * @param[in] iflong Event style
    */
    virtual bool read_hepevt_particle(int i, bool iflong=true);


    /** @brief Read event from file
     *
     *  @param[in] iflong Event style
     *  @param[out] evt Contains parsed even
     */
    bool read_event(GenEvent &evt, bool iflong);

    /** @brief Read event from file
     *
     *  @param[out] evt Contains parsed even
     */
    bool read_event(GenEvent &evt);


    /** @brief Close file stream */
    void close();

    /** @brief Get stream error state */
    bool failed();

    /** @brief  set flag if vertex positions are available */
    void set_vertices_positions_present(bool iflong);
    /** @brief  get flag if vertex positions are available */
    bool get_vertices_positions_present() const;

public:
    char* hepevtbuffer; //!< Pointer to HEPEVT Fortran common block/C struct
private:
    std::ifstream m_file; //!< Input file
    std::istream* m_stream; //!< For ctor when reading from stdin
    bool m_isstream; //!< toggles usage of m_file or m_stream
    bool m_vertices_positions_present; //!< true if vertex positions are available
};

} // namespace HepMC3

#endif

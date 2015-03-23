// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef  HEPMC_IO_ROOT_H
#define  HEPMC_IO_ROOT_H
/**
 *  @file  IO_Root.h
 *  @brief Definition of \b class IO_Root
 *
 *  @class HepMC::IO_Root
 *  @brief GenEvent I/O parsing and serialization for root files
 *
 *  If HepMC was compiled with path to ROOT available, this class can be used
 *  for root file I/O in the same manner as with HepMC::IO_GenEvent class.
 *
 *  @ingroup IO
 *
 */
#include "HepMC/IO/IO_FileBase.h"
#include "HepMC/GenEvent.h"
#include "HepMC/Data/GenEventData.h"

// ROOT header files
#include "TFile.h"
#include "TKey.h"

#include <fstream>

namespace HepMC {

class IO_Root : public IO_Base {
//
// Constructors
//
public:
    /** @brief Default constructor
     *  @warning If file opened in write mode exists, it will be deleted
     */
    IO_Root(const std::string &filename, std::ios::openmode mode);

    /** @brief Default destructor */
    ~IO_Root();

//
// Functions
//
public:

    /** @brief Write event to file
     *
     *  @param[in] evt Event to be serialized
     */
    void write_event(const GenEvent &evt);

    /** @brief Load event from file
     *
     *  @param[out] evt Contains parsed event
     */
    bool fill_next_event(GenEvent &evt);


  // GenEventData* data() {return m_data;};

    /** @brief Close file stream */
    void close();

    /** @brief Get stream error state flags */
    std::ios::iostate rdstate();
//
// Fields
//
private:
    shared_ptr<TFile>  m_file; //!< File handler
    shared_ptr<TIter>  m_next; //!< Iterator for event reading
    std::ios::openmode m_mode; //!< I/O stream mode
};

} // namespace HepMC

#endif
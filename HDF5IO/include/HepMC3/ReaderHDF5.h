// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERHDF5_H
#define HEPMC3_READERHDF5_H
/**
 *  @file  ReaderHDF5.h
 *  @brief Definition of \b class ReaderHDF5
 *
 *  @class HepMC3::ReaderHDF5
 *  @brief GenEvent I/O parsing and serialization for root files
 *
 *  If HepMC was compiled with path to ROOT available, this class can be used
 *  for root file I/O in the same manner as with HepMC::ReaderAscii class.
 *
 *  @ingroup IO
 *
 */
#include <string>
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenRunInfoData.h"

namespace HighFive {
class File;
}

namespace HepMC3 {

class ReaderHDF5 : public Reader {
//
// Constructors
//
public:
    /** @brief Default constructor */
    ReaderHDF5(const std::string &filename);

//
// Functions
//
public:
    /// @brief skip events
    bool skip(const int)  override;

    /** @brief Read event from file
     *
     *  @param[out] evt Contains parsed event
     */
    bool read_event(GenEvent &evt)  override;

    /** @brief Close file stream */
    void close()  override;

    /** @brief Get stream error state */
    bool failed()  override;
//
// Fields
//
private:
    HighFive::File* m_file; 
    int     m_events_count;;

};

} // namespace HepMC3

#endif

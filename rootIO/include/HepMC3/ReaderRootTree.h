// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERROOTTREE_H
#define HEPMC3_READERROOTTREE_H
/**
 *  @file  ReaderRootTree.h
 *  @brief Definition of \b class ReaderRootTree
 *
 *  @class HepMC3::ReaderRootTree
 *  @brief GenEvent I/O parsing and serialization for root files  based on root TTree
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

// ROOT header files
#include "Rtypes.h" // not strictly necessary given the other includes, but Long64_t should be def'd here
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

namespace HepMC3
{

class ReaderRootTree : public Reader
{
//
// Constructors
//
public:
    /** @brief Default constructor */
    ReaderRootTree(const std::string &filename);
    /** @brief Constructor with tree name*/
    ReaderRootTree(const std::string &filename, const std::string &treename, const std::string &branchname);

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
    bool read_event(GenEvent &evt)   override;

    bool read_event_at_index(GenEvent &evt, const Long64_t index);

    /** @brief Close file */
    void close()  override;

    /** @brief Get file  error state */
    bool failed()  override;

private:
    /** @brief init routine */
    bool init();
//
// Fields
//
private:
    TFile* m_file = nullptr;         //!< File handler
public:
    TTree* m_tree = nullptr;//!< Tree handler. Public to allow simple access, e.g. custom branches.
private:
    int   m_events_count = 0; //!< Events count. Needed to read the tree
    GenEventData* m_event_data = nullptr; //!< Pointer to structure that holds event data
    GenRunInfoData* m_run_info_data = nullptr; //!< Pointer to structure that holds run info data
    std::string m_tree_name; //!< Name of TTree
    std::string m_branch_name; //!< Name of TBranch in TTree
};

} // namespace HepMC3

#endif

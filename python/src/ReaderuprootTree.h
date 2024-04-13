// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef READERUPROOTTREE_H
#define READERUPROOTTREE_H
#include "HepMC3/GenEvent.h"
#include "HepMC3/FourVector.h"
#include "HepMC3/Print.h"
#include "HepMC3/Reader.h"
#include "HepMC3/Data/GenEventData.h"
#include "HepMC3/Data/GenRunInfoData.h"
#include <iostream>
#include <array>
#include "HepMC3/Units.h"
#include "HepMC3/Version.h"
#define PYBIND11_DETAILED_ERROR_MESSAGES
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace HepMC3
{
/** @brief  ReaderuprootTree */
class ReaderuprootTree : public Reader
{
public:
    /** @brief Constructor with tree and branch names*/
    ReaderuprootTree(const std::string &filename,const std::string &treename="hepmc3_tree",const std::string &branchname="hepmc3_event");

    /// @brief skip events
    bool skip(const int)  override;

    /** @brief Read event from file
     *
     *  @param[out] evt Contains parsed event
     */
    bool read_event(GenEvent &evt)   override;

    /** @brief Close file */
    void close()  override;

    /** @brief Get file  error state */
    bool failed()  override;

    ~ReaderuprootTree();
private:
    /** @brief init routine */
    bool init(const std::string &filename);

    int m_events_count = 0; //!< Events count. Needed to read the tree
    GenEventData* m_event_data = nullptr; //!< Pointer to structure that holds event data
    GenRunInfoData* m_run_info_data = nullptr; //!< Pointer to structure that holds run info data
    std::string m_tree_name; //!< Name of TTree
    std::string m_branch_name; //!< Name of TBranch in TTree
    pybind11::object m_file;                       //!< Python file handler
    pybind11::object m_tree;                       //!< Python tree handler.
    pybind11::object m_genruninfo;                 //!< Python runInfo handler.
    pybind11::object m_uproot_module;              //!< Python module
    pybind11::object m_numpy_module;              //!< Python module
    long int m_tree_getEntries = 0;             //!< number of processed events
    template <class T> std::vector<T> get_vector(pybind11::object& tr,const std::string& array_name); //!< Get arrays
};

}
#endif

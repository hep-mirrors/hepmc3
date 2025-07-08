// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
/**
 *  @file WriterRootTree.cc
 *  @brief Implementation of \b class WriterRootTree
 *
 */
#include <cstdio>  // sprintf
#include <vector>
#include <utility> // pair
#include <cstring> // strcmp

#include "HepMC3/WriterRootTree.h"
#include "HepMC3/Version.h"
// ROOT header files
#include "TFile.h"
#include "TTree.h"
#include "TObject.h"

namespace HepMC3
{
HEPMC3_DECLARE_WRITER_FILE(WriterRootTree)

WriterRootTree::WriterRootTree(const std::string &filename, std::shared_ptr<GenRunInfo> run):
    m_tree_name("hepmc3_tree"),
    m_branch_name("hepmc3_event")
{
    m_file = TFile::Open(filename.c_str(), "RECREATE");
    if (!init(run)) return;
}

WriterRootTree::WriterRootTree(const std::string &filename, std::shared_ptr<GenRunInfo> run, bool append):
    m_tree_name("hepmc3_tree"),
    m_branch_name("hepmc3_event")
{
    std::string mode = append ? "UPDATE" : "RECREATE";
    m_file = TFile::Open(filename.c_str(), mode.c_str());
    if (!init(run)) return;
}

WriterRootTree::WriterRootTree(const std::string &filename, const std::string &treename, const std::string &branchname, std::shared_ptr<GenRunInfo> run):
    m_tree_name(treename),
    m_branch_name(branchname)
{
    m_file = TFile::Open(filename.c_str(), "RECREATE");
    if (!init(run)) return;
}

WriterRootTree::WriterRootTree(const std::string &filename, const std::string &treename, const std::string &branchname, std::shared_ptr<GenRunInfo> run, bool append):
    m_tree_name(treename),
    m_branch_name(branchname)
{
    std::string mode = append ? "UPDATE" : "RECREATE";
    m_file = TFile::Open(filename.c_str(), mode.c_str());
    if (!init(run)) return;
}

bool WriterRootTree::init(std::shared_ptr<GenRunInfo> run )
{
    if ( !m_file->IsOpen() )
    {
        HEPMC3_ERROR_LEVEL(100,"WriterRootTree: problem opening file: " << m_file->GetName())
        return false;
    }
    m_event_data = new GenEventData();
    m_run_info_data = new GenRunInfoData();
    set_run_info(run);
    if ( run_info() ) run_info()->write_data(*m_run_info_data);

    m_tree = dynamic_cast<TTree*>(m_file->Get(m_tree_name.c_str()));

    if(m_tree)
    { // tree exists -> check if the branches exist. If so, just need to set branch addresses
        // check the branches
        std::vector<std::pair<std::string, TClass*>> required_branches =
        {
                {m_branch_name, TClass::GetClass(typeid(  GenEventData))},
                {"GenRunInfo",  TClass::GetClass(typeid(GenRunInfoData))}
        };

        for(const std::pair<std::string, TClass*> branch_info : required_branches)
        {
            TBranch* branch = m_tree->GetBranch(branch_info.first.c_str());

            if(!branch) // existence check
            {
                HEPMC3_ERROR_LEVEL(100, "WriterRootTree: existing tree '" << m_tree_name
                                << "' missing required branch '" << branch_info.first << "'")
                return false;
            }

            if (std::strcmp(branch->GetClassName(),branch_info.second->GetName()) != 0) // check that it's of the right type
            {
                HEPMC3_ERROR_LEVEL(100, "WriterRootTree: branch '" << branch_info.first
                                << "' has incompatible type: " << branch->GetClassName()
                                << " (expected " << branch_info.second->GetName() << ")")
                return false;
            }
        }

        m_tree->SetBranchAddress(m_branch_name.c_str(), &m_event_data);
        m_tree->SetBranchAddress("GenRunInfo", &m_run_info_data);
    }
    else
    { // the tree doesn't exist -> we need to make it

        // check that there isn't *something* else in the file named m_tree_name
        TObject* existing_obj = m_file->Get(m_tree_name.c_str());
        if(existing_obj)
        {
            HEPMC3_ERROR_LEVEL(100, "WriterRootTree: object named '" << m_tree_name
                              << "' already exists in file but is not a TTree (type: "
                              << existing_obj->ClassName() << ")")
            return false;
        }

        m_tree = new TTree(m_tree_name.c_str(), "hepmc3_tree");
        m_tree->Branch(m_branch_name.c_str(), m_event_data);
        m_tree->Branch("GenRunInfo", m_run_info_data);
    }
    return true;
}

void WriterRootTree::write_event(const GenEvent &evt)
{
    if ( !m_file->IsOpen() ) return;
    bool refill = false;
    if ( evt.run_info()&&(!run_info() || (run_info() != evt.run_info())))  { set_run_info(evt.run_info()); refill = true;}
    if (refill)
    {
        m_run_info_data->weight_names.clear();
        m_run_info_data->tool_name.clear();
        m_run_info_data->tool_version.clear();
        m_run_info_data->tool_description.clear();
        m_run_info_data->attribute_name.clear();
        m_run_info_data->attribute_string.clear();
        run_info()->write_data(*m_run_info_data);
    }

    m_event_data->particles.clear();
    m_event_data->vertices.clear();
    m_event_data->links1.clear();
    m_event_data->links2.clear();
    m_event_data->attribute_id.clear();
    m_event_data->attribute_name.clear();
    m_event_data->attribute_string.clear();

    evt.write_data(*m_event_data);
    m_tree->Fill();
    ++m_events_count;
}

void WriterRootTree::write_run_info() {}

void WriterRootTree::close()
{
    m_file->WriteTObject(m_tree);
    m_file->Close();
    delete m_event_data;
    delete m_run_info_data;
}

bool WriterRootTree::failed()
{
    return !m_file->IsOpen();
}

} // namespace HepMC3

// -*- C++ -*-
//
/**
 *  @file WriterRootTree.cc
 *  @brief Implementation of \b class WriterRootTree
 *
 */
#include "HepMC/WriterRootTree.h"
#include <cstdio>  // sprintf

namespace HepMC
{

WriterRootTree::WriterRootTree(const std::string &filename):
    m_file(filename.c_str(),"RECREATE"),
    m_tree(0),    
    m_events_count(0),
    m_tree_name("hepmc3_tree"),
    m_branch_name("hepmc3_event")
{
    if (!init()) return;
}

WriterRootTree::WriterRootTree(const std::string &filename,const std::string &treename,const std::string &branchname):
    m_file(filename.c_str(),"RECREATE"),
    m_tree(0),    
    m_events_count(0),
    m_tree_name(treename.c_str()),
    m_branch_name(branchname.c_str())
{
    if (!init()) return;
}

bool WriterRootTree::init()
{
    if ( !m_file.IsOpen() )
        {
            ERROR( "WriterRootTree: problem opening file: " <<m_file.GetName() )
            return false;
        }
    m_event_data= new GenEventData();
    m_tree= new TTree("hepmc3_tree",m_tree_name.c_str());
    m_tree->Branch(m_branch_name.c_str(), m_event_data);
    return true;
}

void WriterRootTree::write_event(const GenEvent &evt)
{
    if ( !m_file.IsOpen() ) return;
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

void WriterRootTree::close()
{

    m_tree->Write();
    m_file.Close();
}

bool WriterRootTree::failed()
{
    if ( !m_file.IsOpen() ) return true;

    return false;
}

} // namespace HepMC

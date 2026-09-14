// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#include "WriterRootTreeOPAL.h"
#include "TTree.h"
namespace HepMC3
{
WriterRootTreeOPAL::WriterRootTreeOPAL(const std::string &filename,std::shared_ptr<GenRunInfo> run):WriterRootTree::WriterRootTree(filename,"h10","h10",run) {}
void WriterRootTreeOPAL::init_branches()
{
    m_tree->Branch("Irun", &m_Irun);
    m_tree->Branch("Ievnt", &m_Ievnt);
    m_tree->Branch("Ebeam",&m_Ebeam);
}
void WriterRootTreeOPAL::write_event(const GenEvent &evt)
{
    m_Ievnt=evt.event_number();
    std::vector<HepMC3::ConstGenParticlePtr> beams;
    for (const auto& particle : evt.particles()) {
        if (particle->status() == 4 && std::abs(particle->pid()) == 11)
        { beams.push_back(particle);}
    }
    if (beams.size() == 2) {
        m_Ebeam = std::abs(beams[0]->momentum().e());
    } else {
        m_Ebeam = std::abs(evt.particles().front()->momentum().e());
    }
    WriterRootTree::write_event(evt);
}
void WriterRootTreeOPAL::set_run_number(const int nr) {m_Irun=nr;}
} // namespace HepMC3

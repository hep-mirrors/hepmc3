// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#include "PythiaValidationTool.h"

PythiaValidationTool::PythiaValidationTool( const std::string &filename ):m_filename(filename),m_timer("pythia8 conversion time") {
    m_pythia.readFile(m_filename);
}

void PythiaValidationTool::initialize() {
    m_pythia.init();
}

int PythiaValidationTool::process(GenEvent &hepmc) {
    if( !m_pythia.next() ) return 1;

    // Exclude generation time
    m_timer.start();

    m_tohepmc.fill_next_event(m_pythia.event, &hepmc, -1, &m_pythia.info);

    return 0;
}

void PythiaValidationTool::finalize() {
#ifdef  PYTHIA_VERSION_INTEGER
    if  (PYTHIA_VERSION_INTEGER>8200)  m_pythia.stat();
#else
    m_pythia.statistics();
#endif

}

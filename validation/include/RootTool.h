// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014 The HepMC collaboration (see AUTHORS for details)
//
#ifndef ROOT_TOOL_H
#define ROOT_TOOL_H

#ifdef HEPMC2
#include "HepMC/GenEvent.h"
#else
#include "HepMC/ReaderRoot.h"
#include "HepMC/WriterRoot.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"
#include "Timer.h"

class RootTool : public ValidationTool {
public:
    RootTool(const std::string &filename, std::ios::openmode mode);

    ~RootTool();

public:
    const std::string name() {
        if(m_file_in) return "ROOT input file";
        else          return "ROOT output file";
    }

    const std::string long_name() { return name() + ": " + m_filename; }

    bool   tool_modifies_event() { return (m_file_in) ? true : false; }
    Timer* timer()               { return &m_timer; }

    void initialize();
    int  process(GenEvent &hepmc);
    void finalize();

    bool failed();

private:
    HEPMC3CODE( ReaderRoot *m_file_in;  )
    HEPMC3CODE( WriterRoot *m_file_out; )
    std::string  m_filename;
    Timer        m_timer;
};

#endif

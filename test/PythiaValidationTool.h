// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2023 The HepMC collaboration (see AUTHORS for details)
//
#ifndef PYTHIA_VALIDATION_TOOL_H
#define PYTHIA_VALIDATION_TOOL_H

#ifdef HEPMC2
#include "HepMC/GenEvent.h"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#else
#include "HepMC3/GenEvent.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "Pythia8Plugins/HepMC3.h"
#endif

#include "ValidationTool.h"
#include "Timer.h"

#include "Pythia8/Pythia.h"
/// @class PythiaValidationTool
/// @brief Interface for validatio to Pythia
class PythiaValidationTool : public ValidationTool {
public:
    PythiaValidationTool( const std::string &filename ); ///< Constructor

    const std::string name()      { return "pythia8"; }
    const std::string long_name() { return name() + " config file: " + m_filename; }

    bool   tool_modifies_event() { return true;      }
    Timer* timer()               { return &m_timer;  }

    void initialize();
    int  process(GenEvent &hepmc);
    void finalize();

private:
    Pythia8::Pythia m_pythia; ///< Pythia8 instance
    std::string     m_filename; ///< Used file
    Timer           m_timer;  ///< Timer
    HEPMC2CODE( Pythia8ToHepMC   m_tohepmc; )
    HEPMC3CODE( Pythia8ToHepMC3 m_tohepmc; )
};

#endif

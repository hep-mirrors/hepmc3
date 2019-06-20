// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef PHOTOS_VALIDATION_TOOL_H
#define PHOTOS_VALIDATION_TOOL_H

#ifdef HEPMC2
#include "Photos/PhotosHepMCEvent.h"
#include "HepMC/GenEvent.h"
#else
#include "Photos/PhotosHepMC3Event.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Selector.h"
#endif // ifdef HEPMC2

#include "ValidationTool.h"
#include "Timer.h"

#include "Photos/Photos.h"
#include "Photos/Log.h"

class PhotosValidationTool : public ValidationTool {
public:
    PhotosValidationTool();

public:
    const  std::string name()    { return "Photos++"; }
    bool   tool_modifies_event() { return true;       }
    Timer* timer()               { return &m_timer;   }

    void initialize();
    int  process(GenEvent &hepmc);
    void finalize();

private:
    static const int MAX_PHOTONS_TO_KEEP_TRACK_OF = 4;
    int    m_photons_added[MAX_PHOTONS_TO_KEEP_TRACK_OF];
    int    m_more_photons_added;
    Timer  m_timer;
};

#endif

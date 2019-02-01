// -*- C++ -*-
// -*- C++ -*-
//
// This file is part of HepMC
// Copyright (C) 2014-2019 The HepMC collaboration (see AUTHORS for details)
//
#ifndef HEPMC3_READERLHEF_H
#define HEPMC3_READERLHEF_H
/**
 *  @file  ReaderLHEF.h
 *  @brief Definition of \b class ReaderLHEF
 *
 *  @class HepMC3::ReaderLHEF
 *  @brief GenEvent I/O parsing and serialization for LHEF files
 *
 *
 *  @ingroup IO
 *
 */
#include "HepMC3/Reader.h"
#include "HepMC3/GenEvent.h"
#include <string>
#include <fstream>
#include <istream>
#include "HepMC3/LHEFAttributes.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"
#include "HepMC3/GenVertex.h"
#include <iomanip>


namespace HepMC3 
{
class ReaderLHEF : public Reader
{
public:
    ReaderLHEF(const std::string& filename);
    bool read_event(GenEvent& ev);
    void close();
    bool failed();
    ~ReaderLHEF() ;
private:
    LHEF::Reader* m_reader;
    shared_ptr<HEPRUPAttribute> m_hepr;
    int m_neve;
    bool m_failed;
};
}
#endif